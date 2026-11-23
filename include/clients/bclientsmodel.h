// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BCLIENTSMODEL_H
#define BCLIENTSMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QHash>
#include <QMap>
#include <QSet>
#include <QPair>
#include <QTimer>
#include "blogging.h"
#include "clients/bclientstatus.h"
#include "config/bconfigparser.h"
#include "director/bdirector.h"
#include "director/bclientevent.h"

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
        COL_STATUS,         ///< Age of the last backup or contact (recent, stale, never)
        COL_OS,             ///< Operating system
        COL_VERSION,        ///< Client version
        COL_LAST_CONN,      ///< Last connection time
        COL_JOB_COUNT,      ///< Number of jobs
        COL_TOTAL_BYTES,    ///< Total backed up data
        COL_COUNT           ///< Total column count
    };

    /**
     * @brief TLS authentication mode for config export
     */
    enum TlsMode {
        TLS_AUTO,   ///< Auto-detect from Director's show clients data
        TLS_PSK,    ///< TLS-PSK: Enable=yes, Require=no
        TLS_X509    ///< TLS x509: Enable=yes, Require=yes, + cert file paths
    };

    /**
     * @brief Role with the raw value of a cell, for sorting
     *
     * Port, age, last connection, job count and size are numbers under this
     * role; the other columns return their display text. The widget's proxy
     * model sorts by this role.
     */
    static constexpr int SortRole = Qt::UserRole;

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
     * @brief Job statistics (bytes, count, last job time) from the job list
     * @param jobs JSON array of job objects ("list jobs")
     *
     * The job list is one page of the jobs: it gives only the newest start
     * of a client (the newer of it and the last backup). The number of
     * backups and the data come from "list backups client=" alone
     * (setClientBackups()), never from a page, so they do not change with
     * the page or with the order in which the replies arrive.
     * @since 2.9
     */
    void enrichWithJobData(const QJsonArray &jobs);

    /**
     * @brief The backups of one client ("list backups client=<name>")
     *
     * Only the figures of that client are worked out again. The table is
     * filtered and reset once for a burst of replies (one per client after
     * a connect), BackupsResetDelayMs after the first.
     */
    void setClientBackups(const QString &client, const QJsonArray &backups);
    /**
     * @brief A new round of "list backups client=" requests begins
     *
     * The figures of the last round stay on display until the new replies
     * replace them; nothing falls back to other figures in between.
     * @param clients The clients asked for
     */
    void beginBackupRound(const QStringList &clients);
    /**
     * @brief Every reply of the round is in (or the round timed out)
     *
     * A client of the round without a backup in any reply has none: its
     * figures become 0 (an empty reply does not name its client).
     */
    void finishBackupRound();
    static constexpr int BackupsResetDelayMs = 200;

    /**
     * @brief Enriches client data with address/port from "show clients" response
     * @param jsonData Raw JSON response string
     * @since 2.9
     */
    void enrichWithShowClientData(const QString &jsonData);
    /// The same from a reply that was parsed already (its root object)
    void enrichWithShowClientData(const QJsonObject &reply);

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
     * @brief Age and state of the client's last backup or contact
     *
     * Derived from "lastconnection" or, without it, from the start of the
     * client's newest job in the loaded job list. This is not a reachability
     * check; see BClientStatus.
     * @param client Client JSON object
     */
    BClientStatus::Info clientActivity(const QJsonObject &client) const;

    /**
     * @brief Sets the age in hours from which a last backup counts as stale
     *
     * Resets the model so that views show the new classification.
     */
    void setStaleThresholdHours(int hours);
    int staleThresholdHours() const { return m_staleThresholdHours; }

    /// Client events the Director pushes (gRPC, Onesimus Director)
    using LiveEvent = BClientEvent;

    /// What the events say about a client at a time
    struct LiveStatus {
        enum State { None, Online, Offline };
        State state = None;
        QDateTime at;           ///< Time of the event it rests on
    };

    /// Online after an event lasts this long after the connection ended or
    /// the client answered; then the column shows the last backup again
    static constexpr int OnlineValidSecs = 10 * 60;

    /**
     * @brief Takes a client event of the Director (push stream)
     *
     * Feeds the status column with real data: Online while a connection to
     * the file daemon is open and for OnlineValidSecs after it ended or the
     * client answered a check; Offline after a failed check, until the next
     * contact. Only the rows of the client are updated (dataChanged).
     * @param at When the Director saw it; invalid: now
     */
    void setClientLiveEvent(const QString &clientName, LiveEvent event,
                            const QDateTime &at = QDateTime());

    /// The live state of a client at @p now (None: no event that still holds)
    LiveStatus liveStatus(const QString &clientName, const QDateTime &now) const;

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
     * @brief Timestamp text of the last backup or contact, empty if none is known
     *
     * "lastconnection" of the client, else the newest job start time.
     */
    QString lastActivityTimestamp(const QJsonObject &client) const;

    BDirector *m_director;              ///< Director connection

    // Data members
    QJsonArray m_allClients;            ///< Unfiltered master client data
    QJsonArray m_clients;               ///< Filtered client data (displayed in table)

    // Enrichment maps (resolved per-column in data())
    QMap<QString, QString> m_lastJobTimes;          ///< Client name → last job starttime
    QMap<QString, qint64> m_totalBytes;             ///< Client name → total backed up bytes
    QMap<QString, int> m_jobCounts;                 ///< Client name → number of jobs

    /// The figures of one client from one source
    struct JobStats
    {
        QString lastStart;      ///< Newest start time, as the Director wrote it
        qint64 bytes = 0;
        int count = 0;
    };
    static QHash<QString, JobStats> statsOf(const QJsonArray &jobs);
    QHash<QString, JobStats> m_jobListStats;    ///< From the job list (one page)
    QHash<QString, JobStats> m_backupStats;     ///< From "list backups client=", per client
    QSet<QString> m_backupRoundClients;         ///< Asked for in the running round
    QSet<QString> m_backupRoundSeen;            ///< Named by a reply of the running round
    /// m_lastJobTimes, m_totalBytes, m_jobCounts of one client from both sources
    void mergeStats(const QString &client);
    QTimer *m_backupsTimer = nullptr;           ///< One reset for a burst of backup replies
    QMap<QString, QPair<QString, int>> m_addresses; ///< Client name → {address, port}
    QMap<QString, QJsonObject> m_showClientDetails; ///< Client name → full "show clients" data

    // Client events of the Director (setClientLiveEvent())
    struct LiveRecord {
        LiveStatus::State state = LiveStatus::None;
        QDateTime at;
        QDateTime validUntil;   ///< Invalid: until the next event
    };
    QHash<QString, LiveRecord> m_live;
    /// Tells the views that the status cells of a client changed
    void notifyClientStatus(const QString &clientName);

    int m_staleThresholdHours = BClientStatus::DefaultThresholdHours;

    // Deferred enrichment caches
    QJsonArray m_cachedJobs;            ///< Cached job data for deferred enrichment
    QJsonObject m_cachedShowClients;    ///< Cached "show clients" reply for deferred enrichment
};

#endif // BCLIENTSMODEL_H
