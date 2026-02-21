#include "schedules/bschedulewidget.h"
#include "schedules/bscheduledragresultdialog.h"
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

BScheduleWidget::BScheduleWidget(QWidget *parent)
    : QWidget(parent)
    , m_scheduleModel(new BScheduleModel(this))
    , m_durationStats(new BJobDurationStats(this))
    , m_splitter(new QSplitter(Qt::Horizontal, this))
    , m_scheduleList(new QListWidget(this))
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
    , m_dayLabel(new QLabel(this))
    , m_snapCombo(new QComboBox(this))
    , m_collisionLabel(new QLabel(this))
    , m_jobScheduleIndex(new BJobScheduleIndex(this))
    , m_director(nullptr)
    , m_currentDay(QDate::currentDate().dayOfWeek() - 1)  // Qt: 1=Mon → 0=Mon
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
    m_prevDayButton->setIcon(QIcon::fromTheme("go-previous"));
    m_prevDayButton->setAutoRaise(true);
    m_nextDayButton->setIcon(QIcon::fromTheme("go-next"));
    m_nextDayButton->setAutoRaise(true);

    m_dayLabel->setMinimumWidth(50);
    m_dayLabel->setAlignment(Qt::AlignCenter);

    toolbarLayout->addWidget(m_prevDayButton);
    toolbarLayout->addWidget(m_dayLabel);
    toolbarLayout->addWidget(m_nextDayButton);

    toolbarLayout->addSpacing(10);

    // Snap granularity
    toolbarLayout->addWidget(new QLabel(tr("Snap:"), this));
    m_snapCombo->addItem(tr("15 min"), 15);
    m_snapCombo->addItem(tr("30 min"), 30);
    m_snapCombo->addItem(tr("1 hour"), 60);
    toolbarLayout->addWidget(m_snapCombo);

    toolbarLayout->addStretch();

    // Collision indicator
    m_collisionLabel->setVisible(false);
    m_collisionLabel->setStyleSheet("color: red; font-weight: bold;");
    toolbarLayout->addWidget(m_collisionLabel);

    // Refresh
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);
    toolbarLayout->addWidget(m_refreshButton);

    mainLayout->addLayout(toolbarLayout);

    // === Splitter ===
    // Left side: Schedule list
    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    QLabel *listLabel = new QLabel(tr("Schedules:"), this);
    listLabel->setStyleSheet("font-weight: bold; font-size: 12pt;");
    listLabel->setAlignment(Qt::AlignRight);
    leftLayout->addWidget(listLabel);

    m_scheduleList->setAlternatingRowColors(true);
    leftLayout->addWidget(m_scheduleList);

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
    m_fdScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    m_sdScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_sdScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    sdLayout->addWidget(m_sdScrollArea);

    // Vertical splitter holding both
    m_ganttSplitter->addWidget(fdPane);
    m_ganttSplitter->addWidget(sdPane);
    m_ganttSplitter->setSizes(QList<int>() << 300 << 200);

    m_viewStack->addWidget(m_ganttSplitter);   // index 0 = Gantt (dual)
    m_viewStack->addWidget(m_weeklyPlanner);    // index 1 = Grid

    rightLayout->addWidget(m_viewStack, 1);

    // Stats panel (below Gantt)
    setupStatsPanel();
    rightLayout->addWidget(m_statsPanel);

    m_splitter->addWidget(leftWidget);
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

    connect(m_viewModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onViewModeChanged);
    connect(m_dayViewCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onDayViewModeChanged);
    connect(m_prevDayButton, &QToolButton::clicked,
            this, &BScheduleWidget::onDayNavigationPrev);
    connect(m_nextDayButton, &QToolButton::clicked,
            this, &BScheduleWidget::onDayNavigationNext);
    connect(m_snapCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onSnapChanged);

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

    // === Restore settings ===
    BSettings &s = BSettings::instance();

    int viewMode = s.schedulesViewMode();
    m_viewModeCombo->setCurrentIndex(viewMode);

    int dayViewMode = s.schedulesDayViewMode();
    m_dayViewCombo->setCurrentIndex(dayViewMode);

    m_currentDay = s.schedulesCurrentDay();

    int snapIndex = s.schedulesSnapIndex();
    m_snapCombo->setCurrentIndex(snapIndex);

    QByteArray splitterState = s.schedulesSplitterState();
    if (!splitterState.isEmpty()) {
        m_splitter->restoreState(splitterState);
    }

    QByteArray ganttSplitterState = s.schedulesGanttSplitterState();
    if (!ganttSplitterState.isEmpty()) {
        m_ganttSplitter->restoreState(ganttSplitterState);
    }

    // Update day label
    static const QStringList dayNames = {
        tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
        tr("Friday"), tr("Saturday"), tr("Sunday")
    };
    m_dayLabel->setText(dayNames.value(m_currentDay));
    m_fdGanttWidget->setCurrentDay(m_currentDay);
    m_sdGanttWidget->setCurrentDay(m_currentDay);
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
    updateFilteredViews();
    updateGanttEntries();
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

    int entryCount = m_scheduleModel->allEntries().size();
    emit statusMessageChanged(tr("%1 schedules loaded (%2 run entries)")
        .arg(m_scheduleList->count()).arg(entryCount));

    updateFilteredViews();
    updateGanttEntries();
}

void BScheduleWidget::processListJobsResponse(const QString &jsonData)
{
    m_durationStats->feedJobs(jsonData);

    // Re-enrich Gantt entries with updated durations
    updateGanttEntries();
}

void BScheduleWidget::processShowJobsResponse(const QString &)
{
    // Rebuild job→schedule index when job configs are updated
    if (m_jobConfigModel) {
        m_jobScheduleIndex->rebuild(m_jobConfigModel);
        updateGanttEntries();
    }
}

void BScheduleWidget::setJobConfigModel(BJobConfigModel *model)
{
    m_jobConfigModel = model;
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
    // Selection is now handled via checkboxes (onScheduleCheckChanged)
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
    bool isGantt = (index == 0);
    m_dayViewCombo->setVisible(isGantt);
    m_prevDayButton->setVisible(isGantt);
    m_dayLabel->setVisible(isGantt);
    m_nextDayButton->setVisible(isGantt);
    m_snapCombo->setVisible(isGantt);

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

    m_prevDayButton->setEnabled(index == 0);
    m_nextDayButton->setEnabled(index == 0);

    BSettings::instance().setSchedulesDayViewMode(index);
}

void BScheduleWidget::onDayNavigationPrev()
{
    m_currentDay = (m_currentDay + 6) % 7;  // wrap backwards
    static const QStringList dayNames = {
        tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
        tr("Friday"), tr("Saturday"), tr("Sunday")
    };
    m_dayLabel->setText(dayNames.value(m_currentDay));
    m_fdGanttWidget->setCurrentDay(m_currentDay);
    m_sdGanttWidget->setCurrentDay(m_currentDay);
    BSettings::instance().setSchedulesCurrentDay(m_currentDay);
}

void BScheduleWidget::onDayNavigationNext()
{
    m_currentDay = (m_currentDay + 1) % 7;
    static const QStringList dayNames = {
        tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
        tr("Friday"), tr("Saturday"), tr("Sunday")
    };
    m_dayLabel->setText(dayNames.value(m_currentDay));
    m_fdGanttWidget->setCurrentDay(m_currentDay);
    m_sdGanttWidget->setCurrentDay(m_currentDay);
    BSettings::instance().setSchedulesCurrentDay(m_currentDay);
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
    closeButton->setIcon(QIcon::fromTheme("window-close"));
    closeButton->setAutoRaise(true);
    closeButton->setToolTip(tr("Close"));
    connect(closeButton, &QToolButton::clicked, m_statsPanel, [this]() {
        m_statsPanel->setVisible(false);
    });
    titleRow->addWidget(closeButton);
    statsLayout->addLayout(titleRow);

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
        .arg(BScheduleEntry::levelToString(entry.level))
        .arg(entry.hour, 2, 10, QChar('0'))
        .arg(entry.minute, 2, 10, QChar('0'));
    m_statsTitle->setText(title);

    // Look up job name for level-specific duration stats
    QString lookupName = entry.jobName;
    if (lookupName.isEmpty()) {
        lookupName = entry.scheduleName;  // fallback
    }
    QString levelStr = BScheduleEntry::levelToString(entry.level);

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

void BScheduleWidget::onSnapChanged(int index)
{
    int minutes = m_snapCombo->itemData(index).toInt();
    m_fdGanttWidget->setSnapMinutes(minutes);
    m_sdGanttWidget->setSnapMinutes(minutes);
    BSettings::instance().setSchedulesSnapIndex(index);
}

void BScheduleWidget::updateGanttEntries()
{
    QList<BScheduleEntry> entries = m_scheduleModel->allEntries();

    // Enrich entries with job data (jobName, client, storage) from job→schedule index
    if (m_jobScheduleIndex->isValid()) {
        entries = m_jobScheduleIndex->enrichEntries(entries);
    }

    // Filter by checked schedules
    QSet<QString> checkedSchedules;
    for (int i = 0; i < m_scheduleList->count(); ++i) {
        QListWidgetItem *item = m_scheduleList->item(i);
        if (item->checkState() == Qt::Checked) {
            checkedSchedules.insert(item->text());
        }
    }
    if (!checkedSchedules.isEmpty()) {
        QList<BScheduleEntry> filtered;
        for (const BScheduleEntry &e : entries) {
            if (checkedSchedules.contains(e.scheduleName)) {
                filtered.append(e);
            }
        }
        entries = filtered;
    }

    // Enrich with level-specific duration estimates
    for (BScheduleEntry &e : entries) {
        if (e.estimatedDurationSecs <= 0 && !e.jobName.isEmpty()) {
            QString levelStr = BScheduleEntry::levelToString(e.level);
            e.estimatedDurationSecs = m_durationStats->averageDuration(e.jobName, levelStr);
        }
    }

    // Both widgets receive the identical enriched list
    m_fdGanttWidget->setEntries(entries);
    m_sdGanttWidget->setEntries(entries);
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
}

void BScheduleWidget::updateFilteredViews()
{
    // Build set of checked schedule names
    QSet<QString> checkedSchedules;
    for (int i = 0; i < m_scheduleList->count(); ++i) {
        QListWidgetItem *item = m_scheduleList->item(i);
        if (item->checkState() == Qt::Checked) {
            checkedSchedules.insert(item->text());
        }
    }

    // Use the same pre-parsed entries that the Gantt uses
    QList<BScheduleEntry> allEntries = m_scheduleModel->allEntries();
    QList<BScheduleEntry> filtered;
    for (const BScheduleEntry &e : allEntries) {
        if (checkedSchedules.contains(e.scheduleName)) {
            filtered.append(e);
        }
    }

    m_weeklyPlanner->setEntries(filtered);
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
    dialog->setDragResult(entry, newHour, newMinute, entries, dependencies, m_jobScheduleIndex);

    // Forward the sendCommand signal to the main window
    connect(dialog, &BScheduleDragResultDialog::sendCommand,
            this, &BScheduleWidget::sendCommand);

    dialog->show();
}
