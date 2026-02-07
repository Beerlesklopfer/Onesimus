@echo off
echo Configuring and building Onesimus...
cd /d %~dp0
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cd build
cmake .. -G "NMake Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\6.10.1\msvc2022_64 -DCMAKE_BUILD_TYPE=Release
nmake
