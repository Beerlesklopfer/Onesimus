#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── The source archives of a release are all there ─────────────────────────
#
#   scripts/ci/check-source-assets.sh DIR VERSION QT_VERSION
#
# Run by release.yml before the draft release is created: fails when one of
# the source archives THIRD-PARTY-NOTICES.md promises is missing or empty in
# DIR - whole, or split by split-asset.sh into non-empty .partNN files with
# a .sha256 of the whole file.
set -euo pipefail

dir="${1:?usage: $0 DIR VERSION QT_VERSION}"
version="${2:?usage: $0 DIR VERSION QT_VERSION}"
qt="${3:?usage: $0 DIR VERSION QT_VERSION}"
assets=(
    "onesimus-${version}-src.tar.xz"
    "onesimus-${version}-appimage-debian-sources.tar"
    "onesimus-${version}-qt-${qt}-sources.tar"
)
bad=0
for a in "${assets[@]}"; do
    if [ -s "$dir/$a" ]; then
        echo "check-source-assets: $a ($(du -h "$dir/$a" | cut -f1))"
        continue
    fi
    parts=("$dir/$a".part*)
    if [ -e "${parts[0]}" ] && [ -s "$dir/$a.sha256" ]; then
        for p in "${parts[@]}"; do
            [ -s "$p" ] || { echo "  empty part: $p"; bad=1; }
        done
        echo "check-source-assets: $a (${#parts[@]} parts)"
        continue
    fi
    echo "  missing or empty: $a"
    bad=1
done
[ "$bad" = "0" ] || { echo "::error title=Source archives::a source archive is missing (see above) - not drafting the release" >&2; exit 1; }
