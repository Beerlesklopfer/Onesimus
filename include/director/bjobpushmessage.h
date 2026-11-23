// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BJOBPUSHMESSAGE_H
#define BJOBPUSHMESSAGE_H

#include <QString>

/**
 * @file bjobpushmessage.h
 * @brief Parser for job messages pushed by the Onesimus Director over gRPC
 *
 * The Director sends every job message as a ConsoleTextEvent whose text is
 * exactly
 * @code
 *   JobId=<id> <jobname>: <text>
 * @endcode
 * Message type and timestamp are not transmitted. A job name may contain
 * spaces and colons, so the split between name and text is only exact when
 * the job name is known (it is announced by the JobStatusEvent of the same
 * job). Without a known name the first ": " ends the name.
 *
 * Header-only on purpose: used by the gRPC transport and by the unit tests.
 */
struct BJobPushMessage
{
    bool valid = false;     ///< The text had the job message prefix
    quint64 jobId = 0;
    QString jobName;
    QString text;           ///< Message text, may contain several lines

    /**
     * @brief Parse a ConsoleTextEvent text
     * @param raw Event text
     * @param knownJobName Name of the job, if already known (exact split)
     */
    static BJobPushMessage parse(const QString &raw, const QString &knownJobName = QString())
    {
        BJobPushMessage msg;
        static const QLatin1String prefix("JobId=");
        if (!raw.startsWith(prefix)) {
            return msg;
        }

        int pos = prefix.size();
        const int idStart = pos;
        while (pos < raw.size() && raw.at(pos).isDigit()) {
            ++pos;
        }
        if (pos == idStart || pos >= raw.size() || raw.at(pos) != QLatin1Char(' ')) {
            return msg;
        }
        bool ok = false;
        const quint64 id = raw.mid(idStart, pos - idStart).toULongLong(&ok);
        if (!ok) {
            return msg;
        }
        const QString rest = raw.mid(pos + 1);

        if (!knownJobName.isEmpty() && rest.startsWith(knownJobName + QLatin1String(": "))) {
            msg.jobName = knownJobName;
            msg.text = rest.mid(knownJobName.size() + 2);
        } else {
            const int sep = rest.indexOf(QLatin1String(": "));
            if (sep >= 0) {
                msg.jobName = rest.left(sep);
                msg.text = rest.mid(sep + 2);
            } else if (rest.endsWith(QLatin1Char(':'))) {
                // Empty message text
                msg.jobName = rest.left(rest.size() - 1);
            } else {
                return msg;
            }
        }

        msg.jobId = id;
        msg.valid = true;
        return msg;
    }
};

#endif // BJOBPUSHMESSAGE_H
