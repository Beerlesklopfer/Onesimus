#ifndef BDIRECTORMODEL_H
#define BDIRECTORMODEL_H

#include "db/bresourcemodel.h"

/**
 * @brief Model for Directors table
 *
 * Provides access to directors table with automatic settings history tracking.
 *
 * Table columns (use with column indexes or setHeaderData):
 * - 0: id
 * - 1: name
 * - 2: address
 * - 3: port
 * - 4: password_hash
 * - 5: description
 * - 6: dir_port
 * - 7: query_file
 * - 8: working_directory
 * - 9: pid_directory
 * - 10: scripts_directory
 * - 11: plugin_directory
 * - 12: sub_sys_directory
 * - 13: maximum_concurrent_jobs
 * - 14: tls_enable
 * - 15: tls_require
 * - 16: tls_verify_peer
 * - 17: tls_ca_certificate_file
 * - 18: tls_certificate
 * - 19: tls_key
 * - 20: tls_allowed_cn
 * - 21: backup_system
 * - 22: created_at
 * - 23: updated_at
 * - 24: last_connected_at
 * - 25: is_active
 *
 * @since 0.1.0.5
 */
class BDirectorModel : public BResourceModel
{
    Q_OBJECT

public:
    /**
     * @brief Column indexes for directors table
     */
    enum Column {
        Id = 0,
        Name = 1,
        Address = 2,
        Port = 3,
        PasswordHash = 4,
        Description = 5,
        DirPort = 6,
        QueryFile = 7,
        WorkingDirectory = 8,
        PidDirectory = 9,
        ScriptsDirectory = 10,
        PluginDirectory = 11,
        SubSysDirectory = 12,
        MaximumConcurrentJobs = 13,
        TlsEnable = 14,
        TlsRequire = 15,
        TlsVerifyPeer = 16,
        TlsCaCertificateFile = 17,
        TlsCertificate = 18,
        TlsKey = 19,
        TlsAllowedCn = 20,
        BackupSystem = 21,
        CreatedAt = 22,
        UpdatedAt = 23,
        LastConnectedAt = 24,
        IsActive = 25
    };

    /**
     * @brief Construct Director model
     *
     * @param parent Parent QObject
     * @param db Database connection
     */
    explicit BDirectorModel(QObject *parent = nullptr,
                           QSqlDatabase db = QSqlDatabase());

    /**
     * @brief Initialize model (call after construction)
     *
     * Loads data from database and sets up headers.
     *
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Get Director ID for a given row
     *
     * @param row Row index
     * @return Director ID, or -1 if invalid
     */
    int directorId(int row) const;

    /**
     * @brief Find row index by Director ID
     *
     * @param directorId Director ID
     * @return Row index, or -1 if not found
     */
    int findDirectorRow(int directorId) const;

    /**
     * @brief Get Director name for a given row
     *
     * @param row Row index
     * @return Director name
     */
    QString directorName(int row) const;

    /**
     * @brief Create new Director record
     *
     * @param name Director name
     * @param address Director address
     * @param port Director port
     * @param passwordHash Password hash (MD5)
     * @param backupSystem "bareos" or "bacula" (default: "bareos")
     * @return New Director ID, or -1 on error
     */
    int createDirector(const QString &name,
                      const QString &address,
                      int port,
                      const QString &passwordHash,
                      const QString &backupSystem = "bareos");

    /**
     * @brief Delete Director (soft delete - sets is_active = 0)
     *
     * @param directorId Director ID
     * @return true if successful
     */
    bool deleteDirector(int directorId);

    /**
     * @brief Filter to show only active Directors
     *
     * @param activeOnly If true, shows only is_active = 1
     */
    void setFilterActiveOnly(bool activeOnly = true);
};

#endif // BDIRECTORMODEL_H
