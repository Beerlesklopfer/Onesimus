// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "schedules/bschedulewidget.h"
#include "schedules/bstaggerplan.h"
#include "schedules/bstaggerconstraintsdialog.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QFontDatabase>
#include <QPlainTextEdit>
#include <memory>
#include <QApplication>
#include <QTime>
#include "schedules/bschedulelegend.h"
#include <QDateEdit>
#include <QTabWidget>
#include <QSignalBlocker>
#include "schedules/bscheduledragplan.h"
#include "schedules/bschedulegriddialogs.h"
#include "schedules/bschedulereviewdialog.h"
#include "buiscale.h"
#include "jobs/blevelcolors.h"
#include "bsorttableitem.h"
#include "bsortvalue.h"
#include "config/bsettings.h"
#include "bheaderstate.h"
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
#include <QSpinBox>
#include <QSignalBlocker>
#include <QResizeEvent>
#include <QStyleOptionButton>

namespace {

/// A button whose text is cut with "…" when the toolbar has no room for
/// it; it asks for the room of the whole text. The counter of predicted
/// waits: long in some languages, its whole text is its accessible name.
class BElidingButton : public QPushButton
{
public:
    explicit BElidingButton(QWidget *parent) : QPushButton(parent)
    {
        // It shortens its text itself: no minimum of its whole text
        setProperty("buiscaleMayShrink", true);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }

    void setFullText(const QString &text)
    {
        m_full = text;
        updateGeometry();
        elide();
    }

    QSize sizeHint() const override
    {
        // The room of the whole text, whatever is shown now
        const QFontMetrics fm(font());
        return {frameWidth() + fm.horizontalAdvance(m_full), QPushButton::sizeHint().height()};
    }

    QSize minimumSizeHint() const override
    {
        const QFontMetrics fm(font());
        return {qMin(sizeHint().width(), frameWidth() + fm.horizontalAdvance(QLatin1Char('x')) * 12),
                QPushButton::minimumSizeHint().height()};
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QPushButton::resizeEvent(event);
        elide();
    }

    void showEvent(QShowEvent *event) override
    {
        QPushButton::showEvent(event);
        elide();
    }

private:
    void elide()
    {
        // Laid out only once shown: until then the whole text
        const QFontMetrics fm(font());
        const QString shown = isVisible() && width() < sizeHint().width()
            ? fm.elidedText(m_full, Qt::ElideRight, qMax(0, width() - frameWidth()))
            : m_full;
        if (shown != text())
            setText(shown);
    }

    /// What the style puts around the text (padding, frame)
    int frameWidth() const
    {
        QStyleOptionButton option;
        initStyleOption(&option);
        const QFontMetrics fm(font());
        const QSize text(fm.horizontalAdvance(m_full), fm.height());
        option.text = m_full;
        return style()->sizeFromContents(QStyle::CT_PushButton, &option, text, this).width()
             - text.width();
    }

    QString m_full;
};

} // namespace

BScheduleWidget::BScheduleWidget(QWidget *parent)
    : QWidget(parent)
    , m_scheduleModel(new BScheduleModel(this))
    , m_splitter(new QSplitter(Qt::Horizontal, this))
    , m_leftSplitter(new QSplitter(Qt::Vertical, this))
    , m_scheduleList(new QListWidget(this))
    , m_jobListLabel(new QLabel(tr("Jobs:"), this))
    , m_jobList(new QTableWidget(this))
    , m_viewStack(new QStackedWidget(this))
    , m_ganttTabs(new QTabWidget(this))
    , m_fdGanttWidget(new BScheduleGanttWidget(this))
    , m_fdScrollArea(new QScrollArea(this))
    , m_sdGanttWidget(new BScheduleGanttWidget(this))
    , m_sdScrollArea(new QScrollArea(this))
    , m_gridView(new BScheduleGridView(this))
    , m_refreshButton(new QPushButton(tr("Refresh"), this))
    , m_viewModeCombo(new QComboBox(this))
    , m_dayViewCombo(new QComboBox(this))
    , m_todayButton(new QToolButton(this))
    , m_snapCombo(new QComboBox(this))
    , m_waitButton(new BElidingButton(this))
    , m_runProblemLabel(new QLabel(this))
    , m_jobScheduleIndex(new BJobScheduleIndex(this))
    , m_director(nullptr)
    , m_currentDate(QDate::currentDate())
{
    // The job history is asked for a moment after the connect (shown or
    // not), not in the burst of commands of a connect or a refresh
    m_historyTimer = new QTimer(this);
    m_historyTimer->setSingleShot(true);
    m_historyTimer->setInterval(1500);
    connect(m_historyTimer, &QTimer::timeout, this, &BScheduleWidget::requestHistoryIfDue);

    // Push events can come in bursts (a schedule queues many jobs at once):
    // the timelines are repainted at most every LiveRepaintMs
    m_liveTimer = new QTimer(this);
    m_liveTimer->setSingleShot(true);
    m_liveTimer->setInterval(LiveRepaintMs);
    connect(m_liveTimer, &QTimer::timeout, this, &BScheduleWidget::flushLiveOverlay);

    // The wait prediction: the triggers of a burst (the replies of a
    // connect) are worked out once, in a worker thread
    m_waitTimer = new QTimer(this);
    m_waitTimer->setSingleShot(true);
    m_waitTimer->setInterval(0);
    connect(m_waitTimer, &QTimer::timeout, this, &BScheduleWidget::startWaitPrediction);

    setupUI();

    // Writing schedules: the flow and its dialogs. The configuration
    // methods come with setConfigClient().
    m_saveDialogs = new BScheduleSaveDialogs(this);
    m_persistence = new BSchedulePersistence(nullptr, m_saveDialogs, this);
    connect(m_persistence, &BSchedulePersistence::finished,
            this, &BScheduleWidget::onPersistenceFinished);
    connect(m_persistence, &BSchedulePersistence::reloadRequested, this, [this](const QString &) {
        if (m_connected)
            onRefreshClicked();
    });
    connect(m_gridView, &BScheduleGridView::saveRequested, this, &BScheduleWidget::onGridSaveRequested);
    connect(m_gridView, &BScheduleGridView::deleteRequested, this, &BScheduleWidget::onGridDeleteRequested);
    connect(m_gridView, &BScheduleGridView::copyRequested, this, &BScheduleWidget::onGridCopyRequested);
    connect(m_gridView, &BScheduleGridView::renameRequested, this, &BScheduleWidget::onGridRenameRequested);
    // The raw text and its SHA-256 are read when editing begins: a change
    // made on the Director after that is a conflict when saving
    connect(m_gridView, &BScheduleGridView::modifiedChanged, this, [this](bool modified) {
        if (modified && m_configClient
            && m_configClient->capabilities().supports(BConfig::ResourceType::Schedule,
                                                       BConfig::Operation::ReadRaw)) {
            m_persistence->prepare(m_gridView->model()->loadedSchedule().name);
        }
    });
    updateWritable();

    // Lock group modes: FD always by client, SD always by storage
    m_fdGanttWidget->setGroupMode(BScheduleGanttWidget::GroupByClient);
    m_sdGanttWidget->setGroupMode(BScheduleGanttWidget::GroupByStorage);

    updateHistoryViews();
}

BScheduleWidget::~BScheduleWidget()
{
    m_persistence->setUi(nullptr);
    delete m_saveDialogs;
    m_saveDialogs = nullptr;

    // The reader thread posts its result to this object
    if (m_historyParser) {
        m_historyParser->wait();
    }
    // and so does the worker of the wait prediction
    if (m_waitWorker) {
        m_waitWorker->wait();
    }

    BSettings &s = BSettings::instance();
    s.setSchedulesSplitterState(m_splitter->saveState());
    s.setSchedulesGanttTab(m_ganttTabs->currentIndex());
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

    // The date: a calendar, and "Today"
    QString navBtnStyle =
        "QToolButton { color: #333; font-weight: bold; border: 1px solid #aaa;"
        " border-radius: 3px; background: #ddd; padding: 2px 6px; }"
        "QToolButton:hover { background: #bbb; }"
        "QToolButton:disabled { color: #aaa; background: #eee; border-color: #ccc; }";
    m_todayButton->setText(tr("Today"));
    m_todayButton->setStyleSheet(navBtnStyle);
    m_todayButton->setToolTip(tr("Show today or the current week"));

    // A date with a calendar to pick it from (in the week view: a day of
    // the week to show)
    m_dayPicker = new QDateEdit(QDate::currentDate(), this);
    m_dayPicker->setCalendarPopup(true);
    // "ddd" would name the weekday in the locale of the language setting,
    // which can differ from the texts (an untranslated language): the
    // weekday stands beside the field, from the same texts as the rest
    m_dayPicker->setDisplayFormat(QStringLiteral("yyyy-MM-dd"));
    m_dayPicker->setToolTip(tr("The date on display; the calendar opens from the arrow"));
    connect(m_dayPicker, &QDateEdit::dateChanged, this, [this](const QDate &date) {
        setCurrentDate(date);
    });
    m_dayName = new QLabel(this);

    toolbarLayout->addWidget(m_dayName);
    toolbarLayout->addWidget(m_dayPicker);
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
    BUiScale::track(m_zoomSlider, [this]() { m_zoomSlider->setFixedWidth(BUiScale::px(120)); });
    m_zoomSlider->setToolTip(tr("Zoom level (pixels per hour)"));
    m_zoomLabel->setVisible(false);
    m_zoomSlider->setVisible(false);
    toolbarLayout->addWidget(m_zoomLabel);
    toolbarLayout->addWidget(m_zoomSlider);

    toolbarLayout->addStretch();

    // Predicted waits of the dates on display: the counter, and the way
    // to their list (a button, so that the keyboard reaches it). Whether
    // the list is open is said three times: by the arrow in the text, by
    // the filling of the button (the themes' rule for "disclosure") and,
    // for a screen reader, by the name and the checked state.
    m_waitButton->setCheckable(true);
    m_waitButton->setProperty("disclosure", true);
    connect(m_waitButton, &QPushButton::toggled, this, [this](bool on) {
        if (!m_syncingWaitButton)
            showLowerPage(m_waitPanel, on && m_viewStack->currentIndex() == 0);
        updateWaitButtonText();
        BSettings::instance().setSchedulesWaitListShown(on);
        if (on && m_waitButton->hasFocus())
            m_waitTable->setFocus();
    });
    toolbarLayout->addWidget(m_waitButton);
    // Its text stays whole at another font size as well (after the theme
    // was applied again, which sets the width of buttons)
    connect(&BUiScale::instance(), &BUiScale::changed, m_waitButton, [this]() {
        QTimer::singleShot(0, m_waitButton, [this]() {
            m_waitButton->setMinimumWidth(m_waitButton->sizeHint().width());
        });
    });

    // Run lines that were not understood: named here, shown nowhere else
    m_runProblemLabel->setVisible(false);
    m_runProblemLabel->setStyleSheet("color: red; font-weight: bold;");
    toolbarLayout->addWidget(m_runProblemLabel);

    // The runs the job history measured, beside the planned bars of past
    // dates and today
    m_measuredCheck = new QCheckBox(tr("Measured runs"), this);
    m_measuredCheck->setObjectName(QStringLiteral("showMeasuredRuns"));
    m_measuredCheck->setToolTip(tr("Show the runs of past dates and today as the job history measured "
                                   "them, in a thin track below the planned bars"));
    m_measuredCheck->setChecked(BSettings::instance().schedulesShowMeasuredRuns());
    m_fdGanttWidget->setShowMeasured(m_measuredCheck->isChecked());
    m_sdGanttWidget->setShowMeasured(m_measuredCheck->isChecked());
    connect(m_measuredCheck, &QCheckBox::toggled, this, [this](bool on) {
        m_fdGanttWidget->setShowMeasured(on);
        m_sdGanttWidget->setShowMeasured(on);
        BSettings::instance().setSchedulesShowMeasuredRuns(on);
    });
    toolbarLayout->addWidget(m_measuredCheck);

    // What the marks of the timeline mean
    QPushButton *legendButton = new QPushButton(tr("Legend"), this);
    legendButton->setIcon(QIcon::fromTheme("help-contents"));
    legendButton->setToolTip(tr("What the bars, hourglasses, question marks and diamonds of the timeline mean"));
    connect(legendButton, &QPushButton::clicked, this, [legendButton]() {
        (new BScheduleLegend(legendButton))->popupBelow(legendButton);
    });
    toolbarLayout->addWidget(legendButton);

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
    listLabel->setStyleSheet("font-weight: bold;");
    BUiScale::bind(listLabel, BUiScale::Large);
    listLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    scheduleLayout->addWidget(listLabel);
    m_scheduleList->setObjectName(QStringLiteral("scheduleList"));
    m_scheduleList->setAlternatingRowColors(true);
    scheduleLayout->addWidget(m_scheduleList);

    // --- Job pane ---
    QWidget *jobPane = new QWidget(this);
    QVBoxLayout *jobLayout = new QVBoxLayout(jobPane);
    jobLayout->setContentsMargins(0, 0, 0, 0);
    jobLayout->setSpacing(2);
    m_jobListLabel->setStyleSheet("font-weight: bold;");
    jobLayout->addWidget(m_jobListLabel);

    m_jobList->setObjectName(QStringLiteral("scheduleJobList"));
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

    // Clients (FD) and storages (SD) in two tabs: one above the other does
    // not fit on a laptop. Both timelines keep the same dates, zoom and
    // horizontal position.
    m_fdScrollArea->setWidget(m_fdGanttWidget);
    m_fdScrollArea->setWidgetResizable(true);
    m_fdScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_fdScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_sdScrollArea->setWidget(m_sdGanttWidget);
    m_sdScrollArea->setWidgetResizable(true);
    m_sdScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_sdScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_ganttTabs->addTab(m_fdScrollArea, tr("Clients (FD)"));
    m_ganttTabs->addTab(m_sdScrollArea, tr("Storage (SD)"));
    m_ganttTabs->setDocumentMode(true);

    m_viewStack->addWidget(m_ganttTabs);   // index 0 = timelines (FD, SD)

    // The grid: one schedule resource, the current row of the schedule list.
    // Its level colours are those of the settings, as everywhere else.
    m_gridView->setLevelColorSource(&BLevelColors::getLevelColor);
    connect(&BSettings::instance(), &BSettings::appearanceSettingsChanged, m_gridView, [this]() {
        m_gridView->setLevelColorSource(&BLevelColors::getLevelColor);
    });
    m_viewStack->addWidget(m_gridView);          // index 1 = Grid
    // "Discard": the schedule that waited is on display now, with its jobs
    connect(m_gridView, &BScheduleGridView::pendingScheduleShown, this,
            [this]() { updateGridView(true); });
    // "Keep my changes" after another schedule was chosen in the list: the
    // list goes back to the schedule that stays on display
    connect(m_gridView, &BScheduleGridView::changesKept, this, [this](const QString &name) {
        m_gridScheduleName = name;
        const QList<QListWidgetItem *> named = m_scheduleList->findItems(name, Qt::MatchExactly);
        if (!named.isEmpty() && m_scheduleList->currentItem() != named.first()) {
            const QSignalBlocker blocker(m_scheduleList);
            m_scheduleList->setCurrentItem(named.first());
        }
    });

    setupWaitsPage();
    m_viewStack->addWidget(m_waitsPage);         // index 2 = Measured waits

    // Only the view on display counts for the minimum size: the grid asks
    // for almost 500 px, and as a hidden page it kept the handle below the
    // timeline from moving up and pushed the page out of the window
    auto sizeByCurrentView = [this]() {
        for (int i = 0; i < m_viewStack->count(); ++i) {
            const QSizePolicy::Policy policy =
                i == m_viewStack->currentIndex() ? QSizePolicy::Preferred : QSizePolicy::Ignored;
            m_viewStack->widget(i)->setSizePolicy(policy, policy);
        }
    };
    sizeByCurrentView();
    connect(m_viewStack, &QStackedWidget::currentChanged, this, sizeByCurrentView);

    // The views above; below, with a handle between, the predicted waits
    // and the measured history as two tabs: both at once, one above the
    // other, did not fit. Both tabs are always there while the area is
    // open; the counter opens it at the waits, a run clicked at the
    // history, the ✕ beside the tabs closes it.
    m_rightSplitter = new QSplitter(Qt::Vertical, rightWidget);
    m_rightSplitter->setChildrenCollapsible(false);
    m_rightSplitter->addWidget(m_viewStack);

    m_lowerTabs = new QTabWidget(rightWidget);
    m_lowerTabs->setDocumentMode(true);

    // Predicted waits (opened by the counter)
    setupWaitPanel();
    m_lowerTabs->addTab(m_waitPanel, tr("Predicted waits"));

    // Measured history of a run clicked in the timeline; it scrolls when
    // the room below the timeline is small
    setupStatsPanel();
    auto *statsArea = new QScrollArea(rightWidget);
    statsArea->setFrameShape(QFrame::NoFrame);
    statsArea->setWidgetResizable(true);
    statsArea->setWidget(m_statsPanel);
    m_statsPage = statsArea;
    m_lowerTabs->addTab(m_statsPage, tr("Measured history"));

    // Later start times for the jobs that wait: a proposal, nothing is written
    setupStaggerPanel();
    m_lowerTabs->addTab(m_staggerPage, tr("Stagger"));
    m_lowerTabs->setVisible(false);

    auto *closeLower = new QToolButton(m_lowerTabs);
    closeLower->setText(QStringLiteral("\u2715"));    // ✕
    closeLower->setAutoRaise(true);
    closeLower->setToolTip(tr("Close the tabs below the timeline"));
    closeLower->setAccessibleName(tr("Close the area below the timeline"));
    m_lowerTabs->setCornerWidget(closeLower, Qt::TopRightCorner);
    connect(closeLower, &QToolButton::clicked, this, [this]() {
        m_lowerTabs->setVisible(false);
        syncWaitButton();
    });
    // The counter says whether the list of waits is on display
    connect(m_lowerTabs, &QTabWidget::currentChanged, this, [this]() { syncWaitButton(); });

    m_rightSplitter->addWidget(m_lowerTabs);
    // The handle stays where it is left, also over a restart
    connect(m_rightSplitter, &QSplitter::splitterMoved, this, [this]() {
        if (m_lowerTabs->isVisible() && m_rightSplitter->sizes().value(1) > 0)
            BSettings::instance().setSchedulesLowerHeight(m_rightSplitter->sizes().value(1));
    });
    m_rightSplitter->setStretchFactor(0, 1);
    m_rightSplitter->setStretchFactor(1, 0);
    rightLayout->addWidget(m_rightSplitter, 1);

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
    // A click, also on the schedule that is selected already, chooses it:
    // its jobs in the list below, its rows highlighted in the timelines. A
    // second click on it goes back to the jobs of every checked schedule.
    connect(m_scheduleList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        const QString name = item ? item->text() : QString();
        m_focusSchedule = name == m_focusSchedule ? QString() : name;
        onScheduleSelectionChanged();
    });
    connect(m_scheduleList, &QListWidget::itemDoubleClicked,
            this, &BScheduleWidget::onScheduleDoubleClicked);
    connect(m_scheduleList, &QListWidget::itemChanged,
            this, &BScheduleWidget::onScheduleCheckChanged);
    connect(m_jobList, &QTableWidget::itemSelectionChanged,
            this, &BScheduleWidget::onJobSelectionChanged);
    // A click on the row that is selected already answers as well
    connect(m_jobList, &QTableWidget::cellClicked, this, [this]() { onJobSelectionChanged(); });
    connect(m_jobList, &QTableWidget::cellDoubleClicked,
            this, &BScheduleWidget::onJobListDoubleClicked);

    connect(m_viewModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onViewModeChanged);
    connect(m_dayViewCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onDayViewModeChanged);
    connect(m_todayButton, &QToolButton::clicked,
            this, &BScheduleWidget::onDayNavigationToday);
    connect(m_snapCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BScheduleWidget::onSnapChanged);
    connect(m_zoomSlider, &QSlider::valueChanged,
            this, &BScheduleWidget::onZoomSliderChanged);

    // FD Gantt widget signals
    connect(m_fdGanttWidget, &BScheduleGanttWidget::entryClicked,
            this, &BScheduleWidget::onEntryClicked);
    connect(m_fdGanttWidget, &BScheduleGanttWidget::dragDropped,
            this, &BScheduleWidget::onDragDropped);

    // SD Gantt widget signals
    connect(m_sdGanttWidget, &BScheduleGanttWidget::entryClicked,
            this, &BScheduleWidget::onEntryClicked);
    connect(m_sdGanttWidget, &BScheduleGanttWidget::dragDropped,
            this, &BScheduleWidget::onDragDropped);

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

    const int ganttTab = s.schedulesGanttTab();
    if (ganttTab >= 0 && ganttTab < m_ganttTabs->count())
        m_ganttTabs->setCurrentIndex(ganttTab);

    m_fdGanttWidget->setCurrentDate(m_currentDate);
    m_sdGanttWidget->setCurrentDate(m_currentDate);
    updateDateLabel();
    updateDateControls();
}

void BScheduleWidget::setConnectionState(bool connected)
{
    m_connected = connected;
    m_refreshButton->setEnabled(connected);

    if (!connected) {
        clearData();
    } else {
        // Nothing is asked at once: the history follows a moment after the
        // burst of commands of the connect, whether the view is shown or
        // not (the wait prediction waits for it)
        updateHistoryInfo();
        scheduleHistoryRequest();
    }
}

void BScheduleWidget::clearData()
{
    m_scheduleList->clear();
    m_gridScheduleName.clear();
    // Changes that were made in the grid are not thrown away with the
    // connection: the schedule stays on display, and the next one that
    // arrives is offered, not shown
    if (!m_gridView->isModified())
        m_gridView->clear();
    else
        m_gridView->setUsage({}, false);
    m_gridView->model()->clearExpectedLoad();
    m_fdGanttWidget->setEntries({});
    m_sdGanttWidget->setEntries({});
    m_scheduleModel->clear();
    resetHistory();     // the history belongs to the connection
    // and so do the jobs that run now
    m_liveOverlay.clear();
    flushLiveOverlay();
    m_limits = BWaitLimits();   // and so do the limits
    m_directorLimitAsked = false;
    updateWaitPrediction();
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

void BScheduleWidget::processShowJobDefsResponse(const QString &jsonData)
{
    // On a connect the JobDefs come before the jobs: no index and no
    // prediction of their own, the jobs bring both
    if (!m_jobConfigModel || !m_jobConfigModel->hasConfigs())
        return;
    processShowJobsResponse(jsonData);
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

    // The limits the predicted waits need and nothing else asks for:
    // Maximum Concurrent Jobs of the storages and of the Director. Both
    // commands only read. Without a reply the limits stay unknown.
    emit sendCommand(BDirector::Command::ShowStorages, "");
    m_directorLimitAsked = true;
    emit sendCommand(BDirector::Command::Show, QStringLiteral("directors"));
}

void BScheduleWidget::processShowStoragesResponse(const QString &jsonData)
{
    m_limits.readStorages(jsonData);
    updateWaitPrediction();
}

void BScheduleWidget::processShowDirectorResponse(const QString &jsonData)
{
    // "show <something>" is one command for every kind of resource: only
    // the reply this widget asked for may say that the limit is not known
    if (!m_directorLimitAsked && !jsonData.contains(QLatin1String("\"directors\"")))
        return;
    m_directorLimitAsked = false;
    m_limits.readDirector(jsonData);
    updateWaitPrediction();
}

void BScheduleWidget::processShowClientsResponse(const QString &jsonData)
{
    m_limits.readClients(jsonData);
    updateWaitPrediction();
}

void BScheduleWidget::onScheduleSelectionChanged()
{
    updateJobList();
    // The rows of the chosen schedule stand out in the timelines, and the
    // first of them is scrolled into view
    const QString name = m_scheduleList->findItems(m_focusSchedule, Qt::MatchExactly).isEmpty()
                             ? QString() : m_focusSchedule;
    for (auto [timeline, area] : {std::pair{m_fdGanttWidget, m_fdScrollArea},
                                  std::pair{m_sdGanttWidget, m_sdScrollArea}}) {
        timeline->setHighlightedSchedule(name);
        const int top = timeline->highlightedRowTop();
        if (top >= 0)
            area->ensureVisible(area->horizontalScrollBar()->value(), top, 0, timeline->height() / 4);
    }
}

void BScheduleWidget::updateJobList()
{
    m_jobList->setRowCount(0);

    // The schedule chosen in the list; without one, every checked one
    // (the same filter as the timelines)
    QStringList checkedNames;
    if (!m_focusSchedule.isEmpty() && !m_scheduleList->findItems(m_focusSchedule, Qt::MatchExactly).isEmpty()) {
        checkedNames.append(m_focusSchedule);
    } else {
        for (int i = 0; i < m_scheduleList->count(); ++i) {
            QListWidgetItem *item = m_scheduleList->item(i);
            if (item->checkState() == Qt::Checked)
                checkedNames.append(item->text());
        }
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
    m_todayButton->setVisible(isGantt);
    updateDateControls();
    // The predicted waits are those of the dates of the timeline
    m_waitButton->setVisible(isGantt);
    m_measuredCheck->setVisible(isGantt);
    // Waits and history belong to the timeline
    if (!isGantt && m_lowerTabs)
        m_lowerTabs->setVisible(false);
    else
        showLowerPage(m_waitPanel, m_waitButton->isChecked(), false);

    // The figures of a run clicked in the timeline would take the room of
    // the grid

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

    updateDateControls();

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
    if (m_dayPicker && m_dayPicker->date() != date) {
        const QSignalBlocker blocker(m_dayPicker);
        m_dayPicker->setDate(date);
    }
    updateDateLabel();

    // Other dates, other runs
    updateGanttEntries();
}

void BScheduleWidget::updateDateControls()
{
    const bool gantt = m_viewModeCombo->currentIndex() == 0;
    const bool week = showsWeek();
    // Day: weekday and date with a calendar. Week: the same calendar, a
    // day picked shows its week; the days stand in the timeline's header.
    if (m_dayPicker) {
        m_dayPicker->setVisible(gantt);
        m_dayName->setVisible(gantt && !week);
        m_dayPicker->setToolTip(week
            ? tr("The week on display: pick any day of it; the calendar opens from the arrow")
            : tr("The date on display; the calendar opens from the arrow"));
        const QSignalBlocker blocker(m_dayPicker);
        m_dayPicker->setDate(m_currentDate);
    }
}

void BScheduleWidget::updateDateLabel()
{
    static const QStringList dayAbbrev = {
        tr("Mon"), tr("Tue"), tr("Wed"), tr("Thu"),
        tr("Fri"), tr("Sat"), tr("Sun")
    };

    if (m_dayName)
        m_dayName->setText(dayAbbrev.value(m_currentDate.dayOfWeek() - 1));  // Qt: 1=Mon
}

void BScheduleWidget::setupStatsPanel()
{
    // Its name is the tab's
    m_statsPanel = new QGroupBox(this);
    m_statsPanel->setObjectName(QStringLiteral("measuredHistory"));
    m_statsPanel->setFlat(true);

    QVBoxLayout *statsLayout = new QVBoxLayout(m_statsPanel);
    statsLayout->setContentsMargins(8, 8, 8, 8);
    statsLayout->setSpacing(4);

    // Title row
    QHBoxLayout *titleRow = new QHBoxLayout();
    m_statsTitle = new QLabel(this);
    // Until a run is clicked
    m_statsTitle->setText(tr("Click a run in the timeline: its measured history appears here."));
    m_statsTitle->setStyleSheet("font-weight: bold;");
    BUiScale::bind(m_statsTitle, BUiScale::Large);
    titleRow->addWidget(m_statsTitle, 1);

    // Closed by the ✕ of its tab
    statsLayout->addLayout(titleRow);

    // --- Job config info row ---
    QHBoxLayout *configRow = new QHBoxLayout();
    configRow->setSpacing(20);

    auto addConfigBox = [&](const QString &label) -> QLabel* {
        QVBoxLayout *box = new QVBoxLayout();
        box->setSpacing(0);
        QLabel *hdr = new QLabel(label, this);
        hdr->setStyleSheet("color: gray;");
        BUiScale::bind(hdr, BUiScale::Small);
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
        headerLabel->setStyleSheet("color: gray;");
        BUiScale::bind(headerLabel, BUiScale::Small);
        headerLabel->setAlignment(Qt::AlignCenter);
        QLabel *valueLabel = new QLabel("-", this);
        valueLabel->setStyleSheet("font-weight: bold;");
        BUiScale::bind(valueLabel, BUiScale::Large);
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
    BUiScale::track(m_statsRunsTable, [this]() { m_statsRunsTable->setMaximumHeight(BUiScale::px(140)); });
    m_statsRunsTable->setAlternatingRowColors(true);
    statsLayout->addWidget(m_statsRunsTable);
}

void BScheduleWidget::onEntryClicked(const BScheduleEntry &entry)
{
    updateStatsPanel(entry);
    showLowerPage(m_statsPage, true);
}

void BScheduleWidget::showLowerPage(QWidget *page, bool on, bool bringToFront)
{
    if (!m_lowerTabs || !page || m_lowerTabs->indexOf(page) < 0)
        return;
    if (on) {
        const bool opened = !m_lowerTabs->isVisible();
        if (bringToFront || opened)
            m_lowerTabs->setCurrentWidget(page);
        m_lowerTabs->setVisible(true);
        if (opened)
            fitLowerPanels();
    } else if (m_lowerTabs->isVisible() && m_lowerTabs->currentWidget() == page) {
        // Closing what is on display closes the area; a tab behind
        // another one stays as it is
        m_lowerTabs->setVisible(false);
    }
    syncWaitButton();
}

void BScheduleWidget::setSectionVisible(QWidget *section, const QString &name, bool on)
{
    section->setVisible(on);
    if (QLabel *head = findChild<QLabel *>(QStringLiteral("heading:") + name))
        head->setVisible(on);
}

void BScheduleWidget::syncWaitButton()
{
    if (!m_lowerTabs)
        return;
    const bool shown = m_lowerTabs->isVisible() && m_lowerTabs->currentWidget() == m_waitPanel;
    if (m_waitButton->isChecked() == shown)
        return;
    m_syncingWaitButton = true;
    m_waitButton->setChecked(shown);
    m_syncingWaitButton = false;
}

void BScheduleWidget::fitLowerPanels()
{
    // After the show has reached the splitter: its sizes count the panel
    QTimer::singleShot(0, this, [this]() {
        if (!m_rightSplitter)
            return;
        QList<int> sizes = m_rightSplitter->sizes();
        int total = 0;
        for (int s : sizes)
            total += s;
        if (total <= 0)
            return;
        int lower = 0;
        for (int i = 1; i < sizes.size(); ++i) {
            QWidget *panel = m_rightSplitter->widget(i);
            if (panel->isHidden()) {
                sizes[i] = 0;
                continue;
            }
            // Where the user left the handle; else what the panel asks for
            const int stored = BSettings::instance().schedulesLowerHeight();
            sizes[i] = stored > 0 ? stored : qMax(sizes[i], panel->sizeHint().height());
            lower += sizes[i];
        }
        // The views keep a few rows; without a stored height at least half
        const int viewMin = BSettings::instance().schedulesLowerHeight() > 0
            ? BUiScale::px(120) : qMax(BUiScale::px(160), total / 2);
        if (total - lower < viewMin && lower > 0) {
            // Not room for all: the panels share what is left
            const int room = qMax(0, total - viewMin);
            for (int i = 1; i < sizes.size(); ++i)
                sizes[i] = sizes[i] * room / lower;
            lower = room;
        }
        sizes[0] = total - lower;
        m_rightSplitter->setSizes(sizes);
    });
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

    // What waits among them
    updateWaitPrediction();

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

void BScheduleWidget::updateGridLoad()
{
    // The bar at the bottom of each cell of the grid: how many runs of all
    // enabled schedules start in that hour of the weekday, whichever
    // schedule is on display. Unknown while the jobs are not known: a
    // schedule starts runs only for the jobs that use it.
    BScheduleGridModel *model = m_gridView->model();
    if (!m_jobScheduleIndex->isValid()) {
        model->clearExpectedLoad();
        return;
    }
    QList<BSchedule> enabled;
    for (const BSchedule &schedule : m_scheduleModel->schedules()) {
        if (schedule.enabled) enabled.append(schedule);
    }
    // Whole weeks from today, so that every weekday is counted as often;
    // a run on the first Sunday of the month counts a fraction
    const QDate first = QDate::currentDate();
    const QList<BScheduleEntry> entries = m_jobScheduleIndex->entriesFor(
        enabled, first.startOfDay(), first.addDays(7 * GridLoadWeeks).startOfDay());
    const BJobScheduleIndex::WeekLoad load =
        BJobScheduleIndex::startsPerWeekdayHour(entries, first, GridLoadWeeks);
    for (int weekday = 0; weekday < 7; ++weekday) {
        for (int hour = 0; hour < 24; ++hour)
            model->setExpectedLoad(weekday, hour, load[size_t(weekday)][size_t(hour)]);
    }
}

void BScheduleWidget::updateGridView(bool namesOnly)
{
    updateGridLoad();

    // The grid shows one schedule resource: the current row of the list.
    // The list is rebuilt on every refresh, so the choice is kept by name;
    // without one the first schedule is taken.
    QListWidgetItem *current = m_scheduleList->currentItem();
    if (!current && m_scheduleList->count() > 0) {
        // After a new connection: a schedule with changes that stayed on
        // display is the choice again, if the Director has one of its name
        if (m_gridScheduleName.isEmpty() && m_gridView->isModified())
            m_gridScheduleName = m_gridView->model()->loadedSchedule().name;
        const QList<QListWidgetItem *> named =
            m_scheduleList->findItems(m_gridScheduleName, Qt::MatchExactly);
        current = (!m_gridScheduleName.isEmpty() && !named.isEmpty()) ? named.first()
                                                                      : m_scheduleList->item(0);
        const QSignalBlocker blocker(m_scheduleList);
        m_scheduleList->setCurrentItem(current);
    }
    if (!current) {
        // Changes that were made stay on display
        if (!m_gridView->isModified())
            m_gridView->clear();
        return;
    }
    m_gridScheduleName = current->text();

    // A refresh that brings the same schedule keeps scope and selection.
    // The list of names (".schedule") comes first on every refresh and may
    // come without Run lines: it does not replace a schedule that is on
    // display, the reply with the Run lines follows.
    const BSchedule schedule = m_scheduleModel->schedule(m_gridScheduleName);
    // Just written from the grid: what the Director reports now is what
    // the grid starts from (the undo history ends with the write)
    if (!namesOnly && !m_gridAwaitingReload.isEmpty() && schedule.name == m_gridAwaitingReload) {
        m_gridAwaitingReload.clear();
        m_gridView->setSchedule(schedule);
    }
    const BSchedule &shown = m_gridView->model()->loadedSchedule();
    bool same = m_gridView->hasSchedule() && schedule.name == shown.name;
    if (same && !namesOnly) {
        same = schedule.description == shown.description && schedule.enabled == shown.enabled
            && schedule.runs.size() == shown.runs.size();
        for (int i = 0; same && i < schedule.runs.size(); ++i) {
            same = schedule.runs[i].original == shown.runs[i].original;
        }
    }
    // Offered, not set: a schedule with changes that are not written stays
    // on display, and the view asks whether to keep or to discard them. A
    // reply that reports again what the changes started from takes an open
    // question back.
    if (!same || (!namesOnly && m_gridView->hasPendingSchedule())) {
        m_gridView->offerSchedule(schedule);
    }

    // The jobs of the schedule on display, which may be the one with changes
    const QString shownName = m_gridView->model()->loadedSchedule().name;
    QStringList jobs;
    for (const BJobScheduleIndex::JobRef &job : m_jobScheduleIndex->jobsForSchedule(shownName)) {
        jobs.append(job.jobName);
    }
    m_gridView->setUsage(jobs, m_jobScheduleIndex->isValid());
}

void BScheduleWidget::onDragDropped(int entryIndex, int newHour, int newMinute)
{
    const auto *source = qobject_cast<BScheduleGanttWidget *>(sender());
    if (!source) return;
    const QList<BScheduleEntry> &entries = source->entries();
    if (entryIndex < 0 || entryIndex >= entries.size()) return;

    // A copy: the notice below runs an event loop, and a refresh may
    // replace the entries meanwhile
    const BScheduleEntry entry = entries[entryIndex];
    auto clock = [](int hour, int minute) {
        return QStringLiteral("%1:%2").arg(hour, 2, 10, QLatin1Char('0'))
                                      .arg(minute, 2, 10, QLatin1Char('0'));
    };

    // The user moved a run of an existing schedule and expects that
    // schedule to change. Whether the Director can do that is asked at the
    // gate, which also tells the user if it cannot; then nothing is sent.
    const bool available = m_scheduleChangeGate && m_scheduleChangeGate(this);
    const bool handled = available && writeScheduleChange(entry, newHour, newMinute);
    if (handled) return;

    // Always, also where the notice is not shown again: the drop did nothing
    emit statusMessageChanged(
        tr("Schedule %1 was not changed: changing a schedule needs the configuration methods of the "
           "gRPC interface of the Onesimus Director (onesimus-dird), which are not available "
           "on this connection. %2 stays at %3 (dropped at %4).")
            .arg(entry.scheduleName, entry.jobName.isEmpty() ? entry.scheduleName : entry.jobName,
                 clock(entry.hour, entry.minute), clock(newHour, newMinute)));
}

QStringList BScheduleWidget::jobsUsing(const QString &scheduleName) const
{
    QStringList jobs;
    for (const BJobScheduleIndex::JobRef &job : m_jobScheduleIndex->jobsForSchedule(scheduleName)) {
        if (!jobs.contains(job.jobName))
            jobs.append(job.jobName);
    }
    return jobs;
}

namespace {
// The dialogs of copy, rename and the move of one job (defined further down)
BScheduleWidget::DragScope askDragScopeDialog(QWidget *parent, const QString &schedule, const QStringList &jobs,
                                              const QString &job, const QString &copyName, const QString &newTime);
} // namespace

bool BScheduleWidget::writeScheduleChange(const BScheduleEntry &entry, int newHour, int newMinute)
{
    if (!m_configClient)
        return false;
    auto clock = [](int hour, int minute) {
        return QStringLiteral("%1:%2").arg(hour, 2, 10, QLatin1Char('0'))
                                      .arg(minute, 2, 10, QLatin1Char('0'));
    };
    const QString name = entry.scheduleName;
    if (m_persistence->isBusy() || m_changeRunning || m_staggerApplying) {
        emit statusMessageChanged(tr("Schedule %1 was not changed: another change is being written.").arg(name));
        return true;
    }

    // The Run lines of the schedule as loaded; only the dragged one changes
    const BSchedule loaded = m_scheduleModel->schedule(name);
    QStringList originals;
    for (const BRunLine &run : loaded.runs)
        originals.append(run.original);
    const BScheduleDragPlan::Plan plan = BScheduleDragPlan::plan(originals, entry, newHour, newMinute);
    if (plan.refused()) {
        QString why;
        switch (plan.reason) {
        case BScheduleDragPlan::Reason::NotUnderstood:
            why = tr("Run line %1 is not understood, so the schedule is locked.").arg(plan.reasonLine + 1);
            break;
        case BScheduleDragPlan::Reason::SeveralHours:
            why = tr("the dragged Run line runs at several hours of the day; change it in the grid.");
            break;
        case BScheduleDragPlan::Reason::DraggedLineNotFound:
        case BScheduleDragPlan::Reason::DraggedLineAmbiguous:
            why = tr("the dragged run could not be matched to exactly one Run line; refresh and try again, "
                     "or change it in the grid.");
            break;
        case BScheduleDragPlan::Reason::NoRunLines:
            why = tr("its Run lines are not loaded yet.");
            break;
        case BScheduleDragPlan::Reason::BadTime:
            why = tr("%1 is not a time of day.").arg(clock(newHour, newMinute));
            break;
        case BScheduleDragPlan::Reason::WriteCheckFailed:
        case BScheduleDragPlan::Reason::None:
            why = tr("the new Run line did not read back as intended (%1).").arg(plan.reasonText);
            break;
        }
        emit statusMessageChanged(tr("Schedule %1 was not changed: %2").arg(name, why));
        return true;
    }

    const int line = plan.draggedLine;

    // A schedule several jobs use: the change would move all of them. Moving
    // only the dragged job is the other, explicit choice: a copy of the
    // schedule with the new time, assigned to that job.
    const QStringList users = jobsUsing(name);
    if (users.size() > 1 && !entry.jobName.isEmpty() && canMoveOneJob()) {
        const QString newTime = clock(newHour, newMinute);
        const QString copyName = BStaggerPlan::scheduleName(name, QTime(newHour, newMinute), scheduleNames());
        const DragScopeQuestion ask = m_dragScopeQuestion ? m_dragScopeQuestion : askDragScopeDialog;
        switch (ask(this, name, users, entry.jobName, copyName, newTime)) {
        case DragScope::Cancel:
            emit statusMessageChanged(tr("Schedule %1 was not changed: the move was cancelled.").arg(name));
            return true;
        case DragScope::OneJob:
            moveOneJob(entry, loaded, line, plan.generatedRuns.value(line), copyName, newTime);
            return true;
        case DragScope::AllJobs:
            break;
        }
    }

    const QString note =tr("Run line %1 of schedule %2 moves from %3 to %4 (dragged in the timeline: %5). "
                            "Only this line is written anew; the other lines stay as they are in the file.")
                             .arg(line + 1).arg(name, clock(entry.hour, entry.minute),
                                  clock(newHour, newMinute),
                                  entry.jobName.isEmpty() ? name : entry.jobName);
    emit statusMessageChanged(tr("The Director checks the change of schedule %1...").arg(name));
    m_persistence->saveRuns(loaded, BSchedulePersistence::linesFromDrag(loaded, line, plan.generatedRuns.value(line)),
                            jobsUsing(name), note);
    return true;
}

void BScheduleWidget::setConfigClient(BConfigClient *client)
{
    if (m_configClient)
        disconnect(m_configClient, nullptr, this, nullptr);
    m_configClient = client;
    m_persistence->setClient(client);
    m_persistence->forget();
    if (client)
        connect(client, &BConfigClient::capabilitiesChanged, this, &BScheduleWidget::updateWritable);
    updateWritable();
}

void BScheduleWidget::setSaveUi(BScheduleSaveUi *ui)
{
    m_persistence->setUi(ui ? ui : m_saveDialogs);
}

void BScheduleWidget::setResourceNames(const QStringList &pools, const QStringList &storages)
{
    BRunOverridesEditor::setResourceNames(pools, storages);
}

void BScheduleWidget::updateWritable()
{
    const BConfig::Capabilities caps = m_configClient ? m_configClient->capabilities() : BConfig::Capabilities();
    m_gridView->setWritable(caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::UpdateRaw),
                            caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::Delete));
    QString renameWhyNot;
    const bool rename = canRename(&renameWhyNot);
    m_gridView->setCopyRenameAvailable(
        caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::CreateRaw),
        tr("Copying a schedule needs the configuration methods of the gRPC interface of the Onesimus Director "
           "(create schedules), which are not available on this connection"),
        rename, renameWhyNot);
    // A text read with another session's rights is not kept
    if (!caps.known)
        m_persistence->forget();
    updateStaggerApply();
}

void BScheduleWidget::onGridSaveRequested()
{
    const BScheduleGridModel *model = m_gridView->model();
    const QString name = model->loadedSchedule().name;
    if (!m_gridView->isModified() || name.isEmpty())
        return;
    if (m_gridView->isLocked()) {
        emit statusMessageChanged(tr("Schedule %1 is locked: it contains a Run line that is not understood.")
                                      .arg(name));
        return;
    }
    // Whether the Director can do it; the gate tells the user if not
    if (!m_scheduleSaveGate || !m_scheduleSaveGate(this) || !m_configClient) {
        emit statusMessageChanged(tr("Schedule %1 was not written: changing a schedule needs the "
                                     "configuration methods of the gRPC interface of the Onesimus "
                                     "Director. Your changes are kept in the grid.").arg(name));
        return;
    }
    if (m_persistence->isBusy()) {
        emit statusMessageChanged(tr("Another change is being written."));
        return;
    }
    QList<BScheduleResource::Line> lines;
    QString error;
    if (!BSchedulePersistence::linesFromModel(*model, &lines, &error)) {
        emit statusMessageChanged(tr("Schedule %1 cannot be written: %2").arg(name, error));
        return;
    }
    emit statusMessageChanged(tr("The Director checks the changes of schedule %1...").arg(name));
    m_gridSaving = name;
    m_persistence->saveRuns(model->loadedSchedule(), lines, jobsUsing(name),
                            tr("The changes made in the grid. Lines that were not touched stay as they "
                               "are in the file."),
                            m_gridView->descriptionChanged() ? std::optional<QString>(m_gridView->description())
                                                             : std::nullopt);
}

void BScheduleWidget::onGridDeleteRequested()
{
    const QString name = m_gridView->model()->loadedSchedule().name;
    if (name.isEmpty())
        return;
    if (!m_scheduleDeleteGate || !m_scheduleDeleteGate(this) || !m_configClient) {
        emit statusMessageChanged(tr("Schedule %1 was not deleted: deleting a schedule needs the "
                                     "configuration methods of the gRPC interface of the Onesimus "
                                     "Director.").arg(name));
        return;
    }
    if (m_persistence->isBusy()) {
        emit statusMessageChanged(tr("Another change is being written."));
        return;
    }
    emit statusMessageChanged(tr("The Director checks whether schedule %1 can be deleted...").arg(name));
    m_persistence->deleteSchedule(name, jobsUsing(name));
}

void BScheduleWidget::onPersistenceFinished(const BScheduleReport &report)
{
    const QString saving = m_gridSaving;
    m_gridSaving.clear();
    if (report.kind == BScheduleReport::Written) {
        // The schedule is read again (reloadRequested); a write from the
        // grid makes that the new state of the grid
        if (report.operation == BScheduleReview::Update && saving == report.scheduleName)
            m_gridAwaitingReload = report.scheduleName;
        QString text = report.title + QLatin1Char('.');
        if (report.reloaded)
            text += QLatin1Char(' ') + tr("The Director reloaded its configuration.");
        if (!report.followups.isEmpty())
            text += QLatin1Char(' ') + tr("Follow-up commands: %1").arg(report.followups.join(QLatin1String("; ")));
        emit statusMessageChanged(text);
        return;
    }
    emit statusMessageChanged(report.title + QLatin1String(". ") + report.text);
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
    if (!m_connected || m_historyTimer->isActive()) return;

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
    // The connection may have gone while the timer ran
    if (!m_connected) return;

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
    // The statistics (medians and quantiles per job and level) are worked
    // out there as well
    const quint64 generation = m_historyGeneration;
    const int days = m_historyDays;
    const QDateTime now = BJobHistoryStats::wallClockNow();
    QThread *reader = QThread::create([this, reply, generation, days, now]() {
        HistoryResult result;
        result.reply = BJobHistoryReply::parse(reply);
        if (result.reply.outcome == BJobHistoryReply::Outcome::Runs)
            result.stats.setRuns(result.reply.runs, days, now);
        QMetaObject::invokeMethod(this, [this, result, generation]() {
            applyHistoryReply(result, generation);
        }, Qt::QueuedConnection);
    });
    connect(reader, &QThread::finished, reader, &QObject::deleteLater);
    m_historyParser = reader;
    reader->start();
}

void BScheduleWidget::onCommandFailed(BDirector::Command cmd, const QString &args,
                                      const QString &errorMsg)
{
    // The limits of the predicted waits: without a reply they are not known
    if (cmd == BDirector::Command::ShowStorages) {
        m_limits.storages.clear();
        updateWaitPrediction();
        return;
    }
    if (cmd == BDirector::Command::Show && args == QLatin1String("directors")) {
        m_directorLimitAsked = false;
        m_limits.director.reset();
        updateWaitPrediction();
        return;
    }

    if (cmd != BDirector::Command::LlistJobs) return;
    if (m_historyState != HistoryLoading || args != m_historyArgs) return;

    // Not sent, or the connection ended before the reply
    setHistoryUnavailable(tr("the request got no reply (%1)").arg(errorMsg.simplified()));
    updateHistoryViews();
}

// ============================================================================
// Live overlay: jobs that run or wait now (push events, gRPC only)
// ============================================================================

void BScheduleWidget::setLiveOverlayEnabled(bool enabled)
{
    if (m_liveEnabled == enabled) return;
    m_liveEnabled = enabled;
    if (!enabled) {
        m_liveOverlay.clear();
        flushLiveOverlay();
    }
}

void BScheduleWidget::onJobStatusEvent(quint64 jobId, const QString &jobName, const QString &status,
                                       const QDateTime &when, const QDateTime &scheduled)
{
    if (!m_liveEnabled) return;
    // The Director's timestamp of the event; without one (an older
    // transport) its arrival is as near as the GUI gets
    const QDateTime at = when.isValid() ? when : QDateTime::currentDateTime();
    if (m_liveOverlay.statusEvent(jobId, jobName, status, at, scheduled))
        liveOverlayChanged();
}

void BScheduleWidget::onJobMessage(quint64 jobId, const QString &jobName, const QString &text)
{
    if (!m_liveEnabled) return;
    if (m_liveOverlay.jobMessage(jobId, jobName, text, QDateTime::currentDateTime()))
        liveOverlayChanged();
}

void BScheduleWidget::liveOverlayChanged()
{
    if (!m_liveTimer->isActive())
        m_liveTimer->start();
}

void BScheduleWidget::flushLiveOverlay()
{
    m_liveTimer->stop();
    const QList<BLiveJobOverlay::Job> jobs = m_liveOverlay.jobs();
    m_fdGanttWidget->setLiveJobs(jobs);
    m_sdGanttWidget->setLiveJobs(jobs);
}

void BScheduleWidget::applyHistoryReply(const HistoryResult &result, quint64 generation)
{
    const BJobHistoryReply &reply = result.reply;
    // The connection the reply belongs to is gone
    if (generation != m_historyGeneration || m_historyState != HistoryParsing) return;

    switch (reply.outcome) {
    case BJobHistoryReply::Outcome::Runs: {
        // Jobs that were already running or waiting when the connection
        // was made send no event before they end: the history lists them
        if (m_liveEnabled && m_liveOverlay.seed(reply.runs))
            liveOverlayChanged();
        m_historyStats = result.stats;
        m_historyRuns = reply.runs;
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
    m_historyRuns.clear();
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
    m_historyRuns.clear();
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
    m_fdGanttWidget->setMeasuredRuns(m_historyRuns);
    m_sdGanttWidget->setMeasuredRuns(m_historyRuns);
    updateWaitPrediction();     // other durations, other waits
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

// ============================================================================
// Predicted waits of the dates on display
// ============================================================================

namespace {

enum WaitColumn {
    WaitJob = 0, WaitLevel, WaitScheduled, WaitStart, WaitDuration, WaitMeasured, WaitCertain,
    WaitReason, WaitColumnCount
};

/// A measured wait at or above this is listed even if none is predicted
constexpr qint64 kMeasuredListedFromSecs = 30;

} // namespace

void BScheduleWidget::setupWaitPanel()
{
    // Its name is the tab's
    m_waitPanel = new QGroupBox(this);
    m_waitPanel->setObjectName(QStringLiteral("predictedWaits"));
    m_waitPanel->setFlat(true);

    // The table on the left, with the device writers below it; on the
    // right, scrolling on its own, the reason of the selected row, the
    // notes several rows refer to ([1], [2] ...), what the prediction is
    // based on and the back-test. A handle between them.
    QVBoxLayout *layout = new QVBoxLayout(m_waitPanel);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(4);
    auto *waitSplitter = new QSplitter(Qt::Horizontal, m_waitPanel);
    waitSplitter->setObjectName(QStringLiteral("waitSplitter"));
    waitSplitter->setChildrenCollapsible(false);
    layout->addWidget(waitSplitter);

    QWidget *tableSide = new QWidget(waitSplitter);
    auto *tableLayout = new QVBoxLayout(tableSide);
    tableLayout->setContentsMargins(0, 0, 0, 0);
    tableLayout->setSpacing(4);

    QWidget *notes = new QWidget;
    auto *notesLayout = new QVBoxLayout(notes);
    notesLayout->setContentsMargins(0, 0, 4, 0);
    BUiScale::track(notes, [notesLayout]() { notesLayout->setSpacing(BUiScale::px(8)); });
    auto *notesArea = new QScrollArea(waitSplitter);
    notesArea->setFrameShape(QFrame::NoFrame);
    notesArea->setWidgetResizable(true);
    notesArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    notesArea->setWidget(notes);
    waitSplitter->setStretchFactor(0, 3);
    waitSplitter->setStretchFactor(1, 2);
    // Scaled to the width: 60 : 40, or the share the user left
    const int share = BSettings::instance().schedulesWaitSplit();
    if (share > 0 && share < 1000)
        waitSplitter->setSizes({share * 10, (1000 - share) * 10});
    else
        waitSplitter->setSizes({3000, 2000});
    connect(waitSplitter, &QSplitter::splitterMoved, this, [waitSplitter]() {
        const QList<int> sizes = waitSplitter->sizes();
        const int total = sizes.value(0) + sizes.value(1);
        if (total > 0)
            BSettings::instance().setSchedulesWaitSplit(qBound(1, sizes.value(0) * 1000 / total, 999));
    });
    BUiScale::track(notesArea, [notesArea]() { notesArea->setMinimumWidth(BUiScale::px(240)); });

    // What the prediction is based on, and what it does not know
    m_waitInfoLabel = new QLabel(m_waitPanel);
    m_waitInfoLabel->setWordWrap(true);
    m_waitInfoLabel->setTextFormat(Qt::PlainText);
    m_waitInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // What several rows depend on is said once, here, and the rows name it
    // by its number
    m_waitSharedLabel = new QLabel(m_waitPanel);
    m_waitSharedLabel->setWordWrap(true);
    m_waitSharedLabel->setTextFormat(Qt::PlainText);
    m_waitSharedLabel->setTextInteractionFlags(Qt::TextSelectableByMouse
                                               | Qt::TextSelectableByKeyboard);
    m_waitSharedLabel->setAccessibleName(tr("Reasons shared by several rows"));

    m_waitTable = new QTableWidget(0, WaitColumnCount, m_waitPanel);
    m_waitTable->setHorizontalHeaderLabels({
        tr("Job"), tr("Level"), tr("Scheduled"), tr("Predicted start"),
        tr("Wait"), tr("Measured"), tr("Certain"), tr("Reason")});
    m_waitTable->horizontalHeaderItem(WaitMeasured)->setToolTip(
        tr("What this job waited at this level in the job history (StartTime minus SchedTime): "
           "median / maximum. The catalog's start is set after the connections and Client Run "
           "Before Job, so that time is part of it."));
    m_waitTable->horizontalHeaderItem(WaitDuration)->setToolTip(
        tr("Predicted start minus scheduled time: seconds (\"27 s\"), minutes:seconds "
           "(\"1:35 min\") or, from ten minutes on, hours:minutes (\"0:53 h\")"));
    m_waitTable->horizontalHeaderItem(WaitCertain)->setToolTip(
        tr("No: the prediction depends on a limit the Director did not report or on a "
           "duration that was not measured"));
    m_waitTable->setAccessibleName(tr("Predicted waits of the dates on display"));
    m_waitTable->setAccessibleDescription(
        tr("One row per run that is predicted to wait or whose prediction is uncertain. "
           "The reason of the current row is below the table."));
    m_waitTable->setAlternatingRowColors(true);
    m_waitTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_waitTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_waitTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_waitTable->setTabKeyNavigation(false);    // Tab leaves the table
    m_waitTable->verticalHeader()->setVisible(false);
    m_waitTable->horizontalHeader()->setStretchLastSection(true);
    m_waitTable->setSortingEnabled(true);
    m_waitTable->sortByColumn(WaitDuration, Qt::DescendingOrder);   // longest waits first
    keepColumnWidths(m_waitTable, QStringLiteral("waits"));
    // As high as the panel allows; it scrolls its rows
    BUiScale::track(m_waitTable, [this]() { m_waitTable->setMinimumHeight(BUiScale::px(90)); });
    tableLayout->addWidget(m_waitTable, 1);

    // The reason of the current row in full: the limit with its value or
    // the priority rule, and the runs it waits for
    m_waitDetails = new QLabel(m_waitPanel);
    m_waitDetails->setWordWrap(true);
    m_waitDetails->setTextFormat(Qt::PlainText);
    m_waitDetails->setTextInteractionFlags(Qt::TextSelectableByMouse
                                           | Qt::TextSelectableByKeyboard);
    m_waitDetails->setAccessibleName(tr("Reason of the selected wait"));
    // The right column, in sections with a heading each; a heading goes
    // with its section (setSectionVisible())
    auto heading = [this, notes](const QString &name, const QString &text) {
        QLabel *label = new QLabel(text, notes);
        label->setObjectName(QStringLiteral("heading:") + name);
        BUiScale::bind(label, BUiScale::Normal, BUiScale::Bold);
        return label;
    };
    // First what the reader picked
    notesLayout->addWidget(m_waitDetails);

    // What the Director cannot report: how many jobs the devices of a
    // storage write at a time. One row per storage, filled in
    // updateDevicePanel(); never set without the user.
    m_devicePanel = new QWidget(m_waitPanel);
    QGridLayout *devices = new QGridLayout(m_devicePanel);
    devices->setContentsMargins(0, 0, 0, 0);
    BUiScale::track(m_devicePanel, [devices]() {
        devices->setHorizontalSpacing(BUiScale::px(8));
        devices->setVerticalSpacing(BUiScale::px(2));
    });
    m_devicePanel->setVisible(false);
    notesLayout->addWidget(heading(QStringLiteral("devices"), tr("Parallel writers of the devices")));
    notesLayout->addWidget(m_devicePanel);
    notesLayout->addWidget(heading(QStringLiteral("notes"), tr("Notes")));
    notesLayout->addWidget(m_waitSharedLabel);
    notesLayout->addWidget(heading(QStringLiteral("basis"), tr("Basis of the prediction")));
    notesLayout->addWidget(m_waitInfoLabel);
    notesLayout->addWidget(heading(QStringLiteral("backtest"), tr("Back-test")));
    setSectionVisible(m_devicePanel, QStringLiteral("devices"), false);
    setSectionVisible(m_waitSharedLabel, QStringLiteral("notes"), false);

    // The prediction replayed over the last nights of the history
    m_backtestLabel = new QLabel(m_waitPanel);
    m_backtestLabel->setWordWrap(true);
    m_backtestLabel->setTextFormat(Qt::PlainText);
    m_backtestLabel->setTextInteractionFlags(Qt::TextSelectableByMouse
                                             | Qt::TextSelectableByKeyboard);
    m_backtestLabel->setAccessibleName(tr("Back-test of the prediction"));
    notesLayout->addWidget(m_backtestLabel);
    notesLayout->addStretch(1);

    connect(m_waitTable, &QTableWidget::itemSelectionChanged,
            this, &BScheduleWidget::onWaitRowChanged);

    m_waitButton->setChecked(BSettings::instance().schedulesWaitListShown());
}

// The limit of a hold in a few words, for the arrows of the day view
static QString holdLabel(const BWaitHold &hold)
{
    const QString limit = hold.limitKnown ? QString::number(hold.limit) : QStringLiteral("1?");
    switch (hold.reason) {
    case BWaitHold::Reason::DirectorLimit:
        return BScheduleWidget::tr("Director (%1)").arg(limit);
    case BWaitHold::Reason::StorageLimit:
        return BScheduleWidget::tr("Storage %1 (%2)").arg(hold.resource, limit);
    case BWaitHold::Reason::ClientLimit:
        return BScheduleWidget::tr("Client %1 (%2)").arg(hold.resource, limit);
    case BWaitHold::Reason::JobLimit:
        return BScheduleWidget::tr("Job %1 (%2)").arg(hold.resource, limit);
    case BWaitHold::Reason::Priority:
        return BScheduleWidget::tr("Priority %1").arg(hold.otherPriority);
    case BWaitHold::Reason::QueueOrder:
        return BScheduleWidget::tr("Queue: priority %1 first").arg(hold.otherPriority);
    }
    return QString();
}

void BScheduleWidget::updateWaitPrediction()
{
    if (!m_staggerProposal.shifts.isEmpty())
        clearStagger(tr("The prediction has changed: propose again."));
    // A result still being worked out belongs to the old inputs
    ++m_waitGeneration;
    // The triggers come in bursts (the replies of a connect): one
    // prediction for all of them, after the burst
    m_waitTimer->start();
}

bool BScheduleWidget::waitsWaitForHistory() const
{
    // Without the measured durations every run would be replayed as
    // lasting as long as its job allows: a guess, and an expensive one.
    // Once the history has been fetched, or is known to be unavailable,
    // the prediction goes ahead with what there is.
    return m_connected && !m_historyFetchedAt.isValid()
           && (m_historyState == HistoryNotRequested || m_historyState == HistoryLoading
               || m_historyState == HistoryParsing);
}

BScheduleWidget::WaitInput BScheduleWidget::waitInput() const
{
    WaitInput in;
    in.gate = m_waitWorkerGate;
    in.limits = m_limits;
    const QDate first = m_fdGanttWidget->firstDate();
    in.from = first.startOfDay();
    const QDateTime to = first.addDays(m_fdGanttWidget->dayCount()).startOfDay();

    if (!m_jobConfigModel || !m_jobScheduleIndex->isValid() || waitsWaitForHistory())
        return in;

    in.predict = true;
    in.jobs = m_jobConfigModel->jobConfigsRaw();
    in.jobDefs = m_jobConfigModel->jobDefsConfigsRaw();
    // What the Director cannot report: set by the user, per storage
    in.devices = deviceWriters();
    in.jobInfo = waitJobInfo();
    in.historyRuns = m_historyRuns;
    in.historyStats = m_historyStats;

    // The Director's queue holds the runs of every schedule, checked in
    // the list or not. A schedule that is disabled starts nothing.
    QList<BSchedule> enabled;
    for (const BSchedule &schedule : m_scheduleModel->schedules()) {
        if (schedule.enabled) enabled.append(schedule);
    }
    // With the day before: its runs may still be running or waiting
    in.entries = m_jobScheduleIndex->entriesFor(enabled, in.from.addDays(-1), to);

    // What the timelines show of it: the entries on display
    for (const BScheduleEntry &entry : m_fdGanttWidget->entries())
        in.shownEntries.insert(BScheduleGanttWidget::entryKey(entry));
    return in;
}

BScheduleWidget::WaitOutput BScheduleWidget::computeWaitPrediction(const WaitInput &in)
{
    WaitOutput out;
    out.limits = in.limits;
    if (!in.predict)
        return out;

    const QMap<QString, QJsonObject> &jobs = in.jobs;
    const QMap<QString, QJsonObject> &jobDefs = in.jobDefs;
    out.limits.readJobs(jobs, jobDefs);
    out.limits.devices = in.devices;
    // The history as ground truth: measured waits, start overheads and
    // durations without the wait for the device (BWaitCalibration)
    out.calibration.setRuns(in.historyRuns, in.jobInfo, out.limits.devices);

    for (const BScheduleEntry &entry : in.entries) {
        // A schedule that no enabled job uses starts nothing. (The
        // entries come once per enabled job: BJobScheduleIndex.)
        if (entry.unused || entry.jobName.isEmpty()) continue;

        auto directive = [&](const char *key, bool *known) {
            return BWaitLimits::jobDirective(jobs, jobDefs, entry.jobName,
                                             QLatin1String(key), known);
        };
        bool known = false;

        BWaitRun run;
        run.id = BScheduleGanttWidget::entryKey(entry);
        run.job = entry.jobName;
        run.client = entry.client;
        run.storage = entry.storage;
        // The level of the Run line, else the job's own
        run.level = entry.levelLabel();
        if (run.level.isEmpty())
            run.level = jobs.value(entry.jobName).value(QLatin1String("level")).toString();
        run.priority = entry.priority;

        // Not printed and no JobDefs left to look in: the default, "no"
        const QJsonValue mixed = directive("allowmixedpriority", &known);
        if (known)
            run.allowMixedPriority = mixed.isUndefined() ? std::optional<bool>(false)
                                                         : BWaitLimits::boolValue(mixed);

        // Bareos jobq.cc AcquireResources(): these types take no place
        // at a client or a storage. (The jobs they start to do the
        // copying do use the storage; those are not scheduled runs.)
        const QString type = directive("type", &known).toString().toLower();
        if (type == QLatin1String("copy") || type == QLatin1String("migrate")
            || type == QLatin1String("consolidate")) {
            run.usesClient = false;
            run.usesStorage = false;
        }
        // An Admin job that names no storage has none. For every other
        // job an empty storage means "not known" (it may come from the
        // Pool): the model then treats the storage limit as unknown.
        if (type == QLatin1String("admin") && run.storage.isEmpty())
            run.usesStorage = false;

        const BJobHistoryStats::Estimate estimate = in.historyStats.estimate(entry.jobName, run.level);
        if (estimate.hasHistory()) {
            run.durationBasis = estimate.isMeasured() ? BWaitRun::Duration::Measured
                                                      : BWaitRun::Duration::Estimated;
            run.durationSecs = qMax<qint64>(1, estimate.duration.median);
        }

        // How long the Director lets a run of this job last: the bound
        // of a run that was not measured. The Run line may set its own
        // Max Run Sched Time.
        const BWaitLimits::RunTimeLimits bounds =
            BWaitLimits::runTimeLimits(jobs, jobDefs, entry.jobName, run.level);
        run.maxRunSecs = bounds.maxRunSecs;
        run.maxRunBound = bounds.maxRunBound;
        run.maxRunSchedSecs = bounds.maxRunSchedSecs;
        if (entry.source.spec && entry.source.spec->overrides.maxRunSchedTime) {
            const std::optional<qint64> ofRun = BWaitLimits::timeValue(
                QJsonValue(*entry.source.spec->overrides.maxRunSchedTime));
            if (ofRun && *ofRun > 0) run.maxRunSchedSecs = *ofRun;
        }

        if (!out.calibration.isEmpty())
            out.calibration.apply(&run);

        for (const QDateTime &when : entry.occurrences) {
            run.scheduled = when;
            out.runs.append(run);
        }
    }

    // Runs due at the same moment in the order the history shows them
    // to start: the Director's own order is not known
    BWaitCalibration::orderTies(&out.runs);
    out.results = BScheduleWaitModel::predict(out.runs, out.limits);
    out.backtest = out.calibration.backtest(out.limits, BacktestNights);

    // What the timelines show of it: the entries on display, on their dates
    for (int i = 0; i < out.runs.size() && i < out.results.size(); ++i) {
        const BWaitRun &run = out.runs[i];
        if (run.scheduled < in.from || !in.shownEntries.contains(run.id)) continue;
        out.shown.append(i);

        const BWaitResult &result = out.results[i];
        BScheduleGanttWidget::RunWait wait;
        // Only a held run is drawn as waiting; its start overhead alone is not
        wait.waitSecs = result.waits() ? result.waitSecs : 0;
        wait.certain = result.certain;
        wait.lines = BScheduleWaitModel::describe(i, out.runs, out.results);
        if (!wait.lines.isEmpty() || result.deviceWaitSecs > 0
            || BScheduleWaitModel::discrepancy(run, result)
                   != BScheduleWaitModel::Discrepancy::None) {
            wait.lines << BScheduleWaitModel::describeTiming(i, out.runs, out.results, out.limits);
        }
        // Who it waits for, as arrows in the day view
        QSet<QString> linked;
        auto addLinks = [&](const QList<BWaitHold> &holds, bool possible) {
            for (const BWaitHold &hold : holds) {
                for (int b : hold.blockers) {
                    if (b < 0 || b >= out.runs.size())
                        continue;
                    const QString key = BScheduleGanttWidget::waitKey(out.runs[b].id, out.runs[b].scheduled);
                    if (linked.contains(key))
                        continue;
                    linked.insert(key);
                    BScheduleGanttWidget::RunWait::Link link;
                    link.blockerKey = key;
                    link.freedAt = hold.until;
                    link.possible = possible;
                    link.label = holdLabel(hold);
                    link.priority = hold.reason == BWaitHold::Reason::Priority
                                    || hold.reason == BWaitHold::Reason::QueueOrder;
                    wait.links.append(link);
                }
            }
        };
        if (wait.waitSecs > 0)
            addLinks(result.holds, false);
        addLinks(result.possibleHolds, true);
        out.waits.insert(BScheduleGanttWidget::waitKey(run.id, run.scheduled), wait);
    }
    return out;
}

void BScheduleWidget::startWaitPrediction()
{
    // One worker at a time: the one that runs belongs to older inputs and
    // its result is dropped; the newest inputs follow when it is done
    if (m_waitWorker) {
        m_waitPending = true;
        return;
    }
    m_waitPending = false;

    // The inputs are copied here, in the GUI thread; the replay, the
    // calibration and the back-test run in the worker, and the result
    // comes back as a queued call (as the history reader does it)
    const WaitInput input = waitInput();
    const quint64 generation = m_waitGeneration;
    QThread *worker = QThread::create([this, input, generation]() {
        if (input.gate)
            input.gate();
        const WaitOutput output = computeWaitPrediction(input);
        QMetaObject::invokeMethod(this, [this, output, generation]() {
            applyWaitPrediction(output, generation);
        }, Qt::QueuedConnection);
    });
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    m_waitWorker = worker;
    ++m_waitWorkersStarted;
    worker->start();
}

void BScheduleWidget::applyWaitPrediction(const WaitOutput &output, quint64 generation)
{
    // The worker has posted its last word: it no longer touches this object
    m_waitWorker = nullptr;

    if (generation == m_waitGeneration) {
        // Only what the prediction itself worked out: the limits the
        // Director reported are this widget's (they are the same, or the
        // generation would differ)
        m_limits.jobs = output.limits.jobs;
        m_limits.devices = output.limits.devices;
        m_calibration = output.calibration;
        m_waitRuns = output.runs;
        m_waitResults = output.results;
        m_waitShown = output.shown;
        m_backtest = output.backtest;
        m_fdGanttWidget->setWaits(output.waits);
        m_sdGanttWidget->setWaits(output.waits);
        updateWaitViews();
    } else {
        ++m_waitResultsDropped;
    }

    if (m_waitPending)
        startWaitPrediction();
}

void BScheduleWidget::flushWaitPrediction()
{
    m_waitTimer->stop();
    m_waitPending = false;
    // A worker that still runs gives a result of an older generation
    ++m_waitGeneration;
    QPointer<QThread> running = m_waitWorker;
    applyWaitPrediction(computeWaitPrediction(waitInput()), m_waitGeneration);
    // applyWaitPrediction() forgot the worker; it still must not outlive
    // this object
    m_waitWorker = running;
}

void BScheduleWidget::updateWaitViews()
{
    const bool jobsLoaded = m_jobConfigModel && m_jobScheduleIndex->isValid();
    // The jobs are there, the measured durations are not yet
    const bool forHistory = jobsLoaded && waitsWaitForHistory();
    const bool predicted = jobsLoaded && !forHistory;
    const BWaitSummary sum = BScheduleWaitModel::summary(m_waitResults, m_waitShown);

    // --- The counter ---
    const QString text = predicted ? BScheduleWaitModel::summaryText(sum)
                       : forHistory ? tr("Waiting not predicted yet")
                                    : tr("Waiting not predicted");
    m_waitSummary = text;
    QStringList tip;
    if (forHistory) {
        tip << tr("The job history is being loaded. Without the measured durations every run "
                  "would count as lasting as long as its job allows, and the waits would be a "
                  "guess: they are predicted once the history is in.");
    } else if (!predicted) {
        tip << tr("The jobs of the Director are not loaded: it is not known which job runs "
                  "which schedule.");
    } else {
        tip << tr("Predicted for the dates on display from the limits of the Director "
                  "(Maximum Concurrent Jobs, priorities) and the measured durations.");
        if (sum.uncertain > 0) {
            tip << tr("Uncertain: the prediction depends on a limit the Director did not "
                      "report or on a duration that was not measured.");
        }
    }
    tip << tr("Opens or closes the list of predicted waits below the timeline.");
    m_waitButton->setToolTip(tip.join(QLatin1Char('\n')));
    // Beside the timeline only: the other views do not show these runs
    const bool isTimeline = m_viewStack->currentIndex() == 0;
    m_waitButton->setVisible(isTimeline);
    if (isTimeline && m_waitButton->isChecked())
        showLowerPage(m_waitPanel, true, false);
    // A wait is a finding; runs that only run at the same time are not
    QFont font = m_waitButton->font();
    font.setBold(sum.waiting > 0);
    m_waitButton->setFont(font);
    updateWaitButtonText();     // the text, and the width it needs in this font

    // --- What the prediction used ---
    QStringList info;
    if (forHistory) {
        info << tr("Not predicted yet: the job history is being loaded.");
    } else if (!predicted) {
        info << tr("Not predicted: the jobs of the Director are not loaded.");
    } else {
        info << tr("%1 run(s) on display, replayed with the %2 run(s) of all enabled schedules "
                   "from the day before on.").arg(m_waitShown.size()).arg(m_waitRuns.size());

        // The limits that apply to the runs on display
        QStringList storages, clients, unknown;
        QSet<QString> seenStorages, seenClients, seenJobs;
        for (int i : std::as_const(m_waitShown)) {
            const BWaitRun &run = m_waitRuns[i];
            if (run.usesStorage && run.storage.isEmpty()) {
                const QString text = tr("the storage of job %1 (the job names none)").arg(run.job);
                if (!unknown.contains(text)) unknown << text;
            } else if (run.usesStorage && !seenStorages.contains(run.storage)) {
                seenStorages.insert(run.storage);
                if (m_limits.storages.contains(run.storage))
                    storages << QString("%1 = %2").arg(run.storage).arg(m_limits.storages.value(run.storage));
                else
                    unknown << tr("storage %1").arg(run.storage);
            }
            if (run.usesClient && !run.client.isEmpty() && !seenClients.contains(run.client)) {
                seenClients.insert(run.client);
                if (m_limits.clients.contains(run.client))
                    clients << QString("%1 = %2").arg(run.client).arg(m_limits.clients.value(run.client));
                else
                    unknown << tr("client %1").arg(run.client);
            }
            if (!seenJobs.contains(run.job)) {
                seenJobs.insert(run.job);
                if (!m_limits.jobs.contains(run.job))
                    unknown << tr("job %1").arg(run.job);
            }
        }
        storages.sort();
        clients.sort();
        QStringList limits;
        if (m_limits.director)
            limits << tr("Director = %1").arg(*m_limits.director);
        else
            unknown.prepend(tr("the Director"));
        if (!storages.isEmpty())
            limits << tr("storages: %1").arg(storages.join(QStringLiteral(", ")));
        if (!clients.isEmpty())
            limits << tr("clients: %1").arg(clients.join(QStringLiteral(", ")));
        if (!limits.isEmpty())
            info << tr("Maximum Concurrent Jobs as reported: %1.").arg(limits.join(QStringLiteral("; ")));
        if (!unknown.isEmpty()) {
            info << tr("Maximum Concurrent Jobs not reported by this Director for: %1. "
                       "Runs it matters for are uncertain.").arg(unknown.join(QStringLiteral(", ")));
        }
    }
    m_waitInfoLabel->setText(info.join(QLatin1Char('\n')));

    // --- The list: runs that wait or may wait ---
    QList<int> rows;
    for (int i : std::as_const(m_waitShown)) {
        const BWaitRun &run = m_waitRuns[i];
        const BWaitResult &result = m_waitResults[i];
        // Also what the history says waits, predicted or not
        const bool measuredWaits = run.measuredWait.isValid()
                                   && run.measuredWait.medianSecs >= kMeasuredListedFromSecs;
        const bool off = BScheduleWaitModel::discrepancy(run, result)
                         != BScheduleWaitModel::Discrepancy::None;
        if (result.waits() || !result.certain || measuredWaits || off) rows.append(i);
    }

    // What one unknown makes uncertain is often many rows. It is stated
    // once above the table and the rows name it by its number.
    const QList<BWaitUnknown> shared = BScheduleWaitModel::sharedUnknowns(m_waitResults, rows);
    QStringList sharedLines;
    for (int n = 0; n < shared.size(); ++n) {
        sharedLines << tr("[%1] %2 rows: %3.")
            .arg(n + 1)
            .arg(BScheduleWaitModel::dependingRuns(shared[n], m_waitResults, rows))
            .arg(BScheduleWaitModel::unknownText(shared[n], QDate(), false));
    }
    m_waitSharedLabel->setText(sharedLines.join(QLatin1Char('\n')));
    setSectionVisible(m_waitSharedLabel, QStringLiteral("notes"), !sharedLines.isEmpty());

    m_waitTable->setSortingEnabled(false);
    m_waitTable->clearSpans();
    m_waitTable->clearContents();
    m_waitTable->setRowCount(rows.size());

    // The weekday in the language of the interface (QDateTime::toString()
    // would always write it in English)
    const QString dayFormat = QStringLiteral("ddd yyyy-MM-dd ");
    const QString scheduledDay = showsWeek() ? dayFormat : QString();
    for (int row = 0; row < rows.size(); ++row) {
        const int i = rows[row];
        const BWaitRun &run = m_waitRuns[i];
        const BWaitResult &result = m_waitResults[i];
        const QString details = (BScheduleWaitModel::describe(i, m_waitRuns, m_waitResults)
                                 + BScheduleWaitModel::describeTiming(i, m_waitRuns, m_waitResults,
                                                                      m_limits))
                                    .join(QLatin1Char('\n'));

        QTableWidgetItem *job = new QTableWidgetItem(run.job);
        job->setData(Qt::UserRole, i);
        m_waitTable->setItem(row, WaitJob, job);
        m_waitTable->setItem(row, WaitLevel, new QTableWidgetItem(
            run.level.isEmpty() ? tr("(none)") : run.level));
        m_waitTable->setItem(row, WaitScheduled, new BSortTableItem(
            QLocale().toString(run.scheduled, scheduledDay + QStringLiteral("HH:mm")),
            run.scheduled.toSecsSinceEpoch()));
        // A start on another day than the scheduled one says so. After a
        // wait of seconds the start has seconds: it would read like the
        // scheduled time otherwise.
        const QString startDay = result.start.date() == run.scheduled.date() ? scheduledDay
                                                                             : dayFormat;
        const QString startTime = BScheduleWaitModel::startShowsSeconds(result.waitSecs)
            ? QStringLiteral("HH:mm:ss") : QStringLiteral("HH:mm");
        m_waitTable->setItem(row, WaitStart, new BSortTableItem(
            QLocale().toString(result.start, startDay + startTime), result.start.toSecsSinceEpoch()));
        m_waitTable->setItem(row, WaitDuration, new BSortTableItem(
            result.waits() ? BScheduleWaitModel::formatWait(result.waitSecs) : tr("none"),
            result.waitSecs));
        m_waitTable->setItem(row, WaitMeasured, run.measuredWait.isValid()
            ? new BSortTableItem(QStringLiteral("%1 / %2").arg(
                  BScheduleWaitModel::formatWait(run.measuredWait.medianSecs),
                  BScheduleWaitModel::formatWait(run.measuredWait.maxSecs)),
                  run.measuredWait.medianSecs)
            : new BSortTableItem(tr("not measured"), BSortValue::Missing));
        m_waitTable->setItem(row, WaitCertain, new QTableWidgetItem(
            result.certain ? tr("yes") : tr("no")));
        // A prediction far from what was measured says so first
        QString reason = BScheduleWaitModel::reasonLine(run, result, shared);
        switch (BScheduleWaitModel::discrepancy(run, result)) {
        case BScheduleWaitModel::Discrepancy::TooLow:
            reason = reason.isEmpty() ? tr("Prediction too low")
                                      : tr("Prediction too low; %1").arg(reason);
            break;
        case BScheduleWaitModel::Discrepancy::TooHigh:
            reason = reason.isEmpty() ? tr("Prediction too high")
                                      : tr("Prediction too high; %1").arg(reason);
            break;
        case BScheduleWaitModel::Discrepancy::None:
            if (reason.isEmpty() && result.deviceWaitSecs > 0)
                reason = tr("Waits %1 for the device inside the run")
                             .arg(BScheduleWaitModel::formatWait(result.deviceWaitSecs));
            break;
        }
        m_waitTable->setItem(row, WaitReason, new QTableWidgetItem(reason));

        for (int column = 0; column < WaitColumnCount; ++column) {
            m_waitTable->item(row, column)->setToolTip(details);
        }
    }

    if (rows.isEmpty()) {
        m_waitTable->setRowCount(1);
        QTableWidgetItem *none = new QTableWidgetItem(
            predicted ? tr("No run on display is predicted to wait.")
                      : tr("Not predicted."));
        none->setFlags(Qt::ItemIsEnabled);
        m_waitTable->setItem(0, 0, none);
        m_waitTable->setSpan(0, 0, 1, WaitColumnCount);
    }

    m_waitTable->setSortingEnabled(true);
    fitColumns(m_waitTable);
    // The longest wait is explained at once on the right, unless the
    // reader has picked a row already
    if (!rows.isEmpty() && m_waitTable->currentRow() < 0) {
        const QSignalBlocker blocker(m_waitTable);
        m_waitTable->selectRow(0);
    }
    onWaitRowChanged();

    // --- Parallel writers of the storages, and the back-test ---
    QStringList storages;
    if (predicted) {
        for (int i : std::as_const(m_waitShown)) {
            const BWaitRun &run = m_waitRuns[i];
            if (run.usesStorage && !run.storage.isEmpty() && !storages.contains(run.storage))
                storages << run.storage;
        }
        storages.sort();
    }
    updateDevicePanel(storages);
    m_backtestLabel->setText(backtestText());
    setSectionVisible(m_backtestLabel, QStringLiteral("backtest"), predicted);
}

void BScheduleWidget::updateWaitButtonText()
{
    // An arrow as on a tree: to the right while the list is closed, down
    // while it is open
    const bool open = m_waitButton->isChecked();
    static_cast<BElidingButton *>(m_waitButton)->setFullText(
        (open ? QStringLiteral("\u25BC  ") : QStringLiteral("\u25B6  ")) + m_waitSummary);
    // A screen reader gets words, not the arrow
    m_waitButton->setAccessibleName(
        open ? tr("Predicted waits: %1. The list is open.").arg(m_waitSummary)
             : tr("Predicted waits: %1. The list is closed.").arg(m_waitSummary));
    m_waitButton->setAccessibleDescription(
        tr("Opens or closes the list of predicted waits below the timeline."));
}

void BScheduleWidget::onWaitRowChanged()
{
    const QTableWidgetItem *current = m_waitTable->item(m_waitTable->currentRow(), WaitJob);
    const QVariant index = current ? current->data(Qt::UserRole) : QVariant();
    if (!index.isValid() || index.toInt() < 0 || index.toInt() >= m_waitRuns.size()) {
        // Rows to choose from, or only the line that says there are none
        const bool hasRows = m_waitTable->rowCount() > 0 && m_waitTable->columnSpan(0, 0) == 1;
        m_waitDetails->setText(hasRows ? tr("Select a row to read its reason in full.")
                                       : QString());
        return;
    }

    const int i = index.toInt();
    const BWaitRun &run = m_waitRuns[i];
    QStringList lines;
    lines << tr("%1, scheduled %2:").arg(run.job, QLocale().toString(run.scheduled, QStringLiteral("ddd yyyy-MM-dd HH:mm")));
    lines << BScheduleWaitModel::describe(i, m_waitRuns, m_waitResults);
    lines << BScheduleWaitModel::describeTiming(i, m_waitRuns, m_waitResults, m_limits);
    m_waitDetails->setText(lines.join(QLatin1Char('\n')));
}

// ============================================================================
// What the Director cannot report: the devices of a storage
// ============================================================================

void BScheduleWidget::setDirectorProfileId(const QString &profileId)
{
    if (m_profileId == profileId)
        return;
    m_profileId = profileId;
    m_deviceStorages.clear();   // the rows show the values of another profile
    updateWaitPrediction();
}

QHash<QString, int> BScheduleWidget::deviceWriters() const
{
    QHash<QString, int> writers;
    if (!m_jobConfigModel)
        return writers;
    QSet<QString> storages;
    const QHash<QString, BWaitJobInfo> jobs = waitJobInfo();
    for (const BWaitJobInfo &info : jobs) {
        if (!info.storage.isEmpty()) storages.insert(info.storage);
    }
    for (const QString &storage : std::as_const(m_deviceStorages)) storages.insert(storage);
    const BSettings &settings = BSettings::instance();
    for (const QString &storage : std::as_const(storages)) {
        const int value = settings.schedulesDeviceWriters(m_profileId, storage);
        if (value > 0) writers.insert(storage, value);
    }
    return writers;
}

QHash<QString, BWaitJobInfo> BScheduleWidget::waitJobInfo() const
{
    QHash<QString, BWaitJobInfo> info;
    if (!m_jobConfigModel)
        return info;
    const QMap<QString, QJsonObject> &jobs = m_jobConfigModel->jobConfigsRaw();
    const QMap<QString, QJsonObject> &jobDefs = m_jobConfigModel->jobDefsConfigsRaw();
    for (auto it = jobs.constBegin(); it != jobs.constEnd(); ++it) {
        auto directive = [&](const char *key) {
            return BWaitLimits::jobDirective(jobs, jobDefs, it.key(), QLatin1String(key));
        };
        BWaitJobInfo job;
        job.client = BWaitLimits::resourceName(directive("client"));
        job.storage = BWaitLimits::resourceName(directive("storage"));
        const QJsonValue priority = directive("priority");
        if (priority.isDouble())
            job.priority = priority.toInt(10);
        else if (priority.isString()) {
            bool ok = false;
            const int value = priority.toString().trimmed().toInt(&ok);
            if (ok && value > 0) job.priority = value;
        }
        info.insert(it.key(), job);
    }
    return info;
}

void BScheduleWidget::updateDevicePanel(const QStringList &storages)
{
    QGridLayout *grid = qobject_cast<QGridLayout *>(m_devicePanel->layout());
    if (storages != m_deviceStorages) {
        // Rebuilt only when the storages change: a spin box is never
        // deleted from inside its own signal
        while (QLayoutItem *item = grid->takeAt(0)) {
            if (QWidget *widget = item->widget()) {
                widget->hide();
                widget->deleteLater();
            }
            delete item;
        }
        m_deviceStorages = storages;
        for (int row = 0; row < storages.size(); ++row) {
            const QString &storage = storages[row];
            QLabel *name = new QLabel(tr("Storage %1, parallel writers of its device:").arg(storage),
                                      m_devicePanel);
            QSpinBox *spin = new QSpinBox(m_devicePanel);
            spin->setObjectName(QStringLiteral("deviceWriters:") + storage);
            spin->setRange(0, 1000);
            spin->setSpecialValueText(tr("unknown"));
            spin->setProperty("storage", storage);
            spin->setAccessibleName(tr("Parallel writers of the device of storage %1").arg(storage));
            spin->setToolTip(tr("How many jobs the device(s) of %1 write at a time: Maximum "
                                "Concurrent Jobs of the Device resource in the storage daemon "
                                "(several devices: their sum). The Director cannot read it. "
                                "\"unknown\": runs that depend on it are uncertain.").arg(storage));
            name->setBuddy(spin);
            QLabel *suggestion = new QLabel(m_devicePanel);
            suggestion->setObjectName(QStringLiteral("deviceSuggestion:") + storage);
            suggestion->setWordWrap(true);
            suggestion->setTextFormat(Qt::PlainText);
            suggestion->setTextInteractionFlags(Qt::TextSelectableByMouse);
            QPushButton *use = new QPushButton(m_devicePanel);
            use->setObjectName(QStringLiteral("deviceUse:") + storage);
            use->setProperty("storage", storage);
            // Name, then value and button, then the suggestion in the
            // column's whole width
            name->setWordWrap(true);
            grid->addWidget(name, 3 * row, 0, 1, 3);
            grid->addWidget(spin, 3 * row + 1, 0);
            grid->addWidget(use, 3 * row + 1, 1);
            grid->addWidget(suggestion, 3 * row + 2, 0, 1, 3);
            connect(spin, qOverload<int>(&QSpinBox::valueChanged),
                    this, &BScheduleWidget::onDeviceWritersChanged);
            connect(use, &QPushButton::clicked, this, [this, spin, use]() {
                const int suggested = use->property("suggested").toInt();
                if (suggested > 0) spin->setValue(suggested);   // the user's own click
            });
        }
        grid->setColumnStretch(2, 1);
    }

    const BSettings &settings = BSettings::instance();
    for (const QString &storage : storages) {
        QSpinBox *spin = m_devicePanel->findChild<QSpinBox *>(QStringLiteral("deviceWriters:") + storage);
        QLabel *suggestion = m_devicePanel->findChild<QLabel *>(QStringLiteral("deviceSuggestion:") + storage);
        QPushButton *use = m_devicePanel->findChild<QPushButton *>(QStringLiteral("deviceUse:") + storage);
        if (!spin || !suggestion || !use) continue;
        {
            const QSignalBlocker block(spin);
            spin->setValue(settings.schedulesDeviceWriters(m_profileId, storage));
        }
        const BWaitCalibration::DeviceSuggestion found = m_calibration.suggestDevices(storage);
        QString text;
        if (m_calibration.isEmpty()) {
            text = tr("No job history: no suggestion.");
        } else if (found.suggested > 0) {
            text = tr("History suggests %1.").arg(found.suggested);
        } else {
            text = tr("History gives no suggestion.");
        }
        // The conclusion; what was counted is in the tooltip
        text += QLatin1Char(' ') + found.evidence.value(found.evidence.size() - 1);
        suggestion->setText(text);
        suggestion->setToolTip(found.evidence.join(QLatin1Char('\n')));
        use->setText(found.suggested > 0 ? tr("Use %1").arg(found.suggested) : tr("Use"));
        use->setProperty("suggested", found.suggested);
        use->setEnabled(found.suggested > 0 && spin->value() != found.suggested);
        use->setToolTip(found.evidence.join(QLatin1Char('\n')));
    }
    setSectionVisible(m_devicePanel, QStringLiteral("devices"), !storages.isEmpty());
}

void BScheduleWidget::onDeviceWritersChanged()
{
    const QSpinBox *spin = qobject_cast<const QSpinBox *>(sender());
    if (!spin) return;
    BSettings::instance().setSchedulesDeviceWriters(m_profileId, spin->property("storage").toString(),
                                                    spin->value());
    // Not here directly: this runs inside the spin box's signal
    QMetaObject::invokeMethod(this, &BScheduleWidget::updateWaitPrediction, Qt::QueuedConnection);
}

QString BScheduleWidget::backtestText() const
{
    if (m_calibration.isEmpty())
        return tr("Back-test: no job history to replay.");
    return m_backtest.text();
}

// ============================================================================
// Stagger: later start times for the jobs that wait
// ============================================================================

void BScheduleWidget::setupStaggerPanel()
{
    m_staggerPage = new QWidget;
    m_staggerPage->setObjectName(QStringLiteral("stagger"));
    auto *layout = new QVBoxLayout(m_staggerPage);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    auto *intro = new QLabel(m_staggerPage);
    intro->setWordWrap(true);
    intro->setText(tr("Jobs that are scheduled for the same moment and share a limit (a storage, "
                      "a client, the job slots of the Director, the parallel writers of a device) "
                      "wait for each other. The proposal starts some of them later, on a quarter hour "
                      "(23:15, 23:30 ...), so that they wait less on the dates on display and have "
                      "room to spare. A wait for a higher priority is meant to be and is not counted; "
                      "a job of a lower priority that follows a moved job moves along, so that it "
                      "still runs after it. With \"Constraints...\" a job can be kept where it is "
                      "or kept from starting after a given time. Nothing is changed on the Director."));
    layout->addWidget(intro);

    auto *row = new QHBoxLayout;
    m_staggerButton = new QPushButton(tr("Propose later start times"), m_staggerPage);
    m_staggerButton->setToolTip(tr("Replays the queue with the jobs moved quarter hour by quarter "
                                   "hour and keeps the smallest moves that wait least"));
    connect(m_staggerButton, &QPushButton::clicked, this, &BScheduleWidget::proposeStagger);
    row->addWidget(m_staggerButton, 0, Qt::AlignTop);
    m_staggerConstraintsButton = new QPushButton(tr("Constraints..."), m_staggerPage);
    m_staggerConstraintsButton->setToolTip(tr("Jobs that must not move, or must not start after a "
                                              "given time; kept on this computer for this Director"));
    connect(m_staggerConstraintsButton, &QPushButton::clicked,
            this, &BScheduleWidget::editStaggerConstraints);
    row->addWidget(m_staggerConstraintsButton, 0, Qt::AlignTop);
    m_staggerSummary = new QLabel(m_staggerPage);
    m_staggerSummary->setWordWrap(true);
    m_staggerSummary->setTextInteractionFlags(Qt::TextSelectableByMouse);
    row->addWidget(m_staggerSummary, 1);
    m_staggerApplyButton = new QPushButton(tr("Apply..."), m_staggerPage);
    connect(m_staggerApplyButton, &QPushButton::clicked, this, &BScheduleWidget::applyStagger);
    row->addWidget(m_staggerApplyButton, 0, Qt::AlignTop);
    layout->addLayout(row);

    m_staggerTable = new QTableWidget(0, 8, m_staggerPage);
    m_staggerTable->setHorizontalHeaderLabels({
        tr("Job"), tr("Schedule"), tr("Start now"), tr("Proposed start"), tr("Later by"),
        tr("Wait now"), tr("Wait then"), tr("Note")});
    m_staggerTable->horizontalHeaderItem(5)->setToolTip(
        tr("Time held by a limit or waiting for a device, summed over the job's runs on the "
           "dates on display"));
    m_staggerTable->setAccessibleName(tr("Proposed later start times"));
    m_staggerTable->setAlternatingRowColors(true);
    m_staggerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_staggerTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_staggerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_staggerTable->setTabKeyNavigation(false);
    m_staggerTable->verticalHeader()->setVisible(false);
    m_staggerTable->horizontalHeader()->setStretchLastSection(true);
    keepColumnWidths(m_staggerTable, QStringLiteral("stagger"));
    layout->addWidget(m_staggerTable, 1);

    clearStagger(QString());
}

void BScheduleWidget::keepColumnWidths(QTableWidget *table, const QString &key)
{
    // The widths the user left, as the client table keeps them: restored
    // here, saved whenever the user resizes a column
    QHeaderView *header = table->horizontalHeader();
    const BSettings &settings = BSettings::instance();
    const bool restored = BHeaderState::restore(header, settings.schedulesTableHeaderState(key),
                                                settings.schedulesTableHeaderScale(key));
    table->setProperty("userColumnWidths", restored);
    connect(header, &QHeaderView::sectionResized, this, [table, key](int logicalIndex) {
        // Not when the table sizes its columns to their contents, and not
        // the last column: it takes the width that is left whenever the
        // panel changes its size, which the user did not choose
        QHeaderView *header = table->horizontalHeader();
        if (table->property("autoSizingColumns").toBool()
            || header->visualIndex(logicalIndex) == header->count() - 1)
            return;
        table->setProperty("userColumnWidths", true);
        BSettings::instance().setSchedulesTableHeaderState(key, header->saveState());
        BSettings::instance().setSchedulesTableHeaderScale(key, BHeaderState::currentScale());
    });
}

void BScheduleWidget::fitColumns(QTableWidget *table)
{
    // Widths the user chose stay; until then the columns fit their contents
    if (table->property("userColumnWidths").toBool())
        return;
    table->setProperty("autoSizingColumns", true);
    table->resizeColumnsToContents();
    table->setProperty("autoSizingColumns", false);
}

void BScheduleWidget::clearStagger(const QString &why)
{
    m_staggerProposal = BWaitStagger::Proposal();
    if (!m_staggerTable)
        return;
    m_staggerTable->setRowCount(0);
    m_staggerSummary->setText(why.isEmpty()
        ? tr("No proposal yet.")
        : why);
    updateStaggerApply();
}

void BScheduleWidget::updateStaggerApply()
{
    if (!m_staggerApplyButton)
        return;
    const BConfig::Capabilities caps = m_configClient ? m_configClient->capabilities() : BConfig::Capabilities();
    const bool writable = caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::CreateRaw)
                          && caps.supports(BConfig::ResourceType::Job, BConfig::Operation::ReadRaw)
                          && caps.supports(BConfig::ResourceType::Job, BConfig::Operation::UpdateRaw);
    m_staggerApplyButton->setEnabled(writable && !m_staggerApplying && m_staggerProposal.moved() > 0);
    m_staggerApplyButton->setToolTip(writable
        ? tr("Creates a schedule of its own for every job that starts later and sets the job's "
             "Schedule to it; everything is shown once for confirmation first")
        : tr("Needs the configuration methods of the gRPC interface of the Onesimus Director "
             "(create schedules, read and change jobs), which are not available on this connection"));
}

QHash<QString, int> BScheduleWidget::staggerBounds(QHash<QString, QString> *schedules) const
{
    QHash<QString, int> bounds;
    const QChar sep(0x1F);
    for (const BWaitRun &run : m_waitRuns) {
        if (bounds.contains(run.job))
            continue;
        const QString scheduleName = run.id.section(sep, 0, 0);
        if (schedules)
            schedules->insert(run.job, scheduleName);
        // The latest minute of the day any of its lines starts at: the
        // move must not reach the next day, which would change its days
        int bound = 24 * 60 - 1;
        const BSchedule schedule = m_scheduleModel->schedule(scheduleName);
        if (schedule.runs.isEmpty())
            bound = 0;
        for (const BRunLine &line : schedule.runs) {
            const QList<int> hours = line.spec ? line.spec->hours() : QList<int>();
            if (hours.size() != 1) {
                bound = 0;
                break;
            }
            bound = qMin(bound, 24 * 60 - 1 - (hours.first() * 60 + line.spec->minute));
        }
        bounds.insert(run.job, qMax(0, bound));
    }
    return bounds;
}

void BScheduleWidget::proposeStagger()
{
    if (m_waitRuns.isEmpty()) {
        clearStagger(tr("Nothing to stagger: the waits are not predicted (the jobs or the "
                        "history are not loaded yet)."));
        return;
    }
    QHash<QString, QString> schedules;
    BWaitStagger::Options options;
    options.jobMaxMinutes = staggerBounds(&schedules);
    // On the quarter hour: room for a run that lasts longer than measured
    options.gridMinutes = 15;
    staggerConstraintOptions(&options);
    const QDateTime from = m_fdGanttWidget->firstDate().startOfDay();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_staggerProposal = BWaitStagger::propose(m_waitRuns, m_limits, from, options);
    QApplication::restoreOverrideCursor();
    const BWaitStagger::Proposal &p = m_staggerProposal;

    if (p.totalBeforeSecs == 0) {
        m_staggerSummary->setText(tr("Nothing to stagger: no run on the dates on display waits "
                                     "for a limit or a device."));
    } else if (p.moved() == 0) {
        m_staggerSummary->setText(tr("No later start helps: avoidable wait %1. See the notes "
                                     "of the jobs.").arg(BScheduleWaitModel::formatWait(p.totalBeforeSecs)));
    } else {
        m_staggerSummary->setText(tr("Avoidable wait on the dates on display: now %1, with the "
                                     "proposal %2. %n job(s) start later.", "", p.moved())
                                      .arg(BScheduleWaitModel::formatWait(p.totalBeforeSecs),
                                           p.totalAfterSecs > 0
                                               ? BScheduleWaitModel::formatWait(p.totalAfterSecs)
                                               : tr("none")));
    }

    // The times of a job's runs on display, now and as proposed
    auto times = [&](const QList<BWaitRun> &runs, const QString &job) {
        QList<QTime> found;
        for (int i = 0; i < runs.size() && i < m_waitRuns.size(); ++i) {
            if (runs[i].job != job || m_waitRuns[i].scheduled < from)
                continue;
            const QTime t = runs[i].scheduled.time();
            if (!found.contains(t)) found.append(t);
        }
        std::sort(found.begin(), found.end());
        QStringList text;
        for (const QTime &t : found) text.append(t.toString(QStringLiteral("HH:mm")));
        return text.join(QStringLiteral(", "));
    };

    QList<BWaitStagger::Shift> shown;
    for (const BWaitStagger::Shift &shift : p.shifts) {
        if (shift.minutes > 0 || shift.waitBeforeSecs > 0) shown.append(shift);
    }
    // The moved ones first, then the ones that still wait
    std::stable_sort(shown.begin(), shown.end(), [](const auto &a, const auto &b) {
        return (a.minutes > 0) > (b.minutes > 0);
    });

    m_staggerTable->setSortingEnabled(false);
    m_staggerTable->setRowCount(shown.size());
    for (int row = 0; row < shown.size(); ++row) {
        const BWaitStagger::Shift &shift = shown[row];
        QStringList notes;
        const QString latest = shift.latestStartMinute >= 0
            ? QTime(shift.latestStartMinute / 60, shift.latestStartMinute % 60).toString(QStringLiteral("HH:mm"))
            : QString();
        if (shift.fixed)
            notes << tr("Fixed by constraint");
        else if (shift.minutes == 0 && shift.boundMinutes == 0 && !latest.isEmpty())
            notes << tr("Stays: latest start %1 by constraint").arg(latest);
        else if (shift.minutes == 0 && shift.boundMinutes == 0)
            notes << tr("Stays: a Run line of its schedule runs at several hours, is not understood "
                        "or would move into the next day");
        else {
            if (!shift.follows.isEmpty())
                notes << tr("Moves along to run after %1, which has a higher priority").arg(shift.follows);
            else if (shift.minutes == 0)
                notes << tr("A later start does not help");
            // The constraint held it back: it might have moved further
            if (!latest.isEmpty())
                notes << tr("latest start %1 by constraint").arg(latest);
        }
        const QString note = notes.join(QStringLiteral("; "));

        const QStringList cells = {
            shift.job, schedules.value(shift.job), times(m_waitRuns, shift.job),
            shift.minutes > 0 ? times(p.runs, shift.job) : QString(),
            shift.minutes > 0 ? tr("%n min", "", shift.minutes) : QString(),
            BScheduleWaitModel::formatWait(shift.waitBeforeSecs),
            shift.waitAfterSecs > 0 ? BScheduleWaitModel::formatWait(shift.waitAfterSecs) : tr("none"),
            note};
        for (int column = 0; column < cells.size(); ++column)
            m_staggerTable->setItem(row, column, new QTableWidgetItem(cells[column]));
    }
    fitColumns(m_staggerTable);
    updateStaggerApply();
}

void BScheduleWidget::staggerConstraintOptions(BWaitStagger::Options *options) const
{
    const QHash<QString, BSettings::StaggerConstraint> constraints =
        BSettings::instance().schedulesStaggerConstraints(m_profileId);
    for (auto it = constraints.cbegin(); it != constraints.cend(); ++it) {
        if (it.value().fixed)
            options->fixedJobs.insert(it.key());
        else if (it.value().latestStart.isValid())
            options->latestStartMinutes.insert(
                it.key(), it.value().latestStart.hour() * 60 + it.value().latestStart.minute());
    }
}

void BScheduleWidget::editStaggerConstraints()
{
    // The jobs of the waits on display, in the order of their first run
    QStringList jobs;
    for (const BWaitRun &run : std::as_const(m_waitRuns)) {
        if (!run.job.isEmpty() && !jobs.contains(run.job)) jobs.append(run.job);
    }
    BSettings &settings = BSettings::instance();
    BStaggerConstraintsDialog dialog(jobs, settings.schedulesStaggerConstraints(m_profileId), this);
    if (dialog.exec() != QDialog::Accepted)
        return;
    settings.setSchedulesStaggerConstraints(m_profileId, dialog.constraints());
    // A proposal on display was made without them
    if (!m_staggerProposal.shifts.isEmpty())
        proposeStagger();
}

namespace {

bool confirmStaggerDialog(QWidget *parent, const QString &summary)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(BScheduleWidget::tr("Apply the later start times"));
    auto *layout = new QVBoxLayout(&dialog);
    auto *intro = new QLabel(BScheduleWidget::tr("These resources are written to the Director, in this "
                                                 "order. The Director checks each one; the first that it "
                                                 "refuses stops the rest."), &dialog);
    intro->setWordWrap(true);
    layout->addWidget(intro);
    auto *text = new QPlainTextEdit(summary, &dialog);
    text->setReadOnly(true);
    text->setLineWrapMode(QPlainTextEdit::NoWrap);
    text->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    text->setMinimumSize(BUiScale::px(560), BUiScale::px(320));
    layout->addWidget(text, 1);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, &dialog);
    buttons->addButton(BScheduleWidget::tr("Apply"), QDialogButtonBox::AcceptRole);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    return dialog.exec() == QDialog::Accepted;
}

} // namespace

void BScheduleWidget::applyStagger()
{
    if (m_staggerApplying || m_staggerProposal.moved() == 0 || !m_configClient)
        return;

    // One schedule per base schedule and number of minutes; the jobs of it
    struct Group {
        QString base;
        int minutes = 0;
        QString name;
        QString text;
        QStringList jobs;
    };
    QHash<QString, QString> scheduleOf;
    staggerBounds(&scheduleOf);
    QList<Group> groups;
    QStringList taken;
    for (const BSchedule &schedule : m_scheduleModel->schedules())
        taken.append(schedule.name);
    for (const BWaitStagger::Shift &shift : m_staggerProposal.shifts) {
        if (shift.minutes <= 0) continue;
        const QString base = scheduleOf.value(shift.job);
        auto it = std::find_if(groups.begin(), groups.end(), [&](const Group &g) {
            return g.base == base && g.minutes == shift.minutes;
        });
        if (it != groups.end()) {
            it->jobs.append(shift.job);
            continue;
        }
        const BSchedule schedule = m_scheduleModel->schedule(base);
        Group group;
        group.base = base;
        group.minutes = shift.minutes;
        group.jobs.append(shift.job);
        QStringList lines;
        QString error;
        if (!BStaggerPlan::shiftedRunLines(schedule, shift.minutes, &lines, &error)) {
            emit statusMessageChanged(tr("Nothing was written: %1").arg(error));
            return;
        }
        group.name = BStaggerPlan::scheduleName(base, BStaggerPlan::firstStart(schedule, shift.minutes), taken);
        taken.append(group.name);
        // Configuration text, not interface text: it stays English in the file
        group.text = BStaggerPlan::scheduleText(
            group.name,
            QStringLiteral("Copy of %1, %2 min later (staggered with Onesimus)").arg(base).arg(shift.minutes),
            lines);
        groups.append(group);
    }

    // The jobs' texts as they are on the Director, then everything at once
    // for the confirmation
    QStringList jobs;
    for (const Group &g : groups) jobs << g.jobs;
    m_staggerApplying = true;
    updateStaggerApply();
    emit statusMessageChanged(tr("Reading the jobs that start later..."));

    auto finish = [this](const QString &message, bool reload) {
        m_staggerApplying = false;
        emit statusMessageChanged(message);
        if (reload) {
            clearStagger(message);
            emit sendCommand(BDirector::Command::ShowJobs, QString());
            if (m_connected) onRefreshClicked();
        } else {
            updateStaggerApply();
        }
    };

    readResources(BConfig::ResourceType::Job, jobs,
                  [this, groups, finish](const QHash<QString, BConfig::Reply> &read) {
        // Requests in order: the schedules first, the jobs refer to them
        QList<ConfigStep> steps;
        QString summary;
        for (const Group &g : groups) {
            steps.append({BConfig::Request::create(BConfig::ResourceType::Schedule, g.text, false),
                          tr("schedule %1").arg(g.name)});
            summary += tr("New schedule %1 for %2:").arg(g.name, g.jobs.join(QStringLiteral(", ")))
                       + QLatin1Char('\n') + g.text + QLatin1Char('\n');
        }
        summary += tr("Jobs:") + QLatin1Char('\n');
        for (const Group &g : groups) {
            for (const QString &job : g.jobs) {
                const BConfig::Reply &raw = read.value(job);
                const QString text = BStaggerPlan::jobWithSchedule(raw.text, g.name);
                if (text.isEmpty()) {
                    finish(tr("Nothing was written: the text of job %1 has no closing brace.").arg(job), false);
                    return;
                }
                steps.append({BConfig::Request::update(BConfig::ResourceType::Job, job, raw.sha256, text, false),
                              tr("job %1").arg(job)});
                const QString before = BStaggerPlan::scheduleLine(raw.text);
                summary += QStringLiteral("  %1: %2  ->  %3\n").arg(
                    job, before.isEmpty() ? tr("(Schedule from its JobDefs)") : before,
                    BStaggerPlan::scheduleLine(text));
            }
        }
        const StaggerConfirm confirm = m_staggerConfirm ? m_staggerConfirm : confirmStaggerDialog;
        if (!confirm(this, summary)) {
            finish(tr("Nothing was written: the later start times were not confirmed."), false);
            return;
        }
        writeSteps(steps, tr("Written: %1. The jobs start at their new times from now on."), finish);
    }, finish);
}

// ============================================================================
// Changes of several resources: helpers
// ============================================================================

void BScheduleWidget::readResources(BConfig::ResourceType type, const QStringList &names,
                                    std::function<void(const QHash<QString, BConfig::Reply> &)> next,
                                    ChangeFinish finish)
{
    if (names.isEmpty()) {
        next({});
        return;
    }
    if (!m_configClient) {
        finish(tr("Nothing was written: there is no connection to the configuration methods of the Director."),
               false);
        return;
    }
    struct State {
        QHash<QString, BConfig::Reply> read;
        int pending = 0;
        QString failure;
    };
    auto state = std::make_shared<State>();
    state->pending = names.size();
    for (const QString &name : names) {
        m_configClient->call(BConfig::Request::read(type, name), this,
                             [this, state, type, name, next, finish](const BConfig::Reply &reply) {
            if (reply.ok())
                state->read.insert(name, reply);
            else if (state->failure.isEmpty())
                state->failure = tr("Nothing was written: %1 %2 could not be read (%3).")
                                     .arg(BConfig::typeName(type), name,
                                          reply.message.isEmpty() ? reply.rpcMessage : reply.message);
            if (--state->pending > 0) return;
            if (!state->failure.isEmpty()) {
                finish(state->failure, false);
                return;
            }
            next(state->read);
        });
    }
}

void BScheduleWidget::writeSteps(const QList<ConfigStep> &steps, const QString &doneText, ChangeFinish finish)
{
    if (!m_configClient) {
        finish(tr("Nothing was written: there is no connection to the configuration methods of the Director."),
               false);
        return;
    }
    auto done = std::make_shared<QStringList>();
    auto next = std::make_shared<std::function<void(int)>>();
    // The chain holds itself only through the pending call
    std::weak_ptr<std::function<void(int)>> chain = next;
    *next = [this, steps, doneText, done, chain, finish](int index) {
        const auto next = chain.lock();
        if (!next) return;
        if (index >= steps.size()) {
            finish(QString(doneText).arg(done->join(QStringLiteral(", "))), true);
            return;
        }
        if (!m_configClient) {
            finish(tr("Writing %1 failed: the connection to the configuration methods is gone. Written "
                      "before: %2. Nothing after it was sent.")
                       .arg(steps[index].what, done->isEmpty() ? tr("nothing") : done->join(QStringLiteral(", "))),
                   !done->isEmpty());
            return;
        }
        emit statusMessageChanged(tr("Writing %1...").arg(steps[index].what));
        m_configClient->call(steps[index].request, this,
                             [this, index, steps, done, next, finish](const BConfig::Reply &reply) {
            if (!reply.ok()) {
                QString why = reply.message.isEmpty() ? reply.rpcMessage : reply.message;
                if (reply.problem) why += QStringLiteral(" (%1)").arg(reply.problem->text);
                if (reply.status == BConfig::Status::Conflict)
                    why = tr("it was changed on the Director in the meantime");
                else if (reply.status == BConfig::Status::Exists)
                    why = tr("the name is taken");
                else if (reply.status == BConfig::Status::Referenced) {
                    QStringList who;
                    for (const BConfig::ResourceRef &ref : reply.referencedBy)
                        who.append(BConfig::typeName(ref.type) + QLatin1Char(' ') + ref.name);
                    why = tr("it is still used by %1").arg(who.isEmpty() ? why : who.join(QStringLiteral(", ")));
                }
                if (why.isEmpty())
                    why = BConfig::statusName(reply.status);
                const QString written = done->isEmpty() ? tr("nothing")
                                                        : done->join(QStringLiteral(", "));
                finish(tr("Writing %1 failed: %2. Written before: %3. Nothing after it was sent.")
                           .arg(steps[index].what, why, written), !done->isEmpty());
                return;
            }
            done->append(steps[index].what);
            (*next)(index + 1);
        });
    };
    (*next)(0);
}

namespace {

bool confirmChangeDialog(QWidget *parent, const QString &title, const QString &summary)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(title);
    auto *layout = new QVBoxLayout(&dialog);
    auto *intro = new QLabel(BScheduleWidget::tr("These resources are written to the Director, in this "
                                                 "order. The Director checks each one; the first that it "
                                                 "refuses stops the rest."), &dialog);
    intro->setWordWrap(true);
    layout->addWidget(intro);
    auto *text = new QPlainTextEdit(summary, &dialog);
    text->setReadOnly(true);
    text->setLineWrapMode(QPlainTextEdit::NoWrap);
    text->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    text->setMinimumSize(BUiScale::px(560), BUiScale::px(320));
    layout->addWidget(text, 1);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, &dialog);
    buttons->addButton(BScheduleWidget::tr("Write to the Director"), QDialogButtonBox::AcceptRole);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    return dialog.exec() == QDialog::Accepted;
}

QString askNameDialog(QWidget *parent, const QString &title, const QString &label, const QString &suggestion,
                      const std::function<QString(const QString &)> &problem)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(title);
    auto *layout = new QVBoxLayout(&dialog);
    auto *caption = new QLabel(label, &dialog);
    layout->addWidget(caption);
    auto *edit = new QLineEdit(suggestion, &dialog);
    edit->selectAll();
    caption->setBuddy(edit);
    layout->addWidget(edit);
    auto *error = new QLabel(&dialog);
    error->setTextFormat(Qt::PlainText);
    error->setWordWrap(true);
    layout->addWidget(error);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    // Said while typing: OK only for a name that can be taken
    auto check = [&]() {
        const QString why = problem ? problem(edit->text().trimmed()) : QString();
        error->setText(why);
        error->setVisible(!why.isEmpty());
        buttons->button(QDialogButtonBox::Ok)->setEnabled(why.isEmpty());
    };
    QObject::connect(edit, &QLineEdit::textChanged, &dialog, check);
    check();
    return dialog.exec() == QDialog::Accepted ? edit->text().trimmed() : QString();
}

BScheduleWidget::DragScope askDragScopeDialog(QWidget *parent, const QString &schedule, const QStringList &jobs,
                                              const QString &job, const QString &copyName, const QString &newTime)
{
    QMessageBox box(parent);
    box.setIcon(QMessageBox::Question);
    box.setWindowTitle(BScheduleWidget::tr("Move a run of schedule %1").arg(schedule));
    box.setText(BScheduleWidget::tr("Schedule %1 is used by %n job(s): %2. What should change?", "", jobs.size())
                    .arg(schedule, jobs.join(QStringLiteral(", "))));
    box.setInformativeText(BScheduleWidget::tr("Changing the schedule moves the run for every job that uses it. "
                                               "Moving only %1 creates a copy of the schedule named %2 with the "
                                               "run at %3 and sets the Schedule of %1 to it.")
                               .arg(job, copyName, newTime));
    QPushButton *cancel = box.addButton(QMessageBox::Cancel);
    QPushButton *all = box.addButton(BScheduleWidget::tr("Change schedule %1 for all %n jobs", "", jobs.size())
                                         .arg(schedule), QMessageBox::AcceptRole);
    QPushButton *one = box.addButton(BScheduleWidget::tr("Move only job %1").arg(job), QMessageBox::ActionRole);
    box.setDefaultButton(cancel);
    box.exec();
    if (box.clickedButton() == all) return BScheduleWidget::DragScope::AllJobs;
    if (box.clickedButton() == one) return BScheduleWidget::DragScope::OneJob;
    return BScheduleWidget::DragScope::Cancel;
}

} // namespace

bool BScheduleWidget::confirmChange(const QString &title, const QString &summary)
{
    const ChangeConfirm confirm = m_changeConfirm ? m_changeConfirm : confirmChangeDialog;
    return confirm(this, title, summary);
}

QString BScheduleWidget::copyText(const QString &name, const QString &description, const QStringList &runs,
                                  bool enabled)
{
    // A disabled schedule stays disabled in its copy: otherwise the copy
    // would start jobs the original does not
    return BScheduleResource::createText(name, description, enabled, runs);
}

QStringList BScheduleWidget::scheduleNames() const
{
    QStringList names;
    for (const BSchedule &schedule : m_scheduleModel->schedules())
        names.append(schedule.name);
    return names;
}

QString BScheduleWidget::askScheduleName(const QString &title, const QString &label, const QString &suggestion)
{
    const QStringList taken = scheduleNames();
    const std::function<QString(const QString &)> problem = [taken](const QString &name) {
        return BScheduleResource::nameProblem(name, taken);
    };
    const ScheduleNamePrompt prompt = m_namePrompt ? m_namePrompt : askNameDialog;
    const QString name = prompt(this, title, label, suggestion, problem).trimmed();
    if (name.isEmpty())
        return QString();
    const QString why = problem(name);
    if (!why.isEmpty()) {
        emit statusMessageChanged(tr("Nothing was written: %1").arg(why));
        return QString();
    }
    return name;
}

bool BScheduleWidget::canRename(QString *whyNot) const
{
    const BConfig::Capabilities caps = m_configClient ? m_configClient->capabilities() : BConfig::Capabilities();
    // The Director has no rename of its own (RenameResource is not
    // implemented in onesimus-dird and not in BConfigClient): a rename is a
    // copy, the jobs changed to it, and the old schedule deleted
    const bool can = caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::ReadRaw)
                     && caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::CreateRaw)
                     && caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::Delete)
                     && caps.supports(BConfig::ResourceType::Job, BConfig::Operation::ReadRaw)
                     && caps.supports(BConfig::ResourceType::Job, BConfig::Operation::UpdateRaw);
    if (!can && whyNot)
        *whyNot = tr("Renaming a schedule needs the configuration methods of the gRPC interface of the Onesimus "
                     "Director (read, create and delete schedules, read and change jobs), which are not "
                     "available on this connection");
    return can;
}

bool BScheduleWidget::canMoveOneJob() const
{
    const BConfig::Capabilities caps = m_configClient ? m_configClient->capabilities() : BConfig::Capabilities();
    return caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::CreateRaw)
           && caps.supports(BConfig::ResourceType::Job, BConfig::Operation::ReadRaw)
           && caps.supports(BConfig::ResourceType::Job, BConfig::Operation::UpdateRaw);
}

// ============================================================================
// Copy, rename, move of one job
// ============================================================================

void BScheduleWidget::onGridCopyRequested()
{
    const BSchedule loaded = m_gridView->model()->loadedSchedule();
    if (loaded.name.isEmpty())
        return;
    const BConfig::Capabilities caps = m_configClient ? m_configClient->capabilities() : BConfig::Capabilities();
    if (!caps.supports(BConfig::ResourceType::Schedule, BConfig::Operation::CreateRaw)) {
        emit statusMessageChanged(tr("Schedule %1 was not copied: creating a schedule needs the configuration "
                                     "methods of the gRPC interface of the Onesimus Director.").arg(loaded.name));
        return;
    }
    if (m_persistence->isBusy() || m_changeRunning || m_staggerApplying) {
        emit statusMessageChanged(tr("Another change is being written."));
        return;
    }
    if (loaded.runs.isEmpty()) {
        emit statusMessageChanged(tr("Schedule %1 was not copied: its Run lines are not loaded.").arg(loaded.name));
        return;
    }
    QString suggestion = loaded.name + QStringLiteral("-copy");
    const QStringList taken = scheduleNames();
    for (int n = 2; !BScheduleResource::nameProblem(suggestion, taken).isEmpty() && n < 1000; ++n)
        suggestion = loaded.name + QStringLiteral("-copy-") + QString::number(n);
    const QString name = askScheduleName(tr("Copy schedule %1").arg(loaded.name),
                                         tr("Name of the new schedule:"), suggestion);
    if (name.isEmpty())
        return;
    // The Run lines as the Director reported them, each as it was written
    QStringList runs;
    for (const BRunLine &run : loaded.runs)
        runs.append(run.original);
    emit statusMessageChanged(tr("The Director checks the copy %1 of schedule %2...").arg(name, loaded.name));
    m_persistence->createSchedule(name, copyText(name, loaded.description, runs, loaded.enabled));
}

void BScheduleWidget::onGridRenameRequested()
{
    const BSchedule loaded = m_gridView->model()->loadedSchedule();
    const QString old = loaded.name;
    if (old.isEmpty())
        return;
    QString whyNot;
    if (!canRename(&whyNot)) {
        emit statusMessageChanged(tr("Schedule %1 was not renamed: %2.").arg(old, whyNot));
        return;
    }
    if (m_gridView->isModified()) {
        emit statusMessageChanged(tr("Schedule %1 was not renamed: save or revert its changes first.").arg(old));
        return;
    }
    if (m_persistence->isBusy() || m_changeRunning || m_staggerApplying) {
        emit statusMessageChanged(tr("Another change is being written."));
        return;
    }
    const QString name = askScheduleName(tr("Rename schedule %1").arg(old), tr("New name:"), old);
    if (name.isEmpty())
        return;
    QStringList jobs = jobsUsing(old);
    jobs.sort();   // in a stable order in the summary and the status line

    m_changeRunning = true;
    ChangeFinish finish = [this](const QString &message, bool written) {
        m_changeRunning = false;
        emit statusMessageChanged(message);
        if (written) {
            emit sendCommand(BDirector::Command::ShowJobs, QString());
            if (m_connected) onRefreshClicked();
        }
    };
    emit statusMessageChanged(tr("Reading schedule %1 and the jobs that use it...").arg(old));
    readResources(BConfig::ResourceType::Schedule, {old},
                  [this, loaded, old, name, jobs, finish](const QHash<QString, BConfig::Reply> &schedules) {
        const BConfig::Reply raw = schedules.value(old);
        // The copy is the file's text under the new name: its comments and
        // every directive stay. It has to say what the GUI loaded.
        const BScheduleResource::Parsed parsed = BScheduleResource::parse(raw.text);
        QString error;
        if (!parsed.ok || !BScheduleResource::matchesLoaded(parsed, loaded.runs, &error)) {
            finish(tr("Nothing was written: the file of schedule %1 does not say what was loaded (%2). "
                      "Refresh and try again.").arg(old, parsed.ok ? error : parsed.error), false);
            return;
        }
        const QString newText = BScheduleResource::withName(raw.text, name);
        if (newText.isEmpty()) {
            finish(tr("Nothing was written: the Name of schedule %1 could not be found in its file.").arg(old),
                   false);
            return;
        }
        readResources(BConfig::ResourceType::Job, jobs,
                      [this, old, name, jobs, raw, newText, finish](const QHash<QString, BConfig::Reply> &read) {
            QList<ConfigStep> steps;
            steps.append({BConfig::Request::create(BConfig::ResourceType::Schedule, newText, false),
                          tr("schedule %1").arg(name)});
            QString summary = tr("New schedule %1, the text of %2 under the new name:").arg(name, old)
                              + QLatin1Char('\n') + newText + QLatin1Char('\n');
            if (!jobs.isEmpty())
                summary += tr("Jobs:") + QLatin1Char('\n');
            for (const QString &job : jobs) {
                const BConfig::Reply &jobRaw = read.value(job);
                const QString named = BStaggerPlan::scheduleValue(jobRaw.text);
                // A job that takes its schedule from its JobDefs would get a
                // Schedule of its own, and the JobDefs would still name the
                // old schedule: that is for the user to change
                if (named.isEmpty()) {
                    finish(tr("Nothing was written: job %1 takes its Schedule from its JobDefs. Change the "
                              "JobDefs first, or copy the schedule instead.").arg(job), false);
                    return;
                }
                if (named.compare(old, Qt::CaseInsensitive) != 0) {
                    finish(tr("Nothing was written: on the Director job %1 names schedule %2, not %3. Refresh "
                              "and try again.").arg(job, named, old), false);
                    return;
                }
                const QString text = BStaggerPlan::jobWithSchedule(jobRaw.text, name);
                if (text.isEmpty()) {
                    finish(tr("Nothing was written: the text of job %1 has no closing brace.").arg(job), false);
                    return;
                }
                steps.append({BConfig::Request::update(BConfig::ResourceType::Job, job, jobRaw.sha256, text, false),
                              tr("job %1").arg(job)});
                summary += QStringLiteral("  %1: %2  ->  %3\n").arg(job, BStaggerPlan::scheduleLine(jobRaw.text),
                                                                   BStaggerPlan::scheduleLine(text));
            }
            steps.append({BConfig::Request::remove(BConfig::ResourceType::Schedule, old, raw.sha256, false),
                          tr("deletion of schedule %1").arg(old)});
            summary += tr("Then schedule %1 is deleted.").arg(old) + QLatin1Char('\n');
            if (!confirmChange(tr("Rename schedule %1").arg(old), summary)) {
                finish(tr("Nothing was written: the rename of schedule %1 was not confirmed.").arg(old), false);
                return;
            }
            // The grid shows the schedule under its new name after the refresh
            m_gridScheduleName = name;
            writeSteps(steps, tr("Schedule %1 is now named %2. Written: %3.").arg(old, name, QStringLiteral("%1")),
                       finish);
        }, finish);
    }, finish);
}

void BScheduleWidget::moveOneJob(const BScheduleEntry &entry, const BSchedule &loaded, int line,
                                 const QString &newRun, const QString &copyName, const QString &newTime)
{
    const QString job = entry.jobName;
    const QString base = loaded.name;
    QStringList runs;
    for (const BRunLine &run : loaded.runs)
        runs.append(run.original);
    runs[line] = newRun;
    // Configuration text, not interface text: it stays English in the file
    const QString text = copyText(copyName,
                                  QStringLiteral("Copy of %1 for job %2, Run line %3 at %4 (moved with Onesimus)")
                                      .arg(base, job).arg(line + 1).arg(newTime),
                                  runs, loaded.enabled);

    m_changeRunning = true;
    ChangeFinish finish = [this](const QString &message, bool written) {
        m_changeRunning = false;
        emit statusMessageChanged(message);
        if (written) {
            emit sendCommand(BDirector::Command::ShowJobs, QString());
            if (m_connected) onRefreshClicked();
        }
    };
    emit statusMessageChanged(tr("Reading job %1...").arg(job));
    readResources(BConfig::ResourceType::Job, {job},
                  [this, job, base, copyName, newTime, text, line, finish](const QHash<QString, BConfig::Reply> &read) {
        const BConfig::Reply raw = read.value(job);
        const QString named = BStaggerPlan::scheduleValue(raw.text);
        // Empty: the job takes its schedule from its JobDefs and gets one of
        // its own, as for a later start (BStaggerPlan)
        if (!named.isEmpty() && named.compare(base, Qt::CaseInsensitive) != 0) {
            finish(tr("Nothing was written: on the Director job %1 names schedule %2, not %3. Refresh and try "
                      "again.").arg(job, named, base), false);
            return;
        }
        const QString jobText = BStaggerPlan::jobWithSchedule(raw.text, copyName);
        if (jobText.isEmpty()) {
            finish(tr("Nothing was written: the text of job %1 has no closing brace.").arg(job), false);
            return;
        }
        const QList<ConfigStep> steps = {
            {BConfig::Request::create(BConfig::ResourceType::Schedule, text, false), tr("schedule %1").arg(copyName)},
            {BConfig::Request::update(BConfig::ResourceType::Job, job, raw.sha256, jobText, false),
             tr("job %1").arg(job)},
        };
        const QString before = BStaggerPlan::scheduleLine(raw.text);
        const QString summary =
            tr("New schedule %1, a copy of %2 with Run line %3 at %4, for job %5 only:")
                .arg(copyName, base).arg(line + 1).arg(newTime, job)
            + QLatin1Char('\n') + text + QLatin1Char('\n') + tr("Job:") + QLatin1Char('\n')
            + QStringLiteral("  %1: %2  ->  %3\n").arg(job, before.isEmpty() ? tr("(Schedule from its JobDefs)") : before,
                                                       BStaggerPlan::scheduleLine(jobText))
            + tr("Schedule %1 and its other jobs stay as they are.").arg(base) + QLatin1Char('\n');
        if (!confirmChange(tr("Move only job %1").arg(job), summary)) {
            finish(tr("Nothing was written: the move of job %1 was not confirmed.").arg(job), false);
            return;
        }
        writeSteps(steps, tr("Job %1 now runs at %2 on its own schedule %3. Written: %4.")
                              .arg(job, newTime, copyName, QStringLiteral("%1")), finish);
    }, finish);
}
