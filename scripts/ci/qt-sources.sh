#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── Qt source code of the Windows and macOS packages ───────────────────────
#
#   scripts/ci/qt-sources.sh QT_VERSION OUT.tar MODULE...
#
# Downloads <module>-everywhere-src-QT_VERSION.tar.xz of each MODULE from
# download.qt.io (archive/qt/<major.minor>/QT_VERSION/submodules/), checks it
# against the .sha256 file published next to it and against md5sums.txt of
# that folder, and packs the tarballs with both checksum lists into one
# uncompressed tar (a folder named after OUT.tar). release.yml passes the Qt
# version of the Windows/macOS builds (env QT_VERSION) and the modules whose
# libraries and plugins those packages ship (env QT_SOURCE_MODULES; the
# packaging jobs check that list with check-qt-modules.sh).
set -euo pipefail

ver="${1:?usage: $0 QT_VERSION OUT.tar MODULE...}"
out="${2:?usage: $0 QT_VERSION OUT.tar MODULE...}"
shift 2
[ $# -gt 0 ] || { echo "qt-sources: no modules given" >&2; exit 1; }
[[ "$ver" =~ ^([0-9]+\.[0-9]+)\.[0-9]+$ ]] || { echo "qt-sources: bad Qt version '$ver'" >&2; exit 1; }
base="https://download.qt.io/archive/qt/${BASH_REMATCH[1]}/${ver}/submodules"

out_dir="$(cd "$(dirname "$out")" && pwd)"
top="$(basename "$out" .tar)"
work="$(mktemp -d "${RUNNER_TEMP:-${TMPDIR:-/tmp}}/qt-sources.XXXXXX")"
trap 'rm -rf "$work"' EXIT
mkdir -p "$work/$top"
cd "$work/$top"

curl -fsSL --retry 3 -o md5sums.txt "$base/md5sums.txt"
: > SHA256SUMS
for module in "$@"; do
    file="${module}-everywhere-src-${ver}.tar.xz"
    echo "qt-sources: $file"
    curl -fsSL --retry 3 -o "$file" "$base/$file"
    sha="$(curl -fsSL --retry 3 "$base/$file.sha256" | awk '{print $1; exit}')"
    [[ "$sha" =~ ^[0-9a-f]{64}$ ]] || { echo "qt-sources: no SHA-256 for $file" >&2; exit 1; }
    echo "$sha  $file" | sha256sum -c --status || { echo "qt-sources: SHA-256 mismatch: $file" >&2; exit 1; }
    grep -E "  ${file//./\\.}\$" md5sums.txt | md5sum -c --status \
        || { echo "qt-sources: MD5 (md5sums.txt) mismatch or missing: $file" >&2; exit 1; }
    echo "$sha  $file" >> SHA256SUMS
done
cat > README.txt <<EOF2
Qt ${ver} source code of the Qt libraries and plugins in the Onesimus
Windows and macOS packages, as published on ${base}/
(checked against the .sha256 files and md5sums.txt published there).
Modules: $*
EOF2
cd "$work"
tar --sort=name --owner=0 --group=0 --numeric-owner -cf "$out_dir/$(basename "$out")" "$top"
echo "qt-sources: $out_dir/$(basename "$out") ($(du -h "$out_dir/$(basename "$out")" | cut -f1))"
