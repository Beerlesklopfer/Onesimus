#!/bin/bash
#
# director_test.sh - Automated BareosDirector state machine test script
#
# Usage: ./director_test.sh [options]
#
# Options:
#   -m, --mode <mode>     Test mode: legacy, psk, all (default: legacy)
#   -n, --count <n>       Number of test iterations (default: 5)
#   -t, --test <test>     Test to run: connect, status, jobs, clients, all (default: all)
#   -v, --verbose         Verbose output
#   -h, --help            Show this help
#
# Requirements:
#   - director_test must be built in ../build/
#   - Director console configs must be installed
#   - Password must be set in BAREOS_PASSWORD environment variable
#
# Example:
#   BAREOS_PASSWORD=mypassword ./director_test.sh -m legacy -t connect -n 10
#

# Default values
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/../build"
TEST_BIN="${BUILD_DIR}/director_test"

MODE="legacy"
COUNT=5
TEST="all"
VERBOSE=""
HOST="localhost"
PORT=9101
DIRECTOR="bareos-dir"

# Console names for each mode
CONSOLE_LEGACY="onesimus"
CONSOLE_PSK="onesimus-psk"

# Password (can be overridden by environment variable)
PASSWORD="${BAREOS_PASSWORD:-}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

usage() {
    cat << EOF
Usage: $0 [options]

Options:
  -m, --mode <mode>     Auth mode: legacy, psk, all (default: legacy)
  -n, --count <n>       Number of test iterations (default: 5)
  -t, --test <test>     Test to run: connect, status, jobs, clients, all (default: all)
  -p, --password <pwd>  Password (or set BAREOS_PASSWORD env var)
  -H, --host <host>     Director host (default: localhost)
  -P, --port <port>     Director port (default: 9101)
  -v, --verbose         Verbose output
  -h, --help            Show this help

Environment:
  BAREOS_PASSWORD       Password for authentication

Tests:
  connect   - Test connection and authentication only
  status    - Test status director command
  jobs      - Test .jobs command
  clients   - Test .clients command
  all       - Run all tests

Example:
  BAREOS_PASSWORD=mypassword $0 -m legacy -t connect -n 10
  $0 -m all -t all -p mypassword -n 3
EOF
}

log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

log_test() {
    echo -e "${BLUE}[TEST]${NC} $1"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -m|--mode)
            MODE="$2"
            shift 2
            ;;
        -n|--count)
            COUNT="$2"
            shift 2
            ;;
        -t|--test)
            TEST="$2"
            shift 2
            ;;
        -p|--password)
            PASSWORD="$2"
            shift 2
            ;;
        -H|--host)
            HOST="$2"
            shift 2
            ;;
        -P|--port)
            PORT="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE="--verbose"
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            log_error "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
done

# Validate
if [[ -z "$PASSWORD" ]]; then
    log_error "Password is required. Set BAREOS_PASSWORD or use -p option."
    exit 1
fi

if [[ ! -x "$TEST_BIN" ]]; then
    log_error "Test binary not found: $TEST_BIN"
    log_info "Build it first: cd ${BUILD_DIR} && make director_test"
    exit 1
fi

# Single test function
run_single_test() {
    local mode=$1
    local console=$2
    local test_cmd=$3
    local mode_flag=$4

    if $TEST_BIN --host "$HOST" -p "$PORT" -d "$DIRECTOR" \
                 -c "$console" -P "$PASSWORD" $mode_flag \
                 $VERBOSE "$test_cmd" 2>&1 | grep -q "RESULT: PASSED"; then
        return 0
    else
        return 1
    fi
}

# Test function
run_test() {
    local mode=$1
    local console=$2
    local test_cmd=$3
    local mode_flag=$4
    local success=0
    local fail=0

    echo ""
    echo "========================================"
    echo "Test: $test_cmd (Mode: $mode, $COUNT iterations)"
    echo "Console: $console"
    echo "========================================"

    for i in $(seq 1 $COUNT); do
        if run_single_test "$mode" "$console" "$test_cmd" "$mode_flag"; then
            ((success++))
            echo -n -e "${GREEN}✓${NC}"
        else
            ((fail++))
            echo -n -e "${RED}✗${NC}"
        fi
        sleep 0.5
    done

    echo ""
    echo "Results: $success/$COUNT success, $fail failed"

    if [[ $fail -gt 0 ]]; then
        return 1
    fi
    return 0
}

# Run all tests for a mode
run_mode_tests() {
    local mode=$1
    local console=$2
    local mode_flag=$3
    local tests_passed=0
    local tests_failed=0

    echo ""
    echo "========================================"
    echo "Testing Mode: $mode"
    echo "========================================"

    local test_list=("connect")
    if [[ "$TEST" == "all" ]]; then
        test_list=("connect" "status" "jobs" "clients")
    elif [[ "$TEST" != "connect" ]]; then
        test_list=("$TEST")
    fi

    for test_cmd in "${test_list[@]}"; do
        log_test "Running $test_cmd test..."
        if run_test "$mode" "$console" "$test_cmd" "$mode_flag"; then
            ((tests_passed++))
        else
            ((tests_failed++))
        fi
    done

    echo ""
    echo "Mode $mode: $tests_passed tests passed, $tests_failed failed"

    if [[ $tests_failed -gt 0 ]]; then
        return 1
    fi
    return 0
}

# Main
echo "========================================"
echo "BareosDirector State Machine Test Suite"
echo "========================================"
echo "Host: $HOST:$PORT"
echo "Director: $DIRECTOR"
echo "Mode: $MODE"
echo "Test: $TEST"
echo "Iterations: $COUNT"
echo "========================================"

TOTAL_MODES_PASSED=0
TOTAL_MODES_FAILED=0

case $MODE in
    legacy)
        if run_mode_tests "legacy" "$CONSOLE_LEGACY" "--legacy"; then
            ((TOTAL_MODES_PASSED++))
        else
            ((TOTAL_MODES_FAILED++))
        fi
        ;;
    psk)
        if run_mode_tests "psk" "$CONSOLE_PSK" "--psk"; then
            ((TOTAL_MODES_PASSED++))
        else
            ((TOTAL_MODES_FAILED++))
        fi
        ;;
    all)
        # Test all modes
        if run_mode_tests "legacy" "$CONSOLE_LEGACY" "--legacy"; then
            ((TOTAL_MODES_PASSED++))
        else
            ((TOTAL_MODES_FAILED++))
        fi

        if run_mode_tests "psk" "$CONSOLE_PSK" "--psk"; then
            ((TOTAL_MODES_PASSED++))
        else
            ((TOTAL_MODES_FAILED++))
        fi
        ;;
    *)
        log_error "Unknown mode: $MODE"
        usage
        exit 1
        ;;
esac

echo ""
echo "========================================"
echo "SUMMARY"
echo "========================================"

if [[ $TOTAL_MODES_FAILED -eq 0 ]]; then
    log_info "All test modes passed!"
    exit 0
else
    log_error "$TOTAL_MODES_FAILED mode(s) had failures"
    exit 1
fi
