#include "jobs/bfiltercombomodel.h"
#include <QSet>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

BFilterComboModel::BFilterComboModel(QObject *parent)
    : QObject(parent)
{
}

void BFilterComboModel::updateFromJobsArray(const QJsonArray &jobsArray)
{
    // Use QSet to collect unique values
    QSet<QString> jobNamesSet;
    QSet<QString> clientNamesSet;

    for (const QJsonValue &jobVal : jobsArray) {
        if (!jobVal.isObject()) {
            continue;
        }

        QJsonObject job = jobVal.toObject();

        // Extract job name
        QString jobName = job["name"].toString().trimmed();
        if (!jobName.isEmpty()) {
            jobNamesSet.insert(jobName);
        }

        // Extract client name
        QString clientName = job["client"].toString().trimmed();
        if (!clientName.isEmpty()) {
            clientNamesSet.insert(clientName);
        }
    }

    // Convert sets to sorted lists
    m_jobNames = jobNamesSet.values();
    m_jobNames.sort(Qt::CaseInsensitive);

    m_clientNames = clientNamesSet.values();
    m_clientNames.sort(Qt::CaseInsensitive);

#ifdef IS_DEVELOPER
    qDebug() << "BFilterComboModel: Updated with" << m_jobNames.size() << "unique job names and"
             << m_clientNames.size() << "unique client names";
#endif

    emit dataUpdated();
}

void BFilterComboModel::updateJobNamesFromDotCommand(const QString &dotJobsResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(dotJobsResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "BFilterComboModel: Failed to parse .jobs response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "BFilterComboModel: .jobs response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray jobsArray = result["jobs"].toArray();

    QSet<QString> jobNamesSet;

    for (const QJsonValue &jobVal : jobsArray) {
        if (!jobVal.isObject()) {
            continue;
        }

        QJsonObject job = jobVal.toObject();
        QString jobName = job["name"].toString().trimmed();

        if (!jobName.isEmpty()) {
            jobNamesSet.insert(jobName);
        }
    }

    // Convert to sorted list
    m_jobNames = jobNamesSet.values();
    m_jobNames.sort(Qt::CaseInsensitive);

#ifdef IS_DEVELOPER
    qDebug() << "BFilterComboModel: Updated job names from .jobs -" << m_jobNames.size() << "jobs";
#endif

    emit dataUpdated();
}

void BFilterComboModel::updateClientNamesFromDotCommand(const QString &dotClientsResponse)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(dotClientsResponse.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "BFilterComboModel: Failed to parse .clients response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "BFilterComboModel: .clients response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray clientsArray = result["clients"].toArray();

    QSet<QString> clientNamesSet;

    for (const QJsonValue &clientVal : clientsArray) {
        if (!clientVal.isObject()) {
            continue;
        }

        QJsonObject client = clientVal.toObject();
        QString clientName = client["name"].toString().trimmed();

        if (!clientName.isEmpty()) {
            clientNamesSet.insert(clientName);
        }
    }

    // Convert to sorted list
    m_clientNames = clientNamesSet.values();
    m_clientNames.sort(Qt::CaseInsensitive);

#ifdef IS_DEVELOPER
    qDebug() << "BFilterComboModel: Updated client names from .clients -" << m_clientNames.size() << "clients";
#endif

    emit dataUpdated();
}

void BFilterComboModel::clear()
{
    m_jobNames.clear();
    m_clientNames.clear();
    emit dataUpdated();
}
