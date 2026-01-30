#include "jobs/bjobsfilterwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QTimer>
#include <QSettings>

BJobsFilterWidget::BJobsFilterWidget(QWidget *parent)
    : QWidget(parent)
    , m_filterModel(nullptr)
    , m_toolBox(nullptr)
{
    setupUi();
    connectSignals();

    // Restore last selected tab from settings
    QSettings settings;
    int lastIndex = settings.value("Jobs/filterTabIndex", 0).toInt();
    m_toolBox->setCurrentIndex(lastIndex);
}

void BJobsFilterWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(4, 4, 4, 4);

    // ========================================================================
    // QToolBox for accordion-style sections
    // ========================================================================

    m_toolBox = new QToolBox(this);

    // ========================================================================
    // Basic Filters Page
    // ========================================================================

    QWidget *basicPage = new QWidget();
    QVBoxLayout *basicLayout = new QVBoxLayout(basicPage);
    basicLayout->setContentsMargins(4, 8, 4, 8);
    basicLayout->setSpacing(6);

    // Text filters
    QFormLayout *textLayout = new QFormLayout();
    textLayout->setSpacing(4);

    m_nameFilter = new QLineEdit(this);
    m_nameFilter->setPlaceholderText(tr("Filter by job name..."));
    textLayout->addRow(tr("Job:"), m_nameFilter);

    m_clientFilter = new QLineEdit(this);
    m_clientFilter->setPlaceholderText(tr("Filter by client..."));
    textLayout->addRow(tr("Client:"), m_clientFilter);

    basicLayout->addLayout(textLayout);

    // Status filters - compact
    QLabel *statusLabel = new QLabel(tr("Status:"), this);
    basicLayout->addWidget(statusLabel);

    QHBoxLayout *statusRow = new QHBoxLayout();
    statusRow->setSpacing(2);
    m_statusSuccess = new QCheckBox(tr("OK"), this);
    m_statusWarning = new QCheckBox(tr("Warn"), this);
    m_statusFailed = new QCheckBox(tr("Fail"), this);
    m_statusError = new QCheckBox(tr("Err"), this);

    m_statusSuccess->setChecked(true);
    m_statusWarning->setChecked(true);
    m_statusFailed->setChecked(true);
    m_statusError->setChecked(true);

    statusRow->addWidget(m_statusSuccess);
    statusRow->addWidget(m_statusWarning);
    statusRow->addWidget(m_statusFailed);
    statusRow->addWidget(m_statusError);
    statusRow->addStretch();
    basicLayout->addLayout(statusRow);

    // Level filters - compact
    QLabel *levelLabel = new QLabel(tr("Level:"), this);
    basicLayout->addWidget(levelLabel);

    QHBoxLayout *levelRow = new QHBoxLayout();
    levelRow->setSpacing(2);
    m_levelFull = new QCheckBox(tr("Full"), this);
    m_levelIncremental = new QCheckBox(tr("Incr"), this);
    m_levelDifferential = new QCheckBox(tr("Diff"), this);

    m_levelFull->setChecked(true);
    m_levelIncremental->setChecked(true);
    m_levelDifferential->setChecked(true);

    levelRow->addWidget(m_levelFull);
    levelRow->addWidget(m_levelIncremental);
    levelRow->addWidget(m_levelDifferential);
    levelRow->addStretch();
    basicLayout->addLayout(levelRow);

    basicLayout->addStretch();

    m_toolBox->addItem(basicPage, tr("Basic Filters"));

    // ========================================================================
    // Advanced Filters Page
    // ========================================================================

    QWidget *advancedPage = new QWidget();
    QVBoxLayout *advancedLayout = new QVBoxLayout(advancedPage);
    advancedLayout->setContentsMargins(4, 8, 4, 8);
    advancedLayout->setSpacing(6);

    // Zero Bytes filter
    m_statusZeroBytes = new QCheckBox(tr("Only Zero Bytes"), this);
    m_statusZeroBytes->setChecked(false);
    advancedLayout->addWidget(m_statusZeroBytes);

    // Date range
    m_dateEnabled = new QCheckBox(tr("Date Range:"), this);
    advancedLayout->addWidget(m_dateEnabled);

    QHBoxLayout *dateRow = new QHBoxLayout();
    dateRow->setSpacing(4);
    m_dateFrom = new QDateTimeEdit(this);
    m_dateFrom->setCalendarPopup(true);
    m_dateFrom->setDateTime(QDateTime::currentDateTime().addDays(-30));
    m_dateFrom->setEnabled(false);
    m_dateFrom->setDisplayFormat("dd.MM.yy");

    m_dateTo = new QDateTimeEdit(this);
    m_dateTo->setCalendarPopup(true);
    m_dateTo->setDateTime(QDateTime::currentDateTime());
    m_dateTo->setEnabled(false);
    m_dateTo->setDisplayFormat("dd.MM.yy");

    dateRow->addWidget(m_dateFrom);
    dateRow->addWidget(new QLabel("-", this));
    dateRow->addWidget(m_dateTo);
    dateRow->addStretch();
    advancedLayout->addLayout(dateRow);

    connect(m_dateEnabled, &QCheckBox::toggled, m_dateFrom, &QDateTimeEdit::setEnabled);
    connect(m_dateEnabled, &QCheckBox::toggled, m_dateTo, &QDateTimeEdit::setEnabled);

    // File count
    m_fileCountEnabled = new QCheckBox(tr("File Count:"), this);
    advancedLayout->addWidget(m_fileCountEnabled);

    QHBoxLayout *fileRow = new QHBoxLayout();
    fileRow->setSpacing(4);
    m_fileCountMin = new QSpinBox(this);
    m_fileCountMin->setRange(0, INT_MAX);
    m_fileCountMin->setValue(0);
    m_fileCountMin->setEnabled(false);

    m_fileCountMax = new QSpinBox(this);
    m_fileCountMax->setRange(0, INT_MAX);
    m_fileCountMax->setValue(10000);
    m_fileCountMax->setEnabled(false);

    fileRow->addWidget(m_fileCountMin);
    fileRow->addWidget(new QLabel("-", this));
    fileRow->addWidget(m_fileCountMax);
    fileRow->addStretch();
    advancedLayout->addLayout(fileRow);

    connect(m_fileCountEnabled, &QCheckBox::toggled, m_fileCountMin, &QSpinBox::setEnabled);
    connect(m_fileCountEnabled, &QCheckBox::toggled, m_fileCountMax, &QSpinBox::setEnabled);

    // Byte size
    m_byteSizeEnabled = new QCheckBox(tr("Size (MB):"), this);
    advancedLayout->addWidget(m_byteSizeEnabled);

    QHBoxLayout *sizeRow = new QHBoxLayout();
    sizeRow->setSpacing(4);
    m_byteSizeMin = new QSpinBox(this);
    m_byteSizeMin->setRange(0, INT_MAX);
    m_byteSizeMin->setValue(0);
    m_byteSizeMin->setEnabled(false);

    m_byteSizeMax = new QSpinBox(this);
    m_byteSizeMax->setRange(0, INT_MAX);
    m_byteSizeMax->setValue(100000);
    m_byteSizeMax->setEnabled(false);

    sizeRow->addWidget(m_byteSizeMin);
    sizeRow->addWidget(new QLabel("-", this));
    sizeRow->addWidget(m_byteSizeMax);
    sizeRow->addStretch();
    advancedLayout->addLayout(sizeRow);

    connect(m_byteSizeEnabled, &QCheckBox::toggled, m_byteSizeMin, &QSpinBox::setEnabled);
    connect(m_byteSizeEnabled, &QCheckBox::toggled, m_byteSizeMax, &QSpinBox::setEnabled);

    advancedLayout->addStretch();

    m_toolBox->addItem(advancedPage, tr("Advanced Filters"));

    mainLayout->addWidget(m_toolBox);

    // ========================================================================
    // Buttons
    // ========================================================================

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(4);

    m_applyButton = new QPushButton(tr("Apply"), this);
    m_clearButton = new QPushButton(tr("Clear"), this);

    buttonLayout->addWidget(m_applyButton);
    buttonLayout->addWidget(m_clearButton);

    mainLayout->addLayout(buttonLayout);
}

void BJobsFilterWidget::connectSignals()
{
    connect(m_applyButton, &QPushButton::clicked, this, &BJobsFilterWidget::applyFilters);
    connect(m_clearButton, &QPushButton::clicked, this, &BJobsFilterWidget::clearFilters);

    // Save tab index when changed
    connect(m_toolBox, &QToolBox::currentChanged, this, &BJobsFilterWidget::onToolBoxChanged);

    // Auto-apply on text change (with small delay)
    connect(m_nameFilter, &QLineEdit::textChanged, this, [this]() {
        QTimer::singleShot(300, this, &BJobsFilterWidget::applyFilters);
    });

    connect(m_clientFilter, &QLineEdit::textChanged, this, [this]() {
        QTimer::singleShot(300, this, &BJobsFilterWidget::applyFilters);
    });
}

void BJobsFilterWidget::onToolBoxChanged(int index)
{
    QSettings settings;
    settings.setValue("Jobs/filterTabIndex", index);
}

void BJobsFilterWidget::setFilterModel(BJobsFilterModel *model)
{
    m_filterModel = model;
}

void BJobsFilterWidget::applyFilters()
{
    if (!m_filterModel) {
        return;
    }

    // Apply text filters
    m_filterModel->setNameFilter(m_nameFilter->text());
    m_filterModel->setClientFilter(m_clientFilter->text());

    // Apply status filter
    QSet<QString> statuses;
    if (m_statusSuccess->isChecked()) statuses.insert("T");
    if (m_statusWarning->isChecked()) statuses.insert("W");
    if (m_statusFailed->isChecked()) statuses.insert("f");
    if (m_statusError->isChecked()) statuses.insert("E");

    // If all are checked or none, show all statuses
    if (statuses.size() == 4 || statuses.isEmpty()) {
        m_filterModel->setStatusFilter(QSet<QString>());
    } else {
        m_filterModel->setStatusFilter(statuses);
    }

    // Apply level filter
    QSet<QString> levels;
    if (m_levelFull->isChecked()) levels.insert("F");
    if (m_levelIncremental->isChecked()) levels.insert("I");
    if (m_levelDifferential->isChecked()) levels.insert("D");

    if (levels.size() == 3 || levels.isEmpty()) {
        m_filterModel->setLevelFilter(QSet<QString>());
    } else {
        m_filterModel->setLevelFilter(levels);
    }

    // Apply date filter
    if (m_dateEnabled->isChecked()) {
        m_filterModel->setDateFilter(m_dateFrom->dateTime(), m_dateTo->dateTime());
    } else {
        m_filterModel->setDateFilter(QDateTime(), QDateTime());
    }

    // Apply file count filter
    if (m_fileCountEnabled->isChecked()) {
        m_filterModel->setFileCountFilter(m_fileCountMin->value(), m_fileCountMax->value());
    } else {
        m_filterModel->setFileCountFilter(-1, -1);
    }

    // Apply byte size filter (convert MB to bytes)
    if (m_byteSizeEnabled->isChecked()) {
        qint64 minBytes = static_cast<qint64>(m_byteSizeMin->value()) * 1024 * 1024;
        qint64 maxBytes = static_cast<qint64>(m_byteSizeMax->value()) * 1024 * 1024;
        m_filterModel->setByteSizeFilter(minBytes, maxBytes);
    } else {
        m_filterModel->setByteSizeFilter(-1, -1);
    }

    // Apply zero bytes filter
    m_filterModel->setZeroBytesFilter(m_statusZeroBytes->isChecked());

    emit filtersApplied();
}

void BJobsFilterWidget::clearFilters()
{
    // Clear text filters
    m_nameFilter->clear();
    m_clientFilter->clear();

    // Check all status and level checkboxes
    m_statusSuccess->setChecked(true);
    m_statusWarning->setChecked(true);
    m_statusFailed->setChecked(true);
    m_statusError->setChecked(true);
    m_statusZeroBytes->setChecked(false);

    m_levelFull->setChecked(true);
    m_levelIncremental->setChecked(true);
    m_levelDifferential->setChecked(true);

    // Disable and reset date filter
    m_dateEnabled->setChecked(false);
    m_dateFrom->setDateTime(QDateTime::currentDateTime().addDays(-30));
    m_dateTo->setDateTime(QDateTime::currentDateTime());

    // Disable and reset numeric filters
    m_fileCountEnabled->setChecked(false);
    m_fileCountMin->setValue(0);
    m_fileCountMax->setValue(10000);

    m_byteSizeEnabled->setChecked(false);
    m_byteSizeMin->setValue(0);
    m_byteSizeMax->setValue(100000);

    // Apply cleared filters
    if (m_filterModel) {
        m_filterModel->clearAllFilters();
    }

    emit filtersCleared();
}
