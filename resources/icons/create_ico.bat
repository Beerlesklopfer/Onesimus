@echo off
REM Create Windows ICO file from SVG using ImageMagick
REM Requires ImageMagick to be installed: https://imagemagick.org/

echo Creating Onesimus.ico from SVG...

REM Check if ImageMagick is available
where magick >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: ImageMagick not found. Please install it from https://imagemagick.org/
    echo.
    echo Alternative: Use an online converter like https://convertio.co/svg-ico/
    echo Upload onesimus.svg and download the .ico file
    pause
    exit /b 1
)

REM Create multi-resolution ICO (16, 32, 48, 64, 128, 256 pixels)
magick convert onesimus.svg -background transparent -define icon:auto-resize=256,128,64,48,32,16 onesimus.ico

if %ERRORLEVEL% EQU 0 (
    echo SUCCESS: onesimus.ico created!
) else (
    echo ERROR: Failed to create ico file
)

pause
