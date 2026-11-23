#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# Generates the WixUI bitmaps of the MSI from the product logo
# (resources/icons/onesimus.svg). Run it after a change to the logo and
# commit the results; the Windows build only reads the .bmp files.
#
#   packaging/windows/branding/banner.bmp   493x58   top banner of the inner dialogs
#   packaging/windows/branding/dialog.bmp   493x312  welcome / finish background
#
# WixUI draws its text on the white right part of dialog.bmp (from x=164) and
# on the left part of banner.bmp, so the logo goes into the free area: the
# left band of the dialog, the right end of the banner.
#
# Needs ImageMagick 7 (magick) and an SVG renderer: rsvg-convert or inkscape.
set -euo pipefail

cd "$(dirname "$0")/../.."
svg=resources/icons/onesimus.svg
out=packaging/windows/branding
blue='#2196F3'     # the logo's shield colour
mkdir -p "$out"

command -v magick >/dev/null 2>&1 || { echo "missing tool: magick (ImageMagick 7)" >&2; exit 1; }
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

render() {   # <size> <png>
    if command -v rsvg-convert >/dev/null 2>&1; then
        rsvg-convert -w "$1" -h "$1" -o "$2" "$svg"
    elif command -v inkscape >/dev/null 2>&1; then
        inkscape "$svg" --export-type=png --export-filename="$2" -w "$1" -h "$1" >/dev/null 2>&1
    else
        echo "missing tool: rsvg-convert or inkscape" >&2; exit 1
    fi
}

render 128 "$tmp/logo128.png"
render 48  "$tmp/logo48.png"

# Output: BMP3 + TrueColor, 24-bit bitmaps without alpha, the format the
# Windows Installer dialogs display reliably. -strip keeps them reproducible.

# Dialog: white background, a light band of the shield colour on the left
# 164 px, the logo centred in that band, a thin shield-coloured edge line.
magick -size 493x312 xc:white \
    \( -size 164x312 xc:"$blue" -alpha set -channel A -evaluate set 12% +channel \) -geometry +0+0 -composite \
    -fill "$blue" -draw "rectangle 163,0 164,311" \
    "$tmp/logo128.png" -geometry +18+92 -composite \
    -alpha off -type TrueColor -strip "BMP3:$out/dialog.bmp"

# Banner: white, the logo at the right end, a shield-coloured line at the bottom.
magick -size 493x58 xc:white \
    "$tmp/logo48.png" -geometry +437+5 -composite \
    -fill "$blue" -draw "rectangle 0,56 492,57" \
    -alpha off -type TrueColor -strip "BMP3:$out/banner.bmp"

magick identify "$out/dialog.bmp" "$out/banner.bmp"
