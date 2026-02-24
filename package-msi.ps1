#Requires -Version 5.1
# Onesimus Windows MSI Installer Creator
# Uses WiX v6 (dotnet tool) to create a proper MSI installer
#
# Prerequisites:
#   dotnet tool install --global wix
#   wix extension add WixToolset.UI.wixext/6.0.0
#
# Usage:
#   .\package-msi.ps1                          # Auto-detect from build dir
#   .\package-msi.ps1 -BuildDir .\build        # Explicit build dir
#   .\package-msi.ps1 -SkipDeploy              # Skip windeployqt step

param(
    [string]$BuildDir = "build",
    [switch]$SkipDeploy
)

$ErrorActionPreference = "Stop"

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Onesimus - MSI Installer Creator" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

# ============================================================================
# 1. Prerequisites
# ============================================================================

Write-Host "Pruefe Voraussetzungen..." -ForegroundColor Cyan
Write-Host ""

# WiX CLI
if (-not (Get-Command wix -ErrorAction SilentlyContinue)) {
    Write-Host "FEHLER: WiX CLI nicht gefunden!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Installation:" -ForegroundColor Yellow
    Write-Host "  dotnet tool install --global wix" -ForegroundColor White
    Write-Host "  wix extension add WixToolset.UI.wixext/6.0.0" -ForegroundColor White
    exit 1
}
$wixVersion = (wix --version 2>&1) | Select-Object -First 1
Write-Host "OK WiX: $wixVersion" -ForegroundColor Green

# Build directory and executable
$BuildDir = Resolve-Path $BuildDir -ErrorAction SilentlyContinue
if (-not $BuildDir) {
    Write-Host "FEHLER: Build-Verzeichnis nicht gefunden!" -ForegroundColor Red
    Write-Host "  Zuerst bauen: .\build-windows.ps1" -ForegroundColor Yellow
    exit 1
}

$exePath = Join-Path $BuildDir "Onesimus.exe"
if (-not (Test-Path $exePath)) {
    Write-Host "FEHLER: Onesimus.exe nicht gefunden in: $BuildDir" -ForegroundColor Red
    exit 1
}
Write-Host "OK Onesimus.exe gefunden" -ForegroundColor Green

# Version aus CMakeLists.txt lesen (kanonische Quelle)
$cmakeContent = Get-Content "$PSScriptRoot\CMakeLists.txt" -Raw
if ($cmakeContent -match 'project\(\s*onesimus\s+VERSION\s+(\d+\.\d+\.\d+)') {
    $productVersion = $matches[1]
} else {
    # Fallback: aus Exe-Metadaten
    $fileVersionInfo = (Get-Item $exePath).VersionInfo
    $productVersion = $fileVersionInfo.ProductVersion
    if (-not $productVersion -or $productVersion -eq "") {
        $productVersion = "0.2.0"
    }
}
# MSI requires exactly 3 or 4 numeric parts
$versionParts = $productVersion -split '[.\-~]' | Where-Object { $_ -match '^\d+$' } | Select-Object -First 3
$msiVersion = ($versionParts -join '.')
Write-Host "   Version: $msiVersion" -ForegroundColor Cyan

Write-Host ""

# ============================================================================
# 2. Deploy (windeployqt)
# ============================================================================

$deployDir = Join-Path $BuildDir "msi-staging"

if (-not $SkipDeploy) {
    Write-Host "========================================" -ForegroundColor Magenta
    Write-Host "Deployment (windeployqt)" -ForegroundColor Magenta
    Write-Host "========================================" -ForegroundColor Magenta
    Write-Host ""

    # Clean staging dir
    if (Test-Path $deployDir) {
        Remove-Item -Path $deployDir -Recurse -Force
    }
    New-Item -ItemType Directory -Path $deployDir | Out-Null

    # Copy executable
    Copy-Item -Path $exePath -Destination $deployDir
    Write-Host "OK Onesimus.exe kopiert" -ForegroundColor Green

    # Find windeployqt
    $qtRootDirs = @("C:\Qt", "C:\Qt6")
    $windeployqt = $null

    foreach ($qtRoot in $qtRootDirs) {
        if (Test-Path $qtRoot) {
            $versionDirs = Get-ChildItem -Path $qtRoot -Directory -ErrorAction SilentlyContinue |
                           Where-Object { $_.Name -match '^\d+\.\d+(\.\d+)?$' } |
                           Sort-Object { [version]$_.Name } -Descending

            foreach ($versionDir in $versionDirs) {
                $compilerDir = Get-ChildItem -Path $versionDir.FullName -Directory -ErrorAction SilentlyContinue |
                               Where-Object { $_.Name -like "msvc*64" -and $_.Name -notlike "*arm*" } |
                               Select-Object -First 1

                if ($compilerDir) {
                    $candidate = Join-Path $compilerDir.FullName "bin\windeployqt.exe"
                    if (Test-Path $candidate) {
                        $windeployqt = $candidate
                        break
                    }
                }
            }
        }
        if ($windeployqt) { break }
    }

    if (-not $windeployqt) {
        Write-Host "FEHLER: windeployqt nicht gefunden!" -ForegroundColor Red
        exit 1
    }
    Write-Host "OK windeployqt: $windeployqt" -ForegroundColor Green

    # Copy OpenSSL DLLs if they exist (shared build)
    $opensslBinDir = Join-Path $BuildDir "openssl-install\bin"
    $opensslRootDir = Join-Path $BuildDir "openssl-install"
    if (Test-Path "$opensslBinDir\libssl-3-x64.dll") {
        Copy-Item -Path "$opensslBinDir\libssl-3-x64.dll" -Destination $deployDir
        Copy-Item -Path "$opensslBinDir\libcrypto-3-x64.dll" -Destination $deployDir
        Write-Host "OK OpenSSL DLLs kopiert" -ForegroundColor Green
    }

    # Run windeployqt (Warnungen auf stderr ignorieren)
    # --openssl-root ensures qopensslbackend.dll TLS plugin is deployed (required for PSK)
    Write-Host "Starte windeployqt..." -ForegroundColor Cyan
    $deployExe = Join-Path $deployDir "Onesimus.exe"
    $ErrorActionPreference = "Continue"
    if (Test-Path $opensslRootDir) {
        & $windeployqt --release --no-translations --no-system-d3d-compiler --no-opengl-sw --compiler-runtime --openssl-root $opensslRootDir $deployExe 2>&1
    } else {
        & $windeployqt --release --no-translations --no-system-d3d-compiler --no-opengl-sw --compiler-runtime $deployExe 2>&1
    }
    $ErrorActionPreference = "Stop"

    if ($LASTEXITCODE -ne 0) {
        Write-Host "WARNUNG: windeployqt meldete Fehler (Exit $LASTEXITCODE)" -ForegroundColor Yellow
    } else {
        Write-Host "OK windeployqt abgeschlossen" -ForegroundColor Green
    }
} else {
    if (-not (Test-Path $deployDir)) {
        Write-Host "FEHLER: Staging-Verzeichnis nicht gefunden: $deployDir" -ForegroundColor Red
        Write-Host "  Zuerst ohne -SkipDeploy ausfuehren" -ForegroundColor Yellow
        exit 1
    }
    Write-Host "   Ueberspringe windeployqt (-SkipDeploy)" -ForegroundColor Cyan
}

Write-Host ""

# ============================================================================
# 3. Generate WiX Source (.wxs)
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "WiX-Quelldatei generieren" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

# Stable UpgradeCode (must stay the same across versions!)
$upgradeCode = "E4A7B2C1-3F5D-4E8A-9B1C-6D2F0A8E5C37"

# Collect all files from deploy directory
function Get-DeployFiles {
    param([string]$BaseDir)

    $allFiles = @()
    $allDirs = @()

    # Get all files recursively
    Get-ChildItem -Path $BaseDir -Recurse -File | ForEach-Object {
        $relativePath = $_.FullName.Substring($BaseDir.Length + 1)
        $relativeDir = if ($_.DirectoryName -eq $BaseDir) { "" } else { $_.DirectoryName.Substring($BaseDir.Length + 1) }
        $allFiles += @{
            Name = $_.Name
            FullPath = $_.FullName
            RelativePath = $relativePath
            RelativeDir = $relativeDir
        }

        if ($relativeDir -and $relativeDir -notin $allDirs) {
            $allDirs += $relativeDir
        }
    }

    return @{ Files = $allFiles; Dirs = $allDirs }
}

$deployData = Get-DeployFiles -BaseDir $deployDir
$fileCount = $deployData.Files.Count
Write-Host "   $fileCount Dateien gefunden" -ForegroundColor Cyan

# Helper: Create a safe WiX ID from a path
function Get-SafeId {
    param([string]$Path, [string]$Prefix = "f")
    $safe = $Path -replace '[^a-zA-Z0-9_.]', '_'
    return "${Prefix}_${safe}"
}

# Generate WiX XML
$iconPath = "$PSScriptRoot\resources\icons\onesimus.ico"
$licensePath = "$PSScriptRoot\LICENSE"

# Build directory XML fragments
$directoryXml = ""
$componentXml = ""
$componentRefs = ""

# Root-level files first
$rootFiles = $deployData.Files | Where-Object { $_.RelativeDir -eq "" }
foreach ($file in $rootFiles) {
    $id = Get-SafeId -Path $file.Name -Prefix "comp"
    $fileId = Get-SafeId -Path $file.Name -Prefix "file"
    $componentXml += @"

    <Component Id="$id" Directory="INSTALLFOLDER" Guid="*">
      <File Id="$fileId" Source="$($file.FullPath)" KeyPath="yes" />
    </Component>
"@
    $componentRefs += "      <ComponentRef Id=`"$id`" />`n"
}

# Subdirectory files
$subDirs = $deployData.Dirs | Sort-Object
foreach ($dir in $subDirs) {
    $dirId = Get-SafeId -Path $dir -Prefix "dir"
    $dirFiles = $deployData.Files | Where-Object { $_.RelativeDir -eq $dir }

    foreach ($file in $dirFiles) {
        $id = Get-SafeId -Path $file.RelativePath -Prefix "comp"
        $fileId = Get-SafeId -Path $file.RelativePath -Prefix "file"
        $componentXml += @"

    <Component Id="$id" Directory="$dirId" Guid="*">
      <File Id="$fileId" Source="$($file.FullPath)" KeyPath="yes" />
    </Component>
"@
        $componentRefs += "      <ComponentRef Id=`"$id`" />`n"
    }
}

# Build directory tree XML
$directoryTree = ""
foreach ($dir in $subDirs) {
    $dirId = Get-SafeId -Path $dir -Prefix "dir"
    $dirName = Split-Path $dir -Leaf
    $parentDir = Split-Path $dir -Parent

    if ($parentDir -eq "" -or $parentDir -eq $null) {
        $directoryTree += "        <Directory Id=`"$dirId`" Name=`"$dirName`" />`n"
    }
}

# Handle nested directories (only one level deep in most Qt deployments)
$nestedDirs = $subDirs | Where-Object { $_ -match '\\' }
foreach ($dir in $nestedDirs) {
    $dirId = Get-SafeId -Path $dir -Prefix "dir"
    $dirName = Split-Path $dir -Leaf
    $parentPath = Split-Path $dir -Parent
    $parentId = Get-SafeId -Path $parentPath -Prefix "dir"

    # Remove simple entry, add nested
    $directoryTree = $directoryTree -replace "        <Directory Id=`"$parentId`"([^/]*)/>`n", "        <Directory Id=`"$parentId`"`$1>`n          <Directory Id=`"$dirId`" Name=`"$dirName`" />`n        </Directory>`n"
}

# Build the .wxs content
$wxsContent = @"
<?xml version="1.0" encoding="UTF-8"?>
<Wix xmlns="http://wixtoolset.org/schemas/v4/wxs"
     xmlns:ui="http://wixtoolset.org/schemas/v4/wxs/ui">

  <Package Name="Onesimus"
           Manufacturer="Bernau Family"
           Version="$msiVersion"
           UpgradeCode="$upgradeCode"
           Compressed="yes"
           InstallerVersion="500"
           Scope="perMachine">

    <SummaryInformation Description="Onesimus - Bareos/Bacula Management Tool" />

    <MajorUpgrade DowngradeErrorMessage="Eine neuere Version von Onesimus ist bereits installiert." />
    <MediaTemplate EmbedCab="yes" />

    <!-- Icon fuer Add/Remove Programs -->
    <Icon Id="AppIcon" SourceFile="$iconPath" />
    <Property Id="ARPPRODUCTICON" Value="AppIcon" />
    <Property Id="ARPHELPLINK" Value="https://github.com/Beerlesklopfer/Onesimus" />

    <!-- Install UI mit Feature-Auswahl (Checkboxen fuer Shortcuts) -->
    <ui:WixUI Id="WixUI_FeatureTree" />

    <!-- Lizenz -->
    <WixVariable Id="WixUILicenseRtf" Value="$($BuildDir)\license.rtf" />

    <!-- Features mit Checkboxen -->
    <Feature Id="MainFeature" Title="Onesimus" Description="Onesimus Programmdateien" Level="1">
$componentRefs
    </Feature>

    <Feature Id="DesktopShortcut" Title="Desktop-Verknuepfung" Description="Erstellt eine Verknuepfung auf dem Desktop" Level="1">
      <ComponentRef Id="comp_DesktopShortcut" />
    </Feature>

    <Feature Id="StartMenuShortcut" Title="Startmenue-Verknuepfung" Description="Erstellt eine Verknuepfung im Startmenue" Level="1">
      <ComponentRef Id="comp_StartMenuShortcut" />
    </Feature>

    <!-- Directory Structure -->
    <StandardDirectory Id="ProgramFiles6432Folder">
      <Directory Id="INSTALLFOLDER" Name="Onesimus">
$directoryTree
      </Directory>
    </StandardDirectory>

    <StandardDirectory Id="ProgramMenuFolder">
      <Directory Id="OnesimusMenuFolder" Name="Onesimus" />
    </StandardDirectory>

    <StandardDirectory Id="DesktopFolder" />

    <!-- Components -->
    <ComponentGroup Id="ProductComponents">
$componentXml

    <!-- Start Menu Shortcut -->
    <Component Id="comp_StartMenuShortcut" Directory="OnesimusMenuFolder" Guid="*">
      <Shortcut Id="StartMenuShortcut"
                Name="Onesimus"
                Description="Bareos/Bacula Management Tool"
                Target="[INSTALLFOLDER]Onesimus.exe"
                WorkingDirectory="INSTALLFOLDER"
                Icon="AppIcon" />
      <RemoveFolder Id="RemoveMenuFolder" On="uninstall" />
      <RegistryValue Root="HKCU" Key="Software\Onesimus" Name="StartMenuShortcut" Type="integer" Value="1" KeyPath="yes" />
    </Component>

    <!-- Desktop Shortcut -->
    <Component Id="comp_DesktopShortcut" Directory="DesktopFolder" Guid="*">
      <Shortcut Id="DesktopShortcut"
                Name="Onesimus"
                Description="Bareos/Bacula Management Tool"
                Target="[INSTALLFOLDER]Onesimus.exe"
                WorkingDirectory="INSTALLFOLDER"
                Icon="AppIcon" />
      <RegistryValue Root="HKCU" Key="Software\Onesimus" Name="DesktopShortcut" Type="integer" Value="1" KeyPath="yes" />
    </Component>
    </ComponentGroup>

  </Package>
</Wix>
"@

# Create license.rtf from LICENSE (WiX requires RTF format)
if (Test-Path $licensePath) {
    $licenseText = Get-Content $licensePath -Raw
    $rtfContent = "{\rtf1\ansi\deff0{\fonttbl{\f0\fswiss Segoe UI;}}\f0\fs18 " + ($licenseText -replace "`r`n", "\par`r`n" -replace "`n", "\par`r`n") + "}"
    $rtfPath = Join-Path $BuildDir "license.rtf"
    [System.IO.File]::WriteAllText($rtfPath, $rtfContent, [System.Text.Encoding]::ASCII)
    Write-Host "OK Lizenz-RTF erstellt" -ForegroundColor Green
} else {
    Write-Host "WARNUNG: LICENSE-Datei nicht gefunden, verwende Platzhalter" -ForegroundColor Yellow
    $rtfContent = "{\rtf1\ansi\deff0{\fonttbl{\f0\fswiss Segoe UI;}}\f0\fs18 Onesimus - Bareos/Bacula Management Tool\par\par MIT License\par\par (C) Bernau Family\par\par Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files, to deal in the Software without restriction.\par\par See LICENSE file for full text.}"
    $rtfPath = Join-Path $BuildDir "license.rtf"
    [System.IO.File]::WriteAllText($rtfPath, $rtfContent, [System.Text.Encoding]::ASCII)
}

# Write .wxs file
$wxsPath = Join-Path $BuildDir "onesimus.wxs"
[System.IO.File]::WriteAllText($wxsPath, $wxsContent, (New-Object System.Text.UTF8Encoding $true))
Write-Host "OK WiX-Quelldatei erstellt: $wxsPath" -ForegroundColor Green

Write-Host ""

# ============================================================================
# 4. Build MSI
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "MSI-Installer erstellen" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

$msiOutputName = "Onesimus-$msiVersion-Windows.msi"
$msiOutputPath = Join-Path $BuildDir $msiOutputName

Write-Host "Kompiliere MSI..." -ForegroundColor Cyan

# Run wix build
& wix build $wxsPath -arch x64 -ext WixToolset.UI.wixext -o $msiOutputPath 2>&1

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "FEHLER: MSI-Erstellung fehlgeschlagen!" -ForegroundColor Red
    Write-Host "  WiX-Quelldatei: $wxsPath" -ForegroundColor Gray
    Write-Host "  Ueberpruefen Sie die Fehlermeldungen oben." -ForegroundColor Yellow
    exit 1
}

Write-Host ""

# ============================================================================
# 5. Summary
# ============================================================================

Write-Host "========================================" -ForegroundColor Green
Write-Host "MSI-Installer erfolgreich erstellt!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

if (Test-Path $msiOutputPath) {
    $msiInfo = Get-Item $msiOutputPath
    $msiSizeMB = [math]::Round($msiInfo.Length / 1MB, 2)

    Write-Host "MSI-Datei:  $($msiInfo.Name)" -ForegroundColor Green
    Write-Host "Groesse:    $msiSizeMB MB" -ForegroundColor Cyan
    Write-Host "Pfad:       $($msiInfo.FullName)" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Installation:" -ForegroundColor Yellow
    Write-Host "  Doppelklick auf die MSI-Datei" -ForegroundColor White
    Write-Host "  Oder: msiexec /i `"$msiOutputName`"" -ForegroundColor White
    Write-Host ""
    Write-Host "Stille Installation:" -ForegroundColor Yellow
    Write-Host "  msiexec /i `"$msiOutputName`" /qn" -ForegroundColor White
}

Write-Host ""
