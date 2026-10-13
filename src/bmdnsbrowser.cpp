/**
 * @file bmdnsbrowser.cpp
 * @brief Minimal mDNS/DNS-SD browser (RFC 6762 / RFC 6763)
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "bmdnsbrowser.h"
#include "blogging.h"

#include <QUdpSocket>
#include <QTimer>
#include <QNetworkInterface>
#include <QtEndian>
#include <QSet>

namespace {

const QHostAddress MdnsGroupV4(QStringLiteral("224.0.0.251"));
constexpr quint16 MdnsPort = 5353;

enum : quint16 {
    TypeA = 1,
    TypePTR = 12,
    TypeTXT = 16,
    TypeAAAA = 28,
    TypeSRV = 33,
};

QString normalizeName(QString name)
{
    name = name.toLower();
    while (name.endsWith('.')) {
        name.chop(1);
    }
    return name;
}

// Read a (possibly compressed) DNS name. Returns false on malformed data.
bool readName(const QByteArray &pkt, int &offset, QStringList &labels)
{
    int pos = offset;
    bool jumped = false;
    int jumps = 0;

    while (true) {
        if (pos >= pkt.size()) {
            return false;
        }
        const quint8 len = quint8(pkt[pos]);
        if (len == 0) {
            if (!jumped) {
                offset = pos + 1;
            }
            return true;
        }
        if ((len & 0xC0) == 0xC0) {
            if (pos + 1 >= pkt.size() || ++jumps > 32) {
                return false;
            }
            const int target = ((len & 0x3F) << 8) | quint8(pkt[pos + 1]);
            if (!jumped) {
                offset = pos + 2;
            }
            jumped = true;
            pos = target;
            continue;
        }
        if ((len & 0xC0) != 0 || pos + 1 + len > pkt.size()) {
            return false;
        }
        labels.append(QString::fromUtf8(pkt.constData() + pos + 1, len));
        pos += 1 + len;
    }
}

bool readU16(const QByteArray &pkt, int offset, quint16 &value)
{
    if (offset + 2 > pkt.size()) {
        return false;
    }
    value = qFromBigEndian<quint16>(pkt.constData() + offset);
    return true;
}

bool readU32(const QByteArray &pkt, int offset, quint32 &value)
{
    if (offset + 4 > pkt.size()) {
        return false;
    }
    value = qFromBigEndian<quint32>(pkt.constData() + offset);
    return true;
}

void writeName(QByteArray &out, const QString &name)
{
    const QStringList labels = name.split('.', Qt::SkipEmptyParts);
    for (const QString &label : labels) {
        const QByteArray l = label.toUtf8().left(63);
        out.append(char(l.size()));
        out.append(l);
    }
    out.append('\0');
}

// Address preference: local subnet > other, IPv4 > IPv6, loopback last.
// Multi-homed hosts announce several A/AAAA records; keep the best one stable.
int addressScore(const QHostAddress &addr)
{
    if (addr.isNull()) {
        return -1;
    }
    if (addr.isLoopback()) {
        return 0;
    }
    const bool ipv4 = addr.protocol() == QAbstractSocket::IPv4Protocol;
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
            if (!entry.ip().isLoopback() && entry.prefixLength() > 0
                && addr.isInSubnet(entry.ip(), entry.prefixLength())) {
                return ipv4 ? 4 : 3;
            }
        }
    }
    return ipv4 ? 2 : 1;
}

void appendU16(QByteArray &out, quint16 value)
{
    char buf[2];
    qToBigEndian<quint16>(value, buf);
    out.append(buf, 2);
}

} // namespace

// ============================================================================
// Constructor / Destructor
// ============================================================================

BMdnsBrowser::BMdnsBrowser(QObject *parent)
    : QObject(parent)
{
}

BMdnsBrowser::~BMdnsBrowser()
{
    stop();
}

// ============================================================================
// Start / Stop
// ============================================================================

bool BMdnsBrowser::start(const QString &serviceType)
{
    stop();
    m_serviceType = normalizeName(serviceType);

    m_socket = new QUdpSocket(this);

    // Preferred: share port 5353 with a system responder (avahi/Bonjour) and
    // receive multicast answers. Fallback: ephemeral port, the responder then
    // answers by unicast ("legacy unicast", RFC 6762 6.7).
    m_legacyUnicast = false;
    if (!m_socket->bind(QHostAddress::AnyIPv4, MdnsPort,
                        QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        BLOG_DEBUG() << "mDNS: port 5353 not available (" << m_socket->errorString()
                     << ") - using legacy unicast queries";
        if (!m_socket->bind(QHostAddress::AnyIPv4, 0)) {
            const QString msg = tr("mDNS: cannot open UDP socket: %1").arg(m_socket->errorString());
            BLOG_WARNING() << msg;
            m_socket->deleteLater();
            m_socket = nullptr;
            emit error(msg);
            return false;
        }
        m_legacyUnicast = true;
    } else {
        // Join the group on every multicast-capable IPv4 interface
        bool joined = false;
        const auto interfaces = QNetworkInterface::allInterfaces();
        for (const QNetworkInterface &iface : interfaces) {
            const auto flags = iface.flags();
            if (!(flags & QNetworkInterface::IsUp) || !(flags & QNetworkInterface::CanMulticast)
                || (flags & QNetworkInterface::IsLoopBack)) {
                continue;
            }
            joined |= m_socket->joinMulticastGroup(MdnsGroupV4, iface);
        }
        if (!joined) {
            joined = m_socket->joinMulticastGroup(MdnsGroupV4);
        }
        if (!joined) {
            BLOG_WARNING() << "mDNS: could not join multicast group:" << m_socket->errorString();
        }
    }
    m_socket->setSocketOption(QAbstractSocket::MulticastTtlOption, 255);

    connect(m_socket, &QUdpSocket::readyRead, this, &BMdnsBrowser::onReadyRead);

    m_queryTimer = new QTimer(this);
    connect(m_queryTimer, &QTimer::timeout, this, &BMdnsBrowser::onQueryTimer);
    m_queryCount = 0;
    m_queryTimer->start(1000);

    refresh();
    return true;
}

void BMdnsBrowser::stop()
{
    if (m_queryTimer) {
        m_queryTimer->stop();
        m_queryTimer->deleteLater();
        m_queryTimer = nullptr;
    }
    if (m_socket) {
        m_socket->close();
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}

QList<BMdnsService> BMdnsBrowser::services() const
{
    QList<BMdnsService> result;
    for (const BMdnsService &s : m_services) {
        if (s.isResolved()) {
            result.append(s);
        }
    }
    return result;
}

// ============================================================================
// Queries
// ============================================================================

QByteArray BMdnsBrowser::buildQuery(const QList<QPair<QString, quint16>> &questions,
                                    bool unicastResponse)
{
    QByteArray out;
    appendU16(out, 0);      // ID (0 for multicast queries)
    appendU16(out, 0);      // Flags: standard query
    appendU16(out, quint16(questions.size()));
    appendU16(out, 0);      // ANCOUNT
    appendU16(out, 0);      // NSCOUNT
    appendU16(out, 0);      // ARCOUNT
    for (const auto &q : questions) {
        writeName(out, q.first);
        appendU16(out, q.second);
        appendU16(out, quint16(0x0001 | (unicastResponse ? 0x8000 : 0)));  // IN (+QU bit)
    }
    return out;
}

void BMdnsBrowser::refresh()
{
    if (!m_socket) {
        return;
    }
    const QByteArray query = buildQuery({{m_serviceType, TypePTR}}, m_legacyUnicast);
    m_socket->writeDatagram(query, MdnsGroupV4, MdnsPort);
    resolvePending();
}

void BMdnsBrowser::onQueryTimer()
{
    // Fast start (1 s intervals), then every 10 s
    ++m_queryCount;
    if (m_queryCount == 3) {
        m_queryTimer->setInterval(10000);
    }
    refresh();
}

void BMdnsBrowser::resolvePending()
{
    // Ask for SRV/TXT of instances we only know by PTR, and A of unresolved hosts
    QList<QPair<QString, quint16>> questions;
    for (const BMdnsService &s : std::as_const(m_services)) {
        if (s.port == 0) {
            questions.append({s.fullName, TypeSRV});
            questions.append({s.fullName, TypeTXT});
        } else if (s.address.isNull() && !s.hostName.isEmpty()) {
            questions.append({s.hostName, TypeA});
        }
    }
    if (!questions.isEmpty() && m_socket) {
        m_socket->writeDatagram(buildQuery(questions, m_legacyUnicast), MdnsGroupV4, MdnsPort);
    }
}

// ============================================================================
// Responses
// ============================================================================

void BMdnsBrowser::onReadyRead()
{
    while (m_socket && m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(m_socket->pendingDatagramSize()));
        const qint64 n = m_socket->readDatagram(datagram.data(), datagram.size());
        if (n > 0) {
            datagram.resize(int(n));
            processPacket(datagram);
        }
    }
}

void BMdnsBrowser::processPacket(const QByteArray &pkt)
{
    quint16 flags = 0, qd = 0, an = 0, ns = 0, ar = 0;
    if (!readU16(pkt, 2, flags) || !readU16(pkt, 4, qd) || !readU16(pkt, 6, an)
        || !readU16(pkt, 8, ns) || !readU16(pkt, 10, ar)) {
        return;
    }
    if (!(flags & 0x8000)) {
        return;  // a query, not a response
    }

    int offset = 12;
    for (int i = 0; i < qd; ++i) {
        QStringList labels;
        if (!readName(pkt, offset, labels)) {
            return;
        }
        offset += 4;  // QTYPE + QCLASS
    }

    QSet<QString> touched;
    const int records = an + ns + ar;
    for (int i = 0; i < records; ++i) {
        QStringList nameLabels;
        if (!readName(pkt, offset, nameLabels)) {
            return;
        }
        quint16 type = 0, rdlen = 0;
        quint32 ttl = 0;
        if (!readU16(pkt, offset, type) || !readU32(pkt, offset + 4, ttl)
            || !readU16(pkt, offset + 8, rdlen)) {
            return;
        }
        const int rdata = offset + 10;
        offset = rdata + rdlen;
        if (offset > pkt.size()) {
            return;
        }

        const QString name = normalizeName(nameLabels.join('.'));

        switch (type) {
        case TypePTR: {
            if (name != m_serviceType) {
                break;
            }
            int p = rdata;
            QStringList target;
            if (!readName(pkt, p, target) || target.isEmpty()) {
                break;
            }
            const QString fullName = normalizeName(target.join('.'));
            if (ttl == 0) {
                if (m_services.remove(fullName) > 0) {
                    m_emittedState.remove(fullName);
                    emit serviceRemoved(fullName);
                }
                break;
            }
            BMdnsService &s = m_services[fullName];
            s.fullName = fullName;
            s.instanceName = target.first();
            touched.insert(fullName);
            break;
        }
        case TypeSRV: {
            if (!name.endsWith(m_serviceType) || ttl == 0) {
                break;  // goodbye is handled via the PTR record
            }
            quint16 port = 0;
            int p = rdata + 4;  // skip priority + weight
            if (!readU16(pkt, p, port)) {
                break;
            }
            p += 2;
            QStringList target;
            if (!readName(pkt, p, target)) {
                break;
            }
            BMdnsService &s = m_services[name];
            s.fullName = name;
            if (s.instanceName.isEmpty()) {
                s.instanceName = nameLabels.value(0);
            }
            s.port = port;
            s.hostName = normalizeName(target.join('.'));
            if (m_addresses.contains(s.hostName)) {
                s.address = m_addresses.value(s.hostName);
            }
            touched.insert(name);
            break;
        }
        case TypeTXT: {
            if (!name.endsWith(m_serviceType) || ttl == 0) {
                break;
            }
            BMdnsService &s = m_services[name];
            s.fullName = name;
            if (s.instanceName.isEmpty()) {
                s.instanceName = nameLabels.value(0);
            }
            s.txt.clear();
            int p = rdata;
            while (p < rdata + rdlen) {
                const int len = quint8(pkt[p]);
                if (p + 1 + len > rdata + rdlen) {
                    break;
                }
                const QString entry = QString::fromUtf8(pkt.constData() + p + 1, len);
                const int eq = entry.indexOf('=');
                if (eq > 0) {
                    s.txt.insert(entry.left(eq).toLower(), entry.mid(eq + 1));
                } else if (!entry.isEmpty()) {
                    s.txt.insert(entry.toLower(), QString());
                }
                p += 1 + len;
            }
            touched.insert(name);
            break;
        }
        case TypeA:
        case TypeAAAA: {
            QHostAddress addr;
            if (type == TypeA && rdlen == 4) {
                quint32 ip = 0;
                readU32(pkt, rdata, ip);
                addr = QHostAddress(ip);
            } else if (type == TypeAAAA && rdlen == 16) {
                addr = QHostAddress(reinterpret_cast<const quint8 *>(pkt.constData() + rdata));
                if (addr.isLinkLocal()) {
                    break;  // would need a scope id
                }
            } else {
                break;
            }
            if (ttl == 0 || addressScore(addr) <= addressScore(m_addresses.value(name))) {
                break;  // goodbye, or keep the better address we already have
            }
            m_addresses.insert(name, addr);
            for (auto it = m_services.begin(); it != m_services.end(); ++it) {
                if (it->hostName == name) {
                    it->address = addr;
                    touched.insert(it.key());
                }
            }
            break;
        }
        default:
            break;
        }
    }

    for (const QString &key : std::as_const(touched)) {
        emitIfChanged(key);
    }
    resolvePending();
}

void BMdnsBrowser::emitIfChanged(const QString &key)
{
    const BMdnsService s = m_services.value(key);
    if (!s.isResolved()) {
        return;
    }
    QStringList txt;
    for (auto it = s.txt.cbegin(); it != s.txt.cend(); ++it) {
        txt << it.key() + '=' + it.value();
    }
    const QString state = QStringList{s.hostName, s.address.toString(),
                                      QString::number(s.port), txt.join(',')}.join('|');
    if (m_emittedState.value(key) == state) {
        return;
    }
    m_emittedState.insert(key, state);

    BLOG_DEBUG() << "mDNS: found" << s.instanceName << "at" << s.connectHost() << ":" << s.port
                 << "grpc:" << s.grpcPort();
    emit serviceFound(s);
}
