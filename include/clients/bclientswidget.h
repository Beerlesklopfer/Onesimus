// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BCLIENTSWIDGET_H
#define BCLIENTSWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include "director/bdirector.h"

// Forward declarations
class BClientsModel;
class BCheckableHeaderView;
class QTableView;
class QCheckBox;
class QComboBox;
class QLabel;
class QPushButton;
class QSortFilterProxyModel;
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
    explicit BClientsWidget(BDirector *director, QWidget *parent = nullptr);

    /**
     * @brief Updates widget state based on connection status
     * @param connected True if connected to Director
     * @since 2.9
     */
    void setConnectionState(bool connected);

    /**
     * @brief Returns the clients model
     * @return Pointer to BClientsModel
     * @since 1.0
     */
    BClientsModel* model() const { return m_model; }

    /**
     * @brief Shows the details dialog for the currently selected client
     * @return true if a client was selected and dialog shown
     * @since 2.9
     */
    bool showSelectedClientDetails();

    /**
     * @brief Returns true if a client is currently selected
     * @since 2.9
     */
    bool hasSelection() const;

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

    /**
     * @brief Emitted when client selection changes
     * @param hasSelection True if a client is selected
     * @since 2.9
     */
    void selectionChanged(bool hasSelection);

    /**
     * @brief Emitted when auto-refresh checkbox is toggled
     * @param enabled True if auto-refresh is enabled
     * @since 2.11
     */
    void autoRefreshChanged(bool enabled);

private slots:
    void onRefreshClicked();
    void onClientFilterChanged(const QString &text);
    void onStatusFilterChanged(int index);
    void onClientDoubleClicked(const QModelIndex &index);
    void onContextMenu(const QPoint &pos);
    void updateStatistics();

private:
    void setupUI();
    void setupConnections();
    void applyFilters();

    /**
     * @brief Row of m_model behind an index of the table view
     *
     * The view shows the sorted proxy model, so its row numbers are not the
     * model's. Every row action goes through this.
     * @return -1 for an invalid index
     */
    int sourceRow(const QModelIndex &viewIndex) const;

    // UI Components
    QTableView *m_tableView;
    BClientsModel *m_model;
    QSortFilterProxyModel *m_sortModel;     ///< Sorts m_model by its sort role; the view's model
    BCheckableHeaderView *m_headerView;
    QTimer *m_autoSaveTimer;
    QComboBox *m_clientFilter;
    QComboBox *m_statusFilter;
    QCheckBox *m_autoRefreshCheck;
    QPushButton *m_refreshButton;

    // Statistics labels
    QLabel *m_totalClientsLabel;
    QLabel *m_recentClientsLabel;
    QLabel *m_staleClientsLabel;
    QLabel *m_neverClientsLabel;
    QLabel *m_unknownClientsLabel;

    // Data
    BDirector *m_director;
    QTimer *m_filterDebounceTimer;  ///< Debounce timer for filter updates
};

#endif // BCLIENTSWIDGET_H
