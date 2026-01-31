# TODO List

## Run Job Dialog

- [x] Fetch preselection from Director when job is selected
  - [x] Get default FileSet from job configuration
  - [x] Get default Pool from job configuration
  - [x] Get default Storage from job configuration
  - [x] Get default Client from job configuration
  - [x] Auto-select these values in the combo boxes
- [x] Query job defaults from Director on job selection change
  - [x] Use `.jobs` command to get full job configuration
  - [x] Parse fileset, pool, storage, client from response
  - [x] Update combo boxes with default values

## Job Management

- [ ] Complete job tree view implementation
- [ ] Fix job log panel scrolling issue with multiple dialogs
- [ ] Add tab view for Job Log and Messages in BJobWidget
  - [ ] Remove job log from bottom panel
  - [ ] Create tab widget with "Log" and "Messages" tabs
  - [ ] Move job log to "Log" tab
  - [ ] Add director messages to "Messages" tab

## BVFS File Browser

- [ ] Implement actual restore functionality
- [ ] Add progress indicator for large restore operations
- [ ] Improve performance for large directory trees

## Statistics
- [ ] Add job tends according to jobs to watch crotical development of storage sapces.


## Settings Dialog
- [ ] Add status color customization to settings dialog
  - [ ] Allow users to customize colors for each job status (T, W, f, E, R, A)
  - [ ] Color picker or preset selections
  - [ ] Preview of color changes

## General
- [x] In basicfilters => Status the running job is missing (added Running (R) and Canceled (A))
- [ ] Complete remaining translations (ES, FR, IT, RU)
- [ ] Add Bacula support
- [ ] Implement live job monitoring with progress bars
