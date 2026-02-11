# IncrementBuildNumber.cmake
# Increments the build number and generates version.h from template
#
# Expected variables (passed via -D):
#   SOURCE_DIR, BINARY_DIR, TEMPLATE_FILE, OUTPUT_FILE
#   PROJECT_VERSION, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH
#   PROJECT_NAME, CMAKE_SYSTEM_NAME, CMAKE_SYSTEM_PROCESSOR
#   CMAKE_BUILD_TYPE, CMAKE_CXX_COMPILER_ID, CMAKE_CXX_COMPILER_VERSION

# ============================================================================
# Read and increment build number
# ============================================================================
set(BUILD_NUMBER_FILE "${SOURCE_DIR}/.build_number")

if(EXISTS "${BUILD_NUMBER_FILE}")
    file(READ "${BUILD_NUMBER_FILE}" BUILD_NUMBER)
    string(STRIP "${BUILD_NUMBER}" BUILD_NUMBER)
    math(EXPR BUILD_NUMBER "${BUILD_NUMBER} + 1")
else()
    set(BUILD_NUMBER 1)
endif()

file(WRITE "${BUILD_NUMBER_FILE}" "${BUILD_NUMBER}")

# ============================================================================
# Get current date/time
# ============================================================================
string(TIMESTAMP BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP BUILD_TIME "%H:%M:%S")
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
string(TIMESTAMP BDATE "%Y.%m")

# ============================================================================
# Get git information
# ============================================================================
find_package(Git QUIET)

if(GIT_FOUND)
    # Git commit hash (short)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY "${SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # Git branch name
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY "${SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # Git describe (includes dirty status)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --always --dirty
        WORKING_DIRECTORY "${SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DESCRIBE
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
else()
    set(GIT_COMMIT_HASH "unknown")
    set(GIT_BRANCH "unknown")
    set(GIT_DESCRIBE "unknown")
endif()

# ============================================================================
# Get distribution information (Linux only)
# ============================================================================
set(DISTNAME "")
set(DISTVER "")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Try /etc/os-release first (modern standard)
    if(EXISTS "/etc/os-release")
        file(STRINGS "/etc/os-release" OS_RELEASE_CONTENTS)
        foreach(LINE ${OS_RELEASE_CONTENTS})
            if(LINE MATCHES "^NAME=\"?([^\"]+)\"?")
                set(DISTNAME "${CMAKE_MATCH_1}")
            elseif(LINE MATCHES "^VERSION_ID=\"?([^\"]+)\"?")
                set(DISTVER "${CMAKE_MATCH_1}")
            endif()
        endforeach()
    # Fallback to lsb_release
    elseif(EXISTS "/usr/bin/lsb_release")
        execute_process(
            COMMAND /usr/bin/lsb_release -si
            OUTPUT_VARIABLE DISTNAME
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        execute_process(
            COMMAND /usr/bin/lsb_release -sr
            OUTPUT_VARIABLE DISTVER
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(DISTNAME "Windows")
    # Get Windows version
    execute_process(
        COMMAND cmd /c ver
        OUTPUT_VARIABLE WIN_VER
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(WIN_VER MATCHES "([0-9]+\\.[0-9]+)")
        set(DISTVER "${CMAKE_MATCH_1}")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(DISTNAME "macOS")
    execute_process(
        COMMAND sw_vers -productVersion
        OUTPUT_VARIABLE DISTVER
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif()

# ============================================================================
# Configure the template file
# ============================================================================
configure_file(
    "${TEMPLATE_FILE}"
    "${OUTPUT_FILE}"
    @ONLY
)

message(STATUS "Build number incremented to ${BUILD_NUMBER}")
