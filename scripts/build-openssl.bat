@echo off
REM Build OpenSSL for Onesimus
REM Run this from x64 Native Tools Command Prompt for VS 2022

echo ========================================
echo OpenSSL Build Script for Onesimus
echo ========================================

cd /d "%~dp0.."

REM Check if Strawberry Perl is available
where perl >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: Perl not found! Please install Strawberry Perl.
    exit /b 1
)

REM Check if NASM is available (optional but recommended)
where nasm >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo WARNING: NASM not found. Assembly optimizations will be disabled.
)

echo.
echo Building OpenSSL from external/openssl...
echo.

cd external\openssl

REM Configure OpenSSL
perl Configure VC-WIN64A shared no-tests no-apps no-docs ^
    --prefix="%~dp0..\BUILD\openssl-install" ^
    --openssldir="%~dp0..\BUILD\openssl-install\ssl"

if %ERRORLEVEL% neq 0 (
    echo ERROR: OpenSSL configuration failed!
    exit /b 1
)

REM Build with single-threaded nmake to avoid PDB conflicts
nmake /NOLOGO

if %ERRORLEVEL% neq 0 (
    echo ERROR: OpenSSL build failed!
    exit /b 1
)

REM Install
nmake /NOLOGO install_sw install_ssldirs

if %ERRORLEVEL% neq 0 (
    echo ERROR: OpenSSL install failed!
    exit /b 1
)

echo.
echo ========================================
echo OpenSSL built successfully!
echo Libraries installed to: BUILD\openssl-install
echo ========================================

cd /d "%~dp0.."
