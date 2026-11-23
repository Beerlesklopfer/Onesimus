#Requires -Version 5.1
# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Jörg Bernau

<#
.SYNOPSIS
    Fails when a Windows package lacks the third-party notices.

.DESCRIPTION
    Every package must carry THIRD-PARTY-NOTICES.md, LICENSE and each licence
    text the notices link (LICENSES\<id>.txt, read from the notices of the
    source tree) next to onesimus.exe:
      *.msi  the File/Component/Directory tables of the installer database
             (Windows Installer COM object; nothing is installed)
      *.zip  the entries of the portable ZIP
    The Linux and macOS packages are checked by check-notices.sh.

.EXAMPLE
    scripts\ci\check-notices.ps1 build\Onesimus-0.3.0-windows-x86_64.msi build\deploy\Onesimus-0.3.0-Windows.zip
#>
param(
    [Parameter(Mandatory = $true, ValueFromRemainingArguments = $true)]
    [string[]]$Package
)

$ErrorActionPreference = 'Stop'
$RepoDir = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
$notices = Get-Content (Join-Path $RepoDir 'THIRD-PARTY-NOTICES.md') -Raw
$texts = @([regex]::Matches($notices, 'LICENSES/[A-Za-z0-9.+-]+\.txt') | ForEach-Object { $_.Value } | Sort-Object -Unique)
if ($texts.Count -eq 0) { throw 'THIRD-PARTY-NOTICES.md links no LICENSES/*.txt' }
$required = @('THIRD-PARTY-NOTICES.md', 'LICENSE') + $texts

function Get-MsiRows($db, $sql, $columns) {
    $view = $db.GetType().InvokeMember('OpenView', 'InvokeMethod', $null, $db, @($sql))
    $view.GetType().InvokeMember('Execute', 'InvokeMethod', $null, $view, $null) | Out-Null
    $rows = @()
    while ($true) {
        $rec = $view.GetType().InvokeMember('Fetch', 'InvokeMethod', $null, $view, $null)
        if ($null -eq $rec) { break }
        $row = @{}
        for ($i = 0; $i -lt $columns.Count; $i++) {
            $row[$columns[$i]] = $rec.GetType().InvokeMember('StringData', 'GetProperty', $null, $rec, @($i + 1))
        }
        $rows += $row
    }
    $view.GetType().InvokeMember('Close', 'InvokeMethod', $null, $view, $null) | Out-Null
    return $rows
}

# "SHORT~1.TXT|Long name.txt" -> "Long name.txt"
function Get-LongName($name) { ($name -split '\|')[-1] }

function Test-Msi($path) {
    $installer = New-Object -ComObject WindowsInstaller.Installer
    $db = $installer.GetType().InvokeMember('OpenDatabase', 'InvokeMethod', $null, $installer, @($path, 0))
    $files = Get-MsiRows $db 'SELECT `File`.`FileName`, `Component`.`Directory_` FROM `File`, `Component` WHERE `File`.`Component_` = `Component`.`Component`' @('FileName', 'Directory')
    $dirs = @{}
    foreach ($d in (Get-MsiRows $db 'SELECT `Directory`, `Directory_Parent`, `DefaultDir` FROM `Directory`' @('Directory', 'Parent', 'DefaultDir'))) {
        $dirs[$d.Directory] = $d
    }
    $exe = $files | Where-Object { (Get-LongName $_.FileName) -ieq 'onesimus.exe' } | Select-Object -First 1
    if (-not $exe) { throw "${path}: no onesimus.exe in the File table" }
    $exeDir = $exe.Directory
    $missing = @()
    foreach ($req in $required) {
        $parts = $req -split '/'
        $name = $parts[-1]
        $found = $files | Where-Object {
            if ((Get-LongName $_.FileName) -ine $name) { return $false }
            if ($parts.Count -eq 1) { return $_.Directory -eq $exeDir }
            $d = $dirs[$_.Directory]
            return ($d -and $d.Parent -eq $exeDir -and ((Get-LongName $d.DefaultDir) -ieq $parts[0]))
        }
        if (-not $found) { $missing += $req }
    }
    if ($missing.Count -gt 0) { throw "${path}: missing next to onesimus.exe: $($missing -join ', ')" }
    Write-Host "[check-notices] ok: $path ($($required.Count) files next to onesimus.exe)"
}

function Test-Zip($path) {
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    $zip = [System.IO.Compression.ZipFile]::OpenRead($path)
    try { $entries = @($zip.Entries | ForEach-Object { $_.FullName -replace '\\', '/' }) } finally { $zip.Dispose() }
    $exe = $entries | Where-Object { $_ -match '(^|/)onesimus\.exe$' } | Select-Object -First 1
    if (-not $exe) { throw "${path}: no onesimus.exe" }
    $prefix = if ($exe.Contains('/')) { $exe.Substring(0, $exe.LastIndexOf('/') + 1) } else { '' }
    $missing = @($required | Where-Object { $entries -notcontains ($prefix + $_) })
    if ($missing.Count -gt 0) { throw "${path}: missing next to onesimus.exe: $($missing -join ', ')" }
    Write-Host "[check-notices] ok: $path ($($required.Count) files in '$prefix')"
}

foreach ($p in $Package) {
    $full = (Resolve-Path $p).Path
    switch -Regex ($full) {
        '\.msi$' { Test-Msi $full }
        '\.zip$' { Test-Zip $full }
        default  { throw "unknown package type: $p" }
    }
}
