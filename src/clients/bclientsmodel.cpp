// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "clients/bclientsmodel.h"
#include "bbyteformat.h"
#include "bstatuschip.h"
#include "bsortvalue.h"
#include <QDateTime>
#include <QTimeZone>
#include <QLocale>
#include <QRegularExpression>
#include <QSet>

BClientsModel::BClientsModel(BDirector *director, QObject *parent)
    : QAbstractTableModel(parent)
    , m_director(director)
{
    m_backupsTimer = new QTimer(this);
    m_backupsTimer->setSingleShot(true);
    m_backupsTimer->setInterval(BackupsResetDelayMs);
    connect(m_backupsTimer, &QTimer::timeout, this, &BClientsModel::allClientsChanged);

    m_probeGuard = new QTimer(this);
    m_probeGuard->setSingleShot(true);
    m_probeGuard->setInterval(ProbeGuardMs);
    connect(m_probeGuard, &QTimer::timeout, this, [this]() {
        if (!m_probeClient.isEmpty())
            onClientProbeFinished(m_probeId, m_probeClient, int(BClientProbeStatus::Timeout),
                                  QString(), QString());
    });

    if (m_director) {
        connect(m_director, &BDirector::clientProbeFinished,
                this, &BClientsModel::onClientProbeFinished);
    }
}

void BClientsModel::refresh()
{
    if (!m_director || !m_director->isConnected()) {
        CLIENTS_WARNING << "No Director connection available";
        emit statusMessageChanged(tr("No connection to Director"));
        return;
    }

    CLIENTS_DEBUG << "Requesting client list";
    emit statusMessageChanged(tr("Loading clients..."));
    emit sendCommand(BDirector::Command::ListClients, "");
    // Client-initiated connections (issue #9): one command answered by the
    // Director itself, no file daemon is contacted
    emit sendCommand(BDirector::Command::StatusDirector, "");
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

    QString name = client["name"].toString();

    // Status column with a live fact that still holds (client-initiated
    // connection, client event, probe): a real Connected/Online/Offline,
    // not a guess from the age of the last backup
    if (index.column() == COL_STATUS) {
        const LiveStatus live = liveStatus(name, QDateTime::currentDateTime());
        if (live.state != LiveStatus::None) {
            const bool online = live.state == LiveStatus::Online;
            switch (role) {
            case Qt::DisplayRole:
            case Qt::AccessibleTextRole:
                return liveLabel(live);
            case Qt::ToolTipRole: {
                QString tip = liveToolTip(live);
                const auto probe = m_probes.value(name);
                if (probe.second.isValid() && probe.first.result == BClientReachability::Probe::Unknown)
                    tip += QLatin1Char('\n') + probeText(probe.first, probe.second);
                return tip;
            }
            case SortRole:
                // Before every age: Connected and Online first, then Offline
                return online ? qint64(-2) : qint64(-1);
            case BStatus::ToneRole:
                return int(online ? BStatus::Tone::Success : BStatus::Tone::Error);
            case BStatus::CodeRole:
                return online ? QStringLiteral("●") : QStringLiteral("✕");
            default:
                break;
            }
        } else if (role == Qt::ToolTipRole) {
            // The estimate, and a probe that could not tell
            QString tip = BClientStatus::toolTip(clientActivity(client), m_staleThresholdHours);
            const auto probe = m_probes.value(name);
            if (probe.second.isValid())
                tip += QLatin1Char('\n') + probeText(probe.first, probe.second);
            if (m_probeClient == name)
                tip += QLatin1Char('\n') + tr("Checking the client now...");
            return tip;
        }
    }

    // Display role
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case COL_NAME:
            return name;
        case COL_ADDRESS: {
            if (m_addresses.contains(name))
                return m_addresses[name].first;
            if (m_showClientDetails.contains(name)) {
                QString addr = m_showClientDetails[name]["Address"].toString();
                if (!addr.isEmpty()) return addr;
            }
            return client["address"].toString();
        }
        case COL_PORT: {
            int port = 0;
            if (m_addresses.contains(name))
                port = m_addresses[name].second;
            if (port == 0 && m_showClientDetails.contains(name)) {
                port = m_showClientDetails[name]["FdPort"].toInt(0);
                if (port == 0) port = m_showClientDetails[name]["Port"].toInt(0);
            }
            if (port == 0)
                port = client["port"].toInt(0);
            return port > 0 ? port : QVariant();
        }
        case COL_STATUS:
            return BClientStatus::label(clientActivity(client));
        case COL_OS: {
            QString uname = client["uname"].toString().trimmed();
            bool isLinux = uname.contains("linux", Qt::CaseInsensitive);

            // Try comma-separated distro: ",debian-11.2," or ",ubuntu-22.04,"
            // Compiled once: data() is called for every cell on every repaint
            static const QRegularExpression distroRx(
                R"(,(debian|ubuntu|centos|redhat|red\s?hat|suse|fedora|arch|gentoo|alpine|rocky|alma)[- ]?([\d.]*),)",
                QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch distroMatch = distroRx.match(uname);
            if (distroMatch.hasMatch()) {
                QString distro = distroMatch.captured(1);
                distro[0] = distro[0].toUpper();
                QString ver = distroMatch.captured(2);
                QString label = ver.isEmpty() ? distro : QString("%1 %2").arg(distro, ver);
                return isLinux ? QString("Linux %1").arg(label) : label;
            }

            // Try "deb13" pattern from kernel version strings (e.g., "+deb13-amd64")
            static const QRegularExpression debRx(R"(\+deb(\d+))", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch debMatch = debRx.match(uname);
            if (debMatch.hasMatch())
                return QString("Linux Debian %1").arg(debMatch.captured(1));

            // Check for non-Linux OS names
            for (const char *os : {"Windows", "FreeBSD", "Darwin", "macOS",
                                       "Solaris", "AIX", "HP-UX"}) {
                if (uname.contains(os, Qt::CaseInsensitive))
                    return os;
            }

            // Generic Linux fallback
            if (isLinux)
                return QStringLiteral("Linux");

            // Fallback: return first token
            return uname.section(' ', 0, 0);
        }
        case COL_VERSION: {
            QString uname = client["uname"].toString().trimmed();
            // Extract Bareos FD version: first "X.Y.Z" pattern in the uname string
            // Typical: "bareos-fd 21.1.5 (date) ..."
            static const QRegularExpression versionRx(R"((\d+\.\d+(?:\.\d+)*))");
            QRegularExpressionMatch match = versionRx.match(uname);
            if (match.hasMatch())
                return match.captured(1);
            // Fallback: return everything after the first space
            int spaceIdx = uname.indexOf(' ');
            return (spaceIdx > 0) ? uname.mid(spaceIdx + 1).trimmed() : QString();
        }
        case COL_LAST_CONN: {
            // The same timestamp the status column judges, as the Director wrote it
            const BClientStatus::Info activity = clientActivity(client);
            if (activity.activity == BClientStatus::Activity::Never)
                return tr("Never");
            if (activity.timestamp.isValid())
                return QLocale().toString(activity.timestamp, QLocale::ShortFormat);
            return activity.raw;
        }
        case COL_JOB_COUNT:
            // Empty until "list backups" answered for the client
            return m_jobCounts.contains(name) ? QVariant(m_jobCounts.value(name)) : QVariant();
        case COL_TOTAL_BYTES:
            return m_totalBytes.contains(name) ? QVariant(BByteFormat::format(m_totalBytes.value(name)))
                                               : QVariant();
        }
    }

    // Raw values for sorting: numbers, ages and times must not sort as text
    if (role == SortRole) {
        switch (index.column()) {
        case COL_PORT:
            return qint64(data(index, Qt::DisplayRole).toInt());
        case COL_STATUS:
            return BClientStatus::sortKey(clientActivity(client));
        case COL_LAST_CONN: {
            const BClientStatus::Info activity = clientActivity(client);
            return activity.timestamp.isValid() ? activity.timestamp.toMSecsSinceEpoch()
                                                : BSortValue::Missing;
        }
        case COL_JOB_COUNT:
            return qint64(m_jobCounts.value(name, 0));
        case COL_TOTAL_BYTES:
            return m_totalBytes.value(name, 0);
        default:
            return data(index, Qt::DisplayRole);
        }
    }

    // Status chip (BStatusDelegate): sign and colour of the state; the
    // cell text stays the age. Green is not used: a recent backup or
    // contact is not a verified "online".
    if (index.column() == COL_STATUS
        && (role == BStatus::ToneRole || role == BStatus::CodeRole
            || role == Qt::AccessibleTextRole)) {
        const BClientStatus::Info activity = clientActivity(client);
        const BStatus::Chip chip = BStatus::clientActivity(activity.activity,
                                                           BClientStatus::label(activity));
        if (role == BStatus::ToneRole)
            return int(chip.tone);
        if (role == BStatus::CodeRole)
            return chip.code;
        return chip.fullText;
    }

    // Text alignment
    if (role == Qt::TextAlignmentRole) {
        // Right-align numeric columns
        if (index.column() == COL_PORT || index.column() == COL_JOB_COUNT || index.column() == COL_TOTAL_BYTES) {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
    }

    // Tooltip
    if (role == Qt::ToolTipRole) {
        switch (index.column()) {
        case COL_NAME:
            return tr("Client: %1").arg(name);
        case COL_ADDRESS: {
            QString addr = m_addresses.contains(name) ? m_addresses[name].first : client["address"].toString();
            int port = m_addresses.contains(name) ? m_addresses[name].second : client["port"].toInt();
            return tr("Address: %1:%2").arg(addr).arg(port);
        }
        case COL_PORT: {
            int port = m_addresses.contains(name) ? m_addresses[name].second : client["port"].toInt();
            return tr("FD Port: %1").arg(port);
        }
        case COL_OS:
        case COL_VERSION:
            return client["uname"].toString();  // Full uname as tooltip
        case COL_TOTAL_BYTES:
            return tr("%1 bytes").arg(m_totalBytes.value(name, 0));
        case COL_STATUS:
        case COL_LAST_CONN:
            return BClientStatus::toolTip(clientActivity(client), m_staleThresholdHours);
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant BClientsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::ToolTipRole
        && (section == COL_JOB_COUNT || section == COL_TOTAL_BYTES))
        return tr("From \"list backups client=\": all backups of the client in the catalog, "
                  "not only the jobs on the current page.");
    if (orientation == Qt::Horizontal && role == Qt::ToolTipRole && section == COL_STATUS)
        return tr("Connected while the client holds a connection to the Director (client-initiated "
                  "connections from \"status director\", or client events of the Onesimus Director); "
                  "Online or Offline after a client event or \"Check now\"; otherwise the age of the "
                  "last backup or contact, an estimate.");
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case COL_NAME:
        return tr("Name");
    case COL_ADDRESS:
        return tr("Address");
    case COL_PORT:
        return tr("Port");
    case COL_STATUS:
        return tr("Status");
    case COL_OS:
        return tr("Operating System");
    case COL_VERSION:
        return tr("Version");
    case COL_LAST_CONN:
        return tr("Last Connection");
    case COL_JOB_COUNT:
        return tr("Backups");
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
    m_allClients = QJsonArray();
    m_clients = QJsonArray();
    m_lastJobTimes.clear();
    m_totalBytes.clear();
    m_jobCounts.clear();
    m_jobListStats.clear();
    m_backupStats.clear();
    m_backupRoundClients.clear();
    m_backupRoundSeen.clear();
    m_backupsTimer->stop();
    m_addresses.clear();
    m_showClientDetails.clear();
    m_live.clear();
    m_listedClients.clear();
    m_lastListAt = QDateTime();
    m_probes.clear();
    if (!m_probeClient.isEmpty()) {
        const QString probing = m_probeClient;
        m_probeClient.clear();
        m_probeId = 0;
        m_probeGuard->stop();
        emit probeStateChanged(probing, false);
    }
    m_cachedJobs = QJsonArray();
    m_cachedShowClients = QJsonObject();
    endResetModel();
    emit dataChanged();
}

void BClientsModel::setAllClients(const QJsonArray &clients)
{
    m_allClients = clients;

    // Apply cached enrichments if available
    if (!m_cachedJobs.isEmpty())
        enrichWithJobData(m_cachedJobs);
    if (!m_cachedShowClients.isEmpty())
        enrichWithShowClientData(m_cachedShowClients);

    emit allClientsChanged();
}

QHash<QString, BClientsModel::JobStats> BClientsModel::statsOf(const QJsonArray &jobs)
{
    QHash<QString, JobStats> stats;
    for (const QJsonValue &jobVal : jobs) {
        const QJsonObject job = jobVal.toObject();
        const QString clientName = job["client"].toString();
        if (clientName.isEmpty())
            continue;
        JobStats &s = stats[clientName];

        // Track latest job starttime per client
        const QString startTime = job["starttime"].toString();
        if (!startTime.isEmpty() && startTime > s.lastStart)
            s.lastStart = startTime;

        // Accumulate job bytes per client
        const QJsonValue bytesVal = job["jobbytes"];
        s.bytes += bytesVal.isString() ? bytesVal.toString().toLongLong()
                                       : static_cast<qint64>(bytesVal.toDouble());
        ++s.count;
    }
    return stats;
}

void BClientsModel::mergeStats(const QString &client)
{
    const auto backups = m_backupStats.constFind(client);
    const auto listed = m_jobListStats.constFind(client);
    const bool hasBackups = backups != m_backupStats.constEnd();
    const bool hasListed = listed != m_jobListStats.constEnd();

    // Number and data: the backups of the client, all of them. A page of
    // the job list is never counted: its figures change with the page.
    if (hasBackups) {
        m_totalBytes[client] = backups->bytes;
        m_jobCounts[client] = backups->count;
    } else {
        m_totalBytes.remove(client);    // not known yet: an empty cell
        m_jobCounts.remove(client);
    }

    // The newest start either source knows of
    QString last = hasBackups ? backups->lastStart : QString();
    if (hasListed && listed->lastStart > last)
        last = listed->lastStart;
    if (last.isEmpty())
        m_lastJobTimes.remove(client);
    else
        m_lastJobTimes[client] = last;
}

void BClientsModel::enrichWithJobData(const QJsonArray &jobs)
{
    m_cachedJobs = jobs;

    if (m_allClients.isEmpty())
        return;

    m_jobListStats = statsOf(jobs);
    m_lastJobTimes.clear();
    m_totalBytes.clear();
    m_jobCounts.clear();
    QSet<QString> clients;
    for (auto it = m_jobListStats.constBegin(); it != m_jobListStats.constEnd(); ++it)
        clients.insert(it.key());
    for (auto it = m_backupStats.constBegin(); it != m_backupStats.constEnd(); ++it)
        clients.insert(it.key());
    for (const QString &client : std::as_const(clients))
        mergeStats(client);

    emit allClientsChanged();
}

void BClientsModel::setClientBackups(const QString &client, const QJsonArray &backups)
{
    if (client.isEmpty())
        return;
    m_backupStats.insert(client, statsOf(backups).value(client));
    m_backupRoundSeen.insert(client);
    mergeStats(client);
    // One filter and reset for the replies of all clients
    if (!m_backupsTimer->isActive())
        m_backupsTimer->start();
}

void BClientsModel::beginBackupRound(const QStringList &clients)
{
    m_backupRoundClients = QSet<QString>(clients.begin(), clients.end());
    m_backupRoundSeen.clear();
}

void BClientsModel::finishBackupRound()
{
    QSet<QString> without = m_backupRoundClients;
    for (auto it = m_backupStats.constBegin(); it != m_backupStats.constEnd(); ++it)
        without.insert(it.key());
    without.subtract(m_backupRoundSeen);
    for (const QString &client : std::as_const(without)) {
        m_backupStats.insert(client, JobStats{});
        mergeStats(client);
    }
    m_backupRoundClients.clear();
    m_backupRoundSeen.clear();
    if (!without.isEmpty() && !m_backupsTimer->isActive())
        m_backupsTimer->start();
}

void BClientsModel::enrichWithShowClientData(const QString &jsonData)
{
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError)
        return;
    enrichWithShowClientData(doc.object());
}

void BClientsModel::enrichWithShowClientData(const QJsonObject &root)
{
    m_cachedShowClients = root;

    if (m_allClients.isEmpty())
        return;

    QJsonObject result = root["result"].toObject();

    // Collect client objects from various response formats
    QList<QJsonObject> clientObjects;

    QJsonValue clientsVal = result.contains("clients") ? result["clients"]
                          : result.contains("client")  ? result["client"]
                          : QJsonValue();

    if (clientsVal.isArray()) {
        // Array format: [{"Client": {...}}, ...] or [{...}, ...]
        for (const QJsonValue &val : clientsVal.toArray()) {
            QJsonObject entry = val.toObject();
            clientObjects.append(entry.contains("Client") ? entry["Client"].toObject() : entry);
        }
    } else if (clientsVal.isObject()) {
        // Object format: {"PDC-fd": {"name": "PDC-fd", "address": "..."}, ...}
        QJsonObject obj = clientsVal.toObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if (it.value().isObject())
                clientObjects.append(it.value().toObject());
        }
    }

    m_addresses.clear();
    m_showClientDetails.clear();

    for (const QJsonObject &clientObj : clientObjects) {
        QString name = clientObj["Name"].toString();
        if (name.isEmpty())
            name = clientObj["name"].toString();

        QString address = clientObj["Address"].toString();
        if (address.isEmpty())
            address = clientObj["address"].toString();

        int port = clientObj["FdPort"].toInt(0);
        if (port == 0) port = clientObj["fdport"].toInt(0);
        if (port == 0) port = clientObj["Port"].toInt(0);
        if (port == 0) port = clientObj["port"].toInt(9102);

        if (!name.isEmpty()) {
            if (!address.isEmpty())
                m_addresses[name] = qMakePair(address, port);
            m_showClientDetails[name] = clientObj;
        }
    }

    CLIENTS_DEBUG << "Enriched " << m_addresses.size() << " clients with show-client data";

    emit allClientsChanged();
}

QJsonObject BClientsModel::enrichedClient(int row) const
{
    QJsonObject client = clientAt(row);
    if (client.isEmpty())
        return client;

    QString name = client["name"].toString();

    // Merge "show clients" data (address, port, catalog, password, retention, etc.)
    // Keys may be capitalized (old format) or lowercase (api 2 format)
    if (m_showClientDetails.contains(name)) {
        const QJsonObject &show = m_showClientDetails[name];

        // Merge string fields (try capitalized then lowercase)
        static const QStringList fields = {
            "address", "catalog", "password", "description", "maxconcurrentjobs"
        };
        for (const QString &key : fields) {
            QString capKey = key;
            capKey[0] = capKey[0].toUpper();
            QJsonValue val = show.contains(capKey) ? show[capKey]
                           : show.contains(key)    ? show[key]
                           : QJsonValue();
            if (!val.isUndefined() && !val.toString().isEmpty())
                client[key] = val;
        }

        // Port
        int port = show["FdPort"].toInt(0);
        if (port == 0) port = show["fdport"].toInt(0);
        if (port == 0) port = show["Port"].toInt(0);
        if (port == 0) port = show["port"].toInt(0);
        if (port > 0)
            client["port"] = port;

        // AutoPrune
        if (show.contains("AutoPrune"))
            client["autoprune"] = show["AutoPrune"];
        else if (show.contains("autoprune"))
            client["autoprune"] = show["autoprune"];

        // Retention fields
        if (show.contains("FileRetention"))
            client["fileretention"] = show["FileRetention"];
        else if (show.contains("fileretention"))
            client["fileretention"] = show["fileretention"];
        if (show.contains("JobRetention"))
            client["jobretention"] = show["JobRetention"];
        else if (show.contains("jobretention"))
            client["jobretention"] = show["jobretention"];

        // TLS fields (Director-side Client resource config)
        static const QStringList tlsFields = {
            "TlsEnable", "TlsRequire",
            "TlsCaCertificateFile", "TlsCertificate", "TlsKey",
            "TlsVerifyPeer", "TlsAllowedCn"
        };
        for (const QString &key : tlsFields) {
            QString lower = key.toLower();
            QJsonValue val = show.contains(key) ? show[key]
                           : show.contains(lower) ? show[lower]
                           : QJsonValue();
            if (!val.isUndefined() && !val.isNull()) {
                QString strVal = val.toString();
                if (!strVal.isEmpty() || val.isBool())
                    client[lower] = val;
            }
        }
    }

    // Merge address from enrichment map (if show-client didn't have it)
    if (m_addresses.contains(name)) {
        if (client["address"].toString().isEmpty())
            client["address"] = m_addresses[name].first;
        // Always use port from m_addresses if not already set
        if (client["port"].toInt(0) == 0)
            client["port"] = m_addresses[name].second;
    }

    // Default port to 9102 if still not set
    if (client["port"].toInt(0) == 0)
        client["port"] = 9102;

    // Merge job statistics
    if (m_jobCounts.contains(name))
        client["jobcount"] = m_jobCounts[name];
    if (m_totalBytes.contains(name))
        client["totalbytes"] = QString::number(m_totalBytes[name]);
    if (m_lastJobTimes.contains(name) && client["lastconnection"].toString().isEmpty())
        client["lastconnection"] = m_lastJobTimes[name];

    return client;
}

QList<BConfigResource> BClientsModel::clientResources(int row, TlsMode tlsMode) const
{
    QJsonObject client = enrichedClient(row);
    if (client.isEmpty())
        return {};

    QString clientName = client["name"].toString();

    QString directorName;
    if (m_director)
        directorName = m_director->currentDirectorName();
    if (directorName.isEmpty())
        directorName = "bareos-dir";

    int fdPort = client["port"].toInt(9102);
    QString dirPassword = client["password"].toString();

    // Determine TLS mode: PSK or x509
    bool useX509;
    if (tlsMode == TLS_AUTO)
        useX509 = !client["tlscacertificatefile"].toString().isEmpty();
    else
        useX509 = (tlsMode == TLS_X509);

    QString tlsRequire = useX509 ? "yes" : "no";

    // Get PSK cipher list from Director connection
    QString pskCipherList;
    if (!useX509 && m_director)
        pskCipherList = m_director->tlsCipherList();

    QList<BConfigResource> resources;

    // 1. FD-side Director resource  (bareos-fd.d/director/<dir>.conf)
    //    Password must match the Director-side Client resource
    {
        BConfigResource res("Director", directorName);
        res.setSourceFile(QString("etc/bareos/bareos-fd.d/director/%1.conf").arg(directorName));
        res.setValue("password", BConfigValue(dirPassword.isEmpty() ? "CHANGE_ME" : dirPassword));
        res.setValue("tls enable", BConfigValue("yes"));
        res.setValue("tls require", BConfigValue(tlsRequire));
        if (!pskCipherList.isEmpty())
            res.setValue("tls cipher list", BConfigValue(pskCipherList));
        if (useX509) {
            res.setValue("tls ca certificate file", BConfigValue("/etc/bareos/tls/bareos-ca.pem"));
            res.setValue("tls certificate", BConfigValue(
                QString("/etc/bareos/tls/%1.pem").arg(clientName)));
            res.setValue("tls key", BConfigValue(
                QString("/etc/bareos/tls/%1-key.pem").arg(clientName)));
        }
        resources.append(res);
    }

    // 2. FD-side FileDaemon resource  (bareos-fd.d/client/myself.conf)
    //    Own monitoring password — not available from Director
    {
        BConfigResource res("FileDaemon", clientName);
        res.setSourceFile("etc/bareos/bareos-fd.d/client/myself.conf");
        res.setValue("password", BConfigValue("CHANGE_ME"));
        res.setValue("maximum concurrent jobs", BConfigValue("20"));
        res.setValue("fd port", BConfigValue(QString::number(fdPort)));
        res.setValue("tls enable", BConfigValue("yes"));
        res.setValue("tls require", BConfigValue(tlsRequire));
        if (!pskCipherList.isEmpty())
            res.setValue("tls cipher list", BConfigValue(pskCipherList));
        if (useX509) {
            res.setValue("tls ca certificate file", BConfigValue("/etc/bareos/tls/bareos-ca.pem"));
            res.setValue("tls certificate", BConfigValue(
                QString("/etc/bareos/tls/%1.pem").arg(clientName)));
            res.setValue("tls key", BConfigValue(
                QString("/etc/bareos/tls/%1-key.pem").arg(clientName)));
        }
        resources.append(res);
    }

    // 3. FD-side Messages resource  (bareos-fd.d/messages/Standard.conf)
    {
        BConfigResource res("Messages", "Standard");
        res.setSourceFile("etc/bareos/bareos-fd.d/messages/Standard.conf");
        res.setValue("director", BConfigValue(
            QString("\"%1\" = all, !skipped, !restored").arg(directorName)));
        resources.append(res);
    }

    // 4. Director-side Client resource  (bareos-dir.d/client/<name>.conf)
    {
        BConfigResource res("Client", clientName);
        res.setSourceFile(QString("etc/bareos/bareos-dir.d/client/%1.conf").arg(clientName));

        static const QList<QPair<QString, QString>> fieldMap = {
            {"address",            "address"},
            {"port",               "fd port"},
            {"password",           "password"},
            {"catalog",            "catalog"},
            {"maxconcurrentjobs",  "maximum concurrent jobs"},
            {"autoprune",          "autoprune"},
            {"fileretention",      "file retention"},
            {"jobretention",       "job retention"},
            {"description",        "description"},
        };

        // Fields that Bareos treats as boolean (yes/no, not 0/1)
        static const QSet<QString> boolFields = { "autoprune" };
        // Fields that Bareos treats as time (seconds → human-readable)
        static const QSet<QString> timeFields = { "fileretention", "jobretention" };

        // Convert seconds to best-fit human-readable Bareos time string
        auto secsToTime = [](qint64 secs) -> QString {
            if (secs <= 0) return "0 seconds";
            if (secs % 604800 == 0) return QString("%1 weeks").arg(secs / 604800);
            if (secs % 86400  == 0) return QString("%1 days").arg(secs / 86400);
            if (secs % 3600   == 0) return QString("%1 hours").arg(secs / 3600);
            return QString("%1 seconds").arg(secs);
        };

        for (const auto &pair : fieldMap) {
            if (!client.contains(pair.first))
                continue;

            QJsonValue val = client[pair.first];
            QString strVal;
            if (val.isBool()) {
                strVal = val.toBool() ? "yes" : "no";
            } else if (val.isDouble()) {
                qint64 num = static_cast<qint64>(val.toDouble());
                if (boolFields.contains(pair.first))
                    strVal = (num != 0) ? "yes" : "no";
                else if (timeFields.contains(pair.first))
                    strVal = secsToTime(num);
                else
                    strVal = QString::number(num);
            } else {
                strVal = val.toString();
                // String "0"/"1" for boolean fields
                if (boolFields.contains(pair.first)) {
                    if (strVal == "0") strVal = "no";
                    else if (strVal == "1") strVal = "yes";
                }
                // Numeric string for time fields (e.g. "5184000")
                if (timeFields.contains(pair.first)) {
                    bool ok;
                    qint64 secs = strVal.toLongLong(&ok);
                    if (ok && secs > 0) strVal = secsToTime(secs);
                }
            }

            if (!strVal.isEmpty())
                res.setValue(pair.second, BConfigValue(strVal));
        }

        // Ensure required directives have fallback defaults
        if (!res.hasKey("address"))
            res.setValue("address", BConfigValue(clientName));
        if (!res.hasKey("password"))
            res.setValue("password", BConfigValue(dirPassword.isEmpty() ? "CHANGE_ME" : dirPassword));
        if (!res.hasKey("fd port"))
            res.setValue("fd port", BConfigValue(QString::number(fdPort)));

        // TLS directives for Director-side Client resource
        res.setValue("tls enable", BConfigValue("yes"));
        res.setValue("tls require", BConfigValue(tlsRequire));
        if (!pskCipherList.isEmpty())
            res.setValue("tls cipher list", BConfigValue(pskCipherList));
        if (useX509) {
            // Use actual Director-side paths from show clients if available
            QString caCert = client["tlscacertificatefile"].toString();
            QString cert   = client["tlscertificate"].toString();
            QString key    = client["tlskey"].toString();
            if (!caCert.isEmpty())
                res.setValue("tls ca certificate file", BConfigValue(caCert));
            if (!cert.isEmpty())
                res.setValue("tls certificate", BConfigValue(cert));
            if (!key.isEmpty())
                res.setValue("tls key", BConfigValue(key));
        }

        resources.append(res);
    }

    return resources;
}

QString BClientsModel::lastActivityTimestamp(const QJsonObject &client) const
{
    const QString lastConn = client["lastconnection"].toString();
    if (!BClientStatus::isMissingTimestamp(lastConn))
        return lastConn;
    return m_lastJobTimes.value(client["name"].toString());
}

BClientStatus::Info BClientsModel::clientActivity(const QJsonObject &client) const
{
    // The Director sends its local wall-clock time without a zone, and the
    // protocol does not tell its zone. It is read in this computer's zone:
    // exact when both share a zone, off by the zone difference otherwise
    // (never negative, see BClientStatus::classify()).
    return BClientStatus::classify(lastActivityTimestamp(client),
                                   QDateTime::currentDateTime(),
                                   m_staleThresholdHours,
                                   QTimeZone::systemTimeZone());
}

void BClientsModel::setStaleThresholdHours(int hours)
{
    if (hours < 1)
        hours = BClientStatus::DefaultThresholdHours;
    if (hours == m_staleThresholdHours)
        return;

    beginResetModel();
    m_staleThresholdHours = hours;
    endResetModel();
    emit allClientsChanged();
}

void BClientsModel::setClientLiveEvent(const QString &clientName, LiveEvent event,
                                      const QDateTime &at)
{
    using namespace BClientReachability;
    if (clientName.isEmpty())
        return;
    const QDateTime when = at.isValid() ? at : QDateTime::currentDateTime();
    Inputs &inputs = m_live[clientName];
    switch (event) {
    case LiveEvent::Connected:
        inputs.openEvent = {State::Online, Source::Event, when, QDateTime()};
        break;
    case LiveEvent::Alive:
        inputs.observation = {State::Online, Source::Event, when, when.addSecs(OnlineValidSecs)};
        break;
    case LiveEvent::Disconnected: {
        const bool wasOpen = inputs.openEvent.state == State::Online;
        inputs.openEvent = Fact();
        // The end of a connection that worked: reached until then. It says
        // nothing new about a client a check found unreachable.
        if (!wasOpen && inputs.observation.state == State::Offline)
            return;
        inputs.observation = {State::Online, Source::Event, when, when.addSecs(OnlineValidSecs)};
        break;
    }
    case LiveEvent::Unreachable:
        inputs.openEvent = Fact();
        inputs.observation = {State::Offline, Source::Event, when, QDateTime()};
        break;
    }
    notifyClientStatus(clientName);
    scheduleExpiry(clientName, inputs.observation.validUntil);
}

bool BClientsModel::setStatusDirectorReply(const QString &jsonData, const QDateTime &at)
{
    const BClientReachability::ConnectionList list =
        BClientReachability::parseStatusDirector(jsonData, QTimeZone::systemTimeZone());
    if (!list.recognized) {
        CLIENTS_DEBUG << "status director: no client-connection list in the reply, nothing changed";
        return false;
    }
    setConnectedClients(list, at);
    return true;
}

void BClientsModel::setConnectedClients(const BClientReachability::ConnectionList &list,
                                        const QDateTime &at)
{
    using namespace BClientReachability;
    if (!list.recognized)
        return;
    const QDateTime when = at.isValid() ? at : QDateTime::currentDateTime();

    QSet<QString> listed;
    for (const ClientConnection &connection : list.clients) {
        listed.insert(connection.name);
        Inputs &inputs = m_live[connection.name];
        const QDateTime since = connection.connectTime.isValid() ? connection.connectTime : when;
        const bool changed = inputs.listed.state != State::Online || inputs.listed.at != since;
        inputs.listed = {State::Online, Source::Connection, since, QDateTime()};
        if (changed)
            notifyClientStatus(connection.name);
    }

    // No longer listed: connected until the reply that listed it last
    const QDateTime lastSeen = m_lastListAt.isValid() ? m_lastListAt : when;
    for (const QString &name : std::as_const(m_listedClients)) {
        if (listed.contains(name))
            continue;
        Inputs &inputs = m_live[name];
        inputs.listed = Fact();
        if (!inputs.observation.at.isValid() || inputs.observation.at <= lastSeen) {
            inputs.observation = {State::Online, Source::Connection, lastSeen,
                                  lastSeen.addSecs(OnlineValidSecs)};
            scheduleExpiry(name, inputs.observation.validUntil);
        }
        notifyClientStatus(name);
    }
    m_listedClients = listed;
    m_lastListAt = when;
}

void BClientsModel::setProbeResult(const QString &clientName,
                                   const BClientReachability::ProbeOutcome &outcome,
                                   const QDateTime &at)
{
    using namespace BClientReachability;
    if (clientName.isEmpty())
        return;
    const QDateTime when = at.isValid() ? at : QDateTime::currentDateTime();
    m_probes.insert(clientName, {outcome, when});
    Inputs &inputs = m_live[clientName];
    switch (outcome.result) {
    case Probe::Reachable:
        inputs.observation = {State::Online, Source::Probe, when, when.addSecs(OnlineValidSecs)};
        scheduleExpiry(clientName, inputs.observation.validUntil);
        break;
    case Probe::Unreachable:
        inputs.observation = {State::Offline, Source::Probe, when, QDateTime()};
        break;
    case Probe::Unknown:
        // Nothing to conclude: the status stays as it is
        break;
    }
    notifyClientStatus(clientName);
}

QPair<BClientReachability::ProbeOutcome, QDateTime>
BClientsModel::probeOutcome(const QString &clientName) const
{
    return m_probes.value(clientName);
}

bool BClientsModel::checkClientNow(const QString &clientName)
{
    if (clientName.isEmpty() || !m_probeClient.isEmpty())
        return false;
    if (!m_director || !m_director->isConnected())
        return false;
    m_probeClient = clientName;
    m_probeId = m_director->probeClient(clientName);
    m_probeGuard->start();
    emit probeStateChanged(clientName, true);
    emit statusMessageChanged(tr("Checking client %1...").arg(clientName));
    notifyClientStatus(clientName);
    return true;
}

void BClientsModel::onClientProbeFinished(quint64 id, const QString &clientName, int status,
                                          const QString &text, const QString &error)
{
    // Only the probe this model started; the id is the Director's
    if (m_probeClient.isEmpty() || id != m_probeId)
        return;
    const QString probed = m_probeClient;
    m_probeClient.clear();
    m_probeId = 0;
    m_probeGuard->stop();
    const BClientReachability::ProbeOutcome outcome = BClientReachability::classifyProbe(
        static_cast<BClientProbeStatus>(status), text, error);
    if (!clientName.isEmpty() && clientName != probed) {
        CLIENTS_WARNING << "Probe reply names" << clientName << "instead of" << probed;
    }
    setProbeResult(probed, outcome);
    emit probeStateChanged(probed, false);
    emit statusMessageChanged(probeText(outcome, QDateTime::currentDateTime()));
    emit probeFinished(probed, outcome);
}

BClientsModel::LiveStatus BClientsModel::liveStatus(const QString &clientName,
                                                    const QDateTime &now) const
{
    using namespace BClientReachability;
    const auto it = m_live.constFind(clientName);
    if (it == m_live.constEnd())
        return {};
    // The catalog only matters against an Offline observation
    const QDateTime catalog = it->observation.state == State::Offline
                                  ? lastCatalogActivity(clientName) : QDateTime();
    const Fact fact = resolve(*it, now, catalog);
    LiveStatus live;
    switch (fact.state) {
    case State::None:
        return {};
    case State::Online:
        live.state = LiveStatus::Online;
        break;
    case State::Offline:
        live.state = LiveStatus::Offline;
        break;
    }
    live.at = fact.at;
    live.source = fact.source;
    live.connected = fact.state == State::Online && !fact.validUntil.isValid()
                     && (fact.source == Source::Connection || fact.source == Source::Event);
    return live;
}

QString BClientsModel::liveLabel(const LiveStatus &live)
{
    if (live.state == LiveStatus::Offline)
        return tr("Offline");
    return live.connected ? tr("Connected") : tr("Online");
}

QString BClientsModel::liveToolTip(const LiveStatus &live)
{
    const QString time = QLocale().toString(live.at, QLocale::ShortFormat);
    const bool online = live.state == LiveStatus::Online;
    switch (live.source) {
    case Source::Connection:
        return live.connected
            ? tr("The client holds a client-initiated connection to the Director, connected at %1 "
                 "(\"status director\").").arg(time)
            : tr("The client held a client-initiated connection to the Director at %1 "
                 "(\"status director\"). It is no longer in the list, which also happens while it "
                 "runs a job.").arg(time);
    case Source::Event:
        if (live.connected)
            return tr("The Director has an open connection to the client since %1 (client event of "
                      "the Director).").arg(time);
        return online ? tr("The Director reached the client at %1 (client event of the Director).").arg(time)
                      : tr("The Director could not reach the client at %1 (client event of the Director).").arg(time);
    case Source::Probe:
        return online ? tr("The client answered \"Check now\" at %1.").arg(time)
                      : tr("The Director could not connect to the client at %1 (\"Check now\").").arg(time);
    case Source::None:
    case Source::Catalog:
        break;
    }
    return QString();
}

QString BClientsModel::probeText(const BClientReachability::ProbeOutcome &outcome, const QDateTime &at)
{
    using BClientReachability::ProbeReason;
    const QString time = QLocale().toString(at, QLocale::ShortFormat);
    switch (outcome.reason) {
    case ProbeReason::Answered:
        return tr("Check at %1: the client answered.").arg(time);
    case ProbeReason::ConnectFailed:
        return tr("Check at %1: the Director could not connect to the client.").arg(time);
    case ProbeReason::Disabled:
        return tr("Check at %1: the client is disabled on the Director, so it was not contacted.").arg(time);
    case ProbeReason::NoEvidence:
        return tr("Check at %1: the reply does not tell whether the client answered.").arg(time);
    case ProbeReason::Timeout:
        return tr("Check at %1: no answer in time; the state is unknown.").arg(time);
    case ProbeReason::Error:
        return outcome.detail.isEmpty()
            ? tr("Check at %1 failed; the state is unknown.").arg(time)
            : tr("Check at %1 failed: %2").arg(time, outcome.detail);
    case ProbeReason::NotAvailable:
        return outcome.detail.isEmpty()
            ? tr("Check at %1: not possible on this connection.").arg(time)
            : tr("Check at %1: %2").arg(time, outcome.detail);
    }
    return QString();
}

QDateTime BClientsModel::lastCatalogActivity(const QString &clientName) const
{
    for (const QJsonValue &value : m_allClients) {
        const QJsonObject client = value.toObject();
        if (client.value("name").toString() == clientName) {
            const BClientStatus::Info info = clientActivity(client);
            return info.timestamp;
        }
    }
    return QDateTime();
}

void BClientsModel::scheduleExpiry(const QString &clientName, const QDateTime &validUntil)
{
    // When Online runs out, the cell falls back to the last backup
    if (!validUntil.isValid())
        return;
    const qint64 ms = QDateTime::currentDateTime().msecsTo(validUntil);
    if (ms > 0) {
        QTimer::singleShot(ms + 500, this, [this, clientName]() {
            notifyClientStatus(clientName);
        });
    }
}

void BClientsModel::notifyClientStatus(const QString &clientName)
{
    for (int row = 0; row < m_clients.size(); ++row) {
        if (m_clients.at(row).toObject().value("name").toString() == clientName) {
            emit QAbstractItemModel::dataChanged(index(row, COL_STATUS), index(row, COL_STATUS));
        }
    }
    emit clientStatusChanged(clientName);
    // No allClientsChanged(): that filters and resets the whole table
}
