#!/bin/bash

# Onesimus Build-Script mit OpenSSL-Unterstützung
# Dieses Script automatisiert den Build-Prozess

set -e  # Beende bei Fehler

# --- Default values ---
BUILD_TYPE=Release
OPENSSL_MODE=static
CLEAN=false
JOBS=$(nproc)
BUILD_DIR=build

# --- Usage ---
usage() {
    echo "Usage: build.sh [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -t, --type TYPE        Build type: Release (default), Debug, RelWithDebInfo"
    echo "  -s, --system-openssl   Use system OpenSSL instead of static submodule"
    echo "  -c, --clean            Remove build directory before building"
    echo "  -j, --jobs N           Parallel build jobs (default: nproc = $JOBS)"
    echo "  -d, --build-dir DIR    Build directory (default: build)"
    echo "  -h, --help             Show this help"
    echo ""
    echo "Examples:"
    echo "  build.sh                     # Release, static OpenSSL"
    echo "  build.sh -t Debug            # Debug build"
    echo "  build.sh -s -c               # System OpenSSL, clean rebuild"
    echo "  build.sh -j4 -t Debug        # Debug, 4 jobs"
}

# --- Parse arguments ---
while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            usage
            exit 0
            ;;
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -s|--system-openssl)
            OPENSSL_MODE=system
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        -d|--build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
done

# --- Validate build type ---
case "$BUILD_TYPE" in
    Release|Debug|RelWithDebInfo|MinSizeRel)
        ;;
    *)
        echo "ERROR: Invalid build type '$BUILD_TYPE'"
        echo "Valid types: Release, Debug, RelWithDebInfo, MinSizeRel"
        exit 1
        ;;
esac

# --- Map openssl mode to CMake variable ---
if [ "$OPENSSL_MODE" = "static" ]; then
    USE_STATIC_OPENSSL=ON
else
    USE_STATIC_OPENSSL=OFF
fi

echo "================================"
echo "Onesimus - Build Script"
echo "================================"
echo ""
echo "  Build type:   $BUILD_TYPE"
echo "  OpenSSL:      $OPENSSL_MODE"
echo "  Jobs:         $JOBS"
echo "  Build dir:    $BUILD_DIR"
echo ""

# --- Check prerequisites ---
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake is not installed!"
    echo "Install: sudo apt-get install cmake"
    exit 1
fi

if ! command -v qmake6 &> /dev/null && ! command -v qmake &> /dev/null; then
    echo "WARNING: Qt6 may not be installed!"
    echo "Install: sudo apt-get install qt6-base-dev qt6-tools-dev"
fi

if ! command -v lupdate &> /dev/null && ! command -v lupdate-qt6 &> /dev/null; then
    echo "WARNING: Qt6 LinguistTools (lupdate/lrelease) not found!"
    echo "Install:"
    echo "  Ubuntu/Debian: sudo apt-get install qt6-tools-dev-tools"
    echo "  Fedora/RHEL:   sudo dnf install qt6-linguist"
    echo "  Arch:          sudo pacman -S qt6-tools"
    echo "  openSUSE:      sudo zypper install qt6-linguist-devel"
    echo ""
fi

# --- Git submodules ---
echo "Checking git submodules..."
if [ ! -f "external/openssl/Configure" ]; then
    echo "OpenSSL submodule not initialized. Initializing..."
    git submodule update --init --recursive
    echo "Submodules initialized."
else
    echo "OpenSSL submodule already initialized."
fi
echo ""

# --- Build directory ---
if [ "$CLEAN" = "true" ] && [ -d "$BUILD_DIR" ]; then
    echo "Removing old build directory..."
    rm -rf "$BUILD_DIR"
fi

if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# --- CMake configure ---
echo ""
echo "================================"
echo "CMake Configuration"
echo "================================"
echo ""

cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DUSE_STATIC_OPENSSL="$USE_STATIC_OPENSSL"

# --- Build ---
echo ""
echo "================================"
echo "Compilation"
echo "================================"
echo ""

if [ "$USE_STATIC_OPENSSL" = "ON" ]; then
    echo "NOTE: OpenSSL is being compiled — this may take 5–10 minutes..."
    echo ""
fi

cmake --build "$BUILD_DIR" -j"$JOBS"

echo ""
echo "================================"
echo "Build completed successfully!"
echo "================================"
echo ""
echo "Executable: ./$BUILD_DIR/onesimus"
echo ""

if [ "$USE_STATIC_OPENSSL" = "ON" ]; then
    echo "✓ OpenSSL was statically linked"
    echo "✓ No external OpenSSL libraries required"
fi

echo ""
echo "Run with:     ./$BUILD_DIR/onesimus"
echo "Install with: cd $BUILD_DIR && sudo make install"
echo ""
