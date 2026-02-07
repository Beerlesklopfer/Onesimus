#ifndef BDATABASE_H
#define BDATABASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>

/**
 * @brief Database manager for Onesimus SQLite database
 *
 * Handles database initialization, schema creation, and migrations.
 * SQL scripts are loaded from Qt resources (:/sql/...).
 *
 * Features:
 * - Automatic schema creation on first run
 * - Version tracking and automatic migrations
 * - Transaction support for safe updates
 * - Error reporting
 *
 * @since 0.1.0
 */
class BDatabase : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construct database manager
     * @param parent Parent QObject
     */
    explicit BDatabase(QObject *parent = nullptr);

    /**
     * @brief Destructor - closes database connection
     */
    ~BDatabase();

    /**
     * @brief Initialize database connection
     * @param dbPath Path to SQLite database file (default: user data directory)
     * @return true if successful, false on error
     */
    bool initialize(const QString &dbPath = QString());

    /**
     * @brief Check if database is initialized and ready
     * @return true if database is open and ready
     */
    bool isReady() const;

    /**
     * @brief Get current database schema version
     * @return Current version number, or 0 if not initialized
     */
    int currentVersion() const;

    /**
     * @brief Get last error message
     * @return Error description
     */
    QString lastError() const;

    /**
     * @brief Get database connection
     * @return QSqlDatabase instance
     */
    QSqlDatabase database() const;

    /**
     * @brief Close database connection
     */
    void close();

    /**
     * @brief Import built-in FileSet templates from Qt resources
     *
     * Loads templates from :/templates/filesets/*.json and inserts them
     * into the fileset_templates table with is_builtin = 1.
     * Existing built-in templates are updated, user templates are preserved.
     *
     * @return Number of templates imported, or -1 on error
     */
    int importBuiltinTemplates();

    /**
     * @brief Check if built-in templates need to be imported
     * @return true if templates table is empty or missing built-ins
     */
    bool needsTemplateImport() const;

signals:
    /**
     * @brief Emitted when database initialization starts
     */
    void initializationStarted();

    /**
     * @brief Emitted when database schema is being created
     */
    void schemaCreating();

    /**
     * @brief Emitted when database migration starts
     * @param fromVersion Starting version
     * @param toVersion Target version
     */
    void migrationStarted(int fromVersion, int toVersion);

    /**
     * @brief Emitted when migration completes
     */
    void migrationCompleted();

    /**
     * @brief Emitted when database is ready
     */
    void ready();

    /**
     * @brief Emitted on error
     * @param error Error description
     */
    void error(const QString &error);

private:
    /**
     * @brief Create database schema from v1_initial.sql
     * @return true if successful
     */
    bool createSchema();

    /**
     * @brief Check if database has schema_version table
     * @return true if schema_version exists
     */
    bool hasSchemaVersion() const;

    /**
     * @brief Get current schema version from database
     * @return Version number, or 0 if not found
     */
    int readSchemaVersion() const;

    /**
     * @brief Apply migrations to bring database up to date
     * @return true if successful
     */
    bool applyMigrations();

    /**
     * @brief Execute SQL from Qt resource file
     * @param resourcePath Path to resource (e.g., ":/sql/schema/v1_initial.sql")
     * @return true if successful
     */
    bool executeSqlFromResource(const QString &resourcePath);

    /**
     * @brief Execute SQL script (may contain multiple statements)
     * @param sql SQL script text
     * @return true if successful
     */
    bool executeSqlScript(const QString &sql);

    /**
     * @brief Split SQL into individual statements
     *
     * Handles:
     * - BEGIN...END blocks (triggers)
     * - String literals with quotes
     * - SQL comments (-- to end of line)
     *
     * @param sql SQL script text
     * @return List of individual SQL statements
     */
    QStringList splitSqlStatements(const QString &sql);

    /**
     * @brief Set last error message
     * @param error Error description
     */
    void setLastError(const QString &error);

    /**
     * @brief Get default database path
     * @return Path in user data directory
     */
    QString defaultDatabasePath() const;

private:
    QSqlDatabase m_db;
    QString m_lastError;
    bool m_isReady;
    int m_currentVersion;
    QString m_dbPath;

    // Latest schema version (update when adding migrations)
    static constexpr int LATEST_VERSION = 5;
};

#endif // BDATABASE_H
