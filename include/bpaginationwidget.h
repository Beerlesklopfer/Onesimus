#ifndef BPAGINATIONWIDGET_H
#define BPAGINATIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include "jobs/bjobmodels.h"

/**
 * @brief Widget providing pagination controls
 * @version 2.0
 * @since 2026-01-26
 *
 * Provides:
 * - First/Previous/Next/Last page buttons
 * - Current page display
 * - Page size selector
 * - Direct page jump
 * - Server-side pagination support
 */
class BPaginationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BPaginationWidget(QWidget *parent = nullptr);

    /**
     * @brief Sets the model to paginate (for client-side pagination)
     * @param model Pointer to BJobsModel
     * @since 1.0
     */
    void setModel(BJobsModel *model);

    /**
     * @brief Enables or disables pagination
     * @param enabled True to enable pagination
     * @since 1.0
     */
    void setPaginationEnabled(bool enabled);

    /**
     * @brief Returns whether pagination is enabled
     * @return True if pagination is enabled
     * @since 1.0
     */
    bool isPaginationEnabled() const;

    /**
     * @brief Sets the total job count for server-side pagination
     * @param totalJobs Total number of jobs in the database
     * @since 2.0
     */
    void setTotalJobCount(int totalJobs);

    /**
     * @brief Gets the current page size
     * @return Page size
     * @since 2.0
     */
    int pageSize() const;

    /**
     * @brief Gets the current page (0-indexed)
     * @return Current page number
     * @since 2.0
     */
    int currentPage() const;

    /**
     * @brief Gets the total job count (from server-side pagination)
     * @return Total number of jobs, or 0 if not in server-side mode
     * @since 2.9
     */
    int totalJobCount() const { return m_totalJobs; }

signals:
    /**
     * @brief Emitted when page changes (for client-side pagination)
     * @param page New page number (0-indexed)
     * @since 1.0
     */
    void pageChanged(int page);

    /**
     * @brief Emitted when a new page is requested (for server-side pagination)
     * @param page Page number (0-indexed)
     * @param pageSize Number of items per page
     * @since 2.0
     */
    void pageRequested(int page, int pageSize);

    /**
     * @brief Emitted when pagination is toggled on or off
     * @param enabled True if pagination was enabled
     * @since 2.9
     */
    void paginationToggled(bool enabled);

private slots:
    void onFirstPage();
    void onPreviousPage();
    void onNextPage();
    void onLastPage();
    void onPageSizeChanged(int index);
    void onPageJump();
    void onModelPageChanged(int page);
    void updateControls();

private:
    void setupUi();
    void connectSignals();
    void loadSettings();
    void saveSettings();
    void requestPage(int page);

    BJobsModel *m_model;            ///< The model to paginate

    QPushButton *m_firstButton;     ///< First page button
    QPushButton *m_prevButton;      ///< Previous page button
    QPushButton *m_nextButton;      ///< Next page button
    QPushButton *m_lastButton;      ///< Last page button

    QLabel *m_pageLabel;            ///< Page info label
    QSpinBox *m_pageSpinBox;        ///< Direct page jump spinbox
    QPushButton *m_jumpButton;      ///< Jump to page button
    QComboBox *m_pageSizeCombo;     ///< Page size selector
    QCheckBox *m_enabledCheckBox;   ///< Enable/disable pagination checkbox

    // Server-side pagination
    int m_totalJobs;                ///< Total job count (from jobtotals)
    int m_currentPage;              ///< Current page (0-indexed)
};

#endif // BPAGINATIONWIDGET_H
