#include "jobs/bfiltercombomodel.h"
#include <QSet>
#include <QDebug>

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

void BFilterComboModel::clear()
{
    m_jobNames.clear();
    m_clientNames.clear();
    emit dataUpdated();
}
