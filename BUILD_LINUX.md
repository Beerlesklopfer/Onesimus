# Linux Build Guide

## 🚀 Quick Start

### Automated Build (Recommended)

```bash
./build.sh
```

The script:
- Automatically initializes Git submodules
- Checks all prerequisites
- Prompts for build options
- Configures CMake
- Compiles the project

### Manual Build

```bash
# 1. Install prerequisites
sudo apt-get update
sudo apt-get install -y build-essential cmake git qt6-base-dev qt6-tools-dev \
    libqt6network6-dev perl imagemagick

# 2. Clone repository
git clone <your-repo> onesimus
cd onesimus

# 3. Initialize Git submodules (optional - done automatically by CMake)
git submodule update --init --recursive

# 4. Create build directory
mkdir build && cd build

# 5. Configure CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 6. Compile
make -j$(nproc)

# 7. Run
./onesimus
```

## 📋 Prerequisites

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
    libqt6network6-dev \
    perl \
    make \
    imagemagick
```

**Required Qt6 modules:** Core, Gui, Widgets, Network, LinguistTools
**Note:** qt6-base-dev includes most modules, but explicit Network -dev package ensures headers are available

### Fedora/RHEL/CentOS

```bash
sudo dnf install -y \
    gcc-c++ \
    cmake \
    git \
    qt6-qtbase-devel \
    qt6-qttools-devel \
    qt6-qtbase-network-devel \
    perl \
    make \
    ImageMagick
```

**Required Qt6 modules:** Core, Gui, Widgets, Network, LinguistTools
**Note:** qt6-qtbase-devel includes Core, Gui, and Widgets; Network requires explicit package

### Arch Linux

```bash
sudo pacman -S --needed \
    base-devel \
    cmake \
    git \
    qt6-base \
    qt6-tools \
    perl \
    imagemagick
```

**Required Qt6 modules:** Core, Gui, Widgets, Network, LinguistTools
**Note:** qt6-base includes Core, Gui, Widgets, and Network modules with headers (Arch packages everything together)

### openSUSE

```bash
sudo zypper install -y \
    gcc-c++ \
    cmake \
    git \
    qt6-base-devel \
    qt6-tools-devel \
    qt6-network-devel \
    perl \
    make \
    ImageMagick
```

**Required Qt6 modules:** Core, Gui, Widgets, Network, LinguistTools
**Note:** Explicit Network -devel package ensures all headers are available

## 🎯 Build Options

### Option 1: Static OpenSSL (Default)

```bash
mkdir build && cd build
cmake .. -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)
```

**Advantages:**
- ✅ No external OpenSSL dependencies
- ✅ Consistent OpenSSL version
- ✅ Easy deployment (AppImage, etc.)

**Disadvantage:**
- ⏱️ Longer build time on first run (5-8 minutes)

### Option 2: System OpenSSL

```bash
mkdir build && cd build
cmake .. -DUSE_STATIC_OPENSSL=OFF
make -j$(nproc)
```

**Advantages:**
- ⚡ Faster build (~30 seconds)
- 📦 Smaller binary

**Disadvantage:**
- 📚 Requires libssl-dev/openssl-devel installed

### Build Types

#### Release (Optimized, Default)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

#### Debug (with debug symbols)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

#### RelWithDebInfo (Optimized + debug symbols)
```bash
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

### Advanced Build Options

#### Backup System Selection

```bash
# Bareos only (default and currently only supported)
cmake .. -DBACKUP_SYSTEM=BAREOS

# Bacula only (not yet implemented)
cmake .. -DBACKUP_SYSTEM=BACULA
```

**Note:** Currently only Bareos is fully implemented. Bacula support is planned.

#### Debug Options

Enable detailed debugging output for development and troubleshooting:

```bash
# Enable packet-level debugging
cmake .. -DDEBUG_PACKETS=ON

# Enable JSON message debugging
cmake .. -DDEBUG_JSON=ON

# Enable JSON logging to file
cmake .. -DLOG_JSON=ON
```

**Combined example:**
```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DDEBUG_JSON=ON \
    -DLOG_JSON=ON \
    -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)
```

**Warning:** Debug options generate extensive console output and may impact performance. Use only for development/debugging.

## 🔧 Build Script Options

The `build.sh` script offers interactive options:

```bash
./build.sh
```

**Prompts:**
```
OpenSSL Build Option:
1) Static (from submodule) - recommended for deployment
2) System OpenSSL - faster build

Choose [1/2] (Default: 1):
```

**With existing build directory:**
```
Build directory already exists.
Do you want to recreate it? (y/n)
```

## ⏱️ Build Times

### First Build

| System | Cores | Static OpenSSL | System OpenSSL |
|--------|-------|----------------|----------------|
| Intel i7-12700 | 12 | ~4 min | ~25 sec |
| AMD Ryzen 7 | 8 | ~5 min | ~30 sec |
| Intel i5-10400 | 6 | ~7 min | ~40 sec |
| Raspberry Pi 4 | 4 | ~25 min | ~2 min |

### Subsequent Builds (Incremental)

| System | Time |
|--------|------|
| Intel i7 | ~10-15 sec |
| AMD Ryzen 7 | ~12-18 sec |
| Intel i5 | ~15-20 sec |

## 📦 Installation

### System Installation

```bash
cd build
sudo make install
```

Default installation to: `/usr/local/bin/onesimus`

### Custom Installation Prefix

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/onesimus
make
sudo make install
```

### Create AppImage

```bash
# Download linuxdeployqt
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
chmod +x linuxdeployqt-continuous-x86_64.AppImage

# Create AppImage
./linuxdeployqt-continuous-x86_64.AppImage \
    build/onesimus \
    -appimage \
    -qmake=/usr/lib/qt6/bin/qmake
```

### Flatpak (TODO)

```bash
# Flatpak manifest to be created
flatpak-builder build-flatpak org.onesimus.Onesimus.yml
```

## 🐛 Troubleshooting

### "CMake not found"

```bash
sudo apt-get install cmake
# OR
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

**Custom Qt installation:**
```bash
cmake .. -DCMAKE_PREFIX_PATH=/opt/Qt/6.10.0/gcc_64
```

### "OpenSSL submodule download failed"

```bash
# Initialize manually
git submodule update --init --recursive

# Verify
ls -la external/openssl/Configure
```

### "undefined reference to SSL_*"

OpenSSL was not linked correctly.

**Solution:**
```bash
# Delete build directory
rm -rf build
mkdir build && cd build

# Reconfigure with static OpenSSL
cmake .. -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)
```

### "Permission denied" when running

```bash
chmod +x build/onesimus
```

### OpenSSL Build Fails

**Check logs:**
```bash
cat build/openssl_build-prefix/src/openssl_build-stamp/openssl_build-configure.log
cat build/openssl_build-prefix/src/openssl_build-stamp/openssl_build-build.log
```

**Common causes:**
- Missing Perl modules → `sudo apt-get install perl`
- Compiler errors → `sudo apt-get install build-essential`
- Missing Make → `sudo apt-get install make`

**Rebuild OpenSSL:**
```bash
cd build
rm -rf openssl-install openssl_build-prefix
cmake --build . --target openssl_build
```

### Qt6 Plugins Not Found

```bash
# Set Qt6 plugin path
export QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt6/plugins

# OR in the binary
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
```

## 🔍 Check Dependencies

### Which libraries are required?

```bash
ldd build/onesimus
```

**With static OpenSSL:**
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
# NO libssl.so, NO libcrypto.so → Statically linked! ✓
```

**With system OpenSSL:**
```
...
libssl.so.3
libcrypto.so.3
...
```

### Check OpenSSL Version

```bash
# System version
openssl version

# Statically linked (extract from binary)
strings build/onesimus | grep -i "openssl"
```

## 📊 Understanding Build Output

### Successful Build

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
Onesimus - Build Summary
========================================
Project:       onesimus v1.0.0
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
[ 90%] Building CXX object CMakeFiles/onesimus.dir/src/main.cpp.o
[100%] Linking CXX executable onesimus
```

## 🚀 Performance Optimization

### Ninja instead of Make (faster)

```bash
# Install Ninja
sudo apt-get install ninja-build

# Build with Ninja
cmake .. -G Ninja
ninja
```

### Compiler Cache (ccache)

```bash
# Install ccache
sudo apt-get install ccache

# Enable
export PATH=/usr/lib/ccache:$PATH

# Build
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

## 🔒 Security

### Update OpenSSL

```bash
cd external/openssl
git fetch --all
git checkout openssl-3.6.1  # Newer version
cd ../..

# Rebuild
rm -rf build
mkdir build && cd build
cmake .. -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)
```

### Automatic Submodule Update

```bash
# Update submodule to latest branch state
git submodule update --remote external/openssl

# Rebuild
rm -rf build && mkdir build && cd build
cmake .. && make -j$(nproc)
```

## 📦 Distribution-Specific Packages

### .deb (Debian/Ubuntu)

```bash
# With CPack
cd build
cpack -G DEB

# Or manually with dpkg-deb
mkdir -p onesimus_1.0.0/DEBIAN
mkdir -p onesimus_1.0.0/usr/local/bin

cat > onesimus_1.0.0/DEBIAN/control << EOF
Package: onesimus
Version: 1.0.0
Architecture: amd64
Maintainer: Joerg Bernau <Joerg@bernau.family>
Description: Modern Qt UI for Bareos/Bacula Backup
Depends: libqt6core6, libqt6gui6, libqt6widgets6, libqt6network6
EOF

cp onesimus onesimus_1.0.0/usr/local/bin/
dpkg-deb --build onesimus_1.0.0
```

### .rpm (Fedora/RHEL)

```bash
# With CPack
cd build
cpack -G RPM
```

### Arch Linux PKGBUILD

```bash
cat > PKGBUILD << 'EOF'
pkgname=onesimus
pkgver=1.0.0
pkgrel=1
pkgdesc="Modern Qt UI for Bareos/Bacula Backup"
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

## 🎓 Advanced Usage

### Build Only OpenSSL

```bash
cd build
cmake --build . --target openssl_build
```

### Verbose Build (for debugging)

```bash
make VERBOSE=1
```

### Parallel Build with Specific Number of Jobs

```bash
make -j4  # 4 cores
make -j8  # 8 cores
```

### Clean Build

```bash
# Recompile binary only
make clean
make

# Complete rebuild
rm -rf build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 📚 Summary

| Feature | Static OpenSSL | System OpenSSL |
|---------|----------------|----------------|
| Build time (1st time) | 5-8 min | ~30 sec |
| Build time (after) | ~15 sec | ~15 sec |
| Binary size | ~15-18 MB | ~3-5 MB |
| Dependencies | Only Qt6 | Qt6 + libssl |
| Deployment | ✅ Easy | ⚠️ Complex |
| AppImage | ✅ Ideal | ⚠️ Problematic |
| Production | ✅ Recommended | ⚠️ Not recommended |

**Recommendation:** Static OpenSSL for production and distribution!

## 🆘 Support

In case of problems:
1. Check logs: `cat build/CMakeFiles/CMakeOutput.log`
2. Verbose build: `make VERBOSE=1`
3. OpenSSL logs: `cat build/openssl_build-prefix/src/openssl_build-stamp/*.log`
4. Create GitHub issue with:
   - CMake version: `cmake --version`
   - Qt version: `qmake -v` or `qmake6 -v`
   - OS info: `cat /etc/os-release`
   - Error logs

## ✅ Quick Reference

```bash
# Standard build
./build.sh

# Manual with static OpenSSL
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_STATIC_OPENSSL=ON
make -j$(nproc)

# With system OpenSSL (fast)
cmake .. -DUSE_STATIC_OPENSSL=OFF
make -j$(nproc)

# Run
./build/onesimus

# Install
cd build && sudo make install

# Uninstall
cd build && sudo make uninstall
```

**Happy Building! 🎉**
