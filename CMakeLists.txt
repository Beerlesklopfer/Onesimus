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
set(BACKUP_SYSTEM "BAREOS" CACHE STRING "Backup system to support: BAREOS (BACULA is deprecated for now)")
set_property(CACHE BACKUP_SYSTEM PROPERTY STRINGS "BAREOS")

message(STATUS "")
message(STATUS "========================================")
message(STATUS "Backup System Configuration")
message(STATUS "========================================")

# Setze Compiler-Definitionen basierend auf Auswahl
if(BACKUP_SYSTEM STREQUAL "BACULA")
    # The Bacula backend is deprecated for now: there is no BaculaDirector and
    # the sources under src/director/baculaauth.* are kept but not built.
    message(FATAL_ERROR "BACKUP_SYSTEM=BACULA is deprecated for now and cannot be built. Use -DBACKUP_SYSTEM=BAREOS.")
elseif(BACKUP_SYSTEM STREQUAL "BAREOS")
    set(USE_BAREOS_ONLY TRUE)
    set(BAREOS_VERSION_STR "25.0.0")
else()
    message(FATAL_ERROR "Invalid BACKUP_SYSTEM: ${BACKUP_SYSTEM}. Must be BAREOS")
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
        # Strawberry Perl bevorzugen (Git-MSYS-Perl hat nicht alle Module)
        find_program(PERL_EXECUTABLE perl
            HINTS "C:/Strawberry/perl/bin"
            NO_DEFAULT_PATH
        )
        if(NOT PERL_EXECUTABLE)
            find_program(PERL_EXECUTABLE perl)
        endif()
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
# Minimum Qt version: also stated as the dependency of the DEB/RPM packages
set(ONESIMUS_QT_MIN_VERSION 6.8)
find_package(Qt6 ${ONESIMUS_QT_MIN_VERSION} REQUIRED COMPONENTS Core)

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

# gRPC transport (Onesimus Director) - optional, BNET works without it
option(ENABLE_GRPC "Build gRPC transport for the Onesimus Director (needs QtGrpc/QtProtobuf)" ON)
if(ENABLE_GRPC)
    find_package(Qt6 COMPONENTS Protobuf ProtobufWellKnownTypes Grpc QUIET)
    if(Qt6Protobuf_FOUND AND Qt6ProtobufWellKnownTypes_FOUND AND Qt6Grpc_FOUND
       AND EXISTS ${CMAKE_SOURCE_DIR}/proto/director_service.proto
       AND EXISTS ${CMAKE_SOURCE_DIR}/proto/director_api.proto)
        set(HAVE_GRPC ON)
        message(STATUS "✓ gRPC transport enabled (QtGrpc ${Qt6Grpc_VERSION})")
    else()
        set(HAVE_GRPC OFF)
        message(WARNING "gRPC transport disabled: QtGrpc/QtProtobuf(WellKnownTypes) or proto/ submodule missing")
    endif()
endif()

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
    src/director/bareosauth.cpp

    src/bcheckableheaderview.cpp
    src/bcheckboxdelegate.cpp
    src/director/bjsonstreamreader.cpp
    src/btranslations.cpp
    src/director/bdirector.cpp
    src/director/bareosdirector.cpp
    src/director/bdirectorconfigclient.cpp
    src/db/bdatabase.cpp
    src/db/bresourcemodel.cpp
    src/db/bdirectormodel.cpp
    src/bcleanupdialog.cpp
    src/bcapabilities.cpp
    src/bcapabilitynotice.cpp
    src/buiscale.cpp
    src/bstatuschip.cpp
    src/bstatusdelegate.cpp
    src/bheaderstate.cpp
    src/bcleanupplan.cpp
    src/bcleanuprunner.cpp
    src/bconnectionwizard.cpp
    src/bmdnsbrowser.cpp
    src/bcertificategenerator.cpp
    src/bpfxconverter.cpp
    src/storagewidget.cpp
    src/bbyteformat.cpp
    src/models/bvolumejson.cpp
    src/storage/model/bvolumelist.cpp
    src/storage/model/brecycleforecast.cpp
    src/tools/b321verifier.cpp
    src/tools/b321dialog.cpp
    src/tools/b321action.cpp

    src/config/bcolumnconfiguration.cpp
    src/config/bsettings.cpp
    src/config/bsettingsdialog.cpp
    src/config/bprofilesettingsdialog.cpp
    src/config/bconfigexporter.cpp
    src/config/bsettingshistory.cpp

    src/models/bbasemodels.cpp
    src/models/bresourcemodels.cpp

    src/jobs/bjobmodels.cpp
    src/jobs/bjoblogmodel.cpp
    src/jobs/bjoblogsource.cpp
    src/jobs/bjoblogview.cpp
    src/jobs/bjobdetailsdialog.cpp
    src/jobs/bbvfsmodel.cpp
    src/jobs/bjobfileswidget.cpp
    src/jobs/brestorewizard.cpp
    src/jobs/bjoblogdialog.cpp
    src/jobs/bjobsstatisticswidget.cpp
    src/jobs/bjobwidget.cpp
    src/jobs/bjsonjobview.cpp
    src/jobs/bnewjobdialog.cpp
    src/jobs/brunjobcommand.cpp
    src/jobs/bfilesetwizard.cpp
    src/jobs/bjobwizard.cpp
    src/jobs/fileset/bfilesetdocument.cpp
    src/jobs/fileset/bincludeblockwidget.cpp

    src/schedules/bschedulewidget.cpp

    src/schedules/bschedulelegend.cpp
    src/schedules/bschedulegridaccessible.cpp
    src/schedules/bschedulegriddialogs.cpp
    src/schedules/bschedulegrideditor.cpp
    src/schedules/bschedulegridgeometry.cpp
    src/schedules/bschedulegridview.cpp
    src/schedules/bschedulegridwidget.cpp
    src/schedules/bscheduleminicalendar.cpp
    src/schedules/bschedulepalette.cpp
    src/schedules/brunruledialog.cpp
    src/schedules/bscheduleganttwidget.cpp
    src/schedules/bschedulewizard.cpp
    src/schedules/bjobscheduleindex.cpp
    src/schedules/bscheduledragplan.cpp
    src/schedules/bschedulepersistence.cpp
    src/schedules/bschedulereviewdialog.cpp
    src/schedules/model/bscheduleresource.cpp
    src/config/bresourcetext.cpp
    src/config/bresourcechanges.cpp
    src/config/bresourcepersistence.cpp
    src/config/bresourcereviewdialog.cpp
    src/jobs/bjobdefinitions.cpp
    src/jobs/bjobdefinitionswidget.cpp
    src/schedules/model/brunparser.cpp
    src/schedules/model/brunwriter.cpp
    src/schedules/model/bscheduleoccurrences.cpp
    src/schedules/model/bjobhistorystats.cpp
    src/schedules/model/bschedulewaitmodel.cpp
    src/schedules/model/bwaitcalibration.cpp
    src/schedules/model/bschedulegridmodel.cpp
    src/schedules/model/bschedulegridtext.cpp
    src/jobs/bpaginationwidget.cpp
    src/jobs/bviewpresets.cpp

    src/messages/bmessageswidget.cpp

    src/clients/bclientwidget.cpp
    src/clients/bclientsmodel.cpp
    src/clients/bclientstatus.cpp
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
    include/bbyteformat.h
    include/models/bvolumejson.h
    include/storage/model/bvolumelist.h
    include/storage/model/brecycleforecast.h
    include/tools/b321verifier.h
    include/tools/b321dialog.h
    include/bcleanupdialog.h
    include/bcapabilities.h
    include/bcapabilitynotice.h
    include/buiscale.h
    include/bstatuschip.h
    include/bstatusdelegate.h
    include/bheaderstate.h
    include/bcleanupplan.h
    include/bcleanuprunner.h
    include/bconnectionwizard.h
    include/bmdnsbrowser.h
    include/bcertificategenerator.h
    include/bpfxconverter.h
    include/director/bdirector.h
    include/director/bareosdirector.h
    include/db/bdatabase.h
    include/db/bresourcemodel.h
    include/db/bdirectormodel.h
    include/version.h
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
    include/jobs/bjoblogmodel.h
    include/jobs/bjoblogsource.h
    include/jobs/bjoblogview.h
    include/director/bjobpushmessage.h
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
    include/jobs/brunjobcommand.h
    include/jobs/bfilesetwizard.h
    include/jobs/bjobwizard.h
    include/jobs/fileset/bfilesetdocument.h
    include/jobs/fileset/bincludeblockwidget.h

    include/clients/bclientsmodel.h
    include/clients/bclientstatus.h
    include/bsortvalue.h
    include/bsorttableitem.h
    include/blabelstate.h
    include/jobs/bjobpaging.h
    include/director/bdirectorreply.h
    include/clients/bclientswidget.h
    include/clients/bclientdetailsdialog.h
    include/clients/bnewclientdialog.h

    include/schedules/bschedulewidget.h

    include/schedules/bschedulelegend.h
    include/schedules/bschedulegridaccessible.h
    include/schedules/bschedulegriddialogs.h
    include/schedules/bschedulegrideditor.h
    include/schedules/bschedulegridgeometry.h
    include/schedules/bschedulegridview.h
    include/schedules/bschedulegridwidget.h
    include/schedules/bscheduleminicalendar.h
    include/schedules/bschedulepalette.h
    include/schedules/brunruledialog.h
    include/schedules/bscheduleganttwidget.h
    include/schedules/bschedulewizard.h
    include/schedules/bjobscheduleindex.h
    include/schedules/bscheduledragplan.h
    include/schedules/bschedulepersistence.h
    include/schedules/bschedulereviewdialog.h
    include/schedules/model/bscheduleresource.h
    include/config/bresourcetext.h
    include/config/bresourcechanges.h
    include/config/bresourcepersistence.h
    include/config/bresourcereviewdialog.h
    include/jobs/bjobdefinitions.h
    include/jobs/bjobdefinitionswidget.h
    include/director/bconfigtypes.h
    include/director/bconfigclient.h
    include/director/bdirectorconfigclient.h
    include/schedules/model/brunspec.h
    include/schedules/model/bscheduleoccurrences.h
    include/schedules/model/bjobhistorystats.h
    include/schedules/model/bschedulewaitmodel.h
    include/schedules/model/bwaitcalibration.h
    include/schedules/model/bschedulegridmodel.h
    include/schedules/model/bschedulegridtext.h

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

# Windows application icon and version resource. Version, copyright year
# and file name come from the project; the .rc is generated into the build
# directory (on every platform, so that its content can be checked anywhere)
# and compiled on Windows only.
string(TIMESTAMP ONESIMUS_RC_YEAR "%Y" UTC)
set(ONESIMUS_RC_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/onesimus.ico")
set(ONESIMUS_RC_FILENAME "${PROJECT_NAME}.exe")
set(WIN_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/onesimus.rc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/onesimus.rc.in ${WIN_RC_FILE} @ONLY)
if(NOT EXISTS "${ONESIMUS_RC_ICON}")
    message(FATAL_ERROR "Application icon not found: ${ONESIMUS_RC_ICON}")
endif()
if(WIN32)
    list(APPEND SOURCES ${WIN_RC_FILE})
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

# gRPC clients, generated from the proto submodule:
#  - director_service.proto: service Director (login, command lines, push)
#  - director_api.proto: service DirectorApi (typed RPCs, configuration CRUD)
#    with what it imports: director_commands.proto, onesimus_options.proto
#    (custom options, see below) and google/protobuf/struct.proto (Qt's
#    well-known types)
if(HAVE_GRPC)
    set(ONESIMUS_PROTO_FILES
        ${CMAKE_SOURCE_DIR}/proto/director_service.proto
        ${CMAKE_SOURCE_DIR}/proto/director_api.proto
        ${CMAKE_SOURCE_DIR}/proto/director_commands.proto
    )
    # onesimus_options.proto declares only custom options (extensions of
    # google.protobuf.MessageOptions/FieldOptions) for the generators of the
    # Director. qtprotobufgen writes no header for a file without messages,
    # but director_commands.qpb.h and director_api.qpb.h include one. The
    # options mean nothing to the GUI: an empty header stands in for it.
    set(ONESIMUS_PROTO_STUB_DIR ${CMAKE_BINARY_DIR}/proto_stub)
    file(WRITE ${ONESIMUS_PROTO_STUB_DIR}/onesimus_options.qpb.h
        "// Generated by CMakeLists.txt: onesimus_options.proto has no messages\n"
        "#pragma once\n")
    qt_add_library(onesimus_grpc_proto STATIC)
    qt_add_protobuf(onesimus_grpc_proto
        PROTO_FILES ${ONESIMUS_PROTO_FILES}
        PROTO_INCLUDES ${CMAKE_SOURCE_DIR}/proto
    )
    qt_add_grpc(onesimus_grpc_proto CLIENT
        PROTO_FILES
            ${CMAKE_SOURCE_DIR}/proto/director_service.proto
            ${CMAKE_SOURCE_DIR}/proto/director_api.proto
        PROTO_INCLUDES ${CMAKE_SOURCE_DIR}/proto
    )
    target_include_directories(onesimus_grpc_proto PUBLIC ${ONESIMUS_PROTO_STUB_DIR})
    target_link_libraries(onesimus_grpc_proto PUBLIC Qt6::Protobuf Qt6::ProtobufWellKnownTypes Qt6::Grpc Qt6::Network)
    target_compile_definitions(onesimus_grpc_proto PUBLIC HAVE_GRPC)
    target_sources(${PROJECT_NAME} PRIVATE
        src/director/bgrpctransport.cpp
        include/director/bgrpctransport.h
        src/director/bgrpcconnect.cpp
        include/director/bgrpcconnect.h
        src/director/bconfigproto.cpp
        include/director/bconfigproto.h
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE onesimus_grpc_proto)
endif()

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

    # Install the app icon. The SVG goes to hicolor/scalable and is used by
    # GTK/Qt at every size; PNG sizes are rendered in addition for tools that
    # only read raster icons. (A broken SVG->PNG conversion once shipped an
    # almost empty 256x256 PNG that GNOME preferred over everything else.)
    install(FILES "${CMAKE_SOURCE_DIR}/resources/icons/onesimus.svg"
        DESTINATION share/icons/hicolor/scalable/apps
    )
    find_program(RSVG_CONVERT rsvg-convert)
    find_program(MAGICK magick)
    find_program(CONVERT convert)
    if(MAGICK)
        set(_IM ${MAGICK})
    elseif(CONVERT)
        set(_IM ${CONVERT})
    endif()
    set(_ICON_SVG "${CMAKE_SOURCE_DIR}/resources/icons/onesimus.svg")
    set(_ICON_256 "${CMAKE_BINARY_DIR}/icons/256x256/onesimus.png")
    set(_ICON_OUTPUTS)
    # The SVG is rendered once (ImageMagick may delegate SVG to Inkscape,
    # which fails when several instances run in parallel); the smaller
    # sizes are scaled down from the 256 px PNG.
    if(RSVG_CONVERT)
        set(_render ${RSVG_CONVERT} -w 256 -h 256 "${_ICON_SVG}" -o "${_ICON_256}")
    elseif(_IM)
        # High density: rasterizing at 72 dpi and scaling up/down ends up (almost) empty
        set(_render ${_IM} -background none -density 1200 "${_ICON_SVG}" -resize 256x256 "${_ICON_256}")
    endif()
    if(_render)
        add_custom_command(
            OUTPUT "${_ICON_256}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/icons/256x256"
            COMMAND ${_render}
            DEPENDS "${_ICON_SVG}"
            COMMENT "Rendering onesimus icon 256x256"
        )
        list(APPEND _ICON_OUTPUTS "${_ICON_256}")
        install(FILES "${_ICON_256}" DESTINATION share/icons/hicolor/256x256/apps)
        if(_IM)
            foreach(_size 16 24 32 48 64 128)
                set(_png "${CMAKE_BINARY_DIR}/icons/${_size}x${_size}/onesimus.png")
                add_custom_command(
                    OUTPUT "${_png}"
                    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/icons/${_size}x${_size}"
                    COMMAND ${_IM} "${_ICON_256}" -resize ${_size}x${_size} "${_png}"
                    DEPENDS "${_ICON_256}"
                    COMMENT "Scaling onesimus icon to ${_size}x${_size}"
                )
                list(APPEND _ICON_OUTPUTS "${_png}")
                install(FILES "${_png}" DESTINATION share/icons/hicolor/${_size}x${_size}/apps)
            endforeach()
        endif()
    endif()
    if(_ICON_OUTPUTS)
        add_custom_target(onesimus_icon ALL DEPENDS ${_ICON_OUTPUTS})
    else()
        message(STATUS "No SVG converter found (rsvg-convert, magick, convert) - installing the scalable SVG icon only.")
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

if(HAVE_GRPC)
    target_sources(bareosdirector_lib PRIVATE
        src/director/bgrpctransport.cpp
        include/director/bgrpctransport.h
        src/director/bgrpcconnect.cpp
        include/director/bgrpcconnect.h
        src/director/bconfigproto.cpp
        include/director/bconfigproto.h
    )
    target_link_libraries(bareosdirector_lib PUBLIC onesimus_grpc_proto)
endif()

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

# volumejson_test - "list volumes" reply parsing (grouped by pool and flat)
add_executable(volumejson_test
    test/volumejson_test.cpp
    src/models/bvolumejson.cpp
    include/models/bvolumejson.h
)

target_include_directories(volumejson_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(volumejson_test PRIVATE
    Qt6::Core
)

# CTest: tests that run without a Director (cmake --build . && ctest)
include(CTest)
if(BUILD_TESTING)
    find_package(Qt6 REQUIRED COMPONENTS Test)

    # volumejson_test is built above; register it so ctest runs it
    add_test(NAME volumejson_test COMMAND volumejson_test)

    # bbyteformat_test - byte counts in binary units with IEC names and the
    # size suffixes of Bareos directives ("50G" is 50 GiB)
    add_executable(bbyteformat_test
        test/bbyteformat_test.cpp
        src/bbyteformat.cpp
        include/bbyteformat.h
    )

    target_include_directories(bbyteformat_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bbyteformat_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bbyteformat_test COMMAND bbyteformat_test)

    # bvolumelist_test - the volumes of "list volumes" / "llist volumes":
    # grouped, flat and typed replies, missing fields, the real reply of the
    # production Director (test/fixtures)
    add_executable(bvolumelist_test
        test/bvolumelist_test.cpp
        src/storage/model/bvolumelist.cpp
        src/models/bvolumejson.cpp
        include/storage/model/bvolumelist.h
        include/models/bvolumejson.h
    )

    target_include_directories(bvolumelist_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(bvolumelist_test PRIVATE
        STORAGE_FIXTURE_DIR="${CMAKE_SOURCE_DIR}/test/fixtures"
    )

    target_link_libraries(bvolumelist_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bvolumelist_test COMMAND bvolumelist_test)
    # Catalog times are local times: fix the zone
    set_tests_properties(bvolumelist_test PROPERTIES ENVIRONMENT "TZ=UTC")

    # brecycleforecast_test - "reusable from" of a volume: volume and job
    # retention paths, the min() rule, the cases that never become reusable
    add_executable(brecycleforecast_test
        test/brecycleforecast_test.cpp
        src/storage/model/brecycleforecast.cpp
        src/storage/model/bvolumelist.cpp
        src/models/bvolumejson.cpp
        src/bbyteformat.cpp
        include/storage/model/brecycleforecast.h
        include/storage/model/bvolumelist.h
        include/models/bvolumejson.h
        include/bbyteformat.h
    )

    target_include_directories(brecycleforecast_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(brecycleforecast_test PRIVATE
        STORAGE_FIXTURE_DIR="${CMAKE_SOURCE_DIR}/test/fixtures"
    )

    target_link_libraries(brecycleforecast_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME brecycleforecast_test COMMAND brecycleforecast_test)
    set_tests_properties(brecycleforecast_test PROPERTIES ENVIRONMENT "TZ=UTC")

    # storagewidget_test - the Storage/Volumes tab on the real volume list of
    # the production Director: columns, filters, keyboard, and that it asks
    # for "llist volumes" only (widgets, no Director)
    add_executable(storagewidget_test
        test/storagewidget_test.cpp
        src/storagewidget.cpp
        src/storage/model/bvolumelist.cpp
        src/storage/model/brecycleforecast.cpp
        src/models/bvolumejson.cpp
        src/bbyteformat.cpp
        src/buiscale.cpp
        src/config/bsettings.cpp
        src/bstatuschip.cpp
        src/bstatusdelegate.cpp
        src/clients/bclientstatus.cpp
        ui/storagewidget.ui
        include/storagewidget.h
        include/bstatuschip.h
        include/bstatusdelegate.h
        include/storage/model/bvolumelist.h
        include/storage/model/brecycleforecast.h
        include/models/bvolumejson.h
        include/bbyteformat.h
        include/buiscale.h
        include/config/bsettings.h
    )

    target_include_directories(storagewidget_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(storagewidget_test PRIVATE
        STORAGE_FIXTURE_DIR="${CMAKE_SOURCE_DIR}/test/fixtures"
    )

    # bareosdirector_lib: the tab names commands by BDirector::Command, an
    # enum of the BareosDirector class. Nothing of a Director is created.
    target_link_libraries(storagewidget_test PRIVATE
        bareosdirector_lib
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Test
        OpenSSL::SSL
        OpenSSL::Crypto
    )

    add_test(NAME storagewidget_test COMMAND storagewidget_test)
    # Widgets need a QApplication; no display is used. Settings go to the
    # build directory, never to those of the user.
    set_tests_properties(storagewidget_test PROPERTIES ENVIRONMENT
        "QT_QPA_PLATFORM=offscreen;TZ=UTC;XDG_CONFIG_HOME=${CMAKE_BINARY_DIR}/storagewidget_test_config")

    # bstatuschip_test - status chips of the jobs, clients and volumes tables:
    # tone, code and words of every Bareos JobStatus, the theme tokens of
    # both themes, and how BStatusDelegate paints a chip (widgets, no Director)
    add_executable(bstatuschip_test
        test/bstatuschip_test.cpp
        src/bstatuschip.cpp
        src/bstatusdelegate.cpp
        src/clients/bclientstatus.cpp
        src/buiscale.cpp
        src/config/bsettings.cpp
        include/bstatuschip.h
        include/bstatusdelegate.h
        include/clients/bclientstatus.h
        include/buiscale.h
        include/config/bsettings.h
    )

    target_include_directories(bstatuschip_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(bstatuschip_test PRIVATE
        THEME_DIR="${CMAKE_SOURCE_DIR}/resources/themes"
    )

    target_link_libraries(bstatuschip_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test
    )

    add_test(NAME bstatuschip_test COMMAND bstatuschip_test)
    set_tests_properties(bstatuschip_test PROPERTIES ENVIRONMENT
        "QT_QPA_PLATFORM=offscreen;XDG_CONFIG_HOME=${CMAKE_BINARY_DIR}/bstatuschip_test_config")

    # bjobstable_test - the jobs table: header state in BSettings (round trip,
    # interface zoom, one-time migration of the old column configuration),
    # sorting by value, no row colour by level (widgets, no Director)
    add_executable(bjobstable_test
        test/bjobstable_test.cpp
        src/jobs/bjobmodels.cpp
        src/bheaderstate.cpp
        src/bcheckableheaderview.cpp
        src/config/bcolumnconfiguration.cpp
        src/bstatuschip.cpp
        src/clients/bclientstatus.cpp
        src/buiscale.cpp
        src/config/bsettings.cpp
        include/jobs/bjobmodels.h
        include/bheaderstate.h
        include/bcheckableheaderview.h
        include/config/bcolumnconfiguration.h
        include/bstatuschip.h
        include/buiscale.h
        include/config/bsettings.h
    )

    target_include_directories(bjobstable_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    # bareosdirector_lib: the jobs model keeps a BDirector pointer; nothing
    # of a Director is created
    target_link_libraries(bjobstable_test PRIVATE
        bareosdirector_lib
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Test
        OpenSSL::SSL
        OpenSSL::Crypto
    )

    add_test(NAME bjobstable_test COMMAND bjobstable_test)
    set_tests_properties(bjobstable_test PROPERTIES ENVIRONMENT
        "QT_QPA_PLATFORM=offscreen;TZ=UTC;XDG_CONFIG_HOME=${CMAKE_BINARY_DIR}/bjobstable_test_config")

    # bjoblogmodel_test - job log model, its sources and the push message parser
    add_executable(bjoblogmodel_test
        test/bjoblogmodel_test.cpp
        src/jobs/bjoblogmodel.cpp
        src/jobs/bjoblogsource.cpp
        src/bstatuschip.cpp
        src/clients/bclientstatus.cpp
        include/jobs/bjoblogmodel.h
        include/jobs/bjoblogsource.h
        include/director/bjobpushmessage.h
        include/bstatuschip.h
    )

    target_include_directories(bjoblogmodel_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bjoblogmodel_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Test
    )

    add_test(NAME bjoblogmodel_test COMMAND bjoblogmodel_test)

    # brunjobcommand_test - command line of the "Run Job" dialog
    add_executable(brunjobcommand_test
        test/brunjobcommand_test.cpp
        src/jobs/brunjobcommand.cpp
        include/jobs/brunjobcommand.h
    )

    target_include_directories(brunjobcommand_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(brunjobcommand_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME brunjobcommand_test COMMAND brunjobcommand_test)

    # bcleanup_test - cleanup selection, analysis invalidation and command sequencing
    add_executable(bcleanup_test
        test/bcleanup_test.cpp
        src/bcleanupplan.cpp
        src/bcleanuprunner.cpp
        include/bcleanupplan.h
        include/bcleanuprunner.h
    )

    target_include_directories(bcleanup_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bcleanup_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bcleanup_test COMMAND bcleanup_test)

    # bdatabase_test - schema creation, migrations and repair on a temporary SQLite file
    # (uses the SQL scripts from the application's resource file)
    add_executable(bdatabase_test
        test/bdatabase_test.cpp
        src/db/bdatabase.cpp
        include/db/bdatabase.h
        ${THEME_RESOURCES}
    )

    target_include_directories(bdatabase_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bdatabase_test PRIVATE
        Qt6::Core
        Qt6::Sql
        Qt6::Test
    )

    add_test(NAME bdatabase_test COMMAND bdatabase_test)

    # bsortvalue_test - sort keys of the jobs, clients and storage tables,
    # ordered through a proxy model's sort role and through table items
    add_executable(bsortvalue_test
        test/bsortvalue_test.cpp
        include/bsortvalue.h
        include/bsorttableitem.h
    )

    target_include_directories(bsortvalue_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bsortvalue_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test
    )

    add_test(NAME bsortvalue_test COMMAND bsortvalue_test)

    # bclientstatus_test - age and state of a client's last backup (issue #9)
    add_executable(bclientstatus_test
        test/bclientstatus_test.cpp
        src/clients/bclientstatus.cpp
        include/clients/bclientstatus.h
    )

    target_include_directories(bclientstatus_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bclientstatus_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bclientstatus_test COMMAND bclientstatus_test)

    # bjobpaging_test - page arithmetic of the job list pagination
    add_executable(bjobpaging_test
        test/bjobpaging_test.cpp
        include/jobs/bjobpaging.h
    )

    target_include_directories(bjobpaging_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bjobpaging_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bjobpaging_test COMMAND bjobpaging_test)

    # bdirectorreply_test - verdict on the reply of "reload" (job and client wizards)
    add_executable(bdirectorreply_test
        test/bdirectorreply_test.cpp
        include/director/bdirectorreply.h
    )

    target_include_directories(bdirectorreply_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bdirectorreply_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bdirectorreply_test COMMAND bdirectorreply_test)

    # blogredact_test - password redaction in logged command lines and the
    # owner-only log file (built with file logging to exercise the real logger)
    add_executable(blogredact_test
        test/blogredact_test.cpp
        include/blogging.h
        include/blogredact.h
        include/bprivatefile.h
    )

    target_include_directories(blogredact_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(blogredact_test PRIVATE ONESIMUS_FILE_LOGGING)

    target_link_libraries(blogredact_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME blogredact_test COMMAND blogredact_test)

    # bnetframe_test - limit on the length header of a BNET telegram
    add_executable(bnetframe_test
        test/bnetframe_test.cpp
        include/director/bnetframe.h
    )

    target_include_directories(bnetframe_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bnetframe_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bnetframe_test COMMAND bnetframe_test)

    # bcommandlineconnection_test - --tls* options and password sources of the command line
    add_executable(bcommandlineconnection_test
        test/bcommandlineconnection_test.cpp
        include/bcommandlineconnection.h
    )

    target_include_directories(bcommandlineconnection_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bcommandlineconnection_test PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Test
        OpenSSL::Crypto
    )

    if(USE_STATIC_OPENSSL)
        add_dependencies(bcommandlineconnection_test openssl_build)
    endif()

    add_test(NAME bcommandlineconnection_test COMMAND bcommandlineconnection_test)

    # bdirectorqueue_test - command queue of BareosDirector without a connection
    add_executable(bdirectorqueue_test
        test/bdirectorqueue_test.cpp
    )

    target_link_libraries(bdirectorqueue_test PRIVATE
        bareosdirector_lib
        Qt6::Core
        Qt6::Network
        Qt6::Test
        OpenSSL::SSL
        OpenSSL::Crypto
    )

    add_test(NAME bdirectorqueue_test COMMAND bdirectorqueue_test)

    # brunparser_test - the Run line model: parser, writer and round trips
    add_executable(brunparser_test
        test/brunparser_test.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        include/schedules/model/brunspec.h
    )

    target_include_directories(brunparser_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(brunparser_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME brunparser_test COMMAND brunparser_test)

    # bscheduledragplan_test - the Run lines written after a drag in the
    # schedule view, the schedule entry adapter over the Run line model, and
    # the Messages names for the job wizard
    add_executable(bscheduledragplan_test
        test/bscheduledragplan_test.cpp
        src/schedules/bscheduledragplan.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        src/schedules/model/bscheduleoccurrences.cpp
        src/models/bresourcemodels.cpp
        src/models/bbasemodels.cpp
        src/config/bconfigparser.cpp
        include/schedules/bscheduledragplan.h
        include/schedules/model/brunspec.h
        include/schedules/model/bscheduleoccurrences.h
        include/models/bresourcemodels.h
        include/models/bbasemodels.h
        include/config/bconfigparser.h
    )

    target_include_directories(bscheduledragplan_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bscheduledragplan_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Test
    )

    add_test(NAME bscheduledragplan_test COMMAND bscheduledragplan_test)

    # bscheduleoccurrences_test - when the Run lines of a schedule trigger on
    # concrete dates (weeks of the month and of the year, daylight saving),
    # overlaps on concrete dates, and the entries the schedule views get
    add_executable(bscheduleoccurrences_test
        test/bscheduleoccurrences_test.cpp
        src/schedules/model/bscheduleoccurrences.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        src/schedules/bjobscheduleindex.cpp
        src/schedules/model/bschedulewaitmodel.cpp
        src/models/bresourcemodels.cpp
        src/models/bbasemodels.cpp
        src/config/bconfigparser.cpp
        include/schedules/model/bscheduleoccurrences.h
        include/schedules/model/brunspec.h
        include/schedules/bjobscheduleindex.h
        include/models/bresourcemodels.h
        include/models/bbasemodels.h
        include/config/bconfigparser.h
    )

    target_include_directories(bscheduleoccurrences_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bscheduleoccurrences_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Test
    )

    add_test(NAME bscheduleoccurrences_test COMMAND bscheduleoccurrences_test)

    # bmessagesmodel_test - row signals of the Director messages list at its history limit
    add_executable(bmessagesmodel_test
        test/bmessagesmodel_test.cpp
        src/messages/bmessageswidget.cpp
        src/buiscale.cpp
        include/messages/bmessageswidget.h
        include/buiscale.h
    )

    target_link_libraries(bmessagesmodel_test PRIVATE
        bareosdirector_lib
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Test
        OpenSSL::SSL
        OpenSSL::Crypto
    )

    add_test(NAME bmessagesmodel_test COMMAND bmessagesmodel_test)
    # The widget's translation unit needs a QApplication; no display is used
    set_tests_properties(bmessagesmodel_test PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

    # bschedulepersistence_test - writing schedules: the change written into
    # the raw text of the resource (untouched lines byte for byte, CRLF
    # kept), the flow over the configuration methods with a fake Director
    # (check first, review, every status, conflicts never overwritten,
    # referenced schedules not deleted, locked schedules not written), and
    # with gRPC the bytes of the requests (no Director)
    add_executable(bschedulepersistence_test
        test/bschedulepersistence_test.cpp
        test/bfakeconfigclient.h
        test/bfakesaveui.h
        src/schedules/bschedulepersistence.cpp
        src/schedules/bscheduledragplan.cpp
        src/schedules/model/bscheduleresource.cpp
        src/config/bresourcetext.cpp
        src/config/bresourcepersistence.cpp
        include/config/bresourcetext.h
        include/config/bresourcepersistence.h
        src/schedules/model/bschedulegridmodel.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        include/schedules/bschedulepersistence.h
        include/schedules/bscheduledragplan.h
        include/schedules/model/bscheduleresource.h
        include/schedules/model/bschedulegridmodel.h
        include/schedules/model/brunspec.h
        include/director/bconfigclient.h
        include/director/bconfigtypes.h
    )

    target_include_directories(bschedulepersistence_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/test
    )

    target_link_libraries(bschedulepersistence_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    if(HAVE_GRPC)
        target_sources(bschedulepersistence_test PRIVATE
            src/director/bconfigproto.cpp
            include/director/bconfigproto.h
        )
        target_link_libraries(bschedulepersistence_test PRIVATE onesimus_grpc_proto)
    endif()

    add_test(NAME bschedulepersistence_test COMMAND bschedulepersistence_test)

    # bresourcetext_test - the raw text of a resource: directives read,
    # single directives changed, added and removed with every other line
    # byte for byte (comments, unknown directives, nested blocks, CRLF),
    # unsafe edits refused, the line diff of the review (no Director)
    add_executable(bresourcetext_test
        test/bresourcetext_test.cpp
        src/config/bresourcetext.cpp
        include/config/bresourcetext.h
    )
    target_include_directories(bresourcetext_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(bresourcetext_test PRIVATE Qt6::Core Qt6::Test)
    add_test(NAME bresourcetext_test COMMAND bresourcetext_test)

    # bresourcechanges_test - what the resource form (Edit Job...) changed,
    # as directive changes: spelling of keys and booleans, defaults the form
    # shows, emptied fields, list values not written (no Director)
    add_executable(bresourcechanges_test
        test/bresourcechanges_test.cpp
        src/config/bresourcechanges.cpp
        src/config/bresourcetext.cpp
        src/config/bconfigparser.cpp
        include/config/bresourcechanges.h
        include/config/bresourcetext.h
        include/config/bconfigparser.h
    )
    target_include_directories(bresourcechanges_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(bresourcechanges_test PRIVATE Qt6::Core Qt6::Test)
    add_test(NAME bresourcechanges_test COMMAND bresourcechanges_test)

    # bjobdefinitions_test - Job and JobDefs: values resolved over the JobDefs
    # chain (here, inherited from which JobDefs, default, circles, missing
    # JobDefs), the raw text as the judge of what is set here, who uses a
    # JobDefs (no Director)
    add_executable(bjobdefinitions_test
        test/bjobdefinitions_test.cpp
        src/jobs/bjobdefinitions.cpp
        src/config/bresourcetext.cpp
        include/jobs/bjobdefinitions.h
        include/config/bresourcetext.h
    )
    target_include_directories(bjobdefinitions_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(bjobdefinitions_test PRIVATE Qt6::Core Qt6::Test)
    add_test(NAME bjobdefinitions_test COMMAND bjobdefinitions_test)

    # bjobdefinitionswidget_test - the Job Definitions tab with a fake
    # Director: save flow (check, review with diff and affected jobs, write)
    # and every status, the raw text changed only where edited, delete
    # refused while referenced, create and copy, BNET and missing ACLs
    # read-only with nothing sent (widgets, no Director)
    add_executable(bjobdefinitionswidget_test
        test/bjobdefinitionswidget_test.cpp
        test/bfakeconfigclient.h
        src/jobs/bjobdefinitionswidget.cpp
        src/jobs/bjobdefinitions.cpp
        src/config/bresourcetext.cpp
        src/config/bresourcepersistence.cpp
        src/config/bresourcereviewdialog.cpp
        src/bcapabilities.cpp
        src/bheaderstate.cpp
        src/config/bcolumnconfiguration.cpp
        src/bstatuschip.cpp
        src/bstatusdelegate.cpp
        src/clients/bclientstatus.cpp
        src/buiscale.cpp
        src/config/bsettings.cpp
        include/jobs/bjobdefinitionswidget.h
        include/jobs/bjobdefinitions.h
        include/config/bresourcetext.h
        include/config/bresourcepersistence.h
        include/config/bresourcereviewdialog.h
        include/director/bconfigclient.h
        include/director/bconfigtypes.h
        include/bcapabilities.h
        include/bheaderstate.h
        include/config/bcolumnconfiguration.h
        include/bstatuschip.h
        include/bstatusdelegate.h
        include/clients/bclientstatus.h
        include/buiscale.h
        include/config/bsettings.h
    )
    target_include_directories(bjobdefinitionswidget_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/test
    )
    target_link_libraries(bjobdefinitionswidget_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test
    )
    add_test(NAME bjobdefinitionswidget_test COMMAND bjobdefinitionswidget_test)
    # Widgets need a QApplication; no display is used. The settings go to
    # the build directory, never to those of the user (the test redirects
    # them itself as well).
    set_tests_properties(bjobdefinitionswidget_test PROPERTIES ENVIRONMENT
        "QT_QPA_PLATFORM=offscreen;XDG_CONFIG_HOME=${CMAKE_BINARY_DIR}/bjobdefinitionswidget_test_config")

    # bgrpcconnect_test - reaching the gRPC port: order of the resolved
    # addresses, the TLS probe per address against sockets on the loopback
    # interface, the texts of failed calls (no Director)
    add_executable(bgrpcconnect_test
        test/bgrpcconnect_test.cpp
        src/director/bgrpcconnect.cpp
        include/director/bgrpcconnect.h
    )

    target_include_directories(bgrpcconnect_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bgrpcconnect_test PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Test
    )

    add_test(NAME bgrpcconnect_test COMMAND bgrpcconnect_test)

    # bcapabilities_test - capability table, server detection from the login
    # data, notice texts and the rules for showing a notice (no widgets)
    add_executable(bcapabilities_test
        test/bcapabilities_test.cpp
        src/bcapabilities.cpp
        include/bcapabilities.h
    )

    target_include_directories(bcapabilities_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bcapabilities_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bcapabilities_test COMMAND bcapabilities_test)

    # bjobhistorystats_test - measured durations and waits of past job runs:
    # reading the reply of "llist jobs" and the statistics per job and level
    # (no widgets, no Director)
    add_executable(bjobhistorystats_test
        test/bjobhistorystats_test.cpp
        src/schedules/model/bjobhistorystats.cpp
        include/schedules/model/bjobhistorystats.h
    )

    target_include_directories(bjobhistorystats_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bjobhistorystats_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bjobhistorystats_test COMMAND bjobhistorystats_test)

    # bschedulewaitmodel_test - predicted waiting time of scheduled runs: the
    # replay of the Director's job queue (limits, priority, certainty), the
    # limits read from "show" replies, the bound of a run without history
    # (Max Run Time and its relatives) and the texts (no widgets, no Director)
    add_executable(bschedulewaitmodel_test
        test/bschedulewaitmodel_test.cpp
        src/schedules/model/bschedulewaitmodel.cpp
        include/schedules/model/bschedulewaitmodel.h
    )

    target_include_directories(bschedulewaitmodel_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bschedulewaitmodel_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bschedulewaitmodel_test COMMAND bschedulewaitmodel_test)

    # bwaitcalibration_test - the wait prediction measured against the
    # catalog: measured waits, start overhead, pure durations without the
    # wait for the device, the suggestion of parallel writers, the back-test
    # and the fixture built from the production Director's measured table
    add_executable(bwaitcalibration_test
        test/bwaitcalibration_test.cpp
        src/schedules/model/bwaitcalibration.cpp
        src/schedules/model/bschedulewaitmodel.cpp
        src/schedules/model/bjobhistorystats.cpp
        include/schedules/model/bwaitcalibration.h
        include/schedules/model/bschedulewaitmodel.h
        include/schedules/model/bjobhistorystats.h
    )

    target_include_directories(bwaitcalibration_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bwaitcalibration_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bwaitcalibration_test COMMAND bwaitcalibration_test)

    # b321verifier_test - the 3-2-1 rule per backup job: chain, copies
    # (whole, partial, copies of copies, Migrate), media types, off site,
    # the setting for the original data and the worked example on the
    # production Director's volume list (no widgets, no Director)
    add_executable(b321verifier_test
        test/b321verifier_test.cpp
        src/tools/b321verifier.cpp
        src/storage/model/bvolumelist.cpp
        src/models/bvolumejson.cpp
        src/bbyteformat.cpp
        include/tools/b321verifier.h
        include/storage/model/bvolumelist.h
        include/models/bvolumejson.h
        include/bbyteformat.h
    )

    target_include_directories(b321verifier_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(b321verifier_test PRIVATE
        FIXTURE_DIR="${CMAKE_SOURCE_DIR}/test/fixtures"
    )

    target_link_libraries(b321verifier_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME b321verifier_test COMMAND b321verifier_test)
    set_tests_properties(b321verifier_test PROPERTIES ENVIRONMENT "TZ=UTC")

    # b321dialog_test - Tools > 3-2-1 Verifier on the worked example: chips,
    # details, off-site marks and the setting in BSettings (per profile), the
    # read commands of Refresh and nothing else, keyboard and screen reader
    # texts (widgets, offscreen, no Director)
    add_executable(b321dialog_test
        test/b321dialog_test.cpp
        src/tools/b321dialog.cpp
        src/tools/b321verifier.cpp
        src/storage/model/bvolumelist.cpp
        src/models/bvolumejson.cpp
        src/bbyteformat.cpp
        src/bheaderstate.cpp
        src/config/bcolumnconfiguration.cpp
        src/bstatuschip.cpp
        src/bstatusdelegate.cpp
        src/clients/bclientstatus.cpp
        src/buiscale.cpp
        src/config/bsettings.cpp
        include/tools/b321dialog.h
        include/tools/b321verifier.h
        include/storage/model/bvolumelist.h
        include/models/bvolumejson.h
        include/bheaderstate.h
        include/config/bcolumnconfiguration.h
        include/bstatuschip.h
        include/bstatusdelegate.h
        include/buiscale.h
        include/config/bsettings.h
    )

    target_include_directories(b321dialog_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(b321dialog_test PRIVATE
        FIXTURE_DIR="${CMAKE_SOURCE_DIR}/test/fixtures"
        THEME_DIR="${CMAKE_SOURCE_DIR}/resources/themes"
    )

    # bareosdirector_lib: the dialog names commands by BDirector::Command, an
    # enum of the BareosDirector class. Nothing of a Director is created.
    target_link_libraries(b321dialog_test PRIVATE
        bareosdirector_lib
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Test
        OpenSSL::SSL
        OpenSSL::Crypto
    )

    add_test(NAME b321dialog_test COMMAND b321dialog_test)
    set_tests_properties(b321dialog_test PROPERTIES ENVIRONMENT
        "QT_QPA_PLATFORM=offscreen;TZ=UTC;XDG_CONFIG_HOME=${CMAKE_BINARY_DIR}/b321dialog_test_config")

    # bschedulegridmodel_test - the model behind the schedule grid: cells and
    # scopes from Run lines, selection, and the edits back to Run lines
    # (no widgets, no Director)
    add_executable(bschedulegridmodel_test
        test/bschedulegridmodel_test.cpp
        src/schedules/model/bschedulegridmodel.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        include/schedules/model/bschedulegridmodel.h
        include/schedules/model/brunspec.h
    )

    target_include_directories(bschedulegridmodel_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bschedulegridmodel_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bschedulegridmodel_test COMMAND bschedulegridmodel_test)

    # bschedulegridtext_test - what the schedule grid says in words (scope
    # labels, cell names, readings of calendar rules), the dates of its
    # calendar, and where its parts are (no widgets, no Director)
    add_executable(bschedulegridtext_test
        test/bschedulegridtext_test.cpp
        src/schedules/bschedulegridgeometry.cpp
        src/schedules/model/bschedulegridtext.cpp
        src/schedules/model/bschedulegridmodel.cpp
        src/schedules/model/bscheduleoccurrences.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        include/schedules/bschedulegridgeometry.h
        include/schedules/model/bschedulegridtext.h
        include/schedules/model/bschedulegridmodel.h
        include/schedules/model/bscheduleoccurrences.h
        include/schedules/model/brunspec.h
    )

    target_include_directories(bschedulegridtext_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(bschedulegridtext_test PRIVATE
        Qt6::Core
        Qt6::Test
    )

    add_test(NAME bschedulegridtext_test COMMAND bschedulegridtext_test)

    # bschedulegridwidget_test - the schedule grid on screen: selection with
    # mouse and keyboard, the lists beside it, the table for screen readers
    # and a rendering smoke test in both themes (widgets, no Director)
    add_executable(bschedulegridwidget_test
        test/bschedulegridwidget_test.cpp
        src/buiscale.cpp
        src/schedules/bschedulegridaccessible.cpp
        src/schedules/bschedulegriddialogs.cpp
        src/schedules/bschedulegrideditor.cpp
        src/schedules/bschedulegridgeometry.cpp
        src/schedules/bschedulegridview.cpp
        src/schedules/bschedulegridwidget.cpp
        src/schedules/bscheduleminicalendar.cpp
        src/schedules/bschedulepalette.cpp
        src/schedules/brunruledialog.cpp
        src/schedules/model/bschedulegridtext.cpp
        src/schedules/model/bschedulegridmodel.cpp
        src/schedules/model/bscheduleoccurrences.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        include/buiscale.h
        include/schedules/bschedulegridaccessible.h
        include/schedules/bschedulegriddialogs.h
        include/schedules/bschedulegrideditor.h
        include/schedules/bschedulegridgeometry.h
        include/schedules/bschedulegridview.h
        include/schedules/bschedulegridwidget.h
        include/schedules/bscheduleminicalendar.h
        include/schedules/bschedulepalette.h
        include/schedules/brunruledialog.h
        include/schedules/model/bschedulegridtext.h
        include/schedules/model/bschedulegridmodel.h
        include/schedules/model/bscheduleoccurrences.h
        include/schedules/model/brunspec.h
    )

    target_include_directories(bschedulegridwidget_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    # The themes are read from the source tree
    target_compile_definitions(bschedulegridwidget_test PRIVATE
        ONESIMUS_THEME_DIR="${CMAKE_SOURCE_DIR}/resources/themes"
    )

    target_link_libraries(bschedulegridwidget_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test
    )

    add_test(NAME bschedulegridwidget_test COMMAND bschedulegridwidget_test)
    # Widgets need a QApplication; no display is used. To keep pictures of
    # what the test renders, run it with ONESIMUS_GRID_SCREENSHOT_DIR set to
    # a directory.
    set_tests_properties(bschedulegridwidget_test PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

    # bschedulegridedit_test - editing in the schedule grid: brushes and the
    # palette, painting and erasing with mouse and keyboard, undo and redo
    # back to the reported Run lines, scopes, the calendar rule form, lines
    # that are not understood, a refresh while there are changes (widgets,
    # no Director)
    add_executable(bschedulegridedit_test
        test/bschedulegridedit_test.cpp
        src/buiscale.cpp
        src/schedules/bschedulegridaccessible.cpp
        src/schedules/bschedulegriddialogs.cpp
        src/schedules/bschedulegrideditor.cpp
        src/schedules/bschedulegridgeometry.cpp
        src/schedules/bschedulegridview.cpp
        src/schedules/bschedulegridwidget.cpp
        src/schedules/bscheduleminicalendar.cpp
        src/schedules/bschedulepalette.cpp
        src/schedules/brunruledialog.cpp
        src/schedules/model/bschedulegridtext.cpp
        src/schedules/model/bschedulegridmodel.cpp
        src/schedules/model/bscheduleoccurrences.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        include/buiscale.h
        include/schedules/bschedulegridaccessible.h
        include/schedules/bschedulegriddialogs.h
        include/schedules/bschedulegrideditor.h
        include/schedules/bschedulegridgeometry.h
        include/schedules/bschedulegridview.h
        include/schedules/bschedulegridwidget.h
        include/schedules/bscheduleminicalendar.h
        include/schedules/bschedulepalette.h
        include/schedules/brunruledialog.h
        include/schedules/model/bschedulegridtext.h
        include/schedules/model/bschedulegridmodel.h
        include/schedules/model/bscheduleoccurrences.h
        include/schedules/model/brunspec.h
    )

    target_include_directories(bschedulegridedit_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(bschedulegridedit_test PRIVATE
        ONESIMUS_THEME_DIR="${CMAKE_SOURCE_DIR}/resources/themes"
    )

    target_link_libraries(bschedulegridedit_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test
    )

    add_test(NAME bschedulegridedit_test COMMAND bschedulegridedit_test)
    # As above; pictures with ONESIMUS_GRID_SCREENSHOT_DIR
    set_tests_properties(bschedulegridedit_test PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

    # buiscale_test - font sizes and the interface zoom: roles relative to
    # the base font, clamping, pixel measures, a live change, the stored
    # value in BSettings, and the measures of the schedule timeline at
    # 100 % and 200 % (widgets, no Director)
    add_executable(buiscale_test
        test/buiscale_test.cpp
        src/buiscale.cpp
        src/config/bsettings.cpp
        src/schedules/bscheduleganttwidget.cpp
        src/schedules/model/bjobhistorystats.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        src/schedules/model/bscheduleoccurrences.cpp
        src/models/bresourcemodels.cpp
        src/models/bbasemodels.cpp
        src/config/bconfigparser.cpp
        include/buiscale.h
        include/config/bsettings.h
        include/schedules/bscheduleganttwidget.h
        include/schedules/model/bjobhistorystats.h
        include/models/bresourcemodels.h
        include/models/bbasemodels.h
        include/config/bconfigparser.h
    )

    target_include_directories(buiscale_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(buiscale_test PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test
    )

    add_test(NAME buiscale_test COMMAND buiscale_test)

    # bdialogfonts_test - dialogs and other windows of their own take the
    # interface font and zoom: a plain dialog, a message box and the real
    # job details, settings, connection and restore dialogs at 100 % and
    # 200 %, a zoom change while they are open, per-class fonts of the
    # platform theme, and a theme style sheet with sizes of its own (widgets,
    # no Director, no network). Built from the sources of the application.
    set(BDIALOGFONTS_SOURCES ${SOURCES})
    list(REMOVE_ITEM BDIALOGFONTS_SOURCES src/main.cpp)
    add_executable(bdialogfonts_test
        test/bdialogfonts_test.cpp
        ${BDIALOGFONTS_SOURCES}
        ${HEADERS}
        ${UI_FILES}
    )
    if(HAVE_GRPC)
        target_sources(bdialogfonts_test PRIVATE
            src/director/bgrpctransport.cpp
            include/director/bgrpctransport.h
            src/director/bgrpcconnect.cpp
            include/director/bgrpcconnect.h
            src/director/bconfigproto.cpp
            include/director/bconfigproto.h
        )
    endif()
    target_include_directories(bdialogfonts_test PRIVATE
        $<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>
    )
    target_compile_definitions(bdialogfonts_test PRIVATE
        ONESIMUS_THEME_DIR="${CMAKE_SOURCE_DIR}/resources/themes"
    )
    # The sources of the application are compiled a second time here; their
    # known warnings (OpenSSL's MD5 in bareosauth.cpp) are counted once, for
    # the application
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(bdialogfonts_test PRIVATE -Wno-deprecated-declarations)
    endif()
    target_link_libraries(bdialogfonts_test PRIVATE
        $<TARGET_PROPERTY:${PROJECT_NAME},LINK_LIBRARIES>
        Qt6::Test
    )
    add_test(NAME bdialogfonts_test COMMAND bdialogfonts_test)
    # To keep pictures of the dialogs, run it with
    # ONESIMUS_DIALOG_SCREENSHOT_DIR set to a directory.
    set_tests_properties(bdialogfonts_test PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

    # bscheduletimeline_test - the schedule tab's timeline as a whole: a bar
    # that is dragged and dropped sends no command, asks the capability gate
    # and says so in the status line; schedules no job uses are left out of
    # the prediction, the counter and the list; the counter says whether
    # its list is open; waits of seconds (widgets, no Director)
    add_executable(bscheduletimeline_test
        test/bscheduletimeline_test.cpp
        test/bfakeconfigclient.h
        test/bfakesaveui.h
        src/buiscale.cpp
        src/config/bsettings.cpp
        src/schedules/bschedulewidget.cpp
        src/schedules/bschedulelegend.cpp
        src/schedules/bscheduleganttwidget.cpp
        src/schedules/bscheduledragplan.cpp
        src/schedules/bschedulepersistence.cpp
        include/schedules/bschedulepersistence.h
        include/schedules/bschedulereviewdialog.h
        include/director/bconfigclient.h
        src/schedules/bschedulereviewdialog.cpp
        src/schedules/model/bscheduleresource.cpp
        src/config/bresourcetext.cpp
        src/config/bresourcepersistence.cpp
        src/config/bresourcereviewdialog.cpp
        include/config/bresourcetext.h
        include/config/bresourcepersistence.h
        include/config/bresourcereviewdialog.h
        src/schedules/bjobscheduleindex.cpp
        src/schedules/bschedulegridaccessible.cpp
        src/schedules/bschedulegriddialogs.cpp
        src/schedules/bschedulegrideditor.cpp
        src/schedules/bschedulegridgeometry.cpp
        src/schedules/bschedulegridview.cpp
        src/schedules/bschedulegridwidget.cpp
        src/schedules/bscheduleminicalendar.cpp
        src/schedules/bschedulepalette.cpp
        src/schedules/brunruledialog.cpp
        src/schedules/model/bschedulegridtext.cpp
        src/schedules/model/bschedulegridmodel.cpp
        src/schedules/model/bscheduleoccurrences.cpp
        src/schedules/model/bjobhistorystats.cpp
        src/schedules/model/bschedulewaitmodel.cpp
        src/schedules/model/bwaitcalibration.cpp
        src/schedules/model/brunparser.cpp
        src/schedules/model/brunwriter.cpp
        src/models/bresourcemodels.cpp
        src/models/bbasemodels.cpp
        src/config/bconfigparser.cpp
        include/buiscale.h
        include/config/bsettings.h
        include/schedules/bschedulewidget.h
        include/schedules/bschedulelegend.h
        include/schedules/bscheduleganttwidget.h
        include/schedules/bjobscheduleindex.h
        include/schedules/bschedulegridaccessible.h
        include/schedules/bschedulegriddialogs.h
        include/schedules/bschedulegrideditor.h
        include/schedules/bschedulegridgeometry.h
        include/schedules/bschedulegridview.h
        include/schedules/bschedulegridwidget.h
        include/schedules/bscheduleminicalendar.h
        include/schedules/bschedulepalette.h
        include/schedules/brunruledialog.h
        include/schedules/model/bschedulegridtext.h
        include/schedules/model/bschedulegridmodel.h
        include/schedules/model/bscheduleoccurrences.h
        include/schedules/model/bjobhistorystats.h
        include/schedules/model/bschedulewaitmodel.h
        include/schedules/model/bwaitcalibration.h
        include/schedules/model/brunspec.h
        include/models/bresourcemodels.h
        include/models/bbasemodels.h
        include/config/bconfigparser.h
    )

    target_include_directories(bscheduletimeline_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/test
    )

    # bareosdirector_lib: the schedule tab names commands by
    # BDirector::Command, an enum of the BareosDirector class. Nothing of a
    # Director is created or called.
    target_link_libraries(bscheduletimeline_test PRIVATE
        bareosdirector_lib
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Test
        OpenSSL::SSL
        OpenSSL::Crypto
    )

    add_test(NAME bscheduletimeline_test COMMAND bscheduletimeline_test)
    # Widgets need a QApplication; no display is used. The settings go to
    # the build directory, never to those of the user (the test redirects
    # them itself as well).
    set_tests_properties(bscheduletimeline_test PROPERTIES ENVIRONMENT
        "QT_QPA_PLATFORM=offscreen;XDG_CONFIG_HOME=${CMAKE_BINARY_DIR}/bscheduletimeline_test_config")
    # Widgets need a QApplication; no display is used. The settings the test
    # writes go to the build directory, never to those of the user (the
    # test redirects them itself as well).
    set_tests_properties(buiscale_test PROPERTIES ENVIRONMENT
        "QT_QPA_PLATFORM=offscreen;XDG_CONFIG_HOME=${CMAKE_BINARY_DIR}/buiscale_test_config")
endif()

message(STATUS "")
message(STATUS "Test executables:")
message(STATUS "  bareosauth_test  - BareosAuth class test")
message(STATUS "  director_test    - BareosDirector state machine test")
message(STATUS "  volumejson_test  - list volumes JSON parsing test")
if(BUILD_TESTING)
    message(STATUS "  bbyteformat_test - byte formatting (IEC units) and Bareos size parsing test (registered with CTest)")
    message(STATUS "  bvolumelist_test - list/llist volumes reply model test (registered with CTest)")
    message(STATUS "  brecycleforecast_test - volume \"reusable from\" forecast test (registered with CTest)")
    message(STATUS "  storagewidget_test - Storage/Volumes tab test, offscreen (registered with CTest)")
    message(STATUS "  bjoblogmodel_test - job log model test (registered with CTest)")
    message(STATUS "  brunjobcommand_test - run command test (registered with CTest)")
    message(STATUS "  bcleanup_test    - cleanup selection and sequencing test (registered with CTest)")
    message(STATUS "  bdatabase_test   - database migration test (registered with CTest)")
    message(STATUS "  bsortvalue_test  - table sort key test (registered with CTest)")
    message(STATUS "  bclientstatus_test - client last-backup state test (registered with CTest)")
    message(STATUS "  bjobpaging_test  - job list page arithmetic test (registered with CTest)")
    message(STATUS "  bdirectorreply_test - reload reply test (registered with CTest)")
    message(STATUS "  brunparser_test  - schedule Run line parser and writer test (registered with CTest)")
    message(STATUS "  blogredact_test  - log redaction and log file permission test (registered with CTest)")
    message(STATUS "  bnetframe_test   - BNET frame length limit test (registered with CTest)")
    message(STATUS "  bcommandlineconnection_test - command line TLS/password option test (registered with CTest)")
    message(STATUS "  bdirectorqueue_test - command queue test without a connection (registered with CTest)")
    message(STATUS "  bscheduledragplan_test - schedule drag Run line comparison test (registered with CTest)")
    message(STATUS "  bscheduletimeline_test - schedule timeline test: drop without command, unused schedules, offscreen (registered with CTest)")
    message(STATUS "  bscheduleoccurrences_test - schedule run dates and overlap test (registered with CTest)")
    message(STATUS "  bmessagesmodel_test - Director messages model test (registered with CTest)")
    message(STATUS "  bschedulepersistence_test - schedule resource text, save flow and request bytes test (registered with CTest)")
    message(STATUS "  bgrpcconnect_test - gRPC address order, TLS probe and failure text test (registered with CTest)")
    message(STATUS "  bcapabilities_test - Director capability table and notice test (registered with CTest)")
    message(STATUS "  bjobhistorystats_test - job history reply and statistics test (registered with CTest)")
    message(STATUS "  bschedulewaitmodel_test - predicted waiting time of scheduled runs test (registered with CTest)")
    message(STATUS "  bwaitcalibration_test - wait prediction against the catalog test (registered with CTest)")
    message(STATUS "  b321verifier_test - 3-2-1 rule per backup job test (registered with CTest)")
    message(STATUS "  b321dialog_test  - 3-2-1 verifier window test, offscreen (registered with CTest)")
    message(STATUS "  bschedulegridmodel_test - schedule grid model test (registered with CTest)")
    message(STATUS "  bschedulegridtext_test - schedule grid texts, calendar and geometry test (registered with CTest)")
    message(STATUS "  bschedulegridwidget_test - schedule grid widget test, offscreen (registered with CTest)")
    message(STATUS "  bschedulegridedit_test - schedule grid editing test, offscreen (registered with CTest)")
    message(STATUS "  buiscale_test    - font roles, interface zoom and timeline measures test, offscreen (registered with CTest)")
endif()
message(STATUS "")

# ============================================================================
# CPack Configuration (Package Generation)
# ============================================================================

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Bernau Family")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Onesimus - Bareos Management Tool (Early Access — not for production use)")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}~earlyaccess")
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
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resources/icons/onesimus.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/resources/icons/onesimus.ico")

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
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Joerg Bernau <support@onesimus.io>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "admin")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/Beerlesklopfer/Onesimus")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
        "Onesimus - Bareos/Bacula Backup Management Tool (Early Access)\n WARNING: This is a pre-release version under active development.\n Not suitable for production use. Features may change.\n A Qt6-based graphical management console for Bareos and Bacula\n backup systems. Provides client management, job monitoring,\n schedule visualization, and restore wizard.")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS
        "libqt6core6 (>= ${ONESIMUS_QT_MIN_VERSION}), libqt6gui6 (>= ${ONESIMUS_QT_MIN_VERSION}), libqt6widgets6 (>= ${ONESIMUS_QT_MIN_VERSION}), libqt6network6 (>= ${ONESIMUS_QT_MIN_VERSION})")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

    # RPM package settings
    set(CPACK_RPM_PACKAGE_LICENSE "AGPL-3.0-or-later")
    set(CPACK_RPM_PACKAGE_GROUP "Applications/System")
    set(CPACK_RPM_PACKAGE_REQUIRES "qt6-qtbase >= ${ONESIMUS_QT_MIN_VERSION}")
    set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
endif()

# include(CPack) must come AFTER all CPACK_* variables are set
include(CPack)
