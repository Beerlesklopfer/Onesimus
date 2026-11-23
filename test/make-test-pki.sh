#!/bin/sh
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau
#
# Generates the TEST-ONLY PKI for the live-Director test scripts:
#   ca.key/ca.pem          "Onesimus Test CA" (self-signed)
#   server.key/server.pem  CN=bareos-dir, DNS:localhost, IP:127.0.0.1
#   client.key/client.pem  CN=test-cert (the test-cert console)
#
# The keys are throw-away test material: never use them for a real Director.
# Nothing of it is tracked in git (.gitignore); delete the directory to get a
# fresh PKI.
#
# Usage: test/make-test-pki.sh [DIR]   (default: test/bareos-dir.d/tls)
set -eu

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
DIR="${1:-$SCRIPT_DIR/bareos-dir.d/tls}"
DAYS=825

command -v openssl >/dev/null 2>&1 || { echo "make-test-pki.sh: openssl not found" >&2; exit 1; }

mkdir -p "$DIR"
umask 077
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

serial() { echo "0x$(openssl rand -hex 16)"; }

openssl req -x509 -newkey rsa:2048 -nodes -sha256 -days "$DAYS" \
    -subj "/C=DE/O=Onesimus Test/CN=Onesimus Test CA" \
    -addext "basicConstraints=critical,CA:TRUE" \
    -addext "keyUsage=critical,keyCertSign,cRLSign" \
    -keyout "$DIR/ca.key" -out "$DIR/ca.pem" 2>/dev/null

# leaf NAME CN EXTENSIONS
leaf() {
    openssl req -newkey rsa:2048 -nodes -sha256 \
        -subj "/C=DE/O=Onesimus Test/CN=$2" \
        -keyout "$DIR/$1.key" -out "$WORK/$1.csr" 2>/dev/null
    printf '%s\n' "$3" > "$WORK/$1.ext"
    openssl x509 -req -sha256 -days "$DAYS" -in "$WORK/$1.csr" \
        -CA "$DIR/ca.pem" -CAkey "$DIR/ca.key" -set_serial "$(serial)" \
        -extfile "$WORK/$1.ext" -out "$DIR/$1.pem" 2>/dev/null
}

leaf server bareos-dir "basicConstraints=CA:FALSE
keyUsage=critical,digitalSignature,keyEncipherment
extendedKeyUsage=serverAuth
subjectAltName=DNS:bareos-dir,DNS:localhost,IP:127.0.0.1"

leaf client test-cert "basicConstraints=CA:FALSE
keyUsage=critical,digitalSignature,keyEncipherment
extendedKeyUsage=clientAuth
subjectAltName=DNS:test-cert"

chmod 644 "$DIR"/*.pem
echo "Test PKI written to $DIR (test-only keys, not for real use)"
