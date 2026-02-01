#Requires -Version 5.1
# Onesimus Windows ZIP Package Creator
# Creates a portable ZIP package with all Qt dependencies
# No additional software required - uses built-in PowerShell

param(
    [Parameter(Mandatory=$true)]
    [string]$BuildDir,

    [Parameter(Mandatory=$true)]
    [string]$Version
)

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Onesimus - Windows ZIP Package Creator" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""
Write-Host "Version:    $Version" -ForegroundColor Cyan
Write-Host "Build Dir:  $BuildDir" -ForegroundColor Cyan
Write-Host ""

# ============================================================================
# 1. Verify Prerequisites
# ============================================================================

Write-Host "Checking prerequisites..." -ForegroundColor Cyan
Write-Host ""

# Check if executable exists
$exePath = Join-Path $BuildDir "Onesimus.exe"
if (-not (Test-Path $exePath)) {
    Write-Host "FEHLER: Onesimus.exe nicht gefunden: $exePath" -ForegroundColor Red
    exit 1
}
Write-Host "OK Onesimus.exe gefunden" -ForegroundColor Green

# Check if Qt dependencies were deployed
$qtDllCheck = Join-Path $BuildDir "Qt6Core.dll"
if (-not (Test-Path $qtDllCheck)) {
    Write-Host "WARNUNG: Qt DLLs nicht gefunden - wurde windeployqt ausgefuehrt?" -ForegroundColor Yellow
    Write-Host "         POST_BUILD command sollte dies automatisch erledigen." -ForegroundColor Yellow
} else {
    Write-Host "OK Qt dependencies deployed" -ForegroundColor Green
}

Write-Host ""

# ============================================================================
# 2. Create Package Directory
# ============================================================================

$packageName = "Onesimus-$Version-Windows"
$packageDir = Join-Path $BuildDir $packageName
$zipFile = Join-Path $BuildDir "$packageName.zip"

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Package erstellen" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

# Remove old package if exists
if (Test-Path $packageDir) {
    Write-Host "Loesche altes Package-Verzeichnis..." -ForegroundColor Cyan
    Remove-Item -Path $packageDir -Recurse -Force
}

if (Test-Path $zipFile) {
    Write-Host "Loesche altes ZIP-Archiv..." -ForegroundColor Cyan
    Remove-Item -Path $zipFile -Force
}

# Create new package directory
Write-Host "Erstelle Package-Verzeichnis: $packageName" -ForegroundColor Cyan
New-Item -ItemType Directory -Path $packageDir | Out-Null
Write-Host ""

# ============================================================================
# 3. Copy Files
# ============================================================================

Write-Host "Kopiere Dateien..." -ForegroundColor Cyan
Write-Host ""

# Copy main executable
Write-Host "   Onesimus.exe" -ForegroundColor Gray
Copy-Item -Path $exePath -Destination $packageDir

# Copy Qt DLLs (deployed by windeployqt)
$qtFiles = @(
    "*.dll",
    "iconengines",
    "imageformats",
    "platforms",
    "styles",
    "translations"
)

foreach ($pattern in $qtFiles) {
    $items = Get-ChildItem -Path $BuildDir -Filter $pattern -ErrorAction SilentlyContinue
    foreach ($item in $items) {
        if ($item.PSIsContainer) {
            Write-Host "   $($item.Name)\" -ForegroundColor Gray
            Copy-Item -Path $item.FullName -Destination $packageDir -Recurse
        } else {
            Write-Host "   $($item.Name)" -ForegroundColor Gray
            Copy-Item -Path $item.FullName -Destination $packageDir
        }
    }
}

# Create README for the package
$readmeContent = @"
# Onesimus v$Version

Bareos/Bacula Management Tool

## Installation

1. Extract this ZIP file to any location
2. Run Onesimus.exe

## Requirements

- Windows 10/11 (64-bit)
- No additional software required - all dependencies included

## Features

- Connect to Bareos/Bacula Director
- Manage jobs, clients, filesets, pools, storages
- Run and monitor backup jobs
- Restore files from backups
- Job history and statistics

## Support

GitHub: https://github.com/Beerlesklopfer/Onesimus
Wiki:   https://github.com/Beerlesklopfer/Onesimus/wiki

## License

See LICENSE file

---

Build: v$Version
Created: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
"@

$readmeContent | Out-File -FilePath (Join-Path $packageDir "README.txt") -Encoding UTF8
Write-Host "   README.txt" -ForegroundColor Gray

Write-Host ""

# ============================================================================
# 4. Create ZIP Archive
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "ZIP-Archiv erstellen" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

Write-Host "Komprimiere zu: $packageName.zip" -ForegroundColor Cyan

try {
    Compress-Archive -Path $packageDir -DestinationPath $zipFile -CompressionLevel Optimal
    Write-Host "OK ZIP-Archiv erstellt" -ForegroundColor Green
} catch {
    Write-Host "FEHLER: ZIP-Erstellung fehlgeschlagen!" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    exit 1
}

Write-Host ""

# ============================================================================
# 5. Package Information
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Package Information" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

if (Test-Path $zipFile) {
    $zipInfo = Get-Item $zipFile
    $zipSizeMB = [math]::Round($zipInfo.Length / 1MB, 2)

    Write-Host "Package:  $($zipInfo.Name)" -ForegroundColor Green
    Write-Host "Groesse:  $zipSizeMB MB" -ForegroundColor Cyan
    Write-Host "Pfad:     $($zipInfo.FullName)" -ForegroundColor Cyan
    Write-Host ""
}

# Cleanup package directory (keep only ZIP)
Write-Host "Raeume auf..." -ForegroundColor Cyan
Remove-Item -Path $packageDir -Recurse -Force
Write-Host ""

# ============================================================================
# 6. Summary
# ============================================================================

Write-Host "========================================" -ForegroundColor Green
Write-Host "Package erfolgreich erstellt!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "ZIP-Datei:  $zipFile" -ForegroundColor Green
Write-Host ""
Write-Host "Dieses Package ist portable und kann auf jedem Windows 10/11 System" -ForegroundColor Cyan
Write-Host "ohne Installation verwendet werden." -ForegroundColor Cyan
Write-Host ""
Write-Host "Zum Testen:" -ForegroundColor Yellow
Write-Host "  1. Extrahiere: $packageName.zip" -ForegroundColor White
Write-Host "  2. Starte:     Onesimus.exe" -ForegroundColor White
Write-Host ""
