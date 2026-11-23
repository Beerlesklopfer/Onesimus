#!/usr/bin/env bash
# ─── Onesimus deploy script (Linux / macOS) ─────────────────────────────────
#
# Builds the Onesimus GUI and deploys it into the system paths, modelled on
# deploy.sh of forgeiec-studio: one verb per task, every verb builds what it
# needs first (CMake is incremental, a no-op build costs a second), and every
# privileged step raises itself with sudo on its own.
#
# deploy.fish is the fish twin of this script with the same verbs, flags and
# environment variables; deploy.ps1 does the same job on Windows.
#
# Usage:
#   ./deploy.sh                  build + deploy to /usr (sudo per step)
#   ./deploy.sh build            build only, nothing outside the build tree
#   ./deploy.sh --dry-run all    print every command instead of running it
#   ./deploy.sh help             the full verb table
#
# The build stamps the version: CMake's increment_build_number target
# increments .build_number and regenerates include/version.h on every build.
# Both files are tracked; do not commit the stamped values from a dev build.
set -euo pipefail

# ─── First: not as root ─────────────────────────────────────────────────────
#
# Before anything else. The script calls sudo itself for each privileged step.
# Under `sudo ./deploy.sh` CMake and the compiler run as root as well and leave
# a root-owned build tree (and a root-owned .build_number / version.h in the
# source tree) that the next build as a normal user cannot overwrite.
#
# ONESIMUS_ALLOW_ROOT=1 lifts the block (containers without a normal user).
if [ "$(id -u)" -ne 0 ]; then
    :
elif [ "${ONESIMUS_ALLOW_ROOT:-0}" = "1" ]; then
    printf '[deploy] ONESIMUS_ALLOW_ROOT=1 - continuing as root.\n' >&2
else
    printf '[deploy] Do not run this script as root.\n' >&2
    printf '[deploy]   It raises itself with sudo for every privileged step.\n' >&2
    printf '[deploy]   As root the build tree and version.h become root-owned.\n' >&2
    printf '[deploy]\n' >&2
    printf '[deploy]   Right:  ./deploy.sh %s\n' "${1:-<command>}" >&2
    printf '[deploy]   Container without a user:  ONESIMUS_ALLOW_ROOT=1 ./deploy.sh %s\n' "${1:-<command>}" >&2
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

OS="$(uname -s)"

# ─── Settings (the same environment variables as deploy.fish) ───────────────
cpu_count() {
    nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4
}
# *_SET: given explicitly (env or flag). An existing build tree keeps its own
# build type and OpenSSL mode unless one is given, so a plain `build` never
# switches a system-OpenSSL tree to a 10-minute static OpenSSL build.
BUILD_TYPE_SET=0; [ -n "${BUILD_TYPE:-}" ] && BUILD_TYPE_SET=1
OPENSSL_SET=0;    [ -n "${OPENSSL:-}" ] && OPENSSL_SET=1
BUILD_TYPE="${BUILD_TYPE:-Release}"
JOBS="${JOBS:-$(cpu_count)}"
BUILD_DIR="${BUILD_DIR:-build}"
# static: OpenSSL from the external/openssl submodule, linked statically (the
# default of the former build.sh). system: the distribution's libssl.
OPENSSL="${OPENSSL:-static}"
# OPENSSL_INSTALL_DIR: install prefix of the static OpenSSL build, passed to
# CMake when set. A prefix that already holds the libraries is reused, so one
# kept outside the build trees (CI cache, a shared ~/.cache folder) saves the
# OpenSSL build in every new tree, the AppImage tree included.
OPENSSL_INSTALL_DIR="${OPENSSL_INSTALL_DIR:-}"
# Target of `install` (cmake --install, sudo only when not writable).
PREFIX="${PREFIX:-/usr/local}"
# Target of the deploy step (all/onesimus): /usr/local, the old destination of
# "make install" (the .deb owns /usr). GNOME/KDE also read /usr/local/share.
DEPLOY_PREFIX="${DEPLOY_PREFIX:-/usr/local}"
# Generator for a NEW build tree. An existing tree keeps its generator: CMake
# refuses -G on a tree configured with another one.
if [ -z "${GENERATOR:-}" ] && command -v ninja >/dev/null 2>&1; then
    GENERATOR=Ninja
fi
GENERATOR="${GENERATOR:-}"
CLEAN="${CLEAN:-0}"
DRY_RUN="${DRY_RUN:-0}"

# ─── CLI flags (before or after the verb) ───────────────────────────────────
# The flags of the former build.sh stay valid with the same meaning.
POSITIONAL_ARGS=()
need_arg() { [ $# -ge 2 ] || { printf '[deploy] %s requires an argument\n' "$1" >&2; exit 1; }; }
while [ $# -gt 0 ]; do
    case "$1" in
        -t|--type)            need_arg "$@"; BUILD_TYPE="$2"; BUILD_TYPE_SET=1; shift 2 ;;
        -s|--system-openssl)  OPENSSL=system; OPENSSL_SET=1; shift ;;
        -c|--clean)           CLEAN=1; shift ;;
        -j|--jobs)            need_arg "$@"; JOBS="$2"; shift 2 ;;
        -j[0-9]*)             JOBS="${1#-j}"; shift ;;
        -d|--build-dir)       need_arg "$@"; BUILD_DIR="$2"; shift 2 ;;
        -n|--dry-run)         DRY_RUN=1; shift ;;
        -h|--help)            POSITIONAL_ARGS+=(help); shift ;;
        --)                   shift; POSITIONAL_ARGS+=("$@"); break ;;
        # Everything after `run`/`configure` belongs to onesimus/cmake.
        run|configure)        POSITIONAL_ARGS+=("$@"); break ;;
        *)                    POSITIONAL_ARGS+=("$1"); shift ;;
    esac
done
# `${arr[@]+...}`: macOS ships bash 3.2, where an empty array under `set -u`
# is an "unbound variable".
set -- ${POSITIONAL_ARGS[@]+"${POSITIONAL_ARGS[@]}"}

# ─── Colours and messages ───────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

info()      { echo -e "${CYAN}[deploy]${NC} $*"; }
ok()        { echo -e "${GREEN}[deploy]${NC} $*"; }
warn()      { echo -e "${YELLOW}[deploy]${NC} $*"; }
err()       { echo -e "${RED}[deploy]${NC} $*" >&2; }
die()       { err "$*"; exit 1; }
need_tool() { command -v "$1" >/dev/null 2>&1 || die "missing tool: $1"; }

# Every command that changes something goes through run/run_sudo, so that
# --dry-run shows exactly what a real run would execute and runs none of it.
run() {
    if [ "$DRY_RUN" = "1" ]; then
        printf '  + %s\n' "$*"
    else
        "$@"
    fi
}
run_sudo() {
    if [ "$DRY_RUN" = "1" ]; then
        printf '  + sudo %s\n' "$*"
    elif [ "$(id -u)" -eq 0 ]; then
        "$@"
    else
        need_tool sudo
        sudo "$@"
    fi
}
# sudo only when the target directory is not writable for us.
run_maybe_sudo() {   # <dir> <command...>
    local dir="$1"; shift
    if [ -w "$dir" ]; then run "$@"; else run_sudo "$@"; fi
}

# ─── Checks ─────────────────────────────────────────────────────────────────
validate_settings() {
    case "$BUILD_TYPE" in
        Release|Debug|RelWithDebInfo|MinSizeRel) ;;
        *) die "Invalid build type '$BUILD_TYPE' (Release, Debug, RelWithDebInfo, MinSizeRel)" ;;
    esac
    case "$OPENSSL" in
        static|system) ;;
        *) die "Invalid OPENSSL '$OPENSSL' (static or system)" ;;
    esac
    case "$JOBS" in
        ''|*[!0-9]*) die "Invalid job count '$JOBS'" ;;
    esac
}

check_prerequisites() {
    need_tool cmake
    if ! command -v qmake6 >/dev/null 2>&1 && ! command -v qmake >/dev/null 2>&1 \
       && [ -z "${CMAKE_PREFIX_PATH:-}" ]; then
        warn "Qt6 may not be installed (no qmake6/qmake in PATH)."
        if [ "$OS" = "Darwin" ]; then
            warn "  Install: brew install qt@6"
        else
            warn "  Install: sudo apt-get install qt6-base-dev qt6-tools-dev"
        fi
    fi
    if ! command -v lupdate >/dev/null 2>&1 && ! command -v lupdate-qt6 >/dev/null 2>&1 \
       && [ ! -x /usr/lib/qt6/bin/lupdate ]; then
        warn "Qt6 LinguistTools (lupdate/lrelease) not found. Install:"
        warn "  Ubuntu/Debian: sudo apt-get install qt6-tools-dev-tools"
        warn "  Fedora/RHEL:   sudo dnf install qt6-linguist"
        warn "  Arch:          sudo pacman -S qt6-tools"
        warn "  openSUSE:      sudo zypper install qt6-linguist-devel"
        warn "  macOS:         brew install qt@6"
    fi
}

# Homebrew's Qt is not on CMake's search path by default.
macos_prefix_path() {
    if [ "$OS" = "Darwin" ] && [ -z "${CMAKE_PREFIX_PATH:-}" ] && command -v brew >/dev/null 2>&1; then
        local qt; qt="$(brew --prefix qt@6 2>/dev/null || true)"
        [ -n "$qt" ] && [ -d "$qt" ] && export CMAKE_PREFIX_PATH="$qt"
    fi
    return 0
}

# The submodules the build needs: external/openssl only for a static OpenSSL,
# proto/ for the gRPC transport. wiki/ is not needed (and has an SSH URL).
init_submodules() {
    local paths=()
    if [ "$OPENSSL" = "static" ] && [ ! -f external/openssl/Configure ]; then
        paths+=(external/openssl)
    fi
    if [ ! -f proto/director_service.proto ]; then
        paths+=(proto)
    fi
    if [ ${#paths[@]} -gt 0 ]; then
        info "Initialising submodules: ${paths[*]}"
        run git submodule update --init --recursive "${paths[@]}" \
            || die "git submodule update failed"
    fi
}

app_bin() {
    if [ -x "${BUILD_DIR}/onesimus.app/Contents/MacOS/onesimus" ]; then
        echo "${BUILD_DIR}/onesimus.app/Contents/MacOS/onesimus"
    else
        echo "${BUILD_DIR}/onesimus"
    fi
}

# VERSION_FULL as stamped by the last build (include/version.h).
stamped_version() {
    sed -n 's/^#define VERSION_FULL *"\(.*\)"/\1/p' include/version.h 2>/dev/null | head -1
}

# ─── Development loop ───────────────────────────────────────────────────────
cmd_configure() {
    adopt_tree_settings
    validate_settings
    check_prerequisites
    macos_prefix_path
    init_submodules
    local static=ON
    [ "$OPENSSL" = "static" ] || static=OFF
    local gen=()
    if [ ! -f "${BUILD_DIR}/CMakeCache.txt" ] && [ -n "$GENERATOR" ]; then
        gen=(-G "$GENERATOR")
    fi
    info "Configuring ${BUILD_DIR} (${BUILD_TYPE}, OpenSSL ${OPENSSL}${GENERATOR:+, ${GENERATOR}})"
    run cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" ${gen[@]+"${gen[@]}"} \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DUSE_STATIC_OPENSSL="$static" \
        ${OPENSSL_INSTALL_DIR:+-DOPENSSL_INSTALL_DIR="$OPENSSL_INSTALL_DIR"} \
        ${CMAKE_PREFIX_PATH:+-DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH"} \
        "$@" || die "configure failed"
}

ensure_configured() {
    if [ "$CLEAN" = "1" ] && [ -d "$BUILD_DIR" ]; then
        info "Removing ${BUILD_DIR} (--clean)"
        run rm -rf "$BUILD_DIR"
        CLEAN=0
    fi
    if [ ! -f "${BUILD_DIR}/CMakeCache.txt" ]; then
        cmd_configure
    else
        adopt_tree_settings
        validate_settings
        # Only what was given explicitly is passed on to the existing tree.
        local args=()
        [ "$BUILD_TYPE_SET" = "1" ] && args+=(-DCMAKE_BUILD_TYPE="$BUILD_TYPE")
        if [ "$OPENSSL_SET" = "1" ]; then
            if [ "$OPENSSL" = "static" ]; then args+=(-DUSE_STATIC_OPENSSL=ON); init_submodules
            else args+=(-DUSE_STATIC_OPENSSL=OFF); fi
        fi
        if [ ${#args[@]} -gt 0 ]; then
            info "Re-configuring ${BUILD_DIR}: ${args[*]}"
            run cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" "${args[@]}" >/dev/null || die "re-configure failed"
        fi
    fi
}

# Read build type and OpenSSL mode of an existing tree when not given.
adopt_tree_settings() {
    local cache="${BUILD_DIR}/CMakeCache.txt" v
    [ -f "$cache" ] || return 0
    if [ "$BUILD_TYPE_SET" != "1" ]; then
        v="$(sed -n 's/^CMAKE_BUILD_TYPE:[A-Z]*=//p' "$cache" | head -1)"
        [ -n "$v" ] && BUILD_TYPE="$v"
    fi
    if [ "$OPENSSL_SET" != "1" ]; then
        v="$(sed -n 's/^USE_STATIC_OPENSSL:[A-Z]*=//p' "$cache" | head -1)"
        case "$v" in ON|on|TRUE|1) OPENSSL=static ;; OFF|off|FALSE|0) OPENSSL=system ;; esac
    fi
    return 0
}

cmd_build() {
    need_tool cmake
    ensure_configured
    if [ "$OPENSSL" = "static" ] && [ ! -d "${OPENSSL_INSTALL_DIR:-${BUILD_DIR}/openssl-install}/include/openssl" ]; then
        info "NOTE: OpenSSL is compiled on the first build - this may take 5-10 minutes."
    fi
    info "Building onesimus (${JOBS} jobs)"
    run cmake --build "$BUILD_DIR" -j "$JOBS" || die "build failed"
    if [ "$DRY_RUN" != "1" ]; then
        ok "Built $(app_bin)  (version $(stamped_version))"
        [ "$OPENSSL" = "static" ] && ok "  OpenSSL statically linked - no external OpenSSL libraries required"
    fi
    return 0
}

cmd_rebuild() {
    need_tool cmake
    ensure_configured
    info "Clean rebuild (${JOBS} jobs)"
    run cmake --build "$BUILD_DIR" --clean-first -j "$JOBS" || die "rebuild failed"
}

cmd_run() {
    cmd_build
    local bin; bin="$(app_bin)"
    if [ "$DRY_RUN" = "1" ]; then
        run "$bin" "$@"
        return 0
    fi
    [ -x "$bin" ] || die "binary not found: $bin"
    info "Running $(basename "$bin") $*"
    exec "$bin" "$@"
}

cmd_test() {
    cmd_build
    need_tool ctest
    info "Running tests (${JOBS} jobs, offscreen)"
    run env QT_QPA_PLATFORM=offscreen ctest --test-dir "$BUILD_DIR" -j "$JOBS" --output-on-failure \
        || die "tests failed"
}

# ─── Deploy: build + copy into the system paths ─────────────────────────────
#
# The real deployment, like deploy_forgeiec in forgeiec-studio: the binary and
# the files the .deb also ships (desktop entry, hicolor icons) are copied with
# sudo into DEPLOY_PREFIX (default /usr/local, where the old make install put it), then the desktop caches are refreshed
# so the launcher and Alt-Tab show the new icon at once.
# `install` replaces a symlink instead of writing through it. Say so, because
# a hand-made link (e.g. /usr/bin/onesimus -> /usr/local/bin/onesimus) would
# leave the link target at the old version.
warn_symlink() {
    if [ -L "$1" ]; then
        warn "  $1 is a symlink to $(readlink "$1") - it will be replaced by the binary."
        warn "  To update the link target instead: DEPLOY_PREFIX=$(dirname "$(dirname "$(readlink "$1")")") ./deploy.sh"
    fi
    return 0
}

deploy_onesimus() {
    info "=== onesimus ==="
    cmd_build
    local bin; bin="$(app_bin)"
    if [ "$DRY_RUN" != "1" ]; then
        [ -x "$bin" ] || die "binary not found: $bin"
    fi

    if [ "$OS" = "Darwin" ]; then
        # No .app bundle (the target is not MACOSX_BUNDLE): the binary goes to
        # DEPLOY_PREFIX/bin and finds Homebrew's Qt through its rpath.
        info "  onesimus → ${DEPLOY_PREFIX}/bin/onesimus"
        run_maybe_sudo "${DEPLOY_PREFIX}/bin" install -m 755 "$bin" "${DEPLOY_PREFIX}/bin/onesimus"
        [ "$DRY_RUN" = "1" ] || ok "onesimus deployed (version $(stamped_version))."
        return 0
    fi

    local share="${DEPLOY_PREFIX}/share"
    warn_symlink "${DEPLOY_PREFIX}/bin/onesimus"
    info "  onesimus → ${DEPLOY_PREFIX}/bin/onesimus"
    run_sudo install -d "${DEPLOY_PREFIX}/bin"
    run_sudo install -m 755 "$bin" "${DEPLOY_PREFIX}/bin/onesimus"

    # Generated by CMake from packaging/onesimus.desktop.in (configure_file).
    info "  desktop entry → ${share}/applications/onesimus.desktop"
    run_sudo install -d "${share}/applications"
    run_sudo install -m 644 "${BUILD_DIR}/onesimus.desktop" "${share}/applications/onesimus.desktop"

    info "  hicolor icons → ${share}/icons/hicolor/"
    run_sudo install -d "${share}/icons/hicolor/scalable/apps"
    run_sudo install -m 644 resources/icons/onesimus.svg "${share}/icons/hicolor/scalable/apps/onesimus.svg"
    # PNG sizes exist only when rsvg-convert/ImageMagick was found at configure.
    local png size
    for png in "${BUILD_DIR}"/icons/*x*/onesimus.png; do
        [ -f "$png" ] || continue
        size="$(basename "$(dirname "$png")")"
        run_sudo install -d "${share}/icons/hicolor/${size}/apps"
        run_sudo install -m 644 "$png" "${share}/icons/hicolor/${size}/apps/onesimus.png"
    done

    if command -v gtk-update-icon-cache >/dev/null 2>&1; then
        run_sudo gtk-update-icon-cache -q -f "${share}/icons/hicolor" || true
    fi
    if command -v update-desktop-database >/dev/null 2>&1; then
        run_sudo update-desktop-database -q "${share}/applications" || true
    fi
    [ "$DRY_RUN" = "1" ] || ok "onesimus deployed to ${DEPLOY_PREFIX} (version $(stamped_version))."
}

deploy_all() {
    info "=========================================="
    info "  Onesimus deploy (direct copy)"
    info "=========================================="
    [ "$DRY_RUN" = "1" ] && warn "DRY RUN - commands are printed, not executed."
    deploy_onesimus
    echo ""
    ok "=========================================="
    if [ "$DRY_RUN" = "1" ]; then
        ok "  Dry run finished - nothing was changed."
    else
        ok "  All deployed."
    fi
    ok "=========================================="
}

# ─── install / package / appimage ───────────────────────────────────────────
cmd_install() {
    cmd_build
    info "Installing to ${PREFIX} (cmake --install: binary, desktop entry, icons)"
    run_maybe_sudo "$PREFIX" cmake --install "$BUILD_DIR" --prefix "$PREFIX" || die "install failed"
    if [ "$OS" != "Darwin" ]; then
        command -v gtk-update-icon-cache >/dev/null 2>&1 && \
            run_maybe_sudo "$PREFIX" gtk-update-icon-cache -q -f "${PREFIX}/share/icons/hicolor" 2>/dev/null || true
        command -v update-desktop-database >/dev/null 2>&1 && \
            run_maybe_sudo "$PREFIX" update-desktop-database -q "${PREFIX}/share/applications" 2>/dev/null || true
    fi
    ok "installed to ${PREFIX}."
}

# CPack with the generator CMakeLists.txt sets per platform: DEB on Linux,
# DragNDrop (.dmg) on macOS. PACKAGE_GENERATORS overrides it (e.g. "DEB;RPM").
cmd_package() {
    need_tool cpack
    cmd_build
    # CPack does not clean its staging tree, and a tree left by an aborted run
    # makes the next run fail with a misleading RPATH_CHANGE error.
    run rm -rf "${BUILD_DIR}/_CPack_Packages"
    local gen=()
    [ -n "${PACKAGE_GENERATORS:-}" ] && gen=(-G "$PACKAGE_GENERATORS")
    info "Packaging (cpack${PACKAGE_GENERATORS:+ -G ${PACKAGE_GENERATORS}})"
    run cpack ${gen[@]+"${gen[@]}"} --config "${BUILD_DIR}/CPackConfig.cmake" -B "$BUILD_DIR" \
        || die "cpack failed"
    [ "$DRY_RUN" = "1" ] && return 0
    info "Produced packages:"
    local f found=0
    for f in "$BUILD_DIR"/*.deb "$BUILD_DIR"/*.rpm "$BUILD_DIR"/*.dmg; do
        [ -f "$f" ] || continue
        found=1
        echo "    $(du -h "$f" | cut -f1)  $f"
    done
    [ "$found" = "1" ] || warn "no package found in ${BUILD_DIR}"
    if ls "$BUILD_DIR"/*.deb >/dev/null 2>&1; then
        echo "Install: sudo apt install $(cd "$BUILD_DIR" && pwd)/$(basename "$(ls -1t "$BUILD_DIR"/*.deb | head -1)")"
    fi
}

# linuxdeploy + Qt plugin in a fixed version, checked against SHA-256 (the
# same pins as forgeiec-studio). Change tag AND checksum together.
LINUXDEPLOY_TAG="1-alpha-20250213-2"
LINUXDEPLOY_SHA256="4648f278ab3ef31f819e67c30d50f462640e5365a77637d7e6f2ad9fd0b4522a"
LINUXDEPLOY_QT_TAG="1-alpha-20250213-1"
LINUXDEPLOY_QT_SHA256="15106be885c1c48a021198e7e1e9a48ce9d02a86dd0a1848f00bdbf3c1c92724"
APPIMAGE_TOOLS="${APPIMAGE_TOOLS:-${XDG_CACHE_HOME:-$HOME/.cache}/onesimus/appimage-tools}"

fetch_appimage_tool() {   # <file name> <URL> <sha256>
    local file="${APPIMAGE_TOOLS}/$1"
    if [ -f "$file" ] && echo "$3  $file" | sha256sum -c --status 2>/dev/null; then
        chmod +x "$file"
        return 0
    fi
    info "  downloading $1"
    run mkdir -p "$APPIMAGE_TOOLS"
    [ "$DRY_RUN" = "1" ] && { run curl -fsSL -o "$file" "$2"; return 0; }
    curl -fsSL -o "${file}.part" "$2" || { rm -f "${file}.part"; die "download failed: $2"; }
    echo "$3  ${file}.part" | sha256sum -c --status \
        || { rm -f "${file}.part"; die "SHA-256 mismatch: $1"; }
    mv "${file}.part" "$file"
    chmod +x "$file"
}

# Own build tree (<BUILD_DIR>-appimage): it installs with PREFIX=/usr into an
# AppDir, a dev tree may be configured any way.
cmd_appimage() {
    [ "$OS" = "Linux" ] || die "appimage: Linux only"
    need_tool cmake; need_tool curl; need_tool sha256sum
    validate_settings
    local qmake; qmake="$(command -v qmake6 || command -v qmake || true)"
    [ -n "$qmake" ] || die "missing tool: qmake6"
    local arch; arch="$(uname -m)"
    [ "$arch" = "x86_64" ] || die "appimage: only x86_64 is prepared (here: ${arch})"
    # Absolute: linuxdeploy runs inside dist/appimage, a relative AppDir
    # (the default BUILD_DIR=build) would point nowhere there.
    local base="$BUILD_DIR"
    case "$base" in /*) ;; *) base="${SCRIPT_DIR}/${base}" ;; esac
    local dir="${base}-appimage" appdir="${base}-appimage/AppDir"
    local out="${SCRIPT_DIR}/dist/appimage"
    local static=ON
    [ "$OPENSSL" = "static" ] || static=OFF
    init_submodules

    # linuxdeploy never overwrites what is already in the AppDir.
    run rm -rf "$appdir"
    local gen=()
    if [ ! -f "${dir}/CMakeCache.txt" ] && [ -n "$GENERATOR" ]; then gen=(-G "$GENERATOR"); fi
    info "Building onesimus for AppImage (Release, PREFIX=/usr, OpenSSL ${OPENSSL})"
    run cmake -S "$SCRIPT_DIR" -B "$dir" ${gen[@]+"${gen[@]}"} -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr -DUSE_STATIC_OPENSSL="$static" -DBUILD_TESTING=OFF \
        ${OPENSSL_INSTALL_DIR:+-DOPENSSL_INSTALL_DIR="$OPENSSL_INSTALL_DIR"} \
        || die "appimage configure failed"
    run cmake --build "$dir" -j "$JOBS" || die "appimage build failed"
    run env DESTDIR="$appdir" cmake --install "$dir" || die "appimage install failed"

    fetch_appimage_tool linuxdeploy-x86_64.AppImage \
        "https://github.com/linuxdeploy/linuxdeploy/releases/download/${LINUXDEPLOY_TAG}/linuxdeploy-x86_64.AppImage" \
        "$LINUXDEPLOY_SHA256"
    fetch_appimage_tool linuxdeploy-plugin-qt-x86_64.AppImage \
        "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/${LINUXDEPLOY_QT_TAG}/linuxdeploy-plugin-qt-x86_64.AppImage" \
        "$LINUXDEPLOY_QT_SHA256"

    local version; version="$(stamped_version)"
    [ -n "$version" ] || version="0.0.0"
    run mkdir -p "$out"
    info "linuxdeploy (Qt plugin, version ${version})"
    # APPIMAGE_EXTRACT_AND_RUN: the tools are AppImages themselves and would
    # need FUSE otherwise. EXTRA_PLATFORM_PLUGINS: without it only xcb is
    # bundled (no Wayland, no offscreen).
    if [ "$DRY_RUN" = "1" ]; then
        run linuxdeploy-x86_64.AppImage --appdir "$appdir" --plugin qt \
            --executable "${appdir}/usr/bin/onesimus" \
            --desktop-file "${appdir}/usr/share/applications/onesimus.desktop" \
            --icon-file "${appdir}/usr/share/icons/hicolor/scalable/apps/onesimus.svg" --output appimage
        return 0
    fi
    ( cd "$out" && \
      env ARCH=x86_64 QMAKE="$qmake" APPIMAGE_EXTRACT_AND_RUN=1 \
          EXTRA_PLATFORM_PLUGINS="libqwayland-egl.so;libqwayland-generic.so;libqoffscreen.so" \
          LINUXDEPLOY_OUTPUT_VERSION="$version" \
          PATH="${APPIMAGE_TOOLS}:${PATH}" \
      "${APPIMAGE_TOOLS}/linuxdeploy-x86_64.AppImage" \
          --appdir "$appdir" \
          --plugin qt \
          --executable "${appdir}/usr/bin/onesimus" \
          --desktop-file "${appdir}/usr/share/applications/onesimus.desktop" \
          --icon-file "${appdir}/usr/share/icons/hicolor/scalable/apps/onesimus.svg" \
          --output appimage ) || die "linuxdeploy failed"
    local img; img="$(ls -1t "$out"/*.AppImage 2>/dev/null | head -1)"
    [ -f "$img" ] || die "no AppImage in ${out}"
    ok "AppImage: ${img} ($(du -h "$img" | cut -f1))"
    echo "  sha256: $(sha256sum "$img" | cut -d' ' -f1)"
}

cmd_clean() {
    info "Removing ${BUILD_DIR} ${BUILD_DIR}-appimage"
    run rm -rf "$BUILD_DIR" "${BUILD_DIR}-appimage"
}

show_status() {
    echo ""
    info "Deployed:"
    local inst="${DEPLOY_PREFIX}/bin/onesimus"
    if [ -x "$inst" ]; then
        ok "  ${inst}  ($(date -r "$inst" '+%Y-%m-%d %H:%M' 2>/dev/null || echo '?'))"
    else
        warn "  ${inst} not present"
    fi
    info "Build tree ${BUILD_DIR}:"
    local bin; bin="$(app_bin)"
    if [ -x "$bin" ]; then
        ok "  ${bin}  ($(date -r "$bin" '+%Y-%m-%d %H:%M' 2>/dev/null || echo '?'))"
        [ -f "${BUILD_DIR}/CMakeCache.txt" ] && \
            ok "  $(grep -E '^CMAKE_BUILD_TYPE:' "${BUILD_DIR}/CMakeCache.txt" | cut -d= -f2), OpenSSL static: $(grep -E '^USE_STATIC_OPENSSL:' "${BUILD_DIR}/CMakeCache.txt" | cut -d= -f2)"
    else
        warn "  no binary (./deploy.sh build)"
    fi
    info "Stamped version (include/version.h): $(stamped_version)"
    local f
    for f in "$BUILD_DIR"/*.deb "$BUILD_DIR"/*.rpm "$BUILD_DIR"/*.dmg dist/appimage/*.AppImage; do
        [ -f "$f" ] && info "Package: $f"
    done
    return 0
}

# The common verb table. It is the same in deploy.fish - checked mechanically:
#
#   diff <(bash deploy.sh help   | sed -E 's/deploy\.(sh|fish)/deploy.X/g') \
#        <(fish deploy.fish help | sed -E 's/deploy\.(sh|fish)/deploy.X/g')
show_help() {
    cat <<'EOF'

Usage:  ./deploy.sh [flags] [command] [args]
        ./deploy.fish [flags] [command] [args]   - the same commands

Deploy (build + copy into DEPLOY_PREFIX with sudo per step):
  all | onesimus | gui   Build, then copy binary, desktop entry and icons
                         to /usr (Linux) or /usr/local/bin (macOS) - default
  status                 What is deployed, built and packaged

Development loop (touches the system only with install):
  configure [args]       Create the build tree (extra args go to cmake)
  build                  Build onesimus only
  rebuild                Clean rebuild (--clean-first)
  run [args]             Build and start onesimus
  test                   Build and run ctest (offscreen)
  install                cmake --install into PREFIX (sudo if not writable)
  package                CPack: .deb on Linux, .dmg on macOS
  appimage               AppImage -> dist/appimage/ (Linux x86_64)
  clean                  Remove the build trees
  help                   This table

Flags (same as the former build.sh; before or after the command, but
       everything after run/configure goes to onesimus/cmake):
  -t, --type TYPE        Release (default), Debug, RelWithDebInfo, MinSizeRel
  -s, --system-openssl   System OpenSSL instead of the static submodule
  -c, --clean            Remove the build directory before building
  -j, --jobs N           Parallel jobs (default: number of CPUs)
  -d, --build-dir DIR    Build directory (default: build)
  -n, --dry-run          Print every command instead of running it
  -h, --help             This table

Env: BUILD_DIR, BUILD_TYPE, JOBS, OPENSSL (static|system), GENERATOR, PREFIX,
     DEPLOY_PREFIX, CMAKE_PREFIX_PATH, PACKAGE_GENERATORS, APPIMAGE_TOOLS,
     OPENSSL_INSTALL_DIR, CLEAN, DRY_RUN, ONESIMUS_ALLOW_ROOT

Every build increments .build_number and regenerates include/version.h.

Examples:
  ./deploy.sh                     # build + deploy to /usr
  ./deploy.sh -n                  # show what a deploy would run
  ./deploy.sh build -t Debug      # Debug build, no deploy
  ./deploy.sh build -s -c         # system OpenSSL, clean rebuild
  ./deploy.sh package             # .deb / .dmg
EOF
}

# ─── Main ───────────────────────────────────────────────────────────────────
case "${1:-all}" in
    all|onesimus|gui)  deploy_all ;;
    configure)         shift; cmd_configure "$@" ;;
    build)             cmd_build ;;
    rebuild)           cmd_rebuild ;;
    run)               shift; cmd_run "$@" ;;
    test)              cmd_test ;;
    install)           cmd_install ;;
    package)           cmd_package ;;
    appimage)          cmd_appimage ;;
    clean)             cmd_clean ;;
    status)            show_status ;;
    help)              show_help ;;
    *)
        err "Unknown command: $1"
        show_help
        exit 1
        ;;
esac
