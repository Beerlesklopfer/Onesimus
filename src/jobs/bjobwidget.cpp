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
    , m_levelCheckboxLayout(new QVBoxLayout())
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

    // Level checkboxes will be populated dynamically from .levels command

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

    connect(m_tableView, &BJsonJobView::jobActionRequested,
            this, [this](const QString &command, const QString &args) {
                // Map command to appropriate BDirector::Command enum
                BDirector::Command cmd;

                if (command == "cancel") {
                    cmd = BDirector::Command::Cancel;
                    emit statusMessageChanged(QString("Breche Job ab..."));
                } else if (command == "delete") {
                    cmd = BDirector::Command::Delete;
                    emit statusMessageChanged(QString("Lösche Job..."));
                } else if (command == "rerun") {
                    cmd = BDirector::Command::Rerun;
                    emit statusMessageChanged(QString("Führe Job erneut aus..."));
                } else if (command == "list") {
                    cmd = BDirector::Command::ListJobId;
                    emit statusMessageChanged(QString("Lade Job-Log..."));
                } else {
                    qWarning() << "Unknown job action command:" << command;
                    return;
                }

                // Forward to Director via signal
                emit sendCommand(cmd, args);
            });

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
    // Level checkbox connections will be established dynamically when created
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

    // Add placeholder label (will be replaced with checkboxes from .levels command)
    QLabel *levelPlaceholder = new QLabel(tr("Wird geladen..."), this);
    levelPlaceholder->setObjectName("levelPlaceholder");
    levelLayout->addWidget(levelPlaceholder);

    // Set the dynamic layout
    levelLayout->addLayout(m_levelCheckboxLayout);

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

void BJobWidget::setConnectionState(bool connected)
{
    // Enable/disable buttons based on connection state
    m_refreshButton->setEnabled(connected);

    bool hasSelection = !m_tableView->selectedJobIds().isEmpty();
    m_runJobButton->setEnabled(connected && hasSelection);
    m_cancelJobButton->setEnabled(connected && hasSelection);
    m_detailsButton->setEnabled(connected && hasSelection);

    // ✅ Request filter data from Director when connected
    // Note: Filter data will be requested automatically by onRefreshAll()
    // which is called after API mode is properly activated
    if (!connected) {
        // Clear combo boxes when disconnected
        m_nameFilter->clear();
        m_clientFilter->clear();
    }
}

void BJobWidget::requestFilterData()
{
    if (!m_director) {
        qWarning() << "BJobWidget::requestFilterData: No director set";
        return;
    }

#ifdef IS_DEVELOPER
    qDebug() << "BJobWidget: Requesting filter data using dot-commands (.jobs, .clients, .levels)";
#endif

    // Send .jobs command to get all configured jobs
    emit sendCommand(BDirector::Command::DotJobs, "");

    // Send .clients command to get all configured clients
    emit sendCommand(BDirector::Command::DotClients, "");

    // Send .levels command to get all backup levels
    emit sendCommand(BDirector::Command::DotLevels, "");
}

void BJobWidget::processDotJobsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    qDebug() << "BJobWidget: Processing .jobs response";
#endif

    // Update job names in filter model
    m_filterComboModel->updateJobNamesFromDotCommand(jsonData);

    // Update job name combo box
    QString currentNameFilter = m_nameFilter->currentText();
    m_nameFilter->clear();
    m_nameFilter->addItem("");  // Empty option to show all
    m_nameFilter->addItems(m_filterComboModel->jobNames());
    m_nameFilter->setCurrentText(currentNameFilter);  // Restore previous filter

#ifdef IS_DEVELOPER
    qDebug() << "✓ Job name filter updated with" << m_filterComboModel->jobNames().size() << "jobs";
#endif
}

void BJobWidget::processDotClientsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    qDebug() << "BJobWidget: Processing .clients response";
#endif

    // Update client names in filter model
    m_filterComboModel->updateClientNamesFromDotCommand(jsonData);

    // Update client name combo box
    QString currentClientFilter = m_clientFilter->currentText();
    m_clientFilter->clear();
    m_clientFilter->addItem("");  // Empty option to show all
    m_clientFilter->addItems(m_filterComboModel->clientNames());
    m_clientFilter->setCurrentText(currentClientFilter);  // Restore previous filter

#ifdef IS_DEVELOPER
    qDebug() << "✓ Client name filter updated with" << m_filterComboModel->clientNames().size() << "clients";
#endif
}

void BJobWidget::processDotLevelsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    qDebug() << "BJobWidget: Processing .levels response";
#endif

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "BJobWidget: Failed to parse .levels response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "BJobWidget: .levels response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray levelsArray = result["levels"].toArray();

    // Clear existing level checkboxes
    for (auto it = m_levelCheckboxes.begin(); it != m_levelCheckboxes.end(); ++it) {
        it.value()->deleteLater();
    }
    m_levelCheckboxes.clear();

    // Remove placeholder label if it exists
    QLabel *placeholder = findChild<QLabel*>("levelPlaceholder");
    if (placeholder) {
        placeholder->deleteLater();
    }

    // Create checkboxes for each level
    for (const QJsonValue &levelVal : levelsArray) {
        if (!levelVal.isObject()) {
            continue;
        }

        QJsonObject level = levelVal.toObject();
        QString levelCode = level["level"].toString();        // e.g., "F", "I", "D"
        QString levelName = level["description"].toString();  // e.g., "Full", "Incremental", "Differential"

        if (levelCode.isEmpty() || levelName.isEmpty()) {
            continue;
        }

        // Create checkbox
        QString checkboxText = QString("%1 (%2)").arg(levelName).arg(levelCode);
        QCheckBox *checkbox = new QCheckBox(checkboxText, this);
        checkbox->setChecked(true);  // Default: all checked

        // Connect to filter timer
        connect(checkbox, &QCheckBox::toggled, this, [this]() {
            m_filterTimer->start();
        });

        // Add to layout and map
        m_levelCheckboxLayout->addWidget(checkbox);
        m_levelCheckboxes.insert(levelCode, checkbox);
    }

#ifdef IS_DEVELOPER
    qDebug() << "✓ Created" << m_levelCheckboxes.size() << "level filter checkboxes";
#endif
}

void BJobWidget::onRefreshClicked()
{
    m_refreshButton->setEnabled(false);
    emit statusMessageChanged("Aktualisiere Job-Liste...");
    emit sendCommand(BDirector::Command::ListJobs, "100");
}

void BJobWidget::processJsonResponse(const QString &jsonData)
{
#ifdef DEBUG_JSON
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

#ifdef DEBUG_JSON
    qDebug() << "✓ JSON parsed successfully";
#endif

    // Get jobs array
    QJsonArray jobsArray = m_streamReader->jobsArray();

#ifdef DEBUG_JSON
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

    // Apply level filter - build QSet from dynamic checkboxes
    QSet<QString> levelSet;
    for (auto it = m_levelCheckboxes.constBegin(); it != m_levelCheckboxes.constEnd(); ++it) {
        if (it.value()->isChecked()) {
            levelSet.insert(it.key());  // Insert the level code (e.g., "F", "I", "D")
        }
    }

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

    // Check all level filters (dynamic)
    for (auto it = m_levelCheckboxes.constBegin(); it != m_levelCheckboxes.constEnd(); ++it) {
        it.value()->setChecked(true);
    }

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

void BJobWidget::clearData()
{
#ifdef IS_DEVELOPER
    qDebug() << "BJobWidget: Clearing all data";
#endif

    // Clear table model by setting empty array
    m_tableView->jobsModel()->setJobs(QJsonArray());

    // Clear combo boxes
    m_nameFilter->clear();
    m_clientFilter->clear();

    // Clear filter model
    m_filterComboModel->clear();

    // Clear dynamic level checkboxes
    for (auto it = m_levelCheckboxes.begin(); it != m_levelCheckboxes.end(); ++it) {
        it.value()->deleteLater();
    }
    m_levelCheckboxes.clear();

    // Clear stream reader
    m_streamReader->clear();

    // Reset statistics
    if (m_statsWidget) {
        // Statistics will automatically update from empty model
    }
}
