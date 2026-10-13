#ifndef BMDNSBROWSER_H
#define BMDNSBROWSER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QHash>
#include <QHostAddress>
#include <QDateTime>

class QUdpSocket;
class QTimer;

/**
 * @struct BMdnsService
 * @brief A Director announced via mDNS/DNS-SD
 *
 * The Onesimus Director announces itself as `_onesimus-dir._tcp.local.`:
 * - instance name = Director name
 * - SRV port      = BNET port
 * - TXT           = grpc=<port>, tls=1, fp=<sha256 hex>, version=<x>, product=onesimus-dird
 */
struct BMdnsService
{
    QString instanceName;   ///< Director name (first label of the instance)
    QString fullName;       ///< Full instance name (lower case, key)
    QString hostName;       ///< SRV target (e.g. "backup.local")
    QHostAddress address;   ///< Resolved address (IPv4 preferred)
    int port = 0;           ///< BNET port (SRV)
    QMap<QString, QString> txt;

    int grpcPort() const { return txt.value("grpc").toInt(); }
    bool hasGrpc() const { return grpcPort() > 0; }
    QString fingerprint() const { return txt.value("fp").toLower(); }
    QString version() const { return txt.value("version"); }

    /// Host to connect to: the address if resolved, otherwise the SRV host name
    QString connectHost() const
    {
        return address.isNull() ? hostName : address.toString();
    }

    bool isResolved() const { return port > 0 && (!address.isNull() || !hostName.isEmpty()); }
};

/**
 * @class BMdnsBrowser
 * @brief Minimal cross-platform mDNS/DNS-SD browser (no avahi/Bonjour dependency)
 *
 * Sends PTR queries for a service type to 224.0.0.251:5353 and parses
 * PTR/SRV/TXT/A/AAAA answers (with DNS name compression). Works on Linux,
 * Windows and macOS with QUdpSocket only.
 *
 * Usage:
 * @code
 * auto *browser = new BMdnsBrowser(this);
 * connect(browser, &BMdnsBrowser::serviceFound, this, [](const BMdnsService &s) { ... });
 * browser->start();   // default: _onesimus-dir._tcp.local
 * @endcode
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */
class BMdnsBrowser : public QObject
{
    Q_OBJECT

public:
    static constexpr const char *OnesimusDirectorService = "_onesimus-dir._tcp.local";

    explicit BMdnsBrowser(QObject *parent = nullptr);
    ~BMdnsBrowser() override;

    /// Start browsing (re-queries periodically until stop())
    bool start(const QString &serviceType = QString::fromLatin1(OnesimusDirectorService));
    void stop();
    bool isRunning() const { return m_socket != nullptr; }

    /// Send a new query immediately
    void refresh();

    QList<BMdnsService> services() const;

    // Exposed for tests: build a query / parse a packet without sockets
    static QByteArray buildQuery(const QList<QPair<QString, quint16>> &questions, bool unicastResponse);

signals:
    /// New or updated service (emitted when resolved or when data changes)
    void serviceFound(const BMdnsService &service);
    /// Service said goodbye (TTL 0)
    void serviceRemoved(const QString &fullName);
    /// Socket could not be opened
    void error(const QString &message);

private slots:
    void onReadyRead();
    void onQueryTimer();

private:
    void processPacket(const QByteArray &packet);
    void resolvePending();
    void emitIfChanged(const QString &key);

    QUdpSocket *m_socket = nullptr;
    QTimer *m_queryTimer = nullptr;
    QString m_serviceType;      ///< lower case, without trailing dot
    bool m_legacyUnicast = false;  ///< Not bound to 5353: responses come unicast
    int m_queryCount = 0;

    QHash<QString, BMdnsService> m_services;     ///< key: full instance name
    QHash<QString, QHostAddress> m_addresses;     ///< host name -> address
    QHash<QString, QString> m_emittedState;       ///< key -> last emitted state
};

#endif // BMDNSBROWSER_H
