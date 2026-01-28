#include "jobs/bjobwidget.h"
#include "jobs/bjobdetailsdialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QSplitter>
#include <QGroupBox>
#include <QFormLayout>
#include <QTimer>
#include <QCheckBox>
#include <QDateTimeEdit>

BJobWidget::BJobWidget(QWidget *parent)
    : QWidget(parent)
    , m_tableView(new BJsonJobView(this))
    , m_streamReader(new BJsonStreamReader(this))
    , m_paginationWidget(new BPaginationWidget(this))
    , m_statsWidget(nullptr)   // Optional
    , m_nameFilter(new QComboBox(this))
    , m_clientFilter(new QComboBox(this))
    , m_filterComboModel(new BFilterComboModel(this))
    , m_statusSuccess(new QCheckBox(tr("Successful (T)"), this))
    , m_statusWarning(new QCheckBox(tr("Warning (W)"), this))
    , m_statusFailed(new QCheckBox(tr("Failed (f)"), this))
    , m_statusError(new QCheckBox(tr("Error (E)"), this))
    , m_levelFull(new QCheckBox(tr("Full (F)"), this))
    , m_levelIncremental(new QCheckBox(tr("Incremental (I)"), this))
    , m_levelDifferential(new QCheckBox(tr("Differential (D)"), this))
    , m_dateEnabled(new QCheckBox(tr("Enable Date Filter"), this))
    , m_dateFrom(new QDateTimeEdit(this))
    , m_dateTo(new QDateTimeEdit(this))
    , m_filterTimer(new QTimer(this))
    , m_autoRefreshTimer(new QTimer(this))
    , m_splitter(new QSplitter(Qt::Horizontal, this))
    , m_toggleFiltersButton(new QPushButton(this))
    , m_director(nullptr)
{
    // Initialize checkboxes - all checked by default
    m_statusSuccess->setChecked(true);
    m_statusWarning->setChecked(true);
    m_statusFailed->setChecked(true);
    m_statusError->setChecked(true);
    m_levelFull->setChecked(true);
    m_levelIncremental->setChecked(true);
    m_levelDifferential->setChecked(true);
    
    // Setup date time edits
    m_dateFrom->setCalendarPopup(true);
    m_dateFrom->setDateTime(QDateTime::currentDateTime().addDays(-30));
    m_dateFrom->setEnabled(false);
    m_dateTo->setCalendarPopup(true);
    m_dateTo->setDateTime(QDateTime::currentDateTime());
    m_dateTo->setEnabled(false);
    
    // Setup filter timer (debouncing)
    m_filterTimer->setSingleShot(true);
    m_filterTimer->setInterval(300);  // 300ms delay

    // Setup editable combo boxes
    m_nameFilter->setEditable(true);
    m_nameFilter->setPlaceholderText(tr("Nach Job-Name filtern..."));
    m_nameFilter->setInsertPolicy(QComboBox::NoInsert);  // Don't add typed text as item

    m_clientFilter->setEditable(true);
    m_clientFilter->setPlaceholderText(tr("Nach Client filtern..."));
    m_clientFilter->setInsertPolicy(QComboBox::NoInsert);  // Don't add typed text as item

    setupUI();
    
    // Connect table view signals
    connect(m_tableView, &BJsonJobView::jobDoubleClicked,
            this, &BJobWidget::onJobDoubleClicked);
    
    connect(m_tableView, &BJsonJobView::selectionChanged,
            this, &BJobWidget::onJobSelectionChanged);
    
    // Connect auto-refresh timer
    connect(m_autoRefreshTimer, &QTimer::timeout,
            this, &BJobWidget::onAutoRefreshTimeout);
    
    // Connect filter controls to debounced apply
    connect(m_nameFilter, &QComboBox::currentTextChanged,
            this, [this]() { m_filterTimer->start(); });
    connect(m_clientFilter, &QComboBox::currentTextChanged,
            this, [this]() { m_filterTimer->start(); });
    connect(m_statusSuccess, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_statusWarning, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_statusFailed, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_statusError, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_levelFull, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_levelIncremental, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_levelDifferential, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_dateEnabled, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_dateFrom, &QDateTimeEdit::dateTimeChanged,
            this, [this]() { if (m_dateEnabled->isChecked()) m_filterTimer->start(); });
    connect(m_dateTo, &QDateTimeEdit::dateTimeChanged,
            this, [this]() { if (m_dateEnabled->isChecked()) m_filterTimer->start(); });
    
    // Connect date enable to edit widgets
    connect(m_dateEnabled, &QCheckBox::toggled, m_dateFrom, &QDateTimeEdit::setEnabled);
    connect(m_dateEnabled, &QCheckBox::toggled, m_dateTo, &QDateTimeEdit::setEnabled);
    
    // Connect filter timer to apply
    connect(m_filterTimer, &QTimer::timeout, this, &BJobWidget::applyFilters);

    // Initial refresh
    onRefreshClicked();
}

BJobWidget::~BJobWidget()
{
}

void BJobWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // === TOOLBAR ===
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    // Toggle Filters Button (ganz links)
    m_toggleFiltersButton->setIcon(QIcon::fromTheme("view-filter"));
    m_toggleFiltersButton->setText(tr("Filter"));
    m_toggleFiltersButton->setCheckable(true);
    m_toggleFiltersButton->setChecked(true);
    m_toggleFiltersButton->setToolTip(tr("Filter ein-/ausblenden"));
    toolbarLayout->addWidget(m_toggleFiltersButton);

    toolbarLayout->addSpacing(10);

    // Job control buttons
    m_runJobButton = new QPushButton("Job ausführen", this);
    m_cancelJobButton = new QPushButton("Job abbrechen", this);
    m_detailsButton = new QPushButton("Details", this);
    m_refreshButton = new QPushButton("Aktualisieren", this);

    m_runJobButton->setIcon(QIcon::fromTheme("media-playback-start"));
    m_cancelJobButton->setIcon(QIcon::fromTheme("process-stop"));
    m_detailsButton->setIcon(QIcon::fromTheme("document-properties"));
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));

    toolbarLayout->addWidget(m_runJobButton);
    toolbarLayout->addWidget(m_cancelJobButton);
    toolbarLayout->addWidget(m_detailsButton);

    toolbarLayout->addSpacing(20);

    // Export buttons
    m_exportJsonButton = new QPushButton("Export JSON", this);
    m_exportCsvButton = new QPushButton("Export CSV", this);

    m_exportJsonButton->setIcon(QIcon::fromTheme("document-save"));
    m_exportCsvButton->setIcon(QIcon::fromTheme("text-csv"));

    toolbarLayout->addWidget(m_exportJsonButton);
    toolbarLayout->addWidget(m_exportCsvButton);

    toolbarLayout->addSpacing(20);

    // Auto-refresh controls
    m_autoRefreshCheck = new QCheckBox("Auto-Refresh", this);
    m_refreshIntervalCombo = new QComboBox(this);
    m_refreshIntervalCombo->addItem("5 Sek", 5000);
    m_refreshIntervalCombo->addItem("10 Sek", 10000);
    m_refreshIntervalCombo->addItem("30 Sek", 30000);
    m_refreshIntervalCombo->addItem("1 Min", 60000);
    m_refreshIntervalCombo->addItem("5 Min", 300000);
    m_refreshIntervalCombo->setCurrentIndex(2);
    m_refreshIntervalCombo->setEnabled(false);

    toolbarLayout->addWidget(m_autoRefreshCheck);
    toolbarLayout->addWidget(m_refreshIntervalCombo);

    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_refreshButton);

    mainLayout->addLayout(toolbarLayout);

    // === SPLITTER (Filter links | Table rechts) ===
    m_splitter->setHandleWidth(3);
    m_splitter->setChildrenCollapsible(true);

    // === FILTER CONTAINER (linke Seite) ===
    m_filterContainer = new QWidget(this);
    QVBoxLayout *filterLayout = new QVBoxLayout(m_filterContainer);
    filterLayout->setContentsMargins(0, 0, 5, 0);

    QGroupBox *filterGroup = new QGroupBox(tr("Filters"), m_filterContainer);
    QVBoxLayout *filterGroupLayout = new QVBoxLayout(filterGroup);

    // Text filters
    QGroupBox *textGroup = new QGroupBox(tr("Text Filters"), this);
    QFormLayout *textLayout = new QFormLayout(textGroup);

    // Placeholder text is set in constructor
    textLayout->addRow(tr("Job Name:"), m_nameFilter);
    textLayout->addRow(tr("Client:"), m_clientFilter);

    filterGroupLayout->addWidget(textGroup);

    // Status filters
    QGroupBox *statusGroup = new QGroupBox(tr("Status Filters"), this);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);

    statusLayout->addWidget(m_statusSuccess);
    statusLayout->addWidget(m_statusWarning);
    statusLayout->addWidget(m_statusFailed);
    statusLayout->addWidget(m_statusError);

    filterGroupLayout->addWidget(statusGroup);

    // Level filters
    QGroupBox *levelGroup = new QGroupBox(tr("Backup Level"), this);
    QVBoxLayout *levelLayout = new QVBoxLayout(levelGroup);

    levelLayout->addWidget(m_levelFull);
    levelLayout->addWidget(m_levelIncremental);
    levelLayout->addWidget(m_levelDifferential);

    filterGroupLayout->addWidget(levelGroup);

    // Date range
    QGroupBox *dateGroup = new QGroupBox(tr("Date Range"), this);
    QVBoxLayout *dateLayout = new QVBoxLayout(dateGroup);

    dateLayout->addWidget(m_dateEnabled);

    QFormLayout *dateFormLayout = new QFormLayout();
    dateFormLayout->addRow(tr("From:"), m_dateFrom);
    dateFormLayout->addRow(tr("To:"), m_dateTo);
    dateLayout->addLayout(dateFormLayout);

    filterGroupLayout->addWidget(dateGroup);

    filterGroupLayout->addStretch();

    filterLayout->addWidget(filterGroup);

    m_splitter->addWidget(m_filterContainer);

    // === TABLE CONTAINER (rechte Seite) ===
    QWidget *tableContainer = new QWidget(this);
    QVBoxLayout *tableLayout = new QVBoxLayout(tableContainer);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    tableLayout->addWidget(m_tableView);
    tableLayout->addWidget(m_paginationWidget);

    m_splitter->addWidget(tableContainer);

    // Splitter Größen: Filter 25%, Table 75%
    m_splitter->setStretchFactor(0, 1);  // Filter
    m_splitter->setStretchFactor(1, 3);  // Table

    mainLayout->addWidget(m_splitter);

    // === CONNECTIONS ===

    // Toggle filters button
    connect(m_toggleFiltersButton, &QPushButton::toggled,
            this, &BJobWidget::do_toggleFilters);

    // Pagination
    m_paginationWidget->setModel(m_tableView->jobsModel());

    // Button signals
    connect(m_runJobButton, &QPushButton::clicked,
            this, &BJobWidget::onRunJobClicked);
    connect(m_cancelJobButton, &QPushButton::clicked,
            this, &BJobWidget::onCancelJobClicked);
    connect(m_detailsButton, &QPushButton::clicked,
            this, &BJobWidget::onShowDetailsClicked);
    connect(m_refreshButton, &QPushButton::clicked,
            this, &BJobWidget::onRefreshClicked);

    connect(m_exportJsonButton, &QPushButton::clicked,
            [this]() { m_tableView->exportToJson(false); });
    connect(m_exportCsvButton, &QPushButton::clicked,
            [this]() { m_tableView->exportToCsv(false); });

    connect(m_autoRefreshCheck, &QCheckBox::toggled,
            this, &BJobWidget::toggleAutoRefresh);
    connect(m_refreshIntervalCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index) {
                if (m_autoRefreshCheck->isChecked()) {
                    toggleAutoRefresh(false);
                    toggleAutoRefresh(true);
                }
            });

    // Initial button states
    m_cancelJobButton->setEnabled(false);
    m_detailsButton->setEnabled(false);
}

void BJobWidget::onJobsReceived(const QList<BDirector::JobInfo> &jobs)
{
    // Convert Director jobs to JSON format
    QJsonArray jsonJobs = convertJobsToJson(jobs);
    
    // Update table view
    m_tableView->setJobsData(jsonJobs);
    
    // Update status
    emit statusMessageChanged(QString("Aktualisiert: %1 - %2 Jobs geladen")
        .arg(QDateTime::currentDateTime().toString("HH:mm:ss"))
        .arg(jobs.size()));
}

QJsonArray BJobWidget::convertJobsToJson(const QList<BDirector::JobInfo> &jobs)
{
    QJsonArray jsonArray;
    
    for (const BDirector::JobInfo &job : jobs) {
        QJsonObject jsonJob;
        
        jsonJob["jobid"] = QString::number(job.jobId);
        jsonJob["name"] = job.name;
        jsonJob["client"] = job.clientName;
        jsonJob["starttime"] = job.startTime.isValid() ? 
            job.startTime.toString("yyyy-MM-dd HH:mm:ss") : "";
        jsonJob["duration"] = job.duration;
        jsonJob["type"] = job.type;
        jsonJob["level"] = job.level;
        jsonJob["jobfiles"] = QString::number(job.jobFiles);
        jsonJob["jobbytes"] = QString::number(job.jobBytes);
        jsonJob["jobstatus"] = job.status;
        
        jsonArray.append(jsonJob);
    }
    
    return jsonArray;
}

void BJobWidget::onRunJobClicked()
{
    bool ok;
    QString jobName = QInputDialog::getText(this, "Job ausführen",
        "Job-Name:", QLineEdit::Normal, "", &ok);
    
    if (ok && !jobName.isEmpty()) {
        emit sendCommand(BDirector::Command::Run, jobName);
        
        QMessageBox::information(this, "Job gestartet", 
            QString("Job '%1' wurde gestartet.").arg(jobName));
        
        // Refresh after 2 seconds
        QTimer::singleShot(2000, this, &BJobWidget::onRefreshClicked);
    }
}

void BJobWidget::onCancelJobClicked()
{
    QSet<QString> selectedJobs = m_tableView->selectedJobIds();
    
    if (selectedJobs.isEmpty()) {
        QMessageBox::information(this, "Keine Auswahl",
            "Bitte wählen Sie einen Job zum Abbrechen aus.");
        return;
    }
    
    QString jobId = *selectedJobs.begin();
    
    int ret = QMessageBox::question(this, "Job abbrechen",
        QString("Möchten Sie Job ID %1 wirklich abbrechen?").arg(jobId),
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        emit sendCommand(BDirector::Command::Cancel, QString::number(jobId.toULongLong()));
        
        QMessageBox::information(this, "Job abgebrochen", 
            QString("Job %1 wurde abgebrochen.").arg(jobId));
        
        // Refresh after 1 second
        QTimer::singleShot(1000, this, &BJobWidget::onRefreshClicked);
    }
}

void BJobWidget::onShowDetailsClicked()
{
    QSet<QString> selectedJobs = m_tableView->selectedJobIds();
    
    if (selectedJobs.isEmpty()) {
        return;
    }
    
    // Get the first selected job
    QString jobId = *selectedJobs.begin();
    
    // Find the job in the model
    BJobsModel *model = m_tableView->jobsModel();
    QJsonArray allJobs = model->allJobs();
    
    for (const QJsonValue &jobValue : allJobs) {
        QJsonObject job = jobValue.toObject();
        if (job["jobid"].toString() == jobId) {
            // Show details dialog
            BJobDetailsDialog dialog(job, m_director, this);
            dialog.exec();
            break;
        }
    }
}

void BJobWidget::onRefreshClicked()
{
    m_refreshButton->setEnabled(false);
    emit statusMessageChanged("Aktualisiere Job-Liste...");
    emit sendCommand(BDirector::Command::ListJobs, "100");
}

void BJobWidget::processJsonResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    qDebug() << "========================================";
    qDebug() << "BJobWidget: Processing JSON response";
    qDebug() << "  Data size:" << jsonData.size() << "bytes";
    if (jsonData.size() < 500) {
        qDebug() << "  Raw JSON:" << jsonData;
    } else {
        qDebug() << "  First 500 chars:" << jsonData.left(500);
    }
    qDebug() << "========================================";
#endif

    // Clear previous data
    m_streamReader->clear();

    // Feed data to stream reader
    m_streamReader->receiveData(jsonData.toUtf8());

    // Parse JSON
    if (!m_streamReader->parseJson()) {
        qCritical() << "✗ Failed to parse JSON response";
        emit statusMessageChanged("Fehler beim Parsen der JSON-Daten");
        m_refreshButton->setEnabled(true);
        return;
    }

#ifdef IS_DEVELOPER
    qDebug() << "✓ JSON parsed successfully";
#endif

    // Get jobs array
    QJsonArray jobsArray = m_streamReader->jobsArray();

#ifdef IS_DEVELOPER
    qDebug() << "✓ Extracted" << jobsArray.size() << "jobs from JSON";
    if (jobsArray.isEmpty()) {
        qWarning() << "⚠ Jobs array is empty! Check JSON structure.";
    }
#endif

    // Update table view
    m_tableView->setJobsData(jobsArray);

#ifdef IS_DEVELOPER
    qDebug() << "✓ Table view updated";
#endif

    // Update filter combo box model
    m_filterComboModel->updateFromJobsArray(jobsArray);

    // Update combo boxes with new data
    QString currentNameFilter = m_nameFilter->currentText();
    QString currentClientFilter = m_clientFilter->currentText();

    // Update job name combo box
    m_nameFilter->clear();
    m_nameFilter->addItem("");  // Empty option to show all
    m_nameFilter->addItems(m_filterComboModel->jobNames());
    m_nameFilter->setCurrentText(currentNameFilter);  // Restore previous filter

    // Update client name combo box
    m_clientFilter->clear();
    m_clientFilter->addItem("");  // Empty option to show all
    m_clientFilter->addItems(m_filterComboModel->clientNames());
    m_clientFilter->setCurrentText(currentClientFilter);  // Restore previous filter

#ifdef IS_DEVELOPER
    qDebug() << "✓ Filter combo boxes updated with"
             << m_filterComboModel->jobNames().size() << "job names and"
             << m_filterComboModel->clientNames().size() << "client names";
#endif

    emit statusMessageChanged(QString("%1 Jobs geladen").arg(jobsArray.size()));
    m_refreshButton->setEnabled(true);
}

void BJobWidget::onJobSelectionChanged()
{
    bool hasSelection = !m_tableView->selectedJobIds().isEmpty();
    m_cancelJobButton->setEnabled(hasSelection);
    m_detailsButton->setEnabled(hasSelection);
}

void BJobWidget::onJobDoubleClicked(const QJsonObject &job)
{
    BJobDetailsDialog dialog(job, m_director, this);
    dialog.exec();
}

void BJobWidget::onAutoRefreshTimeout()
{
    onRefreshClicked();
}

void BJobWidget::toggleAutoRefresh(bool enabled)
{
    m_refreshIntervalCombo->setEnabled(enabled);
    m_refreshButton->setVisible(!enabled);  // Hide refresh button when auto-refresh is on
    
    if (enabled) {
        int interval = m_refreshIntervalCombo->currentData().toInt();
        m_autoRefreshTimer->start(interval);
        emit statusMessageChanged(tr("Auto-Refresh aktiviert (%1)")
            .arg(m_refreshIntervalCombo->currentText()));
    } else {
        m_autoRefreshTimer->stop();
        m_refreshButton->setVisible(true);  // Show refresh button again
        emit statusMessageChanged(tr("Auto-Refresh deaktiviert"));
    }
}

QString BJobWidget::formatBytes(qint64 bytes)
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    const qint64 TB = GB * 1024;
    
    if (bytes >= TB) {
        return QString::number(bytes / (double)TB, 'f', 2) + " TB";
    } else if (bytes >= GB) {
        return QString::number(bytes / (double)GB, 'f', 2) + " GB";
    } else if (bytes >= MB) {
        return QString::number(bytes / (double)MB, 'f', 2) + " MB";
    } else if (bytes >= KB) {
        return QString::number(bytes / (double)KB, 'f', 2) + " KB";
    } else {
        return QString::number(bytes) + " B";
    }
}

QString BJobWidget::formatJobStatus(const QString &status)
{
    if (status == "C") return "Erstellt";
    if (status == "R") return "Läuft";
    if (status == "B") return "Blockiert";
    if (status == "T") return "Beendet";
    if (status == "W") return "Wartet";
    if (status == "f") return "Erfolgreich";
    if (status == "E") return "Fehler";
    if (status == "e") return "Kritischer Fehler";
    if (status == "A") return "Abgebrochen";
    return status;
}

void BJobWidget::applyFilters()
{
    BJobsFilterModel *filterModel = m_tableView->filterModel();

    // Apply name filter from combo box
    filterModel->setNameFilter(m_nameFilter->currentText());

    // Apply client filter from combo box
    filterModel->setClientFilter(m_clientFilter->currentText());
    
    // Apply status filter - build QSet from checkboxes
    QSet<QString> statusSet;
    if (m_statusSuccess->isChecked()) statusSet.insert("T");
    if (m_statusWarning->isChecked()) statusSet.insert("W");
    if (m_statusFailed->isChecked()) statusSet.insert("f");
    if (m_statusError->isChecked()) statusSet.insert("E");
    
    filterModel->setStatusFilter(statusSet);
    
    // Apply level filter - build QSet from checkboxes
    QSet<QString> levelSet;
    if (m_levelFull->isChecked()) levelSet.insert("F");
    if (m_levelIncremental->isChecked()) levelSet.insert("I");
    if (m_levelDifferential->isChecked()) levelSet.insert("D");
    
    filterModel->setLevelFilter(levelSet);
    
    // Apply date range filter
    if (m_dateEnabled->isChecked()) {
        filterModel->setDateRange(m_dateFrom->dateTime(), m_dateTo->dateTime());
    } else {
        filterModel->setDateRange(QDateTime(), QDateTime());
    }
    
    // Update status label
    int visibleRows = filterModel->rowCount();
    int totalRows = m_tableView->jobsModel()->rowCount();
    
    if (visibleRows < totalRows) {
        emit statusMessageChanged(tr("Gefiltert: %1 von %2 Jobs")
            .arg(visibleRows)
            .arg(totalRows));
    } else {
        emit statusMessageChanged(tr("%1 Jobs").arg(totalRows));
    }
}

void BJobWidget::clearFilters()
{
    // Clear combo box filters (set to empty option)
    m_nameFilter->setCurrentText("");
    m_clientFilter->setCurrentText("");
    
    // Check all status filters
    m_statusSuccess->setChecked(true);
    m_statusWarning->setChecked(true);
    m_statusFailed->setChecked(true);
    m_statusError->setChecked(true);
    
    // Check all level filters
    m_levelFull->setChecked(true);
    m_levelIncremental->setChecked(true);
    m_levelDifferential->setChecked(true);
    
    // Disable date filter
    m_dateEnabled->setChecked(false);
    
    // Reset date range to default
    m_dateFrom->setDateTime(QDateTime::currentDateTime().addDays(-30));
    m_dateTo->setDateTime(QDateTime::currentDateTime());
    
    // Apply cleared filters
    applyFilters();
}

void BJobWidget::do_toggleFilters(bool visible)
{
    m_filtersVisible = visible;
    m_filterContainer->setVisible(visible);

    if (visible) {
        m_toggleFiltersButton->setText(tr("Filter"));
        m_toggleFiltersButton->setToolTip(tr("Filter ausblenden"));
    } else {
        m_toggleFiltersButton->setText(tr("Filter"));
        m_toggleFiltersButton->setToolTip(tr("Filter einblenden"));
    }
}
