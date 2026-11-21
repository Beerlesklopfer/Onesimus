#Requires -Version 5.1
<#
.SYNOPSIS
    Onesimus deploy script for Windows (counterpart of ./deploy.sh).

.DESCRIPTION
    Builds Onesimus.exe with CMake + MSVC and makes it runnable: windeployqt
    and the OpenSSL DLLs go next to the executable in build\deploy (the CMake
    target "deploy"). Like deploy.ps1 of forgeiec-studio there is no system
    install by default on Windows: "deploy" means build + make runnable; the
    installers (MSI via WiX, portable ZIP) are separate verbs.

    The build stamps the version: CMake's increment_build_number target
    increments .build_number and regenerates include\version.h on every build.

    Environment: OPENSSL_INSTALL_DIR names a prefix for the static OpenSSL
    build that is reused when it already holds the libraries (CI cache).

.PARAMETER Command
    all        Build + windeployqt into build\deploy (default).
    onesimus   Alias of all (also: gui).
    configure  CMake configure only.
    build      Build only (configures when needed).
    rebuild    Clean rebuild (-Clean + build).
    deploy     windeployqt + OpenSSL DLLs into build\deploy only.
    run        Build + deploy, then start build\deploy\Onesimus.exe.
    test       Build + ctest.
    installer  MSI installer via package-msi.ps1 (WiX v6). Aliases: msi, package.
    zip        Portable ZIP via package-windows.ps1.
    msi        Same as installer.
    install    cmake --install into -Prefix (default build\stage).
    status     Built binary, version, installers.
    clean      Remove the build directory.
    help       This help.

.PARAMETER Clean
    Remove the build directory before building (as in the former build-windows.ps1).

.PARAMETER QtPath
    Qt prefix (folder with bin\qmake.exe). Default: newest C:\Qt\<ver>\msvc*64 or C:\Qt6\...

.PARAMETER BuildType
    Release (default), Debug, RelWithDebInfo, MinSizeRel.

.PARAMETER BackupSystem
    Passed to CMake as BACKUP_SYSTEM. Only BAREOS builds today (CMake rejects BACULA/BOTH).

.PARAMETER Generator
    Ninja (default) or NMake.

.PARAMETER BuildDir
    Build directory. Default: build

.PARAMETER Jobs
    Parallel build jobs. Default: the generator's own default.

.PARAMETER Prefix
    Target of "install". Default: <BuildDir>\stage

.PARAMETER Reconfigure
    Delete CMakeCache.txt and configure afresh.

.PARAMETER Version
    Version for "zip". Default: VERSION_FULL from include\version.h.

.PARAMETER DirdZip
    For "installer": onesimus-dird-<version>-windows-x86_64.zip with
    bin\oconsole.exe and bin\ofd.exe. Default: newest one in dist\rust\.
    Without it the MSI holds the GUI only.

.PARAMETER DirdDir
    For "installer": an unpacked onesimus-dird folder (with bin\) instead of -DirdZip.

.PARAMETER DryRun
    Print every command instead of running it.

.EXAMPLE
    .\deploy.ps1
    Build + make runnable (Release).

.EXAMPLE
    .\deploy.ps1 build -BuildType Debug -Clean

.EXAMPLE
    .\deploy.ps1 msi

.EXAMPLE
    .\deploy.ps1 -DryRun all
#>
[CmdletBinding()]
param(
    [Parameter(Position = 0)]
    [ValidateSet('all', 'onesimus', 'gui', 'configure', 'build', 'rebuild', 'deploy',
                 'run', 'test', 'installer', 'package', 'zip', 'msi', 'install',
                 'status', 'clean', 'help')]
    [string]$Command = 'all',

    [switch]$Clean,
    [string]$QtPath,
    [ValidateSet('Release', 'Debug', 'RelWithDebInfo', 'MinSizeRel')]
    [string]$BuildType = 'Release',
    [ValidateSet('BACULA', 'BAREOS', 'BOTH')]
    [string]$BackupSystem = 'BAREOS',
    [ValidateSet('Ninja', 'NMake')]
    [string]$Generator = 'Ninja',
    [string]$BuildDir = 'build',
    [int]$Jobs = 0,
    [string]$Prefix,
    [switch]$Reconfigure,
    [string]$Version,
    [string]$DirdZip,
    [string]$DirdDir,
    [switch]$DryRun
)

# Not 'Stop': Windows PowerShell 5.1 wraps stderr lines of native tools (cmake
# writes warnings there) into ErrorRecords. The real gate is the explicit
# $LASTEXITCODE check in Invoke-Checked.
$ErrorActionPreference = 'Continue'

$ScriptDir = $PSScriptRoot
Set-Location $ScriptDir
if (-not [System.IO.Path]::IsPathRooted($BuildDir)) { $BuildDir = Join-Path $ScriptDir $BuildDir }
$DeployDir = Join-Path $BuildDir 'deploy'
$ExeName   = 'onesimus.exe'

# --- Messages ---------------------------------------------------------------
function Write-Info($msg)    { Write-Host "[deploy] $msg" -ForegroundColor Cyan }
function Write-Ok($msg)      { Write-Host "[deploy] $msg" -ForegroundColor Green }
function Write-Warn($msg)    { Write-Host "[deploy] $msg" -ForegroundColor Yellow }
function Write-Err($msg)     { Write-Host "[deploy] $msg" -ForegroundColor Red }
function Write-Section($msg) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Magenta
    Write-Host $msg -ForegroundColor Magenta
    Write-Host "========================================" -ForegroundColor Magenta
}

# Run a native command and stop on a non-zero exit code. With -DryRun the
# command is printed instead, so a dry run shows exactly what would run.
function Invoke-Checked {
    param([string]$What, [string]$Exe, [string[]]$Arguments)
    if ($DryRun) {
        Write-Host "  + $Exe $($Arguments -join ' ')"
        return
    }
    & $Exe @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "$What failed (exit code $LASTEXITCODE)"
    }
}

function Remove-Tree($path) {
    if ($DryRun) { Write-Host "  + Remove-Item -Recurse -Force $path"; return }
    Remove-Item -Path $path -Recurse -Force
}

function Get-StampedVersion {
    $h = Join-Path $ScriptDir 'include\version.h'
    if (Test-Path $h) {
        $m = Select-String -Path $h -Pattern '^#define VERSION_FULL\s+"([^"]+)"' | Select-Object -First 1
        if ($m) { return $m.Matches[0].Groups[1].Value }
    }
    return '0.0.0'
}

# --- 0. Visual Studio environment -------------------------------------------
function Initialize-VsEnvironment {
    Write-Section "Visual Studio environment"
    if ($env:VSINSTALLDIR) {
        Write-Ok "Visual Studio environment already loaded ($env:VSINSTALLDIR)"
        return
    }
    if ($DryRun) { Write-Host "  + Enter-VsDevShell (x64)"; return }

    # A local helper script wins (as before); otherwise find VS with vswhere.
    $helper = Join-Path $ScriptDir 'Import-VsDevShell.ps1'
    try {
        if (Test-Path $helper) {
            Write-Info "Loading Visual Studio environment via Import-VsDevShell.ps1 ..."
            & $helper -Architecture x64
        } else {
            $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
            if (-not (Test-Path $vswhere)) { throw "vswhere.exe not found ($vswhere)" }
            $vs = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
            if (-not $vs) { throw "no Visual Studio with C++ tools found" }
            $dll = Join-Path $vs 'Common7\Tools\Microsoft.VisualStudio.DevShell.dll'
            Write-Info "Loading Visual Studio environment from $vs ..."
            Import-Module $dll
            Enter-VsDevShell -VsInstallPath $vs -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64' | Out-Null
        }
        # LASTEXITCODE is unreliable here (Enter-VsDevShell runs vswhere itself).
        if (-not $env:VSINSTALLDIR) { throw "the Visual Studio environment was not set" }
        Write-Ok "Visual Studio environment loaded"
    } catch {
        Write-Err "Could not load the Visual Studio environment: $($_.Exception.Message)"
        Write-Warn "Alternatives:"
        Write-Warn "  1. Open 'Developer PowerShell for VS 2022' and run the script there"
        Write-Warn "  2. Install Visual Studio 2022 with 'Desktop development with C++'"
        exit 1
    }
}

# --- 1. Git submodules ------------------------------------------------------
# external/openssl (Windows always links OpenSSL statically) and proto/ (gRPC).
function Initialize-Submodules {
    Write-Section "Git submodules"
    $paths = @()
    if (-not (Test-Path (Join-Path $ScriptDir 'external\openssl\Configure'))) { $paths += 'external/openssl' }
    if (-not (Test-Path (Join-Path $ScriptDir 'proto\director_service.proto'))) { $paths += 'proto' }
    if ($paths.Count -eq 0) { Write-Ok "Submodules present"; return }
    Write-Info "Initialising: $($paths -join ', ')"
    try {
        Invoke-Checked 'git submodule update' 'git' (@('submodule', 'update', '--init', '--recursive') + $paths)
    } catch {
        Write-Err "Could not initialise the submodules. Run manually:"
        Write-Err "  git submodule update --init --recursive $($paths -join ' ')"
        exit 1
    }
}

# --- 2. Prerequisites -------------------------------------------------------
function Test-Prerequisites {
    Write-Section "Prerequisites"
    foreach ($tool in @('git', 'cmake')) {
        if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) { Write-Err "$tool not found"; exit 1 }
    }
    Write-Ok "Git:   $((git --version) -replace 'git version ', '')"
    Write-Ok "CMake: $((cmake --version | Select-Object -First 1) -replace 'cmake version ', '')"
    if (Get-Command perl -ErrorAction SilentlyContinue) {
        Write-Ok "Perl:  available (needed for the static OpenSSL build)"
    } else {
        Write-Err "Perl not found - install Strawberry Perl: https://strawberryperl.com/"
        exit 1
    }
    if ($Generator -eq 'Ninja') {
        if (-not (Get-Command ninja -ErrorAction SilentlyContinue)) { Write-Err "Ninja not found (or use -Generator NMake)"; exit 1 }
        Write-Ok "Ninja: available"
    } else {
        if (-not (Get-Command nmake -ErrorAction SilentlyContinue)) {
            Write-Err "NMake not found - Visual Studio 2022 (or Build Tools) with C++ tools is required"
            exit 1
        }
        Write-Ok "NMake: available"
    }
    if (Get-Command cl -ErrorAction SilentlyContinue) {
        $clVersion = (cl 2>&1 | Select-Object -First 1).ToString()
        if ($clVersion -match "Version\s+([\d\.]+)") { Write-Ok "MSVC:  $($matches[1])" } else { Write-Ok "MSVC:  available" }
    } else {
        Write-Warn "C++ compiler (cl.exe) not found"
    }
}

# --- 3. Qt ------------------------------------------------------------------
function Find-Qt {
    Write-Section "Qt6"
    if ($QtPath -and (Test-Path "$QtPath\bin\qmake.exe")) {
        Write-Ok "Qt (parameter): $QtPath"
        $found = $QtPath
    } else {
        $installs = @()
        foreach ($root in @('C:\Qt', 'C:\Qt6')) {
            if (-not (Test-Path $root)) { continue }
            Get-ChildItem -Path $root -Directory -ErrorAction SilentlyContinue |
                Where-Object { $_.Name -match '^\d+\.\d+(\.\d+)?$' } | ForEach-Object {
                    Get-ChildItem -Path $_.FullName -Directory -ErrorAction SilentlyContinue |
                        Where-Object { $_.Name -like 'msvc*64' -and $_.Name -notlike '*arm*' } | ForEach-Object {
                            $qmake = Join-Path $_.FullName 'bin\qmake.exe'
                            if ((Test-Path $qmake) -and (Test-Path (Join-Path $_.FullName 'lib\cmake\Qt6\Qt6Config.cmake'))) {
                                $installs += @{ Path = $_.FullName; Version = (& $qmake -query QT_VERSION); Compiler = $_.Name }
                            }
                        }
                }
        }
        if ($installs.Count -gt 0) {
            $sel = $installs | Sort-Object { [version]$_.Version } -Descending | Select-Object -First 1
            $found = $sel.Path
            Write-Ok "Qt (automatic): $found  (version $($sel.Version), $($sel.Compiler))"
        }
    }
    if (-not $found) {
        if ($DryRun) { Write-Warn "Qt6 not found - dry run continues with C:\Qt\<version>\msvc2022_64"; return 'C:\Qt\<version>\msvc2022_64' }
        Write-Err "Qt6 was not found automatically."
        Write-Warn "  1. Diagnose: .\check-qt6.ps1"
        Write-Warn "  2. Install Qt6: https://www.qt.io/download-qt-installer"
        $manual = Read-Host "Or enter the Qt path (e.g. C:\Qt\6.8.0\msvc2022_64)"
        if (Test-Path "$manual\bin\qmake.exe") { $found = $manual } else { Write-Err "Qt not found in: $manual"; exit 1 }
    }
    if (-not (Test-Path "$found\lib\cmake\Qt6\Qt6Config.cmake")) {
        Write-Err "Qt6Config.cmake missing: $found\lib\cmake\Qt6\Qt6Config.cmake"
        Write-Warn "  Diagnose with .\check-qt6.ps1, or add the modules with the Qt Maintenance Tool."
        exit 1
    }
    Write-Info "Qt version: $(& "$found\bin\qmake.exe" -query QT_VERSION)"
    return $found
}

# --- 4./5. Configure --------------------------------------------------------
function Invoke-Configure {
    Initialize-VsEnvironment
    Initialize-Submodules
    Test-Prerequisites
    $qt = Find-Qt

    Write-Section "Build directory"
    if ((Test-Path $BuildDir) -and $Clean) {
        Write-Info "Removing $BuildDir (-Clean)"
        Remove-Tree $BuildDir
    }
    $cache = Join-Path $BuildDir 'CMakeCache.txt'
    if ($Reconfigure -and (Test-Path $cache)) {
        Write-Info "Reconfigure: removing $cache"
        if (-not $DryRun) { Remove-Item $cache -Force }
    }

    Write-Section "CMake configuration"
    $genName = if ($Generator -eq 'Ninja') { 'Ninja' } else { 'NMake Makefiles' }
    Write-Info "Generator: $genName, Qt: $qt, build type: $BuildType, backup system: $BackupSystem"
    Write-Info "OpenSSL: static (built from external/openssl)"
    $cmakeArgs = @('-S', $ScriptDir, '-B', $BuildDir)
    # CMake refuses -G on a tree configured with another generator.
    if ($DryRun -or -not (Test-Path (Join-Path $BuildDir 'CMakeCache.txt'))) { $cmakeArgs += @('-G', $genName) }
    $cmakeArgs += @("-DCMAKE_PREFIX_PATH=$qt", "-DCMAKE_BUILD_TYPE=$BuildType",
                    '-DUSE_STATIC_OPENSSL=ON', "-DBACKUP_SYSTEM=$BackupSystem")
    # Reusable OpenSSL prefix (CI cache), as OPENSSL_INSTALL_DIR in deploy.sh.
    if ($env:OPENSSL_INSTALL_DIR) { $cmakeArgs += "-DOPENSSL_INSTALL_DIR=$env:OPENSSL_INSTALL_DIR" }
    Invoke-Checked 'cmake configure' 'cmake' $cmakeArgs
}

function Confirm-Configured {
    if ($Clean -or $Reconfigure -or $DryRun -or -not (Test-Path (Join-Path $BuildDir 'CMakeCache.txt'))) {
        Invoke-Configure
        # -Clean applies once per run.
        $script:Clean = $false
        $script:Reconfigure = $false
    } else {
        Initialize-VsEnvironment
    }
}

# --- 6. Build ---------------------------------------------------------------
function Invoke-Build {
    Confirm-Configured
    Write-Section "Compilation"
    if (-not (Test-Path (Join-Path $BuildDir 'openssl-install'))) {
        Write-Warn "NOTE: the first build downloads nothing but compiles OpenSSL - this takes 5-10 minutes."
    }
    $start = Get-Date
    $buildArgs = @('--build', $BuildDir, '--config', $BuildType)
    if ($Jobs -gt 0) { $buildArgs += @('--parallel', "$Jobs") }
    Invoke-Checked 'build' 'cmake' $buildArgs
    if (-not $DryRun) {
        $d = (Get-Date) - $start
        Write-Ok "Build finished in $($d.ToString('mm\:ss')) min, version $(Get-StampedVersion)"
    }
}

function Invoke-Rebuild {
    $script:Clean = $true
    Invoke-Build
}

# windeployqt + OpenSSL DLLs + compiler runtime into build\deploy (CMake target).
function Invoke-DeployDlls {
    Write-Section "Deploy (windeployqt)"
    Invoke-Checked 'windeployqt' 'cmake' @('--build', $BuildDir, '--config', $BuildType, '--target', 'deploy')
    if (-not $DryRun) {
        $exe = Join-Path $DeployDir $ExeName
        if (-not (Test-Path $exe)) { throw "$exe missing after the deploy target (windeployqt not found by CMake?)" }
        $dlls = (Get-ChildItem $DeployDir -Filter '*.dll' -ErrorAction SilentlyContinue | Measure-Object).Count
        Write-Ok "$exe runnable ($dlls DLLs next to it)"
    }
}

function Invoke-All {
    Write-Section "Onesimus deploy (Windows)"
    Write-Info "Build type: $BuildType, backup system: $BackupSystem, PowerShell $($PSVersionTable.PSVersion)"
    if ($DryRun) { Write-Warn "DRY RUN - commands are printed, not executed." }
    Invoke-Build
    Invoke-DeployDlls
    Show-Summary
}

function Invoke-Run {
    Invoke-Build
    Invoke-DeployDlls
    $exe = Join-Path $DeployDir $ExeName
    if ($DryRun) { Write-Host "  + $exe"; return }
    Write-Info "Starting $exe"
    & $exe
}

function Invoke-Test {
    Invoke-Build
    Write-Section "Tests"
    $env:QT_QPA_PLATFORM = 'offscreen'
    $testArgs = @('--test-dir', $BuildDir, '-C', $BuildType, '--output-on-failure')
    if ($Jobs -gt 0) { $testArgs += @('-j', "$Jobs") }
    Invoke-Checked 'ctest' 'ctest' $testArgs
}

# --- Installers -------------------------------------------------------------
function Invoke-Installer {
    Invoke-Build
    Invoke-DeployDlls
    Write-Section "MSI installer (WiX)"
    $msiArgs = @('-NoProfile', '-ExecutionPolicy', 'Bypass',
        '-File', (Join-Path $ScriptDir 'package-msi.ps1'), '-BuildDir', $BuildDir)
    if ($DirdZip) { $msiArgs += @('-DirdZip', $DirdZip) }
    if ($DirdDir) { $msiArgs += @('-DirdDir', $DirdDir) }
    Invoke-Checked 'package-msi.ps1' 'powershell.exe' $msiArgs
}

function Invoke-Zip {
    Invoke-Build
    Invoke-DeployDlls
    $ver = if ($Version) { $Version } else { Get-StampedVersion }
    Write-Section "Portable ZIP"
    Invoke-Checked 'package-windows.ps1' 'powershell.exe' @('-NoProfile', '-ExecutionPolicy', 'Bypass',
        '-File', (Join-Path $ScriptDir 'package-windows.ps1'), '-BuildDir', $DeployDir, '-Version', $ver)
}

function Invoke-Install {
    Invoke-Build
    Invoke-DeployDlls
    $target = if ($Prefix) { $Prefix } else { Join-Path $BuildDir 'stage' }
    Write-Section "Install -> $target"
    Invoke-Checked 'cmake --install' 'cmake' @('--install', $BuildDir, '--config', $BuildType, '--prefix', $target)
    if (-not $DryRun) { Write-Ok "Installed to $target\bin" }
}

# --- 7. Summary / status / clean --------------------------------------------
function Show-Summary {
    if ($DryRun) { Write-Ok "Dry run finished - nothing was changed."; return }
    Write-Section "Build finished"
    $exe = Join-Path $DeployDir $ExeName
    if (-not (Test-Path $exe)) { $exe = Join-Path $BuildDir $ExeName }
    if (Test-Path $exe) {
        $fi = Get-Item $exe
        Write-Ok "Executable: $exe"
        Write-Info "  Size: $([math]::Round($fi.Length / 1MB, 2)) MB, built $($fi.LastWriteTime), version $(Get-StampedVersion)"
    }
    Write-Info "  OpenSSL is linked statically - no OpenSSL DLLs are required."
    Write-Host ""
    Write-Info "Next steps:"
    Write-Info "  Run:        .\deploy.ps1 run   (or $exe)"
    Write-Info "  Installer:  .\deploy.ps1 installer | msi | zip"
}

function Show-Status {
    Write-Section "Status"
    Write-Info "Build directory: $BuildDir"
    if (-not (Test-Path $BuildDir)) { Write-Warn "  does not exist - nothing built yet."; return }
    foreach ($exe in @((Join-Path $BuildDir $ExeName), (Join-Path $DeployDir $ExeName))) {
        if (Test-Path $exe) {
            $fi = Get-Item $exe
            Write-Ok ("  {0}  {1}" -f $exe, $fi.LastWriteTime.ToString('yyyy-MM-dd HH:mm'))
        }
    }
    if (Test-Path $DeployDir) {
        $dlls = (Get-ChildItem $DeployDir -Filter '*.dll' -ErrorAction SilentlyContinue | Measure-Object).Count
        $plat = Test-Path (Join-Path $DeployDir 'platforms\qwindows.dll')
        Write-Ok "  deploy\: $dlls DLLs, Qt platform plugin: $(if ($plat) { 'yes' } else { 'NO (windeployqt missing?)' })"
    }
    Write-Info "Stamped version (include\version.h): $(Get-StampedVersion)"
    Get-ChildItem -Path $BuildDir -Include '*.msi', '*.zip', '*setup*.exe', 'onesimus-*.exe' -File -ErrorAction SilentlyContinue |
        ForEach-Object { Write-Info "Package: $($_.FullName)" }
}

function Invoke-Clean {
    if (Test-Path $BuildDir) {
        Write-Info "Removing $BuildDir"
        Remove-Tree $BuildDir
    } else {
        Write-Info "$BuildDir does not exist - nothing to do."
    }
}

# --- Main -------------------------------------------------------------------
try {
    switch ($Command) {
        { $_ -in 'all', 'onesimus', 'gui' } { Invoke-All }
        'configure' { Invoke-Configure }
        'build'     { Invoke-Build; Show-Summary }
        'rebuild'   { Invoke-Rebuild; Show-Summary }
        'deploy'    { Confirm-Configured; Invoke-DeployDlls }
        'run'       { Invoke-Run }
        'test'      { Invoke-Test }
        { $_ -in 'installer', 'package', 'msi' } { Invoke-Installer }
        'zip'       { Invoke-Zip }
        'install'   { Invoke-Install }
        'status'    { Show-Status }
        'clean'     { Invoke-Clean }
        'help'      { Get-Help $PSCommandPath -Detailed }
    }
} catch {
    Write-Err $_.Exception.Message
    exit 1
}
