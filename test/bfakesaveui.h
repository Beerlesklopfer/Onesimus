// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BFAKESAVEUI_H
#define BFAKESAVEUI_H

// The dialogs of BSchedulePersistence for tests: the review and the
// reports are answered by the test, and everything asked is logged.

#include <QList>
#include <QStringList>

#include "bfakeconfigclient.h"
#include "schedules/bschedulepersistence.h"

/// The dialogs, answered by the test; everything they are asked is logged
class BFakeSaveUi : public BScheduleSaveUi
{
public:
    explicit BFakeSaveUi(BFakeConfigClient *client) : m_client(client) {}

    bool confirmAnswer = true;
    Answer reportAnswer = Close;
    QList<BScheduleReview> reviews;
    QList<BScheduleReport> reports;
    QStringList log;    ///< "confirm" and "report:<kind>", with the number of requests sent so far
    /// Texts "edited" in the reviews, one per review, in order; then confirmAnswer
    QStringList editedTexts;

    bool confirm(const BScheduleReview &review) override
    {
        reviews.append(review);
        log.append(m_client ? QStringLiteral("confirm@%1 writes=%2").arg(m_client->requests.size()).arg(m_client->writes().size())
                            : QStringLiteral("confirm@"));
        return confirmAnswer;
    }
    Decision decide(const BScheduleReview &review, QString *edited) override
    {
        if (editedTexts.isEmpty())
            return BScheduleSaveUi::decide(review, edited);
        reviews.append(review);
        log.append(m_client ? QStringLiteral("confirm@%1 writes=%2 edited").arg(m_client->requests.size()).arg(m_client->writes().size())
                            : QStringLiteral("confirm@ edited"));
        *edited = editedTexts.takeFirst();
        return Recheck;
    }
    Answer report(const BScheduleReport &report) override
    {
        reports.append(report);
        log.append(QStringLiteral("report:%1").arg(int(report.kind)));
        return reportAnswer;
    }

private:
    BFakeConfigClient *m_client;
};

#endif // BFAKESAVEUI_H
