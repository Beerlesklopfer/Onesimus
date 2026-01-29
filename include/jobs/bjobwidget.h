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
#include "jobs/bjsonjobview.h"
#include "bjsonstreamreader.h"
#include "bpaginationwidget.h"
#include "jobs/bjobsstatisticswidget.h"
#include "bdirector.h"
#include "jobs/bfiltercombomodel.h"

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
     * @param parent Parent widget
     * @since 2.0
     */
    explicit BJobWidget(QWidget *parent = nullptr);
    
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
     * @brief Returns the statistics widget
     * @return Pointer to BJobsStatisticsWidget
     * @since 2.0
     */
    BJobsStatisticsWidget* statisticsWidget() const { return m_statsWidget; }

    /**
     * @brief Sets the director for job operations
     * @param director Pointer to BDirector
     * @since 2.0
     */
    void setDirector(BDirector *director) { m_director = director; }

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
     * @brief Handles selection changes in table
     * @since 1.0
     */
    void onJobSelectionChanged();
    
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
    BJobsStatisticsWidget *m_statsWidget;       ///< Statistics display (optional)

    // Integrated filter controls (from BJobsFilterWidget)
    QComboBox *m_nameFilter;                ///< Job name filter (editable combo box)
    QComboBox *m_clientFilter;              ///< Client name filter (editable combo box)
    BFilterComboModel *m_filterComboModel;  ///< Model for combo box data
    
    QCheckBox *m_statusSuccess;             ///< Filter: Successful (T)
    QCheckBox *m_statusWarning;             ///< Filter: Warning (W)
    QCheckBox *m_statusFailed;              ///< Filter: Failed (f)
    QCheckBox *m_statusError;               ///< Filter: Error (E)

    // Dynamic level checkboxes (populated from .levels dot-command)
    QMap<QString, QCheckBox*> m_levelCheckboxes;  ///< Map of level code -> checkbox (e.g., "F" -> Full checkbox)
    QVBoxLayout *m_levelCheckboxLayout;            ///< Layout containing level checkboxes

    QCheckBox *m_dateEnabled;               ///< Enable date range filter
    QDateTimeEdit *m_dateFrom;              ///< Date range start
    QDateTimeEdit *m_dateTo;                ///< Date range end

    QTimer *m_filterTimer;                  ///< Debounce timer for filters
    
    // Toolbar buttons
    QPushButton *m_runJobButton;            ///< Run job button
    QPushButton *m_cancelJobButton;         ///< Cancel job button
    QPushButton *m_detailsButton;           ///< Show details button
    QPushButton *m_refreshButton;           ///< Refresh button

    // Auto-refresh controls
    QCheckBox *m_autoRefreshCheck;          ///< Auto-refresh checkbox
    QComboBox *m_refreshIntervalCombo;      ///< Refresh interval selector
    QTimer *m_autoRefreshTimer;             ///< Auto-refresh timer
    
    QSplitter *m_splitter;                  ///< Splitter for table/filters
    QPushButton *m_toggleFiltersButton;     ///< Toggle filters button
    QWidget *m_filterContainer;             ///< Container for all filters
    bool m_filtersVisible;                  ///< Filter visibility state

    BDirector *m_director;                  ///< Director connection for job operations
};

#endif // BJOBWIDGET_H
