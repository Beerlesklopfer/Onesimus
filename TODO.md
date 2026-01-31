# TODO List

## Run Job Dialog

- [ ] Fetch preselection from Director when job is selected
  - [ ] Get default FileSet from job configuration
  - [ ] Get default Pool from job configuration
  - [ ] Get default Storage from job configuration
  - [ ] Get default Client from job configuration
  - [ ] Auto-select these values in the combo boxes
- [ ] Query job defaults from Director on job selection change
  - [ ] Use `.jobs` command to get full job configuration
  - [ ] Parse fileset, pool, storage, client from response
  - [ ] Update combo boxes with default values

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


## General
- [ ] In basicfilters => Status the runnig job is missing
- [ ] Complete remaining translations (ES, FR, IT, RU)
- [ ] Add Bacula support
- [ ] Implement live job monitoring with progress bars
