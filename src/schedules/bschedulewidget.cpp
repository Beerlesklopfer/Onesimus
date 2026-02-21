#include "schedules/bschedulewidget.h"
#include "blogging.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>

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

    rightLayout->addWidget(m_viewStack);

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
