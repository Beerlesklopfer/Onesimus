cmake_minimum_required(VERSION 3.16)
project(Onesimus VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# UI-Verzeichnis für AUTOUIC angeben
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

# ============================================================================
# Backup System Auswahl
# ============================================================================
set(BACKUP_SYSTEM "BACULA" CACHE STRING "Backup system to support: BACULA, BAREOS, or BOTH")
set_property(CACHE BACKUP_SYSTEM PROPERTY STRINGS "BACULA" "BAREOS" "BOTH")

message(STATUS "")
message(STATUS "========================================")
message(STATUS "Backup System Configuration")
message(STATUS "========================================")
message(STATUS "Selected system: ${BACKUP_SYSTEM}")

# Setze Compiler-Definitionen basierend auf Auswahl
if(BACKUP_SYSTEM STREQUAL "BACULA")
    add_compile_definitions(USE_BACULA)
    set(USE_BACULA_ONLY TRUE)
    message(STATUS "Building with: Bacula support only")
elseif(BACKUP_SYSTEM STREQUAL "BAREOS")
    add_compile_definitions(USE_BAREOS)
    set(USE_BAREOS_ONLY TRUE)
    message(STATUS "Building with: Bareos support only")
elseif(BACKUP_SYSTEM STREQUAL "BOTH")
    add_compile_definitions(USE_BACULA USE_BAREOS)
    set(USE_BOTH_SYSTEMS TRUE)
    message(STATUS "Building with: Bacula AND Bareos support")
else()
    message(FATAL_ERROR "Invalid BACKUP_SYSTEM: ${BACKUP_SYSTEM}. Must be BACULA, BAREOS, or BOTH")
endif()

message(STATUS "========================================")
message(STATUS "")

# Option für statisches OpenSSL
option(USE_STATIC_OPENSSL "Use static OpenSSL from git submodule (auto-download)" ON)

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
        
        # Bibliotheksnamen
        set(OPENSSL_SSL_LIB ${OPENSSL_INSTALL_DIR}/lib/libssl.a)
        set(OPENSSL_CRYPTO_LIB ${OPENSSL_INSTALL_DIR}/lib/libcrypto.a)
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
    message(STATUS "Using system OpenSSL")
    find_package(OpenSSL REQUIRED)
    
    if(OPENSSL_FOUND)
        message(STATUS "✓ Found OpenSSL ${OPENSSL_VERSION}")
        message(STATUS "  Includes: ${OPENSSL_INCLUDE_DIR}")
        message(STATUS "  Libraries: ${OPENSSL_LIBRARIES}")
    endif()
endif()

# ============================================================================
# Qt6 Configuration
# ============================================================================

# Finde Qt6 Core zuerst
find_package(Qt6 REQUIRED COMPONENTS Core)

# Qt6 Standard Project Setup (empfohlen ab Qt 6.3+)
qt_standard_project_setup()

# Finde restliche Qt6 Komponenten
find_package(Qt6 REQUIRED COMPONENTS
    Gui
    Widgets
    Network
    Sql
)

message(STATUS "✓ Found Qt6 ${Qt6_VERSION}")

# ============================================================================
# Project Files
# ============================================================================

# Include-Verzeichnisse
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Header-Dateien
set(HEADERS
    include/mainwindow.h
    include/jobwidget.h
    include/clientwidget.h
    include/storagewidget.h
    include/settingsdialog.h
)

# Bacula-spezifische Dateien
if(USE_BACULA_ONLY OR USE_BOTH_SYSTEMS)
    list(APPEND HEADERS include/baculadirector.h)
endif()

# Bareos-spezifische Dateien
if(USE_BAREOS_ONLY OR USE_BOTH_SYSTEMS)
    list(APPEND HEADERS include/bareosdirector.h)
endif()

# Quell-Dateien
set(SOURCES
    src/main.cpp    
    src/mainwindow.cpp
    src/jobwidget.cpp
    src/clientwidget.cpp
    src/storagewidget.cpp
    src/settingsdialog.cpp
)

# Bacula-spezifische Dateien
if(USE_BACULA_ONLY OR USE_BOTH_SYSTEMS)
    list(APPEND SOURCES src/baculadirector.cpp)
    # list(APPEND HEADERS include/authenticatebase.h)
    # Präprozessor-Definition setzen
    add_compile_definitions(USE_BACULA_ONLY)
endif()

# Bareos-spezifische Dateien
if(USE_BAREOS_ONLY OR USE_BOTH_SYSTEMS)
    list(APPEND SOURCES src/bareosdirector.cpp)
    # Präprozessor-Definition setzen
    add_compile_definitions(USE_BAREOS)
endif()

# UI-Dateien
set(UI_FILES
    ui/mainwindow.ui
    ui/jobwidget.ui
    ui/clientwidget.ui
    ui/storagewidget.ui
    ui/settingsdialog.ui
)

# ============================================================================
# Executable Target
# ============================================================================

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

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
message(STATUS "Backup System: ${BACKUP_SYSTEM}")
if(USE_BACULA_ONLY)
    message(STATUS "  Support:     Bacula only")
elseif(USE_BAREOS_ONLY)
    message(STATUS "  Support:     Bareos only")
elseif(USE_BOTH_SYSTEMS)
    message(STATUS "  Support:     Bacula AND Bareos")
endif()
message(STATUS "")
message(STATUS "Qt Version:    ${Qt6_VERSION}")
message(STATUS "C++ Standard:  ${CMAKE_CXX_STANDARD}")
message(STATUS "")
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
