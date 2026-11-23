// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjobhistoryproto_test.cpp
 * @brief The typed ListJobs reply read as the command line's "llist jobs"
 */

#include <QTest>
#include <QTimeZone>

#include "director/bjobhistoryproto.h"
#include "schedules/model/bjobhistorystats.h"

namespace pb = onesimus::director::api;

class BJobHistoryProtoTest : public QObject
{
    Q_OBJECT

private:
    static pb::JobInfo job(quint32 id, const QString &name, const QString &start)
    {
        pb::JobInfo info;
        info.setJobid(id);
        info.setName(name);
        info.setClient(QStringLiteral("PDC-fd"));
        info.setType(pb::JobTypeGadget::JobType::JOB_TYPE_BACKUP);
        info.setLevel(pb::JobLevelGadget::JobLevel::JOB_LEVEL_INCREMENTAL);
        info.setJobstatus(pb::JobStatusGadget::JobStatus::JOB_STATUS_TERMINATED);
        info.setSchedtime(QStringLiteral("2026-11-20 23:30:00"));
        info.setStarttime(start);
        info.setEndtime(QStringLiteral("2026-11-20 23:40:00"));
        info.setRealendtime(QStringLiteral("2026-11-20 23:40:00"));
        info.setJobbytes(4298896);
        return info;
    }

private slots:
    void readsOnlyTheHistoryForm()
    {
        const auto ok = BJobHistoryProto::parseCommandLine(QStringLiteral("llist jobs days=30 reverse limit=5000"));
        QVERIFY(ok.valid);
        QCOMPARE(ok.days, 30u);
        QCOMPARE(ok.limit, 5000u);
        QVERIFY(!BJobHistoryProto::parseCommandLine(QStringLiteral("llist jobs days=30 limit=5000")).valid);
        QVERIFY(!BJobHistoryProto::parseCommandLine(QStringLiteral("list jobs days=30 reverse limit=5")).valid);
        QVERIFY(!BJobHistoryProto::parseCommandLine(QStringLiteral("llist jobs days=0 reverse limit=5")).valid);
        QVERIFY(!BJobHistoryProto::parseCommandLine(QStringLiteral("llist jobs days=3 reverse limit=5 client=x")).valid);
    }

    void commandAsksNewestFirst()
    {
        const auto command = BJobHistoryProto::toCommand({true, 14, 200});
        QVERIFY(command.llist());
        QVERIFY(command.reverse());
        QCOMPARE(command.days(), 14u);
        QCOMPARE(command.limit(), 200u);
    }

    void replyReadsLikeTheCommandLine()
    {
        pb::ListJobsResponse response;
        pb::JobInfo a = job(1161, QStringLiteral("Samba4-Weekly"), QStringLiteral("2026-11-21 02:00:16"));
        pb::JobInfo b = job(1160, QStringLiteral("BackupCatalog"), QStringLiteral("2026-11-20 23:30:47"));
        b.setLevel(pb::JobLevelGadget::JobLevel::JOB_LEVEL_FULL);
        b.setJobstatus(pb::JobStatusGadget::JobStatus::JOB_STATUS_WARNINGS);
        b.setSchedtime(QString());
        response.setJobs({a, b});

        const BJobHistoryReply reply = BJobHistoryReply::parse(BJobHistoryProto::toLlistReply(response));
        QCOMPARE(reply.outcome, BJobHistoryReply::Outcome::Runs);
        QCOMPARE(reply.rows, 2);
        QCOMPARE(reply.rowsWithScheduledTime, 1);
        QCOMPARE(reply.runs.size(), 2);
        const BJobRun &first = reply.runs.at(0);
        QCOMPARE(first.jobId, quint64(1161));
        QCOMPARE(first.name, QStringLiteral("Samba4-Weekly"));
        QCOMPARE(first.client, QStringLiteral("PDC-fd"));
        QCOMPARE(first.type, QChar('B'));
        QCOMPARE(first.level, QChar('I'));
        QCOMPARE(first.status, QChar('T'));
        QCOMPARE(first.started, QDateTime(QDate(2026, 11, 21), QTime(2, 0, 16), QTimeZone::UTC));
        QCOMPARE(first.scheduled, QDateTime(QDate(2026, 11, 20), QTime(23, 30), QTimeZone::UTC));
        QCOMPARE(reply.runs.at(1).level, QChar('F'));
        QCOMPARE(reply.runs.at(1).status, QChar('W'));
        QVERIFY(!reply.runs.at(1).scheduled.isValid());
    }

    void valuesAreStringsAndUnspecifiedIsEmpty()
    {
        pb::ListJobsResponse response;
        pb::JobInfo info = job(7, QStringLiteral("x"), QString());
        info.setType(pb::JobTypeGadget::JobType::JOB_TYPE_UNSPECIFIED);
        info.setLevel(pb::JobLevelGadget::JobLevel::JOB_LEVEL_NONE);
        info.setExtraFields({{QStringLiteral("comment"), QStringLiteral("hello")},
                             {QStringLiteral("jobid"), QStringLiteral("999")}});
        response.setJobs({info});
        const QString text = BJobHistoryProto::toLlistReply(response);
        QVERIFY(text.contains(QStringLiteral("\"jobid\":\"7\"")));       // typed wins over extra
        QVERIFY(text.contains(QStringLiteral("\"jobbytes\":\"4298896\"")));
        QVERIFY(text.contains(QStringLiteral("\"type\":\"\"")));
        QVERIFY(text.contains(QStringLiteral("\"level\":\" \"")));
        QVERIFY(text.contains(QStringLiteral("\"comment\":\"hello\"")));
    }

    void emptyReplyIsAnEmptyList()
    {
        const BJobHistoryReply reply = BJobHistoryReply::parse(BJobHistoryProto::toLlistReply(pb::ListJobsResponse()));
        QCOMPARE(reply.outcome, BJobHistoryReply::Outcome::Runs);
        QCOMPARE(reply.rows, 0);
    }

    void oldestFirstIsNoticed()
    {
        pb::ListJobsResponse newest;
        newest.setJobs({job(3, QStringLiteral("c"), QStringLiteral("2026-11-21 02:00:00")),
                        job(2, QStringLiteral("b"), QString()),
                        job(1, QStringLiteral("a"), QStringLiteral("2026-11-20 23:30:00"))});
        QVERIFY(BJobHistoryProto::newestFirst(newest));
        pb::ListJobsResponse oldest;
        oldest.setJobs({job(1, QStringLiteral("a"), QStringLiteral("2026-11-20 23:30:00")),
                        job(3, QStringLiteral("c"), QStringLiteral("2026-11-21 02:00:00"))});
        QVERIFY(!BJobHistoryProto::newestFirst(oldest));
        QVERIFY(BJobHistoryProto::newestFirst(pb::ListJobsResponse()));
    }
};

QTEST_GUILESS_MAIN(BJobHistoryProtoTest)
#include "bjobhistoryproto_test.moc"
