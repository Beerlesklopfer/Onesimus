#include "bjsonstreamreader.h"
#include <QJsonParseError>

BJsonStreamReader::BJsonStreamReader(QObject *parent)
    : QObject(parent)
    , m_device(nullptr)
    , m_pollingTimer(new QTimer(this))
{
    connect(m_pollingTimer, &QTimer::timeout,
            this, &BJsonStreamReader::onPollingTimeout);
}

void BJsonStreamReader::receiveData(const QByteArray &data)
{
    m_buffer.append(data);
}

bool BJsonStreamReader::parseJson()
{
    QJsonParseError jsonError;
    m_jsonDocument = QJsonDocument::fromJson(m_buffer, &jsonError);
    
    if (jsonError.error != QJsonParseError::NoError) {
        QString errorMsg = QString("JSON Parse Error at offset %1: %2")
            .arg(jsonError.offset)
            .arg(jsonError.errorString());
        emit parseError(errorMsg);
        emit jsonParsed(false);
        return false;
    }
    
    emit jsonParsed(true);
    return true;
}

QJsonArray BJsonStreamReader::jobsArray() const
{
    // Case 1: Direct array (Bareos format)
    if (m_jsonDocument.isArray()) {
#ifdef DEBUG_JSON
        qDebug() << "BJsonStreamReader: JSON is direct array";
#endif
        return m_jsonDocument.array();
    }

    // Case 2: Object with nested structure
    if (!m_jsonDocument.isObject()) {
#ifdef DEBUG_JSON
        qDebug() << "BJsonStreamReader: JSON is neither array nor object";
#endif
        return QJsonArray();
    }

    QJsonObject root = m_jsonDocument.object();

#ifdef DEBUG_JSON
    qDebug() << "BJsonStreamReader: JSON root keys:" << root.keys();
#endif

    // Navigate: root -> result -> jobs
    if (root.contains("result") && root["result"].isObject()) {
        QJsonObject result = root["result"].toObject();
        if (result.contains("jobs") && result["jobs"].isArray()) {
#ifdef DEBUG_JSON
            qDebug() << "BJsonStreamReader: Found jobs in result.jobs";
#endif
            return result["jobs"].toArray();
        }
    }

    // Try: root -> jobs directly
    if (root.contains("jobs") && root["jobs"].isArray()) {
#ifdef DEBUG_JSON
        qDebug() << "BJsonStreamReader: Found jobs in root.jobs";
#endif
        return root["jobs"].toArray();
    }

#ifdef DEBUG_JSON
    qDebug() << "BJsonStreamReader: No jobs array found";
#endif

    return QJsonArray();
}

void BJsonStreamReader::clear()
{
    m_buffer.clear();
    m_jsonDocument = QJsonDocument();
    m_lastJobs = QJsonArray();
}

void BJsonStreamReader::connectToDevice(QIODevice *device, int pollingInterval)
{
    if (m_device) {
        disconnectFromDevice();
    }
    
    m_device = device;
    
    if (m_device) {
        connect(m_device, &QIODevice::readyRead,
                this, &BJsonStreamReader::onDeviceReadyRead);
        
        m_pollingTimer->start(pollingInterval);
        
        // Read any available data immediately
        if (m_device->bytesAvailable() > 0) {
            onDeviceReadyRead();
        }
    }
}

void BJsonStreamReader::disconnectFromDevice()
{
    if (m_device) {
        disconnect(m_device, nullptr, this, nullptr);
        m_device = nullptr;
    }
    
    m_pollingTimer->stop();
}

void BJsonStreamReader::onDeviceReadyRead()
{
    if (!m_device) {
        return;
    }
    
    QByteArray data = m_device->readAll();
    if (!data.isEmpty()) {
        receiveData(data);
        
        // Try to parse and check for new jobs
        if (parseJson()) {
            QJsonArray currentJobs = jobsArray();
            
            // Detect new jobs by comparing with last known state
            if (currentJobs.size() > m_lastJobs.size()) {
                QJsonArray newJobs;
                for (int i = m_lastJobs.size(); i < currentJobs.size(); ++i) {
                    newJobs.append(currentJobs[i]);
                }
                
                m_lastJobs = currentJobs;
                emit liveDataReceived(newJobs);
            }
        }
    }
}

void BJsonStreamReader::onPollingTimeout()
{
    // Periodic check for new data
    if (m_device && m_device->bytesAvailable() > 0) {
        onDeviceReadyRead();
    }
}
