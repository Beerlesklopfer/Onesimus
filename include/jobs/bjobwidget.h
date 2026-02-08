#ifndef BJOBWIDGET_H
#define BJOBWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMap>
#include <QJsonObject>
#include <QListView>
#include <QToolBox>
#include <QTabWidget>
#include "blogging.h"
#include "jobs/bjsonjobview.h"
#include "director/bjsonstreamreader.h"
#include "bpaginationwidget.h"
#include "jobs/bjobmodels.h"
#include "bresourcemodels.h"
#include "director/bdirector.h"

// Forward declaration
class BMessagesWidget;

// Debug logging prefixes for Job Widget
#define JOBWIDGET_DEBUG BLOG_DEBUG()
#define JOBWIDGET_WARNING BLOG_WARNING()
#define JOBWIDGET_CRITICAL BLOG_ERROR()

/**
 * @brief Integrated job widget using enhanced BJsonJobView with BDirector backend
 * @version 2.0
 * @since 2026-01-26
 * 
 * Combines the BDirector interface with our enhanced MVC components:
 * - BJsonJobView for display
 * - Filter and pagination support
 * - Real-time updates from BDirector
 * - Job control actions (run, cancel, details)
 */
class BJobWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a JobWidget with BDirector integration
     * @param director Pointer to BDirector
     * @param parent Parent widget
     * @since 2.0
     */
    explicit BJobWidget(BDirector *director, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     * @since 1.0
     */
    ~BJobWidget();

    /**
     * @brief Returns the table view for direct access
     * @return Pointer to BJsonJobView
     * @since 2.0
     */
    BJsonJobView* tableView() const { return m_tableView; }

    /**
     * @brief Triggers run job action (public interface to private slot)
     * @since 2.6
     */
    void triggerRunJob() { onRunJobClicked(); }

    /**
     * @brief Triggers cancel job action (public interface to private slot)
     * @since 2.6
     */
    void triggerCancelJob() { onCancelJobClicked(); }

    /**
     * @brief Triggers show details action (public interface to private slot)
     * @since 2.6
     */
    void triggerShowDetails() { onShowDetailsClicked(); }

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

    /**
     * @brief Shows or hides the job log view
     * @param visible True to show, false to hide
     * @since 2.8
     */
    void setLogViewVisible(bool visible);

    /**
     * @brief Returns the log view widget
     * @return Pointer to log view
     * @since 2.8
     */
    QListView* logView() const { return m_logView; }

    /**
     * @brief Returns the log model
     * @return Pointer to log model
     * @since 2.8
     */
    BJobLogModel* logModel() const { return m_logModel; }

    /**
     * @brief Returns the messages widget
     * @return Pointer to messages widget
     * @since 2.10
     */
    BMessagesWidget* messagesWidget() const { return m_messagesWidget; }

public slots:
    /**
     * @brief Processes JSON response data from BDirector
     * @param jsonData The JSON string received from Director
     * @since 2.0
     */
    void processJsonResponse(const QString &jsonData);

    /**
     * @brief Processes .jobs dot-command response
     * @param jsonData JSON response from .jobs command
     * @since 2.5
     */
    void processDotJobsResponse(const QString &jsonData);

    /**
     * @brief Processes .clients dot-command response
     * @param jsonData JSON response from .clients command
     * @since 2.5
     */
    void processDotClientsResponse(const QString &jsonData);

    /**
     * @brief Processes .levels dot-command response
     * @param jsonData JSON response from .levels command
     * @since 2.7
     */
    void processDotLevelsResponse(const QString &jsonData);

    /**
     * @brief Processes .filesets dot-command response
     * @param jsonData JSON response from .filesets command
     * @since 2.8
     */
    void processDotFilesetsResponse(const QString &jsonData);

    /**
     * @brief Processes .storages dot-command response
     * @param jsonData JSON response from .storages command
     * @since 2.8
     */
    void processDotStoragesResponse(const QString &jsonData);

    /**
     * @brief Processes .pools dot-command response
     * @param jsonData JSON response from .pools command
     * @since 2.8
     */
    void processDotPoolsResponse(const QString &jsonData);

    /**
     * @brief Processes list jobtotals response for server-side pagination
     * @param jsonData JSON response from list jobtotals command
     * @since 2.9
     */
    void processJobTotalsResponse(const QString &jsonData);

    /**
     * @brief Processes messages JSON response (forwards to MessagesWidget)
     * @param jsonData JSON response from messages command
     * @since 2.10
     */
    void processMessagesResponse(const QString &jsonData);

    /**
     * @brief Handles page request from pagination widget
     * @param page Page number (0-indexed)
     * @param pageSize Number of items per page
     * @since 2.9
     */
    void onPageRequested(int page, int pageSize);

    /**
     * @brief Updates UI based on connection state
     * @param connected True if connected to Director, false otherwise
     * @since 2.0
     */
    void setConnectionState(bool connected);

    /**
     * @brief Requests job names and client names from Director using dot-commands
     * @since 2.5
     */
    void requestFilterData();

signals:
    void statusMessageChanged(const QString &message);
    void sendCommand(const BDirector::Command cmd, const QString &args);

private slots:
    /**
     * @brief Handles jobs received from BDirector
     * @param jobs List of JobInfo from BDirector
     * @since 1.0
     */
    void onJobsReceived(const QList<BDirector::JobInfo> &jobs);
    
    /**
     * @brief Handles run job button click
     * @since 1.0
     */
    void onRunJobClicked();
    
    /**
     * @brief Handles cancel job button click
     * @since 1.0
     */
    void onCancelJobClicked();
    
    /**
     * @brief Handles show details button click
     * @since 1.0
     */
    void onShowDetailsClicked();
    
    /**
     * @brief Handles refresh button click
     * @since 1.0
     */
    void onRefreshClicked();
    
    /**
     * @brief Handles selection changes in table (checkbox-based)
     * @since 1.0
     */
    void onJobSelectionChanged();

    /**
     * @brief Handles current row changes (click/keyboard navigation)
     * @param current Current model index
     * @param previous Previous model index
     * @since 2.8
     */
    void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);

    /**
     * @brief Handles job double-click
     * @param job Job data as JSON object
     * @since 2.0
     */
    void onJobDoubleClicked(const QJsonObject &job);
    
    /**
     * @brief Handles auto-refresh timer
     * @since 2.0
     */
    void onAutoRefreshTimeout();
    
    /**
     * @brief Toggles auto-refresh mode
     * @param enabled True to enable auto-refresh
     * @since 2.0
     */
    void toggleAutoRefresh(bool enabled);
    
    /**
     * @brief Applies current filter settings
     * @since 2.4.1
     */
    void applyFilters();
    
    /**
     * @brief Clears all active filters
     * @since 2.4.1
     */
    void clearFilters();

    /**
     * @brief Blendet das Filter-Panel ein oder aus
     * @param visible True = Filter anzeigen, False = Filter ausblenden
     * @since 2.6
     *
     * Zeigt/versteckt den Filter-Container im Splitter und aktualisiert
     * den Toggle-Button entsprechend.
     */
    void do_toggleFilters(bool visible);

    /**
     * @brief Processes job log response for the selected job
     * @param command The command that was sent
     * @param jsonData JSON response containing job log
     * @since 2.8
     */
    void onJobLogReceived(BDirector::Command cmd, const QString &jsonData);

    /**
     * @brief Loads the job log for the currently selected job
     * @since 2.8
     */
    void loadSelectedJobLog();

    /**
     * @brief Handles log history selection change
     * @param index Selected index in history combo
     * @since 2.11
     */
    void onLogHistoryChanged(int index);

    /**
     * @brief Copies selected log lines to clipboard
     * @since 2.11
     */
    void onLogCopyClicked();

    /**
     * @brief Clears the log history
     * @since 2.11
     */
    void onLogClearHistoryClicked();

    /**
     * @brief Filters log view by search text
     * @param text Search text
     * @since 2.11
     */
    void onLogSearchChanged(const QString &text);

private:
    /**
     * @brief Sets up the user interface
     * @since 1.0
     */
    void setupUI();
    
    /**
     * @brief Converts BDirector::JobInfo list to JSON array
     * @param jobs List of JobInfo structures
     * @return QJsonArray for use with our models
     * @since 2.0
     */
    QJsonArray convertJobsToJson(const QList<BDirector::JobInfo> &jobs);
    
    /**
     * @brief Formats bytes into human-readable string
     * @param bytes Byte count
     * @return Formatted string
     * @since 1.0
     */
    QString formatBytes(qint64 bytes);
    
    /**
     * @brief Formats job status code to German description
     * @param status Status code
     * @return German description
     * @since 1.0
     */
    QString formatJobStatus(const QString &status);
    
    BJsonJobView *m_tableView;            ///< Main table view
    BJsonStreamReader *m_streamReader;      ///< JSON stream reader
    BPaginationWidget *m_paginationWidget;  ///< Pagination controls

    // Lower Panel (Job Log + Messages)
    QTabWidget *m_lowerTabWidget;           ///< Tab widget for Job Log and Messages
    BMessagesWidget *m_messagesWidget;      ///< Director messages widget

    // Job Log Display
    QListView *m_logView;                   ///< Log view for selected job
    BJobLogModel *m_logModel;               ///< Log model
    QLabel *m_logTitleLabel;                ///< Title label for log section
    QWidget *m_logContainer;                ///< Container for log section
    QString m_pendingLogJobId;              ///< Job ID of the pending log request

    // Job Log History & Controls
    QComboBox *m_logHistoryCombo;           ///< Dropdown to select from job log history
    QPushButton *m_logCopyButton;           ///< Copy selected log lines
    QPushButton *m_logClearHistoryButton;   ///< Clear log history
    QLineEdit *m_logSearchEdit;             ///< Search within log
    struct JobLogEntry {
        QString jobId;
        QString jobName;
        QStringList logLines;
    };
    QList<JobLogEntry> m_logHistory;        ///< History of loaded job logs
    int m_maxLogHistory = 10;               ///< Maximum number of logs to keep in history

    // Integrated filter controls (from BJobsFilterWidget)
    QComboBox *m_nameFilter;                ///< Job name filter (editable combo box)
    QComboBox *m_clientFilter;              ///< Client name filter (editable combo box)
    BFilterComboModel *m_filterComboModel;  ///< Model for combo box data

    // Configuration data combo boxes (display selected job info)
    QComboBox *m_filesetCombo;              ///< FileSet combo box (read-only, shows current selection)
    QComboBox *m_storageCombo;              ///< Storage combo box (read-only, shows current selection)
    QComboBox *m_poolCombo;                 ///< Pool combo box (read-only, shows current selection)

    QCheckBox *m_statusSuccess;             ///< Filter: Successful (T)
    QCheckBox *m_statusWarning;             ///< Filter: Warning (W)
    QCheckBox *m_statusFailed;              ///< Filter: Failed (f)
    QCheckBox *m_statusError;               ///< Filter: Error (E)
    QCheckBox *m_statusRunning;             ///< Filter: Running (R)
    QCheckBox *m_statusCanceled;            ///< Filter: Canceled (A)
    QCheckBox *m_statusZeroBytes;           ///< Filter: Zero Bytes

    // Dynamic level checkboxes (populated from .levels dot-command)
    QMap<QString, QCheckBox*> m_levelCheckboxes;  ///< Map of level code -> checkbox (e.g., "F" -> Full checkbox)
    QVBoxLayout *m_levelCheckboxLayout;            ///< Layout containing level checkboxes

    QCheckBox *m_dateEnabled;               ///< Enable date range filter
    QDateTimeEdit *m_dateFrom;              ///< Date range start
    QDateTimeEdit *m_dateTo;                ///< Date range end

    QTimer *m_filterTimer;                  ///< Debounce timer for filters

    // Toolbar buttons
    QPushButton *m_refreshButton;           ///< Refresh button

    // Auto-refresh controls
    QCheckBox *m_autoRefreshCheck;          ///< Auto-refresh checkbox
    QComboBox *m_refreshIntervalCombo;      ///< Refresh interval selector
    QTimer *m_autoRefreshTimer;             ///< Auto-refresh timer
    
    QSplitter *m_splitter;                  ///< Splitter for table/filters
    QPushButton *m_toggleFiltersButton;     ///< Toggle filters button
    QPushButton *m_resetFiltersButton;      ///< Reset filters button
    QWidget *m_filterContainer;             ///< Container for all filters
    QToolBox *m_filterToolBox;              ///< ToolBox for accordion-style filter sections
    bool m_filtersVisible;                  ///< Filter visibility state

    BDirector *m_director;                  ///< Director connection for job operations

    // Central data storage (loaded on connect, available for dialogs)
    BFilesetModel *m_filesetModel;          ///< Model for all available filesets
    BStorageModel *m_storageModel;          ///< Model for all available storages
    BPoolModel *m_poolModel;                ///< Model for all available pools
    BLevelModel *m_levelModel;              ///< Model for all available backup levels
    QMap<QString, QJsonObject> m_jobConfigurations;  ///< Job configurations from .jobs (name -> config)

public:
    // Accessor methods for other dialogs to use
    BFilesetModel* filesetModel() const { return m_filesetModel; }
    BStorageModel* storageModel() const { return m_storageModel; }
    BPoolModel* poolModel() const { return m_poolModel; }
    BLevelModel* levelModel() const { return m_levelModel; }

    // Legacy accessors for backward compatibility
    QStringList filesetNames() const { return m_filesetModel ? m_filesetModel->filesetNames() : QStringList(); }
    QStringList storageNames() const { return m_storageModel ? m_storageModel->storageNames() : QStringList(); }
    QStringList poolNames() const { return m_poolModel ? m_poolModel->poolNames() : QStringList(); }

    // Job and Client names from filterComboModel
    QStringList jobNames() const;
    QStringList clientNames() const;

    /**
     * @brief Get job configuration by name
     * @param jobName The name of the job
     * @return QJsonObject with job configuration (fileset, pool, storage, client) or empty object if not found
     * @since 2.9
     */
    QJsonObject jobConfiguration(const QString &jobName) const;
};

#endif // BJOBWIDGET_H
