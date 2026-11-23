// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BPROFILETLS_H
#define BPROFILETLS_H

#include "bconnectionprofile.h"
#include "director/bdirector.h"

#include <QFile>
#include <QSharedPointer>

/**
 * @brief The TLS configuration of the Director connection for a profile
 *
 * The one place that turns a connection profile into BDirector::TLSConfig,
 * for the connect dialog, Reconnect, auto-connect and the connection
 * wizard. It starts from a default-constructed configuration with new
 * QFile objects, so nothing of the profile used before (CA, certificate,
 * key, PFX password) survives a switch to another profile.
 */
inline BDirector::TLSConfig bTlsConfigFromProfile(const BConnectionProfile &profile)
{
    BDirector::TLSConfig config;
    if (profile.legacyAuth || !profile.tlsEnabled) {
        config.tlsEnable = false;
        config.tlsRequire = false;
        config.tlsPSKEnable = false;
        config.tlsVerifyPeer = false;
        return config;
    }

    config.tlsEnable = true;
    config.tlsRequire = true;
    config.tlsPSKEnable = profile.tlsUsePSK;
    // TLS-PSK authenticates with the password; certificates are checked
    // only in certificate mode
    config.tlsVerifyPeer = !profile.tlsUsePSK && profile.tlsVerifyPeer;
    config.tlsCipherList = profile.tlsCipherList;
    config.tlsAllowedCn = profile.tlsAllowedCn;

    // New QFile objects: copies of a TLSConfig share the ones they hold
    config.tlsCaCertFile = QSharedPointer<QFile>(new QFile(profile.tlsCaCertFile));
#ifdef Q_OS_WINDOWS
    config.tlsPfxFile = QSharedPointer<QFile>(new QFile(profile.tlsPfxFile));
    config.tlsPfxPassword = profile.tlsPfxPassword;
#else
    config.tlsCertFile = QSharedPointer<QFile>(new QFile(profile.tlsCertFile));
    config.tlsKeyFile = QSharedPointer<QFile>(new QFile(profile.tlsKeyFile));
#endif
    return config;
}

#endif // BPROFILETLS_H
