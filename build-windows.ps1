#Requires -Version 5.1
# Onesimus Build Script fuer Windows

param(
    [switch]$Clean,
    [string]$QtPath,
    [ValidateSet('Release', 'Debug', 'RelWithDebInfo')]
    [string]$BuildType = 'Release',
    [ValidateSet('BACULA', 'BAREOS', 'BOTH')]
    [string]$BackupSystem = 'BAREOS',
    [ValidateSet('Ninja', 'NMake')]
    [string]$Generator = 'Ninja'
)

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Onesimus - Windows Build" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""
Write-Host "Build-Typ: $BuildType" -ForegroundColor Cyan
Write-Host "Backup-System: $BackupSystem" -ForegroundColor Cyan
Write-Host "PowerShell Version: $($PSVersionTable.PSVersion)" -ForegroundColor Cyan
Write-Host ""

# ============================================================================
# 0. Visual Studio Entwicklungsumgebung laden
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Visual Studio Umgebung" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

# Pruefe ob VS-Umgebung bereits geladen ist
if (-not $env:VSINSTALLDIR) {
    $vsDevShellScript = Join-Path $PSScriptRoot "Import-VsDevShell.ps1"

    if (Test-Path $vsDevShellScript) {
        Write-Host "Lade Visual Studio Entwicklungsumgebung..." -ForegroundColor Cyan
        try {
            & $vsDevShellScript -Architecture x64

            if ($LASTEXITCODE -ne 0) {
                throw "Import-VsDevShell.ps1 ist fehlgeschlagen"
            }
        } catch {
            Write-Host ""
            Write-Host "FEHLER: Visual Studio Umgebung konnte nicht geladen werden!" -ForegroundColor Red
            Write-Host $_.Exception.Message -ForegroundColor Red
            Write-Host ""
            Write-Host "Alternativen:" -ForegroundColor Yellow
            Write-Host "  1. Fuehren Sie manuell aus: .\Import-VsDevShell.ps1" -ForegroundColor White
            Write-Host "  2. Oeffnen Sie: Developer Command Prompt for VS 2022" -ForegroundColor White
            Write-Host ""
            exit 1
        }
    } else {
        Write-Host "WARNUNG: Import-VsDevShell.ps1 nicht gefunden" -ForegroundColor Yellow
        Write-Host "         Hoffe, dass VS-Umgebung bereits geladen ist..." -ForegroundColor Yellow
    }
} else {
    Write-Host "OK Visual Studio Umgebung bereits geladen" -ForegroundColor Green
    Write-Host "   VSINSTALLDIR: $env:VSINSTALLDIR" -ForegroundColor Gray
}

Write-Host ""

# ============================================================================
# 1. Git-Submodule
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Git-Submodule" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

if (-not (Test-Path "external\openssl\Configure")) {
    Write-Host "OpenSSL Submodule nicht vorhanden - wird initialisiert..." -ForegroundColor Cyan
    
    try {
        $output = git submodule update --init --recursive 2>&1
        if ($LASTEXITCODE -eq 0) {
            Write-Host "OK OpenSSL Submodule heruntergeladen" -ForegroundColor Green
        } else {
            throw "Git Submodule Update fehlgeschlagen"
        }
    } catch {
        Write-Host "FEHLER: Git Submodule konnte nicht initialisiert werden!" -ForegroundColor Red
        Write-Host ""
        Write-Host "Bitte fuehren Sie manuell aus:" -ForegroundColor Yellow
        Write-Host "  git submodule update --init --recursive" -ForegroundColor White
        exit 1
    }
} else {
    Write-Host "OK OpenSSL Submodule bereits vorhanden" -ForegroundColor Green
}
Write-Host ""

# ============================================================================
# 2. Voraussetzungen
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Voraussetzungen" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

# Git
if (Get-Command git -ErrorAction SilentlyContinue) {
    $gitVersion = (git --version) -replace "git version ", ""
    Write-Host "OK Git: $gitVersion" -ForegroundColor Green
} else {
    Write-Host "FEHLER: Git nicht gefunden!" -ForegroundColor Red
    exit 1
}

# Perl
if (Get-Command perl -ErrorAction SilentlyContinue) {
    $perlVersion = (perl -v | Select-String "version" | Select-Object -First 1).ToString().Trim()
    Write-Host "OK Perl: $perlVersion" -ForegroundColor Green
} else {
    Write-Host "FEHLER: Perl nicht gefunden!" -ForegroundColor Red
    Write-Host "Installieren Sie Strawberry Perl: https://strawberryperl.com/" -ForegroundColor Yellow
    exit 1
}

# CMake
if (Get-Command cmake -ErrorAction SilentlyContinue) {
    $cmakeVersion = (cmake --version | Select-Object -First 1) -replace "cmake version ", ""
    Write-Host "OK CMake: $cmakeVersion" -ForegroundColor Green
} else {
    Write-Host "FEHLER: CMake nicht gefunden!" -ForegroundColor Red
    exit 1
}

# Build Generator
if ($Generator -eq 'Ninja') {
    if (Get-Command ninja -ErrorAction SilentlyContinue) {
        Write-Host "OK Ninja: Verfuegbar" -ForegroundColor Green
    } else {
        Write-Host "FEHLER: Ninja nicht gefunden!" -ForegroundColor Red
        exit 1
    }
} else {
    if (Get-Command nmake -ErrorAction SilentlyContinue) {
        Write-Host "OK NMake: Verfuegbar" -ForegroundColor Green
    } else {
        Write-Host "FEHLER: NMake nicht gefunden!" -ForegroundColor Red
        Write-Host ""
        Write-Host "Visual Studio Build Tools sind nicht verfuegbar!" -ForegroundColor Yellow
        Write-Host ""
        Write-Host "Moegliche Loesungen:" -ForegroundColor Yellow
        Write-Host "  1. Installieren Sie Visual Studio 2022 (Community, Professional oder Enterprise)" -ForegroundColor White
        Write-Host "  2. Installieren Sie Visual Studio Build Tools 2022" -ForegroundColor White
        Write-Host "  3. Stellen Sie sicher, dass C++ Build Tools installiert sind" -ForegroundColor White
        Write-Host ""
        exit 1
    }
}

# CL (C++ Compiler)
if (Get-Command cl -ErrorAction SilentlyContinue) {
    $clVersion = (cl 2>&1 | Select-Object -First 1).ToString()
    if ($clVersion -match "Version\s+([\d\.]+)") {
        Write-Host "OK MSVC Compiler: Version $($matches[1])" -ForegroundColor Green
    } else {
        Write-Host "OK MSVC Compiler: Verfuegbar" -ForegroundColor Green
    }
} else {
    Write-Host "WARNUNG: C++ Compiler (cl.exe) nicht gefunden" -ForegroundColor Yellow
}

Write-Host ""

# ============================================================================
# 3. Qt6-Installation
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Qt6-Installation" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

$foundQt = $null

# Wenn QtPath als Parameter uebergeben wurde, verwende diesen
if ($QtPath -and (Test-Path "$QtPath\bin\qmake.exe")) {
    $foundQt = $QtPath
    Write-Host "OK Qt gefunden (Parameter): $foundQt" -ForegroundColor Green
} else {
    # Dynamisch nach Qt-Installationen suchen
    $qtRootDirs = @("C:\Qt", "C:\Qt6")
    $qtInstallations = @()

    foreach ($qtRoot in $qtRootDirs) {
        if (Test-Path $qtRoot) {
            # Suche nach Versionsverzeichnissen (z.B. 6.10.1, 6.9.0, etc.)
            $versionDirs = Get-ChildItem -Path $qtRoot -Directory -ErrorAction SilentlyContinue |
                           Where-Object { $_.Name -match '^\d+\.\d+(\.\d+)?$' }

            foreach ($versionDir in $versionDirs) {
                # Suche nach Compiler-Verzeichnissen (msvc2022_64, etc.)
                $compilerDirs = Get-ChildItem -Path $versionDir.FullName -Directory -ErrorAction SilentlyContinue |
                               Where-Object { $_.Name -like "msvc*64" }

                foreach ($compilerDir in $compilerDirs) {
                    $path = $compilerDir.FullName
                    $qmakeExe = Join-Path $path "bin\qmake.exe"
                    $qt6ConfigPath = Join-Path $path "lib\cmake\Qt6\Qt6Config.cmake"

                    if ((Test-Path $qmakeExe) -and (Test-Path $qt6ConfigPath)) {
                        $version = & $qmakeExe -query QT_VERSION
                        $qtInstallations += @{
                            Path = $path
                            Version = $version
                            Compiler = $compilerDir.Name
                        }
                    }
                }
            }
        }
    }

    # Waehle die neueste Qt-Version
    if ($qtInstallations.Count -gt 0) {
        $selectedQt = $qtInstallations | Sort-Object { [version]$_.Version } -Descending | Select-Object -First 1
        $foundQt = $selectedQt.Path
        Write-Host "OK Qt gefunden (automatisch): $foundQt" -ForegroundColor Green
        Write-Host "   Version: $($selectedQt.Version), Compiler: $($selectedQt.Compiler)" -ForegroundColor Gray
    }
}

if (-not $foundQt) {
    Write-Host "FEHLER: Qt6 wurde nicht automatisch gefunden!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Loesungen:" -ForegroundColor Yellow
    Write-Host "  1. Fuehren Sie aus: .\check-qt6.ps1" -ForegroundColor White
    Write-Host "  2. Installieren Sie Qt6: https://www.qt.io/download-qt-installer" -ForegroundColor White
    Write-Host ""
    $manualPath = Read-Host "Oder geben Sie den Qt-Pfad ein (z.B. C:\Qt\6.8.0\msvc2022_64)"
    
    if (Test-Path "$manualPath\bin\qmake.exe") {
        $foundQt = $manualPath
        Write-Host "OK Qt gefunden: $foundQt" -ForegroundColor Green
    } else {
        Write-Host "FEHLER: Qt nicht gefunden in: $manualPath" -ForegroundColor Red
        exit 1
    }
}

# Qt-Version
$qtVersion = & "$foundQt\bin\qmake.exe" -query QT_VERSION
Write-Host "   Qt Version: $qtVersion" -ForegroundColor Cyan

# Pruefe Qt6Config.cmake
$qt6ConfigPath = "$foundQt\lib\cmake\Qt6\Qt6Config.cmake"
if (Test-Path $qt6ConfigPath) {
    Write-Host "OK Qt6Config.cmake vorhanden" -ForegroundColor Green
} else {
    Write-Host ""
    Write-Host "FEHLER: Qt6Config.cmake nicht gefunden!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Pfad: $qt6ConfigPath" -ForegroundColor Gray
    Write-Host ""
    Write-Host "Qt6 ist installiert, aber CMake-Konfigurationsdateien fehlen!" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Loesungen:" -ForegroundColor Yellow
    Write-Host "  1. Diagnose: .\check-qt6.ps1" -ForegroundColor White
    Write-Host "  2. Qt Maintenance Tool -> Module nachinstallieren" -ForegroundColor White
    Write-Host "     - Qt 6.x Additional Libraries" -ForegroundColor Gray
    Write-Host "  3. Qt6 neu installieren" -ForegroundColor White
    Write-Host ""
    exit 1
}

Write-Host ""

# ============================================================================
# 4. Build-Verzeichnis
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Build-Verzeichnis" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

if (Test-Path "build") {
    if ($Clean) {
        Write-Host "Loesche bestehendes Build-Verzeichnis..." -ForegroundColor Cyan
        Remove-Item -Path "build" -Recurse -Force
        Write-Host "OK Build-Verzeichnis geloescht" -ForegroundColor Green
    } else {
        Write-Host "   Build-Verzeichnis existiert (verwende -Clean zum Neuerstellen)" -ForegroundColor Cyan
    }
}

if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
    Write-Host "OK Build-Verzeichnis erstellt" -ForegroundColor Green
}

Write-Host ""

# ============================================================================
# 5. CMake-Konfiguration
# ============================================================================

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "CMake-Konfiguration" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

Write-Host "   Generator: $Generator" -ForegroundColor Cyan
Write-Host "   Qt: $foundQt" -ForegroundColor Cyan
Write-Host "   Build-Typ: $BuildType" -ForegroundColor Cyan
Write-Host "   Backup-System: $BackupSystem" -ForegroundColor Cyan
Write-Host "   OpenSSL: Statisch (automatischer Download)" -ForegroundColor Cyan
Write-Host ""

Push-Location "build"

try {
    $generatorName = if ($Generator -eq 'Ninja') { 'Ninja' } else { 'NMake Makefiles' }
    $cmakeArgs = @(
        "..",
        "-G", $generatorName,
        "-DCMAKE_PREFIX_PATH=$foundQt",
        "-DCMAKE_BUILD_TYPE=$BuildType",
        "-DUSE_STATIC_OPENSSL=ON",
        "-DBACKUP_SYSTEM=$BackupSystem"
    )
    
    Write-Host "Fuehre CMake aus..." -ForegroundColor Cyan
    & cmake $cmakeArgs
    
    if ($LASTEXITCODE -ne 0) {
        throw "CMake-Konfiguration fehlgeschlagen"
    }
    
    Write-Host ""
    Write-Host "OK CMake-Konfiguration erfolgreich" -ForegroundColor Green
    
} catch {
    Write-Host ""
    Write-Host "FEHLER: CMake-Konfiguration fehlgeschlagen!" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    Pop-Location
    exit 1
}

# ============================================================================
# 6. Kompilierung
# ============================================================================

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Kompilierung" -ForegroundColor Magenta
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

Write-Host "HINWEIS: Beim ersten Build wird OpenSSL heruntergeladen und kompiliert." -ForegroundColor Yellow
Write-Host "         Dies kann 5-10 Minuten dauern..." -ForegroundColor Yellow
Write-Host ""

$startTime = Get-Date

try {
    if ($Generator -eq 'Ninja') {
        Write-Host "Starte Ninja..." -ForegroundColor Cyan
        & ninja
    } else {
        Write-Host "Starte NMake..." -ForegroundColor Cyan
        & nmake
    }
    
    if ($LASTEXITCODE -ne 0) {
        throw "Kompilierung fehlgeschlagen"
    }
    
    $endTime = Get-Date
    $duration = $endTime - $startTime
    
    Write-Host ""
    Write-Host "OK Kompilierung erfolgreich!" -ForegroundColor Green
    Write-Host "   Build-Zeit: $($duration.ToString('mm\:ss')) Minuten" -ForegroundColor Cyan
    
} catch {
    Write-Host ""
    Write-Host "FEHLER: Kompilierung fehlgeschlagen!" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    Pop-Location
    exit 1
}

Pop-Location

# ============================================================================
# 7. Zusammenfassung
# ============================================================================

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "Build erfolgreich abgeschlossen!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "OK Ausfuehrbare Datei: build\Onesimus.exe" -ForegroundColor Green
Write-Host ""
Write-Host "   OpenSSL wurde statisch gelinkt - keine DLLs erforderlich!" -ForegroundColor Cyan
Write-Host ""

Write-Host "Naechste Schritte:" -ForegroundColor Cyan
Write-Host ""
Write-Host "  Ausfuehren:" -ForegroundColor White
Write-Host "    cd build" -ForegroundColor Gray
Write-Host "    .\Onesimus.exe" -ForegroundColor Gray
Write-Host ""
Write-Host "  Deployment vorbereiten:" -ForegroundColor White
Write-Host "    cd build" -ForegroundColor Gray
Write-Host "    windeployqt Onesimus.exe" -ForegroundColor Gray
Write-Host ""

# Binary-Informationen
if (Test-Path "build\Onesimus.exe") {
    $fileInfo = Get-Item "build\Onesimus.exe"
    $fileSizeMB = [math]::Round($fileInfo.Length / 1MB, 2)
    
    Write-Host "   Binary-Groesse: $fileSizeMB MB" -ForegroundColor Cyan
    Write-Host "   Erstellt: $($fileInfo.LastWriteTime)" -ForegroundColor Cyan
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""
