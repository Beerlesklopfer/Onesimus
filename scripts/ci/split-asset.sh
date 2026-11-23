#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── Release assets under GitHub's size limit ───────────────────────────────
#
#   scripts/ci/split-asset.sh FILE...
#
# A GitHub release asset may not exceed 2 GiB. A FILE larger than
# ASSET_SPLIT_BYTES (default 2000 MiB) is replaced by FILE.part00,
# FILE.part01, ... of that size plus FILE.sha256 (SHA-256 of the whole
# file). Reassemble and check (also in THIRD-PARTY-NOTICES.md):
#   cat FILE.part* > FILE && sha256sum -c FILE.sha256
# Smaller files stay as they are.
set -euo pipefail

limit="${ASSET_SPLIT_BYTES:-2097152000}"
[ $# -gt 0 ] || { echo "usage: $0 FILE..." >&2; exit 1; }
for f in "$@"; do
    [ -f "$f" ] || { echo "split-asset: no file $f" >&2; exit 1; }
    size="$(wc -c < "$f" | tr -d ' ')"
    if [ "$size" -le "$limit" ]; then
        echo "split-asset: $f ($size bytes) stays whole"
        continue
    fi
    ( cd "$(dirname "$f")" && sha256sum "$(basename "$f")" > "$(basename "$f").sha256" )
    split -b "$limit" -d -a 2 "$f" "$f.part"
    rm -f "$f"
    echo "split-asset: $f ($size bytes) -> $(ls "$f".part* | wc -l | tr -d ' ') parts; reassemble: cat $(basename "$f").part* > $(basename "$f") && sha256sum -c $(basename "$f").sha256"
done
