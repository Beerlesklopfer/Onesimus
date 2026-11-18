#ifndef BSCHEDULEDRAGRESULTDIALOG_H
#define BSCHEDULEDRAGRESULTDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QStringList>
#include <QTableWidget>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include "models/bresourcemodels.h"
#include "schedules/bscheduleganttwidget.h"
#include "schedules/bjobscheduleindex.h"
#include "schedules/bscheduledragplan.h"
#include "director/bdirector.h"

/**
 * @brief Dialog after a drag & drop time change in the Gantt view
 *
 * Apply does not reschedule anything: it creates a new schedule named
 * "<name>-modified" with the dragged time. No job is repointed and the
 * original schedule stays as it is; the dialog says so before and after.
 *
 * The Run lines of the new schedule are the Director's original lines,
 * copied verbatim; only the dragged line is generated (BScheduleDragPlan).
 * If an untouched line has to be respelled the dialog shows both versions
 * and asks for a confirmation. A schedule with a Run line that is not
 * understood is refused.
 *
 * Shows:
 * - Old/new time and delta
 * - What Apply does and does not do
 * - The jobs that use the schedule
 * - Dependency list (level chain, client exclusion, storage contention)
 * - Original and generated Run lines, if they differ
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
     * @param originalRuns Run lines of the entry's schedule as the Director
     *                   reported them (BScheduleModel::runDirectives())
     * @param deps       Calculated dependencies
     * @param index      Job→Schedule cross-reference for shared-schedule warnings
     */
    void setDragResult(const BScheduleEntry &entry,
                       int newHour, int newMinute,
                       const QStringList &originalRuns,
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
    void showPlan();
    void updateApplyEnabled();
    QString newScheduleName() const;
    QString refusalText() const;
    void setStatus(const char *state, const QString &text);
    QString buildConfigText() const;
    QString buildConfigureCommand() const;

    // Data
    BScheduleEntry m_entry;
    int m_newHour = 0;
    int m_newMinute = 0;
    BScheduleDragPlan::Plan m_plan;
    bool m_applied = false;     ///< The Director confirmed the new schedule
    bool m_busy = false;        ///< Waiting for the Director's answer
    QList<BScheduleGanttWidget::DependencyEdge> m_dependencies;
    const BJobScheduleIndex *m_index = nullptr;
    BDirector *m_director = nullptr;

    // UI
    QLabel *m_summaryLabel;
    QLabel *m_effectLabel;
    QLabel *m_compareLabel;
    QTableWidget *m_compareTable;
    QCheckBox *m_confirmCheck;
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
