#!/bin/bash
#
# run_all_tests.sh - Comprehensive test runner for Onesimus
#
# Runs all authentication and director tests, outputs results to test_results.log
#
# Usage: ./test/run_all_tests.sh
#

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"
OUTPUT_FILE="$SCRIPT_DIR/test_results.log"

# Test configuration
TEST_PORT="19101"
DEBUG_LEVEL="50"

# Test credentials
TEST_PSK_CONSOLE="test-psk"
TEST_PSK_PASSWORD="TestPSKPassword123"
TEST_CERT_CONSOLE="test-cert"
TEST_CERT_PASSWORD="TestCertPassword123"
TEST_LEGACY_CONSOLE="test-legacy"
TEST_LEGACY_PASSWORD="TestLegacyPassword123"
ONESIMUS_CONSOLE="onesimus"
ONESIMUS_PASSWORD="RDZTTAJE5pj5O/C2uOi01o8FRsqucVPolersd3AbXBs"

# TLS certificate paths
TLS_DIR="$SCRIPT_DIR/bareos-dir.d/tls"

# Counters
TOTAL_PASSED=0
TOTAL_FAILED=0
TOTAL_SKIPPED=0

# Colors for terminal output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Log to both terminal and file
log() {
    echo -e "$1"
    # Strip ANSI colors for log file
    echo -e "$1" | sed 's/\x1b\[[0-9;]*m//g' >> "$OUTPUT_FILE"
}

log_header() {
    log ""
    log "========================================"
    log " $1"
    log "========================================"
}

log_test() {
    log ""
    log "--- $1 ---"
}

log_result() {
    local name="$1"
    local result="$2"
    local details="$3"

    if [ "$result" = "PASS" ]; then
        log "  ${GREEN}PASS${NC} $name"
        ((TOTAL_PASSED++))
    elif [ "$result" = "FAIL" ]; then
        log "  ${RED}FAIL${NC} $name"
        [ -n "$details" ] && log "       $details"
        ((TOTAL_FAILED++))
    else
        log "  ${YELLOW}SKIP${NC} $name"
        [ -n "$details" ] && log "       $details"
        ((TOTAL_SKIPPED++))
    fi
}

# Initialize output file
init_output() {
    cat > "$OUTPUT_FILE" << EOF
================================================================================
ONESIMUS TEST RESULTS
================================================================================
Date: $(date '+%Y-%m-%d %H:%M:%S')
Host: $(hostname)
User: $(whoami)
Project: $PROJECT_DIR
================================================================================

EOF
}

# Check prerequisites
check_prerequisites() {
    log_header "PREREQUISITES CHECK"

    local prereq_ok=true

    # Check build directory
    if [ -d "$BUILD_DIR" ]; then
        log_result "Build directory exists" "PASS"
    else
        log_result "Build directory exists" "FAIL" "$BUILD_DIR not found"
        prereq_ok=false
    fi

    # Check test binaries
    for bin in bareosauth_test director_test; do
        if [ -x "$BUILD_DIR/$bin" ]; then
            log_result "$bin executable" "PASS"
        else
            log_result "$bin executable" "FAIL" "$BUILD_DIR/$bin not found"
            prereq_ok=false
        fi
    done

    # Check bconsole
    if command -v bconsole &> /dev/null; then
        log_result "bconsole available" "PASS"
    else
        log_result "bconsole available" "SKIP" "bconsole not installed"
    fi

    # Check TLS certificates
    if [ -f "$TLS_DIR/ca.pem" ] && [ -f "$TLS_DIR/server.pem" ] && [ -f "$TLS_DIR/client.pem" ]; then
        log_result "TLS certificates exist" "PASS"
    else
        log_result "TLS certificates exist" "FAIL" "Missing certificates in $TLS_DIR"
        prereq_ok=false
    fi

    if [ "$prereq_ok" = false ]; then
        log ""
        log "${RED}Prerequisites check failed. Please fix the issues above.${NC}"
        return 1
    fi

    return 0
}

# Start test director
start_director() {
    log_header "STARTING TEST DIRECTOR"

    "$SCRIPT_DIR/test_auth.sh" start-bg -p "$TEST_PORT" -d "$DEBUG_LEVEL" >> "$OUTPUT_FILE" 2>&1
    local result=$?

    if [ $result -eq 0 ]; then
        log_result "bareos-dir started on port $TEST_PORT" "PASS"
        sleep 2  # Give it time to fully initialize
        return 0
    else
        log_result "bareos-dir started" "FAIL" "Failed to start bareos-dir"
        return 1
    fi
}

# Stop test director
stop_director() {
    log_header "STOPPING TEST DIRECTOR"

    "$SCRIPT_DIR/test_auth.sh" stop >> "$OUTPUT_FILE" 2>&1
    log_result "bareos-dir stopped" "PASS"
}

# Run bconsole tests
run_bconsole_tests() {
    log_header "BCONSOLE TESTS (via test_auth.sh)"

    "$SCRIPT_DIR/test_auth.sh" test >> "$OUTPUT_FILE" 2>&1
    local result=$?

    if [ $result -eq 0 ]; then
        log_result "bconsole TLS-PSK" "PASS"
        log_result "bconsole Legacy" "PASS"
    else
        log_result "bconsole tests" "FAIL" "Some bconsole tests failed"
    fi
}

# Run bareosauth_test
run_bareosauth_tests() {
    log_header "BAREOSAUTH_TEST"

    local output

    # Legacy mode
    log_test "Legacy Mode"
    output=$("$BUILD_DIR/bareosauth_test" -m legacy -p "$TEST_PORT" \
        -c "$TEST_LEGACY_CONSOLE" -P "$TEST_LEGACY_PASSWORD" --verbose 2>&1)
    echo "$output" >> "$OUTPUT_FILE"

    if echo "$output" | grep -q "AUTH SUCCESS"; then
        log_result "bareosauth_test legacy" "PASS"
    else
        log_result "bareosauth_test legacy" "FAIL"
    fi

    # PSK mode
    log_test "TLS-PSK Mode"
    output=$("$BUILD_DIR/bareosauth_test" -m psk -p "$TEST_PORT" \
        -c "$TEST_PSK_CONSOLE" -P "$TEST_PSK_PASSWORD" --verbose 2>&1)
    echo "$output" >> "$OUTPUT_FILE"

    if echo "$output" | grep -q "AUTH SUCCESS"; then
        log_result "bareosauth_test TLS-PSK" "PASS"
    else
        local error=$(echo "$output" | grep -i "error" | head -1)
        log_result "bareosauth_test TLS-PSK" "FAIL" "$error"
    fi

    # Certificate mode
    log_test "TLS-Certificate Mode"
    if [ -f "$TLS_DIR/ca.pem" ]; then
        output=$("$BUILD_DIR/bareosauth_test" -m cert -p "$TEST_PORT" \
            -c "$TEST_CERT_CONSOLE" -P "$TEST_CERT_PASSWORD" \
            --ca "$TLS_DIR/ca.pem" \
            --cert "$TLS_DIR/client.pem" \
            --key "$TLS_DIR/client.key" \
            --verbose 2>&1)
        echo "$output" >> "$OUTPUT_FILE"

        if echo "$output" | grep -q "AUTH SUCCESS"; then
            log_result "bareosauth_test TLS-Cert" "PASS"
        else
            local error=$(echo "$output" | grep -i "error" | head -1)
            log_result "bareosauth_test TLS-Cert" "FAIL" "$error"
        fi
    else
        log_result "bareosauth_test TLS-Cert" "SKIP" "No certificates"
    fi
}

# Run director_test
run_director_tests() {
    log_header "DIRECTOR_TEST"

    local output

    # Legacy mode
    log_test "Legacy Mode"
    output=$("$BUILD_DIR/director_test" -p "$TEST_PORT" \
        -c "$TEST_LEGACY_CONSOLE" -P "$TEST_LEGACY_PASSWORD" \
        --legacy --verbose connect 2>&1)
    echo "$output" >> "$OUTPUT_FILE"

    if echo "$output" | grep -q "RESULT: SUCCESS\|LoadingResources\|Connected"; then
        log_result "director_test legacy" "PASS"
    else
        local error=$(echo "$output" | grep -i "error\|failed" | head -1)
        log_result "director_test legacy" "FAIL" "$error"
    fi

    # PSK mode
    log_test "TLS-PSK Mode"
    output=$("$BUILD_DIR/director_test" -p "$TEST_PORT" \
        -c "$TEST_PSK_CONSOLE" -P "$TEST_PSK_PASSWORD" \
        --psk --verbose connect 2>&1)
    echo "$output" >> "$OUTPUT_FILE"

    if echo "$output" | grep -q "RESULT: SUCCESS\|LoadingResources\|Connected"; then
        log_result "director_test TLS-PSK" "PASS"
    else
        local error=$(echo "$output" | grep -i "error\|failed" | head -1)
        log_result "director_test TLS-PSK" "FAIL" "$error"
    fi
}

# Run bareos_auth.sh iterations
run_bareos_auth_script() {
    log_header "BAREOS_AUTH.SH (Iteration Tests)"

    local output

    # Legacy mode - 5 iterations
    log_test "Legacy Mode (5 iterations)"
    output=$(BAREOS_PASSWORD="$ONESIMUS_PASSWORD" "$SCRIPT_DIR/bareos_auth.sh" \
        -m legacy -n 5 -P "$TEST_PORT" 2>&1)
    echo "$output" >> "$OUTPUT_FILE"

    if echo "$output" | grep -q "All test modes passed\|5/5 success"; then
        log_result "bareos_auth.sh legacy x5" "PASS"
    else
        local failures=$(echo "$output" | grep -o "[0-9]* failed" | head -1)
        log_result "bareos_auth.sh legacy x5" "FAIL" "$failures"
    fi
}

# Print summary
print_summary() {
    log ""
    log "========================================"
    log " TEST SUMMARY"
    log "========================================"
    log ""
    log "  ${GREEN}Passed:  $TOTAL_PASSED${NC}"
    log "  ${RED}Failed:  $TOTAL_FAILED${NC}"
    log "  ${YELLOW}Skipped: $TOTAL_SKIPPED${NC}"
    log ""
    log "  Total:   $((TOTAL_PASSED + TOTAL_FAILED + TOTAL_SKIPPED))"
    log ""

    if [ $TOTAL_FAILED -eq 0 ]; then
        log "  ${GREEN}ALL TESTS PASSED!${NC}"
    else
        log "  ${RED}SOME TESTS FAILED${NC}"
    fi

    log ""
    log "Full output saved to: $OUTPUT_FILE"
    log ""
}

# Main
main() {
    echo ""
    echo "========================================"
    echo " ONESIMUS TEST RUNNER"
    echo "========================================"
    echo ""
    echo "Output file: $OUTPUT_FILE"
    echo ""

    # Initialize output file
    init_output

    # Check prerequisites
    check_prerequisites || exit 1

    # Start test director
    start_director || exit 1

    # Run all tests
    run_bconsole_tests
    run_bareosauth_tests
    run_director_tests
    run_bareos_auth_script

    # Stop director
    stop_director

    # Print summary
    print_summary

    # Return appropriate exit code
    [ $TOTAL_FAILED -eq 0 ]
}

main "$@"
