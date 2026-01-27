#ifndef BJSONSTREAMREADER_H
#define BJSONSTREAMREADER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QIODevice>
#include <QTimer>

/**
 * @brief Reads JSON data from a QIODevice and parses it into a QJsonDocument
 * @version 2.0
 * @since 2026-01-26
 * 
 * This class receives data via slots and parses it as JSON.
 * Supports both one-shot and live streaming modes.
 */
class BJsonStreamReader : public QObject
{
    Q_OBJECT

public:
    explicit BJsonStreamReader(QObject *parent = nullptr);
    
    /**
     * @brief Returns the parsed JSON document
     * @return QJsonDocument containing parsed data
     * @since 1.0
     */
    QJsonDocument jsonDocument() const { return m_jsonDocument; }
    
    /**
     * @brief Returns the jobs array from the parsed JSON
     * @return QJsonArray containing jobs
     * @since 1.0
     */
    QJsonArray jobsArray() const;
    
    /**
     * @brief Clears the internal buffer and document
     * @since 1.0
     */
    void clear();
    
    /**
     * @brief Connects to a QIODevice for live streaming
     * @param device Pointer to QIODevice (must be open for reading)
     * @param pollingInterval Interval in ms to check for new data
     * @since 2.0
     */
    void connectToDevice(QIODevice *device, int pollingInterval = 1000);
    
    /**
     * @brief Disconnects from the current QIODevice
     * @since 2.0
     */
    void disconnectFromDevice();
    
    /**
     * @brief Returns whether live streaming is active
     * @return True if connected to a device
     * @since 2.0
     */
    bool isLiveStreaming() const { return m_device != nullptr; }

public slots:
    /**
     * @brief Receives and accumulates data chunks
     * @param data The incoming data chunk
     * @since 1.0
     */
    void receiveData(const QByteArray &data);
    
    /**
     * @brief Parses the accumulated data as JSON
     * @return true if parsing was successful
     * @since 1.0
     */
    bool parseJson();

signals:
    /**
     * @brief Emitted when JSON parsing is complete
     * @param success Whether parsing was successful
     * @since 1.0
     */
    void jsonParsed(bool success);
    
    /**
     * @brief Emitted when a parse error occurs
     * @param error Error description
     * @since 1.0
     */
    void parseError(const QString &error);
    
    /**
     * @brief Emitted when new data is received in live mode
     * @param jobs New jobs array
     * @since 2.0
     */
    void liveDataReceived(const QJsonArray &jobs);

private slots:
    void onDeviceReadyRead();
    void onPollingTimeout();

private:
    QByteArray m_buffer;            ///< Accumulated data buffer
    QJsonDocument m_jsonDocument;   ///< Parsed JSON document
    QIODevice *m_device;            ///< Connected device for live streaming
    QTimer *m_pollingTimer;         ///< Timer for polling device
    QJsonArray m_lastJobs;          ///< Last known jobs array (for detecting new jobs)
};

#endif // BJSONSTREAMREADER_H
