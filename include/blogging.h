/**
 * @file blogging.h
 * @brief Conditional file logging for Onesimus
 *
 * Provides macros for logging to file with conditional compilation.
 * Enable with ONESIMUS_FILE_LOGGING defined (cmake option or compiler flag).
 *
 * Usage:
 *   BLOG_DEBUG() << "Debug message";
 *   BLOG_INFO() << "Info message";
 *   BLOG_WARNING() << "Warning message";
 *   BLOG_ERROR() << "Error message";
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */
#ifndef BLOGGING_H
#define BLOGGING_H

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QString>

#ifdef ONESIMUS_FILE_LOGGING

/**
 * @brief File logger singleton for Onesimus
 *
 * Thread-safe logging to file with automatic rotation.
 */
class BFileLogger
{
public:
    static BFileLogger& instance()
    {
        static BFileLogger logger;
        return logger;
    }

    /**
     * @brief Initialize logging with file path
     * @param filePath Path to log file
     * @param maxSizeMB Maximum log file size in MB before rotation
     */
    void init(const QString &filePath, int maxSizeMB = 10)
    {
        QMutexLocker locker(&m_mutex);
        m_filePath = filePath;
        m_maxSize = maxSizeMB * 1024 * 1024;
        m_enabled = !filePath.isEmpty();

        if (m_enabled) {
            m_file.setFileName(filePath);
            if (m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                m_stream.setDevice(&m_file);
                log("INFO", "=== Onesimus Log Started ===");
            } else {
                m_enabled = false;
                qWarning() << "Failed to open log file:" << filePath;
            }
        }
    }

    /**
     * @brief Enable or disable logging at runtime
     */
    void setEnabled(bool enabled)
    {
        QMutexLocker locker(&m_mutex);
        m_enabled = enabled && !m_filePath.isEmpty();
    }

    bool isEnabled() const { return m_enabled; }

    /**
     * @brief Log a message
     * @param level Log level (DEBUG, INFO, WARNING, ERROR)
     * @param message Message to log
     */
    void log(const QString &level, const QString &message)
    {
        if (!m_enabled) return;

        QMutexLocker locker(&m_mutex);

        if (!m_file.isOpen()) return;

        // Check for rotation
        if (m_file.size() > m_maxSize) {
            rotate();
        }

        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        m_stream << timestamp << " [" << level << "] " << message << "\n";
        m_stream.flush();
    }

    /**
     * @brief Close the log file
     */
    void close()
    {
        QMutexLocker locker(&m_mutex);
        if (m_file.isOpen()) {
            log("INFO", "=== Onesimus Log Ended ===");
            m_file.close();
        }
    }

private:
    BFileLogger() = default;
    ~BFileLogger() { close(); }

    BFileLogger(const BFileLogger&) = delete;
    BFileLogger& operator=(const BFileLogger&) = delete;

    void rotate()
    {
        m_file.close();

        // Rename current log to .1, remove old .1 if exists
        QString backupPath = m_filePath + ".1";
        QFile::remove(backupPath);
        QFile::rename(m_filePath, backupPath);

        // Reopen main log file
        m_file.setFileName(m_filePath);
        m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        m_stream.setDevice(&m_file);
        log("INFO", "=== Log rotated ===");
    }

    QMutex m_mutex;
    QFile m_file;
    QTextStream m_stream;
    QString m_filePath;
    qint64 m_maxSize = 10 * 1024 * 1024;  // 10 MB default
    bool m_enabled = false;
};

/**
 * @brief Helper class for stream-style logging
 */
class BLogStream
{
public:
    BLogStream(const QString &level) : m_level(level) {}

    ~BLogStream()
    {
        BFileLogger::instance().log(m_level, m_message);
    }

    template<typename T>
    BLogStream& operator<<(const T &value)
    {
        QDebug dbg(&m_message);
        dbg.nospace().noquote() << value;
        return *this;
    }

private:
    QString m_level;
    QString m_message;
};

// Logging macros - active when ONESIMUS_FILE_LOGGING is defined
#define BLOG_DEBUG()   BLogStream("DEBUG")
#define BLOG_INFO()    BLogStream("INFO")
#define BLOG_WARNING() BLogStream("WARNING")
#define BLOG_ERROR()   BLogStream("ERROR")

// Initialize logging (call from main.cpp)
#define BLOG_INIT(filePath, maxSizeMB) BFileLogger::instance().init(filePath, maxSizeMB)
#define BLOG_CLOSE() BFileLogger::instance().close()
#define BLOG_SET_ENABLED(enabled) BFileLogger::instance().setEnabled(enabled)

#else // ONESIMUS_FILE_LOGGING not defined

// Empty macros when file logging is disabled
class BLogStreamNull
{
public:
    template<typename T>
    BLogStreamNull& operator<<(const T &) { return *this; }
};

#define BLOG_DEBUG()   BLogStreamNull()
#define BLOG_INFO()    BLogStreamNull()
#define BLOG_WARNING() BLogStreamNull()
#define BLOG_ERROR()   BLogStreamNull()

#define BLOG_INIT(filePath, maxSizeMB) ((void)0)
#define BLOG_CLOSE() ((void)0)
#define BLOG_SET_ENABLED(enabled) ((void)0)

#endif // ONESIMUS_FILE_LOGGING

#endif // BLOGGING_H
