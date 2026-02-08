#ifndef BCLIENTSMODEL_H
#define BCLIENTSMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMap>
#include <QPair>
#include "blogging.h"
#include "config/bconfigparser.h"
#include "director/bdirector.h"

// Debug logging prefixes for Clients Model
#define CLIENTS_DEBUG BLOG_DEBUG()
#define CLIENTS_WARNING BLOG_WARNING()
#define CLIENTS_CRITICAL BLOG_ERROR()

/**
 * @brief Table model for Bareos/Bacula backup clients
 * @version 1.0
 * @since 2026-01-28
 *
 * Displays client information in a table format with columns for:
 * - Name
 * - Address (IP/Hostname)
 * - Status (Online/Offline)
 * - Operating System
 * - Version
 * - Last Connection
 * - Job Count
 * - Total Backed Up Data
 */
class BClientsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /**
     * @brief Column indices for the table
     * @since 1.0
     */
    enum Columns {
        COL_NAME = 0,       ///< Client name
        COL_ADDRESS,        ///< IP address or hostname
        COL_PORT,           ///< FD port number
        COL_STATUS,         ///< Online/Offline status
        COL_OS,             ///< Operating system
        COL_VERSION,        ///< Client version
        COL_LAST_CONN,      ///< Last connection time
        COL_JOB_COUNT,      ///< Number of jobs
        COL_TOTAL_BYTES,    ///< Total backed up data
        COL_COUNT           ///< Total column count
    };

    /**
     * @brief Client status enum
     * @since 1.0
     */
    enum Status {
        STATUS_UNKNOWN = 0,
        STATUS_ONLINE,
        STATUS_OFFLINE
    };

    /**
     * @brief TLS authentication mode for config export
     */
    enum TlsMode {
        TLS_AUTO,   ///< Auto-detect from Director's show clients data
        TLS_PSK,    ///< TLS-PSK: Enable=yes, Require=no
        TLS_X509    ///< TLS x509: Enable=yes, Require=yes, + cert file paths
    };

    explicit BClientsModel(BDirector *director, QObject *parent = nullptr);

    // QAbstractTableModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @brief Loads clients from a JSON array
     * @param clients JSON array containing client objects
     * @since 1.0
     */
    void setClients(const QJsonArray &clients);

    /**
     * @brief Returns the client object at the given row
     * @param row Row index
     * @return QJsonObject containing client data
     * @since 1.0
     */
    QJsonObject clientAt(int row) const;

    /**
     * @brief Returns all unfiltered client objects (master data)
     * @return QJsonArray containing all clients
     * @since 1.0
     */
    QJsonArray allClients() const { return m_allClients; }

    /**
     * @brief Stores unfiltered client data and applies cached enrichments
     * @param clients JSON array from "llist clients" response
     * @since 2.9
     */
    void setAllClients(const QJsonArray &clients);

    /**
     * @brief Enriches client data with job statistics (bytes, count, last job time)
     * @param jobs JSON array of job/backup objects
     * @since 2.9
     */
    void enrichWithJobData(const QJsonArray &jobs);

    /**
     * @brief Enriches client data with address/port from "show clients" response
     * @param jsonData Raw JSON response string
     * @since 2.9
     */
    void enrichWithShowClientData(const QString &jsonData);

    /**
     * @brief Requests client list from Director
     * @since 2.9
     */
    void refresh();

    /**
     * @brief Clears all client data
     * @since 1.0
     */
    void clear();

    /**
     * @brief Returns an enriched client object merging llist + show + job data
     * @param row Row index in the filtered (displayed) table
     * @return QJsonObject with all available client data merged
     * @since 2.9
     */
    QJsonObject enrichedClient(int row) const;

    /**
     * @brief Builds Bareos config resources for a client (FD + Director side)
     * @param row Row index in the filtered (displayed) table
     * @param tlsMode TLS mode override (Auto detects from Director data)
     * @return List of BConfigResource objects ready for BResourceWidget
     * @since 2.9
     */
    QList<BConfigResource> clientResources(int row, TlsMode tlsMode = TLS_AUTO) const;

    /**
     * @brief Determines client status from data
     * @param client Client JSON object
     * @return Status enum value
     * @since 1.0
     */
    Status getClientStatus(const QJsonObject &client) const;

    /**
     * @brief Sets active online status for a client (from status client probe)
     * @param clientName Client name
     * @param status Online or Offline
     * @since 2.10
     */
    void setClientOnlineStatus(const QString &clientName, Status status);

    /**
     * @brief Sends status client=<name> for all known clients to probe online status
     * @since 2.10
     */
    void requestStatusChecks();

signals:
    /**
     * @brief Emitted when client data changes
     * @since 1.0
     */
    void dataChanged();

    /**
     * @brief Emitted when unfiltered master data changes (enrichment or new data)
     * @since 2.9
     */
    void allClientsChanged();

    /**
     * @brief Emitted to send command to Director
     * @param cmd Command to send
     * @param args Command arguments
     * @since 2.9
     */
    void sendCommand(const BDirector::Command cmd, const QString &args);

    /**
     * @brief Emitted when status message changes
     * @param message Status message text
     * @since 2.9
     */
    void statusMessageChanged(const QString &message);

private:
    /**
     * @brief Formats bytes into human-readable string
     * @param bytes Byte count
     * @return Formatted string (e.g., "1.5 GB")
     * @since 1.0
     */
    QString formatBytes(qint64 bytes) const;

    /**
     * @brief Returns status icon for given status
     * @param status Status enum value
     * @return QIcon for status
     * @since 1.0
     */
    QIcon getStatusIcon(Status status) const;

    /**
     * @brief Returns status text for given status
     * @param status Status enum value
     * @return Status string
     * @since 1.0
     */
    QString getStatusText(Status status) const;

    /**
     * @brief Returns status color for given status
     * @param status Status enum value
     * @return QColor for status background
     * @since 1.0
     */
    QColor getStatusColor(Status status) const;

    BDirector *m_director;              ///< Director connection

    // Data members
    QJsonArray m_allClients;            ///< Unfiltered master client data
    QJsonArray m_clients;               ///< Filtered client data (displayed in table)

    // Enrichment maps (resolved per-column in data())
    QMap<QString, QString> m_lastJobTimes;          ///< Client name → last job starttime
    QMap<QString, qint64> m_totalBytes;             ///< Client name → total backed up bytes
    QMap<QString, int> m_jobCounts;                 ///< Client name → number of jobs
    QMap<QString, QPair<QString, int>> m_addresses; ///< Client name → {address, port}
    QMap<QString, QJsonObject> m_showClientDetails; ///< Client name → full "show clients" data

    // Active status probe results (from "status client=<name>")
    QMap<QString, Status> m_clientOnlineStatus; ///< Client name → probed online/offline status

    // Deferred enrichment caches
    QJsonArray m_cachedJobs;            ///< Cached job data for deferred enrichment
    QString m_cachedShowClients;        ///< Cached "show clients" response for deferred enrichment
};

#endif // BCLIENTSMODEL_H
