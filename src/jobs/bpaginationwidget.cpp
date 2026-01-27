#include "bpaginationwidget.h"
#include <QHBoxLayout>
#include <QCheckBox>

BPaginationWidget::BPaginationWidget(QWidget *parent)
    : QWidget(parent)
    , m_model(nullptr)
{
    setupUi();
    connectSignals();
    updateControls();
}

void BPaginationWidget::setupUi()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // Enable/disable checkbox
    m_enabledCheckBox = new QCheckBox("Enable Pagination", this);
    layout->addWidget(m_enabledCheckBox);
    
    layout->addSpacing(20);
    
    // Page size selector
    layout->addWidget(new QLabel("Items per page:", this));
    m_pageSizeCombo = new QComboBox(this);
    m_pageSizeCombo->addItem("25", 25);
    m_pageSizeCombo->addItem("50", 50);
    m_pageSizeCombo->addItem("100", 100);
    m_pageSizeCombo->addItem("200", 200);
    m_pageSizeCombo->setCurrentIndex(1);  // Default to 50
    m_pageSizeCombo->setEnabled(false);
    layout->addWidget(m_pageSizeCombo);
    
    layout->addSpacing(20);
    
    // Navigation buttons
    m_firstButton = new QPushButton("<<", this);
    m_firstButton->setToolTip("First Page");
    m_firstButton->setMaximumWidth(40);
    m_firstButton->setEnabled(false);
    layout->addWidget(m_firstButton);
    
    m_prevButton = new QPushButton("<", this);
    m_prevButton->setToolTip("Previous Page");
    m_prevButton->setMaximumWidth(40);
    m_prevButton->setEnabled(false);
    layout->addWidget(m_prevButton);
    
    // Page info label
    m_pageLabel = new QLabel("Page 0 of 0", this);
    m_pageLabel->setMinimumWidth(100);
    m_pageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_pageLabel);
    
    m_nextButton = new QPushButton(">", this);
    m_nextButton->setToolTip("Next Page");
    m_nextButton->setMaximumWidth(40);
    m_nextButton->setEnabled(false);
    layout->addWidget(m_nextButton);
    
    m_lastButton = new QPushButton(">>", this);
    m_lastButton->setToolTip("Last Page");
    m_lastButton->setMaximumWidth(40);
    m_lastButton->setEnabled(false);
    layout->addWidget(m_lastButton);
    
    layout->addSpacing(20);
    
    // Jump to page
    layout->addWidget(new QLabel("Go to page:", this));
    m_pageSpinBox = new QSpinBox(this);
    m_pageSpinBox->setMinimum(1);
    m_pageSpinBox->setMaximum(1);
    m_pageSpinBox->setEnabled(false);
    layout->addWidget(m_pageSpinBox);
    
    m_jumpButton = new QPushButton("Go", this);
    m_jumpButton->setEnabled(false);
    layout->addWidget(m_jumpButton);
    
    layout->addStretch();
}

void BPaginationWidget::connectSignals()
{
    connect(m_enabledCheckBox, &QCheckBox::toggled,
            this, &BPaginationWidget::setPaginationEnabled);
    
    connect(m_firstButton, &QPushButton::clicked,
            this, &BPaginationWidget::onFirstPage);
    
    connect(m_prevButton, &QPushButton::clicked,
            this, &BPaginationWidget::onPreviousPage);
    
    connect(m_nextButton, &QPushButton::clicked,
            this, &BPaginationWidget::onNextPage);
    
    connect(m_lastButton, &QPushButton::clicked,
            this, &BPaginationWidget::onLastPage);
    
    connect(m_pageSizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BPaginationWidget::onPageSizeChanged);
    
    connect(m_jumpButton, &QPushButton::clicked,
            this, &BPaginationWidget::onPageJump);
}

void BPaginationWidget::setModel(BJobsModel *model)
{
    if (m_model) {
        disconnect(m_model, nullptr, this, nullptr);
    }
    
    m_model = model;
    
    if (m_model) {
        connect(m_model, &BJobsModel::currentPageChanged,
                this, &BPaginationWidget::onModelPageChanged);
        
        connect(m_model, &QAbstractItemModel::modelReset,
                this, &BPaginationWidget::updateControls);
    }
    
    updateControls();
}

void BPaginationWidget::setPaginationEnabled(bool enabled)
{
    if (!m_model) {
        return;
    }
    
    int pageSize = m_pageSizeCombo->currentData().toInt();
    m_model->setPaginationEnabled(enabled, pageSize);
    
    // Enable/disable controls
    m_pageSizeCombo->setEnabled(enabled);
    m_firstButton->setEnabled(enabled);
    m_prevButton->setEnabled(enabled);
    m_nextButton->setEnabled(enabled);
    m_lastButton->setEnabled(enabled);
    m_pageSpinBox->setEnabled(enabled);
    m_jumpButton->setEnabled(enabled);
    
    updateControls();
}

bool BPaginationWidget::isPaginationEnabled() const
{
    return m_enabledCheckBox->isChecked();
}

void BPaginationWidget::onFirstPage()
{
    if (m_model) {
        m_model->setCurrentPage(0);
    }
}

void BPaginationWidget::onPreviousPage()
{
    if (m_model && m_model->currentPage() > 0) {
        m_model->setCurrentPage(m_model->currentPage() - 1);
    }
}

void BPaginationWidget::onNextPage()
{
    if (m_model && m_model->currentPage() < m_model->pageCount() - 1) {
        m_model->setCurrentPage(m_model->currentPage() + 1);
    }
}

void BPaginationWidget::onLastPage()
{
    if (m_model) {
        m_model->setCurrentPage(m_model->pageCount() - 1);
    }
}

void BPaginationWidget::onPageSizeChanged(int index)
{
    if (!m_model || !m_enabledCheckBox->isChecked()) {
        return;
    }
    
    int pageSize = m_pageSizeCombo->itemData(index).toInt();
    m_model->setPaginationEnabled(true, pageSize);
    updateControls();
}

void BPaginationWidget::onPageJump()
{
    if (m_model) {
        int page = m_pageSpinBox->value() - 1;  // Convert to 0-indexed
        m_model->setCurrentPage(page);
    }
}

void BPaginationWidget::onModelPageChanged(int page)
{
    updateControls();
    emit pageChanged(page);
}

void BPaginationWidget::updateControls()
{
    if (!m_model || !m_enabledCheckBox->isChecked()) {
        m_pageLabel->setText("Page 0 of 0");
        m_firstButton->setEnabled(false);
        m_prevButton->setEnabled(false);
        m_nextButton->setEnabled(false);
        m_lastButton->setEnabled(false);
        m_pageSpinBox->setMaximum(1);
        m_pageSpinBox->setValue(1);
        return;
    }
    
    int currentPage = m_model->currentPage();
    int pageCount = m_model->pageCount();
    
    // Update label
    m_pageLabel->setText(QString("Page %1 of %2")
        .arg(currentPage + 1)
        .arg(pageCount));
    
    // Update button states
    m_firstButton->setEnabled(currentPage > 0);
    m_prevButton->setEnabled(currentPage > 0);
    m_nextButton->setEnabled(currentPage < pageCount - 1);
    m_lastButton->setEnabled(currentPage < pageCount - 1);
    
    // Update spinbox
    m_pageSpinBox->setMaximum(qMax(1, pageCount));
    m_pageSpinBox->setValue(currentPage + 1);
}
