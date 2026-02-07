#ifndef BMESSAGESWIDGET_H
#define BMESSAGESWIDGET_H

#include <QWidget>
#include <QListView>
#include <QAbstractListModel>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QFont>
#include "blogging.h"
#include "director/bdirector.h"

// Debug logging prefixes for Messages Widget
#define MSGWIDGET_DEBUG BLOG_DEBUG()
#define MSGWIDGET_WARNING BLOG_WARNING()
#define MSGWIDGET_CRITICAL BLOG_ERROR()

/**
 * @brief Represents a single Director message entry
 */
struct BDirectorMessage
{
    QString type;           ///< Message type (info, warning, error, daemon)
    QString text;           ///< Message content
    QDateTime timestamp;    ///< When the message was received
    QString source;         ///< Source (Director, Storage, Client, etc.)

    bool isValid() const { return !text.isEmpty(); }
};

/**
 * @brief Model for displaying Director messages
 * @version 1.0
 * @since 2026-02-03
 *
 * Model for displaying Director daemon messages, alerts, and notifications.
 * These are messages that are NOT job-specific (job logs go to BJobLogModel).
 */
class BMessagesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum MessageType {
        TypeInfo,
        TypeWarning,
        TypeError,
        TypeDaemon
    };

    explicit BMessagesModel(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Parse messages from JSON response (messages command)
     * @param jsonResponse JSON response from Director
     * @return true if messages were parsed successfully
     */
    bool parseJsonResponse(const QString &jsonResponse);

    /**
     * @brief Add a single message
     * @param message The message to add
     */
    void addMessage(const BDirectorMessage &message);

    /**
     * @brief Add a simple text message
     * @param text Message text
     * @param type Message type (info, warning, error)
     */
    void addMessage(const QString &text, MessageType type = TypeInfo);

    /**
     * @brief Clear all messages
     */
    void clear();

    /**
     * @brief Set maximum message history
     * @param maxMessages Maximum number of messages to keep (0 = unlimited)
     */
    void setMaxHistory(int maxMessages) { m_maxHistory = maxMessages; }

    /**
     * @brief Get all messages
     */
    QList<BDirectorMessage> messages() const { return m_messages; }

    /**
     * @brief Get message count
     */
    int messageCount() const { return m_messages.size(); }

private:
    void trimHistory();

    QList<BDirectorMessage> m_messages;
    QFont m_messageFont;
    int m_maxHistory = 1000;  // Default: keep 1000 messages
};

/**
 * @brief Widget for displaying Director messages
 * @version 1.0
 * @since 2026-02-03
 *
 * Displays Director daemon messages, alerts, and notifications.
 * Supports:
 * - Message polling with configurable timer
 * - Message history with configurable retention
 * - Message type filtering (info, warning, error)
 * - Clear messages functionality
 */
class BMessagesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BMessagesWidget(QWidget *parent = nullptr);
    ~BMessagesWidget();

    /**
     * @brief Set the Director for message operations
     * @param director Pointer to BDirector
     */
    void setDirector(BDirector *director);

    /**
     * @brief Get the messages model
     */
    BMessagesModel* model() const { return m_model; }

    /**
     * @brief Start polling messages at specified interval
     * @param intervalMs Interval in milliseconds (0 = stop polling)
     */
    void startPolling(int intervalMs);

    /**
     * @brief Stop polling messages
     */
    void stopPolling();

    /**
     * @brief Check if polling is active
     */
    bool isPolling() const;

    /**
     * @brief Set message history retention
     * @param maxMessages Maximum messages to keep (0 = unlimited)
     */
    void setMaxHistory(int maxMessages);

    /**
     * @brief Clear all messages
     */
    void clearMessages();

    /**
     * @brief Update connection state
     * @param connected True if connected to Director
     */
    void setConnectionState(bool connected);

public slots:
    /**
     * @brief Request messages from Director
     */
    void requestMessages();

    /**
     * @brief Process messages JSON response
     * @param jsonData JSON response from messages command
     */
    void processMessagesResponse(const QString &jsonData);

signals:
    /**
     * @brief Emitted to send a command to the Director
     */
    void sendCommand(const BDirector::Command cmd, const QString &args);

    /**
     * @brief Emitted when status message changes
     */
    void statusMessageChanged(const QString &message);

    /**
     * @brief Emitted when new messages are received
     * @param count Number of new messages
     */
    void newMessagesReceived(int count);

private slots:
    void onPollTimerTimeout();
    void onClearClicked();
    void onRefreshClicked();

private:
    void setupUI();
    void updateMessageCount();

    QListView *m_listView;
    BMessagesModel *m_model;
    QTimer *m_pollTimer;
    BDirector *m_director;

    // UI elements
    QPushButton *m_refreshButton;
    QPushButton *m_clearButton;
    QLabel *m_countLabel;
    QLabel *m_statusLabel;

    bool m_connected;
    int m_pollInterval;
};

#endif // BMESSAGESWIDGET_H
