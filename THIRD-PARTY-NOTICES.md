<!-- SPDX-License-Identifier: AGPL-3.0-or-later -->
<!-- SPDX-FileCopyrightText: 2026 Jörg Bernau -->

# Third-party notices

Onesimus itself is licensed under the GNU Affero General Public License, version 3
(`LICENSE`; see `LICENSING.md` in the source for which files are AGPL-3.0-or-later and which are
AGPL-3.0-only). The binary packages also contain, or are built from, the third-party components
listed below. Each package ships this file together with the licence texts in `LICENSES/`:

| Package | Location of this file |
|---|---|
| `.deb` (Debian 13) | `/usr/share/doc/onesimus/` |
| AppImage | `usr/share/doc/onesimus/` inside the AppImage (`--appimage-extract`) |
| Windows MSI and portable ZIP | next to `onesimus.exe` |
| macOS `.dmg` | `Onesimus.app/Contents/Resources/` |

In the program: *Help → About Onesimus → Third-party licences*.

## Source code

The complete corresponding source code of Onesimus and of the third-party components in the
packages is attached to the same GitHub release as the binaries
(<https://github.com/Onesimus-io/Onesimus/releases>, the release of this version):

| Archive | Contents |
|---|---|
| `onesimus-<version>-src.tar.xz` | Onesimus at the release tag, with the submodules the build uses: `external/openssl` (OpenSSL) and `proto` (onesimus-proto); `SOURCE-REVISIONS.txt` names their commits |
| `onesimus-<version>-appimage-debian-sources.tar` | the Debian source packages (`.dsc`, orig and debian tarballs) of every library bundled in the AppImage, Qt included, in exactly the versions listed in its `bundled-packages.txt`; unpack one with `dpkg-source -x <package>.dsc` |
| `onesimus-<version>-qt-<Qt version>-sources.tar` | the Qt source tarballs (qtbase, qtsvg, qtgrpc, qtimageformats, qttranslations) of the Qt version in the Windows and macOS packages, as published on download.qt.io, with their checksums |

The `.deb` links the Qt of Debian 13 from the system; its source is in Debian (`apt-get source
qt6-base qt6-grpc`) and, for the version of the release build, in the Debian sources archive above.
An archive larger than 2 GiB is attached in parts `<archive>.part00`, `<archive>.part01`, ... with
`<archive>.sha256`; reassemble it with `cat <archive>.part* > <archive> && sha256sum -c
<archive>.sha256`. The upstream links below are additional pointers only.

## Summary

| Component | Version | Licence (SPDX) | .deb | AppImage | MSI / ZIP | dmg |
|---|---|---|---|---|---|---|
| Qt (Core, Gui, Widgets, Network, Sql, Svg, Protobuf, ProtobufWellKnownTypes, plugins) | Debian 13: 6.8.2; Windows/macOS: 6.8.3 | LGPL-3.0-only (chosen from LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only) | linked, not included | included | included | included |
| Qt gRPC (QtGrpc) | as Qt | GPL-3.0-only | linked, not included | included | included | included |
| OpenSSL | 3.6.0 | Apache-2.0 | statically linked | statically linked (+ Debian's `libssl3` for Qt's TLS plugin) | DLLs | statically linked |
| Feather icons (some toolbar icons) | path data, see below | MIT | built in | built in | built in | built in |
| Libraries bundled by linuxdeploy | Debian 13 packages | various, see below | – | included | – | – |
| Third-party code inside the Qt libraries | see below | various | – | via Debian's Qt | included | included |
| Microsoft Visual C++ runtime | MSVC 2022 | Microsoft licence terms | – | – | see below | – |
| onesimus-proto (`proto/` submodule) | submodule commit | AGPL-3.0-only | built in | built in | built in | built in |

## Qt

- **Copyright:** The Qt Company Ltd. and other contributors.
- **Licence:** Qt is offered as LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only (or commercially).
  Onesimus uses Qt under the **GNU Lesser General Public License, version 3**
  (`LICENSES/LGPL-3.0-only.txt`, which builds on `LICENSES/GPL-3.0-only.txt`).
- **Upstream:** <https://www.qt.io/>, <https://code.qt.io/>
- **Modules used:** QtCore, QtGui, QtWidgets, QtNetwork, QtSql, QtSvg (through the SVG plugins), QtProtobuf,
  QtProtobufWellKnownTypes, QtGrpc (see below), and the plugins the deployment tools add (platform,
  style, image format, icon engine, TLS and SQLite plugins; Wayland/XCB on Linux).
- **Linking:** Qt is linked **dynamically** in every package. You may replace the Qt libraries with
  a modified version of the same Qt minor release: on Linux the `.so` files (the `.deb` uses the
  system's Qt; in the AppImage they are in `usr/lib/`), on Windows the `Qt6*.dll` files next to
  `onesimus.exe`, on macOS the frameworks in `Onesimus.app/Contents/Frameworks/` (re-sign the
  bundle afterwards, e.g. `codesign --force --deep --sign - Onesimus.app`). The Onesimus source code
  needed to rebuild the program against another Qt is at the link above.
- **Qt source code:** attached to the release (see "Source code" above): the Debian Qt of the
  `.deb` and the AppImage in `onesimus-<version>-appimage-debian-sources.tar`, the official Qt 6.8.3
  of the Windows and macOS packages in `onesimus-<version>-qt-6.8.3-sources.tar`. Also at
  <https://sources.debian.org/> and <https://download.qt.io/archive/qt/6.8/6.8.3/submodules/>.

## Qt gRPC (QtGrpc)

- **Copyright:** The Qt Company Ltd.; Alexey Edelev, Viktor Kopp and other contributors.
- **Licence:** **GPL-3.0-only** (open-source licence of QtGrpc in Qt 6.8, per the SPDX headers
  `LicenseRef-Qt-Commercial OR GPL-3.0-only` of its sources). Text: `LICENSES/GPL-3.0-only.txt`.
  The AGPL-3.0 and the GPL-3.0 allow combining works under both (section 13 of each); the QtGrpc
  part stays under the GPL-3.0.
- **Upstream:** <https://code.qt.io/cgit/qt/qtgrpc.git/>; source code attached to the release as for
  Qt above (`qt6-grpc` in the Debian sources archive, `qtgrpc` in the Qt sources archive).
- The client code generated by `qtprotobufgen` and `qtgrpcgen` from the Onesimus `.proto` files is
  part of Onesimus. Google's protobuf compiler (`protoc`) is a build tool only; no Google protobuf
  library is shipped.

## OpenSSL

- **Version:** 3.6.0 (git submodule `external/openssl`, branch `openssl-3.6.0`; `VERSION.dat`).
  Its source is part of `onesimus-<version>-src.tar.xz`; Debian's `libssl3t64` of the AppImage is in
  the Debian sources archive.
- **Copyright:** Copyright (c) 1998-2025 The OpenSSL Project Authors. Copyright (c) 1995-1998
  Eric A. Young, Tim J. Hudson. All rights reserved.
- **Licence:** Apache-2.0 (`LICENSES/Apache-2.0.txt`). OpenSSL 3 has no NOTICE file.
- **Upstream:** <https://www.openssl.org/>, <https://github.com/openssl/openssl>
- **In the packages:** linked statically into the program on Linux (`.deb`, AppImage) and macOS;
  shipped as `libssl-3-x64.dll` and `libcrypto-3-x64.dll` on Windows. The AppImage also contains
  Debian's `libssl.so.3`/`libcrypto.so.3` (package `libssl3t64`), which Qt's OpenSSL TLS plugin
  loads; its copyright file is in `usr/share/doc/libssl3t64/`.

## Feather icons

- **Copyright:** Copyright (c) 2013-present Cole Bemis.
- **Licence:** MIT (`LICENSES/MIT.txt`).
- **Upstream:** <https://github.com/feathericons/feather>
- **Used in:** `resources/icons/connect.svg`, `disconnect.svg`, `reconnect.svg` (Feather "link"),
  `delete.svg` (Feather "trash-2"), `exit.svg` (Feather "log-out"): their path data is identical to
  these Feather-derived icons as shipped by Lucide, whose licence file names them as derived from
  Feather under the MIT licence. `moon.svg` and `copy.svg` use the path data of Feather's "moon"
  and "copy" icons. The PNG versions of these icons are renderings of the SVG files. The other
  icons and the Onesimus logo are drawn for Onesimus.

## Libraries bundled in the AppImage

The AppImage is built on Debian 13 with linuxdeploy and its Qt plugin. Besides Qt it bundles the
shared libraries Qt and Onesimus need that are not part of every Linux system, for example ICU,
GLib, D-Bus, libxcb and xkbcommon, Wayland client libraries, HarfBuzz/Graphite, PCRE2,
double-conversion, md4c, zstd, Brotli, libpng, libjpeg-turbo, libtiff, libwebp, LittleCMS, SQLite,
libproxy, curl/GnuTLS/Nettle/libtasn1/p11-kit, Kerberos, OpenLDAP, Cyrus SASL and libsystemd
(and libsecret when built with keychain support). linuxdeploy copies the Debian copyright file of every bundled library into the AppImage
(`usr/share/doc/<package>/copyright`); those files are the notices for these libraries. Licence
texts they refer to as `/usr/share/common-licenses/<name>` are included under
`usr/share/common-licenses/` in the AppImage, and `usr/share/doc/onesimus/bundled-packages.txt`
lists the Debian package and version of every bundled file.

Several of these libraries are under the LGPL or GPL (for example GnuTLS, Nettle, libtasn1,
libidn2, libunistring, librtmp, libsystemd, libmount/libblkid, libkeyutils, libgomp/libatomic with
the GCC runtime library exception). Their complete corresponding source code, the Debian source
package of the listed version, is attached to the release as
`onesimus-<version>-appimage-debian-sources.tar`; <https://snapshot.debian.org/> keeps them as
well.

## Third-party code inside the Qt libraries

Qt's own libraries contain third-party code. The Debian Qt (`.deb` dependency, AppImage) links most
of it as separate system libraries, which the AppImage lists as above. The official Qt build used
for Windows and macOS contains parts of it inside the Qt libraries and plugins; depending on the
platform these include:
double-conversion (BSD-3-Clause), PCRE2 (BSD-3-Clause; sljit BSD-2-Clause), zlib (Zlib), TinyCBOR
(MIT), md4c (MIT), HarfBuzz (MIT), FreeType (FTL OR GPL-2.0-or-later), libpng (libpng-2.0),
libjpeg-turbo (IJG AND BSD-3-Clause), libwebp and libtiff (image format plugins), the easing
equations (BSD-3-Clause), SHA-3/RFC 6234 code and the sRGB ICC profile. The authoritative list with
copyright holders and licence texts for Qt 6.8 is "Licenses Used in Qt":
<https://doc.qt.io/qt-6.8/licenses-used-in-qt.html>, and the `qt_attribution.json` files in the Qt
source code linked above.

## Microsoft Visual C++ runtime (Windows)

The Windows build is made with Microsoft Visual C++ 2022. `windeployqt --compiler-runtime` adds
the Visual C++ runtime to the deployment folder that the MSI installs. The runtime is distributable
code under the Microsoft Visual Studio licence terms, not under an open-source licence:
<https://visualstudio.microsoft.com/license-terms/>.

## onesimus-proto

The `.proto` files of the Onesimus Director interfaces come from the git submodule `proto/`
(<https://github.com/Onesimus-io/onesimus-proto>), Copyright (C) 2025-2026 Jörg Bernau,
AGPL-3.0-only. They are part of Onesimus, not third-party code, and are listed here because they
live in a repository of their own.

## Licence texts

| File | Licence | Used by |
|---|---|---|
| [`LICENSES/LGPL-3.0-only.txt`](LICENSES/LGPL-3.0-only.txt) | GNU Lesser General Public License v3 | Qt |
| [`LICENSES/GPL-3.0-only.txt`](LICENSES/GPL-3.0-only.txt) | GNU General Public License v3 | QtGrpc; the base of the LGPL-3.0 |
| [`LICENSES/Apache-2.0.txt`](LICENSES/Apache-2.0.txt) | Apache License 2.0 | OpenSSL |
| [`LICENSES/MIT.txt`](LICENSES/MIT.txt) | MIT License | Feather icons |
| `LICENSE` | GNU Affero General Public License v3 | Onesimus, onesimus-proto |
