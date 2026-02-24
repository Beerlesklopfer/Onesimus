@echo off
echo Configuring and building Onesimus...
cd /d %~dp0
powershell.exe -ExecutionPolicy Bypass -File build-windows.ps1 -Clean
