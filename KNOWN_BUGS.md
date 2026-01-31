# Known Bugs and Issues

This document lists known bugs and issues in Onesimus.

## Current Issues

### Job Tree View
- **Description**: The job tree view is not fully functional due to ongoing development.
- **Workaround**: Use the table view for job management.
- **Status**: Under development

### Job Log Panel Scrolling
- **Description**: The job log panel may become empty when scrolling to the bottom with multiple dialogs open.
- **Workaround**: Close other job dialogs before scrolling through the main log panel.
- **Status**: Under investigation

### BVFS File Browser
- **Description**: Large directory trees may take time to load due to sequential API calls.
- **Workaround**: Be patient when browsing large backup sets.
- **Status**: Known limitation

## Resolved Issues

### Version 0.1.0.3

- Fixed: Job log flipping/switching between jobs when rapidly selecting different jobs
  - Added pending job ID tracking to ensure correct log is displayed
- Fixed: Job log not displaying in BJobWidget bottom panel
  - Fixed signal handler to use current row instead of checkbox selection
- Fixed: Folder selection in file browser not counting subfolders
  - Added propagation of tree checkbox state to file list items
- Fixed: Restore button width too narrow (only showing "1 file")
  - Added dynamic width calculation using QFontMetrics
- Fixed: Run Job button not being enabled in new job dialog
  - Added button state update after data loading
- Fixed: Run Job dialog allows starting without selecting a job
  - Added minimum requirement check (job must be selected)
- Added: Delete job options (delete record vs purge with volume data)
- Added: Dependent job detection when deleting Full backups
- Added: About Qt option in Help menu
- Added: English translation for Run New Job dialog

### Version 0.1.0

- Fixed: About dialog now shows correct version with build number
- Fixed: Version header auto-generation via CMake

## Reporting Bugs

Please report new bugs by creating an issue in the project repository.

Include the following information:
- Onesimus version (see About dialog)
- Operating system and version
- Steps to reproduce the issue
- Expected behavior
- Actual behavior
- Any error messages or log output
