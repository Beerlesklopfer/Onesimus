#ifndef STORAGEWIDGET_H
#define STORAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QDateTime>
#include <QStringList>
#include "director/bdirector.h"
#include "storage/model/bvolumelist.h"

namespace Ui {
class StorageWidget;
}

/**
 * @brief The Storage/Volumes tab: the volume table ("view B" of the storage plan)
 *
 * Fed by "llist volumes" (BDirector::Command::LlistVolumes), read into a
 * BVolumeList; a short "list volumes" reply is read as well, with the fields
 * it lacks shown as unknown. Each row says when the volume may be reused and
 * why (BRecycleForecast). Read only: the tab sends no command that changes
 * anything.
 */
class StorageWidget : public QWidget
{
    Q_OBJECT

public:
    /// Columns of the volume table
    enum Column {
        VolumeColumn = 0,
        PoolColumn,
        StatusColumn,
        UsedColumn,
        LimitColumn,
        JobsColumn,
        LastWrittenColumn,
        RetentionColumn,
        ReusableColumn,
        ReasonColumn,
        ColumnCount
    };

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

    /**
     * @brief Fixes the present for the forecast (tests and pictures)
     *
     * An invalid value (the default) means the current time at each reply.
     */
    void setReferenceTime(const QDateTime &now) { m_referenceTime = now; }

    /// The volume table (for tests and pictures)
    QTableWidget *volumeTable() const { return m_volumeTable; }

public slots:
    /**
     * @brief Processes the JSON reply of "llist volumes" (or "list volumes")
     * @param jsonData JSON response containing volume information
     * @since 2.7
     */
    void processJsonResponse(const QString &jsonData);

    /**
     * @brief Sets the configured pool names (from .pools)
     *
     * Used to map the lowercased pool keys of the grouped reply back to the
     * configured pool names, and to offer pools without volumes in the pool
     * filter.
     * @param pools Configured pool names
     * @since 2.8
     */
    void setKnownPools(const QStringList &pools);

    /**
     * @brief Updates connection state
     * @param connected True if connected to Director
     * @since 2.7
     */
    void setConnectionState(bool connected);

signals:
    void sendCommand(BDirector::Command, const QString &args);
    void statusMessageChanged(const QString &message);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onRefreshClicked();
    void onVolumeActivated(int row);
    void applyFilters();

private:
    void setupUI();
    void fillFilters();
    void updateStatistics();
    QDateTime referenceTime() const;
    const BVolume *volumeAtRow(int row) const;

    Ui::StorageWidget *ui;
    BDirector *m_director;
    QTableWidget *m_volumeTable = nullptr;
    QPushButton *m_refreshButton = nullptr;
    QLabel *m_noticeLabel = nullptr;     ///< Says which fields the short list lacks
    bool m_refreshRequested = false;    ///< Refresh was started on this tab (button or menu)

    // Filters
    QComboBox *m_poolFilter;
    QComboBox *m_statusFilter;

    // Statistics
    QLabel *m_totalVolumesLabel;
    QLabel *m_usedSizeLabel;

    // Data
    BVolumeList m_volumes;          ///< All volumes of the last reply
    QList<int> m_filtered;          ///< Indices into m_volumes shown in the table
    QStringList m_knownPools;       ///< Configured pool names (from .pools)
    QDateTime m_referenceTime;      ///< Fixed present, or invalid for "now"
};

#endif // STORAGEWIDGET_H
