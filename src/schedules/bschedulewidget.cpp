#include "schedules/bschedulewidget.h"
#include "schedules/bscheduledragresultdialog.h"
#include "bcapabilitynotice.h"
#include "jobs/blevelcolors.h"
#include "bsorttableitem.h"
#include "bsortvalue.h"
#include "config/bsettings.h"
#include "blogging.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include <QHeaderView>
#include <QScrollBar>
#include <QShowEvent>
#include <QStyle>

BScheduleWidget::BScheduleWidget(QWidget *parent)
    : QWidget(parent)
    , m_scheduleModel(new BScheduleModel(this))
    , m_splitter(new QSplitter(Qt::Horizontal, this))
    , m_leftSplitter(new QSplitter(Qt::Vertical, this))
    , m_scheduleList(new QListWidget(this))
    , m_jobListLabel(new QLabel(tr("Jobs:"), this))
    , m_jobList(new QTableWidget(this))
    , m_viewStack(new QStackedWidget(this))
    , m_ganttSplitter(new QSplitter(Qt::Vertical, this))
    , m_fdGanttWidget(new BScheduleGanttWidget(this))
    , m_fdScrollArea(new QScrollArea(this))
    , m_sdGanttWidget(new BScheduleGanttWidget(this))
    , m_sdScrollArea(new QScrollArea(this))
    , m_gridView(new BScheduleGridView(this))
    , m_refreshButton(new QPushButton(tr("Refresh"), this))
    , m_viewModeCombo(new QComboBox(this))
    , m_dayViewCombo(new QComboBox(this))
    , m_prevDayButton(new QToolButton(this))
    , m_nextDayButton(new QToolButton(this))
    , m_todayButton(new QToolButton(this))
    , m_dayLabel(new QLabel(this))
    , m_snapCombo(new QComboBox(this))
    , m_collisionLabel(new QLabel(this))
    , m_runProblemLabel(new QLabel(this))
    , m_jobScheduleIndex(new BJobScheduleIndex(this))
    , m_director(nullptr)
    , m_currentDate(QDate::currentDate())
{
    // The job history is asked for a moment after the view is on screen,
    // not in the burst of commands of a connect or a refresh
    m_historyTimer = new QTimer(this);
    m_historyTimer->setSingleShot(true);
    m_historyTimer->setInterval(1500);
    connect(m_historyTimer, &QTimer::timeout, this, &BScheduleWidget::requestHistoryIfDue);

    setupUI();

    // Lock group modes: FD always by client, SD always by storage
    m_fdGanttWidget->setGroupMode(BScheduleGanttWidget::GroupByClient);
    m_sdGanttWidget->setGroupMode(BScheduleGanttWidget::GroupByStorage);

    updateHistoryViews();
}

BScheduleWidget::~BScheduleWidget()
{
    // The reader thread posts its result to this object
    if (m_historyParser) {
        m_historyParser->wait();
    }

    BSettings &s = BSettings::instance();
    s.setSchedulesSplitterState(m_splitter->saveState());
    s.setSchedulesGanttSplitterState(m_ganttSplitter->saveState());
}

void BScheduleWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // === Toolbar ===
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    // View mode: Gantt / Grid
    m_viewModeCombo->addItem(tr("Timeline"), 0);
    m_viewModeCombo->addItem(tr("Grid"), 1);
    m_viewModeCombo->addItem(tr("Measured waits"), 2);
    toolbarLayout->addWidget(new QLabel(tr("View:"), this));
    toolbarLayout->addWidget(m_viewModeCombo);

    toolbarLayout->addSpacing(10);

    // Day/Week toggle
    m_dayViewCombo->addItem(tr("Day"), 0);
    m_dayViewCombo->addItem(tr("Week"), 1);
    toolbarLayout->addWidget(m_dayViewCombo);

    // Day navigation
    QString navBtnStyle =
        "QToolButton { color: #333; font-weight: bold; border: 1px solid #aaa;"
        " border-radius: 3px; background: #ddd; padding: 2px 6px; }"
        "QToolButton:hover { background: #bbb; }"
        "QToolButton:disabled { color: #aaa; background: #eee; border-color: #ccc; }";
    m_prevDayButton->setText(QStringLiteral("\u25C0"));  // ◀
    m_prevDayButton->setStyleSheet(navBtnStyle);
    m_nextDayButton->setText(QStringLiteral("\u25B6"));  // ▶
    m_nextDayButton->setStyleSheet(navBtnStyle);

    m_prevDayButton->setToolTip(tr("Previous day or week"));
    m_nextDayButton->setToolTip(tr("Next day or week"));
    m_todayButton->setText(tr("Today"));
    m_todayButton->setStyleSheet(navBtnStyle);
    m_todayButton->setToolTip(tr("Show today or the current week"));

    // Wide enough for a week: "2026-11-16 - 2026-11-22"
    m_dayLabel->setMinimumWidth(170);
    m_dayLabel->setAlignment(Qt::AlignCenter);
    m_dayLabel->setToolTip(tr("The date or the week (Monday to Sunday) on display"));

    toolbarLayout->addWidget(m_prevDayButton);
    toolbarLayout->addWidget(m_dayLabel);
    toolbarLayout->addWidget(m_nextDayButton);
    toolbarLayout->addWidget(m_todayButton);

    toolbarLayout->addSpacing(10);

    // Snap granularity (items populated by updateSnapComboForViewMode)
    m_snapLabel = new QLabel(tr("Snap:"), this);
    toolbarLayout->addWidget(m_snapLabel);
    toolbarLayout->addWidget(m_snapCombo);

    toolbarLayout->addSpacing(10);

    // Zoom slider (visible in week mode)
    m_zoomLabel = new QLabel(tr("Zoom:"), this);
    m_zoomSlider = new QSlider(Qt::Horizontal, this);
    m_zoomSlider->setRange(10, 60);    // px per hour: 10 (compact) to 60 (detailed)
    m_zoomSlider->setValue(15);
    m_zoomSlider->setFixedWidth(120);
    m_zoomSlider->setToolTip(tr("Zoom level (pixels per hour)"));
    m_zoomLabel->setVisible(false);
    m_zoomSlider->setVisible(false);
    toolbarLayout->addWidget(m_zoomLabel);
    toolbarLayout->addWidget(m_zoomSlider);

    toolbarLayout->addStretch();

    // Collision indicator
    m_collisionLabel->setVisible(false);
    m_collisionLabel->setStyleSheet("color: red; font-weight: bold;");
    toolbarLayout->addWidget(m_collisionLabel);

    // Run lines that were not understood: named here, shown nowhere else
    m_runProblemLabel->setVisible(false);
    m_runProblemLabel->setStyleSheet("color: red; font-weight: bold;");
    toolbarLayout->addWidget(m_runProblemLabel);

    // Refresh
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);
    toolbarLayout->addWidget(m_refreshButton);

    mainLayout->addLayout(toolbarLayout);

    // === Splitter ===
    // Left side: Schedules (top) + Jobs (bottom) in vertical splitter

    // --- Schedule pane ---
    QWidget *schedulePane = new QWidget(this);
    QVBoxLayout *scheduleLayout = new QVBoxLayout(schedulePane);
    scheduleLayout->setContentsMargins(0, 0, 0, 0);
    scheduleLayout->setSpacing(2);
    QLabel *listLabel = new QLabel(tr("Schedules:"), this);
    listLabel->setStyleSheet("font-weight: bold; font-size: 12pt;");
    listLabel->setAlignment(Qt::AlignRight);
    scheduleLayout->addWidget(listLabel);
    m_scheduleList->setAlternatingRowColors(true);
    scheduleLayout->addWidget(m_scheduleList);

    // --- Job pane ---
    QWidget *jobPane = new QWidget(this);
    QVBoxLayout *jobLayout = new QVBoxLayout(jobPane);
    jobLayout->setContentsMargins(0, 0, 0, 0);
    jobLayout->setSpacing(2);
    m_jobListLabel->setStyleSheet("font-weight: bold; font-size: 10pt;");
    jobLayout->addWidget(m_jobListLabel);

    m_jobList->setColumnCount(6);
    m_jobList->setHorizontalHeaderLabels({tr("Job"), tr("Type"), tr("Client"), tr("Storage"), tr("FileSet"), tr("Median duration")});
    m_jobList->setAlternatingRowColors(true);
    m_jobList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_jobList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_jobList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_jobList->verticalHeader()->setVisible(false);
    m_jobList->horizontalHeader()->setStretchLastSection(true);
    m_jobList->setDragEnabled(true);
    jobLayout->addWidget(m_jobList);

    m_leftSplitter->addWidget(schedulePane);
    m_leftSplitter->addWidget(jobPane);
    m_leftSplitter->setSizes(QList<int>() << 200 << 200);

    // Right side: Stacked widget (Gantt / Grid)
    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    // --- FD pane (top) ---
    QWidget *fdPane = new QWidget(this);
    QVBoxLayout *fdLayout = new QVBoxLayout(fdPane);
    fdLayout->setContentsMargins(0, 0, 0, 0);
    fdLayout->setSpacing(2);
    QLabel *fdLabel = new QLabel(tr("Clients (FD)"), fdPane);
    fdLabel->setStyleSheet("font-weight: bold; font-size: 10pt; padding: 2px 4px;");
    fdLayout->addWidget(fdLabel);
    m_fdScrollArea->setWidget(m_fdGanttWidget);
    m_fdScrollArea->setWidgetResizable(true);
    m_fdScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_fdScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    fdLayout->addWidget(m_fdScrollArea);

    // --- SD pane (bottom) ---
    QWidget *sdPane = new QWidget(this);
    QVBoxLayout *sdLayout = new QVBoxLayout(sdPane);
    sdLayout->setContentsMargins(0, 0, 0, 0);
    sdLayout->setSpacing(2);
    QLabel *sdLabel = new QLabel(tr("Storage (SD)"), sdPane);
    sdLabel->setStyleSheet("font-weight: bold; font-size: 10pt; padding: 2px 4px;");
    sdLayout->addWidget(sdLabel);
    m_sdScrollArea->setWidget(m_sdGanttWidget);
    m_sdScrollArea->setWidgetResizable(true);
    m_sdScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_sdScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    sdLayout->addWidget(m_sdScrollArea);

    // Vertical splitter holding both
    m_ganttSplitter->addWidget(fdPane);
    m_ganttSplitter->addWidget(sdPane);
    m_ganttSplitter->setSizes(QList<int>() << 300 << 200);

    m_viewStack->addWidget(m_ganttSplitter);   // index 0 = Gantt (dual)

    // The grid: one schedule resource, the current row of the schedule list.
    // Its level colours are those of the settings, as everywhere else.
    m_gridView->setLevelColorSource(&BLevelColors::getLevelColor);
    connect(&BSettings::instance(), &BSettings::appearanceSettingsChanged, m_gridView, [this]() {
        m_gridView->setLevelColorSource(&BLevelColors::getLevelColor);
    });
    m_viewStack->addWidget(m_gridView);          // index 1 = Grid

    setupWaitsPage();
    m_viewStack->addWidget(m_waitsPage);         // index 2 = Measured waits

    rightLayout->addWidget(m_viewStack, 1);

    // Stats panel (below Gantt)
    setupStatsPanel();
    rightLayout->addWidget(m_statsPanel);

    m_splitter->addWidget(m_leftSplitter);
    m_splitter->addWidget(rightWidget);
    m_splitter->setSizes(QList<int>() << 200 << 800);

    mainLayout->addWidget(m_splitter);

    // === Connections ===
    connect(m_refreshButton, &QPushButton::clicked,
            this, &BScheduleWidget::onRefreshClicked);
    connect(m_scheduleList, &QListWidget::itemSelectionChanged,
            this, &BScheduleWidget::onScheduleSelectionChanged);
    connect(m_scheduleList, &QListWidget::currentItemChanged,
            this, [this]() { updateGridView(); });
    connect(m_scheduleList, &QListWidget::itemDoubleClicked,
            this, &BScheduleWidget::onScheduleDoubleClicked);
    connect(m_scheduleList, &QListWidget::itemChanged,
            this, &BScheduleWidget::onScheduleCheckChanged);
    connect(m_jobList, &QTableWidget::itemSelectionChanged,
            this, &BScheduleWidget::onJobSelectionChanged);
    connect(m_jobList, &QTableWidget::cellDoubleClicked,
            this, &BScheduleWidget::onJobListDoubleClicked);

    connect(m_viewModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onViewModeChanged);
    connect(m_dayViewCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onDayViewModeChanged);
    connect(m_prevDayButton, &QToolButton::clicked,
            this, &BScheduleWidget::onDayNavigationPrev);
    connect(m_nextDayButton, &QToolButton::clicked,
            this, &BScheduleWidget::onDayNavigationNext);
    connect(m_todayButton, &QToolButton::clicked,
            this, &BScheduleWidget::onDayNavigationToday);
    connect(m_snapCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onSnapChanged);
    connect(m_zoomSlider, &QSlider::valueChanged,
            this, &BScheduleWidget::onZoomSliderChanged);

    // FD Gantt widget signals
    connect(m_fdGanttWidget, &BScheduleGanttWidget::collisionsDetected,
            this, &BScheduleWidget::onCollisionsDetected);
    connect(m_fdGanttWidget, &BScheduleGanttWidget::entryClicked,
            this, &BScheduleWidget::onEntryClicked);
    connect(m_fdGanttWidget, &BScheduleGanttWidget::dragCompleted,
            this, &BScheduleWidget::onFdDragCompleted);

    // SD Gantt widget signals
    connect(m_sdGanttWidget, &BScheduleGanttWidget::collisionsDetected,
            this, &BScheduleWidget::onCollisionsDetected);
    connect(m_sdGanttWidget, &BScheduleGanttWidget::entryClicked,
            this, &BScheduleWidget::onEntryClicked);
    connect(m_sdGanttWidget, &BScheduleGanttWidget::dragCompleted,
            this, &BScheduleWidget::onSdDragCompleted);

    // Horizontal scroll sync between FD and SD
    connect(m_fdScrollArea->horizontalScrollBar(), &QScrollBar::valueChanged,
            this, [this](int value) {
        if (m_sdScrollArea->horizontalScrollBar()->value() != value)
            m_sdScrollArea->horizontalScrollBar()->setValue(value);
    });
    connect(m_sdScrollArea->horizontalScrollBar(), &QScrollBar::valueChanged,
            this, [this](int value) {
        if (m_fdScrollArea->horizontalScrollBar()->value() != value)
            m_fdScrollArea->horizontalScrollBar()->setValue(value);
    });

    // === Restore settings ===
    BSettings &s = BSettings::instance();

    int viewMode = s.schedulesViewMode();
    m_viewModeCombo->setCurrentIndex(viewMode);

    int dayViewMode = s.schedulesDayViewMode();
    m_dayViewCombo->setCurrentIndex(dayViewMode);

    // The date on display is not restored: a session starts with today

    // Populate snap combo for the restored view mode, then restore index
    BScheduleGanttWidget::ViewMode ganttViewMode = (dayViewMode == 0)
        ? BScheduleGanttWidget::DayView : BScheduleGanttWidget::WeekView;
    updateSnapComboForViewMode(ganttViewMode);
    int snapIndex = s.schedulesSnapIndex();
    if (snapIndex < m_snapCombo->count())
        m_snapCombo->setCurrentIndex(snapIndex);
    int snapMinutes = m_snapCombo->currentData().toInt();
    m_fdGanttWidget->setSnapMinutes(snapMinutes);
    m_sdGanttWidget->setSnapMinutes(snapMinutes);

    QByteArray splitterState = s.schedulesSplitterState();
    if (!splitterState.isEmpty()) {
        m_splitter->restoreState(splitterState);
    }

    QByteArray ganttSplitterState = s.schedulesGanttSplitterState();
    if (!ganttSplitterState.isEmpty()) {
        m_ganttSplitter->restoreState(ganttSplitterState);
    }

    m_fdGanttWidget->setCurrentDate(m_currentDate);
    m_sdGanttWidget->setCurrentDate(m_currentDate);
    updateDateLabel();
}

void BScheduleWidget::setConnectionState(bool connected)
{
    m_connected = connected;
    m_refreshButton->setEnabled(connected);

    if (!connected) {
        clearData();
    } else {
        // Nothing is asked here: the history follows when the view is shown
        updateHistoryInfo();
        scheduleHistoryRequest();
    }
}

void BScheduleWidget::clearData()
{
    m_scheduleList->clear();
    m_gridScheduleName.clear();
    m_gridView->clear();
    m_fdGanttWidget->setEntries({});
    m_sdGanttWidget->setEntries({});
    m_scheduleModel->clear();
    resetHistory();     // the history belongs to the connection
    m_collisionLabel->setVisible(false);
    m_runProblemLabel->setVisible(false);
}

void BScheduleWidget::processDotScheduleResponse(const QString &jsonData)
{
#ifdef DEBUG_JSON
    BLOG_DEBUG() << "========================================";
    BLOG_DEBUG() << "BScheduleWidget: Processing .schedule response";
    BLOG_DEBUG() << "  Data size:" << jsonData.size() << "bytes";
    if (jsonData.size() < 2000) {
        BLOG_DEBUG() << "  Raw JSON:" << jsonData;
    } else {
        BLOG_DEBUG() << "  First 2000 chars:" << jsonData.left(2000);
    }
    BLOG_DEBUG() << "========================================";
#elif defined(IS_DEVELOPER)
    BLOG_DEBUG() << "BScheduleWidget: Processing .schedule response";
#endif

    // Feed the model
    m_scheduleModel->parseSchedules(jsonData);

    // Also parse for the list widget (backwards compat)
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "BScheduleWidget: Failed to parse .schedule response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        BLOG_WARNING() << "BScheduleWidget: .schedule response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray schedulesArray = result["schedules"].toArray();

    // Block signals while rebuilding the list to avoid premature filter updates
    m_scheduleList->blockSignals(true);
    m_scheduleList->clear();

    for (const QJsonValue &scheduleVal : schedulesArray) {
        if (!scheduleVal.isObject()) continue;

        QJsonObject schedule = scheduleVal.toObject();
        QString scheduleName = schedule["name"].toString();

        if (!scheduleName.isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(scheduleName);
            item->setData(Qt::UserRole, schedule);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            bool checked = BSettings::instance().schedulesFilterCheckbox(scheduleName, true);
            item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
            m_scheduleList->addItem(item);
        }
    }
    m_scheduleList->blockSignals(false);

    m_refreshButton->setEnabled(m_director != nullptr);
    emit statusMessageChanged(tr("%1 schedules loaded").arg(schedulesArray.size()));

    // Update both views with all schedules (all checked initially)
    updateRunProblems();
    updateGridView(true);   // names; the Run lines follow with "show schedules"
    updateGanttEntries();
    updateJobList();
}

void BScheduleWidget::processShowSchedulesResponse(const QString &jsonData)
{
#ifdef DEBUG_JSON
    BLOG_DEBUG() << "========================================";
    BLOG_DEBUG() << "BScheduleWidget: Processing show schedules response";
    BLOG_DEBUG() << "  Data size:" << jsonData.size() << "bytes";
    if (jsonData.size() < 2000) {
        BLOG_DEBUG() << "  Raw JSON:" << jsonData;
    } else {
        BLOG_DEBUG() << "  First 2000 chars:" << jsonData.left(2000);
    }
    BLOG_DEBUG() << "========================================";
#elif defined(IS_DEVELOPER)
    BLOG_DEBUG() << "BScheduleWidget: Processing show schedules response";
#endif

    // "show schedules" returns full config with Run directives.
    // The response has "schedules" as an object (keyed by name), not an array.
    // Convert to the array format that parseSchedules expects.
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        BLOG_WARNING() << "BScheduleWidget: Failed to parse show schedules response";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonValue schedulesVal = result["schedules"];

    QJsonArray schedulesArray;
    if (schedulesVal.isObject()) {
        // "show schedules" format: { "name1": {...}, "name2": {...} }
        QJsonObject schedulesObj = schedulesVal.toObject();
        for (auto it = schedulesObj.begin(); it != schedulesObj.end(); ++it) {
            if (it.value().isObject()) {
                schedulesArray.append(it.value());
            }
        }
    } else if (schedulesVal.isArray()) {
        schedulesArray = schedulesVal.toArray();
    }

    if (schedulesArray.isEmpty()) {
#ifdef IS_DEVELOPER
        BLOG_DEBUG() << "BScheduleWidget: No schedules in show schedules response";
#endif
        return;
    }

    // Re-wrap as the array format that parseSchedules expects
    QJsonObject wrappedResult;
    wrappedResult["schedules"] = schedulesArray;
    QJsonObject wrappedRoot;
    wrappedRoot["result"] = wrappedResult;
    QJsonDocument wrappedDoc(wrappedRoot);

    m_scheduleModel->parseSchedules(QString::fromUtf8(wrappedDoc.toJson()));

    // Rebuild list with checkbox state
    m_scheduleList->blockSignals(true);
    m_scheduleList->clear();

    for (const QJsonValue &scheduleVal : schedulesArray) {
        if (!scheduleVal.isObject()) continue;
        QJsonObject schedule = scheduleVal.toObject();
        QString scheduleName = schedule["name"].toString();
        if (!scheduleName.isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(scheduleName);
            item->setData(Qt::UserRole, schedule);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            bool checked = BSettings::instance().schedulesFilterCheckbox(scheduleName, true);
            item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
            m_scheduleList->addItem(item);
        }
    }
    m_scheduleList->blockSignals(false);

    m_refreshButton->setEnabled(m_director != nullptr);

    int runLineCount = m_scheduleModel->runLineCount();
    emit statusMessageChanged(tr("%1 schedules loaded (%2 Run lines)")
        .arg(m_scheduleList->count()).arg(runLineCount));

    updateRunProblems();
    updateGridView();
    updateGanttEntries();
    updateJobList();
}

void BScheduleWidget::processShowJobsResponse(const QString &)
{
    // Rebuild job→schedule index when job configs are updated
    if (m_jobConfigModel) {
        m_jobScheduleIndex->rebuild(m_jobConfigModel);
        updateGanttEntries();
        updateJobList();
        updateGridView();   // how many jobs use the schedule on display
    }
}

void BScheduleWidget::setJobConfigModel(BJobConfigModel *model)
{
    m_jobConfigModel = model;
    m_fdGanttWidget->setJobConfigModel(model);
    m_sdGanttWidget->setJobConfigModel(model);
    if (model && model->hasConfigs()) {
        m_jobScheduleIndex->rebuild(model);
    }
}

void BScheduleWidget::onRefreshClicked()
{
    m_refreshButton->setEnabled(false);
    emit statusMessageChanged(tr("Refreshing schedule list..."));
    emit sendCommand(BDirector::Command::DotSchedule, "");
    emit sendCommand(BDirector::Command::ShowSchedules, "");
}

void BScheduleWidget::onScheduleSelectionChanged()
{
    updateJobList();
}

void BScheduleWidget::updateJobList()
{
    m_jobList->setRowCount(0);

    // Collect all checked schedule names (same filter as Gantt)
    QStringList checkedNames;
    for (int i = 0; i < m_scheduleList->count(); ++i) {
        QListWidgetItem *item = m_scheduleList->item(i);
        if (item->checkState() == Qt::Checked)
            checkedNames.append(item->text());
    }

    if (checkedNames.isEmpty()) {
        m_jobListLabel->setText(tr("Jobs:"));
        return;
    }

    if (checkedNames.size() == 1)
        m_jobListLabel->setText(tr("Jobs (%1):").arg(checkedNames.first()));
    else
        m_jobListLabel->setText(tr("Jobs (%1 Schedules):").arg(checkedNames.size()));

    if (!m_jobScheduleIndex->isValid()) return;

    // Aggregate and deduplicate jobs across all checked schedules
    QMap<QString, BJobScheduleIndex::JobRef> jobMap;
    for (const QString &scheduleName : checkedNames) {
        for (const BJobScheduleIndex::JobRef &job : m_jobScheduleIndex->jobsForSchedule(scheduleName)) {
            if (!jobMap.contains(job.jobName))
                jobMap.insert(job.jobName, job);
        }
    }

    const QList<BJobScheduleIndex::JobRef> allJobs = jobMap.values();
    m_jobList->setRowCount(allJobs.size());

    for (int i = 0; i < allJobs.size(); ++i) {
        const BJobScheduleIndex::JobRef &job = allJobs[i];

        m_jobList->setItem(i, 0, new QTableWidgetItem(job.jobName));

        QString jobType = tr("-");
        if (m_jobConfigModel) {
            const auto &raw = m_jobConfigModel->jobConfigsRaw();
            if (raw.contains(job.jobName))
                jobType = raw[job.jobName]["type"].toString(tr("-"));
        }
        m_jobList->setItem(i, 1, new QTableWidgetItem(jobType));

        m_jobList->setItem(i, 2, new QTableWidgetItem(job.client));
        m_jobList->setItem(i, 3, new QTableWidgetItem(job.storage));
        m_jobList->setItem(i, 4, new QTableWidgetItem(job.fileset));

        // Measured median over all levels of the job; "-" without history
        const BJobHistoryStats::Estimate estimate = m_historyStats.estimate(job.jobName, QString());
        QTableWidgetItem *durationItem = new QTableWidgetItem(
            estimate.hasHistory() ? BJobHistoryStats::formatDuration(estimate.duration.median)
                                  : tr("-"));
        durationItem->setToolTip(estimate.hasHistory()
            ? tr("Median of %1 measured runs at all levels in the last %2 days")
                  .arg(estimate.runs).arg(m_historyStats.windowDays())
            : tr("No history"));
        m_jobList->setItem(i, 5, durationItem);
    }

    m_jobList->resizeColumnsToContents();
}

void BScheduleWidget::onScheduleDoubleClicked(QListWidgetItem *item)
{
    if (!item) return;

    QJsonObject schedule = item->data(Qt::UserRole).toJsonObject();

#ifdef IS_DEVELOPER
    BLOG_DEBUG() << "BScheduleWidget: Double-clicked schedule:" << schedule["name"].toString();
#endif

    // TODO: Open schedule details dialog or wizard
}

void BScheduleWidget::onViewModeChanged(int index)
{
    m_viewStack->setCurrentIndex(index);

    // The controls of the timeline are hidden where they have no effect.
    // The grid is not about a date (its calendar has its own navigation)
    // and the table of measured waits is not either.
    const bool isGantt = (index == 0);
    const bool isGrid = (index == 1);
    const bool isWaits = (index == 2);
    m_dayViewCombo->setVisible(isGantt);
    m_snapLabel->setVisible(isGantt);
    m_snapCombo->setVisible(isGantt);
    updateDateLabel();
    bool isWeekGantt = isGantt && (m_dayViewCombo->currentIndex() == 1);
    m_zoomLabel->setVisible(isWeekGantt);
    m_zoomSlider->setVisible(isWeekGantt);
    m_prevDayButton->setVisible(isGantt);
    m_dayLabel->setVisible(isGantt);
    m_nextDayButton->setVisible(isGantt);
    m_todayButton->setVisible(isGantt);

    // The figures of a run clicked in the timeline would take the room of
    // the grid
    if (isGrid)
        m_statsPanel->setVisible(false);

    if (isWaits) {
        // Not stored: the setting keeps meaning "timeline or grid", and the
        // table needs no schedules from the server
        scheduleHistoryRequest();
        return;
    }

    BSettings::instance().setSchedulesViewMode(index);

    // Refresh from server so the new view has data
    onRefreshClicked();
}

void BScheduleWidget::onDayViewModeChanged(int index)
{
    BScheduleGanttWidget::ViewMode mode = (index == 0)
        ? BScheduleGanttWidget::DayView
        : BScheduleGanttWidget::WeekView;

    m_fdGanttWidget->setViewMode(mode);
    m_sdGanttWidget->setViewMode(mode);

    // Apply current zoom value immediately when entering week mode so that
    // m_zoomMinPph is set correctly and scrollbars appear right away.
    if (mode == BScheduleGanttWidget::WeekView) {
        m_fdGanttWidget->setZoomLevel(m_zoomSlider->value());
        m_sdGanttWidget->setZoomLevel(m_zoomSlider->value());
    }

    // Day and week show different dates: the label and the entries follow
    updateDateLabel();
    updateGanttEntries();

    // Show zoom slider only in week mode
    bool isWeek = (mode == BScheduleGanttWidget::WeekView);
    m_zoomLabel->setVisible(isWeek);
    m_zoomSlider->setVisible(isWeek);

    updateSnapComboForViewMode(mode);
    resizeBothGanttWidgets();

    BSettings::instance().setSchedulesDayViewMode(index);
}

bool BScheduleWidget::showsWeek() const
{
    return m_dayViewCombo->currentIndex() == 1;
}

void BScheduleWidget::onDayNavigationPrev()
{
    setCurrentDate(m_currentDate.addDays(showsWeek() ? -7 : -1));
}

void BScheduleWidget::onDayNavigationNext()
{
    setCurrentDate(m_currentDate.addDays(showsWeek() ? 7 : 1));
}

void BScheduleWidget::onDayNavigationToday()
{
    setCurrentDate(QDate::currentDate());
}

void BScheduleWidget::setCurrentDate(const QDate &date)
{
    if (!date.isValid() || date == m_currentDate) return;

    m_currentDate = date;
    m_fdGanttWidget->setCurrentDate(date);
    m_sdGanttWidget->setCurrentDate(date);
    updateDateLabel();

    // Other dates, other runs
    updateGanttEntries();
}

void BScheduleWidget::updateDateLabel()
{
    static const QStringList dayAbbrev = {
        tr("Mon"), tr("Tue"), tr("Wed"), tr("Thu"),
        tr("Fri"), tr("Sat"), tr("Sun")
    };

    if (showsWeek()) {
        const QDate monday = BScheduleGanttWidget::firstDateFor(
            BScheduleGanttWidget::WeekView, m_currentDate);
        m_dayLabel->setText(QString("%1 \u2013 %2")
            .arg(monday.toString(Qt::ISODate), monday.addDays(6).toString(Qt::ISODate)));
    } else {
        m_dayLabel->setText(QString("%1 %2")
            .arg(dayAbbrev.value(m_currentDate.dayOfWeek() - 1),  // Qt: 1=Mon
                 m_currentDate.toString(Qt::ISODate)));
    }
}

void BScheduleWidget::onCollisionsDetected(int certain, int uncertain)
{
    if (certain <= 0 && uncertain <= 0) {
        m_collisionLabel->setVisible(false);
        return;
    }

    QStringList parts;
    if (certain > 0) {
        parts << tr("%1 collision(s)").arg(certain);
    }
    if (uncertain > 0) {
        parts << tr("%1 uncertain").arg(uncertain);
    }
    m_collisionLabel->setText(parts.join(", "));
    m_collisionLabel->setToolTip(uncertain > 0
        ? tr("Uncertain: whether the two runs overlap depends on a duration that was "
             "not measured (no history, or an estimate from other levels).")
        : QString());
    // Red is for what was computed from measured durations
    m_collisionLabel->setStyleSheet(certain > 0 ? "color: red; font-weight: bold;"
                                                : "color: #b26a00; font-weight: bold;");
    m_collisionLabel->setVisible(true);
}

void BScheduleWidget::setupStatsPanel()
{
    m_statsPanel = new QGroupBox(tr("Measured history"), this);
    m_statsPanel->setVisible(false);

    QVBoxLayout *statsLayout = new QVBoxLayout(m_statsPanel);
    statsLayout->setContentsMargins(8, 8, 8, 8);
    statsLayout->setSpacing(4);

    // Title row with close button
    QHBoxLayout *titleRow = new QHBoxLayout();
    m_statsTitle = new QLabel(this);
    m_statsTitle->setStyleSheet("font-weight: bold; font-size: 11pt;");
    titleRow->addWidget(m_statsTitle, 1);

    QToolButton *closeButton = new QToolButton(this);
    closeButton->setText(QStringLiteral("\u2715"));  // Unicode ✕
    closeButton->setToolTip(tr("Close"));
    closeButton->setFixedSize(20, 20);
    closeButton->setStyleSheet(
        "QToolButton { color: #333; font-weight: bold; border: 1px solid #aaa;"
        " border-radius: 3px; background: #ddd; }"
        "QToolButton:hover { background: #c44; color: white; }");
    connect(closeButton, &QToolButton::clicked, m_statsPanel, [this]() {
        m_statsPanel->setVisible(false);
    });
    titleRow->addWidget(closeButton);
    statsLayout->addLayout(titleRow);

    // --- Job config info row ---
    QHBoxLayout *configRow = new QHBoxLayout();
    configRow->setSpacing(20);

    auto addConfigBox = [&](const QString &label) -> QLabel* {
        QVBoxLayout *box = new QVBoxLayout();
        box->setSpacing(0);
        QLabel *hdr = new QLabel(label, this);
        hdr->setStyleSheet("color: gray; font-size: 9pt;");
        hdr->setAlignment(Qt::AlignLeft);
        QLabel *val = new QLabel("-", this);
        val->setWordWrap(false);
        val->setTextInteractionFlags(Qt::TextSelectableByMouse);
        box->addWidget(hdr);
        box->addWidget(val);
        configRow->addLayout(box);
        return val;
    };

    m_statsJobType  = addConfigBox(tr("Type"));
    m_statsClient   = addConfigBox(tr("Client"));
    m_statsFileSet  = addConfigBox(tr("FileSet"));
    m_statsStorage  = addConfigBox(tr("Storage"));
    m_statsPriority = addConfigBox(tr("Priority"));
    configRow->addStretch();
    statsLayout->addLayout(configRow);

    // Separator
    QFrame *hSep = new QFrame(this);
    hSep->setFrameShape(QFrame::HLine);
    hSep->setFrameShadow(QFrame::Sunken);
    statsLayout->addWidget(hSep);

    // Whether the figures below were measured at this level
    m_statsBasis = new QLabel(this);
    m_statsBasis->setWordWrap(true);
    m_statsBasis->setStyleSheet("font-style: italic;");
    statsLayout->addWidget(m_statsBasis);

    // Row: duration (median | 25-75% | 90%) | runs | wait (median | 90%)
    QHBoxLayout *statsRow = new QHBoxLayout();
    statsRow->setSpacing(16);

    auto addStatBox = [&](const QString &label) -> QLabel* {
        QVBoxLayout *box = new QVBoxLayout();
        box->setSpacing(0);
        QLabel *headerLabel = new QLabel(label, this);
        headerLabel->setStyleSheet("color: gray; font-size: 9pt;");
        headerLabel->setAlignment(Qt::AlignCenter);
        QLabel *valueLabel = new QLabel("-", this);
        valueLabel->setStyleSheet("font-size: 12pt; font-weight: bold;");
        valueLabel->setAlignment(Qt::AlignCenter);
        box->addWidget(headerLabel);
        box->addWidget(valueLabel);
        statsRow->addLayout(box);
        return valueLabel;
    };

    m_statsMedian = addStatBox(tr("Median duration"));
    m_statsSpread = addStatBox(tr("25% - 75%"));
    m_statsP90 = addStatBox(tr("90%"));

    // Separator
    QFrame *sep = new QFrame(this);
    sep->setFrameShape(QFrame::VLine);
    statsRow->addWidget(sep);

    m_statsSamples = addStatBox(tr("Runs"));

    QFrame *sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::VLine);
    statsRow->addWidget(sep2);

    m_statsWaitMedian = addStatBox(tr("Median wait"));
    m_statsWaitP90 = addStatBox(tr("90% wait"));

    statsRow->addStretch();
    statsLayout->addLayout(statsRow);

    // Last runs table
    m_statsRunsTable = new QTableWidget(0, 4, this);
    m_statsRunsTable->setHorizontalHeaderLabels({tr("Start"), tr("Level"), tr("Duration"), tr("Wait")});
    m_statsRunsTable->horizontalHeader()->setStretchLastSection(true);
    m_statsRunsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_statsRunsTable->verticalHeader()->setVisible(false);
    m_statsRunsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_statsRunsTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_statsRunsTable->setMaximumHeight(140);
    m_statsRunsTable->setAlternatingRowColors(true);
    statsLayout->addWidget(m_statsRunsTable);
}

void BScheduleWidget::onEntryClicked(const BScheduleEntry &entry)
{
    updateStatsPanel(entry);
    m_statsPanel->setVisible(true);
}

void BScheduleWidget::updateStatsPanel(const BScheduleEntry &entry)
{
    // Title
    QString title = entry.scheduleName;
    if (!entry.jobName.isEmpty()) {
        title += QString(" / %1").arg(entry.jobName);
    }
    title += QString(" (%1, %2:%3)")
        .arg(entry.levelLabel())
        .arg(entry.hour, 2, 10, QChar('0'))
        .arg(entry.minute, 2, 10, QChar('0'));
    m_statsTitle->setText(title);

    // --- Job config fields ---
    {
        QString type = "-", client = "-", fileset = "-", storage = "-";
        int priority = entry.priority;

        // Prefer data from BScheduleEntry (already enriched)
        if (!entry.client.isEmpty())  client  = entry.client;
        if (!entry.storage.isEmpty()) storage = entry.storage;

        // Enrich further from BJobConfigModel if available
        if (m_jobConfigModel && !entry.jobName.isEmpty()) {
            const auto &raw = m_jobConfigModel->jobConfigsRaw();
            if (raw.contains(entry.jobName)) {
                const QJsonObject &cfg = raw[entry.jobName];
                if (!cfg["type"].toString().isEmpty())    type    = cfg["type"].toString();
                if (!cfg["fileset"].toString().isEmpty()) fileset = cfg["fileset"].toString();
                if (!cfg["client"].toString().isEmpty() && client == "-")
                    client = cfg["client"].toString();
                if (!cfg["storage"].toString().isEmpty() && storage == "-")
                    storage = cfg["storage"].toString();
            }
        }

        m_statsJobType->setText(type);
        m_statsClient->setText(client);
        m_statsFileSet->setText(fileset);
        m_statsStorage->setText(storage);
        m_statsPriority->setText(QString::number(priority));
    }

    // What was measured for this job at the level of this run
    const QString levelStr = historyLevel(entry);
    const BJobHistoryStats::Estimate estimate = m_historyStats.estimate(entry.jobName, levelStr);
    const QString none = QStringLiteral("-");

    if (!estimate.hasHistory()) {
        QString why;
        if (entry.jobName.isEmpty()) {
            why = tr("no job uses this schedule");
        } else if (!historyNote().isEmpty()) {
            why = historyNote();
        } else {
            why = tr("no successful run of this job in the last %1 days")
                      .arg(m_historyStats.windowDays());
        }
        m_statsBasis->setText(tr("No history: %1.").arg(why));
        m_statsBasis->setVisible(true);
        m_statsMedian->setText(none);
        m_statsSpread->setText(none);
        m_statsP90->setText(none);
        m_statsSamples->setText(QStringLiteral("0"));
        m_statsWaitMedian->setText(none);
        m_statsWaitP90->setText(none);
    } else {
        if (estimate.isMeasured()) {
            m_statsBasis->setVisible(false);
        } else {
            m_statsBasis->setText(tr("Estimate: no run at this level in the last %1 days. "
                                     "The figures are from all levels of the job.")
                                      .arg(m_historyStats.windowDays()));
            m_statsBasis->setVisible(true);
        }
        m_statsMedian->setText(BJobHistoryStats::formatDuration(estimate.duration.median));
        m_statsSpread->setText(QString("%1 - %2")
            .arg(BJobHistoryStats::formatDuration(estimate.duration.p25),
                 BJobHistoryStats::formatDuration(estimate.duration.p75)));
        m_statsP90->setText(BJobHistoryStats::formatDuration(estimate.duration.p90));
        m_statsSamples->setText(QString::number(estimate.runs));
        if (estimate.wait.isValid()) {
            m_statsWaitMedian->setText(BJobHistoryStats::formatDuration(estimate.wait.median));
            m_statsWaitP90->setText(BJobHistoryStats::formatDuration(estimate.wait.p90));
        } else {
            // No scheduled time delivered: unknown, not zero
            m_statsWaitMedian->setText(tr("unknown"));
            m_statsWaitP90->setText(tr("unknown"));
        }
    }

    // Last runs table: the runs the figures above are from
    m_statsRunsTable->clearSpans();
    const QList<BJobHistoryStats::MeasuredRun> runs = m_historyStats.lastRuns(
        entry.jobName, estimate.isMeasured() ? levelStr : QString(), 5);
    m_statsRunsTable->setRowCount(runs.size());

    for (int i = 0; i < runs.size(); ++i) {
        const BJobHistoryStats::MeasuredRun &run = runs[i];

        m_statsRunsTable->setItem(i, 0, new QTableWidgetItem(
            run.started.toString("yyyy-MM-dd HH:mm")));
        m_statsRunsTable->setItem(i, 1, new QTableWidgetItem(run.level));
        m_statsRunsTable->setItem(i, 2, new QTableWidgetItem(
            BJobHistoryStats::formatDuration(run.durationSecs)));
        m_statsRunsTable->setItem(i, 3, new QTableWidgetItem(
            run.waitSecs ? BJobHistoryStats::formatDuration(*run.waitSecs) : tr("unknown")));
    }

    if (runs.isEmpty()) {
        m_statsRunsTable->setRowCount(1);
        QTableWidgetItem *noData = new QTableWidgetItem(tr("No history"));
        noData->setForeground(Qt::gray);
        m_statsRunsTable->setItem(0, 0, noData);
        m_statsRunsTable->setSpan(0, 0, 1, 4);
    }
}

void BScheduleWidget::onZoomSliderChanged(int value)
{
    m_fdGanttWidget->setZoomLevel(value);
    m_sdGanttWidget->setZoomLevel(value);
    resizeBothGanttWidgets();
}

void BScheduleWidget::onSnapChanged(int index)
{
    int minutes = m_snapCombo->itemData(index).toInt();
    m_fdGanttWidget->setSnapMinutes(minutes);
    m_sdGanttWidget->setSnapMinutes(minutes);
    BSettings::instance().setSchedulesSnapIndex(index);
}

void BScheduleWidget::updateSnapComboForViewMode(BScheduleGanttWidget::ViewMode mode)
{
    m_snapCombo->blockSignals(true);
    int oldMinutes = m_snapCombo->currentData().toInt();
    m_snapCombo->clear();

    if (mode == BScheduleGanttWidget::WeekView) {
        m_snapCombo->addItem(tr("1 hour"), 60);
        m_snapCombo->addItem(tr("2 hours"), 120);
        m_snapCombo->addItem(tr("4 hours"), 240);
    } else {
        m_snapCombo->addItem(tr("15 min"), 15);
        m_snapCombo->addItem(tr("30 min"), 30);
        m_snapCombo->addItem(tr("1 hour"), 60);
    }

    // Try to keep the closest matching value selected
    int bestIndex = 0;
    for (int i = 0; i < m_snapCombo->count(); ++i) {
        if (m_snapCombo->itemData(i).toInt() <= oldMinutes)
            bestIndex = i;
    }
    m_snapCombo->setCurrentIndex(bestIndex);
    m_snapCombo->blockSignals(false);

    int minutes = m_snapCombo->itemData(bestIndex).toInt();
    m_fdGanttWidget->setSnapMinutes(minutes);
    m_sdGanttWidget->setSnapMinutes(minutes);
}

QList<BSchedule> BScheduleWidget::checkedSchedules() const
{
    QList<BSchedule> schedules;
    for (int i = 0; i < m_scheduleList->count(); ++i) {
        QListWidgetItem *item = m_scheduleList->item(i);
        if (item->checkState() == Qt::Checked) {
            schedules.append(m_scheduleModel->schedule(item->text()));
        }
    }
    return schedules;
}

void BScheduleWidget::updateGanttEntries()
{
    // Filter by checked schedules; none checked shows all of them
    QList<BSchedule> schedules = checkedSchedules();
    if (schedules.isEmpty()) {
        schedules = m_scheduleModel->schedules();
    }

    // The dates the timelines show: one day, or the week it is in
    const QDate first = m_fdGanttWidget->firstDate();
    const QDateTime from = first.startOfDay();
    const QDateTime to = first.addDays(m_fdGanttWidget->dayCount()).startOfDay();

    // What really starts on these dates, once per job that runs it. How
    // long a run lasts is not part of an entry: the timelines take it from
    // the measured history (setHistoryStats()).
    const QList<BScheduleEntry> entries = m_jobScheduleIndex->entriesFor(schedules, from, to);

    // Both widgets receive the identical list
    m_fdGanttWidget->setEntries(entries);
    m_sdGanttWidget->setEntries(entries);

    // Something is on display that wants durations
    scheduleHistoryRequest();
}

void BScheduleWidget::updateRunProblems()
{
    // A Run line that is not understood has no time and no day, so neither
    // view can draw it. It is named here and on its schedule in the list.
    const QList<BRunProblem> problems = m_scheduleModel->runProblems();

    QMap<QString, QStringList> bySchedule;
    QStringList all;
    for (const BRunProblem &problem : problems) {
        const QString text = tr("Run line %1 \"%2\": %3")
            .arg(problem.line).arg(problem.text, problem.error);
        bySchedule[problem.scheduleName].append(text);
        all.append(QString("%1: %2").arg(problem.scheduleName, text));
    }

    for (int i = 0; i < m_scheduleList->count(); ++i) {
        QListWidgetItem *item = m_scheduleList->item(i);
        const QStringList lines = bySchedule.value(item->text());
        if (lines.isEmpty()) {
            item->setIcon(QIcon());
            item->setToolTip(QString());
        } else {
            item->setIcon(QIcon::fromTheme("dialog-warning",
                                           style()->standardIcon(QStyle::SP_MessageBoxWarning)));
            item->setToolTip(tr("Not understood and not shown in the views:") + "\n"
                             + lines.join("\n") + "\n"
                             + tr("This schedule cannot be written by the application."));
        }
    }

    m_runProblemLabel->setVisible(!all.isEmpty());
    if (!all.isEmpty()) {
        m_runProblemLabel->setText(tr("%n Run line(s) not understood", "", all.size()));
        m_runProblemLabel->setToolTip(
            tr("These Run lines are not shown in the views, and their schedules cannot "
               "be written by the application:") + "\n" + all.join("\n"));
    }
}

void BScheduleWidget::resizeBothGanttWidgets()
{
    // Stretch-to-fit: widgets auto-size via QScrollArea::widgetResizable
    // Height still needs updating when rows change
    m_fdGanttWidget->setMinimumHeight(m_fdGanttWidget->sizeHint().height());
    m_sdGanttWidget->setMinimumHeight(m_sdGanttWidget->sizeHint().height());
}

void BScheduleWidget::onScheduleCheckChanged(QListWidgetItem *item)
{
    if (item) {
        BSettings::instance().setSchedulesFilterCheckbox(
            item->text(), item->checkState() == Qt::Checked);
    }
    updateGanttEntries();
    updateJobList();
}

void BScheduleWidget::updateGridView(bool namesOnly)
{
    // The grid shows one schedule resource: the current row of the list.
    // The list is rebuilt on every refresh, so the choice is kept by name;
    // without one the first schedule is taken.
    QListWidgetItem *current = m_scheduleList->currentItem();
    if (!current && m_scheduleList->count() > 0) {
        const QList<QListWidgetItem *> named =
            m_scheduleList->findItems(m_gridScheduleName, Qt::MatchExactly);
        current = (!m_gridScheduleName.isEmpty() && !named.isEmpty()) ? named.first()
                                                                      : m_scheduleList->item(0);
        const QSignalBlocker blocker(m_scheduleList);
        m_scheduleList->setCurrentItem(current);
    }
    if (!current) {
        m_gridView->clear();
        return;
    }
    m_gridScheduleName = current->text();

    // A refresh that brings the same schedule keeps scope and selection.
    // The list of names (".schedule") comes first on every refresh and may
    // come without Run lines: it does not replace a schedule that is on
    // display, the reply with the Run lines follows.
    const BSchedule schedule = m_scheduleModel->schedule(m_gridScheduleName);
    const BSchedule &shown = m_gridView->model()->loadedSchedule();
    bool same = m_gridView->hasSchedule() && schedule.name == shown.name;
    if (same && !namesOnly) {
        same = schedule.description == shown.description && schedule.enabled == shown.enabled
            && schedule.runs.size() == shown.runs.size();
        for (int i = 0; same && i < schedule.runs.size(); ++i) {
            same = schedule.runs[i].original == shown.runs[i].original;
        }
    }
    if (!same) {
        m_gridView->setSchedule(schedule);
    }

    QStringList jobs;
    for (const BJobScheduleIndex::JobRef &job : m_jobScheduleIndex->jobsForSchedule(m_gridScheduleName)) {
        jobs.append(job.jobName);
    }
    m_gridView->setUsage(jobs, m_jobScheduleIndex->isValid());
}

void BScheduleWidget::onFdDragCompleted(
    int entryIndex, int newHour, int newMinute,
    const QList<BScheduleGanttWidget::DependencyEdge> &dependencies)
{
    handleDragCompleted(m_fdGanttWidget, entryIndex, newHour, newMinute, dependencies);
}

void BScheduleWidget::onSdDragCompleted(
    int entryIndex, int newHour, int newMinute,
    const QList<BScheduleGanttWidget::DependencyEdge> &dependencies)
{
    handleDragCompleted(m_sdGanttWidget, entryIndex, newHour, newMinute, dependencies);
}

void BScheduleWidget::handleDragCompleted(
    BScheduleGanttWidget *source,
    int entryIndex, int newHour, int newMinute,
    const QList<BScheduleGanttWidget::DependencyEdge> &dependencies)
{
    const QList<BScheduleEntry> &entries = source->entries();
    if (entryIndex < 0 || entryIndex >= entries.size()) return;

    const BScheduleEntry &entry = entries[entryIndex];

    auto *dialog = new BScheduleDragResultDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setDirector(m_director);
    // The dialog compares what it would write with the Director's own Run lines
    dialog->setDragResult(entry, newHour, newMinute,
                          m_scheduleModel->runDirectives(entry.scheduleName),
                          dependencies, m_jobScheduleIndex);

    // Forward the sendCommand signal to the main window
    connect(dialog, &BScheduleDragResultDialog::sendCommand,
            this, &BScheduleWidget::sendCommand);

    dialog->show();

    // The user moved an entry of an existing schedule and expects that
    // schedule to change. Neither Director can do that through the console;
    // the dialog offers a copy instead. Said once per session, after the
    // drag has been handled, and only as additional information: the dialog
    // works as before.
    BDirector *director = m_director;
    QTimer::singleShot(0, dialog, [dialog, director]() {
        BCapabilityNotice::check(dialog, director, BCapabilities::Function::ModifySchedule,
                                 BCapabilities::Request::Implicit);
    });
}

void BScheduleWidget::onJobSelectionChanged()
{
    QList<QTableWidgetItem *> selected = m_jobList->selectedItems();
    if (selected.isEmpty()) return;

    // Get the job name from column 0 of the selected row
    int row = selected.first()->row();
    QTableWidgetItem *nameItem = m_jobList->item(row, 0);
    if (!nameItem) return;

    QString jobName = nameItem->text();

    // Find matching entry in the FD Gantt widget entries
    const QList<BScheduleEntry> &entries = m_fdGanttWidget->entries();
    for (const BScheduleEntry &entry : entries) {
        if (entry.jobName == jobName) {
            onEntryClicked(entry);
            return;
        }
    }
}

void BScheduleWidget::onJobListDoubleClicked(int row, int /*column*/)
{
    QTableWidgetItem *nameItem = m_jobList->item(row, 0);
    if (!nameItem) return;

    const QString jobName = nameItem->text();
    if (!jobName.isEmpty())
        emit editJobRequested(jobName);
}

// ============================================================================
// Job history: measured durations and waits
// ============================================================================

void BScheduleWidget::setupWaitsPage()
{
    m_waitsPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_waitsPage);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // Above the table: the window, when the data was fetched, what was left
    // out; and the only button that asks the Director for the history
    QHBoxLayout *infoRow = new QHBoxLayout();
    m_waitsInfoLabel = new QLabel(m_waitsPage);
    m_waitsInfoLabel->setWordWrap(true);
    m_waitsInfoLabel->setTextFormat(Qt::PlainText);
    m_waitsInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    infoRow->addWidget(m_waitsInfoLabel, 1);

    m_waitsRefreshButton = new QPushButton(tr("Refresh history"), m_waitsPage);
    m_waitsRefreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_waitsRefreshButton->setToolTip(
        tr("Ask the Director again for the jobs of the last days (llist jobs)"));
    m_waitsRefreshButton->setEnabled(false);
    infoRow->addWidget(m_waitsRefreshButton, 0, Qt::AlignTop);
    layout->addLayout(infoRow);

    m_waitsTable = new QTableWidget(0, 8, m_waitsPage);
    m_waitsTable->setHorizontalHeaderLabels({
        tr("Job"), tr("Level"), tr("Runs"),
        tr("Median duration"), tr("90% duration"),
        tr("Median wait"), tr("90% wait"), tr("Last run")});
    m_waitsTable->horizontalHeaderItem(2)->setToolTip(
        tr("Runs in the window that terminated normally or with warnings"));
    m_waitsTable->horizontalHeaderItem(5)->setToolTip(
        tr("Wait: start of the job minus its scheduled time"));
    m_waitsTable->horizontalHeaderItem(6)->setToolTip(
        tr("Nine of ten runs waited no longer than this"));
    m_waitsTable->setAlternatingRowColors(true);
    m_waitsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_waitsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_waitsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_waitsTable->verticalHeader()->setVisible(false);
    m_waitsTable->horizontalHeader()->setStretchLastSection(true);
    m_waitsTable->setSortingEnabled(true);
    // Longest waits first
    m_waitsTable->sortByColumn(6, Qt::DescendingOrder);
    layout->addWidget(m_waitsTable, 1);

    connect(m_waitsRefreshButton, &QPushButton::clicked,
            this, &BScheduleWidget::onHistoryRefreshClicked);
}

void BScheduleWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    scheduleHistoryRequest();
}

void BScheduleWidget::scheduleHistoryRequest()
{
    if (!m_connected || !isVisible() || m_historyTimer->isActive()) return;

    BSettings &s = BSettings::instance();
    const bool windowChanged = m_historyState == HistoryLoaded
        && (s.schedulesHistoryDays() != m_historyDays
            || s.schedulesHistoryRowLimit() != m_historyLimit);

    // Once per connection, and again only for another window. A request
    // that was refused is not repeated by itself.
    if (m_historyState == HistoryNotRequested || windowChanged) {
        m_historyTimer->start();
    }
}

void BScheduleWidget::requestHistoryIfDue()
{
    // The conditions may have gone while the timer ran
    if (!m_connected || !isVisible()) return;

    BSettings &s = BSettings::instance();
    const bool windowChanged = m_historyState == HistoryLoaded
        && (s.schedulesHistoryDays() != m_historyDays
            || s.schedulesHistoryRowLimit() != m_historyLimit);

    if (m_historyState == HistoryNotRequested || windowChanged) {
        requestHistory();
    }
}

void BScheduleWidget::onHistoryRefreshClicked()
{
    requestHistory();
}

void BScheduleWidget::requestHistory()
{
    if (!m_connected) return;
    // One request at a time: replies carry no id of their own
    if (m_historyState == HistoryLoading || m_historyState == HistoryParsing) return;

    BSettings &s = BSettings::instance();
    m_historyDays = s.schedulesHistoryDays();
    m_historyLimit = s.schedulesHistoryRowLimit();
    m_historyArgs = QString("%1,%2").arg(m_historyDays).arg(m_historyLimit);
    m_historyState = HistoryLoading;

    emit sendCommand(BDirector::Command::LlistJobs, m_historyArgs);
    updateHistoryInfo();
}

void BScheduleWidget::processJobHistoryReply(const QString &args, const QString &reply)
{
    // Only the reply this widget waits for
    if (m_historyState != HistoryLoading || args != m_historyArgs) return;

    m_historyState = HistoryParsing;
    updateHistoryInfo();

    // The reply can be several megabytes: it is read off the GUI thread and
    // the result comes back as a queued call
    const quint64 generation = m_historyGeneration;
    QThread *reader = QThread::create([this, reply, generation]() {
        const BJobHistoryReply parsed = BJobHistoryReply::parse(reply);
        QMetaObject::invokeMethod(this, [this, parsed, generation]() {
            applyHistoryReply(parsed, generation);
        }, Qt::QueuedConnection);
    });
    connect(reader, &QThread::finished, reader, &QObject::deleteLater);
    m_historyParser = reader;
    reader->start();
}

void BScheduleWidget::onCommandFailed(BDirector::Command cmd, const QString &args,
                                      const QString &errorMsg)
{
    if (cmd != BDirector::Command::LlistJobs) return;
    if (m_historyState != HistoryLoading || args != m_historyArgs) return;

    // Not sent, or the connection ended before the reply
    setHistoryUnavailable(tr("the request got no reply (%1)").arg(errorMsg.simplified()));
    updateHistoryViews();
}

void BScheduleWidget::applyHistoryReply(const BJobHistoryReply &reply, quint64 generation)
{
    // The connection the reply belongs to is gone
    if (generation != m_historyGeneration || m_historyState != HistoryParsing) return;

    switch (reply.outcome) {
    case BJobHistoryReply::Outcome::Runs: {
        m_historyStats.setRuns(reply.runs, m_historyDays, BJobHistoryStats::wallClockNow());
        m_historyRows = reply.rows;
        m_historyRowsWithSchedTime = reply.rowsWithScheduledTime;
        m_historyFetchedAt = QDateTime::currentDateTime();
        m_historyError.clear();
        m_historyState = HistoryLoaded;

        // At the row limit the reply is cut. "reverse" puts the newest runs
        // first; a Director that ignores the word kept the oldest instead.
        m_historyOldestKept = false;
        if (reply.rows >= m_historyLimit && reply.runs.size() >= 2) {
            const QDateTime first = reply.runs.first().started;
            const QDateTime last = reply.runs.last().started;
            m_historyOldestKept = first.isValid() && last.isValid() && first < last;
        }
        break;
    }
    case BJobHistoryReply::Outcome::Refused:
        setHistoryUnavailable(tr("the Director refused the request (%1)")
            .arg(reply.error.isEmpty() ? tr("no reason given") : reply.error));
        break;
    case BJobHistoryReply::Outcome::Unreadable:
        setHistoryUnavailable(tr("the Director's reply could not be read (%1)").arg(reply.error));
        break;
    }

    updateHistoryViews();
}

void BScheduleWidget::setHistoryUnavailable(const QString &reason)
{
    m_historyStats.clear();
    m_historyFetchedAt = QDateTime();
    m_historyRows = 0;
    m_historyRowsWithSchedTime = 0;
    m_historyOldestKept = false;
    m_historyError = reason;
    m_historyState = HistoryUnavailable;

    BLOG_WARNING() << "BScheduleWidget: no job history:" << reason;

    // Said once per connection in the status line; the line above the table
    // of measured waits and the tooltips keep saying it
    if (!m_historyNoticeShown) {
        m_historyNoticeShown = true;
        emit statusMessageChanged(tr("Schedules: no job history, %1").arg(reason));
    }
}

void BScheduleWidget::resetHistory()
{
    ++m_historyGeneration;      // a reply still being read is dropped
    m_historyTimer->stop();
    m_historyStats.clear();
    m_historyState = HistoryNotRequested;
    m_historyArgs.clear();
    m_historyDays = 0;
    m_historyLimit = 0;
    m_historyRows = 0;
    m_historyRowsWithSchedTime = 0;
    m_historyOldestKept = false;
    m_historyFetchedAt = QDateTime();
    m_historyError.clear();
    m_historyNoticeShown = false;
    updateHistoryViews();
}

QString BScheduleWidget::historyNote() const
{
    switch (m_historyState) {
    case HistoryLoaded:
        return QString();
    case HistoryUnavailable:
        return m_historyError;
    case HistoryLoading:
    case HistoryParsing:
        // A refresh keeps showing the figures of the fetch before it
        return m_historyFetchedAt.isValid() ? QString()
                                            : tr("the job history is being loaded");
    case HistoryNotRequested:
        break;
    }
    return tr("the job history has not been requested yet");
}

QString BScheduleWidget::historyLevel(const BScheduleEntry &entry) const
{
    QString level = entry.levelLabel();
    if (level.isEmpty() && m_jobConfigModel && !entry.jobName.isEmpty()) {
        level = m_jobConfigModel->jobConfigsRaw().value(entry.jobName)
                    .value(QLatin1String("level")).toString();
    }
    return level;
}

void BScheduleWidget::updateHistoryViews()
{
    const QString note = historyNote();
    m_fdGanttWidget->setHistoryStats(m_historyStats, note);
    m_sdGanttWidget->setHistoryStats(m_historyStats, note);
    updateJobList();
    updateWaitsTable();
    updateHistoryInfo();
}

void BScheduleWidget::updateHistoryInfo()
{
    const bool busy = m_historyState == HistoryLoading || m_historyState == HistoryParsing;
    m_waitsRefreshButton->setEnabled(m_connected && !busy);

    QStringList lines;
    if (!m_connected) {
        lines << tr("Not connected.");
    } else if (m_historyState == HistoryNotRequested) {
        lines << tr("The job history has not been requested yet.");
    } else if (m_historyState == HistoryUnavailable) {
        lines << tr("No job history: %1.").arg(m_historyError)
              << tr("The schedule views work without it: runs have no duration and waits are unknown.");
    }

    if (busy) {
        lines << tr("Requesting the jobs of the last %1 days from the Director...")
                     .arg(m_historyDays);
    }

    if (m_historyFetchedAt.isValid()) {
        const BJobHistoryStats::Summary sum = m_historyStats.summary();
        lines << tr("Jobs scheduled in the last %1 days, fetched %2: %3 rows, of which %4 runs "
                    "terminated normally or with warnings and were measured.")
                     .arg(m_historyStats.windowDays())
                     .arg(m_historyFetchedAt.toString("yyyy-MM-dd HH:mm"))
                     .arg(m_historyRows)
                     .arg(sum.counted);

        if (m_historyRows >= m_historyLimit && m_historyLimit > 0) {
            lines << (m_historyOldestKept
                ? tr("The row limit (%1) was reached and this Director returned the oldest "
                     "rows: the newest runs of the window are missing.").arg(m_historyLimit)
                : tr("The row limit (%1) was reached: the oldest runs of the window are "
                     "missing.").arg(m_historyLimit));
        }
        if (m_historyRows > 0 && m_historyRowsWithSchedTime == 0) {
            lines << tr("This Director did not deliver the scheduled time: all waits are unknown.");
        } else if (sum.waitUnknown > 0) {
            lines << tr("%1 runs came without a scheduled time: their wait is unknown.")
                         .arg(sum.waitUnknown);
        }
        if (sum.foreignTimes > 0) {
            lines << tr("%1 rows were left out because they start before their scheduled time: "
                        "copied, migrated and virtual full jobs carry the times of another job.")
                         .arg(sum.foreignTimes);
        }
        if (sum.unusableTimes > 0) {
            lines << tr("%1 rows were left out because their start or end time is missing or "
                        "the end is before the start.").arg(sum.unusableTimes);
        }
    }

    m_waitsInfoLabel->setText(lines.join('\n'));
}

void BScheduleWidget::updateWaitsTable()
{
    const QList<BJobHistoryStats::Group> groups = m_historyStats.groups();

    // Filled unsorted, sorted once at the end by the column the user chose
    m_waitsTable->setSortingEnabled(false);
    m_waitsTable->clearContents();
    m_waitsTable->setRowCount(groups.size());

    auto duration = [](qint64 secs) {
        return new BSortTableItem(BJobHistoryStats::formatDuration(secs), secs);
    };

    for (int row = 0; row < groups.size(); ++row) {
        const BJobHistoryStats::Group &group = groups[row];

        m_waitsTable->setItem(row, 0, new QTableWidgetItem(group.job));
        m_waitsTable->setItem(row, 1, new QTableWidgetItem(
            group.level.isEmpty() ? tr("(none)") : group.level));
        m_waitsTable->setItem(row, 2, new BSortTableItem(QString::number(group.runs), group.runs));
        m_waitsTable->setItem(row, 3, duration(group.duration.median));
        m_waitsTable->setItem(row, 4, duration(group.duration.p90));

        if (group.wait.isValid()) {
            QTableWidgetItem *median = duration(group.wait.median);
            QTableWidgetItem *p90 = duration(group.wait.p90);
            if (group.waitUnknown > 0) {
                const QString tip = tr("From %1 of %2 runs; the others came without a scheduled time.")
                                        .arg(group.wait.count).arg(group.runs);
                median->setToolTip(tip);
                p90->setToolTip(tip);
            }
            m_waitsTable->setItem(row, 5, median);
            m_waitsTable->setItem(row, 6, p90);
        } else {
            // Unknown is not zero: it sorts before every measured wait
            const QString tip = tr("The Director delivered no scheduled time for these runs.");
            for (int column = 5; column <= 6; ++column) {
                QTableWidgetItem *unknown = new BSortTableItem(tr("unknown"), BSortValue::Missing);
                unknown->setToolTip(tip);
                m_waitsTable->setItem(row, column, unknown);
            }
        }

        m_waitsTable->setItem(row, 7, new BSortTableItem(
            group.lastRun.toString("yyyy-MM-dd HH:mm"),
            group.lastRun.isValid() ? group.lastRun.toMSecsSinceEpoch() : BSortValue::Missing));
    }

    m_waitsTable->setSortingEnabled(true);
    m_waitsTable->resizeColumnsToContents();
}
