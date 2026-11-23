#!/bin/sh
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau
# Fails when a tracked source or script file has no SPDX licence header.
set -eu
missing=$(git ls-files \
    | grep -E '\.(cpp|h|hpp|c|mm|sh|fish|py|cmake|ps1|bat)$|CMakeLists\.txt$|\.h\.in$' \
    | grep -v -E '^(external|proto|wiki|docs)/' \
    | while read -r f; do head -c 600 "$f" | grep -q 'SPDX-License-Identifier' || echo "$f"; done)
if [ -n "$missing" ]; then
    echo "::error title=SPDX::files without an SPDX-License-Identifier header:"
    echo "$missing"
    exit 1
fi
echo "SPDX headers: ok"
