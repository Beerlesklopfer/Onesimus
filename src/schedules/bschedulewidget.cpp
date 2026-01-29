#include "schedules/bschedulewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

BScheduleWidget::BScheduleWidget(QWidget *parent)
    : QWidget(parent)
    , m_splitter(new QSplitter(Qt::Horizontal, this))
    , m_scheduleList(new QListWidget(this))
    , m_weeklyPlanner(new BWeeklyPlanner(this))
    , m_refreshButton(new QPushButton("Aktualisieren", this))
    , m_director(nullptr)
{
    setupUI();
}

BScheduleWidget::~BScheduleWidget()
{
}

void BScheduleWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();

    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    m_refreshButton->setEnabled(false);

    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_refreshButton);

    mainLayout->addLayout(toolbarLayout);

    // Splitter with list and planner
    // Left side: Schedule list
    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    QLabel *listLabel = new QLabel("Schedules:", this);
    listLabel->setStyleSheet("font-weight: bold; font-size: 12pt;");
    listLabel->setAlignment(Qt::AlignRight);
    leftLayout->addWidget(listLabel);

    m_scheduleList->setAlternatingRowColors(true);
    leftLayout->addWidget(m_scheduleList);

    // Right side: Weekly planner
    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    QLabel *plannerLabel = new QLabel("Wochenplan:", this);
    plannerLabel->setStyleSheet("font-weight: bold; font-size: 12pt;");
    rightLayout->addWidget(plannerLabel);

    rightLayout->addWidget(m_weeklyPlanner);

    m_splitter->addWidget(leftWidget);
    m_splitter->addWidget(rightWidget);

    // Set initial splitter sizes (30% list, 70% planner)
    m_splitter->setSizes(QList<int>() << 300 << 700);

    mainLayout->addWidget(m_splitter);

    // Connect signals
    connect(m_refreshButton, &QPushButton::clicked,
            this, &BScheduleWidget::onRefreshClicked);
    connect(m_scheduleList, &QListWidget::itemSelectionChanged,
            this, &BScheduleWidget::onScheduleSelectionChanged);
    connect(m_scheduleList, &QListWidget::itemDoubleClicked,
            this, &BScheduleWidget::onScheduleDoubleClicked);
}

void BScheduleWidget::setConnectionState(bool connected)
{
    m_refreshButton->setEnabled(connected);

    if (!connected) {
        clearData();
    }
    // Note: Schedules are automatically requested via MainWindow::onRefreshAll()
    // which is called after connection is established
}

void BScheduleWidget::clearData()
{
#ifdef IS_DEVELOPER
    qDebug() << "BScheduleWidget: Clearing all data";
#endif

    m_scheduleList->clear();
    m_weeklyPlanner->clearSchedule();
}

void BScheduleWidget::processDotScheduleResponse(const QString &jsonData)
{
#ifdef IS_DEVELOPER
    qDebug() << "BScheduleWidget: Processing .schedule response";
#endif

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "BScheduleWidget: Failed to parse .schedule response:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "BScheduleWidget: .schedule response is not a JSON object";
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();
    QJsonArray schedulesArray = result["schedules"].toArray();

    // Clear and populate schedule list
    m_scheduleList->clear();

    for (const QJsonValue &scheduleVal : schedulesArray) {
        if (!scheduleVal.isObject()) {
            continue;
        }

        QJsonObject schedule = scheduleVal.toObject();
        QString scheduleName = schedule["name"].toString();

        if (!scheduleName.isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(scheduleName);
            item->setData(Qt::UserRole, schedule);
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);  // Right-aligned
            m_scheduleList->addItem(item);
        }
    }

#ifdef IS_DEVELOPER
    qDebug() << "✓ Loaded" << schedulesArray.size() << "schedules";
#endif

    // Re-enable refresh button
    m_refreshButton->setEnabled(m_director != nullptr);

    emit statusMessageChanged(QString("%1 Schedules geladen").arg(schedulesArray.size()));
}

void BScheduleWidget::onRefreshClicked()
{
    m_refreshButton->setEnabled(false);
    emit statusMessageChanged("Aktualisiere Schedule-Liste...");
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

#ifdef IS_DEVELOPER
    qDebug() << "BScheduleWidget: Selected schedule:" << schedule["name"].toString();
#endif

    m_weeklyPlanner->setSchedule(schedule);
}

void BScheduleWidget::onScheduleDoubleClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    QJsonObject schedule = item->data(Qt::UserRole).toJsonObject();

#ifdef IS_DEVELOPER
    qDebug() << "BScheduleWidget: Double-clicked schedule:" << schedule["name"].toString();
#endif

    // TODO: Open schedule details dialog
}
