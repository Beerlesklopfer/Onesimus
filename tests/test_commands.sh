#!/bin/bash
# baReos Command-Test: Standalone — startet eigenen Director
# Erstellt Config unter /tmp, startet Director, testet, räumt auf.
set -euo pipefail

BAREOS_DIRD=${BAREOS_DIRD:-./target/release/bareos-dird}
BRCONSOLE=${BRCONSOLE:-./target/release/brconsole}
PORT=29101
PASSWORD="testpass-$(date +%s)"
TMPDIR=$(mktemp -d /tmp/bareos-test-XXXXXX)
PASS=0
FAIL=0
TOTAL=0

cleanup() {
    kill $DIRD_PID 2>/dev/null || true
    wait $DIRD_PID 2>/dev/null || true
    rm -rf "$TMPDIR"
}
trap cleanup EXIT

# === Config erstellen ===
cat > "$TMPDIR/bareos-dir.conf" << EOF
Director {
  Name = test-dir
  Port = $PORT
  Password = "$PASSWORD"
  Maximum Concurrent Jobs = 10
}
Client {
  Name = test-fd
  Address = localhost
  Port = 9102
  Password = "fdpass"
}
Storage {
  Name = File
  Address = localhost
  Port = 9103
  Password = "sdpass"
  Device = FileStorage
  Media Type = File
}
Pool {
  Name = Full
  Pool Type = Backup
  Recycle = yes
  Volume Retention = 365 days
  Maximum Volume Bytes = 50G
  Maximum Volumes = 100
}
Pool {
  Name = Incremental
  Pool Type = Backup
  Recycle = yes
  Volume Retention = 30 days
}
Schedule {
  Name = WeeklyCycle
  Run = Full 1st sun at 23:05
  Run = Incremental mon-sat at 23:05
}
Job {
  Name = BackupTest
  Type = Backup
  Level = Full
  Client = test-fd
  Storage = File
  Pool = Full
  Schedule = WeeklyCycle
}
Console {
  Name = admin
  Password = "adminpass"
}
EOF

cat > "$TMPDIR/brconsole.conf" << EOF
Director {
  Name = test-dir
  address = localhost
  Port = $PORT
  Password = "$PASSWORD"
}
EOF

# === Director starten ===
$BAREOS_DIRD -c "$TMPDIR/bareos-dir.conf" -f -d100 2>"$TMPDIR/director.log" &
DIRD_PID=$!
sleep 2

if ! kill -0 $DIRD_PID 2>/dev/null; then
    echo "FATAL: Director konnte nicht gestartet werden"
    cat "$TMPDIR/director.log"
    exit 1
fi
echo "Director PID=$DIRD_PID Port=$PORT"
echo ""

# === Test-Funktion ===
run_cmd() {
    local cmd="$1"
    local expect="$2"
    TOTAL=$((TOTAL+1))

    # Prüfe ob Director noch lebt
    if ! kill -0 $DIRD_PID 2>/dev/null; then
        printf "  \e[31mCRASH\e[0m %-40s  Director ist abgestuerzt!\n" "$cmd"
        FAIL=$((FAIL+1))
        return
    fi

    local output
    output=$(timeout 10 $BRCONSOLE -c "$TMPDIR/brconsole.conf" -e "$cmd" 2>&1) || true

    if echo "$output" | grep -qiE "$expect"; then
        printf "  \e[32mOK\e[0m   %-40s\n" "$cmd"
        PASS=$((PASS+1))
    else
        printf "  \e[31mFAIL\e[0m %-40s  (erwartet: '%s')\n" "$cmd" "$expect"
        echo "        $(echo "$output" | grep -v "^Connecting\|^$\|Encryption\|1000OK" | head -2)"
        FAIL=$((FAIL+1))
    fi
}

echo "========================================"
echo "baReos Command Test Suite"
echo "========================================"
echo ""

echo "--- Basis ---"
run_cmd "version" "0.25.0"
run_cmd "time" "202"
run_cmd "help" "Available commands"
run_cmd ".help" "version"

echo ""
echo "--- Status ---"
run_cmd "status dir" "Director:|test-dir"
run_cmd ".status dir" "Director:|test-dir"
run_cmd "status all" "Director:|Connections"

echo ""
echo "--- Dot-Queries ---"
run_cmd ".clients" "test-fd"
run_cmd ".jobs" "BackupTest"
run_cmd ".pools" "Full"
run_cmd ".storage" "File"
run_cmd ".defaults" "BackupTest|job="
run_cmd ".jobstatus" "Completed|Running"
run_cmd ".levels" "Full"
run_cmd ".types" "Backup"
run_cmd ".volstatus" "Append"
run_cmd ".schedule" "WeeklyCycle"

echo ""
echo "--- List ---"
run_cmd "list jobs" "BackupTest|Backup|Type"
run_cmd "list clients" "test-fd|Address"
run_cmd "list pools" "Full|Type"
run_cmd "list storages" "File|Address"
run_cmd "list schedules" "WeeklyCycle"

echo ""
echo "--- Show ---"
run_cmd "show all" "Director:|test-dir"
run_cmd "show director" "test-dir"
run_cmd "show clients" "test-fd"
run_cmd "show jobs" "BackupTest"
run_cmd "show storages" "File"
run_cmd "show pools" "Full"

echo ""
echo "--- Configure CRUD ---"
run_cmd "configure" "CRUD|add|delete"
run_cmd "configure add client name=new-fd address=10.0.0.5 password=x" "Created|new-fd"
run_cmd ".clients" "new-fd"
run_cmd "configure read client=new-fd" "new-fd|Client"
run_cmd "configure update client=new-fd address=10.0.0.99" "updated|10.0.0.99"
run_cmd "configure delete client=new-fd" "Deleted|new-fd"
run_cmd "configure export" "Director|test-dir"

echo ""
echo "--- Enable/Disable ---"
run_cmd "enable job=BackupTest" "enabled|already"
run_cmd "disable job=BackupTest" "disabled"
run_cmd "enable job=BackupTest" "enabled"

echo ""
echo "--- Admin ---"
run_cmd "messages" "pending|No"
run_cmd "memory" "Memory|memory"
run_cmd "whoami" "UserAgent"
run_cmd "wait" "running|No"
run_cmd "setdebug level=100" "100"
run_cmd "autodisplay on" "on"
run_cmd "gui on" "gui"
run_cmd "use" "catalog"
run_cmd ".api 0" "api|0"

echo ""
echo "--- Catalog Schema ---"
run_cmd ".catalog" "schema|version|tables"
run_cmd ".catalog version" "v00"
run_cmd ".catalog tables" "client|job"

echo ""
echo "--- Run (ohne FD/SD) ---"
run_cmd "run" "BackupTest|Available"
run_cmd "run job=BackupTest" "queued|Job:|BackupTest"

echo ""
echo "========================================"
printf "Ergebnis: \e[32m%d bestanden\e[0m, \e[31m%d fehlgeschlagen\e[0m, %d total\n" "$PASS" "$FAIL" "$TOTAL"
echo "========================================"

echo ""
echo "Director-Log Fehler:"
grep -i "error\|panic\|fatal" "$TMPDIR/director.log" | grep -v "skipping unknown" | head -5 || echo "  Keine Fehler"

exit $FAIL
