// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file bfakebnetdirector.h
 * @brief An honest fake Director on the loopback interface: login and session
 *
 * The login is the Director's side of the Bareos console handshake (Bareos 25
 * cram_md5.cc, CramMd5Handshake::DoHandshake() with initiated_by_remote =
 * true: challenge first, then answer the console's challenge), in clear text.
 *
 * The session follows what the Onesimus Director serves on its Director port
 * (onesimus-sim src/server/bnet.ts BnetServer.loop(), after odir
 * crates/odir/src/session.rs): in API mode 0 a reply ends with EOD; in API
 * modes 1 and 2 the Director sends MAIN_PROMPT before it reads a command and
 * frames a command with CMD_BEGIN ... CMD_OK, then the reply. Replies can be
 * cut into telegrams at any byte, as a Director does at its packet size.
 *
 * The BNET helpers (Bareos base64, the CRAM-MD5 answer, framing) are in
 * bbnettesthelpers.h, shared with bareosauth_security_test.cpp.
 */
#ifndef BFAKEBNETDIRECTOR_H
#define BFAKEBNETDIRECTOR_H

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QRegularExpression>
#include <QSet>
#include <QTcpServer>
#include <QTcpSocket>

#include "bbnettesthelpers.h"


/**
 * @brief A Director that logs a console in and answers its commands
 *
 * Every command is recorded in commands. The reply to a command line is
 * looked up in setReply(); without one the Director answers
 * {"jsonrpc":"2.0","id":null,"result":{}} in API mode 2 and an empty line in
 * API mode 0. A command named in setSilent() gets no reply at all.
 */
class BFakeBnetDirector : public QObject
{
    Q_OBJECT

public:
    explicit BFakeBnetDirector(const QString &password, QObject *parent = nullptr)
        : QObject(parent), m_password(password)
    {
    }

    bool listen()
    {
        connect(&m_server, &QTcpServer::pendingConnectionAvailable, this, [this]() {
            attach(m_server.nextPendingConnection());
        });
        return m_server.listen(QHostAddress::LocalHost);
    }

    quint16 port() const { return m_server.serverPort(); }

    /// The reply to @p command, cut into telegrams of @p chunk bytes (0: one)
    void setReply(const QByteArray &command, const QByteArray &reply, int chunk = 0)
    {
        m_replies.insert(command, {reply, chunk});
    }

    /// @p command gets no reply (a Director that hangs)
    void setSilent(const QByteArray &command) { m_silent.insert(command); }

    /// A reply in API mode 2 framing that no command asked for now (the late
    /// reply to a command that timed out)
    void sendLateReply(const QByteArray &text)
    {
        sendSignal(BFakeBnet::CmdBegin);
        sendSignal(BFakeBnet::CmdOk);
        send(text);
        sendSignal(BFakeBnet::MainPrompt);
    }

    /// Closes the session (the Director goes away)
    void dropConnection()
    {
        if (m_socket)
            m_socket->abort();
    }

    QList<QByteArray> commands;   ///< every command line, without the line end
    bool loggedIn = false;
    bool sessionClosed = false;   ///< the console closed the connection
    int apiMode = 0;

private:
    struct Reply
    {
        QByteArray text;
        int chunk = 0;
    };

    void attach(QTcpSocket *socket)
    {
        if (!socket)
            return;
        // A new console: a new login
        m_socket = socket;
        m_buffer.clear();
        m_step = 0;
        loggedIn = false;
        apiMode = 0;
        connect(socket, &QTcpSocket::readyRead, this, &BFakeBnetDirector::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, [this]() { sessionClosed = true; });
        if (socket->bytesAvailable() > 0)
            QMetaObject::invokeMethod(this, &BFakeBnetDirector::onReadyRead, Qt::QueuedConnection);
    }

    static bool debug() { return qEnvironmentVariableIsSet("FAKE_BNET_DEBUG"); }

    void send(const QByteArray &payload)
    {
        if (debug())
            qDebug() << "fake Director sends" << payload.left(200);
        m_socket->write(BFakeBnet::frame(payload));
    }

    void sendSignal(qint32 value) { m_socket->write(BFakeBnet::signal(value)); }

    void onReadyRead()
    {
        if (!m_socket)
            return;
        m_buffer.append(m_socket->readAll());
        while (m_buffer.size() >= 4) {
            const qint32 len = qFromBigEndian<qint32>(reinterpret_cast<const uchar *>(m_buffer.constData()));
            if (len < 0) {
                m_buffer.remove(0, 4);
                if (len == BFakeBnet::Terminate && m_socket)
                    m_socket->disconnectFromHost();
                continue;
            }
            if (m_buffer.size() < 4 + len)
                return;
            const QByteArray message = m_buffer.mid(4, len);
            m_buffer.remove(0, 4 + len);
            if (debug())
                qDebug() << "fake Director receives" << message;
            if (loggedIn)
                command(message);
            else
                login(message);
            if (!m_socket)
                return;
        }
    }

    void login(const QByteArray &message)
    {
        switch (m_step++) {
        case 0: // Hello -> our challenge
            m_challenge = "<1804289383.1763884800@R_DIRECTOR::test-dir>";
            send("auth cram-md5 " + m_challenge + " ssl=0\n");
            break;
        case 1: { // console's HMAC
            const QByteArray answer = message.trimmed();
            const bool ok = answer == BFakeBnet::hmacAnswer(m_challenge, m_password, false)
                            || answer == BFakeBnet::hmacAnswer(m_challenge, m_password, true);
            send(ok ? QByteArray("1000 OK auth\n") : QByteArray("1999 Authorization failed.\n"));
            break;
        }
        case 2: { // console's challenge -> our HMAC
            static const QRegularExpression rx(QStringLiteral("auth cram-md5c? (<[^>]+>) ssl=(\\d+)"));
            const QByteArray challenge = rx.match(QString::fromLatin1(message)).captured(1).toLatin1();
            QByteArray answer = BFakeBnet::hmacAnswer(challenge, m_password, false);
            answer.append('\0');   // Bareos sends the terminating NUL
            send(answer);
            break;
        }
        case 3: // console's verdict on our HMAC, then the 18.2 banner
            if (message.startsWith("1000 OK auth")) {
                send("1000\x1eOK: test-dir Version: 25.1.2 (13 November 2026)");
                send("1002\x1eYou are logged in as: admin");
                loggedIn = true;
            }
            break;
        default:
            break;
        }
    }

    void command(const QByteArray &message)
    {
        const QByteArray line = message.trimmed();
        commands.append(line);

        if (line == "quit") {
            m_socket->disconnectFromHost();
            return;
        }
        if (m_silent.contains(line))
            return;

        const int modeBefore = apiMode;
        Reply reply;
        if (line.startsWith(".api ")) {
            apiMode = line.mid(5).toInt();
            reply.text = apiMode == 2 ? QByteArray("{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{\"api\":2}}")
                                      : "api: " + QByteArray::number(apiMode) + "\n";
        } else if (m_replies.contains(line)) {
            reply = m_replies.value(line);
        } else {
            reply.text = apiMode == 2 ? QByteArray("{\"jsonrpc\":\"2.0\",\"id\":null,\"result\":{}}")
                                      : QByteArray("\n");
        }

        // CMD_BEGIN in the mode before the command (".api" switches after it)
        if (modeBefore != 0)
            sendSignal(BFakeBnet::CmdBegin);
        if (apiMode != 0)
            sendSignal(BFakeBnet::CmdOk);

        const int chunk = reply.chunk > 0 ? reply.chunk : int(reply.text.size());
        for (qsizetype off = 0; off < reply.text.size(); off += chunk)
            send(reply.text.mid(off, chunk));

        if (apiMode == 0)
            sendSignal(BFakeBnet::Eod);
        else
            sendSignal(BFakeBnet::MainPrompt);
    }

    QString m_password;
    QTcpServer m_server;
    QPointer<QTcpSocket> m_socket;
    QByteArray m_buffer;
    QByteArray m_challenge;
    int m_step = 0;
    QHash<QByteArray, Reply> m_replies;
    QSet<QByteArray> m_silent;
};

#endif // BFAKEBNETDIRECTOR_H
