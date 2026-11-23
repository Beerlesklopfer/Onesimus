#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── The Qt modules a Windows/macOS package ships ───────────────────────────
#
#   scripts/ci/check-qt-modules.sh DIR MODULE...
#
# DIR is the Windows deploy folder (build\deploy) or Onesimus.app. Every Qt
# library (Qt6*.dll, Qt*.framework) and plugin found there is mapped to the
# Qt source module it comes from; the script fails when one belongs to a
# module that is not in MODULE... (release.yml: env QT_SOURCE_MODULES, the
# modules whose source code qt-sources.sh attaches to the release) or is
# not known here.
set -euo pipefail

dir="${1:?usage: $0 DIR MODULE...}"
shift
[ -d "$dir" ] || { echo "check-qt-modules: no folder $dir" >&2; exit 1; }
modules=" $* "

lib_module() {   # Qt library name without Qt6/Qt prefix -> module
    case "$1" in
        Core|Gui|Widgets|Network|Sql|DBus|OpenGL|OpenGLWidgets|PrintSupport|Concurrent|Xml|Test) echo qtbase ;;
        Svg|SvgWidgets) echo qtsvg ;;
        Grpc|Protobuf|ProtobufWellKnownTypes|ProtobufQtCoreTypes|ProtobufQtGuiTypes) echo qtgrpc ;;
        *) echo "unknown" ;;
    esac
}
plugin_module() {   # <plugin dir> <file name without lib prefix/suffix>
    case "$2" in
        qsvg|qsvgicon) echo qtsvg; return ;;
        qicns|qtga|qtiff|qwbmp|qwebp|qmng|qjp2) echo qtimageformats; return ;;
    esac
    case "$1" in
        platforms|styles|tls|sqldrivers|networkinformation|generic|platforminputcontexts|platformthemes|imageformats) echo qtbase ;;
        *) echo "unknown" ;;
    esac
}

bad=0
seen=""
check() {   # <what> <module>
    seen="$seen $2"
    if [ "$2" = "unknown" ]; then
        echo "  $1: no known Qt source module" ; bad=1
    elif [[ "$modules" != *" $2 "* ]]; then
        echo "  $1: module $2 is not in the source list (${modules# })"; bad=1
    fi
}

while IFS= read -r -d '' f; do
    name="$(basename "$f")"
    case "$name" in
        Qt6*.dll) lib="${name#Qt6}"; lib="${lib%.dll}"; check "$name" "$(lib_module "$lib")" ;;
        Qt*.framework) lib="${name#Qt}"; lib="${lib%.framework}"; check "$name" "$(lib_module "$lib")" ;;
    esac
done < <(find "$dir" -maxdepth 3 \( -name 'Qt6*.dll' -o -name 'Qt*.framework' \) -print0)

# Plugins: <dir>/<plugin dir>/<plugin> (Windows) or Contents/PlugIns/...
while IFS= read -r -d '' f; do
    pdir="$(basename "$(dirname "$f")")"
    base="$(basename "$f")"; base="${base#lib}"; base="${base%.dll}"; base="${base%.dylib}"
    check "$pdir/$(basename "$f")" "$(plugin_module "$pdir" "$base")"
done < <(find "$dir" -mindepth 2 \( -name 'q*.dll' -o -name 'libq*.dylib' \) -print0)

[ "$bad" = "0" ] || { echo "::error title=Qt sources::the package ships Qt code whose source is not attached (see above)" >&2; exit 1; }
echo "check-qt-modules: ok, modules used:$(tr ' ' '\n' <<<"$seen" | sort -u | tr '\n' ' ')"
