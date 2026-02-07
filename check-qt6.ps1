#Requires -Version 5.1
# Qt6 Installation und Diagnose-Tool

param(
    [switch]$Install
)

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Qt6 Installation Diagnose" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

# ============================================================================
# 1. Qt6-Installation suchen
# ============================================================================

Write-Host "Suche Qt6-Installation..." -ForegroundColor Cyan
Write-Host ""

# Dynamisch nach Qt-Installationen suchen
$qtRootDirs = @("C:\Qt", "C:\Qt6")
$foundInstallations = @()

foreach ($qtRoot in $qtRootDirs) {
    if (Test-Path $qtRoot) {
        # Suche nach Versionsverzeichnissen (z.B. 6.10.1, 6.9.0, etc.)
        $versionDirs = Get-ChildItem -Path $qtRoot -Directory -ErrorAction SilentlyContinue |
                       Where-Object { $_.Name -match '^\d+\.\d+\.\d+$' }

        foreach ($versionDir in $versionDirs) {
            # Suche nach Compiler-Verzeichnissen (msvc2022_64, etc.)
            $compilerDirs = Get-ChildItem -Path $versionDir.FullName -Directory -ErrorAction SilentlyContinue |
                           Where-Object { $_.Name -like "msvc*64" }

            foreach ($compilerDir in $compilerDirs) {
                $path = $compilerDir.FullName
                $qmakeExe = Join-Path $path "bin\qmake.exe"

                if (Test-Path $qmakeExe) {
                    $version = & $qmakeExe -query QT_VERSION
                    Write-Host "OK Gefunden: $path" -ForegroundColor Green
                    Write-Host "   Version: $version" -ForegroundColor Cyan
                    Write-Host "   Compiler: $($compilerDir.Name)" -ForegroundColor Cyan

                    # Pruefe Qt6Config.cmake
                    $qt6ConfigCmake = Join-Path $path "lib\cmake\Qt6\Qt6Config.cmake"
                    if (Test-Path $qt6ConfigCmake) {
                        Write-Host "   Qt6Config.cmake: OK Vorhanden" -ForegroundColor Green
                        $foundInstallations += @{
                            Path = $path
                            Version = $version
                            Compiler = $compilerDir.Name
                        }
                    } else {
                        Write-Host "   Qt6Config.cmake: FEHLER Fehlt!" -ForegroundColor Red
                    }
                    Write-Host ""
                }
            }
        }
    }
}

if ($foundInstallations.Count -eq 0) {
    Write-Host "FEHLER: Keine Qt6-Installation gefunden!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Installation erforderlich:" -ForegroundColor Yellow
    Write-Host "  1. Online Installer: https://www.qt.io/download-qt-installer" -ForegroundColor White
    Write-Host "  2. Oder: .\check-qt6.ps1 -Install" -ForegroundColor White
    Write-Host ""
    
    if ($Install) {
        Write-Host "Oeffne Qt Installer Download-Seite..." -ForegroundColor Cyan
        Start-Process "https://www.qt.io/download-qt-installer"
    }
    
    exit 1
}

# ============================================================================
# 2. Detaillierte Modul-Pruefung
# ============================================================================

# Waehle die neueste Qt-Version (sortiert nach Version)
$selectedQt = ($foundInstallations | Sort-Object { [version]$_.Version } -Descending | Select-Object -First 1).Path

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Detaillierte Modul-Pruefung" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""
Write-Host "Ausgewaehlte Installation: $selectedQt" -ForegroundColor Cyan
Write-Host "Version: $(($foundInstallations | Where-Object { $_.Path -eq $selectedQt }).Version)" -ForegroundColor Cyan
Write-Host ""

# Erforderliche Module
$requiredModules = @("Qt6Core", "Qt6Gui", "Qt6Widgets", "Qt6Network", "Qt6Sql")

$allModulesPresent = $true

foreach ($module in $requiredModules) {
    $moduleCMakePath = Join-Path $selectedQt "lib\cmake\$module\${module}Config.cmake"
    
    if (Test-Path $moduleCMakePath) {
        Write-Host "OK $module" -ForegroundColor Green
    } else {
        Write-Host "FEHLER $module - Fehlt!" -ForegroundColor Red
        $allModulesPresent = $false
    }
}

Write-Host ""

# ============================================================================
# 3. CMake Module Path
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "CMake-Konfiguration" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

$cmakeModulePath = Join-Path $selectedQt "lib\cmake"

if (Test-Path $cmakeModulePath) {
    Write-Host "OK CMake Module Path: $cmakeModulePath" -ForegroundColor Green
    Write-Host ""
    
    # Liste verfuegbare Qt6-Module
    $availableModules = Get-ChildItem $cmakeModulePath -Directory | Where-Object { $_.Name -like "Qt6*" }
    
    Write-Host "Verfuegbare Qt6-Module:" -ForegroundColor Cyan
    foreach ($mod in $availableModules) {
        Write-Host "  - $($mod.Name)" -ForegroundColor Gray
    }
    Write-Host ""
    
} else {
    Write-Host "FEHLER: CMake Module Path nicht gefunden!" -ForegroundColor Red
    Write-Host ""
}

# ============================================================================
# 4. Empfohlene CMAKE_PREFIX_PATH
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "CMake Build-Konfiguration" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

Write-Host "Fuer CMake verwenden Sie:" -ForegroundColor Cyan
Write-Host ""
Write-Host "  cmake .. -DCMAKE_PREFIX_PATH=`"$selectedQt`"" -ForegroundColor Green
Write-Host ""
Write-Host "Oder mit build-windows.ps1:" -ForegroundColor Cyan
Write-Host "  .\build-windows.ps1" -ForegroundColor Green
Write-Host ""

# ============================================================================
# 5. Fehlende Module
# ============================================================================

if (-not $allModulesPresent) {
    Write-Host "========================================" -ForegroundColor Magenta
    Write-Host "Fehlende Module" -ForegroundColor Magenta
    Write-Host "========================================" -ForegroundColor Magenta
    Write-Host ""
    
    Write-Host "WARNUNG: Einige erforderliche Qt6-Module fehlen!" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Loesungen:" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "1. Qt Maintenance Tool verwenden:" -ForegroundColor White
    
    $maintenanceTool = "C:\Qt\MaintenanceTool.exe"
    if (Test-Path $maintenanceTool) {
        Write-Host "   $maintenanceTool" -ForegroundColor Green
    } else {
        Write-Host "   Start -> Qt Maintenance Tool" -ForegroundColor Gray
    }
    Write-Host ""
    Write-Host "2. Module installieren:" -ForegroundColor White
    Write-Host "   - Qt 6.x for Desktop (MSVC 2022 64-bit)" -ForegroundColor Gray
    Write-Host "   - Qt 6.x Additional Libraries" -ForegroundColor Gray
    Write-Host ""
    Write-Host "3. Neuinstallation:" -ForegroundColor White
    Write-Host "   https://www.qt.io/download-qt-installer" -ForegroundColor Gray
    Write-Host ""
}

# ============================================================================
# 6. Installation Option
# ============================================================================

if ($Install) {
    Write-Host "========================================" -ForegroundColor Magenta
    Write-Host "Qt6 Installation" -ForegroundColor Magenta
    Write-Host "========================================" -ForegroundColor Magenta
    Write-Host ""
    
    Write-Host "Oeffne Qt Online Installer Download-Seite..." -ForegroundColor Cyan
    Start-Process "https://www.qt.io/download-qt-installer"
    
    Write-Host ""
    Write-Host "Installationsschritte:" -ForegroundColor Cyan
    Write-Host "  1. Qt Online Installer herunterladen" -ForegroundColor White
    Write-Host "  2. Installer ausfuehren" -ForegroundColor White
    Write-Host "  3. Qt Account erstellen (kostenlos)" -ForegroundColor White
    Write-Host "  4. Bei Komponenten waehlen:" -ForegroundColor White
    Write-Host "     - Qt 6.8.0 (oder neuer)" -ForegroundColor Gray
    Write-Host "     - MSVC 2022 64-bit" -ForegroundColor Gray
    Write-Host "     - Qt 6.x Additional Libraries" -ForegroundColor Gray
    Write-Host "     - Developer and Designer Tools" -ForegroundColor Gray
    Write-Host "  5. Installation nach C:\Qt\" -ForegroundColor White
    Write-Host ""
}

# ============================================================================
# 7. Zusammenfassung
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Zusammenfassung" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

if ($foundInstallations.Count -gt 0 -and $allModulesPresent) {
    Write-Host "OK Qt6 ist korrekt installiert!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Zum Bauen verwenden Sie:" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "  .\build-windows.ps1" -ForegroundColor Green
    Write-Host ""
    Write-Host "Oder manuell:" -ForegroundColor Cyan
    Write-Host "  cmake .. -DCMAKE_PREFIX_PATH=`"$selectedQt`"" -ForegroundColor Gray
    Write-Host "  nmake" -ForegroundColor Gray
    Write-Host ""
    
} elseif ($foundInstallations.Count -gt 0 -and -not $allModulesPresent) {
    Write-Host "WARNUNG: Qt6 gefunden, aber Module fehlen!" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Bitte installieren Sie fehlende Module mit:" -ForegroundColor Cyan
    Write-Host "  Qt Maintenance Tool" -ForegroundColor White
    Write-Host ""
    
} else {
    Write-Host "FEHLER: Qt6 nicht installiert!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Installation starten:" -ForegroundColor Cyan
    Write-Host "  .\check-qt6.ps1 -Install" -ForegroundColor Green
    Write-Host ""
}

# ============================================================================
# 8. Quick Fix Script
# ============================================================================

if ($foundInstallations.Count -gt 0) {
    $quickFixContent = @"
# Quick Fix: Qt6 in CMake verwenden

`$env:CMAKE_PREFIX_PATH = "$selectedQt"

Write-Host "CMAKE_PREFIX_PATH gesetzt auf: `$env:CMAKE_PREFIX_PATH" -ForegroundColor Green
Write-Host ""
Write-Host "Jetzt bauen mit:" -ForegroundColor Cyan
Write-Host "  cd build" -ForegroundColor White
Write-Host "  cmake .. -DCMAKE_PREFIX_PATH=`"`$env:CMAKE_PREFIX_PATH`"" -ForegroundColor White
Write-Host "  nmake" -ForegroundColor White
"@

    $quickFixContent | Out-File -FilePath "qt6-quickfix.ps1" -Encoding ASCII
    Write-Host "OK Quick-Fix-Script erstellt: qt6-quickfix.ps1" -ForegroundColor Green
    Write-Host ""
}

Write-Host ""
