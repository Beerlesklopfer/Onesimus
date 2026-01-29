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
 * @version 1.0
 * @since 2026-01-26
 * 
 * Provides:
 * - First/Previous/Next/Last page buttons
 * - Current page display
 * - Page size selector
 * - Direct page jump
 */
class BPaginationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BPaginationWidget(QWidget *parent = nullptr);
    
    /**
     * @brief Sets the model to paginate
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

signals:
    /**
     * @brief Emitted when page changes
     * @param page New page number (0-indexed)
     * @since 1.0
     */
    void pageChanged(int page);

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
};

#endif // BPAGINATIONWIDGET_H
