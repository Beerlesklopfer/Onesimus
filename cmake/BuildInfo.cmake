# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# BuildInfo.cmake - writes <build>/generated/buildinfo.cpp and version.txt
#
# Runs as a script (cmake -P) on every build, from the onesimus_buildinfo
# target. It writes only into the build directory and rewrites a file only
# when its content changed, so a build without source changes compiles
# nothing and the source tree stays clean (GitHub issue #7).
#
# Expected variables (passed via -D):
#   SOURCE_DIR, BINARY_DIR, TEMPLATE_FILE, OUTPUT_FILE, VERSION_FILE,
#   PROJECT_VERSION, INCREMENT (ON: count a build number up in BINARY_DIR)

# ============================================================================
# Git state
# ============================================================================
# Under sudo (sudo make install) or in a tree without .git, git fails. Then an
# existing buildinfo.cpp is kept as it is: no root-owned rebuild of the
# program, no "unknown" replacing the real values.
set(GIT_OK FALSE)
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY "${SOURCE_DIR}"
        RESULT_VARIABLE _git_result
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(_git_result EQUAL 0 AND GIT_COMMIT_HASH)
        set(GIT_OK TRUE)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY "${SOURCE_DIR}"
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --always --dirty
            WORKING_DIRECTORY "${SOURCE_DIR}"
            OUTPUT_VARIABLE GIT_DESCRIBE
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --format=%ct
            WORKING_DIRECTORY "${SOURCE_DIR}"
            OUTPUT_VARIABLE GIT_COMMIT_EPOCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
            WORKING_DIRECTORY "${SOURCE_DIR}"
            OUTPUT_VARIABLE GIT_COMMIT_COUNT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
    endif()
endif()

if(NOT GIT_OK)
    if(EXISTS "${OUTPUT_FILE}" AND NOT INCREMENT)
        return()
    endif()
    set(GIT_COMMIT_HASH "unknown")
    set(GIT_BRANCH "unknown")
    set(GIT_DESCRIBE "unknown")
    set(GIT_COMMIT_EPOCH "")
    set(GIT_COMMIT_COUNT "")
endif()

# ============================================================================
# Build number
# ============================================================================
#   INCREMENT ON: a counter in the build directory, counted up on every build
#     (seeded from an untracked .build_number in the source tree, if any)
#   otherwise: the CI run number, else the number of commits up to HEAD,
#     else an untracked .build_number, else 0
if(INCREMENT)
    set(_counter "${BINARY_DIR}/build_number")
    if(EXISTS "${_counter}")
        file(READ "${_counter}" BUILD_NUMBER)
    elseif(EXISTS "${SOURCE_DIR}/.build_number")
        file(READ "${SOURCE_DIR}/.build_number" BUILD_NUMBER)
    else()
        set(BUILD_NUMBER 0)
    endif()
    string(STRIP "${BUILD_NUMBER}" BUILD_NUMBER)
    math(EXPR BUILD_NUMBER "${BUILD_NUMBER} + 1")
    file(WRITE "${_counter}" "${BUILD_NUMBER}\n")
elseif("$ENV{GITHUB_RUN_NUMBER}" MATCHES "^[0-9]+$")
    set(BUILD_NUMBER "$ENV{GITHUB_RUN_NUMBER}")
elseif(GIT_COMMIT_COUNT MATCHES "^[0-9]+$")
    set(BUILD_NUMBER "${GIT_COMMIT_COUNT}")
elseif(EXISTS "${SOURCE_DIR}/.build_number")
    file(READ "${SOURCE_DIR}/.build_number" BUILD_NUMBER)
    string(STRIP "${BUILD_NUMBER}" BUILD_NUMBER)
else()
    set(BUILD_NUMBER 0)
endif()
if(NOT BUILD_NUMBER MATCHES "^[0-9]+$")
    set(BUILD_NUMBER 0)
endif()

# ============================================================================
# Source date (UTC): SOURCE_DATE_EPOCH, else the commit date of HEAD
# ============================================================================
# Not the time of the build: that would change the file on every build.
# string(TIMESTAMP) reads SOURCE_DATE_EPOCH itself.
if(NOT DEFINED ENV{SOURCE_DATE_EPOCH} AND GIT_COMMIT_EPOCH MATCHES "^[0-9]+$")
    set(ENV{SOURCE_DATE_EPOCH} "${GIT_COMMIT_EPOCH}")
endif()
string(TIMESTAMP SOURCE_DATE "%Y-%m-%d" UTC)
string(TIMESTAMP SOURCE_TIME "%H:%M:%S" UTC)

# ============================================================================
# Write (configure_file/file(CONFIGURE) leave an unchanged file untouched)
# ============================================================================
configure_file("${TEMPLATE_FILE}" "${OUTPUT_FILE}" @ONLY)
file(CONFIGURE OUTPUT "${VERSION_FILE}" CONTENT "${PROJECT_VERSION}.${BUILD_NUMBER}\n" @ONLY)
