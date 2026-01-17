@echo off
REM Bacula Qt UI Build Script für Windows mit statischem OpenSSL
REM Voraussetzungen:
REM - Visual Studio 2019/2022 mit C++ Desktop Development
REM - Qt 6.x installiert
REM - Git
REM - Perl (Strawberry Perl empfohlen: https://strawberryperl.com/)
REM - NASM (optional, für optimiertes OpenSSL)

setlocal EnableDelayedExpansion

echo ========================================
echo Bacula Qt UI - Windows Build mit OpenSSL
echo ========================================
echo.

REM Git-Submodule initialisieren
echo Pruefe Git-Submodule...
if not exist "external\openssl\Configure" (
    echo OpenSSL Submodule nicht initialisiert. Initialisiere...
    git submodule update --init --recursive
    if errorlevel 1 (
        echo FEHLER: Git Submodule konnte nicht initialisiert werden!
        echo Bitte fuehren Sie manuell aus: git submodule update --init --recursive
        pause
        exit /b 1
    )
    echo Submodule erfolgreich initialisiert.
) else (
    echo OpenSSL Submodule bereits initialisiert.
)
echo.

REM Prüfe Perl
echo Pruefe Perl-Installation...
where perl >nul 2>&1
if errorlevel 1 (
    echo FEHLER: Perl nicht gefunden!
    echo Bitte installieren Sie Strawberry Perl: https://strawberryperl.com/
    pause
    exit /b 1
)
perl -v | findstr /C:"version"
echo.

REM Prüfe NMAKE (Visual Studio)
echo Pruefe Visual Studio Build Tools...
where nmake >nul 2>&1
if errorlevel 1 (
    echo FEHLER: NMAKE nicht gefunden!
    echo.
    echo Bitte oeffnen Sie die "Visual Studio Developer Command Prompt"
    echo oder fuehren Sie vcvarsall.bat aus:
    echo.
    echo Beispiel (VS 2022):
    echo "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    echo.
    pause
    exit /b 1
)
echo Visual Studio Build Tools gefunden.
echo.

REM Prüfe CMake
echo Pruefe CMake...
where cmake >nul 2>&1
if errorlevel 1 (
    echo FEHLER: CMake nicht gefunden!
    echo Bitte installieren Sie CMake: https://cmake.org/download/
    pause
    exit /b 1
)
cmake --version
echo.

REM Qt-Installation finden
echo Suche Qt-Installation...
set QT_DIR=
set QT_PATHS=C:\Qt\6.10.0\msvc2022_64;C:\Qt\6.9.0\msvc2022_64;C:\Qt\6.8.0\msvc2022_64;C:\Qt\6.7.0\msvc2022_64

for %%P in (%QT_PATHS%) do (
    if exist "%%P\bin\qmake.exe" (
        set "QT_DIR=%%P"
        goto :qt_found
    )
)

echo WARNUNG: Qt wurde nicht automatisch gefunden.
set /p QT_DIR="Bitte geben Sie den Qt-Pfad ein (z.B. C:\Qt\6.10.0\msvc2022_64): "

:qt_found
if not exist "%QT_DIR%\bin\qmake.exe" (
    echo FEHLER: Qt nicht gefunden in: %QT_DIR%
    pause
    exit /b 1
)
echo Qt gefunden: %QT_DIR%
echo.

REM CMake Pfad zu Qt setzen
set CMAKE_PREFIX_PATH=%QT_DIR%

REM Build-Verzeichnis erstellen
if exist "build" (
    echo Build-Verzeichnis existiert bereits.
    set /p CLEAN="Moechten Sie es neu erstellen? (j/n): "
    if /i "!CLEAN!"=="j" (
        echo Loesche Build-Verzeichnis...
        rmdir /s /q build
    )
)

if not exist "build" (
    echo Erstelle Build-Verzeichnis...
    mkdir build
)

cd build

echo.
echo ========================================
echo CMake-Konfiguration
echo ========================================
echo.
echo Generator: NMake Makefiles
echo Qt: %QT_DIR%
echo OpenSSL: Statisch (Submodule)
echo.

REM CMake konfigurieren
cmake .. ^
    -G "NMake Makefiles" ^
    -DCMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH% ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DUSE_STATIC_OPENSSL=ON

if errorlevel 1 (
    echo.
    echo FEHLER: CMake-Konfiguration fehlgeschlagen!
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo Kompilierung
echo ========================================
echo.
echo Dies kann einige Minuten dauern (OpenSSL wird kompiliert)...
echo.

REM Kompilieren mit NMake
nmake

if errorlevel 1 (
    echo.
    echo FEHLER: Kompilierung fehlgeschlagen!
    cd ..
    pause
    exit /b 1
)

cd ..

echo.
echo ========================================
echo Build erfolgreich!
echo ========================================
echo.
echo Ausfuehrbare Datei: build\BaculaQtUI.exe
echo.
echo OpenSSL wurde statisch gelinkt - keine DLLs erforderlich!
echo.
echo Zum Ausfuehren:
echo   cd build
echo   BaculaQtUI.exe
echo.
pause
