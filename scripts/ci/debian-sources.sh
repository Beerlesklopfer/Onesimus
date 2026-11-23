#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── Debian source packages of the libraries bundled in the AppImage ────────
#
#   scripts/ci/debian-sources.sh BUNDLED_PACKAGES_TXT OUT.tar
#
# Reads usr/share/doc/onesimus/bundled-packages.txt of the AppDir (written by
# appimage-notices.sh: binary package, version, source package, source
# version) and downloads the source package of exactly that version of each
# library (.dsc, orig and debian tarballs) into one uncompressed tar - the
# files are compressed already. The tar holds a folder named after OUT.tar
# with one subfolder per source package, the bundled-packages.txt it was made
# from and a README. Attached to every release (release.yml).
#
# Sources come from apt with a private configuration (deb-src of the
# running Debian release, -updates and -security; no root, the system's apt
# setup is not touched). A version apt no longer offers is taken from
# snapshot.debian.org, each file checked against its snapshot SHA-1. A
# package that cannot be found fails the script.
#
# Needs apt-get, curl, jq, sha1sum, tar and debian-archive-keyring.
set -euo pipefail

list="${1:?usage: $0 BUNDLED_PACKAGES_TXT OUT.tar}"
out="${2:?usage: $0 BUNDLED_PACKAGES_TXT OUT.tar}"
[ -s "$list" ] || { echo "debian-sources: $list missing or empty" >&2; exit 1; }
for tool in apt-get curl jq sha1sum tar; do
    command -v "$tool" >/dev/null || { echo "debian-sources: missing tool $tool" >&2; exit 1; }
done
KEYRING="${DEBIAN_KEYRING:-/usr/share/keyrings/debian-archive-keyring.gpg}"
[ -f "$KEYRING" ] || { echo "debian-sources: no $KEYRING (package debian-archive-keyring)" >&2; exit 1; }
# shellcheck disable=SC1091
. /etc/os-release
suite="${DEBIAN_SUITE:-${VERSION_CODENAME:?no VERSION_CODENAME in /etc/os-release}}"

out_dir="$(cd "$(dirname "$out")" && pwd)"
top="$(basename "$out" .tar)"
work="$(mktemp -d "${RUNNER_TEMP:-${TMPDIR:-/tmp}}/debian-sources.XXXXXX")"
trap 'rm -rf "$work"' EXIT
mkdir -p "$work/apt/lists/partial" "$work/apt/cache/archives/partial" "$work/apt/parts" \
         "$work/apt/prefs" "$work/$top"

cat > "$work/apt/sources.list" <<EOF
deb-src [signed-by=$KEYRING] http://deb.debian.org/debian $suite main
deb-src [signed-by=$KEYRING] http://deb.debian.org/debian $suite-updates main
deb-src [signed-by=$KEYRING] http://security.debian.org/debian-security $suite-security main
EOF
APT=(-o "Dir::Etc::SourceList=$work/apt/sources.list" -o "Dir::Etc::SourceParts=$work/apt/parts"
     -o "Dir::Etc::PreferencesParts=$work/apt/prefs"
     -o "Dir::State::Lists=$work/apt/lists" -o "Dir::Cache=$work/apt/cache"
     -o "Debug::NoLocking=1" -o "APT::Sandbox::User=$(id -un)" -q)
echo "debian-sources: apt-get update (deb-src $suite, $suite-updates, $suite-security)"
apt-get "${APT[@]}" update >/dev/null

# snapshot.debian.org: every file of source package $1 version $2 into $3
from_snapshot() {
    local src="$1" ver="$2" dest="$3" json hash name n=0
    json="$(curl -fsSL --retry 3 "https://snapshot.debian.org/mr/package/${src}/${ver}/srcfiles?fileinfo=1")" || return 1
    while read -r hash; do
        name="$(jq -r --arg h "$hash" '.fileinfo[$h][0].name' <<<"$json")"
        [ -n "$name" ] && [ "$name" != "null" ] || return 1
        curl -fsSL --retry 3 -o "$dest/$name" "https://snapshot.debian.org/file/${hash}" || return 1
        echo "$hash  $dest/$name" | sha1sum -c --status || { echo "debian-sources: SHA-1 mismatch: $name" >&2; return 1; }
        n=$((n + 1))
    done < <(jq -r '.result[].hash' <<<"$json")
    [ "$n" -gt 0 ]
}

failed=0
count=0
# "binary:arch version source source-version" -> unique "source version"
while read -r src ver; do
    dest="$work/$top/${src}_${ver}"
    mkdir -p "$dest"
    if ( cd "$dest" && apt-get "${APT[@]}" source --download-only "${src}=${ver}" >/dev/null 2>&1 ); then
        how=apt
    elif from_snapshot "$src" "$ver" "$dest"; then
        how=snapshot.debian.org
    else
        echo "debian-sources: source ${src} ${ver} not found (apt, snapshot.debian.org)" >&2
        failed=1
        continue
    fi
    ls "$dest"/*.dsc >/dev/null 2>&1 || { echo "debian-sources: no .dsc for ${src} ${ver}" >&2; failed=1; continue; }
    count=$((count + 1))
    echo "debian-sources: ${src} ${ver} ($how, $(du -sh "$dest" | cut -f1))"
done < <(grep -v '^#' "$list" | awk 'NF >= 4 {print $3, $4}' | sort -u)
if grep -v '^#' "$list" | awk 'NF > 0 && NF < 4' | grep -q .; then
    echo "debian-sources: lines without a source package in $list:" >&2
    grep -v '^#' "$list" | awk 'NF > 0 && NF < 4' >&2
    failed=1
fi
[ "$failed" = "0" ] || { echo "debian-sources: incomplete - not writing $out" >&2; exit 1; }

cp "$list" "$work/$top/bundled-packages.txt"
cat > "$work/$top/README.txt" <<EOF
Debian source packages of the libraries bundled in the Onesimus AppImage
(Debian $suite), one folder per source package and version, as listed in
bundled-packages.txt (binary package, version, source package, source
version). Unpack a package with: dpkg-source -x <package>.dsc
EOF
tar -C "$work" --sort=name --owner=0 --group=0 --numeric-owner -cf "$out_dir/$(basename "$out")" "$top"
echo "debian-sources: $count source packages -> $out_dir/$(basename "$out") ($(du -h "$out_dir/$(basename "$out")" | cut -f1))"
