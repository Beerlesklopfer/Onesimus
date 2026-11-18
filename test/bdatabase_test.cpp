/**
 * @file bdatabase_test.cpp
 * @brief Schema creation, migrations and repair of the local SQLite database
 *
 * Works on files in a temporary directory with the SQL scripts from the
 * application's resources. Runs without a Director (registered with CTest).
 * The built-in FileSet templates come from the same resource file.
 */

#include <QtTest>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTemporaryDir>

#include "db/bdatabase.h"

namespace {

bool hasTable(const QSqlDatabase &db, const QString &name, const QString &type = "table")
{
    QSqlQuery query(db);
    query.prepare("SELECT name FROM sqlite_master WHERE type=? AND name=?");
    query.addBindValue(type);
    query.addBindValue(name);
    return query.exec() && query.next();
}

QList<int> recordedVersions(const QSqlDatabase &db)
{
    QList<int> versions;
    QSqlQuery query(db);
    if (query.exec("SELECT version FROM schema_version ORDER BY version")) {
        while (query.next())
            versions.append(query.value(0).toInt());
    }
    return versions;
}

int rowCount(const QSqlDatabase &db, const QString &table)
{
    QSqlQuery query(db);
    if (query.exec("SELECT COUNT(*) FROM " + table) && query.next())
        return query.value(0).toInt();
    return -1;
}

/// The table accepts a template row and enforces its constraints
bool templateTableWorks(const QSqlDatabase &db)
{
    QSqlQuery query(db);
    if (!query.exec("INSERT INTO fileset_templates (name, platform, category, is_builtin, content) "
                    "VALUES ('test template', 'linux', 'system', 0, '{}')")) {
        qWarning() << query.lastError().text();
        return false;
    }
    // CHECK constraint on platform
    if (query.exec("INSERT INTO fileset_templates (name, platform, content) "
                   "VALUES ('bad platform', 'amiga', '{}')")) {
        return false;
    }
    return rowCount(db, "fileset_templates") >= 1;
}

} // namespace

class BDatabaseTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void everyVersionHasAMigrationScript();
    void freshDatabaseReachesLatestVersion();
    void migrationFromVersion4To5();
    void repairsVersion5WithoutTable();
    void failedMigrationIsRolledBack();
    void secondStartChangesNothing();
    void builtinTemplatesAreImported();

private:
    /// Create a database at the latest version and close it again
    void createLatest(const QString &path);
    /// Run statements on a closed database file through a connection of its own
    bool execRaw(const QString &path, const QStringList &statements);

    QTemporaryDir m_dir;
    int m_counter = 0;
    QString newPath() { return m_dir.filePath(QString("test%1.db").arg(++m_counter)); }
};

void BDatabaseTest::initTestCase()
{
    QVERIFY(m_dir.isValid());
    QVERIFY2(QSqlDatabase::isDriverAvailable("QSQLITE"), "Qt SQLite driver missing");
    QVERIFY2(QFile::exists(":/sql/schema/v1_initial.sql"), "SQL resources not linked into the test");
}

void BDatabaseTest::createLatest(const QString &path)
{
    BDatabase db;
    QVERIFY2(db.initialize(path), qPrintable(db.lastError()));
    QCOMPARE(db.currentVersion(), BDatabase::latestVersion());
    db.close();
}

bool BDatabaseTest::execRaw(const QString &path, const QStringList &statements)
{
    bool ok = true;
    {
        QSqlDatabase raw = QSqlDatabase::addDatabase("QSQLITE", "raw");
        raw.setDatabaseName(path);
        ok = raw.open();
        for (const QString &statement : statements) {
            QSqlQuery query(raw);
            if (ok && !query.exec(statement)) {
                qWarning() << statement << query.lastError().text();
                ok = false;
            }
        }
        raw.close();
    }
    QSqlDatabase::removeDatabase("raw");
    return ok;
}

void BDatabaseTest::everyVersionHasAMigrationScript()
{
    QCOMPARE(BDatabase::latestVersion(), 5);
    for (int version = 2; version <= BDatabase::latestVersion(); ++version) {
        const QString path = BDatabase::migrationResourcePath(version);
        QVERIFY2(!path.isEmpty(), qPrintable(QString("no script for version %1").arg(version)));
        QVERIFY2(QFile::exists(path), qPrintable(path));
    }
    // Unmapped versions are reported as such instead of being skipped
    QVERIFY(BDatabase::migrationResourcePath(BDatabase::latestVersion() + 1).isEmpty());
    QVERIFY(BDatabase::migrationResourcePath(1).isEmpty());
    QVERIFY(BDatabase::migrationResourcePath(0).isEmpty());
}

void BDatabaseTest::freshDatabaseReachesLatestVersion()
{
    // createSchema() is v1 only; a new database gets the rest from the migrations
    BDatabase db;
    QVERIFY2(db.initialize(newPath()), qPrintable(db.lastError()));
    QVERIFY(db.isReady());
    QCOMPARE(db.currentVersion(), 5);
    QCOMPARE(recordedVersions(db.database()), QList<int>({1, 2, 3, 4, 5}));

    QVERIFY(hasTable(db.database(), "fileset_templates"));
    QVERIFY(hasTable(db.database(), "idx_fileset_templates_platform", "index"));
    QVERIFY(hasTable(db.database(), "idx_fileset_templates_platform_category", "index"));
    QVERIFY(hasTable(db.database(), "fileset_templates_update_timestamp", "trigger"));

    QVERIFY(templateTableWorks(db.database()));
}

void BDatabaseTest::migrationFromVersion4To5()
{
    // A version 4 database as every installation has had it so far: version 5
    // was never recorded and its table never created
    const QString path = newPath();
    createLatest(path);
    QVERIFY(execRaw(path, {"DROP TABLE fileset_templates",
                           "DELETE FROM schema_version WHERE version = 5"}));

    BDatabase db;
    QSignalSpy migrationStarted(&db, &BDatabase::migrationStarted);
    QVERIFY2(db.initialize(path), qPrintable(db.lastError()));

    QCOMPARE(migrationStarted.count(), 1);
    QCOMPARE(migrationStarted[0][0].toInt(), 4);
    QCOMPARE(migrationStarted[0][1].toInt(), 5);
    QCOMPARE(db.currentVersion(), 5);
    QCOMPARE(recordedVersions(db.database()), QList<int>({1, 2, 3, 4, 5}));
    QVERIFY(hasTable(db.database(), "fileset_templates"));
    QVERIFY(hasTable(db.database(), "fileset_templates_update_timestamp", "trigger"));
    QVERIFY(templateTableWorks(db.database()));
}

void BDatabaseTest::repairsVersion5WithoutTable()
{
    // Version 5 is recorded, but the table does not exist
    const QString path = newPath();
    createLatest(path);
    QVERIFY(execRaw(path, {"DROP TABLE fileset_templates"}));

    BDatabase db;
    QSignalSpy migrationStarted(&db, &BDatabase::migrationStarted);
    QVERIFY2(db.initialize(path), qPrintable(db.lastError()));

    QCOMPARE(migrationStarted.count(), 0);   // Not a migration: the version was current
    QCOMPARE(db.currentVersion(), 5);
    // The repair does not record version 5 a second time
    QCOMPARE(recordedVersions(db.database()), QList<int>({1, 2, 3, 4, 5}));
    QVERIFY(hasTable(db.database(), "fileset_templates"));
    QVERIFY(hasTable(db.database(), "idx_fileset_templates_builtin", "index"));
    QVERIFY(hasTable(db.database(), "fileset_templates_update_timestamp", "trigger"));
    QVERIFY(templateTableWorks(db.database()));
}

void BDatabaseTest::failedMigrationIsRolledBack()
{
    // Version 4 database in which the v5 script must fail half way: its first
    // index name is already taken. Neither the version entry nor the table of
    // the failed script may remain.
    const QString path = newPath();
    createLatest(path);
    QVERIFY(execRaw(path, {"DROP TABLE fileset_templates",
                           "DELETE FROM schema_version WHERE version = 5",
                           "CREATE TABLE blocker (platform TEXT)",
                           "CREATE INDEX idx_fileset_templates_platform ON blocker(platform)"}));

    {
        BDatabase db;
        QVERIFY(!db.initialize(path));
        QVERIFY(!db.isReady());
        QVERIFY(!db.lastError().isEmpty());
        db.close();
    }

    {
        QSqlDatabase raw = QSqlDatabase::addDatabase("QSQLITE", "check");
        raw.setDatabaseName(path);
        QVERIFY(raw.open());
        QCOMPARE(recordedVersions(raw), QList<int>({1, 2, 3, 4}));
        QVERIFY(!hasTable(raw, "fileset_templates"));
        raw.close();
    }
    QSqlDatabase::removeDatabase("check");

    // Once the obstacle is gone the migration goes through
    QVERIFY(execRaw(path, {"DROP TABLE blocker"}));
    BDatabase db;
    QVERIFY2(db.initialize(path), qPrintable(db.lastError()));
    QCOMPARE(db.currentVersion(), 5);
    QVERIFY(hasTable(db.database(), "fileset_templates"));
}

void BDatabaseTest::secondStartChangesNothing()
{
    const QString path = newPath();
    createLatest(path);

    BDatabase db;
    QSignalSpy migrationStarted(&db, &BDatabase::migrationStarted);
    QSignalSpy schemaCreating(&db, &BDatabase::schemaCreating);
    QVERIFY2(db.initialize(path), qPrintable(db.lastError()));
    QCOMPARE(migrationStarted.count(), 0);
    QCOMPARE(schemaCreating.count(), 0);
    QCOMPARE(recordedVersions(db.database()), QList<int>({1, 2, 3, 4, 5}));

    // Data in the table survives the next start
    QVERIFY(templateTableWorks(db.database()));
    const int templates = rowCount(db.database(), "fileset_templates");
    db.close();

    BDatabase again;
    QVERIFY2(again.initialize(path), qPrintable(again.lastError()));
    QCOMPARE(rowCount(again.database(), "fileset_templates"), templates);
    QCOMPARE(recordedVersions(again.database()), QList<int>({1, 2, 3, 4, 5}));
}

void BDatabaseTest::builtinTemplatesAreImported()
{
    // The templates the import reads are in the resource file at all
    QVERIFY2(QFile::exists(":/templates/filesets/templates/filesets/linux_home.json"),
             "FileSet template resources not linked into the test");

    const QString path = newPath();
    int builtin = 0;
    {
        BDatabase db;
        QVERIFY2(db.initialize(path), qPrintable(db.lastError()));

        // initialize() imports them into a new database
        QSqlQuery query(db.database());
        QVERIFY(query.exec("SELECT COUNT(*) FROM fileset_templates WHERE is_builtin = 1"));
        QVERIFY(query.next());
        builtin = query.value(0).toInt();
        QVERIFY2(builtin >= 1, "no built-in template was imported");
        QVERIFY(!db.needsTemplateImport());

        // Each row carries the template's JSON and a valid platform
        QVERIFY(query.exec("SELECT name, platform, content FROM fileset_templates "
                           "WHERE is_builtin = 1"));
        while (query.next()) {
            QVERIFY(!query.value(0).toString().isEmpty());
            QVERIFY(QStringList({"linux", "windows", "macos", "all"})
                        .contains(query.value(1).toString()));
            QVERIFY(query.value(2).toString().trimmed().startsWith('{'));
        }

        // A second import updates the rows instead of adding to them
        QVERIFY(db.importBuiltinTemplates() >= 1);
        QVERIFY(query.exec("SELECT COUNT(*) FROM fileset_templates WHERE is_builtin = 1"));
        QVERIFY(query.next());
        QCOMPARE(query.value(0).toInt(), builtin);
        db.close();
    }

    // Nor does the next start
    BDatabase again;
    QVERIFY2(again.initialize(path), qPrintable(again.lastError()));
    QSqlQuery query(again.database());
    QVERIFY(query.exec("SELECT COUNT(*) FROM fileset_templates WHERE is_builtin = 1"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), builtin);
}

QTEST_GUILESS_MAIN(BDatabaseTest)
#include "bdatabase_test.moc"
