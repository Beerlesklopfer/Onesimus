# BJobFilesWidget Test Application

Standalone test application for inspecting the BJobFilesWidget (file browser with tree/table views) used in Onesimus job details dialog.

## Purpose

This test app allows you to:
- Inspect treeview and table behavior
- Test checkbox selection
- Test folder expansion (lazy loading)
- Test double-click navigation
- Add test data dynamically
- Debug tree/table interactions

## Building

### From Main Project (Recommended)

**Windows:**
```powershell
# From project root
mkdir build
cd build

# Configure with UI tests enabled
cmake .. -G "NMake Makefiles" `
    -DCMAKE_PREFIX_PATH="C:\Qt\6.10.1\msvc2022_64" `
    -DBUILD_UI_TESTS=ON

# Build all (including test)
nmake

# Or build only the test
nmake bjobfileswidget_test

# Run
.\bjobfileswidget_test.exe
```

**Linux:**
```bash
# From project root
mkdir build && cd build

# Configure with UI tests enabled
cmake .. -DBUILD_UI_TESTS=ON

# Build all (including test)
make

# Or build only the test
make bjobfileswidget_test

# Run
./bjobfileswidget_test
```

### Standalone (Not Recommended)

You can also build standalone from this directory, but it's better to use the main project build.

## Features

### Directory Tree (Left Panel)
- Hierarchical folder structure with checkboxes
- Lazy loading of subdirectories on expand
- Checkbox selection propagates to children
- Click folder to load its files
- Expand/collapse folders

### File List (Right Panel)
- Table view with checkboxes
- Columns: Checkbox, Name, Size, Type, Modified
- Double-click files for action simulation
- Checkbox selection for individual files

### Toolbar
- **Add Test Folder** - Adds a new test folder to root
- **Add Test File** - Adds a new file to current file list
- **Clear All** - Clears both tree and file list
- **Status** - Shows current operation/selection

### Sample Data

Pre-populated with:
```
/
├── etc/
│   ├── apache2/
│   ├── ssh/
│   └── systemd/
├── home/
│   ├── user1/
│   └── user2/
└── var/
    ├── log/
    ├── cache/
    └── www/
```

## Testing Scenarios

### Test Checkbox Selection
1. Click checkboxes in tree
2. Verify children get selected automatically
3. Check individual files in file list
4. Watch selection counter update

### Test Lazy Loading
1. Expand folders in tree
2. Observe "Loading..." placeholder disappears
3. Subdirectories appear
4. Verify no duplicate loading

### Test Navigation
1. Click folders in tree
2. File list updates with folder contents
3. Double-click items in file list
4. Status updates appropriately

### Test Dynamic Updates
1. Click "Add Test Folder"
2. New folder appears in tree
3. Click "Add Test File"
4. New file appears in table
5. Test selection on new items

## Code Structure

### Main Components

```cpp
class TreeViewTestWindow : public QMainWindow
{
    // Tree view (left panel)
    QTreeView *m_treeView;
    QStandardItemModel *m_treeModel;

    // Table view (right panel)
    QTableView *m_tableView;
    QStandardItemModel *m_tableModel;

    // UI components
    QSplitter *m_splitter;
    QLabel *m_statusLabel;
    QLabel *m_selectionLabel;
};
```

### Key Functions

- `createTreeItem()` - Creates tree items with checkboxes and metadata
- `loadFilesForDirectory()` - Populates file list for selected folder
- `lazyLoadSubdirectories()` - Loads subdirectories on expand
- `propagateCheckStateToChildren()` - Syncs checkbox states
- `updateSelectionCount()` - Updates selection counter

## Comparing with Real BJobFilesWidget

### Similarities
- Same QTreeView + QTableView layout
- Same QStandardItemModel usage
- Same checkbox behavior
- Same lazy loading pattern
- Same splitter setup (30% / 70%)

### Differences
- Test app uses mock data instead of BVFS
- Test app doesn't connect to Director
- Test app simulates file loading instantly
- Test app has additional test controls

## Debugging Tips

### Enable Qt Debug Output

```cpp
// Add to main()
qputenv("QT_LOGGING_RULES", "qt.widgets.*=true");
```

### Print Tree Structure

```cpp
void printTreeStructure(QStandardItem *item, int depth = 0)
{
    QString indent(depth * 2, ' ');
    qDebug() << indent << item->text()
             << "Checked:" << (item->checkState() == Qt::Checked);

    for (int i = 0; i < item->rowCount(); ++i) {
        printTreeStructure(item->child(i), depth + 1);
    }
}
```

### Monitor Selection Changes

Connect to `itemChanged` signal with verbose logging:

```cpp
connect(m_treeModel, &QStandardItemModel::itemChanged,
        [](QStandardItem *item) {
            qDebug() << "Item changed:" << item->text()
                     << "State:" << item->checkState();
        });
```

## Known Issues to Test

Based on KNOWN_BUGS.md:

### Job Tree View - Table Update
Test if table updates correctly when simulating job status changes.

### Job Tree View - Functionality
Verify tree navigation works in all scenarios.

## Extending the Test

### Add Real BVFS Simulation

Replace mock data with JSON parsing to test real Director responses.

### Add Restore Preview

Implement restore dialog simulation to test full workflow.

### Add Search/Filter

Add search box to test filtering tree and table.

## Integration

This test can be integrated into main CMake build:

```cmake
# In main CMakeLists.txt
if(BUILD_TESTS)
    add_subdirectory(tests/treeview_test)
endif()
```

Then build with:
```bash
cmake .. -DBUILD_TESTS=ON
```

## License

Part of Onesimus project.
