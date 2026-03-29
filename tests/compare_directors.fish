#!/usr/bin/env fish
# =============================================================================
# Onesimus Director — Quantitative Comparison Test Suite (Fish)
# =============================================================================
#
# Runs identical commands against C++ bareos-dir and Rust onesimusd,
# compares outputs and measures response times.
#
# Usage:
#   fish tests/compare_directors.fish [--cpp-port 9101] [--rust-port 29101]
# =============================================================================

set -g CPP_PORT 9101
set -g RUST_PORT 29101
set -g BCONSOLE /usr/bin/bconsole
set -g PASSWORD "SlC3mJTFz3DnQX9ZEJwWJgAIoxe5GbD/OmyQADlFrN9A"
set -g TIMEOUT 10

# Parse args
set -l i 1
while test $i -le (count $argv)
    switch $argv[$i]
        case --cpp-port
            set i (math $i + 1); set CPP_PORT $argv[$i]
        case --rust-port
            set i (math $i + 1); set RUST_PORT $argv[$i]
        case --password
            set i (math $i + 1); set PASSWORD $argv[$i]
        case --timeout
            set i (math $i + 1); set TIMEOUT $argv[$i]
    end
    set i (math $i + 1)
end

# Setup
set -g TIMESTAMP (date +%Y%m%d-%H%M%S)
set -g RESULT_DIR tests/results/compare_$TIMESTAMP
set -g DIFF_DIR $RESULT_DIR/diffs
mkdir -p $DIFF_DIR

# bconsole configs
set -g CPP_CONF (mktemp)
set -g RUST_CONF (mktemp)
echo "Director {
  Name = bareos-dir
  address = localhost
  Port = $CPP_PORT
  Password = \"$PASSWORD\"
}" > $CPP_CONF
echo "Director {
  Name = bareos-dir
  address = localhost
  Port = $RUST_PORT
  Password = \"$PASSWORD\"
}" > $RUST_CONF

# Counters
set -g TOTAL 0
set -g PASS 0
set -g FAIL 0
set -g SKIP 0
set -g MATCH 0
set -g MISMATCH 0

# Colors
set -g RED (set_color red)
set -g GREEN (set_color green)
set -g YELLOW (set_color yellow)
set -g CYAN (set_color cyan)
set -g NC (set_color normal)

# JSON report
set -g JSON_FILE $RESULT_DIR/report.json
echo "{\"timestamp\":\"$TIMESTAMP\",\"cpp_port\":$CPP_PORT,\"rust_port\":$RUST_PORT,\"tests\":[" > $JSON_FILE
set -g FIRST_TEST true

# =============================================================================
# Helpers
# =============================================================================

function run_cmd --argument-names conf cmd
    set -l start_ms (date +%s%3N 2>/dev/null; or math (date +%s) x 1000)
    set -g LAST_OUTPUT (echo $cmd | $BCONSOLE -c $conf -u $TIMEOUT 2>/dev/null | \
        grep -v '^Connecting\|^Enter a\|^You are\|^ Encryption\|^1000 OK')
    set -l end_ms (date +%s%3N 2>/dev/null; or math (date +%s) x 1000)
    set -g LAST_MS (math $end_ms - $start_ms)
end

function normalize
    sed -E \
        -e 's/[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}/TIMESTAMP/g' \
        -e 's/JobId=[0-9]+/JobId=N/g' \
        -e 's/Version: [0-9]+\.[0-9]+\.[0-9]+[^ ]*/Version: X.Y.Z/g' \
        -e 's/[0-9]+ (files|bytes|jobs|errors)/N \1/g' \
        -e '/^$/d' | sort
end

function compare_test --argument-names name cmd mode
    test -z "$mode"; and set mode exact

    set -g TOTAL (math $TOTAL + 1)

    if test "$mode" = skip
        printf "%sSKIP%s %-40s (skipped)\n" $YELLOW $NC $name
        set -g SKIP (math $SKIP + 1)
        return
    end

    # Run against both
    run_cmd $CPP_CONF $cmd
    set -l cpp_out $LAST_OUTPUT
    set -l cpp_ms $LAST_MS

    run_cmd $RUST_CONF $cmd
    set -l rust_out $LAST_OUTPUT
    set -l rust_ms $LAST_MS

    # Compare
    set -l match yes
    set -l diff_file ""

    switch $mode
        case exact
            set -l cpp_norm (echo $cpp_out | normalize)
            set -l rust_norm (echo $rust_out | normalize)
            if test "$cpp_norm" != "$rust_norm"
                set match no
                set diff_file $DIFF_DIR/$name.diff
                diff (echo $cpp_out | psub) (echo $rust_out | psub) > $diff_file 2>/dev/null; or true
            end
        case contains
            set -l cpp_norm (echo $cpp_out | normalize)
            set -l rust_norm (echo $rust_out | normalize)
            if not echo $rust_norm | grep -qF (echo $cpp_norm | head -1)
                set match partial
                set diff_file $DIFF_DIR/$name.diff
                diff (echo $cpp_out | psub) (echo $rust_out | psub) > $diff_file 2>/dev/null; or true
            end
        case structure
            set -l cpp_lines (echo $cpp_out | wc -l | string trim)
            set -l rust_lines (echo $rust_out | wc -l | string trim)
            if test "$cpp_lines" != "$rust_lines"
                set match structure_diff
                set diff_file $DIFF_DIR/$name.diff
                echo "C++ lines: $cpp_lines, Rust lines: $rust_lines" > $diff_file
                diff (echo $cpp_out | psub) (echo $rust_out | psub) >> $diff_file 2>/dev/null; or true
            end
    end

    if test "$match" = yes
        set -g PASS (math $PASS + 1)
        set -g MATCH (math $MATCH + 1)
        printf "%sPASS%s %-40s cpp=%4dms rust=%4dms  %smatch%s\n" $GREEN $NC $name $cpp_ms $rust_ms $GREEN $NC
    else
        set -g FAIL (math $FAIL + 1)
        set -g MISMATCH (math $MISMATCH + 1)
        printf "%sFAIL%s %-40s cpp=%4dms rust=%4dms  %s%s%s\n" $RED $NC $name $cpp_ms $rust_ms $RED $match $NC
    end

    # JSON
    if test "$FIRST_TEST" != true
        echo "," >> $JSON_FILE
    end
    set -g FIRST_TEST false
    echo "  {\"name\":\"$name\",\"match\":\"$match\",\"cpp_ms\":$cpp_ms,\"rust_ms\":$rust_ms}" >> $JSON_FILE
end

# =============================================================================
# Test Suite
# =============================================================================

echo ""
echo "========================================================================="
echo " Onesimus Director — Quantitative Comparison ($TIMESTAMP)"
echo " C++ Director: localhost:$CPP_PORT"
echo " Rust Director: localhost:$RUST_PORT"
echo "========================================================================="
echo ""

# Basic
printf "%s=== Basic Commands ===%s\n" $CYAN $NC
compare_test version          "version"        contains
compare_test time             "time"           contains
compare_test help             "help"           structure
compare_test .help_all        ".help all"      structure

# Status
printf "\n%s=== Status ===%s\n" $CYAN $NC
compare_test status_dir       "status dir"           structure
compare_test status_client    "status client=bareos-fd"  contains
compare_test status_storage   "status storage=File"      contains

# List
printf "\n%s=== List Commands ===%s\n" $CYAN $NC
compare_test list_jobs        "list jobs"          structure
compare_test list_jobs_5      "list jobs limit=5"  structure
compare_test list_clients     "list clients"       structure
compare_test list_pools       "list pools"         structure
compare_test list_volumes     "list volumes"       structure
compare_test list_storages    "list storages"      structure
compare_test list_filesets    "list filesets"       structure
compare_test list_jobtotals   "list jobtotals"     structure

# Dot Commands
printf "\n%s=== Dot Commands ===%s\n" $CYAN $NC
compare_test dot_clients      ".clients"       exact
compare_test dot_jobs         ".jobs"          exact
compare_test dot_pools        ".pools"         exact
compare_test dot_storage      ".storage"       exact
compare_test dot_filesets     ".filesets"       exact
compare_test dot_schedule     ".schedule"      exact
compare_test dot_levels       ".levels"        exact
compare_test dot_types        ".types"         exact
compare_test dot_volstatus    ".volstatus"     exact
compare_test dot_media        ".media"         exact
compare_test dot_catalogs     ".catalogs"      exact
compare_test dot_jobdefs      ".jobdefs"       exact
compare_test dot_mediatypes   ".mediatypes"    exact
compare_test dot_actiononpurge ".actiononpurge" exact
compare_test dot_jobstatus    ".jobstatus"     exact

# SQL
printf "\n%s=== SQL Commands ===%s\n" $CYAN $NC
compare_test sql_count        "sqlquery SELECT COUNT(*) FROM job" contains
compare_test sql_clients      "sqlquery SELECT name FROM client ORDER BY name" exact
compare_test dot_sqltables    ".sql tables"    structure

# Show
printf "\n%s=== Show Commands ===%s\n" $CYAN $NC
compare_test show_director    "show director"  contains
compare_test show_clients     "show clients"   structure
compare_test show_jobs        "show jobs"      structure
compare_test show_pools       "show pools"     structure

# Session
printf "\n%s=== Session Commands ===%s\n" $CYAN $NC
compare_test whoami           "whoami"         exact
compare_test messages         "messages"       contains
compare_test memory           "memory"         structure

# Admin
printf "\n%s=== Admin Commands ===%s\n" $CYAN $NC
compare_test configure_export "configure export" structure
compare_test setdebug_0       "setdebug level=0" contains

# =============================================================================
# Summary
# =============================================================================

echo "" >> $JSON_FILE
echo "],\"summary\":{\"total\":$TOTAL,\"pass\":$PASS,\"fail\":$FAIL,\"skip\":$SKIP,\"match\":$MATCH,\"mismatch\":$MISMATCH}}" >> $JSON_FILE

echo ""
echo "========================================================================="
printf " Results: %s%d PASS%s  %s%d FAIL%s  %s%d SKIP%s  (%d total)\n" \
    $GREEN $PASS $NC $RED $FAIL $NC $YELLOW $SKIP $NC $TOTAL
printf " Output match: %s%d match%s  %s%d mismatch%s\n" \
    $GREEN $MATCH $NC $RED $MISMATCH $NC
echo " Report: $RESULT_DIR/report.json"
echo " Diffs:  $DIFF_DIR/"
echo "========================================================================="
echo ""

# Cleanup
rm -f $CPP_CONF $RUST_CONF

# Exit
test $FAIL -eq 0
