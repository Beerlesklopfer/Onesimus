# Windows Build und Deploy mit PowerShell

## 🚀 Schnellstart

### Einfach - Normale PowerShell (Empfohlen!)

Das Script richtet die Visual Studio Umgebung **automatisch** ein!

1. **PowerShell öffnen** (im Projektverzeichnis)
2. **Script ausführen:**
   ```powershell
   .\deploy.ps1
   ```

**Das war's!** ✨ Das Script lädt die VS-Umgebung automatisch (über
`Import-VsDevShell.ps1`, falls vorhanden, sonst über `vswhere`), baut
`onesimus.exe` und legt mit windeployqt alle Qt- und OpenSSL-DLLs daneben in
`build\deploy\` – von dort ist das Programm lauffähig.

Zum Doppelklicken gibt es `deploy.bat` (dasselbe wie `.\deploy.ps1`;
`deploy.bat rebuild` baut sauber neu, weitere Befehle werden durchgereicht).

### Alternative: Developer PowerShell

Falls gewünscht, können Sie auch direkt eine Developer PowerShell verwenden:

1. **Öffnen Sie:** `Start → Visual Studio 2022 → Developer PowerShell for VS 2022`
2. **Script ausführen:**
   ```powershell
   .\deploy.ps1
   ```

## 📦 Voraussetzungen

- Visual Studio 2022 (oder Build Tools) mit „Desktop development with C++“
- Qt ≥ 6.8 für `msvc2022_64` mit dem Modul **Qt GRPC** (`qtgrpc`: QtGrpc und
  QtProtobuf; die CI nimmt Qt 6.8.3)
- `protoc` (Protobuf-Compiler) im `PATH`, für `qt_add_protobuf`/`qt_add_grpc`
  (die CI nimmt protoc 28.3)
- CMake ≥ 3.25, Ninja, Git
- Strawberry Perl und NASM für den OpenSSL-Build

Build-Verzeichnis und Generator sind überall dieselben: `build\` mit Ninja
(`deploy.ps1`, `deploy.bat` und die Presets `windows-*` in `CMakePresets.json`).
Die Presets brauchen eine Developer PowerShell (cl.exe im `PATH`);
`deploy.ps1` lädt die Umgebung selbst. `-Generator NMake` gibt es weiterhin.

## 🧭 Befehle

`.\deploy.ps1 [Befehl] [Parameter]` – ohne Befehl gilt `all`.

| Befehl | Wirkung |
|---|---|
| `all`, `onesimus`, `gui` | Bauen + windeployqt nach `build\deploy\` (Standard) |
| `configure` | Nur CMake-Konfiguration |
| `build` / `rebuild` | Bauen / sauber neu bauen |
| `deploy` | Nur windeployqt + OpenSSL-DLLs nach `build\deploy\` |
| `run` | Bauen, deployen, `build\deploy\onesimus.exe` starten |
| `test` | Bauen + ctest |
| `installer` (`package`, `msi`) | MSI-Installer über `package-msi.ps1` (WiX v6) |
| `zip` | Portables ZIP über `scripts\windows\package-windows.ps1` |
| `install` | `cmake --install` nach `-Prefix` (Standard `build\stage`) |
| `status` | Gebautes Binary, Version, Installer |
| `clean` | Build-Verzeichnis löschen |
| `help` | `Get-Help .\deploy.ps1 -Detailed` |

Der Build schreibt `version.h` und die Build-Nummer nur ins Build-Verzeichnis
(`build\generated\`), nicht in den Quellbaum. Die Build-Nummer ist in der CI
`GITHUB_RUN_NUMBER`, sonst die Zahl der Commits; `-DONESIMUS_INCREMENT_BUILD=ON`
zählt sie stattdessen bei jedem Build hoch.

## 📋 Parameter

### -Clean
Löscht das Build-Verzeichnis vor dem Build

```powershell
.\deploy.ps1 -Clean
```

### -BuildType
Wählt den Build-Typ: `Release` (Standard), `Debug`, `RelWithDebInfo` oder `MinSizeRel`

```powershell
.\deploy.ps1 -BuildType Debug
.\deploy.ps1 -BuildType Release
```

**Hinweis**: Ein ZIP-Package entsteht mit `.\deploy.ps1 zip`.

### -Generator
Wählt das Build-System: `Ninja` (Standard, schneller) oder `NMake`

```powershell
.\deploy.ps1 -Generator Ninja   # Standard, empfohlen
.\deploy.ps1 -Generator NMake   # Klassisch
```

**Ninja** ist deutlich schneller als NMake, besonders bei inkrementellen Builds!

### -QtPath
Gibt den Qt-Pfad manuell an (überspringt automatische Suche)

```powershell
.\deploy.ps1 -QtPath "C:\Qt\6.10.0\msvc2022_64"
```

### -BackupSystem
Wird als `BACKUP_SYSTEM` an CMake übergeben. Derzeit baut nur `BAREOS` (Standard).

### -BuildDir, -Jobs, -Prefix, -Reconfigure, -Version, -DryRun
Build-Verzeichnis (Standard `build`), parallele Jobs, Ziel von `install`,
frische CMake-Konfiguration, Version für `zip` (Standard: aus `build\generated\version.txt`) und
Probelauf: `-DryRun` zeigt jeden Befehl, ohne ihn auszuführen.

### Kombiniert

```powershell
.\deploy.ps1 -Clean -BuildType Debug -Generator Ninja -QtPath "C:\Qt\6.9.0\msvc2022_64"
```

## ✨ Features

### 🎯 Automatische Umgebung
- ✨ **Automatisches Laden der Visual Studio Umgebung** - Keine Developer Shell nötig!
- ✨ **Automatische Qt-Erkennung** - Findet Qt 6.6 bis 6.10
- ✨ **Flexibler Build-Generator** - Ninja (schnell) oder NMake (klassisch)

### Automatische Checks
- ✅ Git installiert?
- ✅ Perl installiert?
- ✅ CMake installiert?
- ✅ Visual Studio Umgebung aktiv?
- ✅ Qt-Installation gefunden?
- ✅ Build-Generator verfügbar? (Ninja/NMake)

### Farbige Ausgabe
- 🟢 **Grün:** Erfolgreich
- 🔴 **Rot:** Fehler
- 🟡 **Gelb:** Warnung
- 🔵 **Cyan:** Information
- 🟣 **Magenta:** Headers

### Automatische Qt-Suche
Sucht in:
- `C:\Qt\6.10.0\msvc2022_64`
- `C:\Qt\6.9.0\msvc2022_64`
- `C:\Qt\6.8.0\msvc2022_64`
- `C:\Qt\6.7.0\msvc2022_64`
- `C:\Qt\6.6.0\msvc2022_64`
- `C:\Qt6\*`

### Build-Zeit-Messung
Zeigt die Gesamtdauer des Builds an

### Binary-Informationen
- Dateigröße in MB
- Erstellungsdatum

## 🐛 Fehlerbehebung

### "cannot be loaded because running scripts is disabled"

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
```

### "Ninja/NMAKE nicht gefunden"

Sie befinden sich nicht in einer Visual Studio Umgebung, oder der gewählte Generator ist nicht installiert.

**Lösung 1:** Developer Command Prompt verwenden
**Lösung 2:** vcvarsall.bat ausführen (siehe Option 3 oben)
**Lösung 3:** Ninja installieren (schneller): https://github.com/ninja-build/ninja/releases
**Lösung 4:** Anderen Generator verwenden: `.\deploy.ps1 -Generator NMake`

### "Qt nicht gefunden"

Das Script fragt nach dem Pfad. Geben Sie ein:
```
C:\Qt\6.10.0\msvc2022_64
```

Oder verwenden Sie den `-QtPath` Parameter.

### "Perl nicht gefunden"

Installieren Sie Strawberry Perl:
https://strawberryperl.com/

## 📊 Beispiel-Ausgabe

```
========================================
Bacula Qt UI - Windows Build
========================================

Build-Typ: Release
PowerShell Version: 5.1.19041.5247

========================================
Git-Submodule
========================================

✓ OpenSSL Submodule bereits vorhanden

========================================
Voraussetzungen
========================================

✓ Git: 2.47.1
✓ Perl: This is perl 5, version 40, subversion 2
✓ CMake: 3.28.1
✓ NMAKE: Verfügbar

========================================
Qt-Installation
========================================

✓ Qt gefunden (automatisch): C:\Qt\6.10.0\msvc2022_64
ℹ Qt Version: 6.10.0

========================================
Build-Verzeichnis
========================================

✓ Build-Verzeichnis erstellt

========================================
CMake-Konfiguration
========================================

ℹ Generator: Ninja
ℹ Qt: C:\Qt\6.10.0\msvc2022_64
ℹ Build-Typ: Release
ℹ OpenSSL: Statisch (automatischer Download)

...

✓ CMake-Konfiguration erfolgreich

========================================
Kompilierung
========================================

⚠ Beim ersten Build wird OpenSSL heruntergeladen und kompiliert.
⚠ Dies kann 5-10 Minuten dauern...

...

✓ Kompilierung erfolgreich!
ℹ Build-Zeit: 07:23 Minuten

========================================
Build erfolgreich abgeschlossen!
========================================

✓ Ausführbare Datei: build\BaculaQtUI.exe

ℹ OpenSSL wurde statisch gelinkt - keine externen DLLs erforderlich!

Nächste Schritte:

  Ausführen:
    cd build
    .\BaculaQtUI.exe

  Deployment vorbereiten:
    cd build
    windeployqt BaculaQtUI.exe

ℹ Binary-Größe: 18.45 MB
ℹ Erstellt: 17.01.2026 20:45:23
```

## 🎯 Vorteile gegenüber Batch

✅ **Bessere Fehlerbehandlung** - Try/Catch  
✅ **Farbige Ausgabe** - Übersichtlicher  
✅ **Parameter-Support** - Flexible Optionen  
✅ **Moderne Syntax** - Lesbar und wartbar  
✅ **Objekt-orientiert** - PowerShell-Objekte  
✅ **Integrierte Funktionen** - Test-Path, Get-Item, etc.  
✅ **Bessere Strings** - Keine Escape-Probleme  
✅ **Build-Zeit-Messung** - Automatisch  
✅ **Help-System** - `Get-Help .\deploy.ps1`

## 📚 Hilfe anzeigen

```powershell
Get-Help .\deploy.ps1
Get-Help .\deploy.ps1 -Detailed
Get-Help .\deploy.ps1 -Examples
```

## 🔧 Erweiterte Verwendung

### Nur CMake-Konfiguration

**Mit Ninja (empfohlen):**
```powershell
cd build
cmake .. -G Ninja -DCMAKE_PREFIX_PATH=C:\Qt\6.10.0\msvc2022_64
```

**Mit NMake:**
```powershell
cd build
cmake .. -G "NMake Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\6.10.0\msvc2022_64
```

### Nur Kompilierung (nach CMake)

**Mit Ninja:**
```powershell
cd build
ninja
```

**Mit NMake:**
```powershell
cd build
nmake
```

### Rebuild (Clean + Build)

```powershell
.\deploy.ps1 -Clean
```

### Debug-Build für Entwicklung

```powershell
.\deploy.ps1 -Clean -BuildType Debug
```

### Release-Build mit Package

```powershell
.\deploy.ps1 zip -BuildType Release
```

**Erstellt**:
- `build\deploy\onesimus.exe` - Executable
- `build\deploy\*.dll` - Qt- und OpenSSL-DLLs (via windeployqt)
- `build\deploy\Onesimus-x.x.x.x-Windows.zip` - Portable Package

Das ZIP-Package enthält alle notwendigen Dateien und kann ohne Installation verwendet werden.

## 📦 Packaging

### Packaging

```powershell
.\deploy.ps1 zip         # portables ZIP: build\deploy\Onesimus-x.x.x.x-Windows.zip
.\deploy.ps1 installer   # MSI-Installer über WiX v6 (Alias: msi, package)
```

### MSI-Installer

Quelle: `packaging\windows\onesimus.wxs`, Einstellungen (UpgradeCode, Dienstname,
Ports, Verzeichnisse von ofd) in `packaging\windows\config.wxi`. Voraussetzung:

```powershell
dotnet tool install --global wix --version 6.0.2
```

Features (still wählbar mit `ADDLOCAL=...`):

| Feature | Inhalt | Standard |
|---|---|---|
| `Gui` | Onesimus mit Qt-DLLs, Startmenü-Eintrag (Pflicht) | an |
| `DesktopShortcut` | Desktop-Verknüpfung | an |
| `Oconsole` | `bin\oconsole.exe`, `bin\` im System-PATH | an, wenn vorhanden |
| `Ofd` | `bin\ofd.exe` als Dienst `ofd` (Start manuell), Firewall TCP 9102/10102, `C:\ProgramData\Onesimus\bareos-fd.d\` und `log\` | aus |

oconsole und ofd stammen aus dem Windows-Build von onesimus-dird
(`onesimus-dird-<version>-windows-x86_64.zip` mit `bin\oconsole.exe`, `bin\ofd.exe`):
`.\deploy.ps1 installer -DirdZip <zip>` oder die Datei nach `dist\rust\` legen. Fehlt
sie, enthält das MSI nur die GUI. Die MSI-Version ist die GUI-Version; die Version
von onesimus-dird steht in der Property `ONESIMUS_DIRD_VERSION` und unter
`HKLM\Software\Bernau Family\Onesimus\DirdVersion`.

```powershell
msiexec /i Onesimus-1.0.0-windows-x86_64.msi ADDLOCAL=Gui,DesktopShortcut,Oconsole,Ofd /qn /l*v install.log
msiexec /x Onesimus-1.0.0-windows-x86_64.msi /qn    # entfernt auch Dienst und Firewall-Regeln
```

Die Bitmaps des Installers (`packaging\windows\branding\*.bmp`) erzeugt
`packaging/windows/make-branding.sh` aus `resources/icons/onesimus.svg`.

### Package-Inhalt

Das ZIP-Package enthält:
- `Onesimus.exe` - Hauptprogramm
- Qt6 DLLs (Core, Gui, Widgets, Network)
- Qt Plugins (platforms, styles, imageformats)
- Qt Übersetzungen (translations)
- `README.txt` - Installations- und Nutzungshinweise

**Keine Installation erforderlich** - Einfach entpacken und ausführen!
