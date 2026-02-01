#include "bdatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

BDatabase::BDatabase(QObject *parent)
    : QObject(parent)
    , m_isReady(false)
    , m_currentVersion(0)
{
}

BDatabase::~BDatabase()
{
    close();
}

bool BDatabase::initialize(const QString &dbPath)
{
    emit initializationStarted();

    // Determine database path
    m_dbPath = dbPath.isEmpty() ? defaultDatabasePath() : dbPath;

    // Create directory if needed
    QFileInfo fileInfo(m_dbPath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            setLastError(tr("Failed to create database directory: %1").arg(dir.path()));
            return false;
        }
    }

    // Open database connection
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbPath);

    if (!m_db.open()) {
        setLastError(tr("Failed to open database: %1").arg(m_db.lastError().text()));
        return false;
    }

    qDebug() << "Database opened:" << m_dbPath;

    // Enable foreign keys
    QSqlQuery query(m_db);
    if (!query.exec("PRAGMA foreign_keys = ON")) {
        qWarning() << "Failed to enable foreign keys:" << query.lastError().text();
    }

    // Check if database is new or needs migration
    if (!hasSchemaVersion()) {
        // New database - create schema
        qDebug() << "Creating new database schema...";
        emit schemaCreating();

        if (!createSchema()) {
            setLastError(tr("Failed to create database schema"));
            return false;
        }

        m_currentVersion = readSchemaVersion();
        qDebug() << "Database schema created, version:" << m_currentVersion;
    } else {
        // Existing database - check for migrations
        m_currentVersion = readSchemaVersion();
        qDebug() << "Existing database found, version:" << m_currentVersion;

        if (m_currentVersion < LATEST_VERSION) {
            qDebug() << "Database migration needed:" << m_currentVersion << "->" << LATEST_VERSION;
            emit migrationStarted(m_currentVersion, LATEST_VERSION);

            if (!applyMigrations()) {
                setLastError(tr("Failed to apply database migrations"));
                return false;
            }

            m_currentVersion = readSchemaVersion();
            qDebug() << "Database migrated to version:" << m_currentVersion;
            emit migrationCompleted();
        }
    }

    m_isReady = true;
    emit ready();

    qDebug() << "Database ready, version:" << m_currentVersion;
    return true;
}

bool BDatabase::isReady() const
{
    return m_isReady;
}

int BDatabase::currentVersion() const
{
    return m_currentVersion;
}

QString BDatabase::lastError() const
{
    return m_lastError;
}

QSqlDatabase BDatabase::database() const
{
    return m_db;
}

void BDatabase::close()
{
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Database closed";
    }
    m_isReady = false;
}

bool BDatabase::createSchema()
{
    return executeSqlFromResource(":/sql/schema/v1_initial.sql");
}

bool BDatabase::hasSchemaVersion() const
{
    QSqlQuery query(m_db);
    // Check if schema_version table exists
    if (!query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='schema_version'")) {
        return false;
    }

    return query.next();
}

int BDatabase::readSchemaVersion() const
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT MAX(version) FROM schema_version")) {
        qWarning() << "Failed to read schema version:" << query.lastError().text();
        return 0;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

bool BDatabase::applyMigrations()
{
    int currentVer = m_currentVersion;

    // Apply migrations sequentially
    for (int ver = currentVer + 1; ver <= LATEST_VERSION; ++ver) {
        QString migrationPath;

        // Map version to migration file
        switch (ver) {
        case 2:
            migrationPath = ":/sql/migrations/v2_add_console_support.sql";
            break;
        // Add more migrations here as they are created
        default:
            qWarning() << "No migration file found for version" << ver;
            continue;
        }

        qDebug() << "Applying migration to version" << ver << ":" << migrationPath;

        if (!executeSqlFromResource(migrationPath)) {
            setLastError(tr("Failed to apply migration to version %1").arg(ver));
            return false;
        }
    }

    return true;
}

bool BDatabase::executeSqlFromResource(const QString &resourcePath)
{
    // Load SQL from Qt resource
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setLastError(tr("Failed to open SQL resource: %1").arg(resourcePath));
        qWarning() << "Failed to open resource:" << resourcePath;
        return false;
    }

    QTextStream in(&file);
    QString sql = in.readAll();
    file.close();

    qDebug() << "Executing SQL from" << resourcePath << "(" << sql.length() << "bytes)";

    return executeSqlScript(sql);
}

bool BDatabase::executeSqlScript(const QString &sql)
{
    // Start transaction for atomicity
    if (!m_db.transaction()) {
        setLastError(tr("Failed to start transaction: %1").arg(m_db.lastError().text()));
        return false;
    }

    QSqlQuery query(m_db);

    // Split SQL into statements (simple approach - split by semicolon)
    // Note: This doesn't handle semicolons in strings, but works for our schema files
    QStringList statements = sql.split(';', Qt::SkipEmptyParts);

    for (const QString &statement : statements) {
        QString trimmed = statement.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith("--")) {
            continue;  // Skip empty lines and comments
        }

        qDebug() << "Executing:" << trimmed.left(50) << "...";

        if (!query.exec(trimmed)) {
            QString error = tr("SQL execution failed: %1\nStatement: %2")
                                .arg(query.lastError().text())
                                .arg(trimmed.left(100));
            setLastError(error);
            qWarning() << error;

            // Rollback on error
            m_db.rollback();
            return false;
        }
    }

    // Commit transaction
    if (!m_db.commit()) {
        setLastError(tr("Failed to commit transaction: %1").arg(m_db.lastError().text()));
        m_db.rollback();
        return false;
    }

    return true;
}

void BDatabase::setLastError(const QString &error)
{
    m_lastError = error;
    emit this->error(error);
    qWarning() << "Database error:" << error;
}

QString BDatabase::defaultDatabasePath() const
{
    // Store database in user's application data directory
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(dataPath).filePath("onesimus.db");
}
