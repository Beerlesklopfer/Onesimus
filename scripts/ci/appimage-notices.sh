#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── Notices of the libraries linuxdeploy bundled into an AppDir ────────────
#
#   scripts/ci/appimage-notices.sh APPDIR
#
# Run by deploy.sh/deploy.fish appimage between the linuxdeploy pass that
# bundles Qt and the libraries (it copies the Debian copyright file of each
# one to usr/share/doc/<package>/copyright) and the pass that writes the
# AppImage (see THIRD-PARTY-NOTICES.md):
#  - the licence texts those copyright files name as
#    /usr/share/common-licenses/<name> go to usr/share/common-licenses/ in
#    the AppDir;
#  - usr/share/doc/onesimus/bundled-packages.txt names the Debian binary
#    package, its version, source package and source version of every
#    bundled library, so that their source code can be found.
set -euo pipefail

appdir="${1:?usage: $0 APPDIR}"
[ -d "$appdir/usr/share/doc" ] || { echo "appimage-notices: $appdir/usr/share/doc missing (run linuxdeploy first)" >&2; exit 1; }

if [ -d /usr/share/common-licenses ]; then
    mkdir -p "$appdir/usr/share/common-licenses"
    cp -a /usr/share/common-licenses/. "$appdir/usr/share/common-licenses/"
else
    echo "appimage-notices: warning: no /usr/share/common-licenses - the licence texts of the bundled libraries are missing" >&2
fi

if ! command -v dpkg-query >/dev/null 2>&1; then
    echo "appimage-notices: warning: no dpkg-query - bundled-packages.txt not written" >&2
    exit 0
fi
list="$appdir/usr/share/doc/onesimus/bundled-packages.txt"
arch="$(dpkg --print-architecture)"
fmt='${binary:Package} ${Version} ${source:Package} ${source:Version}\n'
mkdir -p "$(dirname "$list")"
{
    echo "# Debian packages bundled in this AppImage by linuxdeploy:"
    echo "# binary package, version, source package, source version."
    echo "# Source code: https://snapshot.debian.org/package/<source>/<source version>/"
    for doc in "$appdir"/usr/share/doc/*/; do
        pkg="$(basename "$doc")"
        [ "$pkg" = "onesimus" ] && continue
        # The package of this architecture (other architectures may be
        # installed too), else an Architecture: all package
        dpkg-query -W -f="$fmt" "$pkg:$arch" 2>/dev/null \
            || dpkg-query -W -f="$fmt" "$pkg" 2>/dev/null \
            || echo "$pkg (not known to dpkg-query)"
    done
} > "$list"
echo "appimage-notices: $(grep -vc '^#' "$list") bundled packages listed in usr/share/doc/onesimus/bundled-packages.txt"
