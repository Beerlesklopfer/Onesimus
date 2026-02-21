#include "schedules/bschedulewidget.h"
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
    , m_ganttWidget(new BScheduleGanttWidget(this))
    , m_ganttScrollArea(new QScrollArea(this))
    , m_weeklyPlanner(new BWeeklyPlanner(this))
    , m_refreshButton(new QPushButton(tr("Refresh"), this))
    , m_viewModeCombo(new QComboBox(this))
    , m_dayViewCombo(new QComboBox(this))
    , m_prevDayButton(new QToolButton(this))
    , m_nextDayButton(new QToolButton(this))
    , m_dayLabel(new QLabel(this))
    , m_zoomSlider(new QSlider(Qt::Horizontal, this))
    , m_groupModeCombo(new QComboBox(this))
    , m_collisionLabel(new QLabel(this))
    , m_director(nullptr)
    , m_currentDay(QDate::currentDate().dayOfWeek() - 1)  // Qt: 1=Mon → 0=Mon
{
    setupUI();
    m_ganttWidget->setDurationStats(m_durationStats);
}

BScheduleWidget::~BScheduleWidget()
{
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

    // Group mode
    m_groupModeCombo->addItem(tr("By Schedule"), 0);
    m_groupModeCombo->addItem(tr("By Client"), 1);
    toolbarLayout->addWidget(m_groupModeCombo);

    toolbarLayout->addSpacing(10);

    // Zoom slider
    toolbarLayout->addWidget(new QLabel(tr("Zoom:"), this));
    m_zoomSlider->setRange(BScheduleGanttWidget::MIN_PIXELS_PER_HOUR,
                           BScheduleGanttWidget::MAX_PIXELS_PER_HOUR);
    m_zoomSlider->setValue(BScheduleGanttWidget::DEFAULT_PIXELS_PER_HOUR);
    m_zoomSlider->setMaximumWidth(120);
    toolbarLayout->addWidget(m_zoomSlider);

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

    // Gantt in scroll area
    m_ganttScrollArea->setWidget(m_ganttWidget);
    m_ganttScrollArea->setWidgetResizable(false);
    m_ganttScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_ganttScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_viewStack->addWidget(m_ganttScrollArea);  // index 0 = Gantt
    m_viewStack->addWidget(m_weeklyPlanner);     // index 1 = Grid

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

    connect(m_viewModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onViewModeChanged);
    connect(m_dayViewCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onDayViewModeChanged);
    connect(m_prevDayButton, &QToolButton::clicked,
            this, &BScheduleWidget::onDayNavigationPrev);
    connect(m_nextDayButton, &QToolButton::clicked,
            this, &BScheduleWidget::onDayNavigationNext);
    connect(m_zoomSlider, &QSlider::valueChanged,
            this, &BScheduleWidget::onZoomChanged);
    connect(m_groupModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onGroupModeChanged);
    connect(m_ganttWidget, &BScheduleGanttWidget::collisionsDetected,
            this, &BScheduleWidget::onCollisionsDetected);
    connect(m_ganttWidget, &BScheduleGanttWidget::entryClicked,
            this, &BScheduleWidget::onEntryClicked);

    // Set initial state
    m_viewStack->setCurrentIndex(0);  // Gantt as default
    onDayViewModeChanged(0);          // Day view

    // Update day label
    static const QStringList dayNames = {
        tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
        tr("Friday"), tr("Saturday"), tr("Sunday")
    };
    m_dayLabel->setText(dayNames.value(m_currentDay));
    m_ganttWidget->setCurrentDay(m_currentDay);
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
    m_ganttWidget->setEntries({});
    m_scheduleModel->clear();
    m_durationStats->clear();
    m_collisionLabel->setVisible(false);
}

void BScheduleWidget::processDotScheduleResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
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

    m_scheduleList->clear();

    for (const QJsonValue &scheduleVal : schedulesArray) {
        if (!scheduleVal.isObject()) continue;

        QJsonObject schedule = scheduleVal.toObject();
        QString scheduleName = schedule["name"].toString();

        if (!scheduleName.isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(scheduleName);
            item->setData(Qt::UserRole, schedule);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            m_scheduleList->addItem(item);
        }
    }

    m_refreshButton->setEnabled(m_director != nullptr);
    emit statusMessageChanged(tr("%1 schedules loaded").arg(schedulesArray.size()));

    // Update Gantt with all entries
    updateGanttEntries();
}

void BScheduleWidget::processListJobsResponse(const QString &jsonData)
{
    m_durationStats->feedJobs(jsonData);

    // Re-enrich Gantt entries with updated durations
    updateGanttEntries();
}

void BScheduleWidget::onRefreshClicked()
{
    m_refreshButton->setEnabled(false);
    emit statusMessageChanged(tr("Refreshing schedule list..."));
    emit sendCommand(BDirector::Command::DotSchedule, "");
}

void BScheduleWidget::onScheduleSelectionChanged()
{
    QList<QListWidgetItem*> selectedItems = m_scheduleList->selectedItems();

    if (selectedItems.isEmpty()) {
        m_weeklyPlanner->clearSchedule();
        return;
    }

    QListWidgetItem *item = selectedItems.first();
    QJsonObject schedule = item->data(Qt::UserRole).toJsonObject();

    // Update weekly planner (Grid view)
    m_weeklyPlanner->setSchedule(schedule);

    // In Gantt view, highlight/filter to selected schedule
    // For now, show all but we could filter later
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
    m_zoomSlider->setVisible(isGantt);
    m_groupModeCombo->setVisible(isGantt);
}

void BScheduleWidget::onDayViewModeChanged(int index)
{
    if (index == 0) {
        m_ganttWidget->setViewMode(BScheduleGanttWidget::DayView);
        m_prevDayButton->setEnabled(true);
        m_nextDayButton->setEnabled(true);
    } else {
        m_ganttWidget->setViewMode(BScheduleGanttWidget::WeekView);
        m_prevDayButton->setEnabled(false);
        m_nextDayButton->setEnabled(false);
    }

    // Resize Gantt to fit content
    m_ganttWidget->setMinimumSize(m_ganttWidget->sizeHint());
    m_ganttWidget->resize(m_ganttWidget->sizeHint());
}

void BScheduleWidget::onDayNavigationPrev()
{
    m_currentDay = (m_currentDay + 6) % 7;  // wrap backwards
    static const QStringList dayNames = {
        tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
        tr("Friday"), tr("Saturday"), tr("Sunday")
    };
    m_dayLabel->setText(dayNames.value(m_currentDay));
    m_ganttWidget->setCurrentDay(m_currentDay);
    m_ganttWidget->setMinimumSize(m_ganttWidget->sizeHint());
}

void BScheduleWidget::onDayNavigationNext()
{
    m_currentDay = (m_currentDay + 1) % 7;
    static const QStringList dayNames = {
        tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
        tr("Friday"), tr("Saturday"), tr("Sunday")
    };
    m_dayLabel->setText(dayNames.value(m_currentDay));
    m_ganttWidget->setCurrentDay(m_currentDay);
    m_ganttWidget->setMinimumSize(m_ganttWidget->sizeHint());
}

void BScheduleWidget::onZoomChanged(int value)
{
    m_ganttWidget->setZoomLevel(value);
    m_ganttWidget->setMinimumSize(m_ganttWidget->sizeHint());
    m_ganttWidget->resize(m_ganttWidget->sizeHint());
}

void BScheduleWidget::onGroupModeChanged(int index)
{
    if (index == 0) {
        m_ganttWidget->setGroupMode(BScheduleGanttWidget::GroupBySchedule);
    } else {
        m_ganttWidget->setGroupMode(BScheduleGanttWidget::GroupByClient);
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

    // Look up job name for duration stats
    QString lookupName = entry.jobName;
    if (lookupName.isEmpty()) {
        lookupName = entry.scheduleName;  // fallback
    }

    int minDur = m_durationStats->minDuration(lookupName);
    int avgDur = m_durationStats->averageDuration(lookupName);
    int maxDur = m_durationStats->maxDuration(lookupName);
    int samples = m_durationStats->sampleCount(lookupName);

    m_statsMin->setText(BJobDurationStats::formatDuration(minDur));
    m_statsAvg->setText(BJobDurationStats::formatDuration(avgDur));
    m_statsMax->setText(BJobDurationStats::formatDuration(maxDur));
    m_statsSamples->setText(QString::number(samples));

    // Trend
    double trendVal = m_durationStats->trend(lookupName);
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

    // Last runs table
    QList<BJobDurationStats::JobRunRecord> runs = m_durationStats->lastRuns(lookupName, 5);
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

void BScheduleWidget::updateGanttEntries()
{
    QList<BScheduleEntry> entries = m_scheduleModel->allEntries();

    // Enrich with duration estimates
    for (BScheduleEntry &e : entries) {
        if (e.estimatedDurationSecs <= 0 && !e.jobName.isEmpty()) {
            e.estimatedDurationSecs = m_durationStats->averageDuration(e.jobName);
        }
    }

    m_ganttWidget->setEntries(entries);
    m_ganttWidget->setMinimumSize(m_ganttWidget->sizeHint());
    m_ganttWidget->resize(m_ganttWidget->sizeHint());
}
