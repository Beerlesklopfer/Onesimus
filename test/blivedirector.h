// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file blivedirector.h
 * @brief Environment gate of the tests that need a live (test) Director
 *
 * Run from CTest without arguments, a live-Director test takes its target
 * from the environment and is skipped (exit code 77, SKIP_RETURN_CODE) when
 * ONESIMUS_LIVE_DIRECTOR is not set. Options given on the command line win.
 *
 *   ONESIMUS_LIVE_DIRECTOR       host[:port] of a test Director (port 9101)
 *   ONESIMUS_LIVE_PASSWORD       console password
 *   ONESIMUS_LIVE_CONSOLE        console name (default: the test's default)
 *   ONESIMUS_LIVE_DIRECTOR_NAME  Director name (default: the test's default)
 *   ONESIMUS_LIVE_MODE           legacy, psk or cert (where the test has it)
 *
 * Never point these at a production Director (test/README.md).
 */
#ifndef BLIVEDIRECTOR_H
#define BLIVEDIRECTOR_H

#include <QCommandLineParser>
#include <QString>
#include <QtGlobal>

#include <cstdio>

namespace BLiveDirector {

/// Exit code CTest reports as "skipped" (SKIP_RETURN_CODE 77)
constexpr int SkipExitCode = 77;

/// The environment value for an option the command line did not set
inline QString valueOrEnv(const QCommandLineParser &parser, const QString &option,
                          const char *envName)
{
    if (parser.isSet(option))
        return parser.value(option);
    const QString env = qEnvironmentVariable(envName);
    return env.isEmpty() ? parser.value(option) : env;
}

/**
 * @brief Whether the test should be skipped: no password on the command
 * line and ONESIMUS_LIVE_DIRECTOR not set. Prints the reason.
 */
inline bool skipWithoutDirector(const QCommandLineParser &parser)
{
    if (parser.isSet(QStringLiteral("password")) || qEnvironmentVariableIsSet("ONESIMUS_LIVE_DIRECTOR"))
        return false;
    std::fprintf(stdout, "SKIP: needs a test Director; set ONESIMUS_LIVE_DIRECTOR=host[:port] and "
                         "ONESIMUS_LIVE_PASSWORD (see test/README.md)\n");
    return true;
}

/// Host and port from ONESIMUS_LIVE_DIRECTOR unless --host / --port are given
inline void applyHostPort(const QCommandLineParser &parser, QString *host, int *port)
{
    const QString target = qEnvironmentVariable("ONESIMUS_LIVE_DIRECTOR");
    if (target.isEmpty())
        return;
    const int colon = target.lastIndexOf(QLatin1Char(':'));
    const QString envHost = colon > 0 ? target.left(colon) : target;
    const int envPort = colon > 0 ? target.mid(colon + 1).toInt() : 9101;
    if (!parser.isSet(QStringLiteral("host")))
        *host = envHost;
    if (!parser.isSet(QStringLiteral("port")) && envPort > 0)
        *port = envPort;
}

} // namespace BLiveDirector

#endif // BLIVEDIRECTOR_H
