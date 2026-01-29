#ifndef STORAGEWIDGET_H
#define STORAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QJsonArray>
#include "bdirector.h"

namespace Ui {
class StorageWidget;
}

class StorageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StorageWidget(BDirector *director, QWidget *parent = nullptr);
    ~StorageWidget();

    /**
     * @brief Triggers refresh action (public interface to private slot)
     * @since 2.6
     */
    void triggerRefresh() { onRefreshClicked(); }

    /**
     * @brief Clears all data from the widget (tables, etc.)
     * @since 2.6
     */
    void clearData();

public slots:
    /**
     * @brief Processes JSON response from list volumes command
     * @param jsonData JSON response containing volume information
     * @since 2.7
     */
    void processJsonResponse(const QString &jsonData);

    /**
     * @brief Updates connection state
     * @param connected True if connected to Director
     * @since 2.7
     */
    void setConnectionState(bool connected);

signals:
    void sendCommand(BDirector::Command, const QString &args);
    void statusMessageChanged(const QString &message);

private slots:
    void onVolumesReceived(const QList<BDirector::VolumeInfo> &volumes);
    void onRefreshClicked();
    void onVolumeSelectionChanged();
    void onVolumeDoubleClicked(int row, int column);
    void onPoolFilterChanged(int index);
    void onStatusFilterChanged(int index);
    void applyFilters();
    void updateStatistics();

private:
    void setupUI();
    void updateVolumeTable(const QList<BDirector::VolumeInfo> &volumes);
    QString formatBytes(qint64 bytes);

    Ui::StorageWidget *ui;
    BDirector *m_director;
    QTableWidget *m_volumeTable;
    QPushButton *m_refreshButton;

    // Filters
    QComboBox *m_poolFilter;
    QComboBox *m_statusFilter;

    // Statistics
    QLabel *m_totalVolumesLabel;
    QLabel *m_totalSizeLabel;
    QLabel *m_usedSizeLabel;
    QLabel *m_availableSizeLabel;

    // Data storage
    QJsonArray m_allVolumes;  // All volumes from server
    QJsonArray m_filteredVolumes;  // Filtered volumes
};

#endif // STORAGEWIDGET_H
