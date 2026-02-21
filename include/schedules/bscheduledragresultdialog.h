#ifndef BSCHEDULEDRAGRESULTDIALOG_H
#define BSCHEDULEDRAGRESULTDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include "models/bresourcemodels.h"
#include "schedules/bscheduleganttwidget.h"
#include "schedules/bjobscheduleindex.h"
#include "director/bdirector.h"

/**
 * @brief Confirmation dialog after a drag & drop time change in the Gantt view
 *
 * Shows:
 * - Old/new time and delta
 * - Shared-schedule warning (if schedule is used by multiple jobs)
 * - Dependency list (level chain, client exclusion, storage contention)
 * - Generated schedule configuration text
 * - Director "configure add" command
 * - Copy/Apply buttons
 */
class BScheduleDragResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BScheduleDragResultDialog(QWidget *parent = nullptr);

    /**
     * @brief Set the drag result data
     * @param entry      The dragged entry (with original time)
     * @param newHour    New hour after drag
     * @param newMinute  New minute after drag
     * @param allEntries All schedule entries (for rebuilding the config)
     * @param deps       Calculated dependencies
     * @param index      Job→Schedule cross-reference for shared-schedule warnings
     */
    void setDragResult(const BScheduleEntry &entry,
                       int newHour, int newMinute,
                       const QList<BScheduleEntry> &allEntries,
                       const QList<BScheduleGanttWidget::DependencyEdge> &deps,
                       const BJobScheduleIndex *index);

    void setDirector(BDirector *director) { m_director = director; }

signals:
    void sendCommand(const BDirector::Command cmd, const QString &args);

private slots:
    void onCopyConfig();
    void onCopyCommand();
    void onApply();
    void onDirectorResponse(bool success, const QString &message);
    void onTimeout();

private:
    void buildUI();
    void generatePreview();
    QString buildConfigText() const;
    QString buildConfigureCommand() const;

    // Data
    BScheduleEntry m_entry;
    int m_newHour = 0;
    int m_newMinute = 0;
    QList<BScheduleEntry> m_allEntries;
    QList<BScheduleGanttWidget::DependencyEdge> m_dependencies;
    const BJobScheduleIndex *m_index = nullptr;
    BDirector *m_director = nullptr;

    // UI
    QLabel *m_summaryLabel;
    QLabel *m_warningLabel;
    QLabel *m_dependencyLabel;
    QTextEdit *m_configEdit;
    QTextEdit *m_commandEdit;
    QPushButton *m_copyConfigButton;
    QPushButton *m_copyCommandButton;
    QPushButton *m_applyButton;
    QPushButton *m_cancelButton;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    QTimer *m_timeoutTimer;

    QList<QMetaObject::Connection> m_connections;
    void disconnectDirectorSignals();
};

#endif // BSCHEDULEDRAGRESULTDIALOG_H
