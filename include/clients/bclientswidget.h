#ifndef BCLIENTSWIDGET_H
#define BCLIENTSWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include "bdirector.h"

// Forward declarations
class BClientsModel;
class QTableView;
class QComboBox;
class QLabel;
class QPushButton;
class QTimer;

/**
 * @brief Widget displaying backup clients with filtering
 * @version 1.0
 * @since 2026-01-28
 *
 * Features:
 * - Client table with sortable columns
 * - ComboBox filter for client names
 * - Status filter (All/Online/Offline)
 * - Statistics display
 * - Refresh button
 * - Double-click to show client details
 */
class BClientsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BClientsWidget(QWidget *parent = nullptr);

    /**
     * @brief Sets the Director connection
     * @param director Pointer to BDirector
     * @since 1.0
     */
    void setDirector(BDirector *director);

    /**
     * @brief Returns the clients model
     * @return Pointer to BClientsModel
     * @since 1.0
     */
    BClientsModel* model() const { return m_model; }

public slots:
    /**
     * @brief Refreshes client list from Director
     * @since 1.0
     */
    void refresh();

    /**
     * @brief Processes JSON response from Director
     * @param jsonData JSON response string
     * @since 1.0
     */
    void processJsonResponse(const QString &jsonData);

    /**
     * @brief Triggers refresh action (public interface to private slot)
     * @since 2.6
     */
    void triggerRefresh() { onRefreshClicked(); }

    /**
     * @brief Clears all data from the widget (tables, combo boxes, etc.)
     * @since 2.6
     */
    void clearData();

signals:
    /**
     * @brief Emitted when a client is double-clicked
     * @param client Client data as QJsonObject
     * @since 1.0
     */
    void clientDoubleClicked(const QJsonObject &client);

    /**
     * @brief Emitted to send command to Director
     * @param cmd Command to send
     * @param args Command arguments
     * @since 1.0
     */
    void sendCommand(const BDirector::Command cmd, const QString &args);

    /**
     * @brief Emitted when status message changes
     * @param message Status message text
     * @since 1.0
     */
    void statusMessageChanged(const QString &message);

private slots:
    void onRefreshClicked();
    void onClientFilterChanged(const QString &text);
    void onStatusFilterChanged(int index);
    void onClientDoubleClicked(const QModelIndex &index);
    void onClientsDataReceived(const QString &command, const QString &jsonData);
    void updateStatistics();

private:
    void setupUI();
    void setupConnections();
    void applyFilters();

    // UI Components
    QTableView *m_tableView;
    BClientsModel *m_model;
    QComboBox *m_clientFilter;
    QComboBox *m_statusFilter;
    QPushButton *m_refreshButton;

    // Statistics labels
    QLabel *m_totalClientsLabel;
    QLabel *m_onlineClientsLabel;
    QLabel *m_offlineClientsLabel;

    // Data
    BDirector *m_director;
    QJsonArray m_allClients;        ///< Unfiltered client data
    QTimer *m_filterDebounceTimer;  ///< Debounce timer for filter updates
};

#endif // BCLIENTSWIDGET_H
