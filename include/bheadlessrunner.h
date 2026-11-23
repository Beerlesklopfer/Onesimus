// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bheadlessrunner.h
 * @brief Headless mode: log in, send the commands one after the other, print
 *        the replies, leave with an exit status
 *
 * The session runs in API mode 2 like the GUI, so every reply is the
 * Director's JSON (one document per command), printed on standard output
 * followed by a line end. Messages about the connection and commands that
 * got no reply go to standard error.
 *
 * Exit status: 0 every command succeeded (or there was none: a login
 * check); 1 at least one command failed (an error reply, no reply in time,
 * or not sent); 2 no session (connection, login or loss of the connection).
 */
#ifndef BHEADLESSRUNNER_H
#define BHEADLESSRUNNER_H

#include <QIODevice>
#include <QObject>
#include <QStringList>
#include <QTimer>

#include "director/bdirector.h"

class BHeadlessRunner : public QObject
{
    Q_OBJECT

public:
    enum ExitCode {
        Success = 0,
        CommandFailed = 1,
        NoSession = 2,
    };

    /**
     * @param director The Director wrapper to use (not owned)
     * @param commands Console commands, sent in this order
     * @param out Replies (standard output)
     * @param err Connection messages and failures without a reply (standard error)
     */
    BHeadlessRunner(BDirector *director, const QStringList &commands,
                    QIODevice *out, QIODevice *err, QObject *parent = nullptr);

    /**
     * @brief The commands of a script: one per line
     *
     * Empty lines and lines starting with '#' are skipped; whitespace at both
     * ends of a line is dropped.
     */
    static QStringList scriptCommands(const QString &script);

    /// How long the login (up to Ready) may take; default 60 s
    void setConnectTimeout(int milliseconds) { m_connectTimer.setInterval(milliseconds); }

    /// Connects and runs the commands; finished() follows
    void start(const QString &host, int port, const QString &directorName,
               const QString &consoleName, const QString &password);

signals:
    /// Everything is done and the session is closed
    void finished(int exitCode);

private:
    void onReady();
    void onReply(BDirector::Command cmd, const QString &reply);
    void onCommandFailed(BDirector::Command cmd, const QString &args, const QString &error);
    void onSessionFailed(const QString &reason);
    void sendNext();
    void finish(int exitCode);
    void writeLine(QIODevice *device, const QString &text);

    BDirector *m_director;
    QStringList m_commands;
    QIODevice *m_out;
    QIODevice *m_err;
    QTimer m_connectTimer;
    QTimer m_closeTimer;
    int m_next = 0;
    bool m_ready = false;
    bool m_waiting = false;          ///< a command is out, its reply not yet in
    bool m_swallowFailure = false;   ///< an error reply is followed by its commandFailed()
    bool m_anyFailed = false;
    bool m_finished = false;
    int m_exitCode = Success;
};

#endif // BHEADLESSRUNNER_H
