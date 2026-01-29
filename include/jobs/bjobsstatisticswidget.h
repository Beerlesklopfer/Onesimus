#ifndef BSTATISTICSWIDGET_H
#define BSTATISTICSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include "jobs/bjobmodels.h"

/**
 * @brief Widget displaying job statistics in real-time
 * @version 1.0
 * @since 2026-01-26
 * 
 * Displays:
 * - Total job count
 * - Success/Warning/Failed breakdown with progress bars
 * - Total files and bytes backed up
 * - Date range
 * - Selected job count
 */
class BJobsStatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BJobsStatisticsWidget(QWidget *parent = nullptr);
    
    /**
     * @brief Sets the model to display statistics for
     * @param model Pointer to BJobsModel
     * @since 1.0
     */
    void setModel(BJobsModel *model);
    
    /**
     * @brief Refreshes the displayed statistics
     * @since 1.0
     */
    void refresh();

private slots:
    void onModelDataChanged();

private:
    void setupUi();
    void updateStatistics();
    QString formatBytes(qint64 bytes) const;
    
    BJobsModel *m_model;                ///< The model to display stats for
    
    // Summary labels
    QLabel *m_totalJobsLabel;           ///< Total jobs count
    QLabel *m_successLabel;             ///< Successful jobs count
    QLabel *m_warningLabel;             ///< Warning jobs count
    QLabel *m_failedLabel;              ///< Failed jobs count
    QLabel *m_totalFilesLabel;          ///< Total files backed up
    QLabel *m_totalBytesLabel;          ///< Total bytes backed up
    QLabel *m_dateRangeLabel;           ///< Date range label
    QLabel *m_selectedLabel;            ///< Selected jobs count
    
    // Progress bars
    QProgressBar *m_successBar;         ///< Success percentage bar
    QProgressBar *m_warningBar;         ///< Warning percentage bar
    QProgressBar *m_failedBar;          ///< Failed percentage bar
};

#endif // BSTATISTICSWIDGET_H
