#include "jobs/bjobsfilterwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QTimer>

BJobsFilterWidget::BJobsFilterWidget(QWidget *parent)
    : QWidget(parent)
    , m_filterModel(nullptr)
{
    setupUi();
    connectSignals();
}

void BJobsFilterWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Text filters group
    QGroupBox *textGroup = new QGroupBox("Text Filters", this);
    QFormLayout *textLayout = new QFormLayout(textGroup);
    
    m_nameFilter = new QLineEdit(this);
    m_nameFilter->setPlaceholderText("Filter by job name...");
    textLayout->addRow("Job Name:", m_nameFilter);
    
    m_clientFilter = new QLineEdit(this);
    m_clientFilter->setPlaceholderText("Filter by client...");
    textLayout->addRow("Client:", m_clientFilter);
    
    mainLayout->addWidget(textGroup);
    
    // Status filters group
    QGroupBox *statusGroup = new QGroupBox("Status Filters", this);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    m_statusSuccess = new QCheckBox("Successful (T)", this);
    m_statusWarning = new QCheckBox("Warning (W)", this);
    m_statusFailed = new QCheckBox("Failed (f)", this);
    m_statusError = new QCheckBox("Error (E)", this);
    
    m_statusSuccess->setChecked(true);
    m_statusWarning->setChecked(true);
    m_statusFailed->setChecked(true);
    m_statusError->setChecked(true);
    
    statusLayout->addWidget(m_statusSuccess);
    statusLayout->addWidget(m_statusWarning);
    statusLayout->addWidget(m_statusFailed);
    statusLayout->addWidget(m_statusError);
    
    mainLayout->addWidget(statusGroup);
    
    // Level filters group
    QGroupBox *levelGroup = new QGroupBox("Backup Level", this);
    QVBoxLayout *levelLayout = new QVBoxLayout(levelGroup);
    
    m_levelFull = new QCheckBox("Full (F)", this);
    m_levelIncremental = new QCheckBox("Incremental (I)", this);
    m_levelDifferential = new QCheckBox("Differential (D)", this);
    
    m_levelFull->setChecked(true);
    m_levelIncremental->setChecked(true);
    m_levelDifferential->setChecked(true);
    
    levelLayout->addWidget(m_levelFull);
    levelLayout->addWidget(m_levelIncremental);
    levelLayout->addWidget(m_levelDifferential);
    
    mainLayout->addWidget(levelGroup);
    
    // Date range group
    QGroupBox *dateGroup = new QGroupBox("Date Range", this);
    QVBoxLayout *dateLayout = new QVBoxLayout(dateGroup);
    
    m_dateEnabled = new QCheckBox("Enable Date Filter", this);
    dateLayout->addWidget(m_dateEnabled);
    
    QFormLayout *dateFormLayout = new QFormLayout();
    
    m_dateFrom = new QDateTimeEdit(this);
    m_dateFrom->setCalendarPopup(true);
    m_dateFrom->setDateTime(QDateTime::currentDateTime().addDays(-30));
    m_dateFrom->setEnabled(false);
    dateFormLayout->addRow("From:", m_dateFrom);
    
    m_dateTo = new QDateTimeEdit(this);
    m_dateTo->setCalendarPopup(true);
    m_dateTo->setDateTime(QDateTime::currentDateTime());
    m_dateTo->setEnabled(false);
    dateFormLayout->addRow("To:", m_dateTo);
    
    dateLayout->addLayout(dateFormLayout);
    
    connect(m_dateEnabled, &QCheckBox::toggled, m_dateFrom, &QDateTimeEdit::setEnabled);
    connect(m_dateEnabled, &QCheckBox::toggled, m_dateTo, &QDateTimeEdit::setEnabled);
    
    mainLayout->addWidget(dateGroup);
    
    // File count group
    QGroupBox *fileGroup = new QGroupBox("File Count Range", this);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroup);
    
    m_fileCountEnabled = new QCheckBox("Enable File Count Filter", this);
    fileLayout->addWidget(m_fileCountEnabled);
    
    QFormLayout *fileFormLayout = new QFormLayout();
    
    m_fileCountMin = new QSpinBox(this);
    m_fileCountMin->setRange(0, INT_MAX);
    m_fileCountMin->setValue(0);
    m_fileCountMin->setEnabled(false);
    fileFormLayout->addRow("Minimum:", m_fileCountMin);
    
    m_fileCountMax = new QSpinBox(this);
    m_fileCountMax->setRange(0, INT_MAX);
    m_fileCountMax->setValue(10000);
    m_fileCountMax->setEnabled(false);
    fileFormLayout->addRow("Maximum:", m_fileCountMax);
    
    fileLayout->addLayout(fileFormLayout);
    
    connect(m_fileCountEnabled, &QCheckBox::toggled, m_fileCountMin, &QSpinBox::setEnabled);
    connect(m_fileCountEnabled, &QCheckBox::toggled, m_fileCountMax, &QSpinBox::setEnabled);
    
    mainLayout->addWidget(fileGroup);
    
    // Byte size group
    QGroupBox *sizeGroup = new QGroupBox("Size Range (MB)", this);
    QVBoxLayout *sizeLayout = new QVBoxLayout(sizeGroup);
    
    m_byteSizeEnabled = new QCheckBox("Enable Size Filter", this);
    sizeLayout->addWidget(m_byteSizeEnabled);
    
    QFormLayout *sizeFormLayout = new QFormLayout();
    
    m_byteSizeMin = new QSpinBox(this);
    m_byteSizeMin->setRange(0, INT_MAX);
    m_byteSizeMin->setValue(0);
    m_byteSizeMin->setEnabled(false);
    sizeFormLayout->addRow("Minimum (MB):", m_byteSizeMin);
    
    m_byteSizeMax = new QSpinBox(this);
    m_byteSizeMax->setRange(0, INT_MAX);
    m_byteSizeMax->setValue(100000);
    m_byteSizeMax->setEnabled(false);
    sizeFormLayout->addRow("Maximum (MB):", m_byteSizeMax);
    
    sizeLayout->addLayout(sizeFormLayout);
    
    connect(m_byteSizeEnabled, &QCheckBox::toggled, m_byteSizeMin, &QSpinBox::setEnabled);
    connect(m_byteSizeEnabled, &QCheckBox::toggled, m_byteSizeMax, &QSpinBox::setEnabled);
    
    mainLayout->addWidget(sizeGroup);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_applyButton = new QPushButton("Apply Filters", this);
    m_clearButton = new QPushButton("Clear All", this);
    
    buttonLayout->addWidget(m_applyButton);
    buttonLayout->addWidget(m_clearButton);
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void BJobsFilterWidget::connectSignals()
{
    connect(m_applyButton, &QPushButton::clicked, this, &BJobsFilterWidget::applyFilters);
    connect(m_clearButton, &QPushButton::clicked, this, &BJobsFilterWidget::clearFilters);
    
    // Auto-apply on text change (with small delay)
    connect(m_nameFilter, &QLineEdit::textChanged, this, [this]() {
        QTimer::singleShot(300, this, &BJobsFilterWidget::applyFilters);
    });
    
    connect(m_clientFilter, &QLineEdit::textChanged, this, [this]() {
        QTimer::singleShot(300, this, &BJobsFilterWidget::applyFilters);
    });
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
