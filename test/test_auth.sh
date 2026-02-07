#!/bin/bash
#
# Test script for Bareos authentication methods
# Tests TLS-PSK, TLS-Certificate, and Legacy authentication
#
# Runs bareos-dir locally with test/bareos-dir.d config using:
#   bareos-dir -c test/bareos-dir.d -f -d <level>
#

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"
TEST_CONFIG_DIR="$SCRIPT_DIR/bareos-dir.d"
TEST_CONFIG_BAREOS_D="$TEST_CONFIG_DIR/bareos-dir.d"
SYSTEM_CONFIG_DIR="/etc/bareos/bareos-dir.d"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test port (different from production 9101)
TEST_PORT="19101"
DEBUG_LEVEL="100"

# PID file for local bareos-dir
PID_FILE="$SCRIPT_DIR/bareos-dir-test.pid"

# Default test parameters
DEFAULT_HOST="localhost"
DEFAULT_DIRECTOR="bareos-dir"

# Test console definitions from test/bareos-dir.d/console/
TEST_PSK_CONSOLE="test-psk"
TEST_PSK_PASSWORD="TestPSKPassword123"

TEST_CERT_CONSOLE="test-cert"
TEST_CERT_PASSWORD="TestCertPassword123"

TEST_LEGACY_CONSOLE="test-legacy"
TEST_LEGACY_PASSWORD="TestLegacyPassword123"

# Default console
DEFAULT_CONSOLE="onesimus"
DEFAULT_PASSWORD="RDZTTAJE5pj5O/C2uOi01o8FRsqucVPolersd3AbXBs"

log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

log_debug() {
    echo -e "${BLUE}[DEBUG]${NC} $1"
}

log_test() {
    echo -e "\n${YELLOW}========================================${NC}"
    echo -e "${YELLOW} TEST: $1${NC}"
    echo -e "${YELLOW}========================================${NC}\n"
}

# Setup test config directory by copying system configs
setup_test_config() {
    log_info "Setting up test configuration in $TEST_CONFIG_BAREOS_D"

    if [ ! -d "$SYSTEM_CONFIG_DIR" ]; then
        log_error "System config not found: $SYSTEM_CONFIG_DIR"
        return 1
    fi

    # Check if base configs already exist (check for catalog as indicator)
    if [ -d "$TEST_CONFIG_BAREOS_D/catalog" ] && [ "$(ls -A $TEST_CONFIG_BAREOS_D/catalog 2>/dev/null)" ]; then
        log_info "Test config already initialized"
        return 0
    fi

    log_warn "Copying base configs from system (requires sudo)..."

    # Ensure target directory structure exists
    mkdir -p "$TEST_CONFIG_BAREOS_D"

    # Copy all system config subdirectories (except console/director which we manage)
    for subdir in catalog client fileset job jobdefs messages pool schedule storage; do
        if [ -d "$SYSTEM_CONFIG_DIR/$subdir" ]; then
            sudo cp -r "$SYSTEM_CONFIG_DIR/$subdir" "$TEST_CONFIG_BAREOS_D/" || {
                log_error "Failed to copy $subdir"
                return 1
            }
        fi
    done

    # Fix ownership
    sudo chown -R "$(whoami):$(id -gn)" "$TEST_CONFIG_DIR"

    log_info "Base configs copied successfully"
    return 0
}

# TLS certificate paths
TLS_DIR="$SCRIPT_DIR/bareos-dir.d/tls"

# Update director config for test port
update_director_port() {
    local port="$1"
    local tls_enable="${2:-yes}"
    local tls_require="${3:-no}"

    local dir_conf="$TEST_CONFIG_BAREOS_D/director/bareos-dir.conf"

    log_info "Updating director config for port $port (TLS Enable=$tls_enable, TLS Require=$tls_require)"

    cat > "$dir_conf" << EOF
Director {
  Name = "bareos-dir"
  QueryFile = "/usr/lib/bareos/scripts/query.sql"
  Maximum Concurrent Jobs = 10
  Password = "DIRECTOR_PASSWORD"
  Messages = "Daemon"
  Auditing = yes
  DIRport = $port
  TLS Enable = $tls_enable
  TLS Require = $tls_require
  TLS CA Certificate File = "$TLS_DIR/ca.pem"
  TLS Certificate = "$TLS_DIR/server.pem"
  TLS Key = "$TLS_DIR/server.key"
}
EOF
}

# Start bareos-dir with test config
start_bareos_dir() {
    local port="${1:-$TEST_PORT}"
    local debug="${2:-$DEBUG_LEVEL}"
    local tls_enable="${3:-yes}"
    local tls_require="${4:-no}"

    log_info "Starting bareos-dir with test config..."
    log_info "  Config: $TEST_CONFIG_DIR"
    log_info "  Port: $port"
    log_info "  Debug level: $debug"
    log_info "  TLS Enable: $tls_enable"
    log_info "  TLS Require: $tls_require"

    # Check if already running
    if [ -f "$PID_FILE" ]; then
        local old_pid=$(cat "$PID_FILE")
        if kill -0 "$old_pid" 2>/dev/null; then
            log_warn "bareos-dir already running with PID $old_pid"
            log_info "Use '$0 stop' to stop it first"
            return 1
        fi
        rm -f "$PID_FILE"
    fi

    # Update director config with port and TLS settings
    update_director_port "$port" "$tls_enable" "$tls_require"

    # Test config first
    log_info "Testing configuration..."
    if ! sudo -u bareos /usr/sbin/bareos-dir -t -c "$TEST_CONFIG_DIR" 2>&1; then
        log_error "Configuration test failed!"
        log_info "Run '$0 setup' to initialize test configs"
        return 1
    fi

    log_info "Configuration OK. Starting bareos-dir in foreground..."
    echo ""
    echo -e "${YELLOW}========================================${NC}"
    echo -e "${YELLOW} sudo bareos-dir -c $TEST_CONFIG_DIR -f -d $debug${NC}"
    echo -e "${YELLOW}========================================${NC}"
    echo ""

    # Start in foreground (user can Ctrl+C to stop)
    sudo -u bareos /usr/sbin/bareos-dir -c "$TEST_CONFIG_DIR" -f -d "$debug"
}

# Start bareos-dir in background
start_bareos_dir_bg() {
    local port="${1:-$TEST_PORT}"
    local debug="${2:-$DEBUG_LEVEL}"
    local tls_enable="${3:-yes}"
    local tls_require="${4:-no}"

    log_info "Starting bareos-dir in background..."

    # Check if already running
    if [ -f "$PID_FILE" ]; then
        local old_pid=$(cat "$PID_FILE")
        if kill -0 "$old_pid" 2>/dev/null; then
            log_warn "bareos-dir already running with PID $old_pid"
            return 0
        fi
        rm -f "$PID_FILE"
    fi

    # Update director config
    update_director_port "$port" "$tls_enable" "$tls_require"

    # Test config
    if ! sudo -u bareos /usr/sbin/bareos-dir -t -c "$TEST_CONFIG_DIR" 2>&1; then
        log_error "Configuration test failed!"
        return 1
    fi

    # Start in background
    sudo -u bareos /usr/sbin/bareos-dir -c "$TEST_CONFIG_DIR" -f -d "$debug" &
    local pid=$!
    echo "$pid" > "$PID_FILE"

    sleep 2

    if kill -0 "$pid" 2>/dev/null; then
        log_info "bareos-dir started with PID $pid on port $port"
        return 0
    else
        log_error "Failed to start bareos-dir"
        rm -f "$PID_FILE"
        return 1
    fi
}

# Stop bareos-dir
stop_bareos_dir() {
    if [ -f "$PID_FILE" ]; then
        local pid=$(cat "$PID_FILE")
        if sudo kill -0 "$pid" 2>/dev/null; then
            log_info "Stopping bareos-dir (PID $pid)..."
            sudo kill "$pid" 2>/dev/null
            sleep 1
            if sudo kill -0 "$pid" 2>/dev/null; then
                log_warn "Sending SIGKILL..."
                sudo kill -9 "$pid" 2>/dev/null
            fi
            log_info "bareos-dir stopped"
        else
            log_info "bareos-dir not running"
        fi
        rm -f "$PID_FILE"
    else
        log_info "No PID file found"
    fi
}

# Run bconsole test
run_bconsole_test() {
    local test_name="$1"
    local host="$2"
    local port="$3"
    local console="$4"
    local password="$5"

    log_debug "Testing $test_name: $console@$host:$port"

    if ! command -v bconsole &> /dev/null; then
        log_warn "bconsole not found"
        return 1
    fi

    # Create temporary bconsole config (needs Console section!)
    local tmp_conf=$(mktemp)
    cat > "$tmp_conf" << EOF
Director {
  Name = "$DEFAULT_DIRECTOR"
  DIRport = $port
  Address = "$host"
  Password = "$password"
}

Console {
  Name = "$console"
  Password = "$password"
}
EOF

    # Try to connect
    local output
    output=$(echo "version" | timeout 10 bconsole -c "$tmp_conf" 2>&1)
    local result=$?

    rm -f "$tmp_conf"

    if [ $result -eq 0 ] && echo "$output" | grep -q "Version:"; then
        log_debug "Success: $(echo "$output" | grep "Version:" | head -1)"
        return 0
    else
        log_debug "Failed: $output"
        return 1
    fi
}

# Run bconsole test with TLS certificates
run_bconsole_cert_test() {
    local test_name="$1"
    local host="$2"
    local port="$3"
    local console="$4"
    local password="$5"

    log_debug "Testing $test_name (TLS-Cert): $console@$host:$port"

    if ! command -v bconsole &> /dev/null; then
        log_warn "bconsole not found"
        return 1
    fi

    # Create temporary bconsole config with TLS certificates
    local tmp_conf=$(mktemp)
    cat > "$tmp_conf" << EOF
Director {
  Name = "$DEFAULT_DIRECTOR"
  DIRport = $port
  Address = "$host"
  Password = "$password"
  TLS Enable = yes
  TLS Require = yes
  TLS Verify Peer = yes
  TLS CA Certificate File = "$TLS_DIR/ca.pem"
  TLS Certificate = "$TLS_DIR/client.pem"
  TLS Key = "$TLS_DIR/client.key"
}

Console {
  Name = "$console"
  Password = "$password"
  TLS Enable = yes
  TLS Require = yes
  TLS CA Certificate File = "$TLS_DIR/ca.pem"
  TLS Certificate = "$TLS_DIR/client.pem"
  TLS Key = "$TLS_DIR/client.key"
}
EOF

    # Try to connect
    local output
    output=$(echo "version" | timeout 10 bconsole -c "$tmp_conf" 2>&1)
    local result=$?

    rm -f "$tmp_conf"

    if [ $result -eq 0 ] && echo "$output" | grep -q "Version:"; then
        log_debug "Success: $(echo "$output" | grep "Version:" | head -1)"
        return 0
    else
        log_debug "Failed: $output"
        return 1
    fi
}

# Run all authentication tests
run_tests() {
    local host="${1:-$DEFAULT_HOST}"
    local port="${2:-$TEST_PORT}"
    local passed=0
    local failed=0
    local skipped=0

    log_info "Running authentication tests against $host:$port"
    echo ""

    # Test 1: TLS-PSK
    log_test "1/3: TLS-PSK Authentication"
    if run_bconsole_test "TLS-PSK" "$host" "$port" "$TEST_PSK_CONSOLE" "$TEST_PSK_PASSWORD"; then
        ((passed++))
        echo -e "TLS-PSK: ${GREEN}PASSED${NC}"
    else
        ((failed++))
        echo -e "TLS-PSK: ${RED}FAILED${NC}"
    fi

    # Test 2: TLS-Certificate
    log_test "2/3: TLS-Certificate Authentication"
    if [ -f "$TLS_DIR/ca.pem" ]; then
        if run_bconsole_cert_test "TLS-Cert" "$host" "$port" "$TEST_CERT_CONSOLE" "$TEST_CERT_PASSWORD"; then
            ((passed++))
            echo -e "TLS-Certificate: ${GREEN}PASSED${NC}"
        else
            ((failed++))
            echo -e "TLS-Certificate: ${RED}FAILED${NC}"
        fi
    else
        ((skipped++))
        echo -e "TLS-Certificate: ${YELLOW}SKIPPED${NC} (no certs in $TLS_DIR)"
    fi

    # Test 3: Legacy
    log_test "3/3: Legacy Authentication (no TLS)"
    if run_bconsole_test "Legacy" "$host" "$port" "$TEST_LEGACY_CONSOLE" "$TEST_LEGACY_PASSWORD"; then
        ((passed++))
        echo -e "Legacy: ${GREEN}PASSED${NC}"
    else
        ((failed++))
        echo -e "Legacy: ${RED}FAILED${NC}"
    fi

    # Summary
    echo ""
    echo -e "${YELLOW}========================================${NC}"
    echo -e "${YELLOW} TEST SUMMARY${NC}"
    echo -e "${YELLOW}========================================${NC}"
    echo -e "Passed:  ${GREEN}$passed${NC}"
    echo -e "Failed:  ${RED}$failed${NC}"
    echo -e "Skipped: ${YELLOW}$skipped${NC}"

    return $failed
}

# Full test cycle: start, test, stop
run_full_test() {
    local port="${1:-$TEST_PORT}"
    local debug="${2:-$DEBUG_LEVEL}"

    log_info "Running full test cycle..."

    # Setup if needed
    setup_test_config || return 1

    # Start bareos-dir in background
    start_bareos_dir_bg "$port" "$debug" || return 1

    # Wait for startup
    sleep 2

    # Run tests
    run_tests "$DEFAULT_HOST" "$port"
    local result=$?

    # Stop bareos-dir
    stop_bareos_dir

    return $result
}

# Show usage
usage() {
    echo "Usage: $0 [command] [options]"
    echo ""
    echo "Commands:"
    echo "  setup           Copy system configs to test/bareos-dir.d (requires sudo)"
    echo "  start           Start bareos-dir with test config (foreground)"
    echo "  start-bg        Start bareos-dir in background"
    echo "  stop            Stop background bareos-dir"
    echo "  test            Run auth tests against running bareos-dir"
    echo "  run             Full cycle: setup, start-bg, test, stop"
    echo "  help            Show this help"
    echo ""
    echo "Options:"
    echo "  -p, --port PORT       Test port (default: $TEST_PORT)"
    echo "  -d, --debug LEVEL     Debug level (default: $DEBUG_LEVEL)"
    echo "  --tls-enable yes|no   TLS Enable setting (default: yes)"
    echo "  --tls-require yes|no  TLS Require setting (default: no)"
    echo ""
    echo "Test Consoles (in test/bareos-dir.d/console/):"
    echo "  test-psk      Password: $TEST_PSK_PASSWORD"
    echo "  test-cert     Password: $TEST_CERT_PASSWORD"
    echo "  test-legacy   Password: $TEST_LEGACY_PASSWORD"
    echo ""
    echo "Examples:"
    echo "  $0 setup                    # Initialize test config (first time)"
    echo "  $0 start -d 200             # Start with debug level 200"
    echo "  $0 start-bg && $0 test      # Start background and test"
    echo "  $0 run                      # Full automatic test cycle"
    echo "  $0 run -p 29101 -d 50       # Custom port and debug level"
}

# Main
main() {
    local command="${1:-help}"
    shift || true

    local port="$TEST_PORT"
    local debug="$DEBUG_LEVEL"
    local tls_enable="yes"
    local tls_require="no"

    # Parse options
    while [[ $# -gt 0 ]]; do
        case $1 in
            -p|--port)
                port="$2"
                shift 2
                ;;
            -d|--debug)
                debug="$2"
                shift 2
                ;;
            --tls-enable)
                tls_enable="$2"
                shift 2
                ;;
            --tls-require)
                tls_require="$2"
                shift 2
                ;;
            *)
                shift
                ;;
        esac
    done

    case $command in
        setup)
            setup_test_config
            ;;
        start)
            start_bareos_dir "$port" "$debug" "$tls_enable" "$tls_require"
            ;;
        start-bg)
            start_bareos_dir_bg "$port" "$debug" "$tls_enable" "$tls_require"
            ;;
        stop)
            stop_bareos_dir
            ;;
        test)
            run_tests "$DEFAULT_HOST" "$port"
            ;;
        run)
            run_full_test "$port" "$debug"
            ;;
        help|--help|-h|-?)
            usage
            ;;
        *)
            log_error "Unknown command: $command"
            usage
            exit 1
            ;;
    esac
}

main "$@"
