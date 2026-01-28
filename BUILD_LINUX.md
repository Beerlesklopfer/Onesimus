# Linux Build Guide

## 🚀 Schnellstart

### Automatischer Build (Empfohlen)

```bash
./build.sh
```

Das Script:
- Initialisiert Git-Submodule automatisch
- Prüft alle Voraussetzungen
- Fragt nach Build-Optionen
- Konfiguriert CMake
- Kompiliert das Projekt

### Manueller Build

```bash
# 1. Voraussetzungen installieren
sudo apt-get update
sudo apt-get install -y build-essential cmake git qt6-base-dev qt6-tools-dev perl

# 2. Repository klonen
git clone <your-repo> bacula-qt-ui
cd bacula-qt-ui

# 3. Git-Submodule initialisieren (optional - wird von CMake automatisch gemacht)
git submodule update --init --recursive

# 4. Build-Verzeichnis erstellen
mkdir build && cd build

# 5. CMake konfigurieren
cmake .. -DCMAKE_BUILD_TYPE=Release

# 6. Kompilieren
make -j$(nproc)

# 7. Ausführen
./BaculaQtUI
```

## 📋 Voraussetzungen

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    qt6-base-dev \
    qt6-tools-dev \
    qt6-base-dev-tools \
    libqt6sql6 \
    perl \
    make
```

### Fedora/RHEL/CentOS

```bash
sudo dnf install -y \
    gcc-c++ \
    cmake \
    git \
    qt6-qtbase-devel \
    qt6-qttools-devel \
    perl \
    make
```

### Arch Linux

```bash
sudo pacman -S --needed \
    base-devel \
    cmake \
    git \
    qt6-base \
    qt6-tools \
    perl
```

### openSUSE

```bash
sudo zypper install -y \
    gcc-c++ \
    cmake \
    git \
    qt6-base-devel \
    qt6-tools-devel \
    perl \
    make
```

## 🎯 Build-Optionen

### Option 1: Statisches OpenSSL (Standard)

```bash
mkdir build && cd build
cmake .. -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)
```

**Vorteile:**
- ✅ Keine externen OpenSSL-Abhängigkeiten
- ✅ Konsistente OpenSSL-Version
- ✅ Einfaches Deployment (AppImage, etc.)

**Nachteil:**
- ⏱️ Längere Build-Zeit beim ersten Mal (5-8 Minuten)

### Option 2: System OpenSSL

```bash
mkdir build && cd build
cmake .. -DUSE_STATIC_OPENSSL=OFF
make -j$(nproc)
```

**Vorteile:**
- ⚡ Schnellerer Build (~30 Sekunden)
- 📦 Kleinere Binary

**Nachteil:**
- 📚 Benötigt libssl-dev/openssl-devel installiert

### Build-Typen

#### Release (Optimiert, Standard)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

#### Debug (mit Debug-Symbolen)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

#### RelWithDebInfo (Optimiert + Debug-Symbole)
```bash
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

## 🔧 Build-Script Optionen

Das `build.sh` Script bietet interaktive Optionen:

```bash
./build.sh
```

**Fragt:**
```
OpenSSL Build-Option:
1) Statisch (aus Submodule) - empfohlen für Deployment
2) System OpenSSL - schneller Build

Wählen Sie [1/2] (Standard: 1):
```

**Bei bestehendem Build-Verzeichnis:**
```
Build-Verzeichnis existiert bereits.
Möchten Sie es neu erstellen? (j/n)
```

## ⏱️ Build-Zeiten

### Erstmaliger Build

| System | Cores | Statisches OpenSSL | System OpenSSL |
|--------|-------|-------------------|----------------|
| Intel i7-12700 | 12 | ~4 min | ~25 sec |
| AMD Ryzen 7 | 8 | ~5 min | ~30 sec |
| Intel i5-10400 | 6 | ~7 min | ~40 sec |
| Raspberry Pi 4 | 4 | ~25 min | ~2 min |

### Nachfolgende Builds (Incremental)

| System | Zeit |
|--------|------|
| Intel i7 | ~10-15 sec |
| AMD Ryzen 7 | ~12-18 sec |
| Intel i5 | ~15-20 sec |

## 📦 Installation

### System-Installation

```bash
cd build
sudo make install
```

Standard-Installation nach: `/usr/local/bin/BaculaQtUI`

### Custom Installation Prefix

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/bacula-qt-ui
make
sudo make install
```

### AppImage erstellen

```bash
# linuxdeployqt herunterladen
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
chmod +x linuxdeployqt-continuous-x86_64.AppImage

# AppImage erstellen
./linuxdeployqt-continuous-x86_64.AppImage \
    build/BaculaQtUI \
    -appimage \
    -qmake=/usr/lib/qt6/bin/qmake
```

### Flatpak (TODO)

```bash
# Flatpak-Manifest wird noch erstellt
flatpak-builder build-flatpak org.bacula.BaculaQtUI.yml
```

## 🐛 Fehlerbehebung

### "CMake not found"

```bash
sudo apt-get install cmake
# ODER
sudo dnf install cmake
```

### "Qt6 not found"

**Ubuntu/Debian:**
```bash
sudo apt-get install qt6-base-dev qt6-tools-dev
```

**Fedora:**
```bash
sudo dnf install qt6-qtbase-devel qt6-qttools-devel
```

**Custom Qt-Installation:**
```bash
cmake .. -DCMAKE_PREFIX_PATH=/opt/Qt/6.10.0/gcc_64
```

### "OpenSSL submodule download failed"

```bash
# Manuell initialisieren
git submodule update --init --recursive

# Prüfen
ls -la external/openssl/Configure
```

### "undefined reference to SSL_*"

OpenSSL wurde nicht korrekt gelinkt.

**Lösung:**
```bash
# Build-Verzeichnis löschen
rm -rf build
mkdir build && cd build

# Neu konfigurieren mit statischem OpenSSL
cmake .. -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)
```

### "Permission denied" beim Ausführen

```bash
chmod +x build/BaculaQtUI
```

### OpenSSL Build schlägt fehl

**Log prüfen:**
```bash
cat build/openssl_build-prefix/src/openssl_build-stamp/openssl_build-configure.log
cat build/openssl_build-prefix/src/openssl_build-stamp/openssl_build-build.log
```

**Häufige Ursachen:**
- Fehlende Perl-Module → `sudo apt-get install perl`
- Compiler-Fehler → `sudo apt-get install build-essential`
- Fehlende Make → `sudo apt-get install make`

**OpenSSL neu bauen:**
```bash
cd build
rm -rf openssl-install openssl_build-prefix
cmake --build . --target openssl_build
```

### Qt6-Plugins nicht gefunden

```bash
# Qt6-Plugin-Pfad setzen
export QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt6/plugins

# ODER in der Binary
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
```

## 🔍 Abhängigkeiten prüfen

### Welche Bibliotheken werden benötigt?

```bash
ldd build/BaculaQtUI
```

**Mit statischem OpenSSL:**
```
linux-vdso.so.1
libQt6Widgets.so.6
libQt6Network.so.6
libQt6Sql.so.6
libQt6Gui.so.6
libQt6Core.so.6
libstdc++.so.6
libm.so.6
libgcc_s.so.1
libc.so.6
# KEIN libssl.so, KEIN libcrypto.so → Statisch gelinkt! ✓
```

**Mit System-OpenSSL:**
```
...
libssl.so.3
libcrypto.so.3
...
```

### OpenSSL-Version prüfen

```bash
# System-Version
openssl version

# Statisch gelinkt (aus Binary extrahieren)
strings build/BaculaQtUI | grep -i "openssl"
```

## 📊 Build-Ausgabe verstehen

### Erfolgreicher Build

```
========================================
Static OpenSSL Configuration
========================================
Checking OpenSSL submodule...
✓ OpenSSL submodule already present
OpenSSL Version: 3.6.0
Build System: Unix Makefiles
Parallel Build: 8 cores

Configuring OpenSSL ExternalProject...
✓ OpenSSL configured as ExternalProject
  Source: external/openssl/
  Install: build/openssl-install/
========================================

✓ Found Qt6 6.8.0

========================================
Bacula Qt UI - Build Summary
========================================
Project:       BaculaQtUI v1.0.0
Build Type:    Release
Install:       /usr/local

Qt Version:    6.8.0
C++ Standard:  17

OpenSSL:       Static (auto-downloaded via git submodule)
  Version:     3.6.0
  Branch:      openssl-3.6.0
  Source:      external/openssl/
  Install:     /path/to/build/openssl-install
========================================

Build commands:
  make -j8      - Compile project
  make install  - Install executable
========================================

[ 10%] Building openssl_build
[ 20%] Performing configure step for 'openssl_build'
...
[ 90%] Building CXX object CMakeFiles/BaculaQtUI.dir/src/main.cpp.o
[100%] Linking CXX executable BaculaQtUI
```

## 🚀 Performance-Optimierung

### Ninja statt Make (schneller)

```bash
# Ninja installieren
sudo apt-get install ninja-build

# Mit Ninja bauen
cmake .. -G Ninja
ninja
```

### Compiler-Cache (ccache)

```bash
# ccache installieren
sudo apt-get install ccache

# Aktivieren
export PATH=/usr/lib/ccache:$PATH

# Bauen
cmake ..
make -j$(nproc)
```

### LTO (Link-Time Optimization)

```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
make -j$(nproc)
```

## 🔒 Sicherheit

### OpenSSL aktualisieren

```bash
cd external/openssl
git fetch --all
git checkout openssl-3.6.1  # Neuere Version
cd ../..

# Rebuild
rm -rf build
mkdir build && cd build
cmake .. -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)
```

### Automatisches Submodule-Update

```bash
# Updated Submodule auf neuesten Branch-Stand
git submodule update --remote external/openssl

# Rebuild
rm -rf build && mkdir build && cd build
cmake .. && make -j$(nproc)
```

## 📦 Distribution-spezifische Pakete

### .deb (Debian/Ubuntu)

```bash
# Mit CPack
cd build
cpack -G DEB

# Oder manuell mit dpkg-deb
mkdir -p bacula-qt-ui_1.0.0/DEBIAN
mkdir -p bacula-qt-ui_1.0.0/usr/local/bin

cat > bacula-qt-ui_1.0.0/DEBIAN/control << EOF
Package: bacula-qt-ui
Version: 1.0.0
Architecture: amd64
Maintainer: Joerg Bernau <Joerg@bernau.family> 
Description: Modern Qt UI for Bacula Backup
Depends: libqt6core6, libqt6gui6, libqt6widgets6, libqt6network6
EOF

cp BaculaQtUI bacula-qt-ui_1.0.0/usr/local/bin/
dpkg-deb --build bacula-qt-ui_1.0.0
```

### .rpm (Fedora/RHEL)

```bash
# Mit CPack
cd build
cpack -G RPM
```

### Arch Linux PKGBUILD

```bash
cat > PKGBUILD << 'EOF'
pkgname=bacula-qt-ui
pkgver=1.0.0
pkgrel=1
pkgdesc="Modern Qt UI for Bacula Backup"
arch=('x86_64')
depends=('qt6-base' 'qt6-tools')
makedepends=('cmake' 'git' 'perl')

build() {
    cmake -B build -S . \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build
}

package() {
    DESTDIR="$pkgdir" cmake --install build
}
EOF

makepkg -si
```

## 🎓 Erweiterte Verwendung

### Nur OpenSSL bauen

```bash
cd build
cmake --build . --target openssl_build
```

### Verbose Build (für Debugging)

```bash
make VERBOSE=1
```

### Parallel-Build mit bestimmter Anzahl Jobs

```bash
make -j4  # 4 Cores
make -j8  # 8 Cores
```

### Clean Build

```bash
# Nur Binary neu kompilieren
make clean
make

# Kompletter Rebuild
rm -rf build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 📚 Zusammenfassung

| Feature | Statisches OpenSSL | System OpenSSL |
|---------|-------------------|----------------|
| Build-Zeit (1. Mal) | 5-8 min | ~30 sec |
| Build-Zeit (danach) | ~15 sec | ~15 sec |
| Binary-Größe | ~15-18 MB | ~3-5 MB |
| Abhängigkeiten | Nur Qt6 | Qt6 + libssl |
| Deployment | ✅ Einfach | ⚠️ Komplex |
| AppImage | ✅ Ideal | ⚠️ Problematisch |
| Produktion | ✅ Empfohlen | ⚠️ Nicht empfohlen |

**Empfehlung:** Statisches OpenSSL für Produktion und Distribution!

## 🆘 Support

Bei Problemen:
1. Prüfen Sie die Logs: `cat build/CMakeFiles/CMakeOutput.log`
2. Verbose Build: `make VERBOSE=1`
3. OpenSSL-Logs: `cat build/openssl_build-prefix/src/openssl_build-stamp/*.log`
4. GitHub Issues erstellen mit:
   - CMake-Version: `cmake --version`
   - Qt-Version: `qmake -v` oder `qmake6 -v`
   - OS-Info: `cat /etc/os-release`
   - Error-Logs

## ✅ Quick Reference

```bash
# Standard Build
./build.sh

# Manuell mit statischem OpenSSL
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)

# Mit System-OpenSSL (schnell)
cmake .. -DUSE_STATIC_OPENSSL=OFF
make -j$(nproc)

# Ausführen
./build/BaculaQtUI

# Installieren
cd build && sudo make install

# Deinstallieren
cd build && sudo make uninstall
```

**Happy Building! 🎉**
