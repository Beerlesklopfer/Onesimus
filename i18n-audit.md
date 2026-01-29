# Internationalization (i18n) Audit Report

**Date**: 2026-01-29
**Project**: Onesimus v1.0.0
**Auditor**: Automated i18n check

## Summary

| Status | Category | Count | Files |
|--------|----------|-------|-------|
| ✅ | Complete | 1 | MainWindow |
| ⚠️ | Needs Work | 15 | Widgets, Dialogs, Components |
| ❌ | Critical | 2 | SettingsDialog, BJobWidget |

## Audit Results by Component

### ✅ COMPLETE - MainWindow
**File**: `src/mainwindow.cpp`

**Status**: All strings converted to English with tr()
- ✅ All menu actions translated
- ✅ All menu titles translated
- ✅ All toolbar strings translated
- ✅ All status messages translated
- ✅ All dialog messages translated
- ✅ Comments updated to English

**Example**:
```cpp
m_connectAction = new QAction(tr("Connect"), this);
m_fileMenu = menuBar()->addMenu(tr("File"));
m_statusLabel = new QLabel(tr("Ready"), this);
```

---

### ❌ CRITICAL - SettingsDialog
**File**: `src/settingsdialog.cpp`

**Status**: ~80-100 German strings need conversion

**Issues**:
- German category labels: "Verbindung", "Erscheinungsbild", "Verhalten", "Erweitert"
- German group box titles: "Director-Verbindung", "TLS/SSL Verschlüsselung", "Farbschema", etc.
- German form labels: "Host:", "Port:", "Director-Name:", etc.
- German button texts: "Übernehmen", "Abbrechen", "Zurücksetzen"
- German tooltips and help texts

**Required Actions**:
1. Convert all QLabel/QGroupBox/QPushButton texts to English
2. Wrap all with tr()
3. Update category list items
4. Update message box texts

**Example fixes needed**:
```cpp
// BEFORE
QGroupBox *connGroup = new QGroupBox("Verbindung");
hostLabel = new QLabel("Host:");

// AFTER
QGroupBox *connGroup = new QGroupBox(tr("Connection"));
hostLabel = new QLabel(tr("Host:"));
```

---

### ❌ CRITICAL - BJobWidget
**File**: `src/jobs/bjobwidget.cpp`

**Status**: Multiple German strings, inconsistent tr() usage

**Issues**:
- German filter labels
- German button texts
- German status messages
- Mixed English/German in UI

**Required Actions**:
1. Audit all QString literals
2. Convert German to English
3. Wrap with tr()
4. Update tooltips

---

### ⚠️ NEEDS WORK - Job Components

#### BJobDetailsDialog
**File**: `src/jobs/bjobdetailsdialog.cpp`

**Likely Issues**:
- Dialog title
- Tab labels ("Details", "Log", "Files")
- Field labels
- Button texts

#### BJobLogDialog
**File**: `src/jobs/bjoblogdialog.cpp`

**Likely Issues**:
- Dialog title
- Log view labels
- Copy/Export buttons

#### BNewJobDialog
**File**: `src/jobs/bnewjobdialog.cpp`

**Likely Issues**:
- Dialog title ("Neuer Job"?)
- Form labels (Client, FileSet, Pool, etc.)
- Validation messages
- Button texts

#### BJobsStatisticsWidget
**File**: `src/jobs/bjobsstatisticswidget.cpp`

**Likely Issues**:
- Statistics labels ("Gesamt", "Erfolgreich", "Fehler")
- Number formatting
- Date/time formatting

#### BJobsFilterWidget
**File**: `src/jobs/bjobsfilterwidget.cpp`

**Likely Issues**:
- Filter labels
- Checkbox texts
- ComboBox items
- Date range labels

#### BPaginationWidget
**File**: `src/jobs/bpaginationwidget.cpp`

**Likely Issues**:
- "Seite X von Y" label
- Navigation button tooltips

---

### ⚠️ NEEDS WORK - Client Components

#### BClientsWidget
**File**: `src/clients/bclientswidget.cpp`

**Likely Issues**:
- Column headers
- Filter labels
- Status texts ("Online", "Offline")
- Context menu items

#### ClientWidget (legacy?)
**File**: `src/clientwidget.cpp`

**Status**: Might be deprecated, check if still used

#### BClientDetailsDialog
**File**: `src/clients/bclientdetailsdialog.cpp`

**Likely Issues**:
- Dialog title
- Tab labels
- Field labels
- Client information labels

---

### ⚠️ NEEDS WORK - StorageWidget
**File**: `src/storagewidget.cpp`

**Likely Issues**:
- Column headers
- Pool labels
- Volume status texts
- Device status texts
- Context menu items

---

### ⚠️ NEEDS WORK - BScheduleWidget
**File**: `src/schedules/bschedulewidget.cpp`

**Likely Issues**:
- Column headers
- Schedule details labels
- Run time formatting
- Level descriptions

---

### ⚠️ INFO - BJsonJobView
**File**: `src/jobs/bjsonjobview.cpp`

**Note**: Likely a data view component with minimal UI strings

---

### ⚠️ INFO - BaculaAuth
**File**: `src/baculaauth.cpp`

**Note**: Backend component, check for any user-facing error messages

---

## Recommended Action Plan

### Priority 1: Critical (Immediate)
1. **SettingsDialog** (~100 strings)
   - Most visible to users
   - Contains language selector (ironic!)
   - Required for complete i18n demo

2. **BJobWidget** (~30-40 strings)
   - Primary widget, most used
   - Filters and labels highly visible

### Priority 2: High (Within Week)
3. **Job Dialogs** (~60-80 strings total)
   - BJobDetailsDialog
   - BNewJobDialog
   - BJobLogDialog
   - BJobsStatisticsWidget
   - BJobsFilterWidget
   - BPaginationWidget

4. **ClientWidget/BClientsWidget** (~30 strings)
   - Second most used widget

### Priority 3: Medium (Within 2 Weeks)
5. **StorageWidget** (~25 strings)
   - Third most used widget

6. **BScheduleWidget** (~20 strings)
   - Fourth most used widget

7. **Client Dialogs** (~30 strings)
   - BClientDetailsDialog

### Priority 4: Low (As Needed)
8. **Backend Components**
   - BaculaAuth (error messages only)
   - BJsonJobView (minimal UI)

---

## Translation File Status

After source normalization, translation files will need population:

| Language | Status | Priority |
|----------|--------|----------|
| 🇬🇧 English (en) | Source | N/A (source language) |
| 🇩🇪 German (de) | ~30% | HIGH (existing users) |
| 🇪🇸 Spanish (es) | 0% | MEDIUM |
| 🇫🇷 French (fr) | 0% | MEDIUM |
| 🇮🇹 Italian (it) | 0% | LOW |
| 🇷🇺 Russian (ru) | 0% | LOW |

---

## Testing Checklist

After string normalization:

### Phase 1: Source Validation
- [ ] All German strings converted to English
- [ ] All strings wrapped with tr()
- [ ] No hardcoded language-specific text
- [ ] Comments in English
- [ ] Consistent terminology

### Phase 2: Translation File Generation
- [ ] Run `lupdate` to extract strings
- [ ] Verify all strings appear in .ts files
- [ ] Check string contexts are correct
- [ ] No duplicate entries

### Phase 3: German Translation
- [ ] Translate all strings to German
- [ ] Use existing German strings as reference
- [ ] Verify formatting placeholders (%1, %2)
- [ ] Test with Qt Linguist

### Phase 4: UI Testing
- [ ] Test with each language
- [ ] Verify no layout breaks
- [ ] Check text truncation
- [ ] Verify date/time formatting
- [ ] Check number formatting
- [ ] Test special characters (ä, ö, ü, ß)

### Phase 5: Edge Cases
- [ ] Test with very long translations
- [ ] Test RTL languages (if supported)
- [ ] Test mixed scripts
- [ ] Test without translation files (fallback to English)

---

## Tools & Commands

### Extract Strings
```bash
cd build
cmake ..
make  # Runs lupdate automatically
```

### Translate with Qt Linguist
```bash
linguist translations/onesimus_de.ts
```

### Check Translation Coverage
```bash
lrelease translations/onesimus_de.ts -verbose
```

### Grep for German Strings
```bash
grep -rn --include='*.cpp' '[äöüßÄÖÜ]' src/
```

### Find Non-tr() Strings
```bash
grep -rn --include='*.cpp' 'new Q.*("' src/ | grep -v 'tr('
```

---

## Notes

### QString Literals Without Translation
Some strings should NOT be translated:
- Object names: `setObjectName("mainToolbar")`
- Style sheet properties: `setStyleSheet("color: red")`
- Debug messages: `qDebug() << "Debug info"`
- File paths: `"/etc/bacula/config"`
- JSON keys: `json["key"]`
- SQL queries: `SELECT * FROM jobs`

### tr() vs QObject::tr()
Use the context-appropriate form:
- In QObject-derived classes: `tr("Text")`
- In non-QObject classes: `QObject::tr("Text")` or `QCoreApplication::translate("Context", "Text")`

### Plural Forms
Use Qt's plural support:
```cpp
tr("%n job(s) selected", "", count);
```

### Context for Translators
Add translator comments when needed:
```cpp
//: This appears in the status bar when connected
m_statusLabel->setText(tr("Connected"));
```

---

## Estimated Effort

| Task | String Count | Time Estimate |
|------|-------------|---------------|
| SettingsDialog | ~100 | 2-3 hours |
| BJobWidget | ~40 | 1-2 hours |
| Job Dialogs | ~80 | 2-3 hours |
| ClientWidget | ~30 | 1 hour |
| StorageWidget | ~25 | 1 hour |
| BScheduleWidget | ~20 | 1 hour |
| Client Dialogs | ~30 | 1 hour |
| Testing | - | 2-3 hours |
| **Total** | **~325** | **11-16 hours** |

---

## Completion Criteria

✅ All UI strings in English source code
✅ All UI strings wrapped with tr()
✅ German translation 100% complete
✅ English source = English "translation" (for locale)
✅ Spanish/French at least 50% complete
✅ No layout breaks with any language
✅ Proper date/time/number formatting per locale
✅ Documentation updated

---

**Generated**: 2026-01-29
**Next Review**: After Phase 2 completion (SettingsDialog + BJobWidget)
