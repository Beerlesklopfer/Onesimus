#ifndef BCLIENTSMODEL_H
#define BCLIENTSMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonObject>

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

    explicit BClientsModel(QObject *parent = nullptr);

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
     * @brief Returns all client objects
     * @return QJsonArray containing all clients
     * @since 1.0
     */
    QJsonArray allClients() const { return m_clients; }

    /**
     * @brief Clears all client data
     * @since 1.0
     */
    void clear();

    /**
     * @brief Determines client status from data
     * @param client Client JSON object
     * @return Status enum value
     * @since 1.0
     */
    Status getClientStatus(const QJsonObject &client) const;

signals:
    /**
     * @brief Emitted when client data changes
     * @since 1.0
     */
    void dataChanged();

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

    // Data members
    QJsonArray m_clients;               ///< All client data
};

#endif // BCLIENTSMODEL_H
