#ifndef BJOBSFILTERWIDGET_H
#define BJOBSFILTERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include "bjobsfiltermodel.h"

/**
 * @brief Widget providing UI controls for filtering jobs
 * @version 1.0
 * @since 2026-01-26
 * 
 * Provides filter controls for:
 * - Name and client text search
 * - Status checkboxes (T/W/f/E)
 * - Level checkboxes (F/I/D)
 * - Date range
 * - File count range
 * - Byte size range
 */
class BJobsFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BJobsFilterWidget(QWidget *parent = nullptr);
    
    /**
     * @brief Sets the filter model to control
     * @param model Pointer to BJobsFilterModel
     * @since 1.0
     */
    void setFilterModel(BJobsFilterModel *model);
    
    /**
     * @brief Applies current filter settings to the model
     * @since 1.0
     */
    void applyFilters();
    
    /**
     * @brief Clears all filter settings
     * @since 1.0
     */
    void clearFilters();

signals:
    /**
     * @brief Emitted when filters are applied
     * @since 1.0
     */
    void filtersApplied();
    
    /**
     * @brief Emitted when filters are cleared
     * @since 1.0
     */
    void filtersCleared();

private:
    void setupUi();
    void connectSignals();
    
    BJobsFilterModel *m_filterModel;    ///< The filter model to control
    
    // Text filters
    QLineEdit *m_nameFilter;            ///< Job name filter input
    QLineEdit *m_clientFilter;          ///< Client name filter input
    
    // Status filters
    QCheckBox *m_statusSuccess;         ///< Status 'T' checkbox
    QCheckBox *m_statusWarning;         ///< Status 'W' checkbox
    QCheckBox *m_statusFailed;          ///< Status 'f' checkbox
    QCheckBox *m_statusError;           ///< Status 'E' checkbox
    
    // Level filters
    QCheckBox *m_levelFull;             ///< Level 'F' checkbox
    QCheckBox *m_levelIncremental;      ///< Level 'I' checkbox
    QCheckBox *m_levelDifferential;     ///< Level 'D' checkbox
    
    // Date filters
    QDateTimeEdit *m_dateFrom;          ///< Start date filter
    QDateTimeEdit *m_dateTo;            ///< End date filter
    QCheckBox *m_dateEnabled;           ///< Enable date filter checkbox
    
    // Numeric filters
    QSpinBox *m_fileCountMin;           ///< Minimum file count
    QSpinBox *m_fileCountMax;           ///< Maximum file count
    QCheckBox *m_fileCountEnabled;      ///< Enable file count filter checkbox
    
    QSpinBox *m_byteSizeMin;            ///< Minimum byte size (MB)
    QSpinBox *m_byteSizeMax;            ///< Maximum byte size (MB)
    QCheckBox *m_byteSizeEnabled;       ///< Enable byte size filter checkbox
    
    // Buttons
    QPushButton *m_applyButton;         ///< Apply filters button
    QPushButton *m_clearButton;         ///< Clear filters button
};

#endif // BJOBSFILTERWIDGET_H
