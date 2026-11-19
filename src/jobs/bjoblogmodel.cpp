#include "jobs/bjoblogmodel.h"
#include "bstatuschip.h"
#include <QBrush>
#include <QColor>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRegularExpression>

// ============================================================================
// Pure helpers
// ============================================================================

namespace {

const int RecentCatalogTexts = 50;

/// The record array of a `list joblog` reply, whatever its nesting
bool findLogArray(const QJsonDocument &doc, QJsonArray &array)
{
    if (doc.isArray()) {
        array = doc.array();
        return true;
    }
    const QJsonObject root = doc.object();
    const QJsonObject result = root.value(QLatin1String("result")).toObject();
    for (const QJsonValue &candidate : {result.value(QLatin1String("joblog")),
                                        root.value(QLatin1String("joblog")),
                                        root.value(QLatin1String("log"))}) {
        if (candidate.isArray()) {
            array = candidate.toArray();
            return true;
        }
    }
    return false;
}

qint64 jsonToInt64(const QJsonValue &value)
{
    // Bareos API mode 2 delivers all columns as strings
    return value.isString() ? value.toString().toLongLong() : qint64(value.toDouble());
}

BJobLog::LineKind rateTermination(const QString &value)
{
    static const QRegularExpression bad(
        QStringLiteral(R"(\*\*\*|\b(errors?|failed|fatal|fehler|fehlgeschlagen)\b)"),
        QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression warn(
        QStringLiteral(R"(\b(warnings?|warnung|warnungen|cancell?ed)\b)"),
        QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression ok(
        QStringLiteral(R"(\bOK\b)"), QRegularExpression::CaseInsensitiveOption);

    if (bad.match(value).hasMatch()) {
        return BJobLog::LineKind::Error;
    }
    if (warn.match(value).hasMatch()) {
        return BJobLog::LineKind::Warning;
    }
    if (ok.match(value).hasMatch()) {
        return BJobLog::LineKind::Success;
    }
    return BJobLog::LineKind::Normal;
}

// The same semantic colours as the status chips (theme tokens, BStatus):
// green only for the line that reports a successful end
BStatus::Tone kindTone(BJobLog::LineKind kind, bool *colored)
{
    *colored = true;
    switch (kind) {
    case BJobLog::LineKind::Success: return BStatus::Tone::Success;
    case BJobLog::LineKind::Fatal:   return BStatus::Tone::Error;
    case BJobLog::LineKind::Error:   return BStatus::Tone::Error;
    case BJobLog::LineKind::Warning: return BStatus::Tone::Warning;
    case BJobLog::LineKind::Normal:  break;
    }
    *colored = false;
    return BStatus::Tone::Neutral;
}

QColor kindColor(BJobLog::LineKind kind, BStatus::Part part)
{
    bool colored = false;
    const BStatus::Tone tone = kindTone(kind, &colored);
    return colored ? BStatus::color(tone, part) : QColor();
}

bool isTerminationLine(const QString &line)
{
    return line.trimmed().startsWith(QLatin1String("Termination:"), Qt::CaseInsensitive);
}

} // namespace

BJobLog::ParsedReply BJobLog::parseCatalogReply(const QString &reply)
{
    ParsedReply parsed;

    QString cleaned = reply.trimmed();
    if (!cleaned.isEmpty() && cleaned.at(0) == QChar(0xFEFF)) {
        cleaned = cleaned.mid(1);   // BOM
    }
    if (cleaned.isEmpty()) {
        return parsed;
    }

    // Skip anything in front of the first JSON delimiter and doubled openers
    QString json = cleaned;
    int jsonStart = -1;
    for (int i = 0; i < json.size(); ++i) {
        if (json.at(i) == QLatin1Char('{') || json.at(i) == QLatin1Char('[')) {
            jsonStart = i;
            break;
        }
    }
    if (jsonStart > 0) {
        json = json.mid(jsonStart);
    }
    while (json.startsWith(QLatin1String("{{")) || json.startsWith(QLatin1String("[["))) {
        json = json.mid(1);
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        // Not JSON: plain text, one record per line
        const QStringList lines = cleaned.split(QLatin1Char('\n'));
        for (const QString &line : lines) {
            Entry entry;
            entry.text = line;
            parsed.entries.append(entry);
        }
        return parsed;
    }

    if (doc.isObject() && doc.object().contains(QLatin1String("error"))) {
        const QJsonValue error = doc.object().value(QLatin1String("error"));
        parsed.isError = true;
        parsed.error = error.isObject() ? error.toObject().value(QLatin1String("message")).toString()
                                        : error.toString();
        return parsed;
    }

    QJsonArray array;
    if (!findLogArray(doc, array)) {
        return parsed;   // e.g. {"result":{}}: a job without log records
    }

    parsed.entries.reserve(array.size());
    for (const QJsonValue &value : std::as_const(array)) {
        Entry entry;
        if (value.isObject()) {
            const QJsonObject record = value.toObject();
            entry.logId = jsonToInt64(record.value(QLatin1String("logid")));
            entry.time = record.value(QLatin1String("time")).toString();
            entry.text = record.value(QLatin1String("logtext")).toString();
        } else {
            entry.text = value.toString();
        }
        parsed.entries.append(entry);
    }
    return parsed;
}

QStringList BJobLog::entryLines(const Entry &entry)
{
    QString text = entry.text;
    text.remove(QLatin1Char('\r'));

    QStringList raw = text.split(QLatin1Char('\n'));
    while (!raw.isEmpty() && raw.constLast().trimmed().isEmpty()) {
        raw.removeLast();
    }
    while (!raw.isEmpty() && raw.constFirst().trimmed().isEmpty()) {
        raw.removeFirst();
    }
    if (raw.isEmpty()) {
        return {};
    }

    const QString prefix = entry.time.isEmpty() ? QString() : QStringLiteral("[%1] ").arg(entry.time);
    const QString indent(prefix.isEmpty() ? 4 : prefix.size(), QLatin1Char(' '));

    QStringList lines;
    lines.reserve(raw.size());
    lines.append(prefix + raw.constFirst().trimmed());
    for (int i = 1; i < raw.size(); ++i) {
        QString line = raw.at(i);
        while (!line.isEmpty() && line.at(line.size() - 1).isSpace()) {
            line.chop(1);
        }
        lines.append(line.isEmpty() ? QString() : indent + line);
    }
    return lines;
}

BJobLog::LineKind BJobLog::classifyLine(const QString &line, bool continuation)
{
    if (continuation) {
        // "<label>: <value>" lines of the termination summary
        static const QRegularExpression keyValue(
            QStringLiteral(R"(^\s*([A-Za-z][^:]{0,40}):(\s.*)?$)"));
        const QRegularExpressionMatch kv = keyValue.match(line);
        if (kv.hasMatch()) {
            if (kv.captured(1).trimmed().compare(QLatin1String("Termination"), Qt::CaseInsensitive) == 0) {
                return rateTermination(kv.captured(2));
            }
            return LineKind::Normal;
        }
    }

    static const QRegularExpression fatal(
        QStringLiteral(R"((?<!non-)\bfatal\b|\b(critical|kritisch)\b)"),
        QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression error(
        QStringLiteral(R"(\b(errors?|fehler|failed|fehlgeschlagen)\b)"),
        QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression warning(
        QStringLiteral(R"(\b(warn|warnings?|warnung|warnungen)\b)"),
        QRegularExpression::CaseInsensitiveOption);

    if (fatal.match(line).hasMatch()) {
        return LineKind::Fatal;
    }
    if (error.match(line).hasMatch()) {
        return LineKind::Error;
    }
    if (warning.match(line).hasMatch()) {
        return LineKind::Warning;
    }
    return LineKind::Normal;
}

bool BJobLog::isAtBottom(int value, int maximum, int tolerance)
{
    return value >= maximum - qMax(0, tolerance);
}

bool BJobLog::isRunningJobStatus(const QString &catalogStatus)
{
    const QString status = catalogStatus.trimmed();
    if (status.isEmpty()) {
        return false;   // unknown: load once
    }
    // Ended: T (OK), W (OK with warnings), E/e (errors), f (fatal), A (canceled),
    // D (verify differences), I (incomplete). Everything else is created,
    // running or waiting for something.
    static const QString ended = QStringLiteral("TWEefADI");
    return status.size() != 1 || !ended.contains(status.at(0));
}

bool BJobLog::isTerminalStatusEvent(const QString &status)
{
    const QString s = status.trimmed();
    if (s.size() == 1) {
        return !isRunningJobStatus(s);
    }
    static const QStringList terminal = {
        QStringLiteral("success"), QStringLiteral("successful"), QStringLiteral("ok"),
        QStringLiteral("completed"), QStringLiteral("terminated"), QStringLiteral("warnings"),
        QStringLiteral("error"), QStringLiteral("fatal"), QStringLiteral("failed"),
        QStringLiteral("canceled"), QStringLiteral("cancelled"), QStringLiteral("incomplete")
    };
    return terminal.contains(s.toLower());
}


// ============================================================================
// BJobLogModel Implementation
// ============================================================================

BJobLogModel::BJobLogModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int BJobLogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_rows.size();
}

QVariant BJobLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_rows.size())
        return QVariant();

    const Row &row = m_rows.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return row.text;

    case Qt::BackgroundRole: {
        const QColor background = kindColor(row.kind, BStatus::Part::Background);
        return background.isValid() ? QVariant(QBrush(background)) : QVariant();
    }

    case Qt::ForegroundRole: {
        // Whoever sets a background also sets a readable foreground
        const QColor foreground = kindColor(row.kind, BStatus::Part::Foreground);
        return foreground.isValid() ? QVariant(QBrush(foreground)) : QVariant();
    }

    case LogIdRole:
        return row.logId;

    case PushRole:
        return row.push;

    default:
        return QVariant();
    }
}

void BJobLogModel::setJob(quint64 jobId)
{
    beginResetModel();
    m_jobId = jobId;
    m_rows.clear();
    m_catalogRowCount = 0;
    m_catalogEntryCount = 0;
    m_lastLogId = 0;
    m_recentCatalogTexts.clear();
    m_pushEntries.clear();
    m_pushSeq = 0;
    m_hasTermination = false;
    endResetModel();
}

void BJobLogModel::appendRows(QList<Row> &rows, const QStringList &lines, qint64 logId, bool push)
{
    for (int i = 0; i < lines.size(); ++i) {
        Row row;
        row.text = lines.at(i);
        row.logId = logId;
        row.push = push;
        row.kind = BJobLog::classifyLine(row.text, i > 0);
        rows.append(row);
    }
}

bool BJobLogModel::appendCatalogEntries(quint64 jobId, const QList<BJobLog::Entry> &entries,
                                        int keepPushFrom)
{
    if (jobId == 0 || jobId != m_jobId) {
        return false;
    }

    // Which records are new? By LogId if every record has one (and the
    // records already held had one, too), otherwise by position.
    bool useLogIds = !entries.isEmpty() && !(m_catalogEntryCount > 0 && m_lastLogId == 0);
    for (int i = 0; useLogIds && i < entries.size(); ++i) {
        useLogIds = entries.at(i).logId > 0;
    }

    QList<BJobLog::Entry> fresh;
    if (useLogIds) {
        qint64 last = m_lastLogId;
        for (const BJobLog::Entry &entry : entries) {
            if (entry.logId > last) {
                fresh.append(entry);
                last = entry.logId;
            }
        }
        m_lastLogId = last;
        m_catalogEntryCount += fresh.size();
    } else if (entries.size() > m_catalogEntryCount) {
        fresh = entries.mid(m_catalogEntryCount);
        m_catalogEntryCount = entries.size();
    }

    if (fresh.isEmpty()) {
        // Nothing new in the catalog; pushed rows (if any) stay as they are
        return true;
    }

    QList<Row> tail;
    for (const BJobLog::Entry &entry : std::as_const(fresh)) {
        const QStringList lines = BJobLog::entryLines(entry);
        for (int i = 1; i < lines.size() && !m_hasTermination; ++i) {
            m_hasTermination = isTerminationLine(lines.at(i));
        }
        appendRows(tail, lines, entry.logId, false);

        m_recentCatalogTexts.append(entry.text);
    }
    while (m_recentCatalogTexts.size() > RecentCatalogTexts) {
        m_recentCatalogTexts.removeFirst();
    }
    const int newCatalogRows = tail.size();

    // The catalog is authoritative: pushed rows are replaced. Only messages
    // pushed too recently to be part of this load are kept behind it.
    QList<PushEntry> kept;
    if (keepPushFrom >= 0) {
        for (const PushEntry &push : std::as_const(m_pushEntries)) {
            if (push.seq < keepPushFrom) {
                continue;
            }
            bool inCatalog = false;
            for (const BJobLog::Entry &entry : std::as_const(fresh)) {
                if (entry.text.contains(push.text)) {
                    inCatalog = true;
                    break;
                }
            }
            if (!inCatalog) {
                kept.append(push);
                BJobLog::Entry pushed;
                pushed.text = push.text;
                appendRows(tail, BJobLog::entryLines(pushed), 0, true);
            }
        }
    }

    replaceTail(tail);
    m_catalogRowCount += newCatalogRows;
    m_pushEntries = kept;
    return true;
}

void BJobLogModel::replaceTail(const QList<Row> &tail)
{
    // Overwrite the pushed rows in place and only insert or remove the
    // difference, so that a view keeps its position.
    const int first = m_catalogRowCount;
    const int oldCount = m_rows.size() - first;
    const int newCount = tail.size();
    const int common = qMin(oldCount, newCount);

    if (common > 0) {
        for (int i = 0; i < common; ++i) {
            m_rows[first + i] = tail.at(i);
        }
        emit dataChanged(index(first), index(first + common - 1));
    }
    if (newCount > oldCount) {
        beginInsertRows(QModelIndex(), first + common, first + newCount - 1);
        m_rows.append(tail.mid(common));
        endInsertRows();
    } else if (oldCount > newCount) {
        beginRemoveRows(QModelIndex(), first + newCount, first + oldCount - 1);
        m_rows.remove(first + newCount, oldCount - newCount);
        endRemoveRows();
    }
}

int BJobLogModel::appendPushText(quint64 jobId, const QString &text)
{
    if (jobId == 0 || jobId != m_jobId) {
        return -1;
    }

    BJobLog::Entry pushed;
    pushed.text = text.trimmed();
    const QStringList lines = BJobLog::entryLines(pushed);
    if (lines.isEmpty()) {
        return -1;
    }

    QList<Row> rows;
    appendRows(rows, lines, 0, true);

    beginInsertRows(QModelIndex(), m_rows.size(), m_rows.size() + rows.size() - 1);
    m_rows.append(rows);
    endInsertRows();

    PushEntry entry;
    entry.seq = m_pushSeq++;
    entry.text = pushed.text;
    m_pushEntries.append(entry);
    return entry.seq;
}

bool BJobLogModel::recentCatalogContains(const QString &text, int lastEntries) const
{
    const QString needle = text.trimmed();
    if (needle.isEmpty()) {
        return false;
    }
    const int from = qMax(0, m_recentCatalogTexts.size() - qMax(0, lastEntries));
    for (int i = m_recentCatalogTexts.size() - 1; i >= from; --i) {
        if (m_recentCatalogTexts.at(i).contains(needle)) {
            return true;
        }
    }
    return false;
}

void BJobLogModel::setLogLines(const QStringList &logLines)
{
    beginResetModel();
    m_jobId = 0;
    m_rows.clear();
    m_catalogEntryCount = 0;
    m_lastLogId = 0;
    m_recentCatalogTexts.clear();
    m_pushEntries.clear();
    m_pushSeq = 0;
    m_hasTermination = false;
    for (const QString &line : logLines) {
        Row row;
        row.text = line;
        // A snapshot has no record structure; indented lines are continuations
        row.kind = BJobLog::classifyLine(line, !line.isEmpty() && line.at(0).isSpace());
        m_rows.append(row);
    }
    m_catalogRowCount = m_rows.size();
    endResetModel();
}

void BJobLogModel::clear()
{
    setJob(0);
}

QStringList BJobLogModel::logLines() const
{
    QStringList lines;
    lines.reserve(m_rows.size());
    for (const Row &row : m_rows) {
        lines.append(row.text);
    }
    return lines;
}
