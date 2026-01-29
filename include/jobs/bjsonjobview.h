#ifndef BJSONJOBVIEW_H
#define BJSONJOBVIEW_H

#include <QTableView>
#include <QMenu>
#include <QTimer>
#include "jobs/bjobmodels.h"
#include "bcheckboxdelegate.h"
#include "bcheckableheaderview.h"
#include "bviewpresets.h"
#include "bcolumnconfiguration.h"

/**
 * @brief Custom table view for displaying Bacula backup jobs with advanced features
 * @version 2.0
 * @since 2026-01-26
 * 
 * Features:
 * - Displays jobs from JSON data
 * - Checkbox selection via delegate
 * - Double-click to show job details dialog
 * - Context menu with job-specific actions
 * - Integrated filtering support
 * - Live update capability
 * - Export functionality
 * - Statistics display
 */
class BJsonJobView : public QTableView
{
    Q_OBJECT

public:
    explicit BJsonJobView(QWidget *parent = nullptr);
    
    /**
     * @brief Sets the jobs data from a JSON array
     * @param jobs JSON array containing job objects
     * @since 1.0
     */
    void setJobsData(const QJsonArray &jobs);
    
    /**
     * @brief Appends new jobs (for live updates)
     * @param jobs JSON array containing new job objects
     * @since 2.0
     */
    void appendJobsData(const QJsonArray &jobs);
    
    /**
     * @brief Returns the underlying jobs model
     * @return Pointer to BJobsModel
     * @since 1.0
     */
    BJobsModel* jobsModel() const { return m_model; }
    
    /**
     * @brief Returns the filter model
     * @return Pointer to BJobsFilterModel
     * @since 2.0
     */
    BJobsFilterModel* filterModel() const { return m_filterModel; }
    
    /**
     * @brief Returns the set of selected job IDs
     * @return QSet of selected job ID strings
     * @since 1.0
     */
    QSet<QString> selectedJobIds() const;

    /**
     * @brief Returns the first selected job as JSON object
     * @return QJsonObject of first selected job, or empty object if none selected
     * @since 2.8
     */
    QJsonObject getSelectedJob() const;

    /**
     * @brief Clears all selections
     * @since 1.0
     */
    void clearSelection();
    
    /**
     * @brief Selects all visible (filtered) jobs
     * @since 2.0
     */
    void selectAllVisible();
    
    /**
     * @brief Enables or disables live update mode
     * @param enabled True to enable live updates
     * @param intervalMs Update check interval in milliseconds
     * @since 2.0
     */
    void setLiveUpdateEnabled(bool enabled, int intervalMs = 5000);
    
    /**
     * @brief Saves current view configuration as preset
     * @param name Preset name
     * @since 2.4
     */
    void saveViewPreset(const QString &name);
    
    /**
     * @brief Loads a view preset
     * @param name Preset name
     * @return True if preset was loaded successfully
     * @since 2.4
     */
    bool loadViewPreset(const QString &name);
    
    /**
     * @brief Gets current view configuration
     * @return Current preset configuration
     * @since 2.4
     */
    BViewPresets::Preset currentPreset() const;
    
    /**
     * @brief Applies a preset configuration
     * @param preset Preset to apply
     * @since 2.4
     */
    void applyPreset(const BViewPresets::Preset &preset);
    
    /**
     * @brief Saves current configuration as auto-save
     * @since 2.4
     */
    void saveCurrentState();
    
    /**
     * @brief Restores last saved state
     * @since 2.4
     */
    void restoreLastState();

    /**
     * @brief Sets the Director connection for job operations
     * @param director Pointer to BDirector
     * @since 2.8
     */
    void setDirector(class BDirector *director) { m_director = director; }

public slots:
    /**
     * @brief Exports selected or all jobs to JSON file
     * @param selectedOnly True to export only selected jobs
     * @since 2.0
     */
    void exportToJson(bool selectedOnly = false);
    
    /**
     * @brief Exports selected or all jobs to CSV file
     * @param selectedOnly True to export only selected jobs
     * @since 2.0
     */
    void exportToCsv(bool selectedOnly = false);
    
    /**
     * @brief Shows statistics dialog
     * @since 2.0
     */
    void showStatistics();

signals:
    /**
     * @brief Emitted when a job is double-clicked
     * @param job The job data as QJsonObject
     * @since 1.0
     */
    void jobDoubleClicked(const QJsonObject &job);

    /**
     * @brief Emitted when the selection changes
     * @since 1.0
     */
    void selectionChanged();

    /**
     * @brief Emitted when the current row changes (via click or keyboard)
     * @param current Current model index
     * @param previous Previous model index
     * @since 2.8
     */
    void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

    /**
     * @brief Emitted when live update receives new data
     * @param count Number of new jobs received
     * @since 2.0
     */
    void liveDataReceived(int count);

    /**
     * @brief Emitted when export is requested
     * @param format Export format ("json" or "csv")
     * @param selectedOnly True if exporting selected jobs only
     * @since 2.0
     */
    void exportRequested(const QString &format, bool selectedOnly);

    /**
     * @brief Emitted when a job action command should be sent to Director
     * @param command The command string to send
     * @param args Optional command arguments
     * @since 2.8
     */
    void jobActionRequested(const QString &command, const QString &args);

    /**
     * @brief Emitted when the table should be refreshed (e.g., after job action)
     * @since 2.8
     */
    void refreshRequested();

protected:
    /**
     * @brief Handles double-click events
     * @param event Mouse event
     * @since 1.0
     */
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    
    /**
     * @brief Handles context menu events
     * @param event Context menu event
     * @since 2.0
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onSelectionChanged();
    void onJobsAppended(int count);
    void onLiveUpdateTimeout();
    void showJobDetails();
    void deleteJob();
    void retryJob();
    void cancelJob();
    void viewJobLog();
    void connectSelectionModel();

private:
    void setupView();
    void createContextMenu();
    QJsonObject getJobAtRow(int row);
    
    BJobsModel *m_model;                    ///< Main data model
    BJobsFilterModel *m_filterModel;        ///< Filter proxy model
    BCheckBoxDelegate *m_checkBoxDelegate;  ///< Checkbox delegate
    BCheckableHeaderView *m_headerView;     ///< Custom header with checkbox
    QMenu *m_contextMenu;                   ///< Context menu
    QTimer *m_liveUpdateTimer;              ///< Timer for live updates
    BColumnConfiguration *m_columnConfig;   ///< Column configuration manager
    QTimer *m_autoSaveTimer;                ///< Auto-save timer for column changes
    class BDirector *m_director;            ///< Director connection for job operations
    
    // Context menu actions
    QAction *m_actionDetails;               ///< Show details action
    QAction *m_actionDelete;                ///< Delete job action
    QAction *m_actionRetry;                 ///< Retry job action
    QAction *m_actionCancel;                ///< Cancel job action
    QAction *m_actionViewLog;               ///< View log action
    QAction *m_actionExportJson;            ///< Export to JSON action
    QAction *m_actionExportCsv;             ///< Export to CSV action
    QAction *m_actionSelectAll;             ///< Select all action
    QAction *m_actionInvertSelection;       ///< Invert selection action
    QAction *m_actionClearSelection;        ///< Clear selection action
};

#endif // BJSONJOBVIEW_H
