// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjobcancel.h
 * @brief Bookkeeping of the Cancel action of the job list
 *
 * The toolbar Cancel acts on every selected job, in the order of their job
 * IDs, and reports what the Director answered for each instead of claiming
 * success. Replies come back in the order the commands were sent (the
 * Director works through its commands one after the other), so the n-th
 * reply belongs to the n-th job; a command that was never answered (refused,
 * timed out, connection lost) comes back through commandFailed() with its
 * job ID.
 *
 * No widgets and no Director: unit-tested in bjobcancel_test.
 */
#ifndef BJOBCANCEL_H
#define BJOBCANCEL_H

#include <QList>
#include <QPair>
#include <QSet>
#include <QString>
#include <QStringList>

#include <algorithm>

#include "director/bdirectorreply.h"

class BJobCancel
{
public:
    /// The selected job IDs that are job IDs (positive numbers), ascending
    static QStringList orderedJobIds(const QSet<QString> &selection)
    {
        QList<qulonglong> ids;
        for (const QString &text : selection) {
            bool ok = false;
            const qulonglong id = text.trimmed().toULongLong(&ok);
            if (ok && id > 0 && !ids.contains(id)) {
                ids.append(id);
            }
        }
        std::sort(ids.begin(), ids.end());
        QStringList out;
        for (const qulonglong id : ids) {
            out << QString::number(id);
        }
        return out;
    }

    /// Starts a run for @p jobIds (as sent, in this order)
    void start(const QStringList &jobIds)
    {
        m_pending = jobIds;
        m_requested.clear();
        m_failed.clear();
    }

    bool isRunning() const { return !m_pending.isEmpty(); }

    /**
     * @brief The Director answered the oldest cancel still open
     * @return false if no cancel was open (the reply is someone else's)
     */
    bool takeReply(const QString &reply)
    {
        if (m_pending.isEmpty()) {
            return false;
        }
        const QString jobId = m_pending.takeFirst();
        if (BDirectorReply::isErrorReply(reply)) {
            m_failed.append({jobId, BDirectorReply::errorText(reply)});
        } else {
            m_requested.append(jobId);
        }
        return true;
    }

    /**
     * @brief The cancel of @p jobId failed without a reply (or with an error
     *        reply already taken by takeReply())
     * @return false if that job was not open
     */
    bool takeFailure(const QString &jobId, const QString &error)
    {
        const QString id = jobId.trimmed();
        if (!m_pending.removeOne(id)) {
            return false;
        }
        m_failed.append({id, error});
        return true;
    }

    /// Jobs the Director accepted the cancel for
    QStringList requested() const { return m_requested; }

    /// Jobs whose cancel failed, with the reason
    QList<QPair<QString, QString>> failed() const { return m_failed; }

private:
    QStringList m_pending;
    QStringList m_requested;
    QList<QPair<QString, QString>> m_failed;
};

#endif // BJOBCANCEL_H
