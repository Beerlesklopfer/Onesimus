#!/usr/bin/env bash
# Writes the screenshots of the website from the simulator onesimus-sim.
#
#   tools/screenshots.sh [TARGET_DIR]
#
# TARGET_DIR receives <lang>/<name>.png for en and de (default: the website
# worktree, static/img/screens). For every language a fresh simulator is
# started with the scenario "screens", a fixed seed and start time, and the
# harness onesimus_screens connects to it over gRPC (127.0.0.1 only), opens
# the views and dialogs and grabs them. The harness keeps its settings in a
# directory of its own (/tmp/onesimus-screens) and never reads or writes the
# settings or profiles of the user.
#
# Environment (all optional):
#   SIM_DIR      the onesimus-sim repository, built (default ~/Dokumente/Projekte/onesimus-sim)
#   BUILD_DIR    CMake build directory of the GUI (default ~/.cache/onesimus-agents/build-screenshots);
#                configured with -DONESIMUS_BUILD_SCREENS=ON and built when the harness is missing
#   GRPC_PORT    gRPC port of the simulator (default 11101)
#   BNET_PORT    its Director (BNET) port, also the port of the GUI's profile (default 11100; not
#                GRPC_PORT-1000, which is the gRPC port of a simulator on the default ports)
#   SCENARIO     scenario of the simulator (default screens)
#   SIM_START    virtual start time (default "2026-11-20 23:40", Europe/Berlin)
#   SIM_SEED     history seed (default 7)
#   SIM_SPEED    virtual seconds per real second (default 1: the GUI's clock runs along)
#   LANGS        languages (default "en de")
#   ONLY         comma separated shot names (default all); the status bar shots
#                (statusbar-usage, statusbar-near, statusbar-licence, statusbar-expired)
#                run each with a simulator of their own (--usage free, near,
#                licence-ending, expired) in the light theme
#   WORK_DIR     scratch directory (default /tmp/onesimus-screens; its content is replaced)
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo="$(cd "$here/.." && pwd)"

TARGET_DIR="${1:-$HOME/.cache/onesimus-agents/website-site/static/img/screens}"
SIM_DIR="${SIM_DIR:-$HOME/Dokumente/Projekte/onesimus-sim}"
BUILD_DIR="${BUILD_DIR:-$HOME/.cache/onesimus-agents/build-screenshots}"
GRPC_PORT="${GRPC_PORT:-11101}"
BNET_PORT="${BNET_PORT:-11100}"
SCENARIO="${SCENARIO:-screens}"
SIM_START="${SIM_START:-2026-11-20 23:40}"
SIM_SEED="${SIM_SEED:-7}"
SIM_SPEED="${SIM_SPEED:-1}"
LANGS="${LANGS:-en de}"
ONLY="${ONLY:-}"
WORK_DIR="${WORK_DIR:-/tmp/onesimus-screens}"
SIM_TZ="Europe/Berlin"
PASSWORD="demo"   # the demo console "admin" of the scenario, not a secret

harness="$BUILD_DIR/onesimus_screens"
clocklib="$BUILD_DIR/libonesimus_screens_clock.so"

die() { echo "screenshots.sh: $*" >&2; exit 1; }

[ -f "$SIM_DIR/dist/src/cli/main.js" ] || die "simulator not built: $SIM_DIR/dist (npm install && npm run build)"
if [ ! -x "$harness" ] || [ ! -f "$clocklib" ]; then
    echo "Building onesimus_screens in $BUILD_DIR ..."
    nice -n 19 cmake -S "$repo" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -DONESIMUS_BUILD_SCREENS=ON >/dev/null
    nice -n 19 ionice -c3 cmake --build "$BUILD_DIR" --target onesimus_screens -j4
fi
for port in "$GRPC_PORT" "$BNET_PORT"; do
    if ss -ltn 2>/dev/null | grep -q ":$port "; then
        die "port $port is in use: set GRPC_PORT / BNET_PORT to free ports"
    fi
done

# The work directory: ours only (marker), replaced on every run
if [ -e "$WORK_DIR" ]; then
    [ -f "$WORK_DIR/.onesimus-screens-work" ] || die "$WORK_DIR exists and is not ours"
    rm -rf "$WORK_DIR"
fi
mkdir -p "$WORK_DIR"
touch "$WORK_DIR/.onesimus-screens-work"

# A certificate of its own, valid from 2020 to 2040: the GUI runs with the
# clock of the simulator (2026-11-20 23:40), which may lie before "now"
certdir="$WORK_DIR/cert"
mkdir -p "$certdir"
openssl req -x509 -newkey ec -pkeyopt ec_paramgen_curve:prime256v1 -nodes \
    -keyout "$certdir/key.pem" -out "$certdir/cert.pem" \
    -not_before 20200101000000Z -not_after 20400101000000Z -subj /CN=onesimus-sim \
    -addext "subjectAltName=DNS:localhost,DNS:onesimus-sim,IP:127.0.0.1,IP:::1" 2>/dev/null
fingerprint="$(openssl x509 -in "$certdir/cert.pem" -outform DER | sha256sum | cut -d' ' -f1)"

start_epoch="$(TZ="$SIM_TZ" date -d "$SIM_START" +%s)"
simpid=""
stop_sim() {
    if [ -n "$simpid" ] && kill -0 "$simpid" 2>/dev/null; then
        kill "$simpid" 2>/dev/null || true
        wait "$simpid" 2>/dev/null || true
    fi
    simpid=""
}
trap stop_sim EXIT

# The status bar shots: each a run of its own, the simulator's --usage
# preset decides what the Director reports; light theme
statusbar_usage() {
    case "$1" in
        statusbar-usage) echo free ;;
        statusbar-near) echo near ;;
        statusbar-licence) echo licence-ending ;;
        statusbar-expired) echo expired ;;
    esac
}
STATUSBAR_SHOTS="statusbar-usage statusbar-near statusbar-licence statusbar-expired"

# run_shots LANG ONLY THEME [simulator arguments...]: a fresh simulator, one
# harness run; ONLY may be empty (all shots but the status bar ones)
run_shots() {
    local lang="$1" only="$2" theme="$3"
    shift 3
    local out="$WORK_DIR/shots/$lang"
    local simlog="$WORK_DIR/sim-$lang.log"
    mkdir -p "$out"

    # A fresh simulator per run: every run sees the same moment of the night
    local real_start
    real_start="$(date +%s)"
    ( cd "$SIM_DIR" && exec node dist/src/cli/main.js serve --scenario "$SCENARIO" \
        --host 127.0.0.1 --grpc-port "$GRPC_PORT" --bnet-port "$BNET_PORT" \
        --seed "$SIM_SEED" --start "$SIM_START" --speed "$SIM_SPEED" --cert-dir "$certdir" "$@" ) >"$simlog" 2>&1 &
    simpid=$!
    for _ in $(seq 1 100); do
        grep -q "gRPC (TLS) on" "$simlog" 2>/dev/null && break
        kill -0 "$simpid" 2>/dev/null || { cat "$simlog" >&2; die "the simulator did not start"; }
        sleep 0.1
    done
    grep -q "$fingerprint" "$simlog" || { cat "$simlog" >&2; die "the simulator uses another certificate"; }

    # The GUI's wall clock follows the virtual clock of the simulator
    local offset=$(( start_epoch - real_start ))
    echo "== $lang${only:+ ($only)}: simulator pid $simpid, clock offset ${offset}s"
    local only_arg=()
    [ -n "$only" ] && only_arg=(--only "$only")
    if ! TZ="$SIM_TZ" LANG=C.UTF-8 LC_ALL=C.UTF-8 \
         ONESIMUS_SCREENS_PASSWORD="$PASSWORD" ONESIMUS_SCREENS_CLOCK_OFFSET="$offset" \
         LD_PRELOAD="$clocklib" \
         "$harness" --home "$WORK_DIR/home/demo" --out "$out" --lang "$lang" --theme "$theme" \
             --host 127.0.0.1 --port "$BNET_PORT" --grpc-port "$GRPC_PORT" \
             --console admin --director demo-dir --fingerprint "$fingerprint" "${only_arg[@]}" \
             2> >(grep -v -e '^Mutable view on type' -e 'propagateSizeHints' -e 'does not support raise' >&2); then
        echo "screenshots.sh: some shots of $lang${only:+ ($only)} failed" >&2
        status=1
    fi
    stop_sim
}

wanted() { [ -z "$ONLY" ] || [[ ",$ONLY," == *",$1,"* ]]; }

status=0
for lang in $LANGS; do
    out="$WORK_DIR/shots/$lang"
    # The views and dialogs (dark theme); the status bar shots are left out
    main_only=""
    if [ -n "$ONLY" ]; then
        for name in ${ONLY//,/ }; do
            [ -n "$(statusbar_usage "$name")" ] || main_only="${main_only:+$main_only,}$name"
        done
    fi
    if [ -z "$ONLY" ] || [ -n "$main_only" ]; then
        # The Director reports the free tier usage: the label in the status bar
        run_shots "$lang" "$main_only" dark --usage free
    fi
    for name in $STATUSBAR_SHOTS; do
        wanted "$name" || continue
        run_shots "$lang" "$name" light --usage "$(statusbar_usage "$name")"
    done

    mkdir -p "$TARGET_DIR/$lang"
    compgen -G "$out/*.png" >/dev/null || { echo "screenshots.sh: no pictures for $lang" >&2; status=1; continue; }
    cp "$out"/*.png "$TARGET_DIR/$lang/"
    echo "== $lang: $(ls "$out" | wc -l) pictures -> $TARGET_DIR/$lang"
done
exit $status
