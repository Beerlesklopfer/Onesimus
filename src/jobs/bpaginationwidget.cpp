// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bpaginationwidget.h"
#include "buiscale.h"
#include "jobs/bjobpaging.h"
#include "config/bsettings.h"
#include <QHBoxLayout>
#include <QCheckBox>

BPaginationWidget::BPaginationWidget(QWidget *parent)
    : QWidget(parent)
    , m_model(nullptr)
    , m_totalJobs(0)
    , m_currentPage(0)
{
    setupUi();
    connectSignals();
    loadSettings();
    updateControls();
}

void BPaginationWidget::setupUi()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // Enable/disable checkbox
    m_enabledCheckBox = new QCheckBox(tr("Enable Pagination"), this);
    layout->addWidget(m_enabledCheckBox);
    
    layout->addSpacing(20);
    
    // Page size selector
    layout->addWidget(new QLabel(tr("Items per page:"), this));
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
    m_firstButton->setToolTip(tr("First Page"));
    BUiScale::track(m_firstButton, [this]() { m_firstButton->setMaximumWidth(BUiScale::px(40)); });
    m_firstButton->setEnabled(false);
    layout->addWidget(m_firstButton);
    
    m_prevButton = new QPushButton("<", this);
    m_prevButton->setToolTip(tr("Previous Page"));
    BUiScale::track(m_prevButton, [this]() { m_prevButton->setMaximumWidth(BUiScale::px(40)); });
    m_prevButton->setEnabled(false);
    layout->addWidget(m_prevButton);
    
    // Page info label
    m_pageLabel = new QLabel(tr("Page 0 of 0"), this);
    BUiScale::track(m_pageLabel, [this]() { m_pageLabel->setMinimumWidth(BUiScale::px(100)); });
    m_pageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_pageLabel);
    
    m_nextButton = new QPushButton(">", this);
    m_nextButton->setToolTip(tr("Next Page"));
    BUiScale::track(m_nextButton, [this]() { m_nextButton->setMaximumWidth(BUiScale::px(40)); });
    m_nextButton->setEnabled(false);
    layout->addWidget(m_nextButton);
    
    m_lastButton = new QPushButton(">>", this);
    m_lastButton->setToolTip(tr("Last Page"));
    BUiScale::track(m_lastButton, [this]() { m_lastButton->setMaximumWidth(BUiScale::px(40)); });
    m_lastButton->setEnabled(false);
    layout->addWidget(m_lastButton);
    
    layout->addSpacing(20);
    
    // Jump to page
    layout->addWidget(new QLabel(tr("Go to page:"), this));
    m_pageSpinBox = new QSpinBox(this);
    m_pageSpinBox->setMinimum(1);
    m_pageSpinBox->setMaximum(1);
    m_pageSpinBox->setEnabled(false);
    layout->addWidget(m_pageSpinBox);
    
    m_jumpButton = new QPushButton(tr("Go"), this);
    m_jumpButton->setEnabled(false);
    layout->addWidget(m_jumpButton);
    
    layout->addStretch();
}

void BPaginationWidget::connectSignals()
{
    connect(m_enabledCheckBox, &QCheckBox::toggled,
            this, &BPaginationWidget::setPaginationEnabled);
    // Forward the checkbox's toggled signal directly as paginationToggled
    connect(m_enabledCheckBox, &QCheckBox::toggled,
            this, &BPaginationWidget::paginationToggled);

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

        // Note: Do NOT enable client-side pagination in the model here.
        // When the pagination checkbox is checked, we use server-side pagination
        // (ListJobTotals → ListJobs with limit/offset). The model receives one
        // page at a time and must show all rows without further client-side paging.
    }

    updateControls();
}

void BPaginationWidget::setPaginationEnabled(bool enabled)
{
    if (!m_model) {
        return;
    }

    // When enabling pagination, we use server-side pagination (Director handles limit/offset).
    // The model must NOT use client-side pagination on top of that.
    // Only disable client-side pagination when turning off pagination entirely.
    if (!enabled) {
        m_model->setPaginationEnabled(false, 0);
    }

    // Reset server-side state
    m_totalJobs = 0;
    m_currentPage = 0;

    // Enable/disable controls
    m_pageSizeCombo->setEnabled(enabled);
    m_firstButton->setEnabled(enabled);
    m_prevButton->setEnabled(enabled);
    m_nextButton->setEnabled(enabled);
    m_lastButton->setEnabled(enabled);
    m_pageSpinBox->setEnabled(enabled);
    m_jumpButton->setEnabled(enabled);

    // Save settings
    saveSettings();

    updateControls();

}

bool BPaginationWidget::isPaginationEnabled() const
{
    return m_enabledCheckBox->isChecked();
}

void BPaginationWidget::setTotalJobCount(int totalJobs)
{
    m_totalJobs = totalJobs;

    // A refresh keeps the page the user is on; if the list became shorter,
    // the last existing page takes its place. Label and request both use
    // currentPage(), so they cannot diverge.
    m_currentPage = BJobPaging::clampPage(m_currentPage, m_totalJobs, pageSize());

    // When server-side pagination is active (totalJobs > 0), disable client-side
    // pagination in the model. The server already delivers one page of data,
    // so the model must show ALL rows it receives without further paging.
    if (m_model && m_totalJobs > 0) {
        m_model->setPaginationEnabled(false, pageSize());
    }

    updateControls();
}

int BPaginationWidget::pageSize() const
{
    return m_pageSizeCombo->currentData().toInt();
}

int BPaginationWidget::currentPage() const
{
    return m_currentPage;
}

void BPaginationWidget::requestPage(int page)
{
    int ps = pageSize();
    page = BJobPaging::clampPage(page, m_totalJobs, ps);

    m_currentPage = page;
    emit pageRequested(page, ps);
    updateControls();
}

void BPaginationWidget::onFirstPage()
{
    if (m_totalJobs > 0) {
        // Server-side pagination
        requestPage(0);
    } else if (m_model) {
        m_model->setCurrentPage(0);
    }
}

void BPaginationWidget::onPreviousPage()
{
    if (m_totalJobs > 0) {
        // Server-side pagination
        if (m_currentPage > 0) {
            requestPage(m_currentPage - 1);
        }
    } else if (m_model && m_model->currentPage() > 0) {
        m_model->setCurrentPage(m_model->currentPage() - 1);
    }
}

void BPaginationWidget::onNextPage()
{
    if (m_totalJobs > 0) {
        // Server-side pagination
        int ps = pageSize();
        int maxPage = (m_totalJobs - 1) / ps;
        if (m_currentPage < maxPage) {
            requestPage(m_currentPage + 1);
        }
    } else if (m_model && m_model->currentPage() < m_model->pageCount() - 1) {
        m_model->setCurrentPage(m_model->currentPage() + 1);
    }
}

void BPaginationWidget::onLastPage()
{
    if (m_totalJobs > 0) {
        // Server-side pagination
        int ps = pageSize();
        int maxPage = (m_totalJobs - 1) / ps;
        requestPage(maxPage);
    } else if (m_model) {
        m_model->setCurrentPage(m_model->pageCount() - 1);
    }
}

void BPaginationWidget::onPageSizeChanged(int index)
{
    if (!m_enabledCheckBox->isChecked()) {
        return;
    }

    int newPageSize = m_pageSizeCombo->itemData(index).toInt();

    // Save settings
    saveSettings();

    if (m_totalJobs > 0) {
        // Server-side pagination - reset to first page with new page size
        m_currentPage = 0;
        emit pageRequested(0, newPageSize);
        updateControls();
    } else if (m_model) {
        m_model->setPaginationEnabled(true, newPageSize);
        updateControls();
    }
}

void BPaginationWidget::onPageJump()
{
    int page = m_pageSpinBox->value() - 1;  // Convert to 0-indexed

    if (m_totalJobs > 0) {
        // Server-side pagination
        requestPage(page);
    } else if (m_model) {
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
    if (!m_enabledCheckBox->isChecked()) {
        m_pageLabel->setText(tr("Page 0 of 0"));
        m_firstButton->setEnabled(false);
        m_prevButton->setEnabled(false);
        m_nextButton->setEnabled(false);
        m_lastButton->setEnabled(false);
        m_pageSpinBox->setMaximum(1);
        m_pageSpinBox->setValue(1);
        return;
    }

    int currentPageNum = 0;
    int pageCount = 0;

    if (m_totalJobs > 0) {
        // Server-side pagination mode
        int ps = pageSize();
        pageCount = (m_totalJobs + ps - 1) / ps;  // Ceiling division
        currentPageNum = m_currentPage;
    } else if (m_model) {
        // Client-side pagination mode
        currentPageNum = m_model->currentPage();
        pageCount = m_model->pageCount();
    } else {
        m_pageLabel->setText(tr("Page 0 of 0"));
        m_firstButton->setEnabled(false);
        m_prevButton->setEnabled(false);
        m_nextButton->setEnabled(false);
        m_lastButton->setEnabled(false);
        m_pageSpinBox->setMaximum(1);
        m_pageSpinBox->setValue(1);
        return;
    }

    // Update label
    m_pageLabel->setText(tr("Page %1 of %2")
        .arg(currentPageNum + 1)
        .arg(pageCount));

    // Update button states
    m_firstButton->setEnabled(currentPageNum > 0);
    m_prevButton->setEnabled(currentPageNum > 0);
    m_nextButton->setEnabled(currentPageNum < pageCount - 1);
    m_lastButton->setEnabled(currentPageNum < pageCount - 1);

    // Update spinbox
    m_pageSpinBox->setMaximum(qMax(1, pageCount));
    m_pageSpinBox->setValue(currentPageNum + 1);
}

void BPaginationWidget::loadSettings()
{
    BSettings &settings = BSettings::instance();

    // Load pagination enabled state
    bool enabled = settings.jobsPaginationEnabled();
    m_enabledCheckBox->setChecked(enabled);

    // Load page size
    int pageSize = settings.jobsPaginationPageSize();
    int index = m_pageSizeCombo->findData(pageSize);
    if (index >= 0) {
        m_pageSizeCombo->setCurrentIndex(index);
    }

    // Enable/disable controls based on loaded state
    m_pageSizeCombo->setEnabled(enabled);
    m_firstButton->setEnabled(enabled);
    m_prevButton->setEnabled(enabled);
    m_nextButton->setEnabled(enabled);
    m_lastButton->setEnabled(enabled);
    m_pageSpinBox->setEnabled(enabled);
    m_jumpButton->setEnabled(enabled);
}

void BPaginationWidget::saveSettings()
{
    BSettings &settings = BSettings::instance();

    // Save pagination enabled state
    settings.setJobsPaginationEnabled(m_enabledCheckBox->isChecked());

    // Save page size
    int pageSize = m_pageSizeCombo->currentData().toInt();
    settings.setJobsPaginationPageSize(pageSize);
}
