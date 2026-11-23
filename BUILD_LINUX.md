# Linux Build Guide

## 🚀 Quick Start

### Automated Build (Recommended)

```bash
./deploy.sh build      # build only
./deploy.sh            # build + deploy to /usr (sudo per step)
./deploy.sh -n         # dry run: print the commands a deploy would run
```

The script:
- Initializes the Git submodules it needs (external/openssl for a static OpenSSL, proto/)
- Checks the prerequisites
- Configures CMake (Ninja when installed) and compiles the project
- Deploys binary, desktop entry and icons into /usr (`all`, the default)

`./deploy.fish` is the same script for the fish shell. See
[Build Script Options](#-build-script-options) for all commands.

### Manual Build

```bash
# 1. Install prerequisites
sudo apt-get update
sudo apt-get install -y build-essential cmake git \
    qt6-base-dev qt6-base-private-dev qt6-tools-dev \
    qt6-tools-dev-tools libqt6network6-dev \
    qt6-grpc-dev qt6-grpc-tools protobuf-compiler libprotobuf-dev \
    perl imagemagick libssl-dev libxkbcommon-dev libsecret-1-dev

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

CMake 3.25 or newer (Debian 13: 3.31, Ubuntu 24.04: 3.28), Qt 6.8 or newer with the private
headers, and for the gRPC transport to the Onesimus Director QtGrpc/QtProtobuf with `protoc`
(Debian 13 below; without them the build falls back to BNET only).

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    qt6-base-dev \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    qt6-base-dev-tools \
    libqt6network6-dev \
    libsecret-1-dev \
    perl \
    make \
    imagemagick
```

**Required Qt6 modules:** Core, Gui, Widgets, Network, LinguistTools

**Important packages:**
- **libsecret-1-dev**: the system keychain (Secret Service: GNOME Keyring, KWallet,
  KeePassXC) that keeps the passwords of the connection profiles out of the settings file.
  Required by default; at run time the package needs `libsecret-1-0` (a `Depends` of the
  `.deb`). `-DONESIMUS_KEYCHAIN=OFF` builds without it; the passwords then stay in the
  settings file and the program says so at every start.
- **qt6-base-dev**: Qt6 base development files (Core, Gui, Widgets)
- **qt6-tools-dev**: Qt6 tools including Designer, UIC
- **qt6-tools-dev-tools**: Qt6 LinguistTools (lupdate, lrelease) for translations
- **libqt6network6-dev**: Qt6 Network module development files

### Debian 13 (trixie) with gRPC - the CI environment

The gRPC transport needs QtGrpc/QtProtobuf, which Debian ships from Qt 6.8 on
(Debian 13; Ubuntu 24.04 has Qt 6.4 only). The CI Linux job
([`.github/workflows/ci.yml`](.github/workflows/ci.yml)) installs exactly this list in a
`debian:trixie` container and then runs `./deploy.sh configure`, `build` and `test`:

```bash
sudo apt-get install -y --no-install-recommends \
    ca-certificates git curl file perl make build-essential cmake ninja-build ccache \
    dpkg-dev librsvg2-bin \
    qt6-base-dev qt6-base-private-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools \
    qt6-grpc-dev qt6-grpc-tools protobuf-compiler libprotobuf-dev libgl-dev libsecret-1-dev pkgconf
```

`OPENSSL_INSTALL_DIR=<dir>` keeps the static OpenSSL build outside the build tree:
the first build compiles it into `<dir>`, every later tree (and `./deploy.sh appimage`)
reuses it. The CI caches that directory per OpenSSL submodule commit.

Release packages (`.deb`, AppImage) are built the same way by
[`.github/workflows/release.yml`](.github/workflows/release.yml) on a `v*` tag; see
"CI and Releases" in the README.

### Fedora/RHEL/CentOS

```bash
sudo dnf install -y \
    gcc-c++ \
    cmake \
    git \
    qt6-qtbase-devel \
    qt6-qttools-devel \
    qt6-linguist \
    qt6-qtbase-network-devel \
    libsecret-devel \
    perl \
    make \
    ImageMagick
```

**Required Qt6 modules:** Core, Gui, Widgets, Network, LinguistTools

**Important packages:**
- **qt6-qtbase-devel**: Qt6 base development files (Core, Gui, Widgets)
- **qt6-qttools-devel**: Qt6 tools including Designer, UIC
- **qt6-linguist**: Qt6 LinguistTools (lupdate, lrelease) for translations
- **qt6-qtbase-network-devel**: Qt6 Network module development files

### Arch Linux

```bash
sudo pacman -S --needed \
    base-devel \
    cmake \
    git \
    qt6-base \
    qt6-tools \
    libsecret \
    perl \
    imagemagick
```

**Required Qt6 modules:** Core, Gui, Widgets, Network, LinguistTools

**Important packages:**
- **qt6-base**: Qt6 base (Core, Gui, Widgets, Network) with headers
- **qt6-tools**: Qt6 tools including Designer, UIC, and LinguistTools (lupdate, lrelease) for translations

**Note:** Arch packages Qt6 modules comprehensively, so qt6-tools includes LinguistTools automatically

### openSUSE

```bash
sudo zypper install -y \
    gcc-c++ \
    cmake \
    git \
    qt6-base-devel \
    qt6-tools-devel \
    qt6-linguist-devel \
    qt6-network-devel \
    libsecret-devel \
    perl \
    make \
    ImageMagick
```

**Required Qt6 modules:** Core, Gui, Widgets, Network, LinguistTools

**Important packages:**
- **qt6-base-devel**: Qt6 base development files (Core, Gui, Widgets)
- **qt6-tools-devel**: Qt6 tools including Designer, UIC
- **qt6-linguist-devel**: Qt6 LinguistTools (lupdate, lrelease) for translations
- **qt6-network-devel**: Qt6 Network module development files

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

`deploy.sh` (and its fish twin `deploy.fish`) takes a command and flags.
`./deploy.sh help` prints the full table.

| Command | What it does |
|---|---|
| `all`, `onesimus`, `gui` (default) | Build, then copy `onesimus` to `/usr/local/bin`, the desktop entry to `/usr/local/share/applications`, the icons to `/usr/local/share/icons/hicolor` (`DEPLOY_PREFIX`, default `/usr/local`, the old destination of `make install`), and refresh the icon/desktop caches. Each step runs `sudo` on its own; do not start the script with `sudo`. |
| `configure [args]` | Create the build tree; extra arguments go to cmake |
| `build` / `rebuild` | Build (incremental) / clean rebuild |
| `run [args]` | Build and start onesimus |
| `test` | Build and run ctest (offscreen) |
| `install` | `cmake --install` into `PREFIX` (default `/usr/local`, sudo only when not writable) |
| `package` | `.deb` via CPack in the build directory |
| `appimage` | AppImage in `dist/appimage/` (linuxdeploy in a pinned, checksummed version) |
| `status` | What is deployed, built (with stamped version) and packaged |
| `clean` | Remove the build trees |

| Flag | Meaning |
|---|---|
| `-t, --type TYPE` | Release (default), Debug, RelWithDebInfo, MinSizeRel |
| `-s, --system-openssl` | System OpenSSL instead of the static submodule (default: static) |
| `-c, --clean` | Remove the build directory first |
| `-j, --jobs N` | Parallel jobs (default: number of CPUs) |
| `-d, --build-dir DIR` | Build directory (default: `build`) |
| `-n, --dry-run` | Print every command instead of running it |

Environment: `BUILD_DIR`, `BUILD_TYPE`, `JOBS`, `OPENSSL` (static/system),
`GENERATOR`, `PREFIX`, `DEPLOY_PREFIX` (default `/usr/local`), `CMAKE_PREFIX_PATH`,
`PACKAGE_GENERATORS`, `APPIMAGE_TOOLS`, `CLEAN`, `DRY_RUN`, `ONESIMUS_ALLOW_ROOT`.

An existing build tree keeps its build type and OpenSSL mode unless `-t`/`-s`
(or `BUILD_TYPE`/`OPENSSL`) is given. The build writes `version.h`,
`buildinfo.cpp` and `version.txt` into `<build>/generated/` and nothing into the
source tree, so an unchanged build compiles nothing. The build number is
`GITHUB_RUN_NUMBER` in CI, else the number of commits up to HEAD;
`-DONESIMUS_INCREMENT_BUILD=ON` counts it up on every build instead.

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
./deploy.sh appimage     # -> dist/appimage/Onesimus-<version>-x86_64.AppImage
```

The script builds a Release tree with `PREFIX=/usr`, installs it into an AppDir and
runs linuxdeploy with the Qt plugin (pinned versions, checked against SHA-256).
The AppImage needs a glibc at least as new as the build system's; the release
workflow builds it on Debian 13.

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
Maintainer: Joerg Bernau <support@onesimus.io>
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
# Standard build / build + deploy to /usr
./deploy.sh build
./deploy.sh

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
