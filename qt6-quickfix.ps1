# Quick Fix: Qt6 in CMake verwenden

$env:CMAKE_PREFIX_PATH = "C:\Qt\6.10.1\msvc2022_64"

Write-Host "CMAKE_PREFIX_PATH gesetzt auf: $env:CMAKE_PREFIX_PATH" -ForegroundColor Green
Write-Host ""
Write-Host "Jetzt bauen mit:" -ForegroundColor Cyan
Write-Host "  cd build" -ForegroundColor White
Write-Host "  cmake .. -DCMAKE_PREFIX_PATH="$env:CMAKE_PREFIX_PATH"" -ForegroundColor White
Write-Host "  nmake" -ForegroundColor White
