#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── Third-party notices in a package ───────────────────────────────────────
#
# Fails when a built package lacks THIRD-PARTY-NOTICES.md, LICENSE or one of
# the licence texts the notices link (LICENSES/<id>.txt, read from the
# notices of the source tree). Used by ci.yml and release.yml.
#
#   scripts/ci/check-notices.sh PACKAGE...
#
# PACKAGE is one of
#   *.deb        /usr/share/doc/onesimus/            (dpkg-deb -c)
#   *.AppImage   usr/share/doc/onesimus/             (--appimage-extract)
#   *.zip        next to onesimus.exe                (unzip -l; Windows ZIP)
#   *.dmg        Onesimus.app/Contents/Resources/    (hdiutil, macOS only)
#   *.app        Contents/Resources/                 (a bundle directory)
#   a directory  an AppDir (usr/share/doc/onesimus/) or a Windows deploy
#                folder (next to onesimus.exe)
# The MSI is checked by check-notices.ps1 (Windows Installer database).
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
NOTICES="$SRC_DIR/THIRD-PARTY-NOTICES.md"

die()  { echo "::error title=check-notices::$*" >&2; exit 1; }
info() { echo "[check-notices] $*"; }

[ $# -gt 0 ] || die "usage: $0 PACKAGE..."
[ -f "$NOTICES" ] || die "no $NOTICES"

# The files every package needs, relative to its notices directory
REQUIRED=(THIRD-PARTY-NOTICES.md LICENSE)
while IFS= read -r f; do REQUIRED+=("$f"); done \
    < <(grep -o 'LICENSES/[A-Za-z0-9.+-]*\.txt' "$NOTICES" | sort -u)
[ ${#REQUIRED[@]} -gt 2 ] || die "the notices link no LICENSES/*.txt"
for f in "${REQUIRED[@]}"; do
    [ -f "$SRC_DIR/$f" ] || die "$f is linked in THIRD-PARTY-NOTICES.md but not in the source tree"
done

WORK="$(mktemp -d "${RUNNER_TEMP:-${TMPDIR:-/tmp}}/check-notices.XXXXXX")"
MOUNT=""
cleanup() {
    [ -n "$MOUNT" ] && hdiutil detach "$MOUNT" -quiet >/dev/null 2>&1 || true
    rm -rf "$WORK"
}
trap cleanup EXIT

# check_list <package> <prefix> <listing file>: every required file must be
# a line "<prefix><file>" of the listing.
check_list() {
    local pkg="$1" prefix="$2" list="$3" f missing=0
    for f in "${REQUIRED[@]}"; do
        if ! grep -qxF "${prefix}${f}" "$list"; then
            echo "  missing: ${prefix}${f}"
            missing=1
        fi
    done
    [ "$missing" = "0" ] || die "$pkg lacks third-party notices (see above)"
    info "ok: $pkg (${#REQUIRED[@]} files in ${prefix:-/})"
}

# check_dir <package> <dir>: every required file must exist under dir
check_dir() {
    local pkg="$1" dir="$2" f missing=0
    for f in "${REQUIRED[@]}"; do
        if [ ! -s "$dir/$f" ]; then
            echo "  missing: $dir/$f"
            missing=1
        fi
    done
    [ "$missing" = "0" ] || die "$pkg lacks third-party notices (see above)"
    cmp -s "$dir/THIRD-PARTY-NOTICES.md" "$NOTICES" \
        || die "$pkg: THIRD-PARTY-NOTICES.md differs from the source tree"
    info "ok: $pkg (${#REQUIRED[@]} files in ${dir})"
}

for pkg in "$@"; do
    [ -e "$pkg" ] || die "no such package: $pkg"
    case "$pkg" in
        *.deb)
            command -v dpkg-deb >/dev/null || die "dpkg-deb missing"
            dpkg-deb -c "$pkg" | awk '{print $NF}' > "$WORK/list"
            check_list "$pkg" "./usr/share/doc/onesimus/" "$WORK/list"
            ;;
        *.AppImage)
            abs="$(cd "$(dirname "$pkg")" && pwd)/$(basename "$pkg")"
            rm -rf "$WORK/appimage"; mkdir -p "$WORK/appimage"
            chmod +x "$abs"
            ( cd "$WORK/appimage" && "$abs" --appimage-extract >/dev/null ) \
                || die "$pkg: --appimage-extract failed"
            check_dir "$pkg" "$WORK/appimage/squashfs-root/usr/share/doc/onesimus"
            ;;
        *.zip)
            command -v unzip >/dev/null || die "unzip missing"
            # Compress-Archive may write backslashes; the notices must lie in
            # the folder of onesimus.exe.
            unzip -Z1 "$pkg" | tr '\\' '/' > "$WORK/list"
            exe="$(grep -i -m1 '\(^\|/\)onesimus\.exe$' "$WORK/list" || true)"
            [ -n "$exe" ] || die "$pkg: no onesimus.exe"
            prefix="${exe%/*}/"; [ "$prefix" = "$exe/" ] && prefix=""
            check_list "$pkg" "$prefix" "$WORK/list"
            ;;
        *.dmg)
            [ "$(uname -s)" = "Darwin" ] || die "$pkg: a dmg is checked on macOS only"
            MOUNT="$WORK/mnt"; mkdir -p "$MOUNT"
            hdiutil attach -nobrowse -readonly -noautoopen -mountpoint "$MOUNT" "$pkg" >/dev/null \
                || die "$pkg: hdiutil attach failed"
            check_dir "$pkg" "$MOUNT/Onesimus.app/Contents/Resources"
            hdiutil detach "$MOUNT" -quiet || true
            MOUNT=""
            ;;
        *.app|*.app/)
            check_dir "$pkg" "${pkg%/}/Contents/Resources"
            ;;
        *)
            if [ -d "$pkg/usr/share/doc/onesimus" ]; then
                check_dir "$pkg" "$pkg/usr/share/doc/onesimus"
            elif [ -d "$pkg" ] && ls "$pkg" | grep -qix 'onesimus.exe'; then
                check_dir "$pkg" "$pkg"
            else
                die "$pkg: unknown package type"
            fi
            ;;
    esac
done
