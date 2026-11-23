#!/usr/bin/env bash
# ─── Onesimus.app → Onesimus-<version>-macos-<arch>.dmg ─────────────────────
#
# Used by ci.yml (unsigned), release.yml (signed and notarised when the
# secrets are set) and ./deploy.sh package / cmake --build . --target dmg.
#
#   scripts/ci/macos-package.sh [BUILD_DIR]      (default: build)
#
# 1. Copies <BUILD_DIR>/Onesimus.app into a staging folder and runs
#    macdeployqt on the copy (Qt frameworks incl. QtGrpc/QtProtobuf, the
#    cocoa platform plugin, styles, image formats, TLS backends). Of the SQL
#    drivers only SQLite stays (the only one Onesimus uses; the others link
#    client libraries that are not on the machine).
# 2. Checks every Mach-O file of the bundle with otool: no load command and
#    no rpath may point into the Qt installation, Homebrew (/opt/homebrew,
#    /usr/local) or anywhere else outside the bundle and the system, every
#    @rpath reference must exist in Contents/Frameworks. Then starts the app
#    once with --version.
# 3. Signs the bundle inside out (dylibs and plugins, frameworks, then the
#    app) with the hardened runtime and a secure timestamp, no --deep, no
#    entitlements, and verifies it (codesign --verify --strict, spctl).
#    Without a certificate everything is signed ad hoc (Apple Silicon does
#    not start unsigned code), and the dmg is not notarised.
# 4. Builds the dmg (Onesimus.app + a link to /Applications), signs it,
#    notarises it with notarytool (--wait) and staples the ticket, then
#    writes <dmg>.sha256.
#
# Signing, from the environment (GitHub secrets in release.yml):
#   MACOS_CERTIFICATE_P12_BASE64  Developer ID Application certificate with
#                                 its private key, .p12, base64 -w0
#   MACOS_CERTIFICATE_PASSWORD    password of the .p12
#   MACOS_SIGN_IDENTITY           optional: name or SHA-1 of the identity;
#                                 default: the first "Developer ID
#                                 Application" identity in the keychain
# Notarisation (App Store Connect API key for notarytool):
#   APPSTORE_API_KEY_ID, APPSTORE_API_ISSUER_ID,
#   APPSTORE_API_KEY_P8_BASE64    the AuthKey_<id>.p8 file, base64 -w0
#
#   no secrets             → ad-hoc signed, not notarised (warning)
#   certificate only       → signed, not notarised (warning)
#   certificate + API key  → signed, notarised, stapled
#
# The certificate goes into a temporary keychain that is deleted on exit,
# also after a failure. No secret is ever printed.
#
# Other environment: MACDEPLOYQT (path to macdeployqt; default: from
# QT_ROOT_DIR, the CMake cache or PATH), OUT_DIR (default: BUILD_DIR).
set -euo pipefail
set +x   # never trace: the secrets are in the environment

die()    { echo "::error title=macos-package::$*" >&2; exit 1; }
warn()   { echo "::warning title=macos-package::$*" >&2; }
info()   { echo "[macos-package] $*"; }
group()  { echo "::group::$*"; }
endgroup() { echo "::endgroup::"; }

[ "$(uname -s)" = "Darwin" ] || die "macOS only"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
BUILD_DIR="${1:-${BUILD_DIR:-build}}"
case "$BUILD_DIR" in /*) ;; *) BUILD_DIR="$SRC_DIR/$BUILD_DIR" ;; esac
OUT_DIR="${OUT_DIR:-$BUILD_DIR}"
case "$OUT_DIR" in /*) ;; *) OUT_DIR="$SRC_DIR/$OUT_DIR" ;; esac
APP_SRC="$BUILD_DIR/Onesimus.app"
[ -x "$APP_SRC/Contents/MacOS/Onesimus" ] || die "no bundle: $APP_SRC (build first)"

# ─── Tools ──────────────────────────────────────────────────────────────────
if [ -z "${MACDEPLOYQT:-}" ]; then
    if [ -n "${QT_ROOT_DIR:-}" ] && [ -x "$QT_ROOT_DIR/bin/macdeployqt" ]; then
        MACDEPLOYQT="$QT_ROOT_DIR/bin/macdeployqt"
    elif [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
        MACDEPLOYQT="$(sed -n 's/^MACDEPLOYQT_EXECUTABLE:[A-Z]*=//p' "$BUILD_DIR/CMakeCache.txt" | head -1)"
    fi
    if [ -z "${MACDEPLOYQT:-}" ] || [ ! -x "$MACDEPLOYQT" ]; then
        MACDEPLOYQT="$(command -v macdeployqt || true)"
    fi
fi
[ -n "$MACDEPLOYQT" ] && [ -x "$MACDEPLOYQT" ] || die "macdeployqt not found (set MACDEPLOYQT or QT_ROOT_DIR)"
QT_PREFIX="$(cd "$(dirname "$MACDEPLOYQT")/.." && pwd -P)"
info "macdeployqt: $MACDEPLOYQT (Qt prefix $QT_PREFIX)"

# ─── Secrets: which mode ────────────────────────────────────────────────────
SIGN=0; NOTARIZE=0
if [ -n "${MACOS_CERTIFICATE_P12_BASE64:-}" ]; then
    [ -n "${MACOS_CERTIFICATE_PASSWORD:-}" ] || die "MACOS_CERTIFICATE_P12_BASE64 is set but MACOS_CERTIFICATE_PASSWORD is not"
    SIGN=1
fi
api_set=0
for v in APPSTORE_API_KEY_ID APPSTORE_API_ISSUER_ID APPSTORE_API_KEY_P8_BASE64; do
    [ -n "${!v:-}" ] && api_set=$((api_set + 1))
done
if [ "$api_set" -eq 3 ]; then
    if [ "$SIGN" = "1" ]; then NOTARIZE=1
    else warn "App Store Connect API key set but no certificate: notarisation needs a Developer ID signature - skipped"; fi
elif [ "$api_set" -ne 0 ]; then
    die "APPSTORE_API_KEY_ID, APPSTORE_API_ISSUER_ID and APPSTORE_API_KEY_P8_BASE64 must be set together"
fi
if [ "$SIGN" = "0" ]; then
    warn "No signing certificate (MACOS_CERTIFICATE_P12_BASE64): the dmg is signed ad hoc only and not notarised. Gatekeeper will refuse it on other Macs."
elif [ "$NOTARIZE" = "0" ]; then
    warn "No App Store Connect API key (APPSTORE_API_*): the dmg is signed but not notarised. Gatekeeper will refuse it on other Macs."
fi

# ─── Work folder and cleanup (also after a failure) ─────────────────────────
WORK="$(mktemp -d "${RUNNER_TEMP:-${TMPDIR:-/tmp}}/onesimus-dmg.XXXXXX")"
KEYCHAIN=""
OLD_KEYCHAINS=()
cleanup() {
    local rc=$?
    if [ -n "$KEYCHAIN" ]; then
        if [ ${#OLD_KEYCHAINS[@]} -gt 0 ]; then
            security list-keychains -d user -s "${OLD_KEYCHAINS[@]}" >/dev/null 2>&1 || true
        fi
        security delete-keychain "$KEYCHAIN" >/dev/null 2>&1 || true
        info "temporary keychain deleted"
    fi
    rm -rf "$WORK"
    exit $rc
}
trap cleanup EXIT
trap 'exit 130' INT TERM
umask 077

STAGE="$WORK/stage"        # becomes the dmg's content
APP="$STAGE/Onesimus.app"
mkdir -p "$STAGE"

# ─── 1. Copy + macdeployqt ──────────────────────────────────────────────────
group "macdeployqt"
ditto "$APP_SRC" "$APP"
"$MACDEPLOYQT" "$APP" -verbose=1 -always-overwrite
# Only the SQLite driver: the others link libpq/libmysqlclient/ODBC, which
# are neither shipped nor used.
if [ -d "$APP/Contents/PlugIns/sqldrivers" ]; then
    find "$APP/Contents/PlugIns/sqldrivers" -type f ! -name 'libqsqlite.dylib' -print -delete
fi
endgroup

VERSION="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleShortVersionString' "$APP/Contents/Info.plist")"
[ -n "$VERSION" ] || die "no CFBundleShortVersionString in Info.plist"
ARCHS="$(lipo -archs "$APP/Contents/MacOS/Onesimus")"
case "$ARCHS" in
    arm64) ARCH=arm64 ;;
    x86_64) ARCH=x86_64 ;;
    *arm64*x86_64*|*x86_64*arm64*) ARCH=universal ;;
    *) die "unexpected architectures: $ARCHS" ;;
esac
DMG_NAME="Onesimus-${VERSION}-macos-${ARCH}.dmg"
info "version $VERSION, architectures: $ARCHS → $DMG_NAME"
info "QtCore of the Qt installation: $(lipo -archs "$QT_PREFIX/lib/QtCore.framework/QtCore" 2>/dev/null || echo '?')"

# ─── 2. otool check ─────────────────────────────────────────────────────────
macho_files() {   # every Mach-O file of the bundle, one per line
    find "$APP" -type f -print0 | while IFS= read -r -d '' f; do
        if file -b "$f" | grep -q 'Mach-O'; then printf '%s\n' "$f"; fi
    done
}

# Absolute rpaths left by the build (Qt's lib dir) are removed first.
group "rpaths"
while IFS= read -r f; do
    otool -l "$f" | awk '/cmd LC_RPATH/{r=1} r&&/ path /{print $2; r=0}' | while IFS= read -r rp; do
        case "$rp" in
            @*) ;;
            *) info "  delete rpath $rp from ${f#$STAGE/}"
               install_name_tool -delete_rpath "$rp" "$f" ;;
        esac
    done
done < <(macho_files)
endgroup

group "otool check"
bad=0
while IFS= read -r f; do
    rel="${f#$STAGE/}"
    # Load commands. otool -L prints the file name (per architecture of a
    # fat file) and, for a dylib or framework, its own install name first.
    own_id="$(otool -D "$f" | awk 'NR>1 && $0 !~ /:$/ {print $1; exit}')"
    while IFS= read -r dep; do
        [ -n "$own_id" ] && [ "$dep" = "$own_id" ] && continue
        case "$dep" in
            /System/*|/usr/lib/*) ;;
            @executable_path/*|@loader_path/*) ;;
            @rpath/*)
                name="${dep#@rpath/}"
                [ -e "$APP/Contents/Frameworks/$name" ] || { echo "  $rel: $dep not in Contents/Frameworks"; bad=1; } ;;
            *) echo "  $rel: links $dep"; bad=1 ;;
        esac
        case "$dep" in
            "$QT_PREFIX"/*|/opt/homebrew/*|/usr/local/*) echo "  $rel: points to $dep"; bad=1 ;;
        esac
    done < <(otool -L "$f" | awk '$0 !~ /:$/ {print $1}' | sort -u)
    while IFS= read -r rp; do
        case "$rp" in
            @*) ;;
            *) echo "  $rel: rpath $rp"; bad=1 ;;
        esac
    done < <(otool -l "$f" | awk '/cmd LC_RPATH/{r=1} r&&/ path /{print $2; r=0}')
done < <(macho_files)
endgroup
[ "$bad" = "0" ] || die "the bundle refers to files outside itself (see above)"
info "otool check passed: $(macho_files | wc -l | tr -d ' ') Mach-O files, nothing outside the bundle and the system"

group "bundle contents"
( cd "$STAGE" && find Onesimus.app -maxdepth 4 | sort )
plutil -p "$APP/Contents/Info.plist"
otool -L "$APP/Contents/MacOS/Onesimus"
endgroup

# ─── 3. Signing ─────────────────────────────────────────────────────────────
if [ "$SIGN" = "1" ]; then
    group "temporary keychain"
    KEYCHAIN="$WORK/signing.keychain-db"
    kc_pw="$(openssl rand -hex 24)"
    p12="$WORK/cert.p12"
    printf '%s' "$MACOS_CERTIFICATE_P12_BASE64" | base64 --decode > "$p12" \
        || die "MACOS_CERTIFICATE_P12_BASE64 is not valid base64"
    while IFS= read -r k; do OLD_KEYCHAINS+=("$k"); done \
        < <(security list-keychains -d user | sed -e 's/^ *"//' -e 's/" *$//')
    security create-keychain -p "$kc_pw" "$KEYCHAIN"
    security set-keychain-settings -lut 21600 "$KEYCHAIN"
    security unlock-keychain -p "$kc_pw" "$KEYCHAIN"
    security import "$p12" -k "$KEYCHAIN" -f pkcs12 -P "$MACOS_CERTIFICATE_PASSWORD" \
        -T /usr/bin/codesign >/dev/null \
        || die "importing the .p12 failed (wrong password, or not created with -keypbe PBE-SHA1-3DES -certpbe PBE-SHA1-3DES -macalg sha1?)"
    rm -f "$p12"
    security set-key-partition-list -S apple-tool:,apple:,codesign: -s -k "$kc_pw" "$KEYCHAIN" >/dev/null
    security list-keychains -d user -s "$KEYCHAIN" ${OLD_KEYCHAINS[@]+"${OLD_KEYCHAINS[@]}"}
    unset kc_pw
    if [ -n "${MACOS_SIGN_IDENTITY:-}" ]; then
        IDENTITY="$MACOS_SIGN_IDENTITY"
    else
        IDENTITY="$(security find-identity -v -p codesigning "$KEYCHAIN" \
            | awk '/"Developer ID Application/{print $2; exit}')"
        [ -n "$IDENTITY" ] || die "no \"Developer ID Application\" identity in the certificate"
    fi
    info "signing identity: $(security find-identity -v -p codesigning "$KEYCHAIN" | grep -F "$IDENTITY" | head -1 | sed 's/^ *[0-9]*) //')"
    endgroup
    SIGN_ARGS=(--force --options runtime --timestamp --sign "$IDENTITY" --keychain "$KEYCHAIN")
else
    SIGN_ARGS=(--force --sign -)
fi

group "codesign (inside out)"
# a) loose dylibs: plugins and libraries in Frameworks
while IFS= read -r -d '' f; do
    codesign "${SIGN_ARGS[@]}" "$f"
done < <(find "$APP/Contents/PlugIns" "$APP/Contents/Frameworks" -type f -name '*.dylib' -print0 2>/dev/null)
# b) frameworks (signs Versions/Current with its Info.plist)
while IFS= read -r -d '' fw; do
    codesign "${SIGN_ARGS[@]}" "$fw"
done < <(find "$APP/Contents/Frameworks" -maxdepth 1 -type d -name '*.framework' -print0 2>/dev/null)
# c) any other Mach-O executable inside (none expected), then the app
while IFS= read -r f; do
    case "$f" in
        "$APP/Contents/MacOS/Onesimus"|*.dylib|*.framework/*) ;;
        *) info "  extra Mach-O: ${f#$STAGE/}"; codesign "${SIGN_ARGS[@]}" "$f" ;;
    esac
done < <(macho_files)
codesign "${SIGN_ARGS[@]}" "$APP"
endgroup

group "verify signature"
codesign --verify --strict --deep --verbose=2 "$APP"
codesign -dv --verbose=2 "$APP" 2>&1 | grep -E '^(Identifier|Format|Authority|TeamIdentifier|Timestamp|Runtime|flags|CodeDirectory)' || true
endgroup

# Smoke test of the finished bundle: it loads its frameworks and the cocoa
# plugin from inside and prints the version. A broken install name or a
# signature the hardened runtime refuses fails here.
group "start test (--version)"
env -u QT_PLUGIN_PATH -u DYLD_LIBRARY_PATH -u DYLD_FRAMEWORK_PATH \
    "$APP/Contents/MacOS/Onesimus" --version 2>&1 | tee "$WORK/version.txt" || die "Onesimus.app did not start"
grep -q "$VERSION" "$WORK/version.txt" || die "Onesimus --version does not name $VERSION"
endgroup

# ─── 4. dmg ─────────────────────────────────────────────────────────────────
group "dmg"
ln -s /Applications "$STAGE/Applications"
mkdir -p "$OUT_DIR"
DMG="$OUT_DIR/$DMG_NAME"
rm -f "$DMG" "$DMG.sha256"
ok=0
for attempt in 1 2 3; do
    # hdiutil fails now and then with "Resource busy" on the CI runners
    if hdiutil create -volname "Onesimus" -srcfolder "$STAGE" -fs HFS+ -format UDZO -ov "$DMG"; then
        ok=1; break
    fi
    warn "hdiutil create failed (attempt $attempt), retrying"
    sleep 5
done
[ "$ok" = "1" ] || die "hdiutil create failed"
if [ "$SIGN" = "1" ]; then
    codesign --force --timestamp --sign "$IDENTITY" --keychain "$KEYCHAIN" "$DMG"
    codesign --verify --strict --verbose=2 "$DMG"
fi
endgroup

if [ "$NOTARIZE" = "1" ]; then
    group "notarytool"
    p8="$WORK/AuthKey_${APPSTORE_API_KEY_ID}.p8"
    printf '%s' "$APPSTORE_API_KEY_P8_BASE64" | base64 --decode > "$p8" \
        || die "APPSTORE_API_KEY_P8_BASE64 is not valid base64"
    NOTARY_AUTH=(--key "$p8" --key-id "$APPSTORE_API_KEY_ID" --issuer "$APPSTORE_API_ISSUER_ID")
    set +e
    xcrun notarytool submit "$DMG" "${NOTARY_AUTH[@]}" --wait --timeout 45m \
        --output-format json > "$WORK/notary.json"
    rc=$?
    set -e
    cat "$WORK/notary.json"; echo
    sub_id="$(plutil -extract id raw -o - "$WORK/notary.json" 2>/dev/null || true)"
    status="$(plutil -extract status raw -o - "$WORK/notary.json" 2>/dev/null || true)"
    if [ "$rc" -ne 0 ] || [ "$status" != "Accepted" ]; then
        [ -n "$sub_id" ] && xcrun notarytool log "$sub_id" "${NOTARY_AUTH[@]}" || true
        die "notarisation failed (status: ${status:-unknown})"
    fi
    rm -f "$p8"
    xcrun stapler staple "$DMG"
    xcrun stapler validate "$DMG"
    endgroup
fi

group "Gatekeeper (spctl)"
if [ "$NOTARIZE" = "1" ]; then
    spctl -a -vv -t open --context context:primary-signature "$DMG"
    spctl -a -vv -t exec "$APP"
else
    # Without notarisation Gatekeeper rejects it; shown, not fatal.
    spctl -a -vv -t exec "$APP" || info "spctl rejects the app (expected without notarisation)"
fi
endgroup

( cd "$OUT_DIR" && shasum -a 256 "$DMG_NAME" > "$DMG_NAME.sha256" )
info "dmg:    $DMG ($(du -h "$DMG" | cut -f1))"
info "sha256: $(cut -d' ' -f1 "$DMG.sha256")"
if [ "$NOTARIZE" = "1" ]; then info "signed (Developer ID), notarised and stapled"
elif [ "$SIGN" = "1" ]; then info "signed (Developer ID), NOT notarised"
else info "ad-hoc signed only, NOT notarised"; fi
