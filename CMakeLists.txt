cmake_minimum_required(VERSION 3.16)
project(onesimus VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# Version Information Generation (version.h)
# ============================================================================
# Version scheme: MAJOR.MINOR.PATCH.BUILD
#   - Triplet (MAJOR.MINOR.PATCH) is set in project() above
#   - BUILD number auto-increments on every build via custom target
#   - version.h is regenerated at build time (not configure time)
# ============================================================================

message(STATUS "")
message(STATUS "========================================")
message(STATUS "Version: ${PROJECT_VERSION} (build number increments at build time)")
message(STATUS "========================================")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# UI-Verzeichnis für AUTOUIC angeben
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(IS_DEVELOPER)
endif()

# Option für detailliertes Packet Debugging (getrennt von IS_DEVELOPER)
option(DEBUG_PACKETS "Enable detailed packet-level debugging output" OFF)
if(DEBUG_PACKETS)
    add_compile_definitions(DEBUG_PACKETS)
    message(STATUS "Packet debugging: ENABLED")
else()
    message(STATUS "Packet debugging: DISABLED (use -DDEBUG_PACKETS=ON to enable)")
endif()

# Option für detailliertes JSON Debugging (getrennt von IS_DEVELOPER)
option(DEBUG_JSON "Enable detailed JSON message debugging output" OFF)
if(DEBUG_JSON)
    add_compile_definitions(DEBUG_JSON)
    message(STATUS "JSON debugging: ENABLED")
else()
    message(STATUS "JSON debugging: DISABLED (use -DDEBUG_JSON=ON to enable)")
endif()

# Option für JSON Logging zu Datei (getrennt von IS_DEVELOPER)
option(LOG_JSON "Enable JSON logging to file" OFF)
if(LOG_JSON)
    add_compile_definitions(LOG_JSON)
    message(STATUS "JSON logging: ENABLED")
else()
    message(STATUS "JSON logging: DISABLED (use -DLOG_JSON=ON to enable)")
endif()

# Option für Onesimus File Logging (writes to AppDataLocation/onesimus.log)
option(ONESIMUS_FILE_LOGGING "Enable file logging with BLOG_* macros" OFF)
if(ONESIMUS_FILE_LOGGING)
    add_compile_definitions(ONESIMUS_FILE_LOGGING)
    message(STATUS "File logging: ENABLED (logs to AppData/onesimus.log)")
else()
    message(STATUS "File logging: DISABLED (use -DONESIMUS_FILE_LOGGING=ON to enable)")
endif()

# ============================================================================
# Backup System Auswahl
# ============================================================================
set(BACKUP_SYSTEM "BAREOS" CACHE STRING "Backup system to support: BACULA or BAREOS")
set_property(CACHE BACKUP_SYSTEM PROPERTY STRINGS "BACULA" "BAREOS")

message(STATUS "")
message(STATUS "========================================")
message(STATUS "Backup System Configuration")
message(STATUS "========================================")

# Setze Compiler-Definitionen basierend auf Auswahl
if(BACKUP_SYSTEM STREQUAL "BACULA")
    set(USE_BACULA_ONLY TRUE)
elseif(BACKUP_SYSTEM STREQUAL "BAREOS")
    set(USE_BAREOS_ONLY TRUE)
    set(BAREOS_VERSION_STR "25.0.0")
else()
    message(FATAL_ERROR "Invalid BACKUP_SYSTEM: ${BACKUP_SYSTEM}. Must be BACULA or BAREOS")
endif()

message(STATUS "========================================")
message(STATUS "")

set(USE_STATIC_OPENSSL_DEFAULT ON)
find_package(OpenSSL)
if(TARGET OpenSSL::SSL)
    set(USE_STATIC_OPENSSL_DEFAULT OFF)
endif()

# Option für statisches OpenSSL
option(USE_STATIC_OPENSSL "Use static OpenSSL from git submodule (auto-download)" ${USE_STATIC_OPENSSL_DEFAULT})

# ============================================================================
# OpenSSL Automatischer Download und Build via Git Submodule
# ============================================================================
if(USE_STATIC_OPENSSL)
    message(STATUS "")
    message(STATUS "========================================")
    message(STATUS "Static OpenSSL Configuration")
    message(STATUS "========================================")
    
    set(OPENSSL_SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/external/openssl)
    set(OPENSSL_INSTALL_DIR ${CMAKE_BINARY_DIR}/openssl-install)
    
    # Automatischer Download via Git Submodule
    message(STATUS "Checking OpenSSL submodule...")
    
    if(NOT EXISTS ${OPENSSL_SUBMODULE_DIR}/Configure)
        message(STATUS "OpenSSL not found - initializing git submodule...")
        
        find_package(Git QUIET)
        if(NOT GIT_FOUND)
            message(FATAL_ERROR "Git not found! Required to download OpenSSL submodule.")
        endif()
        
        # Git submodule initialisieren und herunterladen
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --depth 1 external/openssl
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT
            OUTPUT_VARIABLE GIT_SUBMOD_OUTPUT
            ERROR_VARIABLE GIT_SUBMOD_ERROR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE
        )
        
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(WARNING 
                "Git submodule update failed (trying full clone):\n"
                "${GIT_SUBMOD_ERROR}")
            
            # Fallback: Ohne --depth
            execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive external/openssl
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT2
                OUTPUT_QUIET
                ERROR_VARIABLE GIT_SUBMOD_ERROR2
            )
            
            if(NOT GIT_SUBMOD_RESULT2 EQUAL "0")
                message(FATAL_ERROR 
                    "Failed to initialize OpenSSL submodule!\n"
                    "Error: ${GIT_SUBMOD_ERROR2}\n"
                    "\n"
                    "Please run manually:\n"
                    "  cd ${CMAKE_SOURCE_DIR}\n"
                    "  git submodule update --init --recursive\n")
            endif()
        endif()
        
        if(EXISTS ${OPENSSL_SUBMODULE_DIR}/Configure)
            message(STATUS "✓ OpenSSL submodule downloaded successfully (openssl-3.6.0)")
        else()
            message(FATAL_ERROR "OpenSSL submodule download failed!")
        endif()
    else()
        message(STATUS "✓ OpenSSL submodule already present")
    endif()
    
    # OpenSSL Version aus VERSION.dat lesen
    if(EXISTS ${OPENSSL_SUBMODULE_DIR}/VERSION.dat)
        file(READ ${OPENSSL_SUBMODULE_DIR}/VERSION.dat OPENSSL_VERSION_INFO)
        string(REGEX MATCH "MAJOR=([0-9]+)" _ ${OPENSSL_VERSION_INFO})
        set(OPENSSL_MAJOR ${CMAKE_MATCH_1})
        string(REGEX MATCH "MINOR=([0-9]+)" _ ${OPENSSL_VERSION_INFO})
        set(OPENSSL_MINOR ${CMAKE_MATCH_1})
        string(REGEX MATCH "PATCH=([0-9]+)" _ ${OPENSSL_VERSION_INFO})
        set(OPENSSL_PATCH ${CMAKE_MATCH_1})
        
        if(OPENSSL_MAJOR)
            set(OPENSSL_VERSION_STRING "${OPENSSL_MAJOR}.${OPENSSL_MINOR}.${OPENSSL_PATCH}")
            message(STATUS "OpenSSL Version: ${OPENSSL_VERSION_STRING}")
        endif()
    endif()
    
    # Plattform-spezifische Build-Konfiguration
    if(WIN32)
        # Windows mit Visual Studio
        find_program(PERL_EXECUTABLE perl)
        if(NOT PERL_EXECUTABLE)
            message(FATAL_ERROR 
                "Perl not found! Required for OpenSSL Windows build.\n"
                "\n"
                "Install Strawberry Perl:\n"
                "  https://strawberryperl.com/\n"
                "\n"
                "Or disable static OpenSSL:\n"
                "  cmake .. -DUSE_STATIC_OPENSSL=OFF")
        endif()
        
        message(STATUS "Perl: ${PERL_EXECUTABLE}")
        message(STATUS "Build System: NMAKE (Visual Studio)")
        
        set(OPENSSL_CONFIGURE_COMMAND 
            ${PERL_EXECUTABLE} Configure VC-WIN64A
            shared
            no-tests
            no-apps
            no-docs
            --prefix=${OPENSSL_INSTALL_DIR}
            --openssldir=${OPENSSL_INSTALL_DIR}/ssl
        )
        
        set(OPENSSL_BUILD_COMMAND nmake)
        set(OPENSSL_INSTALL_COMMAND nmake install_sw install_ssldirs)
        
        # Windows System-Bibliotheken
        set(OPENSSL_SYSTEM_LIBS ws2_32 crypt32 advapi32 user32)
        
        # Bibliotheksnamen
        set(OPENSSL_SSL_LIB ${OPENSSL_INSTALL_DIR}/lib/libssl.lib)
        set(OPENSSL_CRYPTO_LIB ${OPENSSL_INSTALL_DIR}/lib/libcrypto.lib)
        
    else() #(USE_STATIC_OPENSSL)
        # Linux/Unix
        message(STATUS "Build System: Unix Makefiles")
        
        # CPU-Kerne ermitteln für parallelen Build
        include(ProcessorCount)
        ProcessorCount(N_CORES)
        if(N_CORES EQUAL 0)
            set(N_CORES 1)
        endif()
        message(STATUS "Parallel Build: ${N_CORES} cores")
        
        set(OPENSSL_CONFIGURE_COMMAND 
            ./config
            shared
            no-tests
            no-apps
            no-docs
            --prefix=${OPENSSL_INSTALL_DIR}
            --openssldir=${OPENSSL_INSTALL_DIR}/ssl
        )
        
        set(OPENSSL_BUILD_COMMAND make -j${N_CORES})
        set(OPENSSL_INSTALL_COMMAND make install_sw install_ssldirs)
        
        # Linux System-Bibliotheken
        set(OPENSSL_SYSTEM_LIBS pthread dl)
        
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(OPENSSL_SSL_LIB ${OPENSSL_INSTALL_DIR}/lib64/libssl.a)
            set(OPENSSL_CRYPTO_LIB ${OPENSSL_INSTALL_DIR}/lib64/libcrypto.a)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(OPENSSL_SSL_LIB ${OPENSSL_INSTALL_DIR}/lib/libssl.a)
            set(OPENSSL_CRYPTO_LIB ${OPENSSL_INSTALL_DIR}/lib/libcrypto.a)
        else()
            message(FATAL_ERROR "Unknown architecture")
        endif()        # Bibliotheksnamen
    endif()
    
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_INSTALL_DIR}/include)
    
    # ========================================================================
    # OpenSSL als ExternalProject
    # ========================================================================
    include(ExternalProject)
    
    message(STATUS "Configuring OpenSSL ExternalProject...")
    
    ExternalProject_Add(openssl_build
        SOURCE_DIR ${OPENSSL_SUBMODULE_DIR}
        CONFIGURE_COMMAND ${OPENSSL_CONFIGURE_COMMAND}
        BUILD_COMMAND ${OPENSSL_BUILD_COMMAND}
        INSTALL_COMMAND ${OPENSSL_INSTALL_COMMAND}
        BUILD_IN_SOURCE 1
        BUILD_ALWAYS 0
        LOG_CONFIGURE ON
        LOG_BUILD ON
        LOG_INSTALL ON
        LOG_OUTPUT_ON_FAILURE ON
        BUILD_BYPRODUCTS
            ${OPENSSL_SSL_LIB}
            ${OPENSSL_CRYPTO_LIB}
    )
    
    # ========================================================================
    # OpenSSL Interface Libraries
    # ========================================================================
    
    # Interface Library für Include-Verzeichnisse und Linking
    add_library(OpenSSL_SSL INTERFACE)
    add_library(OpenSSL_Crypto INTERFACE)
    
    # Include-Verzeichnisse setzen
    target_include_directories(OpenSSL_SSL INTERFACE ${OPENSSL_INCLUDE_DIR})
    target_include_directories(OpenSSL_Crypto INTERFACE ${OPENSSL_INCLUDE_DIR})
    
    # Link-Bibliotheken setzen
    target_link_libraries(OpenSSL_SSL INTERFACE
        ${OPENSSL_SSL_LIB}
        OpenSSL_Crypto
    )
    
    target_link_libraries(OpenSSL_Crypto INTERFACE
        ${OPENSSL_CRYPTO_LIB}
        ${OPENSSL_SYSTEM_LIBS}
    )
    
    # Abhängigkeiten definieren: OpenSSL muss vor diesen Targets gebaut sein
    add_dependencies(OpenSSL_SSL openssl_build)
    add_dependencies(OpenSSL_Crypto openssl_build)
    
    # Aliase für Standard-Konvention
    add_library(OpenSSL::SSL ALIAS OpenSSL_SSL)
    add_library(OpenSSL::Crypto ALIAS OpenSSL_Crypto)
    
    message(STATUS "✓ OpenSSL configured as ExternalProject")
    message(STATUS "  Source: ${OPENSSL_SUBMODULE_DIR}")
    message(STATUS "  Install: ${OPENSSL_INSTALL_DIR}")
    message(STATUS "========================================")
    message(STATUS "")
    
else()
    # ========================================================================
    # System OpenSSL verwenden
    # ========================================================================
    if(TARGET OpenSSL::SSL)
        message(STATUS "Using system OpenSSL")
        message(STATUS "✓ Found OpenSSL ${OPENSSL_VERSION}")
        message(STATUS "  Includes: ${OPENSSL_INCLUDE_DIR}")
        message(STATUS "  Libraries: ${OPENSSL_LIBRARIES}")
    else()
        message(FATAL_ERROR "OpenSSL not found. Either install the openssl development package or set -DUSE_STATIC_OPENSSL=ON to compile it locally")
    endif()
endif()

# ============================================================================
# Qt6 Configuration
# ============================================================================

# Finde Qt6 Core zuerst
find_package(Qt6 6.8 REQUIRED COMPONENTS Core)

# Qt6 Standard Project Setup (empfohlen ab Qt 6.3+)
qt_standard_project_setup()

# Finde restliche Qt6 Komponenten
find_package(Qt6 REQUIRED COMPONENTS
    Gui
    Widgets
    Network
    Sql
    LinguistTools
)

# GuiPrivate: Windows has the package, Linux uses private include dirs directly
if(WIN32)
    find_package(Qt6 REQUIRED COMPONENTS GuiPrivate)
endif()

message(STATUS "✓ Found Qt6 ${Qt6_VERSION}")

# ============================================================================
# Qt Linguist Tools Configuration
# ============================================================================

set(TS_FILES
    ${CMAKE_SOURCE_DIR}/translations/onesimus_de.ts
    ${CMAKE_SOURCE_DIR}/translations/onesimus_en.ts
    ${CMAKE_SOURCE_DIR}/translations/onesimus_es.ts
    ${CMAKE_SOURCE_DIR}/translations/onesimus_fr.ts
    ${CMAKE_SOURCE_DIR}/translations/onesimus_it.ts
    ${CMAKE_SOURCE_DIR}/translations/onesimus_ru.ts
)

qt_add_translations(${PROJECT_NAME}
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "/translations"
    LUPDATE_OPTIONS -no-obsolete
)

# ============================================================================
# Project Files
# ============================================================================

# Include-Verzeichnisse
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Quell-Dateien
set(SOURCES
    src/main.cpp
    src/bmainwindow.cpp
    src/director/baculaauth.cpp
    src/director/bareosauth.cpp

    src/bcheckableheaderview.cpp
    src/bcheckboxdelegate.cpp
    src/director/bjsonstreamreader.cpp
    src/btranslations.cpp
    src/director/bdirector.cpp
    src/director/bareosdirector.cpp
    src/db/bdatabase.cpp
    src/db/bresourcemodel.cpp
    src/db/bdirectormodel.cpp
    src/bcleanupdialog.cpp
    src/bconnectionwizard.cpp
    src/bcertificategenerator.cpp
    src/bpfxconverter.cpp
    src/storagewidget.cpp

    src/config/bcolumnconfiguration.cpp
    src/config/bsettings.cpp
    src/config/bsettingsdialog.cpp
    src/config/bprofilesettingsdialog.cpp
    src/config/bconfigexporter.cpp
    src/config/bsettingshistory.cpp

    src/models/bbasemodels.cpp
    src/models/bresourcemodels.cpp

    src/jobs/bjobmodels.cpp
    src/jobs/bjobdetailsdialog.cpp
    src/jobs/bbvfsmodel.cpp
    src/jobs/bjobfileswidget.cpp
    src/jobs/brestorewizard.cpp
    src/jobs/bjoblogdialog.cpp
    src/jobs/bjobsstatisticswidget.cpp
    src/jobs/bjobwidget.cpp
    src/jobs/bjsonjobview.cpp
    src/jobs/bnewjobdialog.cpp
    src/jobs/bfilesetwizard.cpp
    src/jobs/bjobwizard.cpp
    src/jobs/fileset/bfilesetdocument.cpp
    src/jobs/fileset/bincludeblockwidget.cpp

    src/schedules/bschedulewidget.cpp
    src/schedules/bweeklyplanner.cpp
    src/schedules/bscheduleganttwidget.cpp
    src/jobs/bpaginationwidget.cpp
    src/jobs/bviewpresets.cpp

    src/messages/bmessageswidget.cpp

    src/clients/bclientwidget.cpp
    src/clients/bclientsmodel.cpp
    src/clients/bclientswidget.cpp
    src/clients/bclientdetailsdialog.cpp
    src/clients/bnewclientdialog.cpp

    src/config/bconfigparser.cpp
    src/config/bdirectiveschema.cpp
    src/config/bresourceform.cpp
    src/config/bincludeoptionsform.cpp
    src/config/beditablelistmodel.cpp
    src/config/bundocommands.cpp
    src/config/beditablelistdelegate.cpp
    src/config/beditablelistwidget.cpp

    src/director/bresourcewidget.cpp
    src/director/bresourcewidgets.cpp
    src/director/bresourcedialog.cpp
    src/director/bconfigimportdialog.cpp
)

# Header-Dateien
set(HEADERS
    include/bmainwindow.h
    include/bcheckableheaderview.h
    include/clients/bclientwidget.h
    include/storagewidget.h
    include/bcleanupdialog.h
    include/bconnectionwizard.h
    include/bcertificategenerator.h
    include/bpfxconverter.h
    include/director/bdirector.h
    include/director/bareosdirector.h
    include/db/bdatabase.h
    include/db/bresourcemodel.h
    include/db/bdirectormodel.h
    include/version.h
    include/director/baculaauth.h
    include/director/bareosauth.h
    include/director/bjsonstreamreader.h
    include/bpaginationwidget.h
    include/btranslations.h

    include/config/bcolumnconfiguration.h
    include/config/bsettings.h
    include/config/bsettingsdialog.h
    include/config/bprofilesettingsdialog.h
    include/config/bconfigexporter.h
    include/config/bsettingshistory.h

    include/models/bbasemodels.h
    include/models/bresourcemodels.h

    include/jobs/bcheckboxdelegate.h
    include/jobs/bjobmodels.h
    include/jobs/bjobdetailsdialog.h
    include/jobs/bbvfsmodel.h
    include/jobs/bjobfileswidget.h
    include/jobs/bjoblogdialog.h
    include/jobs/brestorewizard.h
    include/jobs/bjobsstatisticswidget.h
    include/jobs/bjobwidget.h
    include/jobs/bjsonjobview.h
    include/jobs/bviewpresets.h
    include/jobs/blevelcolors.h
    include/jobs/bnewjobdialog.h
    include/jobs/bfilesetwizard.h
    include/jobs/bjobwizard.h
    include/jobs/fileset/bfilesetdocument.h
    include/jobs/fileset/bincludeblockwidget.h

    include/clients/bclientsmodel.h
    include/clients/bclientswidget.h
    include/clients/bclientdetailsdialog.h
    include/clients/bnewclientdialog.h

    include/schedules/bschedulewidget.h
    include/schedules/bweeklyplanner.h
    include/schedules/bscheduleganttwidget.h

    include/messages/bmessageswidget.h

    include/config/bconfigparser.h
    include/config/bdirectiveschema.h
    include/config/bresourceform.h
    include/config/bincludeoptionsform.h
    include/config/beditablelistmodel.h
    include/config/bundocommands.h
    include/config/beditablelistdelegate.h
    include/config/beditablelistwidget.h

    include/director/bresourcewidget.h
    include/director/bresourcewidgets.h
    include/director/bresourcedialog.h
    include/director/bconfigimportdialog.h)

# Bacula-spezifische Dateien
if(USE_BACULA_ONLY)
    message(STATUS "========================================")
    message(STATUS "Building with BACULA backend")
    message(STATUS "========================================")
    list(APPEND SOURCES src/director/baculaauth.cpp)
    list(APPEND HEADERS include/director/baculaauth.h)
    add_compile_definitions(USE_BACULA)
endif()

# Bareos-spezifische Dateien
if(USE_BAREOS_ONLY)
    message(STATUS "========================================")
    message(STATUS "Building with BAREOS backend")
    message(STATUS "========================================")
    list(APPEND SOURCES src/director/bareosauth.cpp)
    list(APPEND HEADERS include/director/bareosauth.h)
    add_compile_definitions(USE_BAREOS)
    add_compile_definitions(BAREOS_VERSION)
endif()

# UI-Dateien
set(UI_FILES
    ui/bmainwindow.ui
    ui/jobwidget.ui
    ui/bclientwidget.ui
    ui/storagewidget.ui
    ui/bsettingsdialog.ui
)

# ============================================================================
# Qt Ressourcen automatisch generieren und einbinden
# ============================================================================

set(RESOURCE_PREFIX_ICONS  icons)
set(RESOURCE_PREFIX_IMAGES images)

set(RESOURCE_CONTENT 
    res/img/logo_bareos.png 
    res/img/logo_bacula.png 
    )

# Generierte qrc-Datei im Build-Verzeichnis
set(QT_AUTO_QRC ${CMAKE_BINARY_DIR}/resources_autogen.qrc)

# qrc-Datei schreiben
file(WRITE ${QT_AUTO_QRC} "<!DOCTYPE RCC><RCC version=\"1.0\">\n")
file(APPEND ${QT_AUTO_QRC} "<qresource prefix=\"/${RESOURCE_PREFIX_IMAGES}\">\n")

foreach(res_file IN LISTS RESOURCE_CONTENT)
    # Nur der Dateiname als Alias
    get_filename_component(res_name ${res_file} NAME)
    file(APPEND ${QT_AUTO_QRC} "    <file alias=\"${res_name}\">${CMAKE_SOURCE_DIR}/${res_file}</file>\n")
endforeach()

file(APPEND ${QT_AUTO_QRC} "</qresource>\n</RCC>\n")

# CMake Qt Ressourcen automatisch kompilieren
qt6_add_resources(QT_RESOURCES
    ${QT_AUTO_QRC}
)

# Kompilierte Ressourcen zu den Quellen hinzufügen
list(APPEND SOURCES ${QT_RESOURCES})

# Theme Resources
qt6_add_resources(THEME_RESOURCES
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)
list(APPEND SOURCES ${THEME_RESOURCES})

# ============================================================================
# Executable Target
# ============================================================================

# Windows application icon resource
if(WIN32)
    set(WIN_RC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/onesimus.rc)
    if(EXISTS ${WIN_RC_FILE})
        list(APPEND SOURCES ${WIN_RC_FILE})
    endif()
endif()

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

# Hide console window on Windows (GUI application)
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# ============================================================================
# Build Number Auto-Increment (runs on every build)
# ============================================================================
# Increments .build_number and regenerates version.h before compiling.
# Version scheme: MAJOR.MINOR.PATCH.BUILD (triplet from project() + auto build number)

add_custom_target(increment_build_number
    COMMAND ${CMAKE_COMMAND}
        -DSOURCE_DIR=${CMAKE_SOURCE_DIR}
        -DBINARY_DIR=${CMAKE_BINARY_DIR}
        -DTEMPLATE_FILE=${CMAKE_SOURCE_DIR}/include/version.h.in
        -DOUTPUT_FILE=${CMAKE_SOURCE_DIR}/include/version.h
        -DPROJECT_VERSION=${PROJECT_VERSION}
        -DPROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        -DPROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        -DPROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        -DPROJECT_NAME=${PROJECT_NAME}
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}
        -DCMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}
        -P ${CMAKE_SOURCE_DIR}/cmake/IncrementBuildNumber.cmake
    COMMENT "Incrementing build number..."
)
add_dependencies(${PROJECT_NAME} increment_build_number)

# ============================================================================
# Link Libraries
# ============================================================================

# Qt6 Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
)

# GuiPrivate for QZipWriter/QZipReader
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::GuiPrivate)
else()
    # On Linux, Qt6GuiPrivate package may not exist; use private include dirs
    target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6Gui_PRIVATE_INCLUDE_DIRS})
endif()

# OpenSSL Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Bei statischem OpenSSL: Hauptprojekt hängt von OpenSSL-Build ab
if(USE_STATIC_OPENSSL)
    add_dependencies(${PROJECT_NAME} openssl_build)
endif()

# ============================================================================
# Installation
# ============================================================================

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Install desktop file and icon for Linux
if(UNIX AND NOT APPLE)
    # Install .desktop file
    configure_file(
        "${CMAKE_SOURCE_DIR}/packaging/onesimus.desktop.in"
        "${CMAKE_BINARY_DIR}/onesimus.desktop"
        @ONLY
    )
    install(FILES "${CMAKE_BINARY_DIR}/onesimus.desktop"
        DESTINATION share/applications
    )

    # Install icon (convert SVG to 256x256 PNG at build time)
    find_program(RSVG_CONVERT rsvg-convert)
    find_program(MAGICK magick)
    find_program(CONVERT convert)
    if(RSVG_CONVERT)
        set(_ICON_CMD ${RSVG_CONVERT} -w 256 -h 256
            "${CMAKE_SOURCE_DIR}/resources/icons/onesimus.svg"
            -o "${CMAKE_BINARY_DIR}/onesimus.png")
    elseif(MAGICK)
        set(_ICON_CMD ${MAGICK} convert -background none -resize 256x256
            "${CMAKE_SOURCE_DIR}/resources/icons/onesimus.svg"
            "${CMAKE_BINARY_DIR}/onesimus.png")
    elseif(CONVERT)
        set(_ICON_CMD ${CONVERT} -background none -resize 256x256
            "${CMAKE_SOURCE_DIR}/resources/icons/onesimus.svg"
            "${CMAKE_BINARY_DIR}/onesimus.png")
    endif()
    if(_ICON_CMD)
        add_custom_command(
            OUTPUT "${CMAKE_BINARY_DIR}/onesimus.png"
            COMMAND ${_ICON_CMD}
            DEPENDS "${CMAKE_SOURCE_DIR}/resources/icons/onesimus.svg"
            COMMENT "Converting onesimus.svg to PNG..."
        )
        add_custom_target(onesimus_icon ALL DEPENDS "${CMAKE_BINARY_DIR}/onesimus.png")
        install(FILES "${CMAKE_BINARY_DIR}/onesimus.png"
            DESTINATION share/icons/hicolor/256x256/apps
        )
    else()
        message(WARNING "No SVG converter found (rsvg-convert, magick, convert) — app icon will not be installed.")
    endif()
endif()

# ============================================================================
# Build Summary
# ============================================================================

message(STATUS "")
message(STATUS "========================================")
message(STATUS "Onesimus - Build Summary")
message(STATUS "========================================")
message(STATUS "Project:       ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build Type:    ${CMAKE_BUILD_TYPE}")
message(STATUS "Install:       ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Qt Version:    ${Qt6_VERSION}")
message(STATUS "C++ Standard:  ${CMAKE_CXX_STANDARD}")
message(STATUS "========================================")

if(USE_STATIC_OPENSSL)
    message(STATUS "OpenSSL:       Static (auto-downloaded via git submodule)")
    if(OPENSSL_VERSION_STRING)
        message(STATUS "  Version:     ${OPENSSL_VERSION_STRING}")
    endif()
    message(STATUS "  Branch:      openssl-3.6.0")
    message(STATUS "  Source:      external/openssl/")
    message(STATUS "  Install:     ${OPENSSL_INSTALL_DIR}")
    message(STATUS "  Libraries:   ${OPENSSL_SSL_LIB}")
    message(STATUS "               ${OPENSSL_CRYPTO_LIB}")
else()
    message(STATUS "OpenSSL:       System")
    message(STATUS "  Version:     ${OPENSSL_VERSION}")
    message(STATUS "  Includes:    ${OPENSSL_INCLUDE_DIR}")
endif()
message(STATUS "========================================")
message(STATUS "")
message(STATUS "Build commands:")
if(WIN32)
    message(STATUS "  nmake         - Compile project")
    message(STATUS "  nmake install - Install executable")
else()
    message(STATUS "  make -j${N_CORES}      - Compile project")
    message(STATUS "  make install  - Install executable")
endif()
message(STATUS "========================================")
message(STATUS "")

# ============================================================================
# OpenSSL DLLs automatisch zu Qt kopieren
# ============================================================================

if(USE_STATIC_OPENSSL AND WIN32)
    # Prüfe ob wir shared libraries gebaut haben
    if(EXISTS "${OPENSSL_INSTALL_DIR}/bin/libssl-3-x64.dll")
        # Qt Bin-Verzeichnis
        get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
        get_filename_component(QT_BIN_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)

        message(STATUS "")
        message(STATUS "========================================")
        message(STATUS "Copying OpenSSL DLLs to Qt")
        message(STATUS "========================================")
        message(STATUS "  From:     ${OPENSSL_INSTALL_DIR}/bin")
        message(STATUS "  To:       ${QT_BIN_DIR}")

        # Kopiere nach OpenSSL-Build
        ExternalProject_Add_Step(openssl_build copy_dlls_to_qt
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${OPENSSL_INSTALL_DIR}/bin/libssl-3-x64.dll"
                "${QT_BIN_DIR}/libssl-3-x64.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${OPENSSL_INSTALL_DIR}/bin/libcrypto-3-x64.dll"
                "${QT_BIN_DIR}/libcrypto-3-x64.dll"
            DEPENDEES install
            COMMENT "Copying OpenSSL DLLs to Qt bin directory"
        )

        message(STATUS "✓ OpenSSL DLLs werden automatisch kopiert")
        message(STATUS "========================================")
        message(STATUS "")
    endif()
endif()

# ============================================================================
# Windows Deployment (windeployqt)
# ============================================================================

if(WIN32)
    # Find windeployqt
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${QT_BIN_DIR})

    if(WINDEPLOYQT_EXECUTABLE)
        message(STATUS "")
        message(STATUS "========================================")
        message(STATUS "Windows Deployment")
        message(STATUS "========================================")
        message(STATUS "  windeployqt: ${WINDEPLOYQT_EXECUTABLE}")
        message(STATUS "")
        message(STATUS "After building, run:")
        message(STATUS "  nmake deploy")
        message(STATUS "  nmake package  (to create installer)")
        message(STATUS "========================================")
        message(STATUS "")

        # Custom target for deployment
        add_custom_target(deploy
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/deploy"
            COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${CMAKE_BINARY_DIR}/deploy/"
            # Copy OpenSSL DLLs first (needed for windeployqt to find them)
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_BINARY_DIR}/openssl-install/bin/libssl-3-x64.dll"
                "${CMAKE_BINARY_DIR}/deploy/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_BINARY_DIR}/openssl-install/bin/libcrypto-3-x64.dll"
                "${CMAKE_BINARY_DIR}/deploy/"
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                $<IF:$<CONFIG:Debug>,--debug,--release>
                --no-translations
                --no-system-d3d-compiler
                --no-opengl-sw
                --compiler-runtime
                --openssl-root "${CMAKE_BINARY_DIR}/openssl-install"
                "${CMAKE_BINARY_DIR}/deploy/${PROJECT_NAME}.exe"
            DEPENDS ${PROJECT_NAME}
            COMMENT "Running windeployqt with OpenSSL support..."
        )
    endif()
endif()

# ============================================================================
# Linux AppImage
# ============================================================================

if(UNIX AND NOT APPLE)
    message(STATUS "")
    message(STATUS "========================================")
    message(STATUS "Linux Deployment (AppImage)")
    message(STATUS "========================================")
    message(STATUS "After building, run:")
    message(STATUS "  make appimage")
    message(STATUS "========================================")
    message(STATUS "")

    # Custom target for AppImage creation
    add_custom_target(appimage
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/AppDir/usr/bin"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/AppDir/usr/share/applications"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/AppDir/usr/share/icons/hicolor/256x256/apps"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${CMAKE_BINARY_DIR}/AppDir/usr/bin/"
        COMMAND ${CMAKE_COMMAND} -E echo "[Desktop Entry]\\nType=Application\\nName=Onesimus\\nExec=${PROJECT_NAME}\\nIcon=${PROJECT_NAME}\\nCategories=Utility;" > "${CMAKE_BINARY_DIR}/AppDir/usr/share/applications/${PROJECT_NAME}.desktop"
        COMMAND ${CMAKE_COMMAND} -E echo "AppImage target prepared. Download linuxdeploy from https://github.com/linuxdeploy/linuxdeploy/releases"
        COMMAND ${CMAKE_COMMAND} -E echo "Then run: linuxdeploy --appdir AppDir --plugin qt --output appimage"
        DEPENDS ${PROJECT_NAME}
        COMMENT "Preparing AppImage directory structure..."
        VERBATIM
    )
endif()

# ============================================================================
# macOS Deployment (macdeployqt)
# ============================================================================

if(APPLE)
    # Find macdeployqt
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS ${QT_BIN_DIR})

    if(MACDEPLOYQT_EXECUTABLE)
        message(STATUS "")
        message(STATUS "========================================")
        message(STATUS "macOS Deployment")
        message(STATUS "========================================")
        message(STATUS "  macdeployqt: ${MACDEPLOYQT_EXECUTABLE}")
        message(STATUS "")
        message(STATUS "After building, run:")
        message(STATUS "  make deploy")
        message(STATUS "  make dmg (to create DMG)")
        message(STATUS "========================================")
        message(STATUS "")

        # Custom target for deployment
        add_custom_target(deploy
            COMMAND ${MACDEPLOYQT_EXECUTABLE} "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../.." -always-overwrite
            DEPENDS ${PROJECT_NAME}
            COMMENT "Running macdeployqt to collect Qt dependencies..."
        )

        # Custom target for DMG creation
        add_custom_target(dmg
            COMMAND ${MACDEPLOYQT_EXECUTABLE} "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../.." -dmg
            DEPENDS deploy
            COMMENT "Creating DMG installer..."
        )
    endif()
endif()

# ============================================================================
# Test Executables
# ============================================================================

# bareosauth_lib - Object library for BareosAuth (shared between main and test)
add_library(bareosauth_lib OBJECT
    src/director/bareosauth.cpp
    include/director/bareosauth.h
)

target_include_directories(bareosauth_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(bareosauth_lib PUBLIC
    Qt6::Core
    Qt6::Network
    OpenSSL::SSL
    OpenSSL::Crypto
)

if(USE_STATIC_OPENSSL)
    add_dependencies(bareosauth_lib openssl_build)
endif()

# bareosauth_test - Test using BareosAuth class
add_executable(bareosauth_test
    test/bareosauth_test.cpp
)

target_link_libraries(bareosauth_test PRIVATE
    bareosauth_lib
    Qt6::Core
    Qt6::Network
    OpenSSL::SSL
    OpenSSL::Crypto
)

# bareosdirector_lib - Object library for BareosDirector (shared between main and test)
add_library(bareosdirector_lib OBJECT
    src/director/bareosdirector.cpp
    src/director/bareosauth.cpp
    src/director/bjsonstreamreader.cpp
    include/director/bareosdirector.h
    include/director/bareosauth.h
    include/director/bjsonstreamreader.h
)

target_include_directories(bareosdirector_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(bareosdirector_lib PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    OpenSSL::SSL
    OpenSSL::Crypto
)

if(USE_STATIC_OPENSSL)
    add_dependencies(bareosdirector_lib openssl_build)
endif()

# director_test - BareosDirector state machine test
add_executable(director_test
    test/director_test.cpp
)

target_link_libraries(director_test PRIVATE
    bareosdirector_lib
    Qt6::Core
    Qt6::Network
    OpenSSL::SSL
    OpenSSL::Crypto
)

message(STATUS "")
message(STATUS "Test executables:")
message(STATUS "  bareosauth_test  - BareosAuth class test")
message(STATUS "  director_test    - BareosDirector state machine test")
message(STATUS "")

# ============================================================================
# CPack Configuration (Package Generation)
# ============================================================================

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Bernau Family")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Onesimus - Bareos Management Tool")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

if(WIN32)
    # NSIS Generator for Windows
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
    set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resources/icon.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/resources/icon.ico")

    # Install the deployed files
    install(DIRECTORY "${CMAKE_BINARY_DIR}/deploy/" DESTINATION bin)
elseif(APPLE)
    # DMG Generator for macOS
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
    set(CPACK_DMG_FORMAT "UDZO")
elseif(UNIX)
    # DEB and RPM for Linux
    set(CPACK_GENERATOR "DEB")

    # Debian package settings
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Joerg Bernau <joerg@bernau.family>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "admin")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/Beerlesklopfer/Onesimus")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
        "Onesimus - Bareos/Bacula Backup Management Tool\n A Qt6-based graphical management console for Bareos and Bacula\n backup systems. Provides client management, job monitoring,\n configuration import/export, and Director administration.")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS
        "libqt6core6 (>= 6.2), libqt6gui6 (>= 6.2), libqt6widgets6 (>= 6.2), libqt6network6 (>= 6.2)")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

    # RPM package settings
    set(CPACK_RPM_PACKAGE_LICENSE "Proprietary")
    set(CPACK_RPM_PACKAGE_GROUP "Applications/System")
    set(CPACK_RPM_PACKAGE_REQUIRES "qt6-qtbase >= 6.2")
    set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
endif()

# include(CPack) must come AFTER all CPACK_* variables are set
include(CPack)
