<!-- SPDX-License-Identifier: AGPL-3.0-or-later -->
<!-- SPDX-FileCopyrightText: 2026 Jörg Bernau -->

# Licensing

Onesimus is distributed under the **GNU Affero General Public License, version 3** ([LICENSE](LICENSE)).
Every source and script file states its licence in an SPDX header.

- **Own code** is `AGPL-3.0-or-later`: you may also use it under any later version of the AGPL.
- **Files that follow the Bareos (or Bacula) source** are `AGPL-3.0-only`. Bareos grants its code
  under "version three of the GNU Affero General Public License" only (for example
  `core/src/dird/ua_cmds.cc`, `core/LICENSE`), so code derived from it cannot be offered under a later
  version. These files name the Bareos copyright holders (Free Software Foundation Europe e.V.,
  Planets Communications B.V., Bareos GmbH & Co. KG) in their SPDX headers.

Because the program contains both, the program as a whole is distributed under AGPL-3.0 (version 3).

## Bareos-related files

The list is deliberately cautious: a file is listed when it ports Bareos code or reproduces Bareos
behaviour with references to the Bareos source. Whether a file is a derived work in the legal sense
has not been decided; until a legal review, these files stay `AGPL-3.0-only`.

**Ports of Bareos code** (the file says it follows a Bareos function step by step):

| File | Follows |
|---|---|
| `src/schedules/model/brunparser.cpp`, `include/schedules/model/brunspec.h` | `dird/run_conf.cc` `StoreRun()`, `lib/lex.cc` |
| `src/schedules/model/bscheduleoccurrences.cpp`, `include/schedules/model/bscheduleoccurrences.h` | `dird/date_time.cc`, `lib/btime.cc` `TmWoy()` |
| `src/director/bareosauth.cpp`, `include/director/bareosauth.h` | CRAM-MD5 handshake and its base64 variants (`lib/cram_md5.cc`, `lib/bsock.cc`, `lib/tls_openssl_private.cc`) |
| `src/director/baculaauth.cpp`, `include/director/baculaauth.h` | the Bacula handshake (not built; the Bacula backend is deprecated) |
| `test/bquote_test.cpp` | `lib/scan.cc` `next_arg()`, the quoted-string state of `lib/lex.cc` |
| `test/bbnettesthelpers.h` | `BinToBase64()` of `lib/base64.cc` and the CRAM-MD5 answer of `lib/cram_md5.cc`, for the fake Directors of the tests |

**Own implementations of Bareos behaviour** (written against the Bareos source as the reference
for semantics, cited in comments):

| File | Behaviour |
|---|---|
| `schedules/model/bschedulewaitmodel`, `bwaitcalibration`, `bjobhistorystats`, `schedules/bstaggerplan` | job queue, resources and device reservation of Director and storage daemon |
| `storage/model/brecycleforecast`, `storage/model/bvolumelist` | volume status, pruning, recycling and truncation |
| `bbyteformat` | size units of `lib/edit.cc` |
| `director/baclmatrix` | ACL evaluation of the Director (`dird/ua_acl.cc`) |
| `bstatuschip` | the job status letters of `include/job_status.h` |
| `tools/b321verifier` | which backups and copies a restore can use (`dird/ua_restore.cc`, `dird/migrate.cc`, `cats/sql_update.cc`) |
| `director/bareosdirector`, `models/bvolumejson` | the output of list commands (`dird/ua_output.cc`, `cats/sql_list.cc`) |
| `bquote` (header only) | quoting of command lines and resource text (`lib/scan.cc`, `lib/lex.cc`), the name rule of `lib/edit.cc` `IsNameValid()`, the characters `configure add` refuses (`dird/ua_configure.cc`) |
| `jobs/fileset/bfilesetdocument` | the FileSet grammar and keywords (`dird/inc_conf.cc`, `dird/dird_conf.cc` `fs_items`, `FilesetResource::PrintConfig`) |
| `test/bfakebnetdirector.h` | the Director's side of the console login (`lib/cram_md5.cc`) and the BNET signals of the command loop (`lib/bsock.h`, after the Onesimus Director), as a fake Director for tests |

(`.h` in `include/`, `.cpp` in `src/`.)

**Test configuration modelled on the Bareos default configuration** (same resource names and structure,
partly the same descriptions as `/usr/lib/bareos/defaultconfigs/bareos-dir.d/`; not identical copies):
`test/bareos-dir.d/bareos-dir.d/{catalog/MyCatalog,client/bareos-fd,director/bareos-dir,fileset/SelfTest,messages/Daemon,messages/Standard,pool/Full,schedule/WeeklyCycle,storage/File}.conf`.
The console and job files in that directory and `test/configs/*.conf` are own files (AGPL-3.0-or-later).

Not listed, because they contain no Bareos text or code: the directive descriptions in
`resources/directives/*.json` (own short texts; directive names are facts of the interface).

The `proto` submodule (`onesimus-proto`) and the Onesimus Director (`onesimus-director`) keep their
own lists of Bareos-related files.
