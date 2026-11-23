#!/usr/bin/env fish
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

# ─── Onesimus deploy script (Linux / macOS), fish edition ───────────────────
#
# Fish twin of deploy.sh: the same verbs, flags and environment variables,
# modelled on deploy.fish of forgeiec-studio. Every verb builds what it needs
# first; every privileged step raises itself with sudo on its own.
#
# Usage:
#   ./deploy.fish                  build + deploy to /usr (sudo per step)
#   ./deploy.fish build            build only
#   ./deploy.fish --dry-run all    print every command instead of running it
#   ./deploy.fish help             the full verb table
#
# The build stamps the version: CMake's increment_build_number target
# increments .build_number and regenerates include/version.h on every build.
# Both files are tracked; do not commit the stamped values from a dev build.

# ─── First: not as root (see deploy.sh for the reason) ──────────────────────
if test (id -u) -ne 0
    # nothing to do
else if test "$ONESIMUS_ALLOW_ROOT" = 1
    printf '[deploy] ONESIMUS_ALLOW_ROOT=1 - continuing as root.\n' >&2
else
    set -l first (test (count $argv) -gt 0; and echo $argv[1]; or echo "<command>")
    printf '[deploy] Do not run this script as root.\n' >&2
    printf '[deploy]   It raises itself with sudo for every privileged step.\n' >&2
    printf '[deploy]   As root the build tree and version.h become root-owned.\n' >&2
    printf '[deploy]\n' >&2
    printf '[deploy]   Right:  ./deploy.fish %s\n' $first >&2
    printf '[deploy]   Container without a user:  ONESIMUS_ALLOW_ROOT=1 ./deploy.fish %s\n' $first >&2
    exit 1
end

set -g SCRIPT_DIR (cd (dirname (status filename)); and pwd)
cd $SCRIPT_DIR
set -g OS (uname -s)

# ─── Settings (the same environment variables as deploy.sh) ─────────────────
function cpu_count
    nproc 2>/dev/null; or sysctl -n hw.ncpu 2>/dev/null; or echo 4
end
set -g BUILD_TYPE_SET 0; set -q BUILD_TYPE; and test -n "$BUILD_TYPE"; and set BUILD_TYPE_SET 1
set -g OPENSSL_SET 0;    set -q OPENSSL; and test -n "$OPENSSL"; and set OPENSSL_SET 1
set -q BUILD_TYPE; and test -n "$BUILD_TYPE"; or set -g BUILD_TYPE Release
set -q JOBS; and test -n "$JOBS"; or set -g JOBS (cpu_count)
set -q BUILD_DIR; and test -n "$BUILD_DIR"; or set -g BUILD_DIR build
set -q OPENSSL; and test -n "$OPENSSL"; or set -g OPENSSL static
set -q PREFIX; and test -n "$PREFIX"; or set -g PREFIX /usr/local
# Target of the deploy step: /usr/local, the old destination of "make install"
set -q DEPLOY_PREFIX; and test -n "$DEPLOY_PREFIX"; or set -g DEPLOY_PREFIX /usr/local
if not set -q GENERATOR; or test -z "$GENERATOR"
    if command -q ninja
        set -g GENERATOR Ninja
    else
        set -g GENERATOR ""
    end
end
set -q CLEAN; or set -g CLEAN 0
set -q DRY_RUN; or set -g DRY_RUN 0

# ─── CLI flags (before or after the verb; same as the former build.fish) ────
set -g positional
set -l i 1
while test $i -le (count $argv)
    set -l a $argv[$i]
    switch $a
        case -t --type -j --jobs -d --build-dir
            set i (math $i + 1)
            if test $i -gt (count $argv)
                printf '[deploy] %s requires an argument\n' $a >&2
                exit 1
            end
            switch $a
                case -t --type
                    set -g BUILD_TYPE $argv[$i]; set -g BUILD_TYPE_SET 1
                case -j --jobs
                    set -g JOBS $argv[$i]
                case '*'
                    set -g BUILD_DIR $argv[$i]
            end
        case '-j*'  # -j4 (fish globs have no [0-9])
            set -g JOBS (string sub -s 3 -- $a)
        case -s --system-openssl
            set -g OPENSSL system; set -g OPENSSL_SET 1
        case -c --clean
            set -g CLEAN 1
        case -n --dry-run
            set -g DRY_RUN 1
        case -h --help
            set -a positional help
        case --
            set -a positional $argv[(math $i + 1)..-1]
            break
        case run configure
            # Everything after `run`/`configure` belongs to onesimus/cmake.
            set -a positional $argv[$i..-1]
            break
        case '*'
            set -a positional $a
    end
    set i (math $i + 1)
end

# ─── Messages ───────────────────────────────────────────────────────────────
function info; set_color cyan;   echo -n "[deploy]"; set_color normal; echo " $argv"; end
function ok;   set_color green;  echo -n "[deploy]"; set_color normal; echo " $argv"; end
function warn; set_color yellow; echo -n "[deploy]"; set_color normal; echo " $argv"; end
function err;  set_color red;    echo -n "[deploy]" >&2; set_color normal; echo " $argv" >&2; end
function die;  err $argv; exit 1; end
function need; command -q $argv[1]; or die "missing tool: $argv[1]"; end

# Every command that changes something goes through run/run_sudo, so that
# --dry-run shows exactly what a real run would execute and runs none of it.
function run
    if test "$DRY_RUN" = 1
        echo "  + $argv"
    else
        command $argv
    end
end
function run_sudo
    if test "$DRY_RUN" = 1
        echo "  + sudo $argv"
    else if test (id -u) -eq 0
        command $argv
    else
        need sudo
        sudo $argv
    end
end
function run_maybe_sudo --argument-names dir
    if test -w $dir
        run $argv[2..-1]
    else
        run_sudo $argv[2..-1]
    end
end

# ─── Checks ─────────────────────────────────────────────────────────────────
function validate_settings
    contains -- $BUILD_TYPE Release Debug RelWithDebInfo MinSizeRel
    or die "Invalid build type '$BUILD_TYPE' (Release, Debug, RelWithDebInfo, MinSizeRel)"
    contains -- $OPENSSL static system
    or die "Invalid OPENSSL '$OPENSSL' (static or system)"
    string match -qr '^[0-9]+$' -- $JOBS
    or die "Invalid job count '$JOBS'"
end

function check_prerequisites
    need cmake
    if not command -q qmake6; and not command -q qmake; and test -z "$CMAKE_PREFIX_PATH"
        warn "Qt6 may not be installed (no qmake6/qmake in PATH)."
        if test "$OS" = Darwin
            warn "  Install: brew install qt@6"
        else
            warn "  Install: sudo apt-get install qt6-base-dev qt6-tools-dev"
        end
    end
    if not command -q lupdate; and not command -q lupdate-qt6; and not test -x /usr/lib/qt6/bin/lupdate
        warn "Qt6 LinguistTools (lupdate/lrelease) not found. Install:"
        warn "  Ubuntu/Debian: sudo apt-get install qt6-tools-dev-tools"
        warn "  Fedora/RHEL:   sudo dnf install qt6-linguist"
        warn "  Arch:          sudo pacman -S qt6-tools"
        warn "  openSUSE:      sudo zypper install qt6-linguist-devel"
        warn "  macOS:         brew install qt@6"
    end
end

function macos_prefix_path
    if test "$OS" = Darwin; and test -z "$CMAKE_PREFIX_PATH"; and command -q brew
        set -l qt (brew --prefix qt@6 2>/dev/null)
        if test -n "$qt"; and test -d "$qt"
            set -gx CMAKE_PREFIX_PATH $qt
        end
    end
end

function init_submodules
    set -l paths
    if test "$OPENSSL" = static; and not test -f external/openssl/Configure
        set -a paths external/openssl
    end
    if not test -f proto/director_service.proto
        set -a paths proto
    end
    if test (count $paths) -gt 0
        info "Initialising submodules: $paths"
        run git submodule update --init --recursive $paths
        or die "git submodule update failed"
    end
end

function app_bin
    if test -x $BUILD_DIR/Onesimus.app/Contents/MacOS/Onesimus
        echo $BUILD_DIR/Onesimus.app/Contents/MacOS/Onesimus
    else
        echo $BUILD_DIR/onesimus
    end
end

function stamped_version
    sed -n 's/^#define VERSION_FULL *"\(.*\)"/\1/p' include/version.h 2>/dev/null | head -1
end

function static_flag
    if test "$OPENSSL" = static
        echo ON
    else
        echo OFF
    end
end

# ─── Development loop ───────────────────────────────────────────────────────
function cmd_configure
    adopt_tree_settings
    validate_settings
    check_prerequisites
    macos_prefix_path
    init_submodules
    set -l gen
    if not test -f $BUILD_DIR/CMakeCache.txt; and test -n "$GENERATOR"
        set gen -G $GENERATOR
    end
    set -l pp
    test -n "$CMAKE_PREFIX_PATH"; and set pp -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH
    # OPENSSL_INSTALL_DIR: reusable static OpenSSL prefix (see deploy.sh).
    test -n "$OPENSSL_INSTALL_DIR"; and set -a pp -DOPENSSL_INSTALL_DIR=$OPENSSL_INSTALL_DIR
    set -l g ""
    test -n "$GENERATOR"; and set g ", $GENERATOR"
    info "Configuring $BUILD_DIR ($BUILD_TYPE, OpenSSL $OPENSSL$g)"
    run cmake -S $SCRIPT_DIR -B $BUILD_DIR $gen \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DUSE_STATIC_OPENSSL=(static_flag) \
        $pp $argv
    or die "configure failed"
end

function adopt_tree_settings
    set -l cache $BUILD_DIR/CMakeCache.txt
    test -f $cache; or return 0
    if test "$BUILD_TYPE_SET" != 1
        set -l v (sed -n 's/^CMAKE_BUILD_TYPE:[A-Z]*=//p' $cache | head -1)
        test -n "$v"; and set -g BUILD_TYPE $v
    end
    if test "$OPENSSL_SET" != 1
        set -l v (sed -n 's/^USE_STATIC_OPENSSL:[A-Z]*=//p' $cache | head -1)
        switch "$v"
            case ON on TRUE 1
                set -g OPENSSL static
            case OFF off FALSE 0
                set -g OPENSSL system
        end
    end
    return 0
end

function ensure_configured
    if test "$CLEAN" = 1; and test -d $BUILD_DIR
        info "Removing $BUILD_DIR (--clean)"
        run rm -rf $BUILD_DIR
        set -g CLEAN 0
    end
    if not test -f $BUILD_DIR/CMakeCache.txt
        cmd_configure
    else
        adopt_tree_settings
        validate_settings
        set -l args
        test "$BUILD_TYPE_SET" = 1; and set -a args -DCMAKE_BUILD_TYPE=$BUILD_TYPE
        if test "$OPENSSL_SET" = 1
            set -a args -DUSE_STATIC_OPENSSL=(static_flag)
            test "$OPENSSL" = static; and init_submodules
        end
        if test (count $args) -gt 0
            info "Re-configuring $BUILD_DIR: $args"
            run cmake -S $SCRIPT_DIR -B $BUILD_DIR $args >/dev/null
            or die "re-configure failed"
        end
    end
end

function cmd_build
    need cmake
    ensure_configured
    if test "$OPENSSL" = static; and not test -d (test -n "$OPENSSL_INSTALL_DIR"; and echo $OPENSSL_INSTALL_DIR; or echo $BUILD_DIR/openssl-install)/include/openssl
        info "NOTE: OpenSSL is compiled on the first build - this may take 5-10 minutes."
    end
    info "Building onesimus ($JOBS jobs)"
    run cmake --build $BUILD_DIR -j $JOBS
    or die "build failed"
    if test "$DRY_RUN" != 1
        ok "Built "(app_bin)"  (version "(stamped_version)")"
        test "$OPENSSL" = static
        and ok "  OpenSSL statically linked - no external OpenSSL libraries required"
    end
    return 0
end

function cmd_rebuild
    need cmake
    ensure_configured
    info "Clean rebuild ($JOBS jobs)"
    run cmake --build $BUILD_DIR --clean-first -j $JOBS
    or die "rebuild failed"
end

function cmd_run
    cmd_build
    set -l bin (app_bin)
    if test "$DRY_RUN" = 1
        run $bin $argv
        return 0
    end
    test -x $bin; or die "binary not found: $bin"
    info "Running "(basename $bin)" $argv"
    exec $bin $argv
end

function cmd_test
    cmd_build
    need ctest
    info "Running tests ($JOBS jobs, offscreen)"
    run env QT_QPA_PLATFORM=offscreen ctest --test-dir $BUILD_DIR -j $JOBS --output-on-failure
    or die "tests failed"
end

# ─── Deploy: build + copy into the system paths (see deploy.sh) ─────────────
# `install` replaces a symlink instead of writing through it (see deploy.sh).
function warn_symlink --argument-names f
    if test -L $f
        set -l target (readlink $f)
        warn "  $f is a symlink to $target - it will be replaced by the binary."
        warn "  To update the link target instead: DEPLOY_PREFIX="(dirname (dirname $target))" ./deploy.fish"
    end
    return 0
end

function deploy_onesimus
    info "=== onesimus ==="
    cmd_build
    set -l bin (app_bin)
    if test "$DRY_RUN" != 1
        test -x $bin; or die "binary not found: $bin"
    end

    if test "$OS" = Darwin
        # The binary of the build tree's Onesimus.app (a development install);
        # the self-contained Onesimus.app comes in the .dmg of `package`.
        info "  onesimus → $DEPLOY_PREFIX/bin/onesimus"
        run_maybe_sudo $DEPLOY_PREFIX/bin install -m 755 $bin $DEPLOY_PREFIX/bin/onesimus
        test "$DRY_RUN" = 1; or ok "onesimus deployed (version "(stamped_version)")."
        return 0
    end

    set -l share $DEPLOY_PREFIX/share
    warn_symlink $DEPLOY_PREFIX/bin/onesimus
    info "  onesimus → $DEPLOY_PREFIX/bin/onesimus"
    run_sudo install -d $DEPLOY_PREFIX/bin
    run_sudo install -m 755 $bin $DEPLOY_PREFIX/bin/onesimus

    info "  desktop entry → $share/applications/onesimus.desktop"
    run_sudo install -d $share/applications
    run_sudo install -m 644 $BUILD_DIR/onesimus.desktop $share/applications/onesimus.desktop

    info "  hicolor icons → $share/icons/hicolor/"
    run_sudo install -d $share/icons/hicolor/scalable/apps
    run_sudo install -m 644 resources/icons/onesimus.svg $share/icons/hicolor/scalable/apps/onesimus.svg
    for png in $BUILD_DIR/icons/*x*/onesimus.png
        test -f $png; or continue
        set -l size (basename (dirname $png))
        run_sudo install -d $share/icons/hicolor/$size/apps
        run_sudo install -m 644 $png $share/icons/hicolor/$size/apps/onesimus.png
    end

    if command -q gtk-update-icon-cache
        run_sudo gtk-update-icon-cache -q -f $share/icons/hicolor; or true
    end
    if command -q update-desktop-database
        run_sudo update-desktop-database -q $share/applications; or true
    end
    test "$DRY_RUN" = 1; or ok "onesimus deployed to $DEPLOY_PREFIX (version "(stamped_version)")."
end

function deploy_all
    info "=========================================="
    info "  Onesimus deploy (direct copy)"
    info "=========================================="
    test "$DRY_RUN" = 1; and warn "DRY RUN - commands are printed, not executed."
    deploy_onesimus
    echo ""
    ok "=========================================="
    if test "$DRY_RUN" = 1
        ok "  Dry run finished - nothing was changed."
    else
        ok "  All deployed."
    end
    ok "=========================================="
end

# ─── install / package / appimage ───────────────────────────────────────────
function cmd_install
    cmd_build
    info "Installing to $PREFIX (cmake --install: binary, desktop entry, icons)"
    run_maybe_sudo $PREFIX cmake --install $BUILD_DIR --prefix $PREFIX
    or die "install failed"
    if test "$OS" != Darwin
        command -q gtk-update-icon-cache
        and run_maybe_sudo $PREFIX gtk-update-icon-cache -q -f $PREFIX/share/icons/hicolor 2>/dev/null
        command -q update-desktop-database
        and run_maybe_sudo $PREFIX update-desktop-database -q $PREFIX/share/applications 2>/dev/null
    end
    ok "installed to $PREFIX."
end

function cmd_package
    # macOS: the .dmg comes from scripts/ci/macos-package.sh (see deploy.sh)
    if test "$OS" = Darwin; and test -z "$PACKAGE_GENERATORS"
        cmd_build
        info "Packaging (scripts/ci/macos-package.sh)"
        run scripts/ci/macos-package.sh $BUILD_DIR
        or die "macos-package.sh failed"
        return 0
    end
    need cpack
    cmd_build
    run rm -rf $BUILD_DIR/_CPack_Packages
    set -l gen
    set -q PACKAGE_GENERATORS; and test -n "$PACKAGE_GENERATORS"; and set gen -G $PACKAGE_GENERATORS
    info "Packaging (cpack $gen)"
    run cpack $gen --config $BUILD_DIR/CPackConfig.cmake -B $BUILD_DIR
    or die "cpack failed"
    test "$DRY_RUN" = 1; and return 0
    info "Produced packages:"
    set -l found 0
    for f in $BUILD_DIR/*.deb $BUILD_DIR/*.rpm $BUILD_DIR/*.dmg
        test -f $f; or continue
        set found 1
        echo "    "(du -h $f | cut -f1)"  $f"
    end
    test $found = 1; or warn "no package found in $BUILD_DIR"
    set -l debs $BUILD_DIR/*.deb
    if test (count $debs) -gt 0
        echo "Install: sudo apt install "(realpath (ls -1t $debs | head -1))
    end
end

# Same pins as deploy.sh (and forgeiec-studio). Change tag AND checksum together.
set -g LINUXDEPLOY_TAG 1-alpha-20250213-2
set -g LINUXDEPLOY_SHA256 4648f278ab3ef31f819e67c30d50f462640e5365a77637d7e6f2ad9fd0b4522a
set -g LINUXDEPLOY_QT_TAG 1-alpha-20250213-1
set -g LINUXDEPLOY_QT_SHA256 15106be885c1c48a021198e7e1e9a48ce9d02a86dd0a1848f00bdbf3c1c92724
if not set -q APPIMAGE_TOOLS; or test -z "$APPIMAGE_TOOLS"
    set -l cache $XDG_CACHE_HOME
    test -n "$cache"; or set cache $HOME/.cache
    set -g APPIMAGE_TOOLS $cache/onesimus/appimage-tools
end

function fetch_appimage_tool --argument-names name url sum
    set -l file $APPIMAGE_TOOLS/$name
    if test -f $file; and echo "$sum  $file" | sha256sum -c --status 2>/dev/null
        chmod +x $file
        return 0
    end
    info "  downloading $name"
    run mkdir -p $APPIMAGE_TOOLS
    if test "$DRY_RUN" = 1
        run curl -fsSL -o $file $url
        return 0
    end
    curl -fsSL -o $file.part $url; or begin; rm -f $file.part; die "download failed: $url"; end
    echo "$sum  $file.part" | sha256sum -c --status
    or begin; rm -f $file.part; die "SHA-256 mismatch: $name"; end
    mv $file.part $file
    chmod +x $file
end

function cmd_appimage
    test "$OS" = Linux; or die "appimage: Linux only"
    need cmake; need curl; need sha256sum
    validate_settings
    set -l qmake (command -s qmake6; or command -s qmake)
    test -n "$qmake"; or die "missing tool: qmake6"
    set -l arch (uname -m)
    test "$arch" = x86_64; or die "appimage: only x86_64 is prepared (here: $arch)"
    # Absolute: linuxdeploy runs inside dist/appimage, a relative AppDir
    # (the default BUILD_DIR=build) would point nowhere there.
    set -l base $BUILD_DIR
    string match -q '/*' -- $base; or set base $SCRIPT_DIR/$base
    set -l dir $base-appimage
    set -l appdir $base-appimage/AppDir
    set -l out $SCRIPT_DIR/dist/appimage
    init_submodules

    run rm -rf $appdir
    set -l gen
    if not test -f $dir/CMakeCache.txt; and test -n "$GENERATOR"
        set gen -G $GENERATOR
    end
    set -l ossl
    test -n "$OPENSSL_INSTALL_DIR"; and set ossl -DOPENSSL_INSTALL_DIR=$OPENSSL_INSTALL_DIR
    info "Building onesimus for AppImage (Release, PREFIX=/usr, OpenSSL $OPENSSL)"
    run cmake -S $SCRIPT_DIR -B $dir $gen -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr -DUSE_STATIC_OPENSSL=(static_flag) -DBUILD_TESTING=OFF \
        $ossl
    or die "appimage configure failed"
    run cmake --build $dir -j $JOBS; or die "appimage build failed"
    run env DESTDIR=$appdir cmake --install $dir; or die "appimage install failed"

    fetch_appimage_tool linuxdeploy-x86_64.AppImage \
        https://github.com/linuxdeploy/linuxdeploy/releases/download/$LINUXDEPLOY_TAG/linuxdeploy-x86_64.AppImage \
        $LINUXDEPLOY_SHA256
    fetch_appimage_tool linuxdeploy-plugin-qt-x86_64.AppImage \
        https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/$LINUXDEPLOY_QT_TAG/linuxdeploy-plugin-qt-x86_64.AppImage \
        $LINUXDEPLOY_QT_SHA256

    set -l version (stamped_version)
    test -n "$version"; or set version 0.0.0
    run mkdir -p $out
    info "linuxdeploy (Qt plugin, version $version)"
    set -l ld_args --appdir $appdir --plugin qt \
        --executable $appdir/usr/bin/onesimus \
        --desktop-file $appdir/usr/share/applications/onesimus.desktop \
        --icon-file $appdir/usr/share/icons/hicolor/scalable/apps/onesimus.svg
    if test "$DRY_RUN" = 1
        run linuxdeploy-x86_64.AppImage $ld_args
        run $SCRIPT_DIR/scripts/ci/appimage-notices.sh $appdir
        run linuxdeploy-x86_64.AppImage --appdir $appdir --output appimage
        return 0
    end
    # Two passes as in deploy.sh: bundle, add the notices of the bundled
    # libraries (scripts/ci/appimage-notices.sh), then write the AppImage.
    pushd $out
    env ARCH=x86_64 QMAKE=$qmake APPIMAGE_EXTRACT_AND_RUN=1 \
        EXTRA_PLATFORM_PLUGINS="libqwayland-egl.so;libqwayland-generic.so;libqoffscreen.so" \
        PATH="$APPIMAGE_TOOLS:$PATH" \
        $APPIMAGE_TOOLS/linuxdeploy-x86_64.AppImage $ld_args
    set -l rc $status
    if test $rc -eq 0
        $SCRIPT_DIR/scripts/ci/appimage-notices.sh $appdir
        and $SCRIPT_DIR/scripts/ci/check-notices.sh $appdir
        set rc $status
    end
    if test $rc -eq 0
        env ARCH=x86_64 APPIMAGE_EXTRACT_AND_RUN=1 \
            LINUXDEPLOY_OUTPUT_VERSION=$version \
            PATH="$APPIMAGE_TOOLS:$PATH" \
            $APPIMAGE_TOOLS/linuxdeploy-x86_64.AppImage --appdir $appdir --output appimage
        set rc $status
    end
    popd
    test $rc -eq 0; or die "linuxdeploy failed"
    set -l img (ls -1t $out/*.AppImage 2>/dev/null | head -1)
    test -f "$img"; or die "no AppImage in $out"
    ok "AppImage: $img ("(du -h $img | cut -f1)")"
    echo "  sha256: "(sha256sum $img | cut -d' ' -f1)
end

function cmd_clean
    info "Removing $BUILD_DIR $BUILD_DIR-appimage"
    run rm -rf $BUILD_DIR $BUILD_DIR-appimage
end

function show_status
    echo ""
    info "Deployed:"
    set -l inst $DEPLOY_PREFIX/bin/onesimus
    if test -x $inst
        ok "  $inst  ("(date -r $inst '+%Y-%m-%d %H:%M' 2>/dev/null; or echo '?')")"
    else
        warn "  $inst not present"
    end
    info "Build tree $BUILD_DIR:"
    set -l bin (app_bin)
    if test -x $bin
        ok "  $bin  ("(date -r $bin '+%Y-%m-%d %H:%M' 2>/dev/null; or echo '?')")"
        if test -f $BUILD_DIR/CMakeCache.txt
            set -l bt (grep -E '^CMAKE_BUILD_TYPE:' $BUILD_DIR/CMakeCache.txt | cut -d= -f2)
            set -l so (grep -E '^USE_STATIC_OPENSSL:' $BUILD_DIR/CMakeCache.txt | cut -d= -f2)
            ok "  $bt, OpenSSL static: $so"
        end
    else
        warn "  no binary (./deploy.fish build)"
    end
    info "Stamped version (include/version.h): "(stamped_version)
    for f in $BUILD_DIR/*.deb $BUILD_DIR/*.rpm $BUILD_DIR/*.dmg dist/appimage/*.AppImage
        test -f $f; and info "Package: $f"
    end
    return 0
end

# The common verb table - the same as in deploy.sh, checked mechanically:
#
#   diff <(bash deploy.sh help   | sed -E 's/deploy\.(sh|fish)/deploy.X/g') \
#        <(fish deploy.fish help | sed -E 's/deploy\.(sh|fish)/deploy.X/g')
function show_help
    echo '
Usage:  ./deploy.fish [flags] [command] [args]
        ./deploy.sh [flags] [command] [args]   - the same commands

Deploy (build + copy into DEPLOY_PREFIX with sudo per step):
  all | onesimus | gui   Build, then copy binary, desktop entry and icons
                         to /usr (Linux) or /usr/local/bin (macOS) - default
  status                 What is deployed, built and packaged

Development loop (touches the system only with install):
  configure [args]       Create the build tree (extra args go to cmake)
  build                  Build onesimus only
  rebuild                Clean rebuild (--clean-first)
  run [args]             Build and start onesimus
  test                   Build and run ctest (offscreen)
  install                cmake --install into PREFIX (sudo if not writable)
  package                .deb on Linux (CPack), .dmg on macOS (macos-package.sh)
  appimage               AppImage -> dist/appimage/ (Linux x86_64)
  clean                  Remove the build trees
  help                   This table

Flags (same as the former build.sh; before or after the command, but
       everything after run/configure goes to onesimus/cmake):
  -t, --type TYPE        Release (default), Debug, RelWithDebInfo, MinSizeRel
  -s, --system-openssl   System OpenSSL instead of the static submodule
  -c, --clean            Remove the build directory before building
  -j, --jobs N           Parallel jobs (default: number of CPUs)
  -d, --build-dir DIR    Build directory (default: build)
  -n, --dry-run          Print every command instead of running it
  -h, --help             This table

Env: BUILD_DIR, BUILD_TYPE, JOBS, OPENSSL (static|system), GENERATOR, PREFIX,
     DEPLOY_PREFIX, CMAKE_PREFIX_PATH, PACKAGE_GENERATORS, APPIMAGE_TOOLS,
     OPENSSL_INSTALL_DIR, CLEAN, DRY_RUN, ONESIMUS_ALLOW_ROOT

Every build increments .build_number and regenerates include/version.h.

Examples:
  ./deploy.fish                     # build + deploy to /usr
  ./deploy.fish -n                  # show what a deploy would run
  ./deploy.fish build -t Debug      # Debug build, no deploy
  ./deploy.fish build -s -c         # system OpenSSL, clean rebuild
  ./deploy.fish package             # .deb / .dmg'
end

# ─── Main ───────────────────────────────────────────────────────────────────
set -l cmd all
test (count $positional) -gt 0; and set cmd $positional[1]
set -l rest $positional[2..-1]
switch $cmd
    case all onesimus gui
        deploy_all
    case configure
        cmd_configure $rest
    case build
        cmd_build
    case rebuild
        cmd_rebuild
    case run
        cmd_run $rest
    case test
        cmd_test
    case install
        cmd_install
    case package
        cmd_package
    case appimage
        cmd_appimage
    case clean
        cmd_clean
    case status
        show_status
    case help
        show_help
    case '*'
        err "Unknown command: $cmd"
        show_help
        exit 1
end
