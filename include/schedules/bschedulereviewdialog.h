#ifndef BSCHEDULEREVIEWDIALOG_H
#define BSCHEDULEREVIEWDIALOG_H

#include <QDialog>
#include <QPointer>

#include "config/bresourcereviewdialog.h"
#include "schedules/bschedulepersistence.h"

class QDialogButtonBox;
class QLabel;
class QListWidget;
class QPlainTextEdit;
class QPushButton;
class QTabWidget;

/**
 * @file bschedulereviewdialog.h
 * @brief The dialogs of writing a schedule: the review before, the report after
 */

/**
 * @brief The review before a schedule is written
 *
 * Left the Run lines as they are in the file, right as they will be; a
 * changed, new or removed line is marked by a word in front of it and by
 * colour. Below: the jobs that use the schedule (the change applies to all
 * of them), what the Director names as referencing it, the resource text
 * before and after, and the file. "Write to the Director" is never the
 * default button: Enter cancels.
 */
class BScheduleReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BScheduleReviewDialog(const BScheduleReview &review, QWidget *parent = nullptr);

    QListWidget *oldList() const { return m_old; }
    QListWidget *newList() const { return m_new; }
    QListWidget *jobList() const { return m_jobs; }
    QPushButton *writeButton() const { return m_write; }
    QTabWidget *textTabs() const { return m_texts; }

    /// "changed: ", "new: ", "removed: " or nothing
    static QString stateWord(BScheduleReview::Row::State state);

protected:
    void showEvent(QShowEvent *event) override;

private:
    QListWidget *m_old = nullptr;
    QListWidget *m_new = nullptr;
    QListWidget *m_jobs = nullptr;
    QTabWidget *m_texts = nullptr;
    QPushButton *m_write = nullptr;
};

/**
 * @brief What came of a write: success with follow-ups, or why not
 *
 * The report dialog of every resource (BResourceReportDialog) for a
 * schedule: a conflict shows the text the Director has next to the text
 * that was to be written and offers "Reload"; nothing is ever written from
 * here. An invalid configuration names file, line and column.
 */
class BScheduleReportDialog : public BResourceReportDialog
{
    Q_OBJECT

public:
    explicit BScheduleReportDialog(const BScheduleReport &report, QWidget *parent = nullptr);

    /// The report of every resource for a schedule report
    static BResourceReport toResourceReport(const BScheduleReport &report);
};

/**
 * @brief BScheduleSaveUi with the two dialogs, modal over a widget
 *
 * A write that went through without follow-ups and with a reload is not
 * shown in a dialog; the schedule view says so in the status line.
 */
class BScheduleSaveDialogs : public BScheduleSaveUi
{
public:
    explicit BScheduleSaveDialogs(QWidget *parent) : m_parent(parent) {}

    bool confirm(const BScheduleReview &review) override;
    Answer report(const BScheduleReport &report) override;

    /// Does a report need a dialog?
    static bool needsDialog(const BScheduleReport &report);

private:
    QPointer<QWidget> m_parent;
};

#endif // BSCHEDULEREVIEWDIALOG_H
