#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── Source archive of Onesimus with its submodules ─────────────────────────
#
#   scripts/ci/source-archive.sh VERSION OUT_DIR [REV]
#
# Writes OUT_DIR/onesimus-VERSION-src.tar.xz: the tree of REV (default HEAD)
# with the submodules the build uses, external/openssl and proto, at the
# commits REV records (GitHub's automatic archives leave submodules out).
# The submodules must be checked out at those commits (git submodule update
# --init external/openssl proto); wiki/ is not part of the build and stays
# out. Attached to every release (release.yml), named in
# THIRD-PARTY-NOTICES.md as the corresponding source of Onesimus and OpenSSL.
set -euo pipefail

version="${1:?usage: $0 VERSION OUT_DIR [REV]}"
out_dir="${2:?usage: $0 VERSION OUT_DIR [REV]}"
rev="${3:-HEAD}"
SUBMODULES=(external/openssl proto)

src_dir="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$src_dir"
mkdir -p "$out_dir"
out_dir="$(cd "$out_dir" && pwd)"
name="onesimus-${version}-src"
work="$(mktemp -d "${RUNNER_TEMP:-${TMPDIR:-/tmp}}/source-archive.XXXXXX")"
trap 'rm -rf "$work"' EXIT

git archive --format=tar --prefix="${name}/" "$rev" | tar -x -C "$work"
for sub in "${SUBMODULES[@]}"; do
    want="$(git rev-parse "${rev}:${sub}")"
    have="$(git -C "$sub" rev-parse HEAD 2>/dev/null || true)"
    [ "$have" = "$want" ] || { echo "source-archive: $sub is at '${have:-not checked out}', $rev records $want" >&2; exit 1; }
    rm -rf "${work:?}/${name}/${sub}"
    mkdir -p "$work/${name}/${sub}"
    git -C "$sub" archive --format=tar HEAD | tar -x -C "$work/${name}/${sub}"
    echo "source-archive: $sub at $want"
done
# Other submodules (wiki/) leave an empty folder behind
find "$work/${name}" -type d -empty -delete
# The submodule commits, for whoever unpacks the archive without git
{
    echo "Onesimus ${version} source, git revision $(git rev-parse "$rev")"
    for sub in "${SUBMODULES[@]}"; do
        echo "${sub} $(git rev-parse "${rev}:${sub}") ($(git config -f .gitmodules "submodule.${sub}.url"))"
    done
} > "$work/${name}/SOURCE-REVISIONS.txt"

# Reproducible order and owner; timestamps from the commit
mtime="$(git log -1 --format=%cI "$rev")"
tar -C "$work" --sort=name --owner=0 --group=0 --numeric-owner --mtime="$mtime" -cf - "$name" \
    | xz -T0 -9 > "$out_dir/${name}.tar.xz"
echo "source-archive: $out_dir/${name}.tar.xz ($(du -h "$out_dir/${name}.tar.xz" | cut -f1))"
