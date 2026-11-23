<!-- SPDX-License-Identifier: AGPL-3.0-or-later -->
<!-- SPDX-FileCopyrightText: 2026 Jörg Bernau -->

# Known bugs and open work

This file lists the known bugs and the planned work for the Onesimus GUI. Every entry was
checked against the code on 2026-11-23; file references are to that state. Report new bugs on
[GitHub Issues](https://github.com/Onesimus-io/Onesimus/issues); `#n` below is a GitHub issue.

## Known bugs

### Director/connection
- Without client events (Bareos over BNET) the clients table shows the age of the last activity,
  not Online/Offline (`src/clients/bclientsmodel.cpp:61`, `:808`). The Onesimus Director sends the
  events. `status director` and an on-demand probe are still open (#9).

### Jobs
- The jobs table does not follow job status events: it changes only on Refresh or by the
  auto-refresh timer (`src/bmainwindow.cpp:497` passes the events to the schedule view only, `:571`).

### Restore
- The BVFS file browser sends its directory listings one command at a time, page by page
  (`src/jobs/bbvfsmodel.cpp:452-505`), so large trees load slowly.

### To verify
- Job log scroll flip and the empty log panel (#10): the job log was rewritten as one append-only
  model with follow-tail (0ee8227). Not yet confirmed against a running job.
- Job statistics: the figures are summed over the jobs loaded into the table, not over the
  whole catalog or the current filter (`src/jobs/bjobmodels.cpp:499-526`). Not yet decided
  whether that is the intended meaning.
- Add Job / Add JobDefs wizard (`src/jobs/bjobwizard.cpp`): there is no automated test, and no
  end-to-end run against a Director is recorded.

## Open work

### Director/connection
- Match replies by request ID instead of queue position
  (`src/director/bareosdirector.cpp:1285`), with framing by BNET signal and prompt handling.
- Simultaneous connections to several Directors (today one connection at a time, switched by
  profile).
- Restrict the GUI by the logged-in console's ACLs (hide or disable what CommandACL and the
  resource ACLs do not allow, honour `*all*` and `!name`). Today there is the "My Permissions"
  dialog (`src/bmainwindow.cpp:923`) and the ACL matrix, which shows and changes ACLs but
  restricts nothing.
- ACL matrix: typed fields for the effective and locked ACL values in the proto. The matrix still
  falls back to `extra_fields` (`src/director/baclmatrix.cpp:20-25`).
- Warn when the Onesimus catalog extension is damaged after a Bareos upgrade.
- Free tier: tell the user when a backup job is held by the monthly limit. Today only the status
  bar tool tip says so (`src/busagestatus.cpp:156-163`).
- Command line in the GUI with completion (#12; parked).
- Bacula backend: deprecated and not built; `src/director/baculaauth.*` stays in the tree.

### Jobs
- A list of Job and JobDefs resources in the main window, showing inherited and overridden values,
  with New and Copy. Today: Edit Job…, Edit JobDefs… and Delete JobDefs… in the Jobs menu
  (`src/bmainwindow.cpp:1066`, `:1159`, `:1220`).
- Job wizard: RunScript blocks cannot be sent with `configure add`
  (`src/jobs/bjobwizard.cpp:794`), and the wizard does not use CreateResource on the Onesimus
  Director (#11).

### Restore
- No open items besides the browser speed above.

### Schedules
- No open items.

### Storage
- Storage insights with charts: pool overview, consumption per job and client, growth trends and
  capacity forecast, retention analysis with what-if, recycling recommendations. Today there is
  the volume table with "reusable from"; there is no chart code yet.
- Per-volume operations (label, relabel, update) and a storage daemon status view.
  `Command::StatusStorage` has no UI (`src/director/bareosdirector.cpp:2189`).

### Configuration/wizards
- Storage resources: no wizard and no editor (Storage, SD devices, autochangers).
- Resource Edit… writes only Job and JobDefs (`src/director/bresourcewidget.cpp:185`). Client,
  Storage, Pool and the other types are read-only.
- FileSet wizard: Plugin lines are read and written (`src/jobs/fileset/bfilesetdocument.cpp:806`,
  `:852`), but the Include editor has no field for them.
- Nested resources (FileSet Include/Options/Exclude, Job RunScript) in `configure add` on plain
  Bareos (#11).
- Export the whole Director configuration as one archive (today per resource type,
  `src/director/bresourcewidget.cpp`).
- Undo/redo for the editable lists: commands are pushed (`src/config/bundocommands.cpp`), but
  nothing calls `undo()`. Only the schedule grid editor has a working undo.
- Idea: a bareos-fd plugin that lists a client's paths from its file index (mlocate/plocate) for
  browsing while building a FileSet. Nothing exists yet.

### UI
- No open items besides those above.

### Build/packaging
- Most tests still compile their own copies of application sources; only `bdialogfonts_test` and
  `bappwidgets_test` link `onesimus_core` (`CMakeLists.txt`, test section).
- Compiler warnings left: OpenSSL's deprecated `MD5_*` in `src/director/bareosauth.cpp:1126-1152`
  (CRAM-MD5 of the BNET login), an unused `placementText()` in `src/tools/b321dialog.cpp:110`.
- Tests still missing for `BJsonStreamReader` and `BConfigExporter`.

### Docs/i18n
- Translations: German is complete. Spanish, French, Italian and Russian each have 245 of 3884
  messages unfinished (`translations/*.ts`).
