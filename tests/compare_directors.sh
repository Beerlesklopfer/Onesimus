#!/bin/bash
# =============================================================================
# Onesimus Director — Quantitative Comparison Test Suite
# =============================================================================
#
# Runs identical commands against both C++ bareos-dir and Rust onesimusd,
# compares outputs and measures response times.
#
# Designed for OpenClaw integration: each test is a self-contained check
# with PASS/FAIL/SKIP result, timing, and diff output.
#
# Usage:
#   ./tests/compare_directors.sh [--cpp-port 9101] [--rust-port 29101]
#
# Requirements:
#   - C++ bareos-dir running on --cpp-port (default: 9101)
#   - Rust onesimusd running on --rust-port (default: 29101)
#   - bconsole installed (/usr/bin/bconsole)
#   - Same bareos-dir.d/ config and catalog DB for both
#
# Output:
#   - Console: colored PASS/FAIL per test
#   - tests/results/compare_YYYYMMDD-HHMMSS/report.json (machine-readable)
#   - tests/results/compare_YYYYMMDD-HHMMSS/diffs/ (per-test diffs)
# =============================================================================

set -euo pipefail

# --- Configuration ---
CPP_PORT=${CPP_PORT:-9101}
RUST_PORT=${RUST_PORT:-29101}
BCONSOLE=${BCONSOLE:-/usr/bin/bconsole}
PASSWORD=${PASSWORD:-"SlC3mJTFz3DnQX9ZEJwWJgAIoxe5GbD/OmyQADlFrN9A"}
TIMEOUT=${TIMEOUT:-10}

# Parse args
while [[ $# -gt 0 ]]; do
    case "$1" in
        --cpp-port)  CPP_PORT="$2"; shift 2 ;;
        --rust-port) RUST_PORT="$2"; shift 2 ;;
        --password)  PASSWORD="$2"; shift 2 ;;
        --timeout)   TIMEOUT="$2"; shift 2 ;;
        *) echo "Unknown arg: $1"; exit 1 ;;
    esac
done

# --- Setup ---
TIMESTAMP=$(date +%Y%m%d-%H%M%S)
RESULT_DIR="tests/results/compare_${TIMESTAMP}"
DIFF_DIR="${RESULT_DIR}/diffs"
mkdir -p "$DIFF_DIR"

# bconsole configs
CPP_CONF=$(mktemp)
RUST_CONF=$(mktemp)
cat > "$CPP_CONF" << EOF
Director {
  Name = bareos-dir
  address = localhost
  Port = ${CPP_PORT}
  Password = "${PASSWORD}"
}
EOF
cat > "$RUST_CONF" << EOF
Director {
  Name = bareos-dir
  address = localhost
  Port = ${RUST_PORT}
  Password = "${PASSWORD}"
}
EOF
trap "rm -f $CPP_CONF $RUST_CONF" EXIT

# --- Counters ---
TOTAL=0
PASS=0
FAIL=0
SKIP=0
MATCH=0
MISMATCH=0

# --- JSON report ---
JSON_FILE="${RESULT_DIR}/report.json"
echo '{"timestamp":"'$TIMESTAMP'","cpp_port":'$CPP_PORT',"rust_port":'$RUST_PORT',"tests":[' > "$JSON_FILE"
FIRST_TEST=true

# --- Colors ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
NC='\033[0m'

# =============================================================================
# Test runner
# =============================================================================

# run_cmd <config> <command> → stdout=output, sets global REPLY_MS
REPLY_MS=0
run_cmd() {
    local conf="$1"
    local cmd="$2"
    local start_s end_s
    start_s=$(date +%s)
    local output
    output=$(echo "$cmd" | $BCONSOLE -c "$conf" -u "$TIMEOUT" 2>/dev/null | \
             grep -v '^Connecting\|^Enter a\|^You are\|^ Encryption\|^1000 OK' || true)
    end_s=$(date +%s)
    REPLY_MS=$(( (end_s - start_s) * 1000 ))
    echo "$output"
}

# normalize: strip timestamps, session IDs, PIDs, whitespace differences
normalize() {
    sed -E \
        -e 's/[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}/TIMESTAMP/g' \
        -e 's/JobId=[0-9]+/JobId=N/g' \
        -e 's/Version: [0-9]+\.[0-9]+\.[0-9]+[^ ]*/Version: X.Y.Z/g' \
        -e 's/[0-9]+ (files|bytes|jobs|errors)/N \1/g' \
        -e 's/^\s+$//' \
        -e '/^$/d' | \
    sort
}

# compare_test <test_name> <command> [<match_mode>]
# match_mode: "exact" (default), "contains", "structure" (same columns), "skip"
compare_test() {
    local name="$1"
    local cmd="$2"
    local mode="${3:-exact}"

    TOTAL=$((TOTAL + 1))

    if [[ "$mode" == "skip" ]]; then
        printf "${YELLOW}SKIP${NC} %-40s (skipped)\n" "$name"
        SKIP=$((SKIP + 1))
        return
    fi

    # Run against both directors
    local cpp_out rust_out
    cpp_out=$(run_cmd "$CPP_CONF" "$cmd")
    local cpp_ms=$REPLY_MS
    rust_out=$(run_cmd "$RUST_CONF" "$cmd")
    local rust_ms=$REPLY_MS

    # Check if both responded
    if [[ -z "$cpp_out" && -z "$rust_out" ]]; then
        printf "${YELLOW}SKIP${NC} %-40s (no output from either)\n" "$name"
        SKIP=$((SKIP + 1))
        return
    fi

    local result="PASS"
    local match="yes"
    local diff_file=""

    case "$mode" in
        exact)
            if [[ "$cpp_out" != "$rust_out" ]]; then
                # Try normalized comparison
                local cpp_norm=$(echo "$cpp_out" | normalize)
                local rust_norm=$(echo "$rust_out" | normalize)
                if [[ "$cpp_norm" != "$rust_norm" ]]; then
                    match="no"
                    diff_file="${DIFF_DIR}/${name}.diff"
                    diff <(echo "$cpp_out") <(echo "$rust_out") > "$diff_file" 2>/dev/null || true
                fi
            fi
            ;;
        contains)
            # Rust output must contain same key data as C++
            local cpp_norm=$(echo "$cpp_out" | normalize)
            local rust_norm=$(echo "$rust_out" | normalize)
            # Check if all normalized C++ lines appear in Rust output
            local missing=0
            while IFS= read -r line; do
                if [[ -n "$line" ]] && ! echo "$rust_norm" | grep -qF "$line"; then
                    missing=$((missing + 1))
                fi
            done <<< "$cpp_norm"
            if [[ $missing -gt 0 ]]; then
                match="partial"
                diff_file="${DIFF_DIR}/${name}.diff"
                diff <(echo "$cpp_out") <(echo "$rust_out") > "$diff_file" 2>/dev/null || true
            fi
            ;;
        structure)
            # Same number of rows/columns (for table output)
            local cpp_lines=$(echo "$cpp_out" | wc -l)
            local rust_lines=$(echo "$rust_out" | wc -l)
            if [[ "$cpp_lines" != "$rust_lines" ]]; then
                match="structure_diff"
                diff_file="${DIFF_DIR}/${name}.diff"
                echo "C++ lines: $cpp_lines, Rust lines: $rust_lines" > "$diff_file"
                diff <(echo "$cpp_out") <(echo "$rust_out") >> "$diff_file" 2>/dev/null || true
            fi
            ;;
    esac

    if [[ "$match" == "yes" ]]; then
        PASS=$((PASS + 1))
        MATCH=$((MATCH + 1))
        printf "${GREEN}PASS${NC} %-40s cpp=%4dms rust=%4dms  ${GREEN}match${NC}\n" "$name" "$cpp_ms" "$rust_ms"
    else
        FAIL=$((FAIL + 1))
        MISMATCH=$((MISMATCH + 1))
        printf "${RED}FAIL${NC} %-40s cpp=%4dms rust=%4dms  ${RED}${match}${NC}\n" "$name" "$cpp_ms" "$rust_ms"
    fi

    # JSON entry
    if [[ "$FIRST_TEST" != "true" ]]; then echo ',' >> "$JSON_FILE"; fi
    FIRST_TEST=false
    cat >> "$JSON_FILE" << JSONEOF
  {"name":"$name","command":"$(echo "$cmd" | sed 's/"/\\"/g')","mode":"$mode","match":"$match","cpp_ms":$cpp_ms,"rust_ms":$rust_ms,"diff":"${diff_file##*/}"}
JSONEOF
}

# =============================================================================
# Test Suite
# =============================================================================

echo ""
echo "========================================================================="
echo " Onesimus Director — Quantitative Comparison ($TIMESTAMP)"
echo " C++ Director: localhost:${CPP_PORT}"
echo " Rust Director: localhost:${RUST_PORT}"
echo "========================================================================="
echo ""

# --- Basic Commands ---
printf "${CYAN}=== Basic Commands ===${NC}\n"
compare_test "version"           "version"            "contains"
compare_test "time"              "time"               "contains"
compare_test "help"              "help"               "structure"
compare_test ".help_all"         ".help all"          "structure"

# --- Status ---
printf "\n${CYAN}=== Status ===${NC}\n"
compare_test "status_dir"        "status dir"         "structure"
compare_test "status_client"     "status client=bareos-fd" "contains"
compare_test "status_storage"    "status storage=File" "contains"

# --- List Commands (DB) ---
printf "\n${CYAN}=== List Commands ===${NC}\n"
compare_test "list_jobs"         "list jobs"          "structure"
compare_test "list_jobs_limit5"  "list jobs limit=5"  "structure"
compare_test "list_clients"      "list clients"       "structure"
compare_test "list_pools"        "list pools"         "structure"
compare_test "list_volumes"      "list volumes"       "structure"
compare_test "list_storages"     "list storages"      "structure"
compare_test "list_filesets"     "list filesets"       "structure"
compare_test "list_jobtotals"    "list jobtotals"     "structure"

# --- Dot Commands (Completion) ---
printf "\n${CYAN}=== Dot Commands ===${NC}\n"
compare_test "dot_clients"       ".clients"           "exact"
compare_test "dot_jobs"          ".jobs"              "exact"
compare_test "dot_pools"         ".pools"             "exact"
compare_test "dot_storage"       ".storage"           "exact"
compare_test "dot_filesets"      ".filesets"          "exact"
compare_test "dot_schedule"      ".schedule"          "exact"
compare_test "dot_levels"        ".levels"            "exact"
compare_test "dot_types"         ".types"             "exact"
compare_test "dot_volstatus"     ".volstatus"         "exact"
compare_test "dot_media"         ".media"             "exact"
compare_test "dot_catalogs"      ".catalogs"          "exact"
compare_test "dot_jobdefs"       ".jobdefs"           "exact"
compare_test "dot_mediatypes"    ".mediatypes"        "exact"
compare_test "dot_actiononpurge" ".actiononpurge"     "exact"
compare_test "dot_jobstatus"     ".jobstatus"         "exact"

# --- SQL ---
printf "\n${CYAN}=== SQL Commands ===${NC}\n"
compare_test "sqlquery_count"    "sqlquery SELECT COUNT(*) FROM job" "contains"
compare_test "sqlquery_clients"  "sqlquery SELECT name FROM client ORDER BY name" "exact"
compare_test "dot_sqltables"     ".sql tables"        "structure"

# --- Show (Config) ---
printf "\n${CYAN}=== Show Commands ===${NC}\n"
compare_test "show_director"     "show director"      "contains"
compare_test "show_clients"      "show clients"       "structure"
compare_test "show_jobs"         "show jobs"          "structure"
compare_test "show_pools"        "show pools"         "structure"

# --- Session Commands ---
printf "\n${CYAN}=== Session Commands ===${NC}\n"
compare_test "whoami"            "whoami"             "exact"
compare_test "messages"          "messages"           "contains"
compare_test "memory"            "memory"             "structure"

# --- Admin (read-only) ---
printf "\n${CYAN}=== Admin Commands ===${NC}\n"
compare_test "configure_export"  "configure export"   "structure"
compare_test "setdebug_0"       "setdebug level=0"    "contains"

# =============================================================================
# Summary
# =============================================================================

echo "" >> "$JSON_FILE"
echo '],"summary":{"total":'$TOTAL',"pass":'$PASS',"fail":'$FAIL',"skip":'$SKIP',"match":'$MATCH',"mismatch":'$MISMATCH'}}' >> "$JSON_FILE"

echo ""
echo "========================================================================="
printf " Results: ${GREEN}${PASS} PASS${NC}  ${RED}${FAIL} FAIL${NC}  ${YELLOW}${SKIP} SKIP${NC}  (${TOTAL} total)\n"
printf " Output match: ${GREEN}${MATCH} match${NC}  ${RED}${MISMATCH} mismatch${NC}\n"
echo " Report: ${RESULT_DIR}/report.json"
echo " Diffs:  ${DIFF_DIR}/"
echo "========================================================================="
echo ""

# Exit code: 0 if all pass, 1 if any fail
[[ $FAIL -eq 0 ]]
