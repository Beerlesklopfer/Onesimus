@echo off
REM Double-click launcher for deploy.ps1 (replaces run-build.bat and rebuild.bat).
REM   deploy.bat            build + windeployqt (deploy.ps1 all)
REM   deploy.bat rebuild    clean rebuild (the former rebuild.bat)
REM   deploy.bat msi        any other deploy.ps1 command and parameters
cd /d %~dp0
powershell.exe -NoProfile -ExecutionPolicy Bypass -File deploy.ps1 %*
