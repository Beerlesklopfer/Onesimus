# Quick Fix: Qt6 in CMake verwenden
# Sucht automatisch nach der neuesten Qt6-Installation

$qtRootDirs = @("C:\Qt", "C:\Qt6")
$foundQt = $null

foreach ($qtRoot in $qtRootDirs) {
    if (Test-Path $qtRoot) {
        $versionDirs = Get-ChildItem -Path $qtRoot -Directory -ErrorAction SilentlyContinue |
                       Where-Object { $_.Name -match '^\d+\.\d+(\.\d+)?$' } |
                       Sort-Object { [version]$_.Name } -Descending

        foreach ($versionDir in $versionDirs) {
            $compilerDir = Get-ChildItem -Path $versionDir.FullName -Directory -ErrorAction SilentlyContinue |
                           Where-Object { $_.Name -like "msvc*64" -and $_.Name -notlike "*arm*" } |
                           Select-Object -First 1

            if ($compilerDir -and (Test-Path "$($compilerDir.FullName)\bin\qmake.exe")) {
                $foundQt = $compilerDir.FullName
                break
            }
        }
    }
    if ($foundQt) { break }
}

if (-not $foundQt) {
    Write-Host "FEHLER: Keine Qt6-Installation gefunden!" -ForegroundColor Red
    Write-Host "Gesucht in: $($qtRootDirs -join ', ')" -ForegroundColor Yellow
    exit 1
}

$env:CMAKE_PREFIX_PATH = $foundQt
[System.Environment]::SetEnvironmentVariable("CMAKE_PREFIX_PATH", $foundQt, "User")

Write-Host "CMAKE_PREFIX_PATH gesetzt auf: $env:CMAKE_PREFIX_PATH" -ForegroundColor Green
Write-Host "(Auch als User-Umgebungsvariable gespeichert)" -ForegroundColor Gray
Write-Host ""
Write-Host "Jetzt bauen mit:" -ForegroundColor Cyan
Write-Host "  .\build-windows.ps1" -ForegroundColor White
Write-Host ""
Write-Host "Oder manuell:" -ForegroundColor Cyan
Write-Host "  cd build" -ForegroundColor White
Write-Host "  cmake .. -DCMAKE_PREFIX_PATH=`"$env:CMAKE_PREFIX_PATH`"" -ForegroundColor White
Write-Host "  nmake" -ForegroundColor White
