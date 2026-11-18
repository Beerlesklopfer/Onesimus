#include "schedules/bschedulewidget.h"
#include "schedules/bscheduledragresultdialog.h"
#include "bcapabilitynotice.h"
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
#include <QSet>
#include <QHeaderView>
#include <QScrollBar>
#include <QStyle>

BScheduleWidget::BScheduleWidget(QWidget *parent)
    : QWidget(parent)
    , m_scheduleModel(new BScheduleModel(this))
    , m_durationStats(new BJobDurationStats(this))
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
    , m_weeklyPlanner(new BWeeklyPlanner(this))
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
    setupUI();
    m_fdGanttWidget->setDurationStats(m_durationStats);
    m_sdGanttWidget->setDurationStats(m_durationStats);

    // Lock group modes: FD always by client, SD always by storage
    m_fdGanttWidget->setGroupMode(BScheduleGanttWidget::GroupByClient);
    m_sdGanttWidget->setGroupMode(BScheduleGanttWidget::GroupByStorage);
}

BScheduleWidget::~BScheduleWidget()
{
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
    toolbarLayout->addWidget(new QLabel(tr("Snap:"), this));
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
    m_jobList->setHorizontalHeaderLabels({tr("Job"), tr("Type"), tr("Client"), tr("Storage"), tr("FileSet"), tr("Duration")});
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

    // Wrap weekly planner in scroll area
    m_gridScrollArea = new QScrollArea(this);
    m_gridScrollArea->setWidget(m_weeklyPlanner);
    m_gridScrollArea->setWidgetResizable(true);
    m_gridScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_gridScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Below the grid: the Run lines that have no run in the week on display,
    // so that they do not go unnoticed
    QWidget *gridPane = new QWidget(this);
    QVBoxLayout *gridLayout = new QVBoxLayout(gridPane);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(2);
    gridLayout->addWidget(m_gridScrollArea, 1);
    m_outsideWeekLabel = new QLabel(gridPane);
    m_outsideWeekLabel->setWordWrap(true);
    m_outsideWeekLabel->setTextFormat(Qt::PlainText);
    m_outsideWeekLabel->setVisible(false);
    gridLayout->addWidget(m_outsideWeekLabel);
    m_viewStack->addWidget(gridPane);            // index 1 = Grid

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
    m_refreshButton->setEnabled(connected);

    if (!connected) {
        clearData();
    }
}

void BScheduleWidget::clearData()
{
    m_scheduleList->clear();
    m_weeklyPlanner->clearSchedule();
    m_fdGanttWidget->setEntries({});
    m_sdGanttWidget->setEntries({});
    m_scheduleModel->clear();
    m_durationStats->clear();
    m_collisionLabel->setVisible(false);
    m_runProblemLabel->setVisible(false);
    m_outsideWeekLabel->setVisible(false);
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
    updateFilteredViews();
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
    updateFilteredViews();
    updateGanttEntries();
    updateJobList();
}

void BScheduleWidget::processListJobsResponse(const QString &jsonData)
{
    m_durationStats->feedJobs(jsonData);

    // Re-enrich Gantt entries with updated durations
    updateGanttEntries();
    updateJobList();  // Refresh duration column with updated stats
}

void BScheduleWidget::processShowJobsResponse(const QString &)
{
    // Rebuild job→schedule index when job configs are updated
    if (m_jobConfigModel) {
        m_jobScheduleIndex->rebuild(m_jobConfigModel);
        updateGanttEntries();
        updateJobList();
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

        int avgSecs = m_durationStats->averageDuration(job.jobName);
        QString durStr = (avgSecs > 0)
            ? BJobDurationStats::formatDuration(avgSecs)
            : tr("-");
        m_jobList->setItem(i, 5, new QTableWidgetItem(durStr));
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

    // Show/hide Gantt-specific controls
    // The date navigation stays: the grid shows a concrete week too
    bool isGantt = (index == 0);
    m_dayViewCombo->setVisible(isGantt);
    m_snapCombo->setVisible(isGantt);
    updateDateLabel();
    bool isWeekGantt = isGantt && (m_dayViewCombo->currentIndex() == 1);
    m_zoomLabel->setVisible(isWeekGantt);
    m_zoomSlider->setVisible(isWeekGantt);

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
    return m_viewModeCombo->currentIndex() == 1 || m_dayViewCombo->currentIndex() == 1;
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
    updateFilteredViews();
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

void BScheduleWidget::onCollisionsDetected(int count)
{
    if (count > 0) {
        m_collisionLabel->setText(tr("%1 collision(s)").arg(count));
        m_collisionLabel->setVisible(true);
    } else {
        m_collisionLabel->setVisible(false);
    }
}

void BScheduleWidget::setupStatsPanel()
{
    m_statsPanel = new QGroupBox(tr("Duration Statistics"), this);
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

    // Duration stats row: Min | Avg | Max | Samples | Trend
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

    m_statsMin = addStatBox(tr("Min"));
    m_statsAvg = addStatBox(tr("Avg"));
    m_statsMax = addStatBox(tr("Max"));

    // Separator
    QFrame *sep = new QFrame(this);
    sep->setFrameShape(QFrame::VLine);
    statsRow->addWidget(sep);

    m_statsSamples = addStatBox(tr("Samples"));
    m_statsTrend = addStatBox(tr("Trend"));

    statsRow->addStretch();
    statsLayout->addLayout(statsRow);

    // Last runs table
    m_statsRunsTable = new QTableWidget(0, 3, this);
    m_statsRunsTable->setHorizontalHeaderLabels({tr("Date"), tr("Level"), tr("Duration")});
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

    // Look up job name for level-specific duration stats
    QString lookupName = entry.jobName;
    if (lookupName.isEmpty()) {
        lookupName = entry.scheduleName;  // fallback
    }
    QString levelStr = entry.levelLabel();

    int minDur = m_durationStats->minDuration(lookupName, levelStr);
    int avgDur = m_durationStats->averageDuration(lookupName, levelStr);
    int maxDur = m_durationStats->maxDuration(lookupName, levelStr);
    int samples = m_durationStats->sampleCount(lookupName, levelStr);

    m_statsMin->setText(BJobDurationStats::formatDuration(minDur));
    m_statsAvg->setText(BJobDurationStats::formatDuration(avgDur));
    m_statsMax->setText(BJobDurationStats::formatDuration(maxDur));
    m_statsSamples->setText(QString::number(samples));

    // Trend (level-specific)
    double trendVal = m_durationStats->trend(lookupName, levelStr);
    if (samples < 4 || qAbs(trendVal) < 1.0) {
        m_statsTrend->setText("-");
        m_statsTrend->setStyleSheet("font-size: 12pt; font-weight: bold;");
    } else if (trendVal > 0) {
        m_statsTrend->setText(QString("+%1").arg(BJobDurationStats::formatDuration(static_cast<int>(trendVal))));
        m_statsTrend->setStyleSheet("font-size: 12pt; font-weight: bold; color: #e64a19;");  // orange-red = slower
    } else {
        m_statsTrend->setText(QString("-%1").arg(BJobDurationStats::formatDuration(static_cast<int>(-trendVal))));
        m_statsTrend->setStyleSheet("font-size: 12pt; font-weight: bold; color: #388e3c;");  // green = faster
    }

    // Last runs table (level-specific)
    QList<BJobDurationStats::JobRunRecord> runs = m_durationStats->lastRuns(lookupName, levelStr, 5);
    m_statsRunsTable->setRowCount(runs.size());

    for (int i = 0; i < runs.size(); ++i) {
        const auto &run = runs[i];

        QString dateStr = run.startTime.isValid()
            ? run.startTime.toString("yyyy-MM-dd HH:mm")
            : tr("(unknown)");
        m_statsRunsTable->setItem(i, 0, new QTableWidgetItem(dateStr));
        m_statsRunsTable->setItem(i, 1, new QTableWidgetItem(run.level));
        m_statsRunsTable->setItem(i, 2, new QTableWidgetItem(
            BJobDurationStats::formatDuration(run.durationSecs)));
    }

    if (runs.isEmpty()) {
        m_statsRunsTable->setRowCount(1);
        QTableWidgetItem *noData = new QTableWidgetItem(tr("No historical data available"));
        noData->setForeground(Qt::gray);
        m_statsRunsTable->setItem(0, 0, noData);
        m_statsRunsTable->setSpan(0, 0, 1, 3);
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

    // What really starts on these dates, once per job that runs it
    QList<BScheduleEntry> entries = m_jobScheduleIndex->entriesFor(schedules, from, to);

    // Enrich with level-specific duration estimates
    for (BScheduleEntry &e : entries) {
        if (e.estimatedDurationSecs <= 0 && !e.jobName.isEmpty()) {
            QString levelStr = e.levelLabel();
            e.estimatedDurationSecs = m_durationStats->averageDuration(e.jobName, levelStr);
        }
    }

    // Both widgets receive the identical list
    m_fdGanttWidget->setEntries(entries);
    m_sdGanttWidget->setEntries(entries);
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
    updateFilteredViews();
    updateGanttEntries();
    updateJobList();
}

void BScheduleWidget::updateFilteredViews()
{
    // The grid shows the week (Monday to Sunday) of the date on display
    const QDate monday = BScheduleGanttWidget::firstDateFor(
        BScheduleGanttWidget::WeekView, m_currentDate);
    const QDateTime from = monday.startOfDay();
    const QDateTime to = monday.addDays(7).startOfDay();

    QList<BScheduleEntry> entries;
    QStringList outside;    // Understood Run lines without a run in this week

    for (const BSchedule &schedule : checkedSchedules()) {
        const QList<BOccurrence> occurrences =
            BScheduleOccurrences::occurrences(schedule, from, to);
        entries.append(BScheduleEntry::fromOccurrences(schedule, occurrences));

        QSet<int> ran;
        for (const BOccurrence &occurrence : occurrences) {
            ran.insert(occurrence.run);
        }
        for (int i = 0; i < schedule.runs.size(); ++i) {
            const BRunLine &line = schedule.runs[i];
            if (!line.spec || ran.contains(i)) continue;

            const QDateTime next = BScheduleOccurrences::next(*line.spec, to);
            outside.append(next.isValid()
                ? tr("%1: %2 (next run after this week: %3)")
                      .arg(schedule.name, line.original, next.date().toString(Qt::ISODate))
                : tr("%1: %2 (no run in the ten years after this week)")
                      .arg(schedule.name, line.original));
        }
    }

    m_weeklyPlanner->setWeek(monday, entries);

    // A run that is not in this week is not in the grid. Say so, or the
    // schedule looks as if it had fewer runs than it has.
    m_outsideWeekLabel->setVisible(!outside.isEmpty());
    if (!outside.isEmpty()) {
        const int shown = qMin(3, outside.size());
        QString text = tr("%n Run line(s) of the shown schedules do not run in this week:",
                          "", outside.size())
                       + " " + outside.mid(0, shown).join("; ");
        if (outside.size() > shown) {
            text += " " + tr("... (all of them in the tooltip)");
        }
        m_outsideWeekLabel->setText(text);
        m_outsideWeekLabel->setToolTip(outside.join("\n"));
    }
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
