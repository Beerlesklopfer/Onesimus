<!-- SPDX-License-Identifier: AGPL-3.0-or-later -->
<!-- SPDX-FileCopyrightText: 2026 Jörg Bernau -->

# Onesimus

Onesimus is a desktop management console for backup Directors, written in C++17 with Qt 6. It
connects to a **Bareos** Director over the Bareos console protocol, and to the **Onesimus
Director** (`odir`, a Director written in Rust) over gRPC. You use it to watch jobs and their logs,
restore files, plan schedules, check volumes and clients, and edit parts of the Director
configuration.

[![CI](https://github.com/Onesimus-io/Onesimus/actions/workflows/ci.yml/badge.svg)](https://github.com/Onesimus-io/Onesimus/actions/workflows/ci.yml)
[![Licence: AGPL-3.0](https://img.shields.io/badge/licence-AGPL--3.0-blue)](LICENSING.md)

> **Pre-release.** Version 0.3.0 is an early-access release. Test it against a Director you can
> afford to lose before you rely on it, and expect changes. Known bugs and open work are listed in
> [TODO.md](TODO.md).

## Why Onesimus

- **Desktop-native.** It runs on your workstation like a console: no web server, no PHP, nothing
  to install on the backup server.
- **Through the Director only.** Onesimus never opens the catalog database. Every request goes
  through the Director, so the console's ACLs apply to everything it does.
- **Two kinds of Director.** With Bareos it speaks the console protocol in JSON API mode. With the
  Onesimus Director it uses a typed gRPC API, finds Directors in the local network by mDNS, and
  offers functions that Bareos does not have (marked *Onesimus Director* below). Where the
  connected Director cannot do something, the GUI says so instead of failing silently.

### The five questions

Onesimus is meant to answer the questions every backup administrator asks. Today it answers one
of them in part:

| # | Question | Status in 0.3.0 |
|---|----------|-----------------|
| 1 | Where does my storage go? | planned |
| 2 | When will the pool be full? | planned |
| 3 | Which jobs and clients use the most storage? | planned |
| 4 | What happens if I change retention? | planned |
| 5 | Which volumes can I recycle? | partly: the volume table shows from when each volume is reusable, and why |

## Features in 0.3.0

**Jobs and logs**
- Jobs table with filters (name, client, status, level, date range), statistics, pagination and
  export as JSON or CSV.
- Run a job (with an estimate of files and bytes), cancel the selected jobs, job details.
- Job log that follows a running job.

**Restore**
- Restore wizard from a job: browse the backed-up files (BVFS), select files and directories,
  choose the target client, the Restore Job and the path, check the preview, run.

**Schedules and timeline**
- Timeline of the clients (FD) and storages (SD) by day or week, built from the measured job
  history, with utilisation and a calendar.
- Predicted waiting times of the jobs, calibrated against the catalog.
- Stagger: proposes later start times for jobs that wait, on the quarter hour, with constraints
  per profile. Applying the proposal needs the *Onesimus Director*.
- Schedule wizard and grid editor by weekday and hour. Creating a schedule works with both
  Directors; changing and deleting one needs the *Onesimus Director*.

**Storage and volumes**
- Volume table with filters by pool and status: used bytes, limit, jobs, last written, retention,
  "reusable from" and the reason; details per volume.
- Database cleanup (Tools menu) of old jobs, with a preview. Prune and purge of all volumes with a
  preview: *Onesimus Director*.

**Clients**
- Client table with the age of the last backup (recent, stale, never). With the *Onesimus
  Director* it shows Online or Offline from the Director's client events.
- Client details with configuration export. A new-client dialog generates the File Daemon and
  Director configuration, runs `configure add client` and exports the File Daemon files as ZIP.

**Configuration and wizards**
- Add Job and Add JobDefs wizards. Edit Job, Edit JobDefs, Delete Job and Delete JobDefs write
  over gRPC: *Onesimus Director*.
- FileSet wizard with 22 templates. It writes the FileSet to the *Onesimus Director*; with Bareos
  you copy the generated configuration text.
- Import of an existing Director configuration from files or a ZIP archive.

**ACL matrix** (Tools menu, *Onesimus Director*)
- The ACLs and profiles of all consoles in one matrix. Catalog ACLs can be changed; new consoles
  and profiles are created from proposed configuration text.
- Separately, Help > My Permissions lists the commands the logged-in console may use.

**3-2-1 verifier** (Tools menu)
- Checks each backup job against the 3-2-1 rule (3 copies, 2 media types, 1 off site) from what
  the catalog records.

**Interface and languages**
- English, German, Spanish, French, Italian and Russian (View > Language; the system language by
  default). German has 6 of 3890 texts untranslated, the other four 251 each
  (`translations/*.ts`, 0.3.0).
- Light, dark and system theme; interface zoom.

**Connection and security**
- Connection wizard and connection profiles for several Directors (one connection at a time).
- Bareos: CRAM-MD5 login in which the Director must prove the password as well, TLS-PSK, or TLS
  with certificates (CA and host name check; PFX files on Windows). A profile that requires TLS
  never falls back to clear text. A certificate generator is built in.
- Onesimus Director: gRPC over TLS; the server certificate is confirmed on first use and pinned.
- The passwords of the profiles are kept in the system keychain (Secret Service on Linux, login
  keychain on macOS, Credential Manager on Windows).
- Headless mode for the console protocol: `onesimus --headless --host <host> -e "<command>"`
  prints the Director's JSON reply.

The [CHANGELOG](CHANGELOG.md) lists every change of this release.

## Install

Download the packages from [GitHub Releases](https://github.com/Onesimus-io/Onesimus/releases):

| Package | For |
|---------|-----|
| `.deb` | Debian 13 (trixie), x86_64 |
| `.AppImage` | Linux x86_64 (built on Debian 13) |
| `.msi` | Windows 64-bit; the 0.3.0 MSI contains the GUI only |
| `-Windows.zip` | Windows 64-bit, portable (unpack and run `onesimus.exe`) |
| `-macos-arm64.dmg` | macOS 12 or later on Apple Silicon; signed and notarised by Apple |

The MSI installs silently with `msiexec /i Onesimus-0.3.0-windows-x86_64.msi /qn`.
`SHA256SUMS` in the release lists the checksum of every file. The Linux and Windows packages are
not signed. The source archives (Onesimus with its submodules, the Qt sources of the Windows and
macOS packages, the Debian sources of the libraries in the AppImage) are attached to the same
release; [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md) explains them.

## First connection

1. Start Onesimus and open **File > Connection Wizard...**.
2. Bareos: enter the Director's address, port and name, the console name and its password, and
   choose TLS-PSK or certificates if your Director uses them. Instead of typing, you can import
   the Director configuration from a ZIP file.
3. Onesimus Director: choose the gRPC transport. Directors in the local network appear in the list
   by themselves; otherwise enter the address. Confirm the server certificate on the first
   connection.

## Build from source

You need Qt 6.8 or later (with Qt gRPC for the Onesimus Director transport), CMake 3.21 or later
and a C++17 compiler.

```bash
./deploy.sh build     # Linux/macOS: build into ./build
./deploy.sh run       # build and start
./deploy.sh help      # all commands (test, package, appimage, install, ...)
```

```powershell
.\deploy.ps1 build    # Windows (Developer PowerShell for VS 2022)
.\deploy.ps1 help
```

Details per platform: [BUILD_LINUX.md](BUILD_LINUX.md), [BUILD_WINDOWS.md](BUILD_WINDOWS.md),
[BUILD_OSX.md](BUILD_OSX.md). Architecture, tests, translations, CI and the release process are
described in [CONTRIBUTING.md](CONTRIBUTING.md).

## Documentation

- [TODO.md](TODO.md): known bugs and planned work
- [CHANGELOG.md](CHANGELOG.md): changes per version
- [SECURITY.md](SECURITY.md): supported versions and how to report a vulnerability
- [LICENSING.md](LICENSING.md) and [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md)
- Website: [onesimus.io](https://onesimus.io); editions and pricing:
  [onesimus.io/pricing](https://onesimus.io/pricing/)

## Licence

Onesimus is distributed under the GNU Affero General Public License, version 3
([LICENSE](LICENSE)). Its own code is `AGPL-3.0-or-later`; files that follow the Bareos source are
`AGPL-3.0-only`, so the program as a whole is distributed under AGPL-3.0. Each file states its
licence in an SPDX header; [LICENSING.md](LICENSING.md) lists the Bareos-related files.
Third-party components and their licences are listed in
[THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md); the licence texts are in [LICENSES/](LICENSES/).

Onesimus is built with the [Qt framework](https://www.qt.io/) and [OpenSSL](https://www.openssl.org/),
and follows the protocols and behaviour of [Bareos](https://www.bareos.com/).

## Support and contact

- Bugs and feature requests: [GitHub Issues](https://github.com/Onesimus-io/Onesimus/issues)
- Questions: [GitHub Discussions](https://github.com/Onesimus-io/Onesimus/discussions)
- Vulnerabilities: privately, as described in [SECURITY.md](SECURITY.md)
- Onesimus is built by one developer in spare time. You can support it through
  [PayPal](https://www.paypal.com/pool/9mSSenuhxX?sr=wccr).
