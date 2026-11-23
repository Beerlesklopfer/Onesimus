<!-- SPDX-License-Identifier: AGPL-3.0-or-later -->
<!-- SPDX-FileCopyrightText: 2026 Jörg Bernau -->

# Contributing to Onesimus

Bug reports, translations and pull requests are welcome. Report bugs on
[GitHub Issues](https://github.com/Onesimus-io/Onesimus/issues) (check [TODO.md](TODO.md) first,
it lists the known ones) and vulnerabilities privately as described in [SECURITY.md](SECURITY.md).
Pull requests go against the `development` branch.

## Building

`./deploy.sh` (Linux, macOS), `./deploy.fish` (the same for fish) and `.\deploy.ps1` (Windows)
build, test and package the GUI; `help` prints every command. The platform guides are
[BUILD_LINUX.md](BUILD_LINUX.md), [BUILD_WINDOWS.md](BUILD_WINDOWS.md) and
[BUILD_OSX.md](BUILD_OSX.md).

Every build increments `.build_number` and regenerates `include/version.h`. Both files are
tracked; do not commit the values a development build stamped into them.

CMake options besides the build type:

| Option | Default | Effect |
|---|---|---|
| `USE_STATIC_OPENSSL` | `ON` | OpenSSL from the `external/openssl` submodule instead of the system's |
| `ENABLE_GRPC` | `ON` | gRPC transport for the Onesimus Director (needs Qt gRPC and Qt Protobuf) |
| `ONESIMUS_KEYCHAIN` | `ON` | Profile secrets in the system keychain (Linux: libsecret) |
| `ONESIMUS_FILE_LOGGING` | `OFF` | `BLOG_*` macros write `onesimus.log` in the application data directory |
| `DEBUG_PACKETS` | `OFF` | Log the raw telegrams of the console protocol |
| `DEBUG_JSON` | `OFF` | Log the JSON requests and replies |
| `LOG_JSON` | `OFF` | Write all JSON replies to a file |

A `Debug` build also defines `IS_DEVELOPER` (developer menus and extra diagnostics).

## Source layout

```
include/, src/     headers and sources, by area (director, jobs, clients, schedules,
                   storage, config, tools, models, ...); src/ mirrors include/
ui/                Qt Designer forms
resources/         directive schemas (directives/), FileSet templates, icons, themes
translations/      Qt Linguist files onesimus_<lang>.ts
test/              unit tests (CTest) and live-Director scripts (test/README.md)
packaging/         Debian, Windows (WiX) and macOS packaging
scripts/ci/        scripts of the CI and release workflows
proto/             gRPC API of the Onesimus Director (submodule)
external/openssl/  OpenSSL (submodule)
```

## Architecture

- **Director connection.** `BDirector` is the object the widgets talk to; it forwards commands to
  `BareosDirector`, which runs in a worker thread. `BareosDirector` speaks either the Bareos console
  protocol (BNET, login in `BareosAuth`: CRAM-MD5, TLS-PSK, certificates) or gRPC to the Onesimus
  Director (`BGrpcTransport`, with the certificate pin in `bgrpcconnect.cpp`).
- **Capabilities.** `BCapabilities` lists which functions each kind of Director offers;
  `BCapabilityNotice` tells the user when the connected Director cannot do something.
- **Configuration.** `BDirectiveSchema` loads the JSON schemas in `resources/directives/`;
  `BResourceForm` builds edit forms from them. Writes to the Onesimus Director go through
  `BConfigClient` / `BDirectorConfigClient`.
- **Models and views.** Qt model/view throughout; shared base models live in `include/models/`.

## Rules for code

- C++17 and Qt 6 conventions; signals and slots; RAII and Qt parent ownership.
- Every user-visible text is English and wrapped in `tr()`. Other languages come only from the
  `.ts` files.
- Every source and script file starts with an SPDX header. Own code is `AGPL-3.0-or-later`; code
  that follows the Bareos source is `AGPL-3.0-only` with the Bareos copyright holders and is listed
  in [LICENSING.md](LICENSING.md). `scripts/ci/check-spdx.sh` checks the headers in CI.
- Commit messages follow Conventional Commits (`fix(acl): ...`, `docs(readme): ...`).
- Add or extend a test in `test/` for a fix where possible, and run `./deploy.sh test`.

## Tests

`./deploy.sh test` (or `.\deploy.ps1 test`) builds and runs CTest with the offscreen platform.
None of these tests needs a Director. The live-Director tests (`ctest -L live-director`) are
skipped unless `ONESIMUS_LIVE_DIRECTOR` names a test Director you set up yourself; see
[test/README.md](test/README.md).

## Translations

```bash
cmake --build build --target update_translations   # extract new texts into translations/*.ts
linguist translations/onesimus_de.ts                # translate with Qt Linguist
```

The CI reports a warning (not an error) when `lupdate` would change the `.ts` files.

## CI

Every push and pull request runs [`.github/workflows/ci.yml`](.github/workflows/ci.yml):

| Job | What it does |
|---|---|
| Linux | Debian 13 container (distribution Qt 6.8 with Qt gRPC): SPDX check, `./deploy.sh configure`, `build`, `test`, `.deb` with a check of the third-party notices, lupdate check (warning only) |
| Windows | MSVC 2022, Qt 6.8.3: `deploy.ps1` build, test and MSI (GUI only), notices check |
| macOS | `Onesimus.app` and a dmg (ad-hoc signed; signed and notarised only on a manual run with `macos_sign`) |

The static OpenSSL build is cached per OpenSSL submodule commit.

## Releases

1. Set the version in `CMakeLists.txt` (`project(onesimus VERSION x.y.z ...)`), update
   `CHANGELOG.md`, push, and wait for a green CI run.
2. Tag that commit `vx.y.z` and push the tag. A tag with a suffix (`vx.y.z-rc.1`) becomes a
   pre-release.
3. [`.github/workflows/release.yml`](.github/workflows/release.yml) checks that the tag matches
   the project version (`scripts/ci/check-tag-version.sh`), builds the `.deb`, the AppImage, the
   MSI, the portable ZIP and the macOS dmg (signed and notarised when the signing secrets are
   set), builds the source archives, writes `SHA256SUMS` and creates a **draft** GitHub Release.
4. Review the draft and publish it by hand.

The MSI contains oconsole and the ofd service only when a Windows build of onesimus-dird is given
(repository variables `DIRD_WINDOWS_ZIP_URL` and `DIRD_WINDOWS_ZIP_SHA256`, or the inputs of a
manual run); otherwise it holds the GUI only. Its features are `Gui`, `DesktopShortcut`,
`Oconsole` and `Ofd`; `ADDLOCAL=` selects them in a silent install. The ofd service is installed
with start mode manual, because it needs a configuration in `C:\ProgramData\Onesimus\bareos-fd.d\`
first.
