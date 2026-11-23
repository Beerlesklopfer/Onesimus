# macOS Build Guide

Onesimus baut auf macOS als App-Bundle `Onesimus.app` und wird als
`Onesimus-<version>-macos-arm64.dmg` ausgeliefert. Geprüft wird das in der CI
auf `macos-15` (Apple Silicon) mit Qt 6.8.3 aus
[install-qt-action](https://github.com/jurplel/install-qt-action) (`.github/workflows/ci.yml`,
Job `macos`); einen anderen Mac-Aufbau prüft niemand regelmäßig.

## Schnellstart

```bash
./deploy.sh build      # bauen: build/Onesimus.app
./deploy.sh run        # bauen und starten
./deploy.sh package    # .dmg über scripts/ci/macos-package.sh (siehe unten)
./deploy.sh            # bauen + Binary nach /usr/local/bin/onesimus (Entwickler-Installation)
./deploy.sh -n         # Probelauf: zeigt die Befehle, ohne sie auszuführen
```

Das Script initialisiert die nötigen Submodule (`external/openssl` bei statischem
OpenSSL, `proto/`), setzt `CMAKE_PREFIX_PATH` auf `brew --prefix qt@6`, wenn er
nicht gesetzt ist, konfiguriert CMake (Ninja, falls installiert) und kompiliert.
Alle Befehle und Flags: `./deploy.sh help` und [BUILD_LINUX.md](BUILD_LINUX.md#-build-script-options).
`./deploy.fish` ist dasselbe Script für die Fish-Shell.

Qt aus dem Qt-Installer oder aus install-qt-action:

```bash
CMAKE_PREFIX_PATH=~/Qt/6.8.3/macos ./deploy.sh build
```

## Voraussetzungen

- Xcode oder die Command Line Tools (`xcode-select --install`)
- CMake ≥ 3.25, Perl (für den OpenSSL-Build), optional Ninja
- Qt ≥ 6.8 mit dem Modul **Qt GRPC** (QtGrpc/QtProtobuf) und `protoc`
  (in der CI: `brew install ninja protobuf`)

## Das App-Bundle

Auf macOS ist das GUI-Target ein `MACOSX_BUNDLE` (CMakeLists.txt):

```
build/Onesimus.app/Contents/
├── Info.plist          aus packaging/macos/Info.plist.in
├── MacOS/Onesimus
└── Resources/onesimus.icns
```

- Bundle-ID `io.onesimus.Onesimus`, Version aus `project(onesimus VERSION …)`
- `LSMinimumSystemVersion` 12.0: `CMAKE_OSX_DEPLOYMENT_TARGET` ist 12.0, wenn
  weder `-DCMAKE_OSX_DEPLOYMENT_TARGET` noch `MACOSX_DEPLOYMENT_TARGET` gesetzt
  ist (Qt 6.8 unterstützt macOS 12 und neuer). Das statische OpenSSL wird mit
  demselben Ziel gebaut.
- `NSHighResolutionCapable`, `NSLocalNetworkUsageDescription` (Verbindung zu
  Directors im lokalen Netz, mDNS)
- `resources/icons/onesimus.icns` ist aus `resources/icons/onesimus.svg`
  erzeugt (16–1024 px) und eingecheckt.

Im Build-Baum enthält das Bundle noch keine Qt-Frameworks; es findet Qt über
den rpath der Qt-Installation. Ein eigenständiges Bundle entsteht erst beim
Paketieren.

## DMG: scripts/ci/macos-package.sh

Ein Script für CI, Release und lokal (`./deploy.sh package` oder
`cmake --build build --target dmg`):

```bash
scripts/ci/macos-package.sh build        # BUILD_DIR, Standard: build
```

1. kopiert `build/Onesimus.app` in einen Staging-Ordner und ruft dort
   `macdeployqt` auf (Qt-Frameworks inkl. QtGrpc/QtProtobuf, Plattform-Plugin
   cocoa, Styles, Bildformate, TLS-Backends). Von den SQL-Treibern bleibt nur
   SQLite.
2. prüft jede Mach-O-Datei mit `otool`: kein Verweis und kein rpath in die
   Qt-Installation, nach Homebrew (`/opt/homebrew`, `/usr/local`) oder sonst
   außerhalb von Bundle und System; jedes `@rpath/…` muss in
   `Contents/Frameworks` liegen. Eine `libssl`/`libcrypto`-dylib im Bundle ist
   ein Fehler, weil OpenSSL statisch gelinkt sein muss.
3. signiert von innen nach außen (dylibs/Plugins, Frameworks, dann die App,
   ohne `--deep`), prüft mit `codesign --verify --strict` und startet
   `Onesimus.app/Contents/MacOS/Onesimus --version`.
4. baut die dmg (Onesimus.app und ein Link auf /Applications), signiert,
   notarisiert und heftet das Ticket an (wenn die Secrets da sind) und
   schreibt `<dmg>.sha256`.

macdeployqt wird aus `MACDEPLOYQT`, `QT_ROOT_DIR/bin`, dem CMake-Cache oder
`PATH` genommen; `OUT_DIR` legt den Zielordner fest (Standard: `BUILD_DIR`).

### Signieren und Notarisieren

Das Script liest die Zugangsdaten aus der Umgebung (in GitHub: Repository-Secrets):

| Variable | Inhalt |
|---|---|
| `MACOS_CERTIFICATE_P12_BASE64` | Zertifikat „Developer ID Application“ mit privatem Schlüssel als .p12, `base64 -w0` |
| `MACOS_CERTIFICATE_PASSWORD` | Passwort der .p12 |
| `MACOS_SIGN_IDENTITY` | optional; sonst die erste „Developer ID Application“-Identität |
| `APPSTORE_API_KEY_ID` | Key-ID des App-Store-Connect-API-Schlüssels für `notarytool` |
| `APPSTORE_API_ISSUER_ID` | Issuer-ID dieses Schlüssels |
| `APPSTORE_API_KEY_P8_BASE64` | die Datei `AuthKey_<id>.p8`, `base64 -w0` |

- keine Secrets: nur ad hoc signiert, nicht notarisiert (Warnung). Gatekeeper
  lehnt so eine dmg auf anderen Macs ab.
- nur das Zertifikat: signiert, nicht notarisiert (Warnung)
- Zertifikat und API-Schlüssel: signiert, notarisiert, Ticket angeheftet

Signiert wird mit hardened runtime (`--options runtime`) und `--timestamp`,
ohne Entitlements. Das Zertifikat kommt in einen temporären Keychain, der am
Ende gelöscht wird, auch nach einem Fehler.

Die .p12 mit Legacy-Verschlüsselung erzeugen, sonst importiert der
macOS-Keychain sie nicht:

```bash
openssl pkcs12 -export -inkey devid.key -in devid.pem -out devid.p12 -keypbe PBE-SHA1-3DES -certpbe PBE-SHA1-3DES -macalg sha1
base64 -w0 devid.p12      # Inhalt für MACOS_CERTIFICATE_P12_BASE64
```

In der CI ist die dmg eines Push-Laufs unsigniert (Artefakt `macos-dmg`). Ein
von Hand gestarteter Lauf mit `macos_sign=true` signiert und notarisiert mit
denselben Secrets wie das Release:

```bash
gh workflow run ci.yml --ref <branch> -f macos_sign=true
```

`release.yml` (Job `macos`) baut die dmg für ein Versions-Tag und hängt sie
mit `.sha256` an den Release-Entwurf.

## Architektur

Gebaut wird arm64 (der `macos-15`-Runner). Qt 6.8.3 aus install-qt-action
ist universal (x86_64 und arm64); eine universelle dmg bräuchte zusätzlich
`-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"` und ein statisches OpenSSL für
beide Architekturen (zwei OpenSSL-Builds, mit `lipo` vereint). Das ist nicht
umgesetzt.

## Fehlerbehebung

### OpenSSL-Build schlägt fehl

```bash
cat build/openssl_build-prefix/src/openssl_build-stamp/openssl_build-configure-*.log
cat build/openssl_build-prefix/src/openssl_build-stamp/openssl_build-build-*.log
```

### `macos-package.sh`: „OpenSSL dylibs in the bundle“

Der Build hat ein fremdes OpenSSL gelinkt (z. B. mit `-s`/`OPENSSL=system`
Homebrews openssl). Für die dmg mit statischem OpenSSL bauen
(`./deploy.sh -c build`).

### Abhängigkeiten ansehen

```bash
otool -L build/Onesimus.app/Contents/MacOS/Onesimus
```
