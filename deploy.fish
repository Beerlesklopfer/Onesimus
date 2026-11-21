#!/usr/bin/env fish

# Onesimus Build-Script mit OpenSSL-Unterstützung
# Dieses Script automatisiert den Build-Prozess

# --- Default values ---
set build_type Release
set openssl_mode static
set clean false
set jobs (nproc)
set build_dir build

# --- Usage ---
function usage
    echo "Usage: build.fish [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -t, --type TYPE        Build type: Release (default), Debug, RelWithDebInfo"
    echo "  -s, --system-openssl   Use system OpenSSL instead of static submodule"
    echo "  -c, --clean            Remove build directory before building"
    echo "  -j, --jobs N           Parallel build jobs (default: nproc = $jobs)"
    echo "  -d, --build-dir DIR    Build directory (default: build)"
    echo "  -h, --help             Show this help"
    echo ""
    echo "Examples:"
    echo "  build.fish                     # Release, static OpenSSL"
    echo "  build.fish -t Debug            # Debug build"
    echo "  build.fish -s -c               # System OpenSSL, clean rebuild"
    echo "  build.fish -j4 -t Debug        # Debug, 4 jobs"
end

# --- Parse arguments ---
set i 1
while test $i -le (count $argv)
    switch $argv[$i]
        case -h --help
            usage
            exit 0
        case -t --type
            set i (math $i + 1)
            if test $i -gt (count $argv)
                echo "ERROR: --type requires an argument"
                exit 1
            end
            set build_type $argv[$i]
        case -s --system-openssl
            set openssl_mode system
        case -c --clean
            set clean true
        case -j --jobs
            set i (math $i + 1)
            if test $i -gt (count $argv)
                echo "ERROR: --jobs requires an argument"
                exit 1
            end
            set jobs $argv[$i]
        case -d --build-dir
            set i (math $i + 1)
            if test $i -gt (count $argv)
                echo "ERROR: --build-dir requires an argument"
                exit 1
            end
            set build_dir $argv[$i]
        case '*'
            echo "Unknown option: $argv[$i]"
            usage
            exit 1
    end
    set i (math $i + 1)
end

# --- Validate build type ---
switch $build_type
    case Release Debug RelWithDebInfo MinSizeRel
        # valid
    case '*'
        echo "ERROR: Invalid build type '$build_type'"
        echo "Valid types: Release, Debug, RelWithDebInfo, MinSizeRel"
        exit 1
end

# --- Map openssl mode to CMake variable ---
if test $openssl_mode = static
    set use_static_openssl ON
else
    set use_static_openssl OFF
end

echo "================================"
echo "Onesimus - Build Script"
echo "================================"
echo ""
echo "  Build type:   $build_type"
echo "  OpenSSL:      $openssl_mode"
echo "  Jobs:         $jobs"
echo "  Build dir:    $build_dir"
echo ""

# --- Check prerequisites ---
if not command -q cmake
    echo "ERROR: CMake is not installed!"
    echo "Install: sudo apt-get install cmake"
    exit 1
end

if not command -q qmake6; and not command -q qmake
    echo "WARNING: Qt6 may not be installed!"
    echo "Install: sudo apt-get install qt6-base-dev qt6-tools-dev"
end

if not command -q lupdate; and not command -q lupdate-qt6
    echo "WARNING: Qt6 LinguistTools (lupdate/lrelease) not found!"
    echo "Install:"
    echo "  Ubuntu/Debian: sudo apt-get install qt6-tools-dev-tools"
    echo "  Fedora/RHEL:   sudo dnf install qt6-linguist"
    echo "  Arch:          sudo pacman -S qt6-tools"
    echo "  openSUSE:      sudo zypper install qt6-linguist-devel"
    echo ""
end

# --- Git submodules ---
echo "Checking git submodules..."
if not test -f external/openssl/Configure
    echo "OpenSSL submodule not initialized. Initializing..."
    git submodule update --init --recursive
    echo "Submodules initialized."
else
    echo "OpenSSL submodule already initialized."
end
echo ""

# --- Build directory ---
if test $clean = true; and test -d $build_dir
    echo "Removing old build directory..."
    rm -rf $build_dir
end

if not test -d $build_dir
    echo "Creating build directory..."
    mkdir -p $build_dir
end

# --- CMake configure ---
echo ""
echo "================================"
echo "CMake Configuration"
echo "================================"
echo ""

cmake -S . -B $build_dir \
    -DCMAKE_BUILD_TYPE=$build_type \
    -DUSE_STATIC_OPENSSL=$use_static_openssl

# --- Build ---
echo ""
echo "================================"
echo "Compilation"
echo "================================"
echo ""

if test $use_static_openssl = ON
    echo "NOTE: OpenSSL is being compiled — this may take 5–10 minutes..."
    echo ""
end

cmake --build $build_dir -j$jobs

echo ""
echo "================================"
echo "Build completed successfully!"
echo "================================"
echo ""
echo "Executable: ./$build_dir/onesimus"
echo ""

if test $use_static_openssl = ON
    echo "✓ OpenSSL was statically linked"
    echo "✓ No external OpenSSL libraries required"
end

echo ""
echo "Run with:     ./$build_dir/onesimus"
echo "Install with: cd $build_dir && sudo make install"
echo ""
