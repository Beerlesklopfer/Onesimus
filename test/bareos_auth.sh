#!/bin/bash
#
# bareos_auth.sh - Automated Bareos authentication test script
#
# Usage: ./bareos_auth.sh [options]
#
# Options:
#   -m, --mode <mode>     Test mode: legacy, psk, cert, all (default: all)
#   -n, --count <n>       Number of test iterations (default: 10)
#   -v, --verbose         Verbose output
#   -h, --help            Show this help
#
# Requirements:
#   - bareosauth_test must be built in ../build/
#   - Director console configs must be installed for each mode
#   - Password must be set in BAREOS_PASSWORD environment variable or config
#
# Example:
#   BAREOS_PASSWORD=mypassword ./bareos_auth.sh -m legacy -n 20
#

# Don't use set -e because of bash arithmetic issues with ((var++))

# Default values
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/../build"
TEST_BIN="${BUILD_DIR}/bareosauth_test"

MODE="all"
COUNT=10
VERBOSE=""
HOST="localhost"
PORT=9101
DIRECTOR="bareos-dir"

# Console names for each mode
CONSOLE_LEGACY="onesimus"
CONSOLE_PSK="onesimus-psk"
CONSOLE_CERT="onesimus-cert"

# Certificate files (adjust paths as needed)
CA_FILE="/etc/bareos/ssl/bareos-ca.pem"
CERT_FILE="/etc/bareos/ssl/bareos-client.pem"
KEY_FILE="/etc/bareos/ssl/bareos-client.key"

# Password (can be overridden by environment variable)
PASSWORD="${BAREOS_PASSWORD:-}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

usage() {
    cat << EOF
Usage: $0 [options]

Options:
  -m, --mode <mode>     Test mode: legacy, psk, cert, all (default: all)
  -n, --count <n>       Number of test iterations (default: 10)
  -p, --password <pwd>  Password (or set BAREOS_PASSWORD env var)
  -H, --host <host>     Director host (default: localhost)
  -P, --port <port>     Director port (default: 9101)
  -v, --verbose         Verbose output
  -h, --help            Show this help

Environment:
  BAREOS_PASSWORD       Password for authentication

Example:
  BAREOS_PASSWORD=mypassword $0 -m legacy -n 20
  $0 -m all -p mypassword -n 5
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
    log_info "Build it first: cd ${BUILD_DIR} && make bareosauth_test"
    exit 1
fi

# Test function
run_test() {
    local mode=$1
    local console=$2
    local extra_args=$3
    local success=0
    local fail=0

    echo ""
    echo "========================================"
    echo "Testing mode: $mode ($COUNT iterations)"
    echo "Console: $console"
    echo "========================================"

    for i in $(seq 1 $COUNT); do
        if $TEST_BIN --host "$HOST" -p "$PORT" -d "$DIRECTOR" \
                     -c "$console" -P "$PASSWORD" -m "$mode" \
                     $extra_args $VERBOSE 2>&1 | grep -q "AUTH SUCCESS"; then
            ((success++))
            echo -n -e "${GREEN}✓${NC}"
        else
            ((fail++))
            echo -n -e "${RED}✗${NC}"
        fi
        sleep 0.3
    done

    echo ""
    echo "Results: $success/$COUNT success, $fail failed"

    if [[ $fail -gt 0 ]]; then
        return 1
    fi
    return 0
}

# Main
echo "========================================"
echo "Bareos Authentication Test Suite"
echo "========================================"
echo "Host: $HOST:$PORT"
echo "Director: $DIRECTOR"
echo "Mode: $MODE"
echo "Iterations: $COUNT"
echo "========================================"

TOTAL_SUCCESS=0
TOTAL_FAIL=0

case $MODE in
    legacy)
        if run_test "legacy" "$CONSOLE_LEGACY" ""; then
            ((TOTAL_SUCCESS++))
        else
            ((TOTAL_FAIL++))
        fi
        ;;
    psk)
        if run_test "psk" "$CONSOLE_PSK" ""; then
            ((TOTAL_SUCCESS++))
        else
            ((TOTAL_FAIL++))
        fi
        ;;
    cert)
        extra="--ca $CA_FILE --cert $CERT_FILE --key $KEY_FILE"
        if run_test "cert" "$CONSOLE_CERT" "$extra"; then
            ((TOTAL_SUCCESS++))
        else
            ((TOTAL_FAIL++))
        fi
        ;;
    all)
        # Test all modes
        if run_test "legacy" "$CONSOLE_LEGACY" ""; then
            ((TOTAL_SUCCESS++))
        else
            ((TOTAL_FAIL++))
        fi

        if run_test "psk" "$CONSOLE_PSK" ""; then
            ((TOTAL_SUCCESS++))
        else
            ((TOTAL_FAIL++))
        fi

        extra="--ca $CA_FILE --cert $CERT_FILE --key $KEY_FILE"
        if run_test "cert" "$CONSOLE_CERT" "$extra"; then
            ((TOTAL_SUCCESS++))
        else
            ((TOTAL_FAIL++))
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

if [[ $TOTAL_FAIL -eq 0 ]]; then
    log_info "All test modes passed!"
    exit 0
else
    log_error "$TOTAL_FAIL mode(s) had failures"
    exit 1
fi
