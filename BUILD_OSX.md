# macOS Build Guide

## 🚀 Schnellstart

### Automatischer Build (Empfohlen)

```bash
./build.sh
```

Das Script:
- Initialisiert Git-Submodule automatisch
- Prüft alle Voraussetzungen (Homebrew, Qt, CMake, etc.)
- Fragt nach Build-Optionen
- Konfiguriert CMake
- Kompiliert das Projekt

### Manueller Build

```bash
# 1. Voraussetzungen installieren
brew install cmake git qt@6

# 2. Repository klonen
git clone <your-repo> bacula-qt-ui
cd bacula-qt-ui

# 3. Build-Verzeichnis erstellen
mkdir build && cd build

# 4. CMake konfigurieren
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)

# 5. Kompilieren
make -j$(sysctl -n hw.ncpu)

# 6. Ausführen
./BaculaQtUI.app/Contents/MacOS/BaculaQtUI
```

## 📋 Voraussetzungen

### 1. Xcode Command Line Tools

```bash
xcode-select --install
```

Oder installieren Sie Xcode aus dem App Store.

### 2. Homebrew

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 3. Pakete via Homebrew

```bash
brew install \
    cmake \
    git \
    qt@6 \
    perl
```

**Optional (für Performance):**
```bash
brew install ninja ccache
```

### Alternativen zu Homebrew

#### MacPorts

```bash
sudo port install cmake git qt6 perl5
```

#### Qt Installer (Standalone)

Download: https://www.qt.io/download-qt-installer

**Empfohlene Installation:**
- Qt 6.8.0 oder höher
- macOS Clang Compiler
- Qt Creator (optional)

## 🎯 Build-Optionen

### Option 1: Statisches OpenSSL (Standard)

```bash
mkdir build && cd build
cmake .. \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6) \
    -DUSE_STATIC_OPENSSL=ON
make -j$(sysctl -n hw.ncpu)
```

**Vorteile:**
- ✅ Keine externen OpenSSL-Abhängigkeiten
- ✅ Konsistente OpenSSL-Version
- ✅ Einfaches Deployment (.app Bundle)
- ✅ Funktioniert auf allen macOS-Versionen

**Build-Zeit:** ~5-8 Minuten (beim ersten Mal)

### Option 2: System OpenSSL (Homebrew)

```bash
brew install openssl@3

mkdir build && cd build
cmake .. \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6) \
    -DUSE_STATIC_OPENSSL=OFF \
    -DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
make -j$(sysctl -n hw.ncpu)
```

**Vorteile:**
- ⚡ Schnellerer Build (~30 Sekunden)
- 📦 Kleinere Binary

**Nachteile:**
- 📚 Benötigt OpenSSL via Homebrew
- ⚠️ Deployment komplexer

### Build-Typen

```bash
# Release (Optimiert, Standard)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Debug (mit Debug-Symbolen)
cmake .. -DCMAKE_BUILD_TYPE=Debug

# RelWithDebInfo (Optimiert + Debug-Symbole)
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

### Universal Binary (Intel + Apple Silicon)

```bash
cmake .. \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
make -j$(sysctl -n hw.ncpu)
```

**Hinweis:** Qt6 muss für beide Architekturen installiert sein.

## ⏱️ Build-Zeiten

### Erstmaliger Build

| Mac | CPU | Cores | Statisches OpenSSL | System OpenSSL |
|-----|-----|-------|-------------------|----------------|
| Mac Studio M2 Ultra | M2 Ultra | 24 | ~3 min | ~20 sec |
| MacBook Pro M3 Max | M3 Max | 16 | ~4 min | ~25 sec |
| MacBook Pro M1 Pro | M1 Pro | 10 | ~5 min | ~30 sec |
| MacBook Air M1 | M1 | 8 | ~6 min | ~35 sec |
| iMac Intel i9 | i9-9900K | 8 | ~7 min | ~40 sec |
| MacBook Pro Intel i7 | i7-9750H | 6 | ~9 min | ~50 sec |

### Nachfolgende Builds (Incremental)

| Mac | Zeit |
|-----|------|
| Apple Silicon (M1/M2/M3) | ~8-12 sec |
| Intel | ~12-18 sec |

## 🍎 macOS-spezifische Features

### App Bundle erstellen

CMake erstellt automatisch ein `.app` Bundle:

```
build/BaculaQtUI.app/
├── Contents/
│   ├── Info.plist
│   ├── MacOS/
│   │   └── BaculaQtUI
│   ├── Resources/
│   │   └── (Icons, etc.)
│   └── Frameworks/
│       └── (Qt-Frameworks)
```

### macdeployqt verwenden

```bash
cd build

# Qt-Frameworks in das Bundle kopieren
$(brew --prefix qt@6)/bin/macdeployqt BaculaQtUI.app

# Optional: Code signieren
codesign --force --deep --sign - BaculaQtUI.app
```

### DMG-Installer erstellen

```bash
# create-dmg via Homebrew installieren
brew install create-dmg

# DMG erstellen
create-dmg \
    --volname "Bacula Qt UI" \
    --volicon "resources/icon.icns" \
    --window-pos 200 120 \
    --window-size 800 400 \
    --icon-size 100 \
    --icon "BaculaQtUI.app" 200 190 \
    --hide-extension "BaculaQtUI.app" \
    --app-drop-link 600 185 \
    "BaculaQtUI-1.0.0.dmg" \
    "build/BaculaQtUI.app"
```

**Oder manuell mit hdiutil:**

```bash
cd build

# Temporären Ordner erstellen
mkdir -p dmg-temp
cp -r BaculaQtUI.app dmg-temp/
ln -s /Applications dmg-temp/Applications

# DMG erstellen
hdiutil create -volname "Bacula Qt UI" \
    -srcfolder dmg-temp \
    -ov -format UDZO \
    BaculaQtUI-1.0.0.dmg

# Aufräumen
rm -rf dmg-temp
```

### Code Signing & Notarization

```bash
# Developer Certificate ID finden
security find-identity -v -p codesigning

# App signieren
codesign --force --deep \
    --sign "Developer ID Application: Joerg Bernau <support@onesimus.io> (TEAM_ID)" \
    --options runtime \
    BaculaQtUI.app

# Verifizieren
codesign --verify --verbose BaculaQtUI.app
spctl -a -v BaculaQtUI.app

# Notarisierung (für Distribution außerhalb App Store)
xcrun notarytool submit BaculaQtUI-1.0.0.dmg \
    --apple-id "your@email.com" \
    --team-id "TEAM_ID" \
    --password "app-specific-password"

# Notarisierung anheften
xcrun stapler staple BaculaQtUI.app
```

## 🐛 Fehlerbehebung

### "cmake: command not found"

```bash
brew install cmake
# ODER
sudo port install cmake
```

### "Qt6 not found"

**Mit Homebrew:**
```bash
brew install qt@6

# Qt-Pfad setzen
export CMAKE_PREFIX_PATH=$(brew --prefix qt@6)
```

**Mit Qt Installer:**
```bash
cmake .. -DCMAKE_PREFIX_PATH=~/Qt/6.8.0/macos
```

### "OpenSSL submodule download failed"

```bash
# Manuell initialisieren
git submodule update --init --recursive

# Prüfen
ls -la external/openssl/Configure
```

### "dyld: Library not loaded: @rpath/..."

Qt-Frameworks wurden nicht in das Bundle kopiert.

**Lösung:**
```bash
cd build
$(brew --prefix qt@6)/bin/macdeployqt BaculaQtUI.app
```

### "perl: command not found"

```bash
brew install perl
```

### M1/M2/M3 Mac: Rosetta 2 erforderlich

Für Intel-binaries auf Apple Silicon:

```bash
softwareupdate --install-rosetta
```

### OpenSSL Build schlägt fehl

**Logs prüfen:**
```bash
cat build/openssl_build-prefix/src/openssl_build-stamp/openssl_build-configure.log
cat build/openssl_build-prefix/src/openssl_build-stamp/openssl_build-build.log
```

**Häufige Ursachen:**
- Xcode Command Line Tools fehlen
- Perl nicht installiert
- Inkompatible Compiler-Flags

**OpenSSL neu bauen:**
```bash
cd build
rm -rf openssl-install openssl_build-prefix
cmake --build . --target openssl_build
```

### "Code signature invalid"

```bash
# Signatur entfernen und neu signieren
codesign --remove-signature BaculaQtUI.app
codesign --force --deep --sign - BaculaQtUI.app
```

## 🔍 Abhängigkeiten prüfen

### Welche Frameworks werden benötigt?

```bash
otool -L build/BaculaQtUI.app/Contents/MacOS/BaculaQtUI
```

**Mit statischem OpenSSL:**
```
@rpath/QtWidgets.framework/Versions/A/QtWidgets
@rpath/QtNetwork.framework/Versions/A/QtNetwork
@rpath/QtSql.framework/Versions/A/QtSql
@rpath/QtGui.framework/Versions/A/QtGui
@rpath/QtCore.framework/Versions/A/QtCore
/usr/lib/libc++.1.dylib
/usr/lib/libSystem.B.dylib
# KEIN OpenSSL → Statisch gelinkt! ✓
```

**Mit Homebrew OpenSSL:**
```
...
/opt/homebrew/opt/openssl@3/lib/libssl.3.dylib
/opt/homebrew/opt/openssl@3/lib/libcrypto.3.dylib
...
```

### Bundle-Struktur prüfen

```bash
tree -L 3 build/BaculaQtUI.app
```

## 🚀 Performance-Optimierung

### Ninja statt Make (schneller)

```bash
brew install ninja

cmake .. -G Ninja \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
ninja
```

**Speed-up:** ~20-30% schneller als make

### ccache (Compiler-Cache)

```bash
brew install ccache

# ccache aktivieren
export PATH=/opt/homebrew/opt/ccache/libexec:$PATH

# Bauen
cmake ..
make -j$(sysctl -n hw.ncpu)

# Statistiken anzeigen
ccache -s
```

### LTO (Link-Time Optimization)

```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
make -j$(sysctl -n hw.ncpu)
```

### Parallel Jobs optimieren

```bash
# Alle verfügbaren Cores
make -j$(sysctl -n hw.ncpu)

# Oder manuell (z.B. 8 Cores)
make -j8
```

## 🔒 Sicherheit & Updates

### OpenSSL aktualisieren

```bash
cd external/openssl
git fetch --all
git checkout openssl-3.6.1  # Neuere Version
cd ../..

# Rebuild
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
make -j$(sysctl -n hw.ncpu)
```

### Homebrew-Pakete aktualisieren

```bash
brew update
brew upgrade qt@6 cmake openssl@3
```

## 📦 Distribution

### Option 1: DMG-Installer

```bash
# Siehe "DMG-Installer erstellen" oben
create-dmg ...
```

### Option 2: PKG-Installer

```bash
# pkgbuild verwenden
pkgbuild \
    --root build/BaculaQtUI.app \
    --identifier com.bacula.baculaqtui \
    --version 1.0.0 \
    --install-location /Applications \
    BaculaQtUI-1.0.0.pkg
```

### Option 3: Homebrew Cask

```ruby
# bacula-qt-ui.rb
cask "bacula-qt-ui" do
  version "1.0.0"
  sha256 "..."
  
  url "https://github.com/your/repo/releases/download/v#{version}/BaculaQtUI-#{version}.dmg"
  name "Bacula Qt UI"
  desc "Modern Qt interface for Bacula Backup"
  homepage "https://github.com/your/repo"
  
  app "BaculaQtUI.app"
end
```

### Option 4: App Store (erfordert Apple Developer Account)

1. Xcode öffnen
2. Projekt signieren mit App Store Zertifikat
3. Archive erstellen
4. Via Xcode zu App Store Connect hochladen

## 🍺 Homebrew Formula (für Entwickler)

```ruby
# bacula-qt-ui.rb
class BaculaQtUi < Formula
  desc "Modern Qt interface for Bacula Backup"
  homepage "https://github.com/your/repo"
  url "https://github.com/your/repo/archive/v1.0.0.tar.gz"
  sha256 "..."
  license "GPL-3.0"
  
  depends_on "cmake" => :build
  depends_on "git" => :build
  depends_on "qt@6"
  
  def install
    system "cmake", "-S", ".", "-B", "build",
           "-DCMAKE_BUILD_TYPE=Release",
           "-DUSE_STATIC_OPENSSL=ON",
           *std_cmake_args
    system "cmake", "--build", "build"
    
    # App Bundle installieren
    prefix.install "build/BaculaQtUI.app"
    bin.write_exec_script "#{prefix}/BaculaQtUI.app/Contents/MacOS/BaculaQtUI"
  end
  
  test do
    assert_predicate prefix/"BaculaQtUI.app", :exist?
  end
end
```

**Installation:**
```bash
brew install --cask bacula-qt-ui
# ODER
brew install bacula-qt-ui
```

## 🎓 Erweiterte Verwendung

### Qt Creator verwenden

```bash
# Qt Creator öffnen
open $(brew --prefix qt@6)/bin/QtCreator.app

# Projekt öffnen: CMakeLists.txt
# Build konfigurieren
# F5 zum Debuggen
```

### Xcode-Projekt generieren

```bash
cmake .. -G Xcode \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)

open BaculaQtUI.xcodeproj
```

### Nur OpenSSL bauen

```bash
cd build
cmake --build . --target openssl_build
```

### Verbose Build

```bash
make VERBOSE=1
# ODER mit Ninja
ninja -v
```

### Clean Build

```bash
# Nur Binary neu kompilieren
make clean
make

# Kompletter Rebuild
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
make -j$(sysctl -n hw.ncpu)
```

## 🎨 macOS-Spezifische Anpassungen

### Dark Mode Support

Qt6 unterstützt automatisch macOS Dark Mode.

### Retina Display Support

```cpp
// In main.cpp (bereits implementiert in Qt6)
QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
```

### Native macOS Widgets

Qt6 verwendet native macOS-Widgets automatisch.

### Menu Bar Integration

```cpp
// Menu Bar auf macOS
menuBar()->setNativeMenuBar(true);
```

## 📊 Vergleichstabelle

| Feature | Statisches OpenSSL | Homebrew OpenSSL | System OpenSSL |
|---------|-------------------|------------------|----------------|
| Build-Zeit (1x) | 5-8 min | ~30 sec | N/A (deprecated) |
| Binary-Größe | ~18-22 MB | ~5-8 MB | ~5-8 MB |
| Abhängigkeiten | Nur Qt6 | Qt6 + Homebrew | Qt6 |
| Deployment | ✅ Einfach | ⚠️ Komplex | ❌ Nicht empfohlen |
| Universal Binary | ✅ Möglich | ✅ Möglich | ⚠️ Problematisch |
| **Produktion** | ✅ **Empfohlen** | ⚠️ OK | ❌ Veraltet |

**Hinweis:** macOS 10.15+ verwendet LibreSSL statt OpenSSL, daher ist System-OpenSSL nicht empfohlen.

## 🆘 Support

### System-Informationen sammeln

```bash
# macOS Version
sw_vers

# Xcode Version
xcodebuild -version

# Homebrew Info
brew --version
brew --prefix qt@6

# CMake Version
cmake --version

# CPU Info
sysctl -n machdep.cpu.brand_string
sysctl -n hw.ncpu
```

### Logs erstellen

```bash
# CMake-Logs
cat build/CMakeFiles/CMakeOutput.log
cat build/CMakeFiles/CMakeError.log

# OpenSSL-Build-Logs
find build/openssl_build-prefix -name "*.log" -exec cat {} \;

# Vollständiger Build mit Logs
make VERBOSE=1 2>&1 | tee build.log
```

### GitHub Issue erstellen

Bitte anhängen:
- System-Info (sw_vers, xcodebuild -version)
- CMake-Version
- Qt-Version
- Error-Logs
- Build-Befehl

## ✅ Quick Reference

```bash
# Standard Build (empfohlen)
./build.sh

# Manuell mit statischem OpenSSL
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
make -j$(sysctl -n hw.ncpu)

# Mit Homebrew OpenSSL (schnell)
cmake .. \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6) \
    -DUSE_STATIC_OPENSSL=OFF \
    -DOPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
make -j$(sysctl -n hw.ncpu)

# Universal Binary (Intel + Apple Silicon)
cmake .. \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
make -j$(sysctl -n hw.ncpu)

# Ausführen
open build/BaculaQtUI.app
# ODER
./build/BaculaQtUI.app/Contents/MacOS/BaculaQtUI

# Deployment vorbereiten
$(brew --prefix qt@6)/bin/macdeployqt build/BaculaQtUI.app

# DMG erstellen
create-dmg ... BaculaQtUI-1.0.0.dmg build/BaculaQtUI.app

# Code signieren
codesign --force --deep --sign - build/BaculaQtUI.app
```

## 🎉 Fertig!

**Happy Building auf macOS!** 🍎

Für weitere Hilfe:
- [Qt for macOS Documentation](https://doc.qt.io/qt-6/macos.html)
- [CMake on macOS](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#xcode)
- [Apple Developer Documentation](https://developer.apple.com/documentation/)
