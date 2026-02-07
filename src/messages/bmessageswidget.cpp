/**
 * @file bmessageswidget.cpp
 * @brief Implementation of Director messages widget
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */

#include "messages/bmessageswidget.h"
#include "director/bdirector.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QScrollBar>

// ============================================================================
// BMessagesModel Implementation
// ============================================================================

BMessagesModel::BMessagesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_messageFont = QFont("Monospace", 9);
}

int BMessagesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_messages.size();
}

QVariant BMessagesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_messages.size())
        return QVariant();

    const BDirectorMessage &msg = m_messages.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return QString("[%1] %2: %3")
            .arg(msg.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(msg.type.toUpper())
            .arg(msg.text);

    case Qt::FontRole:
        return m_messageFont;

    case Qt::ForegroundRole:
        // Color based on message type
        if (msg.type == "error" || msg.type == "fatal")
            return QColor(Qt::red);
        if (msg.type == "warning")
            return QColor(255, 165, 0);  // Orange
        if (msg.type == "daemon")
            return QColor(Qt::cyan);
        return QVariant();

    case Qt::ToolTipRole:
        return QString("Source: %1\nTime: %2\n\n%3")
            .arg(msg.source)
            .arg(msg.timestamp.toString(Qt::ISODate))
            .arg(msg.text);

    default:
        return QVariant();
    }
}

bool BMessagesModel::parseJsonResponse(const QString &jsonResponse)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonResponse.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        MSGWIDGET_WARNING << "JSON parse error:" << error.errorString();
        return false;
    }

    if (!doc.isObject()) {
        MSGWIDGET_WARNING << "Invalid JSON structure";
        return false;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Check for messages array
    if (!result.contains("messages")) {
        return false;
    }

    QJsonArray messagesArray = result["messages"].toArray();
    int newCount = 0;

    beginResetModel();

    for (const QJsonValue &val : messagesArray) {
        QJsonObject msgObj = val.toObject();

        BDirectorMessage msg;
        msg.type = msgObj["type"].toString("info");
        msg.text = msgObj["logtext"].toString(msgObj["message"].toString());
        msg.source = msgObj["source"].toString("Director");

        // Parse timestamp if available
        QString timeStr = msgObj["time"].toString();
        if (!timeStr.isEmpty()) {
            msg.timestamp = QDateTime::fromString(timeStr, Qt::ISODate);
        }
        if (!msg.timestamp.isValid()) {
            msg.timestamp = QDateTime::currentDateTime();
        }

        if (msg.isValid()) {
            m_messages.append(msg);
            newCount++;
        }
    }

    trimHistory();
    endResetModel();

    MSGWIDGET_DEBUG << "Parsed" << newCount << "messages, total:" << m_messages.size();
    return newCount > 0;
}

void BMessagesModel::addMessage(const BDirectorMessage &message)
{
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
    m_messages.append(message);
    endInsertRows();

    trimHistory();
}

void BMessagesModel::addMessage(const QString &text, MessageType type)
{
    BDirectorMessage msg;
    msg.text = text;
    msg.timestamp = QDateTime::currentDateTime();
    msg.source = "Client";

    switch (type) {
    case TypeInfo:
        msg.type = "info";
        break;
    case TypeWarning:
        msg.type = "warning";
        break;
    case TypeError:
        msg.type = "error";
        break;
    case TypeDaemon:
        msg.type = "daemon";
        break;
    }

    addMessage(msg);
}

void BMessagesModel::clear()
{
    beginResetModel();
    m_messages.clear();
    endResetModel();
}

void BMessagesModel::trimHistory()
{
    if (m_maxHistory > 0 && m_messages.size() > m_maxHistory) {
        beginRemoveRows(QModelIndex(), 0, m_messages.size() - m_maxHistory - 1);
        while (m_messages.size() > m_maxHistory) {
            m_messages.removeFirst();
        }
        endRemoveRows();
    }
}

// ============================================================================
// BMessagesWidget Implementation
// ============================================================================

BMessagesWidget::BMessagesWidget(QWidget *parent)
    : QWidget(parent)
    , m_listView(nullptr)
    , m_model(new BMessagesModel(this))
    , m_pollTimer(new QTimer(this))
    , m_director(nullptr)
    , m_connected(false)
    , m_pollInterval(0)
{
    setupUI();

    connect(m_pollTimer, &QTimer::timeout, this, &BMessagesWidget::onPollTimerTimeout);
}

BMessagesWidget::~BMessagesWidget()
{
    stopPolling();
}

void BMessagesWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);

    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->setContentsMargins(4, 4, 4, 4);

    m_refreshButton = new QPushButton(tr("Refresh"), this);
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);
    connect(m_refreshButton, &QPushButton::clicked, this, &BMessagesWidget::onRefreshClicked);
    toolbarLayout->addWidget(m_refreshButton);

    m_clearButton = new QPushButton(tr("Clear"), this);
    m_clearButton->setIcon(QIcon::fromTheme("edit-clear"));
    connect(m_clearButton, &QPushButton::clicked, this, &BMessagesWidget::onClearClicked);
    toolbarLayout->addWidget(m_clearButton);

    toolbarLayout->addStretch();

    m_countLabel = new QLabel(tr("0 messages"), this);
    toolbarLayout->addWidget(m_countLabel);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("color: gray; font-style: italic;");
    toolbarLayout->addWidget(m_statusLabel);

    mainLayout->addLayout(toolbarLayout);

    // Messages list
    m_listView = new QListView(this);
    m_listView->setModel(m_model);
    m_listView->setAlternatingRowColors(true);
    m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_listView->setUniformItemSizes(false);
    m_listView->setWordWrap(true);
    mainLayout->addWidget(m_listView);

    // Connect model changes to update count
    connect(m_model, &QAbstractListModel::rowsInserted, this, &BMessagesWidget::updateMessageCount);
    connect(m_model, &QAbstractListModel::rowsRemoved, this, &BMessagesWidget::updateMessageCount);
    connect(m_model, &QAbstractListModel::modelReset, this, &BMessagesWidget::updateMessageCount);
}

void BMessagesWidget::setDirector(BDirector *director)
{
    m_director = director;
}

void BMessagesWidget::startPolling(int intervalMs)
{
    m_pollInterval = intervalMs;

    if (intervalMs > 0 && m_connected) {
        m_pollTimer->start(intervalMs);
        m_statusLabel->setText(tr("Polling every %1s").arg(intervalMs / 1000));
        MSGWIDGET_DEBUG << "Started polling at" << intervalMs << "ms interval";
    } else {
        stopPolling();
    }
}

void BMessagesWidget::stopPolling()
{
    m_pollTimer->stop();
    m_statusLabel->setText("");
    MSGWIDGET_DEBUG << "Stopped polling";
}

bool BMessagesWidget::isPolling() const
{
    return m_pollTimer->isActive();
}

void BMessagesWidget::setMaxHistory(int maxMessages)
{
    m_model->setMaxHistory(maxMessages);
}

void BMessagesWidget::clearMessages()
{
    m_model->clear();
    emit statusMessageChanged(tr("Messages cleared"));
}

void BMessagesWidget::setConnectionState(bool connected)
{
    m_connected = connected;
    m_refreshButton->setEnabled(connected);

    if (connected) {
        // Restore polling if it was configured
        if (m_pollInterval > 0) {
            startPolling(m_pollInterval);
        }
    } else {
        stopPolling();
        m_statusLabel->setText(tr("Not connected"));
    }
}

void BMessagesWidget::requestMessages()
{
    if (!m_connected || !m_director) {
        return;
    }

    // Request messages from Director using "messages" command
    emit sendCommand(BDirector::Command::Messages, "");
}

void BMessagesWidget::processMessagesResponse(const QString &jsonData)
{
    int countBefore = m_model->messageCount();

    if (m_model->parseJsonResponse(jsonData)) {
        int newMessages = m_model->messageCount() - countBefore;
        if (newMessages > 0) {
            emit newMessagesReceived(newMessages);

            // Auto-scroll to bottom
            m_listView->scrollToBottom();
        }
    }
}

void BMessagesWidget::onPollTimerTimeout()
{
    requestMessages();
}

void BMessagesWidget::onClearClicked()
{
    clearMessages();
}

void BMessagesWidget::onRefreshClicked()
{
    requestMessages();
    emit statusMessageChanged(tr("Refreshing messages..."));
}

void BMessagesWidget::updateMessageCount()
{
    int count = m_model->messageCount();
    m_countLabel->setText(tr("%1 message(s)").arg(count));
}
