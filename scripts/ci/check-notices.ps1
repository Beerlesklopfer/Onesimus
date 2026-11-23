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
      *.msi  an administrative extract (msiexec /a: the files in their
             installed layout, nothing is installed); on failure the
             File/Component/Directory tables are printed
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

# The installer tables, printed when the check fails: row counts and the
# first files with their directory chain (for the CI log).
function Show-MsiDiagnostics($path) {
    try {
        $installer = New-Object -ComObject WindowsInstaller.Installer
        $db = $installer.GetType().InvokeMember('OpenDatabase', 'InvokeMethod', $null, $installer, @($path, 0))
        $files = @(Get-MsiRows $db 'SELECT `File`, `FileName`, `Component_` FROM `File`' @('File', 'FileName', 'Component'))
        $comps = @(Get-MsiRows $db 'SELECT `Component`, `Directory_` FROM `Component`' @('Component', 'Directory'))
        $dirRows = @(Get-MsiRows $db 'SELECT `Directory`, `Directory_Parent`, `DefaultDir` FROM `Directory`' @('Directory', 'Parent', 'DefaultDir'))
        Write-Host "[check-notices] MSI tables: File $($files.Count), Component $($comps.Count), Directory $($dirRows.Count) rows"
        $compDir = @{}; foreach ($c in $comps) { $compDir[$c.Component] = $c.Directory }
        $dirs = @{}; foreach ($d in $dirRows) { $dirs[$d.Directory] = $d }
        foreach ($f in ($files | Select-Object -First 20)) {
            $chain = @()
            $id = $compDir[$f.Component]
            for ($n = 0; $id -and $n -lt 10; $n++) {
                $d = $dirs[$id]
                if (-not $d) { $chain += "?$id"; break }
                $chain = @("$id($(Get-LongName $d.DefaultDir))") + $chain
                $id = $d.Parent
            }
            Write-Host "  $($f.FileName)  <-  $($chain -join ' / ')"
        }
    } catch {
        Write-Host "[check-notices] could not read the MSI tables: $($_.Exception.Message)"
    }
}

# Administrative install (msiexec /a): unpacks the files of the MSI into a
# folder in the layout they would be installed in - no installation, no
# registry, no services; the files are then checked on disk next to
# onesimus.exe.
function Test-Msi($path) {
    $tmp = Join-Path ([System.IO.Path]::GetTempPath()) ('onesimus-msi-' + [guid]::NewGuid().ToString('N'))
    $log = "$tmp.log"
    New-Item -ItemType Directory -Force -Path $tmp | Out-Null
    try {
        $msiArgs = @('/a', "`"$path`"", '/qn', "TARGETDIR=`"$tmp`"", '/l*v', "`"$log`"")
        $proc = Start-Process -FilePath 'msiexec.exe' -ArgumentList $msiArgs -Wait -PassThru
        if ($proc.ExitCode -ne 0) {
            if (Test-Path $log) { Get-Content $log -Tail 40 | ForEach-Object { Write-Host "  $_" } }
            Show-MsiDiagnostics $path
            throw "${path}: administrative extract failed (msiexec exit code $($proc.ExitCode))"
        }
        $exe = Get-ChildItem -Path $tmp -Recurse -File -Filter 'onesimus.exe' | Select-Object -First 1
        if (-not $exe) {
            Write-Host "[check-notices] files extracted from the MSI (first 40):"
            Get-ChildItem -Path $tmp -Recurse -File | Select-Object -First 40 |
                ForEach-Object { Write-Host "  $($_.FullName.Substring($tmp.Length))" }
            Show-MsiDiagnostics $path
            throw "${path}: no onesimus.exe among the extracted files"
        }
        $dir = $exe.DirectoryName
        $missing = @($required | Where-Object {
            $f = Join-Path $dir ($_ -replace '/', '\')
            -not ((Test-Path -LiteralPath $f -PathType Leaf) -and ((Get-Item -LiteralPath $f).Length -gt 0))
        })
        if ($missing.Count -gt 0) {
            Write-Host "[check-notices] next to $($exe.FullName.Substring($tmp.Length)):"
            Get-ChildItem -Path $dir | ForEach-Object { Write-Host "  $($_.Name)" }
            Show-MsiDiagnostics $path
            throw "${path}: missing next to onesimus.exe: $($missing -join ', ')"
        }
        Write-Host "[check-notices] ok: $path ($($required.Count) files next to onesimus.exe, $($exe.DirectoryName.Substring($tmp.Length)))"
    } finally {
        Remove-Item -Recurse -Force -Path $tmp -ErrorAction SilentlyContinue
        Remove-Item -Force -Path $log -ErrorAction SilentlyContinue
    }
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
