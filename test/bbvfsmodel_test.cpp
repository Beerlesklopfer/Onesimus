// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bbvfsmodel_test.cpp
 * @brief The BVFS file browser model: paged listings and the root asked for
 *        by path (P0-16), late replies of abandoned requests dropped (P0-15)
 *
 * The command lines
 * follow the Bareos 25 sources (core/src/dird/ua_dotcmds.cc BvfsParseArg:
 * limit= default 2000, offset=, "path=" for the root; cats/bvfs.cc get_root())
 * and the Bareos WebUI (RestoreModel.php: limit=1000, "path=" for the root).
 * The replies are written by hand in the shape of the Director's JSON
 * output; they were not recorded from a Director.
 *
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>
#include <QSignalSpy>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "jobs/bbvfsmodel.h"
#include "jobs/bbvfsreplyrouter.h"

using Command = BDirector::Command;

namespace {

struct Sent {
    Command cmd;
    QString args;
};

QString json(const QJsonObject &result)
{
    return QString::fromUtf8(QJsonDocument(QJsonObject{{"jsonrpc", "2.0"}, {"id", 1}, {"result", result}})
                                 .toJson(QJsonDocument::Compact));
}

QString jobIdsReply(const QStringList &ids)
{
    QJsonArray array;
    for (const QString &id : ids)
        array.append(QJsonObject{{"id", id}});
    return json({{"jobids", array}});
}

QJsonObject dirEntry(int pathId, const QString &name, const QString &fullPath)
{
    return {{"type", "D"}, {"pathid", pathId}, {"fileid", 0}, {"jobid", 0},
            {"name", name}, {"fullpath", fullPath}};
}

/// One page of .bvfs_lsdirs: "." (and ".." unless root), then the given directories
QString dirsPage(int selfPathId, int parentPathId, const QList<QJsonObject> &dirs, bool withSpecial = true)
{
    QJsonArray array;
    if (withSpecial) {
        array.append(dirEntry(selfPathId, ".", "."));
        if (parentPathId > 0)
            array.append(dirEntry(parentPathId, "..", ".."));
    }
    for (const QJsonObject &d : dirs)
        array.append(d);
    return json({{"directories", array}});
}

QString filesPage(int pathId, int first, int count)
{
    QJsonArray array;
    for (int i = first; i < first + count; ++i) {
        array.append(QJsonObject{{"type", "F"}, {"pathid", pathId}, {"fileid", 1000 + i}, {"jobid", 5},
                                 {"name", QStringLiteral("file%1").arg(i, 5, 10, QChar('0'))},
                                 {"stat", QJsonObject{{"size", 10}, {"mtime", 0}, {"mode", 0100644}}}});
    }
    return json({{"files", array}});
}

QList<QJsonObject> subdirs(int firstPathId, int first, int count, const QString &parent)
{
    QList<QJsonObject> list;
    for (int i = first; i < first + count; ++i) {
        const QString name = QStringLiteral("d%1/").arg(i, 5, 10, QChar('0'));
        list << dirEntry(firstPathId + i, name, parent + name);
    }
    return list;
}

} // namespace

class BBvfsModelTest : public QObject
{
    Q_OBJECT

private slots:
    void listArgsUsesEmptyPathForRoot();
    void lastPageDetection();
    void rootListingTakesPathIdFromDirector();
    void directoryLargerThanOnePageIsComplete();
    void filesLargerThanOnePageAreComplete();
    void staleReplyOfDestroyedModelIsDropped();
    void staleReplyAfterResetIsDropped();
    void failedRequestDoesNotStallTheQueue();
    void lateChildrenOfCheckedDirectoryArriveChecked();
    void uncheckingInsideCheckedDirectoryKeepsTheRestSelected();
    void unixRootKeepsItsName();
};

// ---------------------------------------------------------------------------
// Model
// ---------------------------------------------------------------------------

void BBvfsModelTest::listArgsUsesEmptyPathForRoot()
{
    QCOMPARE(BBvfsModel::listArgs("1,2", 0, 0), QStringLiteral("jobid=1,2 path= limit=1000 offset=0"));
    QCOMPARE(BBvfsModel::listArgs("7", 42, 2000, 500), QStringLiteral("jobid=7 pathid=42 limit=500 offset=2000"));
}

void BBvfsModelTest::lastPageDetection()
{
    QVERIFY(BBvfsModel::isLastPage(0));
    QVERIFY(BBvfsModel::isLastPage(999));
    QVERIFY(!BBvfsModel::isLastPage(1000));
    // A Director that ignores limit= returned everything at once
    QVERIFY(BBvfsModel::isLastPage(1500));
}

void BBvfsModelTest::rootListingTakesPathIdFromDirector()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    BBvfsModel model(&router);

    model.loadJob(5, false);
    QCOMPARE(sent.last().cmd, Command::BvfsUpdate);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=5"));
    router.handleReply(Command::BvfsUpdate, "OK", false);

    // Never "pathid=1": the root is asked for by its path
    QCOMPARE(sent.last().cmd, Command::BvfsLsDirs);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=5 path= limit=1000 offset=0"));

    router.handleReply(Command::BvfsLsDirs,
                       dirsPage(17, 0, {dirEntry(18, "/", "/"), dirEntry(19, "C:/", "C:/")}), true);
    QCOMPARE(model.rootPathId(), 17);
    QCOMPARE(model.rowCount(), 2);
    QVERIFY(!model.isBusy());
}

void BBvfsModelTest::directoryLargerThanOnePageIsComplete()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    BBvfsModel model(&router);
    QSignalSpy finished(&model, &BBvfsModel::loadingFinished);

    model.loadJob(9, true);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=9"));
    router.handleReply(Command::BvfsGetJobIds, jobIdsReply({"3", "9"}), true);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=3,9"));
    router.handleReply(Command::BvfsUpdate, "{\"result\":{}}", true);

    // Page 1: "." plus 999 directories = a full page of 1000 rows
    router.handleReply(Command::BvfsLsDirs, dirsPage(1, 0, subdirs(100, 0, 999, "/")), true);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=3,9 path= limit=1000 offset=1000"));
    QCOMPARE(finished.size(), 0);
    QVERIFY(model.isBusy());

    // Page 2: a second full page (no "." on later pages)
    router.handleReply(Command::BvfsLsDirs, dirsPage(1, 0, subdirs(100, 999, 1000, "/"), false), true);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=3,9 path= limit=1000 offset=2000"));

    // Page 3: short, the last one
    router.handleReply(Command::BvfsLsDirs, dirsPage(1, 0, subdirs(100, 1999, 3, "/"), false), true);

    QCOMPARE(model.rowCount(), 999 + 1000 + 3);
    QCOMPARE(finished.size(), 1);
    QVERIFY(!model.isBusy());
    QCOMPARE(model.index(2001, 0).data().toString(), QStringLiteral("d02001"));
}

void BBvfsModelTest::filesLargerThanOnePageAreComplete()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    BBvfsModel model(&router);

    model.loadJob(5, false);
    router.handleReply(Command::BvfsUpdate, "OK", false);
    router.handleReply(Command::BvfsLsDirs, dirsPage(1, 0, {dirEntry(40, "/", "/")}), true);

    const QModelIndex slash = model.index(0, 0);
    model.loadFilesForDirectory(slash);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=5 pathid=40 limit=1000 offset=0"));
    router.handleReply(Command::BvfsLsFiles, filesPage(40, 0, 1000), true);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=5 pathid=40 limit=1000 offset=1000"));
    router.handleReply(Command::BvfsLsFiles, filesPage(40, 1000, 250), true);

    QCOMPARE(model.rowCount(slash), 1250);
    QVERIFY(!model.isBusy());

    // Selecting the directory and then removing one file keeps the 1249 others
    model.setCheckable(true);
    QVERIFY(model.setData(slash, Qt::Checked, Qt::CheckStateRole));
    QCOMPARE(model.selectedDirIds(), QList<int>{40});
    QVERIFY(model.setData(model.index(1249, 0, slash), Qt::Unchecked, Qt::CheckStateRole));
    QCOMPARE(model.selectedDirIds(), QList<int>{});
    QCOMPARE(model.selectedFileIds().size(), 1249);
}

void BBvfsModelTest::staleReplyOfDestroyedModelIsDropped()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });

    // Wizard A asks for the chain of job 5 and is closed before the reply
    auto *first = new BBvfsModel(&router);
    first->loadJob(5, true);
    delete first;

    // Wizard B asks for the chain of job 9
    BBvfsModel second(&router);
    second.loadJob(9, true);
    QCOMPARE(sent.size(), 2);

    // The late reply for job 5 arrives first and must not be taken by B
    router.handleReply(Command::BvfsGetJobIds, jobIdsReply({"1", "5"}), true);
    QCOMPARE(sent.size(), 2);
    QVERIFY(second.bvfsJobIds().isEmpty());

    // B's own reply
    router.handleReply(Command::BvfsGetJobIds, jobIdsReply({"2", "9"}), true);
    QCOMPARE(second.bvfsJobIds(), QStringLiteral("2,9"));
    QCOMPARE(sent.last().cmd, Command::BvfsUpdate);
    QCOMPARE(sent.last().args, QStringLiteral("jobid=2,9"));
}

void BBvfsModelTest::staleReplyAfterResetIsDropped()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    BBvfsModel model(&router);

    model.loadJob(5, true);          // chain of job 5 asked for
    model.loadJob(5, false);         // scope changed: single job, starts with bvfs_update
    QCOMPARE(sent.last().cmd, Command::BvfsUpdate);

    router.handleReply(Command::BvfsGetJobIds, jobIdsReply({"1", "5"}), true);
    QCOMPARE(model.bvfsJobIds(), QStringLiteral("5"));
    QCOMPARE(sent.size(), 2);
}

void BBvfsModelTest::failedRequestDoesNotStallTheQueue()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    BBvfsModel model(&router);
    QSignalSpy errors(&model, &BBvfsModel::errorOccurred);
    QSignalSpy finished(&model, &BBvfsModel::loadingFinished);

    model.loadJob(5, false);
    router.reset("The connection to the Director was lost.");

    QCOMPARE(errors.size(), 1);
    QCOMPARE(finished.size(), 1);
    QVERIFY(!model.isBusy());
}

void BBvfsModelTest::lateChildrenOfCheckedDirectoryArriveChecked()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    BBvfsModel model(&router);
    model.setCheckable(true);

    model.loadJob(5, false);
    router.handleReply(Command::BvfsUpdate, "OK", false);
    router.handleReply(Command::BvfsLsDirs, dirsPage(1, 0, {dirEntry(40, "/", "/")}), true);
    const QModelIndex slash = model.index(0, 0);

    // Checking does not load the subtree: the directory ID covers it
    const int before = sent.size();
    QVERIFY(model.setData(slash, Qt::Checked, Qt::CheckStateRole));
    QCOMPARE(sent.size(), before);
    QVERIFY(!model.isBusy());

    model.fetchMore(slash);
    router.handleReply(Command::BvfsLsDirs, dirsPage(40, 1, subdirs(100, 0, 2, "/")), true);
    QCOMPARE(model.index(0, 0, slash).data(Qt::CheckStateRole).toInt(), int(Qt::Checked));
    QCOMPARE(model.selectedDirIds(), QList<int>{40});
}

void BBvfsModelTest::uncheckingInsideCheckedDirectoryKeepsTheRestSelected()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    BBvfsModel model(&router);
    model.setCheckable(true);

    model.loadJob(5, false);
    router.handleReply(Command::BvfsUpdate, "OK", false);
    router.handleReply(Command::BvfsLsDirs, dirsPage(1, 0, {dirEntry(40, "/", "/")}), true);
    const QModelIndex slash = model.index(0, 0);
    QVERIFY(model.setData(slash, Qt::Checked, Qt::CheckStateRole));

    // Only the subdirectories are known when one of them is unchecked
    model.fetchMore(slash);
    router.handleReply(Command::BvfsLsDirs, dirsPage(40, 1, subdirs(100, 0, 3, "/")), true);
    QVERIFY(model.setData(model.index(0, 0, slash), Qt::Unchecked, Qt::CheckStateRole));

    // The files of "/" are fetched now, so none of them is lost
    QVERIFY(model.isBusy());
    QCOMPARE(sent.last().args, QStringLiteral("jobid=5 pathid=40 limit=1000 offset=0"));
    QCOMPARE(sent.last().cmd, Command::BvfsLsFiles);
    router.handleReply(Command::BvfsLsFiles, filesPage(40, 0, 2), true);
    QVERIFY(!model.isBusy());

    QCOMPARE(model.selectedDirIds(), (QList<int>{101, 102}));
    QCOMPARE(model.selectedFileIds(), (QStringList{"1000", "1001"}));
}

void BBvfsModelTest::unixRootKeepsItsName()
{
    QList<Sent> sent;
    BBvfsReplyRouter router([&](Command c, const QString &a) { sent.append({c, a}); });
    BBvfsModel model(&router);

    model.loadJob(5, false);
    router.handleReply(Command::BvfsUpdate, "OK", false);
    router.handleReply(Command::BvfsLsDirs,
                       dirsPage(1, 0, {dirEntry(40, "/", "/"), dirEntry(41, "C:/", "C:/")}), true);
    QCOMPARE(model.index(0, 0).data().toString(), QStringLiteral("/"));
    QCOMPARE(model.index(1, 0).data().toString(), QStringLiteral("C:"));

    model.fetchMore(model.index(0, 0));
    router.handleReply(Command::BvfsLsDirs, dirsPage(40, 1, {dirEntry(50, "etc/", "/etc/")}), true);
    QCOMPARE(model.index(0, 0, model.index(0, 0)).data().toString(), QStringLiteral("etc"));
    QCOMPARE(model.index(0, 0, model.index(0, 0)).data(BBvfsModel::FullPathRole).toString(),
             QStringLiteral("/etc/"));
}

QTEST_MAIN(BBvfsModelTest)
#include "bbvfsmodel_test.moc"
