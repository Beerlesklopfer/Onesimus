#include "jobs/bjoblogmodel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QBrush>
#include <QDebug>

BJobLogModel::BJobLogModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_logFont("Courier", 9)
{
}

int BJobLogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_logLines.size();
}

QVariant BJobLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_logLines.size())
        return QVariant();

    const QString &logLine = m_logLines.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return logLine;

    case Qt::FontRole:
        return m_logFont;

    case Qt::ForegroundRole:
        // Gray out empty or status messages
        if (logLine.isEmpty() ||
            logLine.startsWith("No log") ||
            logLine.startsWith("Loading")) {
            return QBrush(Qt::gray);
        }
        return QVariant();

    default:
        return QVariant();
    }
}

bool BJobLogModel::parseJsonResponse(const QString &jsonResponse)
{
    if (jsonResponse.isEmpty()) {
        beginResetModel();
        m_logLines.clear();
        m_logLines.append(tr("No log data available."));
        endResetModel();
        return true;
    }

    // Try to parse as JSON first
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &parseError);

    QStringList logLines;

    if (parseError.error == QJsonParseError::NoError) {
        // Successfully parsed as JSON - extract log entries
        qDebug() << "BJobLogModel: Parsing JSON log response";

        if (doc.isObject()) {
            QJsonObject root = doc.object();

            // Try to find log data in various possible structures
            // Structure 1: { "result": { "joblog": [...] } }
            if (root.contains("result") && root["result"].isObject()) {
                QJsonObject result = root["result"].toObject();
                if (result.contains("joblog") && result["joblog"].isArray()) {
                    QJsonArray logArray = result["joblog"].toArray();
                    for (const QJsonValue &val : logArray) {
                        logLines.append(val.toString());
                    }
                    qDebug() << "BJobLogModel: Found" << logLines.size() << "log entries in result.joblog";
                }
            }
            // Structure 2: { "joblog": [...] }
            else if (root.contains("joblog") && root["joblog"].isArray()) {
                QJsonArray logArray = root["joblog"].toArray();
                for (const QJsonValue &val : logArray) {
                    logLines.append(val.toString());
                }
                qDebug() << "BJobLogModel: Found" << logLines.size() << "log entries in joblog";
            }
            // Structure 3: { "log": [...] }
            else if (root.contains("log") && root["log"].isArray()) {
                QJsonArray logArray = root["log"].toArray();
                for (const QJsonValue &val : logArray) {
                    logLines.append(val.toString());
                }
                qDebug() << "BJobLogModel: Found" << logLines.size() << "log entries in log";
            }
        }
        // Structure 4: Direct array of log lines
        else if (doc.isArray()) {
            QJsonArray logArray = doc.array();
            for (const QJsonValue &val : logArray) {
                logLines.append(val.toString());
            }
            qDebug() << "BJobLogModel: Found" << logLines.size() << "log entries in direct array";
        }

        if (logLines.isEmpty()) {
            logLines.append(tr("No log entries found in JSON response."));
            qDebug() << "BJobLogModel: JSON parsed but no log entries found";
        }
    } else {
        // Not JSON, display as plain text - split by lines
        qDebug() << "BJobLogModel: Not valid JSON, treating as plain text";
        logLines = jsonResponse.split('\n');
    }

    // Update model
    beginResetModel();
    m_logLines = logLines;
    endResetModel();

    return true;
}

void BJobLogModel::setLogLines(const QStringList &logLines)
{
    beginResetModel();
    m_logLines = logLines;
    endResetModel();
}

void BJobLogModel::clear()
{
    beginResetModel();
    m_logLines.clear();
    endResetModel();
}
