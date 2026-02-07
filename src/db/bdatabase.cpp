#include "db/bdatabase.h"
#include "blogging.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

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

    BLOG_DEBUG() << "Database opened:" << m_dbPath;

    // Enable foreign keys
    QSqlQuery query(m_db);
    if (!query.exec("PRAGMA foreign_keys = ON")) {
        BLOG_WARNING() << "Failed to enable foreign keys:" << query.lastError().text();
    }

    // Check if database is new or needs migration
    if (!hasSchemaVersion()) {
        // New database - create schema
        BLOG_DEBUG() << "Creating new database schema...";
        emit schemaCreating();

        if (!createSchema()) {
            // Preserve the detailed error from createSchema() and add context
            QString detailedError = m_lastError;
            setLastError(tr("Failed to create database schema: %1").arg(detailedError));
            return false;
        }

        m_currentVersion = readSchemaVersion();
        BLOG_DEBUG() << "Database schema created, version:" << m_currentVersion;
    } else {
        // Existing database
        m_currentVersion = readSchemaVersion();
        BLOG_DEBUG() << "Existing database found, version:" << m_currentVersion;
    }

    // Check if migrations are needed (both for new and existing databases)
    if (m_currentVersion < LATEST_VERSION) {
        BLOG_DEBUG() << "Database migration needed:" << m_currentVersion << "->" << LATEST_VERSION;
        emit migrationStarted(m_currentVersion, LATEST_VERSION);

        if (!applyMigrations()) {
            // Preserve the detailed error and add context
            QString detailedError = m_lastError;
            setLastError(tr("Failed to apply database migrations: %1").arg(detailedError));
            return false;
        }

        m_currentVersion = readSchemaVersion();
        BLOG_DEBUG() << "Database migrated to version:" << m_currentVersion;
        emit migrationCompleted();
    }

    m_isReady = true;

    // Import built-in templates if needed (after schema is ready)
    if (needsTemplateImport()) {
        int count = importBuiltinTemplates();
        if (count < 0) {
            BLOG_WARNING() << "Failed to import built-in templates:" << m_lastError;
            // Non-fatal - continue with empty templates
        }
    }

    emit ready();

    BLOG_DEBUG() << "Database ready, version:" << m_currentVersion;
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
        BLOG_DEBUG() << "Database closed";
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
        BLOG_WARNING() << "Failed to read schema version:" << query.lastError().text();
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
        case 3:
            migrationPath = ":/sql/migrations/v3_unified_settings_history.sql";
            break;
        case 4:
            migrationPath = ":/sql/migrations/v4_add_system_to_settings.sql";
            break;
        // Future migrations:
        // case 5:
        //     migrationPath = ":/sql/migrations/v5_....sql";
        //     break;
        default:
            BLOG_WARNING() << "No migration file found for version" << ver;
            continue;
        }

        BLOG_DEBUG() << "Applying migration to version" << ver << ":" << migrationPath;

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
        BLOG_WARNING() << "Failed to open resource:" << resourcePath;
        return false;
    }

    QTextStream in(&file);
    QString sql = in.readAll();
    file.close();

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

    // Parse SQL into statements, handling BEGIN...END blocks (triggers, etc.)
    QStringList statements = splitSqlStatements(sql);

    for (const QString &statement : statements) {
        // Remove SQL comment lines (lines starting with --)
        QStringList lines = statement.split('\n');
        QStringList cleanedLines;
        for (const QString &line : lines) {
            QString trimmedLine = line.trimmed();
            if (!trimmedLine.isEmpty() && !trimmedLine.startsWith("--")) {
                cleanedLines.append(line);
            }
        }
        QString cleaned = cleanedLines.join('\n').trimmed();

        if (cleaned.isEmpty()) {
            continue;  // Skip empty statements
        }

        if (!query.exec(cleaned)) {
            QString error = tr("SQL execution failed: %1\nStatement: %2")
                                .arg(query.lastError().text())
                                .arg(cleaned.left(100));
            setLastError(error);
            BLOG_WARNING() << error;

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

QStringList BDatabase::splitSqlStatements(const QString &sql)
{
    QStringList statements;
    QString currentStatement;
    int beginEndDepth = 0;
    bool inString = false;
    QChar stringChar;

    int i = 0;
    while (i < sql.length()) {
        QChar c = sql[i];

        // Handle SQL comments (-- to end of line) - skip them entirely
        // Comments can contain apostrophes (e.g., "doesn't") that would confuse string parsing
        if (!inString && c == '-' && i + 1 < sql.length() && sql[i + 1] == '-') {
            // Skip to end of line
            while (i < sql.length() && sql[i] != '\n') {
                i++;
            }
            // Skip the newline too if present
            if (i < sql.length() && sql[i] == '\n') {
                currentStatement += '\n';  // Preserve newline for formatting
                i++;
            }
            continue;
        }

        // Handle string literals (don't parse keywords inside strings)
        if ((c == '\'' || c == '"') && !inString) {
            inString = true;
            stringChar = c;
            currentStatement += c;
            i++;
            continue;
        }

        if (inString) {
            currentStatement += c;
            if (c == stringChar) {
                // Check for escaped quote (doubled)
                if (i + 1 < sql.length() && sql[i + 1] == stringChar) {
                    currentStatement += sql[i + 1];
                    i += 2;
                    continue;
                }
                inString = false;
            }
            i++;
            continue;
        }

        // Check for BEGIN keyword (case insensitive)
        if (i + 5 <= sql.length()) {
            QString word = sql.mid(i, 5).toUpper();
            if (word == "BEGIN") {
                // Make sure it's a word boundary
                bool leftBoundary = (i == 0 || !sql[i - 1].isLetterOrNumber());
                bool rightBoundary = (i + 5 >= sql.length() || !sql[i + 5].isLetterOrNumber());
                if (leftBoundary && rightBoundary) {
                    beginEndDepth++;
                    currentStatement += sql.mid(i, 5);
                    i += 5;
                    continue;
                }
            }
        }

        // Check for END keyword (case insensitive)
        if (i + 3 <= sql.length()) {
            QString word = sql.mid(i, 3).toUpper();
            if (word == "END") {
                // Make sure it's a word boundary
                bool leftBoundary = (i == 0 || !sql[i - 1].isLetterOrNumber());
                bool rightBoundary = (i + 3 >= sql.length() || !sql[i + 3].isLetterOrNumber());
                if (leftBoundary && rightBoundary && beginEndDepth > 0) {
                    beginEndDepth--;
                    currentStatement += sql.mid(i, 3);
                    i += 3;
                    continue;
                }
            }
        }

        // Handle semicolon - only split if not inside BEGIN...END
        if (c == ';') {
            if (beginEndDepth == 0 && !inString) {
                currentStatement = currentStatement.trimmed();
                if (!currentStatement.isEmpty()) {
                    statements.append(currentStatement);
                }
                currentStatement.clear();
                i++;
                continue;
            }
        }

        currentStatement += c;
        i++;
    }

    // Don't forget the last statement (might not end with semicolon)
    currentStatement = currentStatement.trimmed();
    if (!currentStatement.isEmpty()) {
        statements.append(currentStatement);
    }

    return statements;
}

void BDatabase::setLastError(const QString &error)
{
    m_lastError = error;
    emit this->error(error);
    BLOG_WARNING() << "Database error:" << error;
}

QString BDatabase::defaultDatabasePath() const
{
    // Store database in user's application data directory
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(dataPath).filePath("onesimus.db");
}

bool BDatabase::needsTemplateImport() const
{
    if (!m_isReady) return false;

    QSqlQuery query(m_db);
    // Check if any built-in templates exist
    if (!query.exec("SELECT COUNT(*) FROM fileset_templates WHERE is_builtin = 1")) {
        return true;  // Table might not exist yet
    }

    if (query.next()) {
        return query.value(0).toInt() == 0;
    }

    return true;
}

int BDatabase::importBuiltinTemplates()
{
    if (!m_isReady) {
        setLastError(tr("Database not ready"));
        return -1;
    }

    BLOG_DEBUG() << "Importing built-in FileSet templates...";

    // List all template files from Qt resources
    QDir resourceDir(":/templates/filesets");
    QStringList filters;
    filters << "*.json";
    QStringList templateFiles = resourceDir.entryList(filters, QDir::Files);

    if (templateFiles.isEmpty()) {
        BLOG_DEBUG() << "No template files found in resources";
        return 0;
    }

    int imported = 0;
    QSqlQuery query(m_db);

    // Prepare insert/update query (UPSERT)
    query.prepare(R"(
        INSERT INTO fileset_templates (name, description, platform, category, is_builtin, content)
        VALUES (:name, :description, :platform, :category, 1, :content)
        ON CONFLICT(name) DO UPDATE SET
            description = excluded.description,
            platform = excluded.platform,
            category = excluded.category,
            content = excluded.content,
            updated_at = CURRENT_TIMESTAMP
        WHERE is_builtin = 1
    )");

    for (const QString &fileName : templateFiles) {
        QString resourcePath = ":/templates/filesets/" + fileName;
        QFile file(resourcePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            BLOG_WARNING() << "Failed to open template file:" << resourcePath;
            continue;
        }

        QByteArray jsonData = file.readAll();
        file.close();

        // Parse JSON to extract metadata
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isNull() || !doc.isObject()) {
            BLOG_WARNING() << "Invalid JSON in template file:" << resourcePath;
            continue;
        }

        QJsonObject obj = doc.object();

        // Extract metadata from JSON
        QString name = obj.value("name").toString();
        if (name.isEmpty()) {
            // Use filename without extension as fallback name
            name = QFileInfo(fileName).baseName();
        }

        QString description = obj.value("description").toString();
        QString platform = obj.value("platform").toString("all");
        QString category = obj.value("category").toString("custom");

        // Validate platform and category
        QStringList validPlatforms = {"linux", "windows", "macos", "all"};
        QStringList validCategories = {"system", "database", "web", "mail", "container", "custom"};

        if (!validPlatforms.contains(platform)) platform = "all";
        if (!validCategories.contains(category)) category = "custom";

        // Bind values
        query.bindValue(":name", name);
        query.bindValue(":description", description);
        query.bindValue(":platform", platform);
        query.bindValue(":category", category);
        query.bindValue(":content", QString::fromUtf8(jsonData));

        if (query.exec()) {
            imported++;
            BLOG_DEBUG() << "Imported template:" << name;
        } else {
            BLOG_WARNING() << "Failed to import template:" << name << "-" << query.lastError().text();
        }
    }

    BLOG_DEBUG() << "Imported" << imported << "built-in templates";
    return imported;
}
