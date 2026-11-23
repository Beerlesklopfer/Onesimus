// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bcommandlineconnection.h
 * @brief Maps the connection options of the command line to the Director configuration
 *
 * Kept apart from main.cpp so that the mapping can be tested: --tls used to
 * write every file name into the CA field and never switched TLS on.
 */
#ifndef BCOMMANDLINECONNECTION_H
#define BCOMMANDLINECONNECTION_H

#include <QCoreApplication>
#include <QFile>
#include <QSharedPointer>
#include <QString>

#include "director/bareosdirector.h"

/// TLS options as given on the command line (empty = not given)
struct BCommandLineTlsOptions {
    bool tls = false;       ///< --tls
    QString caFile;         ///< --tls-ca
    QString certFile;       ///< --tls-cert (Linux, macOS)
    QString keyFile;        ///< --tls-key (Linux, macOS)
    QString pfxFile;        ///< --tls-pfx (Windows)
    QString pfxPassword;    ///< --tls-pfx-password (Windows)

    bool hasCertificateOptions() const
    {
        return !caFile.isEmpty() || !certFile.isEmpty() || !keyFile.isEmpty()
               || !pfxFile.isEmpty() || !pfxPassword.isEmpty();
    }
};

/**
 * @brief Write the TLS options of the command line into a Director TLS configuration
 *
 * - without --tls nothing is changed; a --tls-* option without --tls is an
 *   error, because the user expects an encrypted connection
 * - --tls alone: TLS-PSK with the console password (the Bareos default)
 * - --tls with certificate options: certificate TLS, the Director's
 *   certificate is verified (against --tls-ca if given, otherwise against
 *   the system's certificate store)
 * - in both cases TLS is enabled and required
 *
 * Every option goes into its own field; files must exist.
 *
 * @return an empty string, or the error to show to the user (@p config is
 *         then unchanged)
 */
inline QString bApplyCommandLineTls(const BCommandLineTlsOptions &options,
                                    BareosDirector::TLSConfig &config)
{
    const auto text = [](const char *source) {
        return QCoreApplication::translate("CommandLine", source);
    };

    if (!options.tls) {
        if (options.hasCertificateOptions()) {
            return text("The --tls-* options have no effect without --tls. "
                        "Add --tls to connect with TLS.");
        }
        return QString();
    }

    if (options.certFile.isEmpty() != options.keyFile.isEmpty()) {
        return text("--tls-cert and --tls-key must be given together.");
    }
    if (!options.pfxPassword.isEmpty() && options.pfxFile.isEmpty()) {
        return text("--tls-pfx-password needs --tls-pfx.");
    }
#ifdef Q_OS_WINDOWS
    if (!options.certFile.isEmpty()) {
        return text("--tls-cert and --tls-key are not available on Windows; use --tls-pfx.");
    }
#else
    if (!options.pfxFile.isEmpty()) {
        return text("--tls-pfx is only available on Windows; use --tls-cert and --tls-key.");
    }
#endif
    for (const QString &file : {options.caFile, options.certFile, options.keyFile, options.pfxFile}) {
        if (!file.isEmpty() && !QFile::exists(file)) {
            return text("TLS file not found: %1").arg(file);
        }
    }

    const bool certificateMode = options.hasCertificateOptions();

    config.tlsEnable = true;
    config.tlsRequire = true;
    config.tlsPSKEnable = !certificateMode;
    config.tlsVerifyPeer = certificateMode;

    // New QFile objects: copies of a TLSConfig share the ones they hold
    config.tlsCaCertFile = QSharedPointer<QFile>(new QFile(options.caFile));
#ifdef Q_OS_WINDOWS
    config.tlsPfxFile = QSharedPointer<QFile>(new QFile(options.pfxFile));
    config.tlsPfxPassword = options.pfxPassword;
#else
    config.tlsCertFile = QSharedPointer<QFile>(new QFile(options.certFile));
    config.tlsKeyFile = QSharedPointer<QFile>(new QFile(options.keyFile));
#endif

    return QString();
}

/**
 * @brief Console password from the command line, a file or the environment
 *
 * -P/--password shows the password in the process list; --password-file
 * (first line of the file) and the environment variable ONESIMUS_PASSWORD
 * do not. Order: -P, then --password-file, then the environment variable.
 * -P together with --password-file is an error.
 *
 * @param error Set to the message to show if the password cannot be determined
 * @return the password; empty if none was given or on error
 */
inline QString bResolveCommandLinePassword(bool passwordOptionSet, const QString &passwordOption,
                                           const QString &passwordFile,
                                           const QString &environmentPassword, QString *error)
{
    const auto text = [](const char *source) {
        return QCoreApplication::translate("CommandLine", source);
    };

    if (passwordOptionSet && !passwordFile.isEmpty()) {
        *error = text("Give either --password or --password-file, not both.");
        return QString();
    }
    if (passwordOptionSet) {
        return passwordOption;
    }
    if (!passwordFile.isEmpty()) {
        QFile file(passwordFile);
        if (!file.open(QIODevice::ReadOnly)) {
            *error = text("Cannot read the password file %1: %2").arg(passwordFile, file.errorString());
            return QString();
        }
        // First line, without the line ending (LF or CRLF)
        QByteArray line = file.readLine();
        while (line.endsWith('\n') || line.endsWith('\r')) {
            line.chop(1);
        }
        if (line.isEmpty()) {
            *error = text("The password file %1 is empty.").arg(passwordFile);
            return QString();
        }
        return QString::fromUtf8(line);
    }
    return environmentPassword;
}

#endif // BCOMMANDLINECONNECTION_H
