# Windows Build mit PowerShell

## 🚀 Schnellstart

### Einfach - Normale PowerShell (Empfohlen!)

Das Script richtet die Visual Studio Umgebung **automatisch** ein!

1. **PowerShell öffnen** (im Projektverzeichnis)
2. **Script ausführen:**
   ```powershell
   .\build-windows.ps1
   ```

**Das war's!** ✨ Das Script lädt die VS-Umgebung automatisch.

### Alternative: Developer PowerShell

Falls gewünscht, können Sie auch direkt eine Developer PowerShell verwenden:

1. **Öffnen Sie:** `Start → Visual Studio 2022 → Developer PowerShell for VS 2022`
2. **Script ausführen:**
   ```powershell
   .\build-windows.ps1
   ```

## 📋 Parameter

### -Clean
Löscht das Build-Verzeichnis vor dem Build

```powershell
.\build-windows.ps1 -Clean
```

### -BuildType
Wählt den Build-Typ: `Release` (Standard), `Debug`, oder `RelWithDebInfo`

```powershell
.\build-windows.ps1 -BuildType Debug
.\build-windows.ps1 -BuildType Release
```

**Hinweis**: Release-Builds erstellen automatisch ein ZIP-Package!

### -Generator
Wählt das Build-System: `Ninja` (Standard, schneller) oder `NMake`

```powershell
.\build-windows.ps1 -Generator Ninja   # Standard, empfohlen
.\build-windows.ps1 -Generator NMake   # Klassisch
```

**Ninja** ist deutlich schneller als NMake, besonders bei inkrementellen Builds!

### -QtPath
Gibt den Qt-Pfad manuell an (überspringt automatische Suche)

```powershell
.\build-windows.ps1 -QtPath "C:\Qt\6.10.0\msvc2022_64"
```

### Kombiniert

```powershell
.\build-windows.ps1 -Clean -BuildType Debug -Generator Ninja -QtPath "C:\Qt\6.9.0\msvc2022_64"
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
**Lösung 4:** Anderen Generator verwenden: `.\build-windows.ps1 -Generator NMake`

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
✅ **Help-System** - `Get-Help .\build-windows.ps1`

## 📚 Hilfe anzeigen

```powershell
Get-Help .\build-windows.ps1
Get-Help .\build-windows.ps1 -Detailed
Get-Help .\build-windows.ps1 -Examples
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
.\build-windows.ps1 -Clean
```

### Debug-Build für Entwicklung

```powershell
.\build-windows.ps1 -Clean -BuildType Debug
```

### Release-Build mit Automatischem Package

```powershell
.\build-windows.ps1 -BuildType Release
```

**Automatisch erstellt**:
- `build\Onesimus.exe` - Executable
- `build\*.dll` - Qt DLLs (via windeployqt)
- `build\Onesimus-x.x.x.x-Windows.zip` - Portable Package

Das ZIP-Package enthält alle notwendigen Dateien und kann ohne Installation verwendet werden.

## 📦 Packaging

### Automatisches Packaging (Release-Build)

Release-Builds erstellen automatisch ein ZIP-Package:

```powershell
.\build-windows.ps1 -BuildType Release
```

Erstellt: `build\Onesimus-x.x.x.x-Windows.zip`

### Manuelles Packaging

```powershell
# Nach dem Build
cd build
nmake package
```

### Package-Inhalt

Das ZIP-Package enthält:
- `Onesimus.exe` - Hauptprogramm
- Qt6 DLLs (Core, Gui, Widgets, Network)
- Qt Plugins (platforms, styles, imageformats)
- Qt Übersetzungen (translations)
- `README.txt` - Installations- und Nutzungshinweise

**Keine Installation erforderlich** - Einfach entpacken und ausführen!
