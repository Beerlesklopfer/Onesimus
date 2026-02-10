#include "jobs/bjobwidget.h"
#include "blogging.h"
#include "jobs/bjobdetailsdialog.h"
#include "jobs/bnewjobdialog.h"
#include "jobs/blevelcolors.h"
#include "messages/bmessageswidget.h"
#include "config/bsettings.h"
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
#include <QListView>
#include <QScrollArea>
#include <QFontMetrics>
#include <QClipboard>
#include <QApplication>

BJobWidget::BJobWidget(BDirector *director, QWidget *parent)
    : QWidget(parent)
    , m_tableView(new BJsonJobView(director, this))
    , m_streamReader(new BJsonStreamReader(this))
    , m_paginationWidget(new BPaginationWidget(this))
    , m_lowerTabWidget(new QTabWidget(this))
    , m_messagesWidget(new BMessagesWidget(this))
    , m_logView(new QListView(this))
    , m_logModel(new BJobLogModel(this))
    , m_logTitleLabel(new QLabel(this))
    , m_logContainer(new QWidget(this))
    , m_logHistoryCombo(new QComboBox(this))
    , m_logCopyButton(new QPushButton(tr("Copy"), this))
    , m_logClearHistoryButton(new QPushButton(tr("Clear History"), this))
    , m_logSearchEdit(new QLineEdit(this))
    , m_nameFilter(new QComboBox(this))
    , m_clientFilter(new QComboBox(this))
    , m_filterComboModel(new BFilterComboModel(this))
    , m_filesetCombo(new QComboBox(this))
    , m_storageCombo(new QComboBox(this))
    , m_poolCombo(new QComboBox(this))
    , m_statusSuccess(new QCheckBox(tr("Successful (T)"), this))
    , m_statusWarning(new QCheckBox(tr("Warning (W)"), this))
    , m_statusFailed(new QCheckBox(tr("Failed (F)"), this))
    , m_statusError(new QCheckBox(tr("Error (E)"), this))
    , m_statusRunning(new QCheckBox(tr("Running (R)"), this))
    , m_statusCanceled(new QCheckBox(tr("Canceled (A)"), this))
    , m_statusZeroBytes(new QCheckBox(tr("Zero Bytes"), this))
    , m_levelCheckboxLayout(new QVBoxLayout())
    , m_dateEnabled(new QCheckBox(tr("Enable Date Filter"), this))
    , m_dateFrom(new QDateTimeEdit(this))
    , m_dateTo(new QDateTimeEdit(this))
    , m_filterTimer(new QTimer(this))
    , m_autoRefreshTimer(new QTimer(this))
    , m_splitter(new QSplitter(Qt::Horizontal, this))
    , m_toggleFiltersButton(new QPushButton(this))
    , m_filterToolBox(nullptr)  // Will be created in setupUI
    , m_director(director)
    , m_filesetModel(new BFilesetModel(this))
    , m_storageModel(new BStorageModel(this))
    , m_poolModel(new BPoolModel(this))
    , m_catalogModel(new BCatalogModel(this))
    , m_levelModel(new BLevelModel(this))
    , m_jobConfigModel(new BJobConfigModel(this))
{
    // Initialize checkboxes - all checked by default (except Zero Bytes)
    m_statusSuccess->setChecked(true);
    m_statusWarning->setChecked(true);
    m_statusFailed->setChecked(true);
    m_statusError->setChecked(true);
    m_statusRunning->setChecked(true);
    m_statusCanceled->setChecked(true);
    m_statusZeroBytes->setChecked(false);
    m_statusZeroBytes->setToolTip(
        tr("When enabled, only shows jobs with 0 bytes transferred.\n"
           "Warning: This filter persists across restarts and may\n"
           "cause an empty table if no zero-byte jobs exist."));

    // Level checkboxes will be populated dynamically from .levels command

    // Setup date time edits
    m_dateEnabled->setToolTip(
        tr("When enabled, only shows jobs within the selected date range.\n"
           "Warning: This filter persists across restarts and may\n"
           "cause an empty or limited table if the range is too narrow."));
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

    // Setup info combo boxes (read-only, show current job selection)
    m_filesetCombo->setEnabled(false);
    m_filesetCombo->setPlaceholderText(tr("(kein Job ausgewählt)"));

    m_storageCombo->setEnabled(false);
    m_storageCombo->setPlaceholderText(tr("(kein Job ausgewählt)"));

    m_poolCombo->setEnabled(false);
    m_poolCombo->setPlaceholderText(tr("(kein Job ausgewählt)"));

    // Setup log view
    m_logView->setModel(m_logModel);
    m_logView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_logView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_logView->setAlternatingRowColors(true);

    m_logTitleLabel->setText(tr("<b>Job Log</b> - Kein Job ausgewählt"));
    m_logTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Setup log history controls
    m_logHistoryCombo->setPlaceholderText(tr("-- Select Job Log --"));
    m_logHistoryCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_logHistoryCombo->setToolTip(tr("Select from previously loaded job logs"));

    m_logCopyButton->setToolTip(tr("Copy selected lines to clipboard"));
    m_logCopyButton->setIcon(QIcon::fromTheme("edit-copy"));

    m_logClearHistoryButton->setToolTip(tr("Clear job log history"));
    m_logClearHistoryButton->setIcon(QIcon::fromTheme("edit-clear"));

    m_logSearchEdit->setPlaceholderText(tr("Search in log..."));
    m_logSearchEdit->setClearButtonEnabled(true);
    m_logSearchEdit->setToolTip(tr("Filter log lines by search text"));

    setupUI();

    // Connect messages widget signals to forward commands
    connect(m_messagesWidget, &BMessagesWidget::sendCommand, this,
            [this](const BDirector::Command cmd, const QString &args) {
                emit sendCommand(cmd, args);
            });
    connect(m_messagesWidget, &BMessagesWidget::statusMessageChanged, this,
            [this](const QString &message) {
                emit statusMessageChanged(message);
            });

    // Connect table view signals
    connect(m_tableView, &BJsonJobView::jobDoubleClicked,
            this, &BJobWidget::onJobDoubleClicked);

    // Connect checkbox selection changes
    connect(m_tableView, &BJsonJobView::selectionChanged,
            this, &BJobWidget::onJobSelectionChanged);

    // Connect current row changes (click or keyboard navigation)
    bool connectionSuccess = connect(m_tableView, &BJsonJobView::currentRowChanged,
                                     this, &BJobWidget::onCurrentRowChanged);

    connect(m_tableView, &BJsonJobView::jobActionRequested,
            this, [this](const QString &command, const QString &args) {
                // Map command to appropriate BDirector::Command enum
                BDirector::Command cmd;

                if (command == "cancel") {
                    cmd = BDirector::Command::Cancel;
                    emit statusMessageChanged(QString("Breche Job ab..."));
                } else if (command == "delete") {
                    cmd = BDirector::Command::DeleteJob;
                    emit statusMessageChanged(QString("Lösche Job..."));
                } else if (command == "rerun") {
                    cmd = BDirector::Command::Rerun;
                    emit statusMessageChanged(QString("Führe Job erneut aus..."));
                } else if (command == "list") {
                    cmd = BDirector::Command::ListJobId;
                    emit statusMessageChanged(QString("Lade Job-Log..."));
                } else if (command == "purge") {
                    // Purge jobs - args contains "jobs jobid=XX yes"
                    cmd = BDirector::Command::Custom;
                    emit statusMessageChanged(QString("Lösche Job-Daten..."));
                    emit sendCommand(cmd, "purge " + args);
                    return;
                } else if (command == "prune") {
                    // Prune jobs - args contains "jobs client=XX yes"
                    cmd = BDirector::Command::Custom;
                    emit statusMessageChanged(QString("Prune abgelaufener Jobs..."));
                    emit sendCommand(cmd, "prune " + args);
                    return;
                } else {
                    BLOG_WARNING() << "Unknown job action command:" << command;
                    return;
                }

                // Forward to Director via signal
                emit sendCommand(cmd, args);
            });

    connect(m_tableView, &BJsonJobView::refreshRequested,
            this, &BJobWidget::onRefreshClicked);

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
    connect(m_statusRunning, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_statusCanceled, &QCheckBox::toggled,
            this, [this]() { m_filterTimer->start(); });
    connect(m_statusZeroBytes, &QCheckBox::toggled,
            this, [this](bool checked) {
                m_filterTimer->start();
                if (checked) {
                    emit statusMessageChanged(
                        tr("⚠ Zero Bytes filter active — only jobs with 0 bytes are shown. "
                           "This filter persists across restarts!"));
                }
            });
    // Level checkbox connections will be established dynamically when created
    connect(m_dateEnabled, &QCheckBox::toggled,
            this, [this](bool checked) {
                m_filterTimer->start();
                if (checked) {
                    emit statusMessageChanged(
                        tr("⚠ Date filter active — only jobs within the selected range are shown. "
                           "This filter persists across restarts!"));
                }
            });
    connect(m_dateFrom, &QDateTimeEdit::dateTimeChanged,
            this, [this]() { if (m_dateEnabled->isChecked()) m_filterTimer->start(); });
    connect(m_dateTo, &QDateTimeEdit::dateTimeChanged,
            this, [this]() { if (m_dateEnabled->isChecked()) m_filterTimer->start(); });
    
    // Connect date enable to edit widgets
    connect(m_dateEnabled, &QCheckBox::toggled, m_dateFrom, &QDateTimeEdit::setEnabled);
    connect(m_dateEnabled, &QCheckBox::toggled, m_dateTo, &QDateTimeEdit::setEnabled);
    
    // Connect filter timer to apply
    connect(m_filterTimer, &QTimer::timeout, this, &BJobWidget::applyFilters);

    // Connect combobox filters to save current index
    connect(m_nameFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                BSettings::instance().setJobsFilterCombobox("name", index);
            });
    connect(m_clientFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                BSettings::instance().setJobsFilterCombobox("client", index);
            });

    // Connect checkbox filters to save state
    connect(m_statusSuccess, &QCheckBox::toggled,
            this, [this](bool checked) {
                BSettings::instance().setJobsFilterCheckbox("status_success", checked);
            });
    connect(m_statusWarning, &QCheckBox::toggled,
            this, [this](bool checked) {
                BSettings::instance().setJobsFilterCheckbox("status_warning", checked);
            });
    connect(m_statusFailed, &QCheckBox::toggled,
            this, [this](bool checked) {
                BSettings::instance().setJobsFilterCheckbox("status_failed", checked);
            });
    connect(m_statusError, &QCheckBox::toggled,
            this, [this](bool checked) {
                BSettings::instance().setJobsFilterCheckbox("status_error", checked);
            });
    connect(m_statusRunning, &QCheckBox::toggled,
            this, [this](bool checked) {
                BSettings::instance().setJobsFilterCheckbox("status_running", checked);
            });
    connect(m_statusCanceled, &QCheckBox::toggled,
            this, [this](bool checked) {
                BSettings::instance().setJobsFilterCheckbox("status_canceled", checked);
            });
    connect(m_statusZeroBytes, &QCheckBox::toggled,
            this, [this](bool checked) {
                BSettings::instance().setJobsFilterCheckbox("status_zero_bytes", checked);
            });
    connect(m_dateEnabled, &QCheckBox::toggled,
            this, [this](bool checked) {
                BSettings::instance().setJobsFilterDateEnabled(checked);
            });

    // Connect date range to save state
    connect(m_dateFrom, &QDateTimeEdit::dateTimeChanged,
            this, [this](const QDateTime &dateTime) {
                BSettings::instance().setJobsFilterDateFrom(dateTime);
            });
    connect(m_dateTo, &QDateTimeEdit::dateTimeChanged,
            this, [this](const QDateTime &dateTime) {
                BSettings::instance().setJobsFilterDateTo(dateTime);
            });

    // Restore checkbox filter states from settings
    m_statusSuccess->setChecked(BSettings::instance().jobsFilterCheckbox("status_success", true));
    m_statusWarning->setChecked(BSettings::instance().jobsFilterCheckbox("status_warning", true));
    m_statusFailed->setChecked(BSettings::instance().jobsFilterCheckbox("status_failed", true));
    m_statusError->setChecked(BSettings::instance().jobsFilterCheckbox("status_error", true));
    m_statusRunning->setChecked(BSettings::instance().jobsFilterCheckbox("status_running", true));
    m_statusCanceled->setChecked(BSettings::instance().jobsFilterCheckbox("status_canceled", true));
    m_statusZeroBytes->setChecked(BSettings::instance().jobsFilterCheckbox("status_zero_bytes", false));
    m_dateEnabled->setChecked(BSettings::instance().jobsFilterDateEnabled());
    m_dateFrom->setDateTime(BSettings::instance().jobsFilterDateFrom());
    m_dateTo->setDateTime(BSettings::instance().jobsFilterDateTo());

    // Startup hint: warn if limiting filters are active from a previous session
    QTimer::singleShot(0, this, [this]() {
        QStringList activeHints;
        if (m_statusZeroBytes->isChecked()) {
            activeHints << tr("Zero Bytes");
        }
        if (m_dateEnabled->isChecked()) {
            activeHints << tr("Date Range (%1 – %2)")
                .arg(m_dateFrom->dateTime().toString("dd.MM.yyyy"))
                .arg(m_dateTo->dateTime().toString("dd.MM.yyyy"));
        }
        // Check if any status filter is unchecked
        QStringList disabledStatuses;
        if (!m_statusSuccess->isChecked()) disabledStatuses << "T";
        if (!m_statusWarning->isChecked()) disabledStatuses << "W";
        if (!m_statusFailed->isChecked()) disabledStatuses << "F";
        if (!m_statusError->isChecked()) disabledStatuses << "E";
        if (!m_statusRunning->isChecked()) disabledStatuses << "R";
        if (!m_statusCanceled->isChecked()) disabledStatuses << "A";
        if (!disabledStatuses.isEmpty()) {
            activeHints << tr("Status (hidden: %1)").arg(disabledStatuses.join(", "));
        }

        if (!activeHints.isEmpty()) {
            emit statusMessageChanged(
                tr("⚠ Active filters from last session: %1 — results may be limited")
                    .arg(activeHints.join("; ")));
        }
    });

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

    // Refresh button
    m_refreshButton = new QPushButton(tr("Refresh"), this);
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));

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

    // === FILTER CONTAINER (linke Seite) mit QToolBox ===
    m_filterContainer = new QWidget(this);
    QVBoxLayout *filterLayout = new QVBoxLayout(m_filterContainer);
    filterLayout->setContentsMargins(0, 0, 5, 0);
    filterLayout->setSpacing(4);

    // QToolBox für Akkordeon-Style Filter-Sektionen
    m_filterToolBox = new QToolBox(this);
    // Set minimum width relative to font size (~30 average characters)
    QFontMetrics fm(font());
    m_filterToolBox->setMinimumWidth(fm.averageCharWidth() * 30);

    // ========================================================================
    // Page 1: Basis-Filter (Text + Status)
    // ========================================================================
    QWidget *basicPage = new QWidget();
    QVBoxLayout *basicLayout = new QVBoxLayout(basicPage);
    basicLayout->setContentsMargins(8, 8, 8, 8);
    basicLayout->setSpacing(8);

    // Text filters
    QFormLayout *textLayout = new QFormLayout();
    textLayout->setSpacing(4);
    textLayout->addRow(tr("Job Name:"), m_nameFilter);
    textLayout->addRow(tr("Client:"), m_clientFilter);
    basicLayout->addLayout(textLayout);

    // Status filters - vertical list layout
    QLabel *statusLabel = new QLabel(tr("<b>Status:</b>"), this);
    basicLayout->addWidget(statusLabel);

    QVBoxLayout *statusLayout = new QVBoxLayout();
    statusLayout->setSpacing(4);
    statusLayout->addWidget(m_statusSuccess);
    statusLayout->addWidget(m_statusWarning);
    statusLayout->addWidget(m_statusFailed);
    statusLayout->addWidget(m_statusError);
    statusLayout->addWidget(m_statusRunning);
    statusLayout->addWidget(m_statusCanceled);
    statusLayout->addWidget(m_statusZeroBytes);
    basicLayout->addLayout(statusLayout);

    basicLayout->addStretch();

    m_filterToolBox->addItem(basicPage, QIcon::fromTheme("view-filter"), tr("Basic Filters"));

    // ========================================================================
    // Page 2: Backup Level (dynamisch)
    // ========================================================================
    QWidget *levelPage = new QWidget();
    QVBoxLayout *levelPageLayout = new QVBoxLayout(levelPage);
    levelPageLayout->setContentsMargins(8, 8, 8, 8);
    levelPageLayout->setSpacing(4);

    // Create scroll area for level checkboxes
    QScrollArea *levelScrollArea = new QScrollArea(this);
    levelScrollArea->setWidgetResizable(true);
    levelScrollArea->setFrameShape(QFrame::NoFrame);

    // Create widget to hold the checkboxes
    QWidget *levelScrollWidget = new QWidget(this);
    QVBoxLayout *levelLayout = new QVBoxLayout(levelScrollWidget);
    levelLayout->setContentsMargins(0, 0, 0, 0);
    levelLayout->setSpacing(4);

    // Add placeholder label (will be replaced with checkboxes from .levels command)
    QLabel *levelPlaceholder = new QLabel(tr("Wird geladen..."), this);
    levelPlaceholder->setObjectName("levelPlaceholder");
    levelLayout->addWidget(levelPlaceholder);

    // Set the dynamic layout for level checkboxes
    levelLayout->addLayout(m_levelCheckboxLayout);
    levelLayout->addStretch();

    levelScrollArea->setWidget(levelScrollWidget);
    levelPageLayout->addWidget(levelScrollArea);

    m_filterToolBox->addItem(levelPage, QIcon::fromTheme("folder"), tr("Backup Level"));

    // ========================================================================
    // Page 3: Zeitraum
    // ========================================================================
    QWidget *datePage = new QWidget();
    QVBoxLayout *datePageLayout = new QVBoxLayout(datePage);
    datePageLayout->setContentsMargins(8, 8, 8, 8);
    datePageLayout->setSpacing(8);

    datePageLayout->addWidget(m_dateEnabled);

    QFormLayout *dateFormLayout = new QFormLayout();
    dateFormLayout->setSpacing(4);
    dateFormLayout->addRow(tr("From:"), m_dateFrom);
    dateFormLayout->addRow(tr("To:"), m_dateTo);
    datePageLayout->addLayout(dateFormLayout);

    datePageLayout->addStretch();

    m_filterToolBox->addItem(datePage, QIcon::fromTheme("x-office-calendar"), tr("Date Range"));

    // ========================================================================
    // Page 4: Job-Details (ausgewählter Job)
    // ========================================================================
    QWidget *jobInfoPage = new QWidget();
    QVBoxLayout *jobInfoPageLayout = new QVBoxLayout(jobInfoPage);
    jobInfoPageLayout->setContentsMargins(8, 8, 8, 8);
    jobInfoPageLayout->setSpacing(4);

    QFormLayout *jobInfoLayout = new QFormLayout();
    jobInfoLayout->setSpacing(4);
    jobInfoLayout->addRow(tr("FileSet:"), m_filesetCombo);
    jobInfoLayout->addRow(tr("Storage:"), m_storageCombo);
    jobInfoLayout->addRow(tr("Pool:"), m_poolCombo);
    jobInfoPageLayout->addLayout(jobInfoLayout);

    jobInfoPageLayout->addStretch();

    m_filterToolBox->addItem(jobInfoPage, QIcon::fromTheme("dialog-information"), tr("Job Details"));

    filterLayout->addWidget(m_filterToolBox, 1);  // Stretch factor 1

    // Reset Filters Button
    m_resetFiltersButton = new QPushButton(tr("Filter zurücksetzen"), this);
    m_resetFiltersButton->setIcon(QIcon::fromTheme("edit-clear"));
    filterLayout->addWidget(m_resetFiltersButton);

    m_splitter->addWidget(m_filterContainer);

    // === RIGHT SIDE: Vertical Splitter for Table and Log ===
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, this);
    verticalSplitter->setHandleWidth(3);

    // === TABLE CONTAINER (oben) ===
    QWidget *tableContainer = new QWidget(this);
    QVBoxLayout *tableLayout = new QVBoxLayout(tableContainer);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    tableLayout->addWidget(m_tableView);
    tableLayout->addWidget(m_paginationWidget);

    verticalSplitter->addWidget(tableContainer);

    // === LOWER TAB WIDGET (Job Log + Messages) ===

    // Job Log Tab
    QVBoxLayout *logLayout = new QVBoxLayout(m_logContainer);
    logLayout->setContentsMargins(5, 5, 5, 5);
    logLayout->setSpacing(5);

    // Log toolbar with history selector and controls
    QHBoxLayout *logToolbarLayout = new QHBoxLayout();
    logToolbarLayout->setContentsMargins(0, 0, 0, 0);
    logToolbarLayout->setSpacing(5);

    logToolbarLayout->addWidget(new QLabel(tr("History:"), this));
    logToolbarLayout->addWidget(m_logHistoryCombo, 1);  // stretch factor 1
    logToolbarLayout->addWidget(m_logSearchEdit);
    logToolbarLayout->addWidget(m_logCopyButton);
    logToolbarLayout->addWidget(m_logClearHistoryButton);

    logLayout->addWidget(m_logTitleLabel);
    logLayout->addLayout(logToolbarLayout);
    logLayout->addWidget(m_logView);

    // Connect log history controls
    connect(m_logHistoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BJobWidget::onLogHistoryChanged);
    connect(m_logCopyButton, &QPushButton::clicked,
            this, &BJobWidget::onLogCopyClicked);
    connect(m_logClearHistoryButton, &QPushButton::clicked,
            this, &BJobWidget::onLogClearHistoryClicked);
    connect(m_logSearchEdit, &QLineEdit::textChanged,
            this, &BJobWidget::onLogSearchChanged);

    m_lowerTabWidget->addTab(m_logContainer, tr("Job Log"));

    // Messages Tab
    m_lowerTabWidget->addTab(m_messagesWidget, tr("Messages"));

    verticalSplitter->addWidget(m_lowerTabWidget);

    // Vertikaler Splitter Größen: Table 70%, Lower Panel 30%
    verticalSplitter->setStretchFactor(0, 7);  // Table
    verticalSplitter->setStretchFactor(1, 3);  // Lower Panel (Tabs)

    m_splitter->addWidget(verticalSplitter);

    // Splitter Größen: Filter 25%, Content 75%
    m_splitter->setStretchFactor(0, 1);  // Filter
    m_splitter->setStretchFactor(1, 3);  // Content (Table + Log)

    mainLayout->addWidget(m_splitter);

    // === CONNECTIONS ===

    // Toggle filters button
    connect(m_toggleFiltersButton, &QPushButton::toggled,
            this, &BJobWidget::do_toggleFilters);

    // Reset filters button
    connect(m_resetFiltersButton, &QPushButton::clicked,
            this, &BJobWidget::clearFilters);

    // Pagination
    m_paginationWidget->setModel(m_tableView->jobsModel());

    // Connect pagination for server-side pagination
    connect(m_paginationWidget, &BPaginationWidget::pageRequested,
            this, &BJobWidget::onPageRequested);

    // Auto-refresh when pagination is toggled
    connect(m_paginationWidget, &BPaginationWidget::paginationToggled,
            this, [this](bool /*enabled*/) {
                onRefreshClicked();
            });

    // Button signals
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
}

void BJobWidget::onJobsReceived(const QList<BDirector::JobInfo> &jobs)
{
    // Convert Director jobs to JSON format
    QJsonArray jsonJobs = convertJobsToJson(jobs);

    // Update table view
    m_tableView->setJobsData(jsonJobs);

    // Apply saved filters after loading jobs
    applyFilters();

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
    // Open new job dialog with JobWidget reference
    BNewJobDialog dialog(this, m_director, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Job was started, refresh after a short delay
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

    // Check if confirmation is required
    bool shouldConfirm = BSettings::instance().behaviorConfirmJobCancel();

    bool proceed = true;
    if (shouldConfirm) {
        int ret = QMessageBox::question(this, "Job abbrechen",
            QString("Möchten Sie Job ID %1 wirklich abbrechen?").arg(jobId),
            QMessageBox::Yes | QMessageBox::No);
        proceed = (ret == QMessageBox::Yes);
    }

    if (proceed) {
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
            // Show details dialog (pass this BJobWidget for shared log model)
            BJobDetailsDialog dialog(job, this, m_director, this);
            dialog.exec();
            break;
        }
    }
}

void BJobWidget::setConnectionState(bool connected)
{
    // Enable/disable buttons based on connection state
    m_refreshButton->setEnabled(connected);

    // Update messages widget connection state
    m_messagesWidget->setConnectionState(connected);

    // ✅ Request filter data from Director when connected
    // Note: Filter data will be requested automatically by onRefreshAll()
    // which is called after API mode is properly activated
    if (connected) {
        // Start message polling
        int msgPollInterval = BSettings::instance().messagesPollInterval();
        if (msgPollInterval > 0) {
            m_messagesWidget->startPolling(msgPollInterval);
        }
    } else {
        // Clear combo boxes when disconnected
        m_nameFilter->clear();
        m_clientFilter->clear();
        m_messagesWidget->stopPolling();
    }
}

void BJobWidget::requestFilterData()
{
    if (!m_director) {
        BLOG_WARNING() << "BJobWidget::requestFilterData: No director set";
        return;
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Requesting filter data using dot-commands (.jobs, .clients, .levels)";
#endif

    // Send .jobs command to get all configured jobs
    emit sendCommand(BDirector::Command::DotJobs, "");

    // Send .clients command to get all configured clients
    emit sendCommand(BDirector::Command::DotClients, "");

    // Send .levels command to get all backup levels
    emit sendCommand(BDirector::Command::DotLevels, "");

    // Send .filesets command to get all configured filesets
    emit sendCommand(BDirector::Command::DotFilesets, "");

    // Send .storages command to get all configured storages
    emit sendCommand(BDirector::Command::DotStorages, "");

    // Send .pools command to get all configured pools
    emit sendCommand(BDirector::Command::DotPools, "");

    // Send .catalogs command to get all configured catalogs
    emit sendCommand(BDirector::Command::DotCatalogs, "");
}

void BJobWidget::processDotJobsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Processing .jobs response";
#endif

    // Parse JSON to extract job configurations
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
        QJsonObject root = doc.object();
        QJsonObject result = root["result"].toObject();
        QJsonArray jobsArray = result["jobs"].toArray();

        // Clear previous configurations
        m_jobConfigurations.clear();

        // Store job configurations by name
        for (const QJsonValue &jobVal : jobsArray) {
            if (jobVal.isObject()) {
                QJsonObject jobConfig = jobVal.toObject();
                QString jobName = jobConfig["name"].toString();
                if (!jobName.isEmpty()) {
                    m_jobConfigurations[jobName] = jobConfig;
                }
            }
        }

#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "✓ Stored" << m_jobConfigurations.size() << "job configurations";
        // Show first config as sample
        if (!m_jobConfigurations.isEmpty()) {
            QString firstKey = m_jobConfigurations.firstKey();
            BLOG_DEBUG() << "  Sample config for" << firstKey << ":" << m_jobConfigurations[firstKey];
        }
#endif
    }

    // Update job names in filter model
    m_filterComboModel->updateJobNamesFromDotCommand(jsonData);

    // Update job name combo box
    int savedNameIndex = BSettings::instance().jobsFilterCombobox("name", 0);
    m_nameFilter->clear();
    m_nameFilter->addItem("");  // Empty option to show all
    m_nameFilter->addItems(m_filterComboModel->jobNames());

    // Restore saved index if valid
    if (savedNameIndex >= 0 && savedNameIndex < m_nameFilter->count()) {
        m_nameFilter->setCurrentIndex(savedNameIndex);
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Job name filter updated with" << m_filterComboModel->jobNames().size() << "jobs";
#endif
}

void BJobWidget::processDotClientsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Processing .clients response";
#endif

    // Update client names in filter model
    m_filterComboModel->updateClientNamesFromDotCommand(jsonData);

    // Update client name combo box
    int savedClientIndex = BSettings::instance().jobsFilterCombobox("client", 0);
    m_clientFilter->clear();
    m_clientFilter->addItem("");  // Empty option to show all
    m_clientFilter->addItems(m_filterComboModel->clientNames());

    // Restore saved index if valid
    if (savedClientIndex >= 0 && savedClientIndex < m_clientFilter->count()) {
        m_clientFilter->setCurrentIndex(savedClientIndex);
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Client name filter updated with" << m_filterComboModel->clientNames().size() << "clients";
#endif
}

void BJobWidget::processDotLevelsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Processing .levels response";
#endif

    // Parse response using model
    m_levelModel->parseLevels(jsonData);

    // Clear existing level checkboxes - must remove from layout BEFORE deleteLater
    for (auto it = m_levelCheckboxes.begin(); it != m_levelCheckboxes.end(); ++it) {
        m_levelCheckboxLayout->removeWidget(it.value());
        it.value()->deleteLater();
    }
    m_levelCheckboxes.clear();

    // Remove placeholder label if it exists
    QLabel *placeholder = findChild<QLabel*>("levelPlaceholder");
    if (placeholder) {
        placeholder->deleteLater();
    }

    // Get level codes and descriptions from model
    QStringList levelCodes = m_levelModel->levelCodes();
    QStringList levelDescriptions = m_levelModel->levelDescriptions();

    // Get visible levels from settings
    QStringList visibleLevels = BSettings::instance().visibleLevels();

    // Create checkboxes for each level (skip duplicates and non-visible levels)
    for (int i = 0; i < levelCodes.size() && i < levelDescriptions.size(); ++i) {
        QString levelCode = levelCodes[i];
        QString levelName = levelDescriptions[i];

        if (levelCode.isEmpty() || levelName.isEmpty()) {
            continue;
        }

        // Skip if checkbox for this level already exists
        if (m_levelCheckboxes.contains(levelCode)) {
            continue;
        }

        // Skip if level is not in visible levels (filter by level name)
        if (!visibleLevels.contains(levelName)) {
            continue;
        }

        // Create checkbox
        QString checkboxText = QString("%1 (%2)").arg(levelName).arg(levelCode);
        QCheckBox *checkbox = new QCheckBox(checkboxText, this);

        // Apply background color based on level
        QColor levelColor = BLevelColors::getLevelColor(levelCode);
        if (levelColor.isValid()) {
            // Create a stylesheet with the level color as background
            QString styleSheet = QString(
                "QCheckBox { "
                "  background-color: rgb(%1, %2, %3); "
                "  padding: 3px; "
                "  border-radius: 3px; "
                "}"
            ).arg(levelColor.red()).arg(levelColor.green()).arg(levelColor.blue());
            checkbox->setStyleSheet(styleSheet);
        }

        // Restore state from settings (default to checked)
        QString checkboxKey = QString("level_%1").arg(levelCode);
        checkbox->setChecked(BSettings::instance().jobsFilterCheckbox(checkboxKey, true));

        // Connect to filter timer and save state
        connect(checkbox, &QCheckBox::toggled, this, [this, levelCode, checkboxKey](bool checked) {
            m_filterTimer->start();

            // Save current level filter state
            BSettings::instance().setJobsFilterCheckbox(checkboxKey, checked);
        });

        // Add to layout and map
        m_levelCheckboxLayout->addWidget(checkbox);
        m_levelCheckboxes.insert(levelCode, checkbox);
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Created" << m_levelCheckboxes.size() << "level filter checkboxes";
#endif
}

void BJobWidget::processDotFilesetsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Processing .filesets response";
#endif

    // Parse response using model
    m_filesetModel->parseFilesets(jsonData);

    // Update combo box with all filesets (this provides the options for user selection)
    // Note: The current job's fileset will be selected when a row is clicked
    m_filesetCombo->clear();
    m_filesetCombo->addItems(m_filesetModel->filesetNames());

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Loaded" << m_filesetModel->filesetNames().size() << "filesets";
#endif
}

void BJobWidget::processDotStoragesResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Processing .storages response";
#endif

    // Parse response using model
    m_storageModel->parseStorages(jsonData);

    // Update combo box with all storages
    m_storageCombo->clear();
    m_storageCombo->addItems(m_storageModel->storageNames());

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Loaded" << m_storageModel->storageNames().size() << "storages";
#endif
}

void BJobWidget::processDotPoolsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Processing .pools response";
#endif

    // Parse response using model
    m_poolModel->parsePools(jsonData);

    // Update combo box with all pools
    m_poolCombo->clear();
    m_poolCombo->addItems(m_poolModel->poolNames());

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Loaded" << m_poolModel->poolNames().size() << "pools";
#endif
}

void BJobWidget::processDotCatalogsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Processing .catalogs response";
#endif

    m_catalogModel->parseCatalogs(jsonData);

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Loaded" << m_catalogModel->catalogNames().size() << "catalogs";
#endif
}

void BJobWidget::processJobTotalsResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Processing list jobtotals response";
#endif

    // Parse JSON to extract total job count
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "Failed to parse jobtotals response:" << parseError.errorString();
        m_refreshButton->setEnabled(true);
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "Jobtotals response is not a JSON object";
        m_refreshButton->setEnabled(true);
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Extract total job count from jobtotals
    // Bareos returns jobtotals as an object: {"jobs": "2163", "files": "...", "bytes": "..."}
    // with string-typed numeric values
    int totalJobs = 0;
    if (result.contains("jobtotals")) {
        QJsonValue jt = result["jobtotals"];
        if (jt.isObject()) {
            // Bareos format: jobtotals is a summary object
            QJsonObject totals = jt.toObject();
            totalJobs = totals["jobs"].toVariant().toInt();
        } else if (jt.isArray()) {
            // Fallback: array of per-type totals
            QJsonArray jobtotals = jt.toArray();
            for (const QJsonValue &v : jobtotals) {
                QJsonObject total = v.toObject();
                if (total.contains("jobs")) {
                    totalJobs += total["jobs"].toVariant().toInt();
                }
            }
        }
    }

    // totalJobs parsed from jobtotals response

    // Update pagination widget with total count
    m_paginationWidget->setTotalJobCount(totalJobs);

    // Fetch first page of jobs
    int pageSize = m_paginationWidget->pageSize();
    if (BSettings::instance().behaviorJobsNewestFirst()) {
        // Reverse offset: page 0 = newest jobs
        int reverseOffset = qMax(0, totalJobs - pageSize);
        emit sendCommand(BDirector::Command::ListJobs, QString("%1,%2").arg(pageSize).arg(reverseOffset));
    } else {
        // Normal offset: page 0 = oldest jobs
        emit sendCommand(BDirector::Command::ListJobs, QString("%1,0").arg(pageSize));
    }
}

void BJobWidget::processMessagesResponse(const QString &jsonData)
{
    // Forward to the messages widget
    if (m_messagesWidget) {
        m_messagesWidget->processMessagesResponse(jsonData);
    }
}

void BJobWidget::onPageRequested(int page, int pageSize)
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Page requested - page:" << page << "pageSize:" << pageSize;
#endif

    m_refreshButton->setEnabled(false);
    emit statusMessageChanged(tr("Lade Seite %1...").arg(page + 1));

    if (BSettings::instance().behaviorJobsNewestFirst()) {
        // Reverse offset: page 0 = newest jobs, page N = oldest jobs
        int totalJobs = m_paginationWidget->totalJobCount();
        int reverseOffset = qMax(0, totalJobs - (page + 1) * pageSize);
        int actualLimit = qMin(pageSize, totalJobs - page * pageSize);
        if (actualLimit <= 0) actualLimit = pageSize;
        emit sendCommand(BDirector::Command::ListJobs, QString("%1,%2").arg(actualLimit).arg(reverseOffset));
    } else {
        // Normal offset: page 0 = oldest jobs
        int offset = page * pageSize;
        emit sendCommand(BDirector::Command::ListJobs, QString("%1,%2").arg(pageSize).arg(offset));
    }
}

void BJobWidget::onRefreshClicked()
{
    m_refreshButton->setEnabled(false);
    emit statusMessageChanged(tr("Aktualisiere Job-Liste..."));

    if (m_paginationWidget->isPaginationEnabled()) {
        // Server-side pagination: First fetch job totals to know total count
        emit sendCommand(BDirector::Command::ListJobTotals, "");
    } else {
        // No pagination: Fetch jobs based on sort order setting
        int maxJobs = BSettings::instance().behaviorMaxJobsDisplay();
        if (BSettings::instance().behaviorJobsNewestFirst()) {
            emit sendCommand(BDirector::Command::ListJobsLast, QString::number(maxJobs));
        } else {
            emit sendCommand(BDirector::Command::ListJobs, QString::number(maxJobs));
        }
    }
}

void BJobWidget::processJsonResponse(const QString &jsonData)
{
#ifdef DEBUG_JSON
    BLOG_DEBUG() << "========================================";
    BLOG_DEBUG() << "BJobWidget: Processing JSON response";
    BLOG_DEBUG() << "  Data size:" << jsonData.size() << "bytes";
    if (jsonData.size() < 500) {
        BLOG_DEBUG() << "  Raw JSON:" << jsonData;
    } else {
        BLOG_DEBUG() << "  First 500 chars:" << jsonData.left(500);
    }
    BLOG_DEBUG() << "========================================";
#endif

    // Clear previous data
    m_streamReader->clear();

    // Feed data to stream reader
    m_streamReader->receiveData(jsonData.toUtf8());

    // Parse JSON
    if (!m_streamReader->parseJson()) {
        BLOG_ERROR() << "✗ Failed to parse JSON response";
        emit statusMessageChanged(tr("Error parsing JSON data"));
        m_refreshButton->setEnabled(true);
        return;
    }

#ifdef DEBUG_JSON
    BLOG_DEBUG() << "✓ JSON parsed successfully";
#endif

    // Get jobs array
    QJsonArray jobsArray = m_streamReader->jobsArray();

    // Enrich job data with configuration information (fileset, storage, pool)
    QJsonArray enrichedJobsArray;
    for (const QJsonValue &jobVal : jobsArray) {
        if (!jobVal.isObject()) {
            enrichedJobsArray.append(jobVal);
            continue;
        }

        QJsonObject job = jobVal.toObject();
        QString jobName = job["name"].toString();

        // Find matching job configuration
        if (m_jobConfigurations.contains(jobName)) {
            QJsonObject jobConfig = m_jobConfigurations[jobName];

            // Add fileset, storage, pool from configuration
            if (jobConfig.contains("fileset") && !jobConfig["fileset"].toString().isEmpty()) {
                job["fileset"] = jobConfig["fileset"];
            }
            if (jobConfig.contains("storage") && !jobConfig["storage"].toString().isEmpty()) {
                job["storage"] = jobConfig["storage"];
            }
            if (jobConfig.contains("pool") && !jobConfig["pool"].toString().isEmpty()) {
                job["pool"] = jobConfig["pool"];
            }
        }

        enrichedJobsArray.append(job);
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Enriched" << enrichedJobsArray.size() << "jobs with configuration data";
#endif

    // Update table view with enriched data
    m_tableView->setJobsData(enrichedJobsArray);

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Table view updated";
#endif

    // Update filter combo box model with enriched data
    m_filterComboModel->updateFromJobsArray(enrichedJobsArray);

    // Update combo boxes with new data
    int savedNameIndex = BSettings::instance().jobsFilterCombobox("name", 0);
    int savedClientIndex = BSettings::instance().jobsFilterCombobox("client", 0);

    // Update job name combo box
    m_nameFilter->clear();
    m_nameFilter->addItem("");  // Empty option to show all
    m_nameFilter->addItems(m_filterComboModel->jobNames());

    // Restore saved index if valid
    if (savedNameIndex >= 0 && savedNameIndex < m_nameFilter->count()) {
        m_nameFilter->setCurrentIndex(savedNameIndex);
    }

    // Update client name combo box
    m_clientFilter->clear();
    m_clientFilter->addItem("");  // Empty option to show all
    m_clientFilter->addItems(m_filterComboModel->clientNames());

    // Restore saved index if valid
    if (savedClientIndex >= 0 && savedClientIndex < m_clientFilter->count()) {
        m_clientFilter->setCurrentIndex(savedClientIndex);
    }

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "✓ Filter combo boxes updated with"
             << m_filterComboModel->jobNames().size() << "job names and"
             << m_filterComboModel->clientNames().size() << "client names";
#endif

    emit statusMessageChanged(QString("%1 Jobs geladen").arg(enrichedJobsArray.size()));
    m_refreshButton->setEnabled(true);

    // Apply saved filters after loading jobs
    applyFilters();
}

void BJobWidget::onJobSelectionChanged()
{
    bool hasSelection = !m_tableView->selectedJobIds().isEmpty();

    // Update job info combo boxes based on selection
    if (hasSelection) {
        // Get the first selected job
        QJsonObject selectedJob = m_tableView->getSelectedJob();

        if (!selectedJob.isEmpty()) {
            // Extract fileset, storage, pool from selected job
            QString fileset = selectedJob["fileset"].toString();
            QString storage = selectedJob["storage"].toString();
            QString pool = selectedJob["pool"].toString();

            BLOG_DEBUG() << "  JobID:" << selectedJob["jobid"].toString();
            BLOG_DEBUG() << "  Name:" << selectedJob["name"].toString();
            BLOG_DEBUG() << "  Fileset:" << fileset;
            BLOG_DEBUG() << "  Storage:" << storage;
            BLOG_DEBUG() << "  Pool:" << pool;

            // Update FileSet combo - fill with all filesets and select current job's fileset
            m_filesetCombo->clear();
            m_filesetCombo->addItem("");  // Empty placeholder at index 0
            m_filesetCombo->addItems(m_filesetModel->filesetNames());
            if (!fileset.isEmpty()) {
                int index = m_filesetCombo->findText(fileset);
                if (index >= 0) {
                    m_filesetCombo->setCurrentIndex(index);
                } else {
                    // Value not in list - add it and select
                    m_filesetCombo->addItem(fileset);
                    m_filesetCombo->setCurrentIndex(m_filesetCombo->count() - 1);
                }
            } else {
                m_filesetCombo->setCurrentIndex(0);  // Show empty placeholder
            }

            // Update Storage combo - fill with all storages and select current job's storage
            m_storageCombo->clear();
            m_storageCombo->addItem("");  // Empty placeholder at index 0
            m_storageCombo->addItems(m_storageModel->storageNames());
            if (!storage.isEmpty()) {
                int index = m_storageCombo->findText(storage);
                if (index >= 0) {
                    m_storageCombo->setCurrentIndex(index);
                } else {
                    // Value not in list - add it and select
                    m_storageCombo->addItem(storage);
                    m_storageCombo->setCurrentIndex(m_storageCombo->count() - 1);
                }
            } else {
                m_storageCombo->setCurrentIndex(0);  // Show empty placeholder
            }

            // Update Pool combo - fill with all pools and select current job's pool
            m_poolCombo->clear();
            m_poolCombo->addItem("");  // Empty placeholder at index 0
            m_poolCombo->addItems(m_poolModel->poolNames());
            if (!pool.isEmpty()) {
                int index = m_poolCombo->findText(pool);
                if (index >= 0) {
                    m_poolCombo->setCurrentIndex(index);
                } else {
                    // Value not in list - add it and select
                    m_poolCombo->addItem(pool);
                    m_poolCombo->setCurrentIndex(m_poolCombo->count() - 1);
                }
            } else {
                m_poolCombo->setCurrentIndex(0);  // Show empty placeholder
            }

            // Enable combo boxes (visual feedback)
            m_filesetCombo->setEnabled(true);
            m_storageCombo->setEnabled(true);
            m_poolCombo->setEnabled(true);

            // Load job log for selected job
            loadSelectedJobLog();
        } else {
        }
    } else {
        // No selection - clear and disable combo boxes
        m_filesetCombo->clear();
        m_storageCombo->clear();
        m_poolCombo->clear();

        m_filesetCombo->setEnabled(false);
        m_storageCombo->setEnabled(false);
        m_poolCombo->setEnabled(false);

        // Clear log display
        m_logModel->clear();
        m_logTitleLabel->setText(tr("<b>Job Log</b> - Kein Job ausgewählt"));
    }
}

void BJobWidget::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    BLOG_DEBUG() << "  Previous row:" << previous.row();
    BLOG_DEBUG() << "  Current row:" << current.row();
    BLOG_DEBUG() << "  Current valid:" << current.isValid();

    if (!current.isValid()) {
        // Clear and disable combo boxes
        m_filesetCombo->clear();
        m_storageCombo->clear();
        m_poolCombo->clear();
        m_filesetCombo->setEnabled(false);
        m_storageCombo->setEnabled(false);
        m_poolCombo->setEnabled(false);

        // Clear log display
        m_logModel->clear();
        m_logTitleLabel->setText(tr("<b>Job Log</b> - Kein Job ausgewählt"));
        return;
    }

    // Get the job at the current row by mapping through filter model
    QModelIndex sourceIndex = m_tableView->filterModel()->mapToSource(current);

    if (!sourceIndex.isValid()) {
        return;
    }

    // Get job data from model
    QJsonObject job = m_tableView->jobsModel()->jobAt(sourceIndex.row());

    if (job.isEmpty()) {
        return;
    }

    // Extract job details
    QString fileset = job["fileset"].toString();
    QString storage = job["storage"].toString();
    QString pool = job["pool"].toString();
    QString jobId = job["jobid"].toString();
    QString jobName = job["name"].toString();

    BLOG_DEBUG() << "  JobID:" << jobId;
    BLOG_DEBUG() << "  Name:" << jobName;
    BLOG_DEBUG() << "  Fileset:" << fileset;
    BLOG_DEBUG() << "  Storage:" << storage;
    BLOG_DEBUG() << "  Pool:" << pool;

    // Update FileSet combo - fill with all filesets and select current job's fileset
    m_filesetCombo->clear();
    m_filesetCombo->addItem("");  // Empty placeholder at index 0
    m_filesetCombo->addItems(m_filesetModel->filesetNames());
    if (!fileset.isEmpty()) {
        int index = m_filesetCombo->findText(fileset);
        if (index >= 0) {
            m_filesetCombo->setCurrentIndex(index);
        } else {
            // Value not in list - add it and select
            m_filesetCombo->addItem(fileset);
            m_filesetCombo->setCurrentIndex(m_filesetCombo->count() - 1);
        }
    } else {
        m_filesetCombo->setCurrentIndex(0);  // Show empty placeholder
    }

    // Update Storage combo - fill with all storages and select current job's storage
    m_storageCombo->clear();
    m_storageCombo->addItem("");  // Empty placeholder at index 0
    m_storageCombo->addItems(m_storageModel->storageNames());
    if (!storage.isEmpty()) {
        int index = m_storageCombo->findText(storage);
        if (index >= 0) {
            m_storageCombo->setCurrentIndex(index);
        } else {
            // Value not in list - add it and select
            m_storageCombo->addItem(storage);
            m_storageCombo->setCurrentIndex(m_storageCombo->count() - 1);
        }
    } else {
        m_storageCombo->setCurrentIndex(0);  // Show empty placeholder
    }

    // Update Pool combo - fill with all pools and select current job's pool
    m_poolCombo->clear();
    m_poolCombo->addItem("");  // Empty placeholder at index 0
    m_poolCombo->addItems(m_poolModel->poolNames());
    if (!pool.isEmpty()) {
        int index = m_poolCombo->findText(pool);
        if (index >= 0) {
            m_poolCombo->setCurrentIndex(index);
        } else {
            // Value not in list - add it and select
            m_poolCombo->addItem(pool);
            m_poolCombo->setCurrentIndex(m_poolCombo->count() - 1);
        }
    } else {
        m_poolCombo->setCurrentIndex(0);  // Show empty placeholder
    }

    // Enable combo boxes
    m_filesetCombo->setEnabled(true);
    m_storageCombo->setEnabled(true);
    m_poolCombo->setEnabled(true);

    // Load job log
    if (!jobId.isEmpty()) {
        m_logTitleLabel->setText(QString("<b>Job Log</b> - Job %1: %2 - Lädt...").arg(jobId, jobName));

        // Clear current log
        m_logModel->clear();

        // Store the job ID and name we're requesting (for use in response handler)
        m_pendingLogJobId = jobId;
        m_pendingLogJobName = jobName;

        // Connect to Director signal if not already connected
        if (m_director) {
            connect(m_director, &BDirector::jsonResult,
                    this, &BJobWidget::onJobLogReceived,
                    Qt::UniqueConnection);

            // Request job log
            QMetaObject::invokeMethod(m_director, "doSend",
                                      Qt::QueuedConnection,
                                      Q_ARG(BDirector::Command, BDirector::Command::ListJobId),
                                      Q_ARG(QString, jobId));
        }
    } else {
        m_pendingLogJobId.clear();
        m_pendingLogJobName.clear();
    }

}

void BJobWidget::onJobDoubleClicked(const QJsonObject &job)
{
    // Pass this BJobWidget for shared log model access
    BJobDetailsDialog dialog(job, this, m_director, this);
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
    if (status == "C") return tr("Created");
    if (status == "R") return tr("Running");
    if (status == "B") return tr("Blocked");
    if (status == "T") return tr("OK");
    if (status == "W") return tr("Warning");
    if (status == "F") return tr("Failed");
    if (status == "E") return tr("Error");
    if (status == "e") return tr("Non-fatal Error");
    if (status == "A") return tr("Canceled");
    return status;
}

void BJobWidget::applyFilters()
{
    BJobsFilterModel *filterModel = m_tableView->filterModel();

    // Apply name filter from combo box
    filterModel->setNameFilter(m_nameFilter->currentText());

    // Apply client filter from combo box
    filterModel->setClientFilter(m_clientFilter->currentText());
    
    // Apply status filter - build set of EXCLUDED statuses from unchecked boxes
    // (jobs with status codes not covered by any checkbox always pass through)
    QSet<QString> excludedStatuses;
    if (!m_statusSuccess->isChecked()) excludedStatuses.insert("T");
    if (!m_statusWarning->isChecked()) excludedStatuses.insert("W");
    if (!m_statusFailed->isChecked()) { excludedStatuses.insert("F"); excludedStatuses.insert("f"); }
    if (!m_statusError->isChecked()) { excludedStatuses.insert("E"); excludedStatuses.insert("e"); }
    if (!m_statusRunning->isChecked()) excludedStatuses.insert("R");
    if (!m_statusCanceled->isChecked()) excludedStatuses.insert("A");

    filterModel->setStatusFilter(excludedStatuses);

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

    // Apply zero bytes filter
    filterModel->setZeroBytesFilter(m_statusZeroBytes->isChecked());

    // Update status label
    int visibleRows = filterModel->rowCount();
    int totalRows = m_tableView->jobsModel()->rowCount();

    if (visibleRows == 0 && totalRows > 0) {
        // All jobs filtered out - warn the user
        QString hint;
        if (m_statusZeroBytes->isChecked()) {
            hint = tr("⚠ No jobs visible — 'Zero Bytes' filter is active!");
        } else {
            hint = tr("⚠ No jobs visible — check your filter settings (%1 jobs loaded)")
                .arg(totalRows);
        }
        emit statusMessageChanged(hint);
    } else if (visibleRows < totalRows) {
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
    m_statusRunning->setChecked(true);
    m_statusCanceled->setChecked(true);
    m_statusZeroBytes->setChecked(false);

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

void BJobWidget::loadSelectedJobLog()
{

    QJsonObject selectedJob = m_tableView->getSelectedJob();
    if (selectedJob.isEmpty()) {
        return;
    }

    QString jobId = selectedJob["jobid"].toString();
    QString jobName = selectedJob["name"].toString();

    if (jobId.isEmpty()) {
        return;
    }

    // Update title label
    m_logTitleLabel->setText(tr("<b>Job Log</b> - Job: %1 (ID: %2) - Lädt...").arg(jobName).arg(jobId));

    // Clear current log
    m_logModel->clear();

    // Store the job ID and name we're requesting (for use in response handler)
    m_pendingLogJobId = jobId;
    m_pendingLogJobName = jobName;

    // Connect to Director signal if not already connected
    if (m_director) {
        connect(m_director, &BDirector::jsonResult,
                this, &BJobWidget::onJobLogReceived,
                Qt::UniqueConnection);

        // Request job log
        QMetaObject::invokeMethod(m_director, "doSend",
                                  Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::ListJobId),
                                  Q_ARG(QString, jobId));
    }
}

void BJobWidget::onJobLogReceived(BDirector::Command cmd, const QString &jsonData)
{
    // Enum-based routing: only process ListJobId responses
    if (cmd != BDirector::Command::ListJobId) return;

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        return;  // Not valid JSON
    }

    if (!doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Verify "joblog" key in result as a safety check
    if (!result.contains("joblog")) {
        return;  // Not a job log response
    }

    // Verify we have a pending request (trust command queue FIFO ordering
    // instead of re-verifying the current selection, which can change
    // between request and response due to model resets)
    if (m_pendingLogJobId.isEmpty()) {
        return;  // No pending request
    }

    QString jobId = m_pendingLogJobId;
    QString jobName = m_pendingLogJobName;

    // Clear pending request
    m_pendingLogJobId.clear();
    m_pendingLogJobName.clear();

    // Parse and display the log
    if (m_logModel->parseJsonResponse(jsonData)) {
        int lineCount = m_logModel->rowCount();
        m_logTitleLabel->setText(tr("<b>Job Log</b> - Job: %1 (ID: %2) - %3 Zeilen")
                                 .arg(jobName)
                                 .arg(jobId)
                                 .arg(lineCount));

        // Add to history (or update existing entry)
        bool found = false;
        for (int i = 0; i < m_logHistory.size(); ++i) {
            if (m_logHistory[i].jobId == jobId) {
                // Update existing entry
                m_logHistory[i].logLines = m_logModel->logLines();
                m_logHistoryCombo->setItemText(i, QString("%1 (ID: %2)").arg(jobName, jobId));
                m_logHistoryCombo->setCurrentIndex(i);
                found = true;
                break;
            }
        }

        if (!found) {
            // Add new entry
            JobLogEntry entry;
            entry.jobId = jobId;
            entry.jobName = jobName;
            entry.logLines = m_logModel->logLines();

            // Remove oldest if at max capacity
            while (m_logHistory.size() >= m_maxLogHistory) {
                m_logHistory.removeFirst();
                m_logHistoryCombo->removeItem(0);
            }

            m_logHistory.append(entry);
            m_logHistoryCombo->addItem(QString("%1 (ID: %2)").arg(jobName, jobId));
            m_logHistoryCombo->setCurrentIndex(m_logHistoryCombo->count() - 1);
        }

        // Clear search filter for new log
        m_logSearchEdit->clear();

    } else {
        m_logTitleLabel->setText(tr("<b>Job Log</b> - Job: %1 (ID: %2) - Load error")
                                 .arg(jobName)
                                 .arg(jobId));
    }
}

void BJobWidget::setLogViewVisible(bool visible)
{
    if (m_logContainer) {
        m_logContainer->setVisible(visible);
    }
}

void BJobWidget::clearData()
{
#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BJobWidget: Clearing all data";
#endif

    // Clear table model by setting empty array
    m_tableView->jobsModel()->setJobs(QJsonArray());

    // Clear combo boxes
    m_nameFilter->clear();
    m_clientFilter->clear();
    m_filesetCombo->clear();
    m_storageCombo->clear();
    m_poolCombo->clear();

    // Clear filter model
    m_filterComboModel->clear();

    // Clear resource models
    m_filesetModel->clear();
    m_storageModel->clear();
    m_poolModel->clear();
    m_levelModel->clear();

    // Clear dynamic level checkboxes
    for (auto it = m_levelCheckboxes.begin(); it != m_levelCheckboxes.end(); ++it) {
        it.value()->deleteLater();
    }
    m_levelCheckboxes.clear();

    // Clear log and history
    m_logModel->clear();
    m_logHistory.clear();
    m_logHistoryCombo->clear();
    m_logSearchEdit->clear();
    m_logTitleLabel->setText(tr("<b>Job Log</b> - Kein Job ausgewählt"));

    // Clear messages
    m_messagesWidget->clearMessages();

    // Clear stream reader
    m_streamReader->clear();
}

QStringList BJobWidget::jobNames() const
{
    return m_filterComboModel ? m_filterComboModel->jobNames() : QStringList();
}

QStringList BJobWidget::clientNames() const
{
    return m_filterComboModel ? m_filterComboModel->clientNames() : QStringList();
}

QStringList BJobWidget::catalogNames() const
{
    return m_catalogModel ? m_catalogModel->catalogNames() : QStringList();
}

QStringList BJobWidget::jobDefsNames() const
{
    return m_jobConfigModel ? m_jobConfigModel->jobDefsNames() : QStringList();
}

QJsonObject BJobWidget::jobConfiguration(const QString &jobName) const
{
    return m_jobConfigurations.value(jobName, QJsonObject());
}

// ============================================================================
// Job Log History Functions
// ============================================================================

void BJobWidget::onLogHistoryChanged(int index)
{
    if (index < 0 || index >= m_logHistory.size()) {
        return;
    }

    const JobLogEntry &entry = m_logHistory.at(index);

    // Update the log model with the selected history entry
    m_logModel->setLogLines(entry.logLines);

    // Update title
    m_logTitleLabel->setText(tr("<b>Job Log</b> - Job: %1 (ID: %2) - %3 Zeilen")
                             .arg(entry.jobName)
                             .arg(entry.jobId)
                             .arg(entry.logLines.size()));

    // Apply search filter if active
    if (!m_logSearchEdit->text().isEmpty()) {
        onLogSearchChanged(m_logSearchEdit->text());
    }

#ifdef IS_DEVELOPER
    JOBWIDGET_DEBUG << "Loaded job log from history: " << entry.jobName
                    << " (ID: " << entry.jobId << ")";
#endif
}

void BJobWidget::onLogCopyClicked()
{
    QModelIndexList selected = m_logView->selectionModel()->selectedIndexes();

    if (selected.isEmpty()) {
        // Copy all lines if nothing selected
        QStringList allLines = m_logModel->logLines();
        if (!allLines.isEmpty()) {
            QApplication::clipboard()->setText(allLines.join("\n"));
            emit statusMessageChanged(tr("Copied %1 log lines to clipboard").arg(allLines.size()));
        }
        return;
    }

    // Sort by row to maintain order
    std::sort(selected.begin(), selected.end(),
              [](const QModelIndex &a, const QModelIndex &b) { return a.row() < b.row(); });

    QStringList lines;
    for (const QModelIndex &index : selected) {
        lines.append(index.data(Qt::DisplayRole).toString());
    }

    QApplication::clipboard()->setText(lines.join("\n"));
    emit statusMessageChanged(tr("Copied %1 selected lines to clipboard").arg(lines.size()));

#ifdef IS_DEVELOPER
    JOBWIDGET_DEBUG << "Copied " << lines.size() << " lines to clipboard";
#endif
}

void BJobWidget::onLogClearHistoryClicked()
{
    m_logHistory.clear();
    m_logHistoryCombo->clear();
    m_logModel->clear();
    m_logSearchEdit->clear();
    m_logTitleLabel->setText(tr("<b>Job Log</b> - Kein Job ausgewählt"));

    emit statusMessageChanged(tr("Job log history cleared"));

#ifdef IS_DEVELOPER
    JOBWIDGET_DEBUG << "Cleared job log history";
#endif
}

void BJobWidget::onLogSearchChanged(const QString &text)
{
    if (text.isEmpty()) {
        // Restore full log from current history selection
        int index = m_logHistoryCombo->currentIndex();
        if (index >= 0 && index < m_logHistory.size()) {
            m_logModel->setLogLines(m_logHistory.at(index).logLines);
        }
        return;
    }

    // Filter log lines by search text
    int index = m_logHistoryCombo->currentIndex();
    if (index < 0 || index >= m_logHistory.size()) {
        return;
    }

    QStringList filteredLines;
    for (const QString &line : m_logHistory.at(index).logLines) {
        if (line.contains(text, Qt::CaseInsensitive)) {
            filteredLines.append(line);
        }
    }

    m_logModel->setLogLines(filteredLines);

#ifdef IS_DEVELOPER
    JOBWIDGET_DEBUG << "Search filter: " << filteredLines.size()
                    << " of " << m_logHistory.at(index).logLines.size() << " lines match";
#endif
}
