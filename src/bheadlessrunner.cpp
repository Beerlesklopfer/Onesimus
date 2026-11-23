// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bheadlessrunner.cpp
 * @brief Headless mode (see the header)
 */

#include "bheadlessrunner.h"
#include "director/bdirectorreply.h"

BHeadlessRunner::BHeadlessRunner(BDirector *director, const QStringList &commands,
                                 QIODevice *out, QIODevice *err, QObject *parent)
    : QObject(parent)
    , m_director(director)
    , m_commands(commands)
    , m_out(out)
    , m_err(err)
{
    m_connectTimer.setSingleShot(true);
    m_connectTimer.setInterval(60000);
    connect(&m_connectTimer, &QTimer::timeout, this, [this]() {
        onSessionFailed(QStringLiteral("No session with the Director within %1 seconds")
                            .arg(m_connectTimer.interval() / 1000));
    });

    // After "quit": wait for the Director to close, but not for ever
    m_closeTimer.setSingleShot(true);
    m_closeTimer.setInterval(3000);
    connect(&m_closeTimer, &QTimer::timeout, this, [this]() { emit finished(m_exitCode); });

    connect(m_director, &BDirector::allResourcesLoaded, this, &BHeadlessRunner::onReady);
    connect(m_director, &BDirector::jsonResult, this, &BHeadlessRunner::onReply);
    connect(m_director, &BDirector::textResult, this, &BHeadlessRunner::onReply);
    connect(m_director, &BDirector::commandFailed, this, &BHeadlessRunner::onCommandFailed);
    connect(m_director, &BDirector::protocolError, this, &BHeadlessRunner::onSessionFailed);
    connect(m_director, &BDirector::authentificationSucceeded, this,
            [this](bool success, const QString &reason) {
        if (!success) {
            onSessionFailed(reason);
        }
    });
    connect(m_director, &BDirector::disconnected, this, [this]() {
        if (m_finished) {
            if (m_closeTimer.isActive()) {
                m_closeTimer.stop();
                emit finished(m_exitCode);
            }
            return;
        }
        // The reason (a refused login, say) may follow the closed
        // connection by a moment; give it the chance to be the message
        QTimer::singleShot(300, this, [this]() {
            onSessionFailed(QStringLiteral("The Director closed the connection"));
        });
    });
}

QStringList BHeadlessRunner::scriptCommands(const QString &script)
{
    QStringList commands;
    const QStringList lines = script.split(QLatin1Char('\n'));
    for (const QString &line : lines) {
        const QString command = line.trimmed();
        if (command.isEmpty() || command.startsWith(QLatin1Char('#'))) {
            continue;
        }
        commands << command;
    }
    return commands;
}

void BHeadlessRunner::start(const QString &host, int port, const QString &directorName,
                            const QString &consoleName, const QString &password)
{
    m_connectTimer.start();
    m_director->connect(host, port, directorName, consoleName, password);
}

void BHeadlessRunner::onReady()
{
    if (m_ready || m_finished) {
        return;
    }
    m_ready = true;
    m_connectTimer.stop();
    sendNext();
}

void BHeadlessRunner::onReply(BDirector::Command cmd, const QString &reply)
{
    if (cmd != BDirector::Command::Custom || !m_waiting || m_finished) {
        return;
    }
    m_waiting = false;
    writeLine(m_out, reply);
    if (BDirectorReply::isErrorReply(reply)) {
        // The Director also reports it through commandFailed(), right after
        m_anyFailed = true;
        m_swallowFailure = true;
    }
    sendNext();
}

void BHeadlessRunner::onCommandFailed(BDirector::Command cmd, const QString &args,
                                      const QString &error)
{
    if (cmd != BDirector::Command::Custom || m_finished) {
        return;
    }
    if (m_swallowFailure) {
        m_swallowFailure = false;   // the error reply printed in onReply()
        return;
    }
    if (!m_waiting) {
        return;
    }
    m_waiting = false;
    m_anyFailed = true;
    writeLine(m_err, QStringLiteral("%1: %2").arg(args, error));
    sendNext();
}

void BHeadlessRunner::onSessionFailed(const QString &reason)
{
    if (m_finished) {
        return;
    }
    writeLine(m_err, reason);
    finish(NoSession);
}

void BHeadlessRunner::sendNext()
{
    if (m_next >= m_commands.size()) {
        finish(m_anyFailed ? CommandFailed : Success);
        return;
    }
    m_waiting = true;
    m_director->doSend(BDirector::Command::Custom, m_commands.at(m_next++));
}

void BHeadlessRunner::finish(int exitCode)
{
    if (m_finished) {
        return;
    }
    m_finished = true;
    m_exitCode = exitCode;
    m_connectTimer.stop();

    if (m_director->connectionState() == BDirector::ConnectionState::Disconnected) {
        emit finished(m_exitCode);
        return;
    }
    // Say "quit" and wait for the Director to close the connection
    m_closeTimer.start();
    m_director->disconnect();
}

void BHeadlessRunner::writeLine(QIODevice *device, const QString &text)
{
    if (!device) {
        return;
    }
    QByteArray bytes = text.toUtf8();
    if (!bytes.endsWith('\n')) {
        bytes.append('\n');
    }
    device->write(bytes);
}
