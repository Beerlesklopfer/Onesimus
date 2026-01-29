#include "clients/bclientsmodel.h"
#include <QBrush>
#include <QColor>
#include <QDateTime>
#include <QIcon>
#include <QLocale>

BClientsModel::BClientsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int BClientsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_clients.size();
}

int BClientsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return COL_COUNT;
}

QVariant BClientsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_clients.size())
        return QVariant();

    QJsonObject client = m_clients[index.row()].toObject();

    // Display role
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case COL_NAME:
            return client["name"].toString();
        case COL_ADDRESS:
            return client["address"].toString();
        case COL_STATUS:
            return getStatusText(getClientStatus(client));
        case COL_OS:
            return client["uname"].toString();  // Operating system from uname
        case COL_VERSION:
            return client["version"].toString();
        case COL_LAST_CONN: {
            QString lastConn = client["lastconnection"].toString();
            if (lastConn.isEmpty() || lastConn == "0")
                return tr("Never");
            QDateTime dt = QDateTime::fromString(lastConn, Qt::ISODate);
            if (!dt.isValid())
                dt = QDateTime::fromString(lastConn, "yyyy-MM-dd HH:mm:ss");
            if (dt.isValid())
                return QLocale().toString(dt, QLocale::ShortFormat);
            return lastConn;
        }
        case COL_JOB_COUNT:
            return client["jobcount"].toInt();
        case COL_TOTAL_BYTES:
            return formatBytes(client["totalbytes"].toString().toLongLong());
        }
    }

    // Icon for status column
    if (role == Qt::DecorationRole && index.column() == COL_STATUS) {
        return getStatusIcon(getClientStatus(client));
    }

    // Background color for status column
    if (role == Qt::BackgroundRole && index.column() == COL_STATUS) {
        return QBrush(getStatusColor(getClientStatus(client)));
    }

    // Text alignment
    if (role == Qt::TextAlignmentRole) {
        // Right-align numeric columns
        if (index.column() == COL_JOB_COUNT || index.column() == COL_TOTAL_BYTES) {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
        // Center status column
        if (index.column() == COL_STATUS) {
            return int(Qt::AlignCenter);
        }
    }

    // Tooltip
    if (role == Qt::ToolTipRole) {
        switch (index.column()) {
        case COL_NAME:
            return tr("Client: %1").arg(client["name"].toString());
        case COL_ADDRESS:
            return tr("Address: %1:%2")
                .arg(client["address"].toString())
                .arg(client["port"].toInt());
        case COL_TOTAL_BYTES:
            return tr("%1 bytes").arg(client["totalbytes"].toString());
        case COL_STATUS: {
            Status status = getClientStatus(client);
            if (status == STATUS_ONLINE)
                return tr("Client is online and available");
            else if (status == STATUS_OFFLINE)
                return tr("Client is offline or unavailable");
            else
                return tr("Client status unknown");
        }
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant BClientsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case COL_NAME:
        return tr("Name");
    case COL_ADDRESS:
        return tr("Address");
    case COL_STATUS:
        return tr("Status");
    case COL_OS:
        return tr("Operating System");
    case COL_VERSION:
        return tr("Version");
    case COL_LAST_CONN:
        return tr("Last Connection");
    case COL_JOB_COUNT:
        return tr("Jobs");
    case COL_TOTAL_BYTES:
        return tr("Total Data");
    default:
        return QVariant();
    }
}

void BClientsModel::setClients(const QJsonArray &clients)
{
    beginResetModel();
    m_clients = clients;
    endResetModel();
    emit dataChanged();
}

QJsonObject BClientsModel::clientAt(int row) const
{
    if (row >= 0 && row < m_clients.size())
        return m_clients[row].toObject();
    return QJsonObject();
}

void BClientsModel::clear()
{
    beginResetModel();
    m_clients = QJsonArray();
    endResetModel();
    emit dataChanged();
}

QString BClientsModel::formatBytes(qint64 bytes) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;

    if (bytes >= TB) {
        return QString("%1 TB").arg(bytes / (double)TB, 0, 'f', 2);
    } else if (bytes >= GB) {
        return QString("%1 GB").arg(bytes / (double)GB, 0, 'f', 2);
    } else if (bytes >= MB) {
        return QString("%1 MB").arg(bytes / (double)MB, 0, 'f', 2);
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(bytes / (double)KB, 0, 'f', 2);
    } else {
        return QString("%1 B").arg(bytes);
    }
}

BClientsModel::Status BClientsModel::getClientStatus(const QJsonObject &client) const
{
    // Check if client has recent connection
    QString lastConn = client["lastconnection"].toString();
    if (lastConn.isEmpty() || lastConn == "0")
        return STATUS_OFFLINE;

    QDateTime lastConnTime = QDateTime::fromString(lastConn, Qt::ISODate);
    if (!lastConnTime.isValid())
        lastConnTime = QDateTime::fromString(lastConn, "yyyy-MM-dd HH:mm:ss");

    if (lastConnTime.isValid()) {
        // Consider offline if MORE than 24 hours have passed since last connection
        qint64 secsSinceLastConn = lastConnTime.secsTo(QDateTime::currentDateTime());
        if (secsSinceLastConn > 86400) {  // > 24 hours = offline
            return STATUS_OFFLINE;
        } else {
            return STATUS_ONLINE;  // <= 24 hours = online
        }
    }

    return STATUS_OFFLINE;
}

QIcon BClientsModel::getStatusIcon(Status status) const
{
    switch (status) {
    case STATUS_ONLINE:
        return QIcon::fromTheme("network-idle", QIcon::fromTheme("dialog-ok"));
    case STATUS_OFFLINE:
        return QIcon::fromTheme("network-offline", QIcon::fromTheme("dialog-error"));
    default:
        return QIcon::fromTheme("dialog-question");
    }
}

QString BClientsModel::getStatusText(Status status) const
{
    switch (status) {
    case STATUS_ONLINE:
        return tr("Online");
    case STATUS_OFFLINE:
        return tr("Offline");
    default:
        return tr("Unknown");
    }
}

QColor BClientsModel::getStatusColor(Status status) const
{
    switch (status) {
    case STATUS_ONLINE:
        return QColor(200, 255, 200);  // Light green
    case STATUS_OFFLINE:
        return QColor(220, 220, 220);  // Light gray
    default:
        return QColor(255, 255, 200);  // Light yellow
    }
}
