#Requires -Version 5.1
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

<#
.SYNOPSIS
    Builds the Onesimus MSI (WiX Toolset v6) from packaging\windows\onesimus.wxs.

.DESCRIPTION
    Input:
      - <BuildDir>\deploy: onesimus.exe with the windeployqt output
        (".\deploy.ps1 deploy" or any deploy.ps1 verb that deploys).
      - optional: the Windows build of onesimus-dird, a zip named
        onesimus-dird-<version>-windows-x86_64.zip with bin\oconsole.exe and
        bin\ofd.exe (-DirdZip, or found in dist\rust\), or an unpacked folder
        that contains bin\ (-DirdDir). Missing binaries are skipped: the
        feature is then neither shown nor installed.

    Output: <OutDir>\Onesimus-<version>-windows-x86_64.msi and a .sha256 file
    next to it. The MSI version is the GUI version from CMakeLists.txt; the
    dird version is recorded in the property ONESIMUS_DIRD_VERSION and in
    HKLM\Software\Bernau Family\Onesimus\DirdVersion.

    Prerequisites (the release workflow installs them the same way):
      dotnet tool install --global wix --version 6.0.2
      wix extension add -g WixToolset.UI.wixext/6.0.2 WixToolset.Firewall.wixext/6.0.2

.EXAMPLE
    .\package-msi.ps1
.EXAMPLE
    .\package-msi.ps1 -DirdZip C:\Downloads\onesimus-dird-0.9.0-windows-x86_64.zip
.EXAMPLE
    msiexec /i Onesimus-1.0.0-windows-x86_64.msi ADDLOCAL=Gui,Oconsole,Ofd /qn /l*v install.log
#>
[CmdletBinding()]
param(
    [string]$BuildDir = 'build',
    [string]$DirdZip,
    [string]$DirdDir,
    [string]$OutDir
)

$ErrorActionPreference = 'Stop'
$WixVersion = '6.0.2'
$Extensions = @("WixToolset.UI.wixext/$WixVersion", "WixToolset.Firewall.wixext/$WixVersion")

$RepoDir = $PSScriptRoot
function Write-Info($msg) { Write-Host "[msi] $msg" -ForegroundColor Cyan }
function Write-Ok($msg)   { Write-Host "[msi] $msg" -ForegroundColor Green }
function Write-Warn($msg) { Write-Host "[msi] $msg" -ForegroundColor Yellow }

if (-not [System.IO.Path]::IsPathRooted($BuildDir)) { $BuildDir = Join-Path $RepoDir $BuildDir }
if (-not $OutDir) { $OutDir = $BuildDir }
if (-not [System.IO.Path]::IsPathRooted($OutDir)) { $OutDir = Join-Path $RepoDir $OutDir }
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

# --- WiX -------------------------------------------------------------------
if (-not (Get-Command wix -ErrorAction SilentlyContinue)) {
    throw "WiX CLI not found. Install: dotnet tool install --global wix --version $WixVersion"
}
Write-Info "WiX $((wix --version) | Select-Object -First 1)"
$installed = (wix extension list -g) -join "`n"
foreach ($ext in $Extensions) {
    $id, $ver = $ext -split '/'
    if ($installed -notmatch [regex]::Escape($id) + '\s+' + [regex]::Escape($ver)) {
        Write-Info "Adding WiX extension $ext"
        wix extension add -g $ext
        if ($LASTEXITCODE -ne 0) { throw "wix extension add $ext failed" }
    }
}

# --- GUI -------------------------------------------------------------------
$GuiDir = Join-Path $BuildDir 'deploy'
if (-not (Test-Path (Join-Path $GuiDir 'onesimus.exe'))) {
    throw "$GuiDir\onesimus.exe missing - run .\deploy.ps1 deploy first"
}
if (-not (Test-Path (Join-Path $GuiDir 'Qt6Core.dll'))) {
    throw "$GuiDir has no Qt6Core.dll - windeployqt did not run"
}

# The GUI version (project() in CMakeLists.txt) is the MSI version.
$cmake = Get-Content (Join-Path $RepoDir 'CMakeLists.txt') -Raw
if ($cmake -notmatch 'project\(\s*onesimus\s+VERSION\s+(\d+\.\d+\.\d+)') { throw 'No project(onesimus VERSION ...) in CMakeLists.txt' }
$Version = $matches[1]
Write-Info "Version: $Version"

# --- onesimus-dird (optional) ---------------------------------------------
$DirdVersion = 'none'
$DirdBin = Join-Path $BuildDir 'dird-missing'
if (-not $DirdDir) {
    if (-not $DirdZip) {
        $DirdZip = Get-ChildItem -Path (Join-Path $RepoDir 'dist\rust') -Filter 'onesimus-dird-*-windows-x86_64.zip' -ErrorAction SilentlyContinue |
                   Sort-Object LastWriteTime -Descending | Select-Object -First 1 -ExpandProperty FullName
    }
    if ($DirdZip) {
        if (-not (Test-Path $DirdZip)) { throw "dird zip not found: $DirdZip" }
        $DirdDir = Join-Path $BuildDir 'dird'
        if (Test-Path $DirdDir) { Remove-Item -Recurse -Force $DirdDir }
        Write-Info "Unpacking $DirdZip"
        Expand-Archive -Path $DirdZip -DestinationPath $DirdDir
        # A zip may wrap everything in one top folder: use the folder with bin\.
        $binParent = Get-ChildItem -Path $DirdDir -Directory -Recurse -Filter 'bin' | Select-Object -First 1
        if ($binParent) { $DirdDir = $binParent.Parent.FullName }
        if ((Split-Path $DirdZip -Leaf) -match '^onesimus-dird-(.+)-windows-x86_64\.zip$') { $DirdVersion = $matches[1] }
    }
}
if ($DirdDir) {
    $DirdBin = Join-Path $DirdDir 'bin'
    if ($DirdVersion -eq 'none' -and (Test-Path (Join-Path $DirdDir 'VERSION'))) {
        $DirdVersion = (Get-Content (Join-Path $DirdDir 'VERSION') -TotalCount 1).Trim()
    }
}
$HaveOconsole = [int](Test-Path (Join-Path $DirdBin 'oconsole.exe'))
$HaveOfd      = [int](Test-Path (Join-Path $DirdBin 'ofd.exe'))
if ($HaveOconsole) { Write-Ok "oconsole: $DirdBin\oconsole.exe (dird $DirdVersion)" } else { Write-Warn 'oconsole.exe not provided - feature Oconsole skipped' }
if ($HaveOfd)      { Write-Ok "ofd:      $DirdBin\ofd.exe (dird $DirdVersion)" }      else { Write-Warn 'ofd.exe not provided - feature Ofd skipped' }

# --- Licence as RTF --------------------------------------------------------
$licenseText = Get-Content (Join-Path $RepoDir 'LICENSE') -Raw
$escaped = $licenseText -replace '\\', '\\' -replace '\{', '\{' -replace '\}', '\}' -replace "`r?`n", "\par`r`n"
$rtf = "{\rtf1\ansi\deff0{\fonttbl{\f0\fswiss Segoe UI;}}\f0\fs16 " + $escaped + '}'
$LicenseRtf = Join-Path $BuildDir 'license.rtf'
[System.IO.File]::WriteAllText($LicenseRtf, $rtf, [System.Text.Encoding]::ASCII)

# --- Build -----------------------------------------------------------------
$msi = Join-Path $OutDir "Onesimus-$Version-windows-x86_64.msi"
$wixArgs = @('build', (Join-Path $RepoDir 'packaging\windows\onesimus.wxs'),
    '-arch', 'x64',
    '-bindpath', "Gui=$GuiDir",
    '-d', "ProductVersion=$Version",
    '-d', "HaveOconsole=$HaveOconsole",
    '-d', "HaveOfd=$HaveOfd",
    '-d', "DirdBinDir=$DirdBin",
    '-d', "DirdVersion=$DirdVersion",
    '-d', "LicenseRtf=$LicenseRtf",
    '-d', "RepoDir=$RepoDir",
    '-o', $msi)
foreach ($ext in $Extensions) { $wixArgs += @('-ext', $ext) }
Write-Info "wix $($wixArgs -join ' ')"
& wix @wixArgs
if ($LASTEXITCODE -ne 0) { throw "wix build failed (exit code $LASTEXITCODE)" }

$hash = (Get-FileHash -Algorithm SHA256 $msi).Hash.ToLower()
"$hash  $(Split-Path $msi -Leaf)" | Out-File -Encoding ascii -NoNewline "$msi.sha256"
Write-Ok "MSI: $msi ($([math]::Round((Get-Item $msi).Length / 1MB, 1)) MB)"
Write-Ok "sha256: $hash"
Write-Host ''
Write-Host 'Silent install examples:'
Write-Host "  msiexec /i `"$(Split-Path $msi -Leaf)`" /qn                          (GUI + oconsole)"
Write-Host "  msiexec /i `"$(Split-Path $msi -Leaf)`" ADDLOCAL=Gui,Oconsole,Ofd /qn  (with the ofd service)"
Write-Host "  msiexec /i `"$(Split-Path $msi -Leaf)`" ADDLOCAL=ALL REMOVE=DesktopShortcut /qn"
