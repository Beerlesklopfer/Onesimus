// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BRUNRULEDIALOG_H
#define BRUNRULEDIALOG_H

#include <QDialog>
#include <QList>

#include <functional>

#include "schedules/model/brunspec.h"

class QAbstractButton;
class QCheckBox;
class QDialogButtonBox;
class QGridLayout;
class QGroupBox;
class QLabel;
class QSpinBox;
class BRunOverridesEditor;

/**
 * @brief The form for a calendar rule
 *
 * A calendar rule is a Run line by day of the month ("on 1", "on 1/2") or
 * by week of the year ("w01/w02"). It does not belong to a weekday and
 * cannot be painted in the grid, so it is edited here as a whole: what
 * runs (level and overrides), the days of the month, the weeks of the
 * year, the other restrictions a Run line can carry (weekdays, weeks of
 * the month, months) and the time.
 *
 * Every mask of the Run line model has its own buttons, so a rule that is
 * opened is shown completely and nothing of it is lost by editing another
 * part. The form shows the Run line as it would be written and how it
 * reads. It can only be accepted while that line can be written and is a
 * calendar rule: without days of the month and weeks of the year the run
 * belongs to the grid.
 *
 * The form does not talk to a Director and changes nothing by itself; the
 * caller takes spec() and hands it to the editor.
 */
class BRunRuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BRunRuleDialog(QWidget *parent = nullptr);

    /// The day the row of weekdays starts with
    void setFirstDayOfWeek(Qt::DayOfWeek day);

    /// Shows a rule to edit. A new rule starts as "on the 1st at 21:00".
    void setSpec(const BRunSpec &spec);
    BRunSpec spec() const;

    /// Can it be accepted? Otherwise why not
    bool isValid(QString *error = nullptr) const;

    QString runText() const;        ///< The Run line as it would be written
    QString readingText() const;    ///< "on the 1st of every month at 21:00"

    // The parts, for tests and for the keyboard
    BRunOverridesEditor *overridesEditor() const { return m_overrides; }
    QGroupBox *daysBox() const { return m_daysBox; }
    QGroupBox *weeksBox() const { return m_weeksBox; }
    QList<QAbstractButton *> dayButtons() const { return m_days; }          ///< 1 ... 31
    QList<QAbstractButton *> weekOfYearButtons() const { return m_woys; }   ///< w00 ... w53
    QList<QAbstractButton *> hourButtons() const { return m_hours; }        ///< 00 ... 23
    QList<QCheckBox *> weekdayBoxes() const { return m_wdays; }             ///< By weekday of Bareos: Sunday first
    QList<QCheckBox *> weekOfMonthBoxes() const { return m_woms; }          ///< "1st" ... "5th"
    QCheckBox *lastBox() const { return m_last; }
    QList<QCheckBox *> monthBoxes() const { return m_months; }
    QCheckBox *everyHourBox() const { return m_everyHour; }
    QSpinBox *minuteBox() const { return m_minute; }
    QDialogButtonBox *buttons() const { return m_buttons; }

private:
    QList<QAbstractButton *> addMaskButtons(QGridLayout *layout, int count, int perRow,
                                            const std::function<QString(int)> &text,
                                            const std::function<QString(int)> &name);
    void addQuickButtons(QGridLayout *layout, int row, const QList<QAbstractButton *> &buttons,
                         int firstNumber);
    void layoutWeekdays();
    void updatePreview();

    Qt::DayOfWeek m_firstDay = Qt::Monday;

    BRunOverridesEditor *m_overrides;
    QGroupBox *m_daysBox;
    QGroupBox *m_weeksBox;
    QList<QAbstractButton *> m_days;
    QList<QAbstractButton *> m_woys;
    QList<QAbstractButton *> m_hours;
    QGridLayout *m_wdayLayout = nullptr;
    QList<QCheckBox *> m_wdays;
    QList<QCheckBox *> m_woms;
    QCheckBox *m_last;
    QList<QCheckBox *> m_months;
    QCheckBox *m_everyHour;
    QSpinBox *m_minute;
    QLabel *m_runText;
    QLabel *m_reading;
    QLabel *m_error;
    QDialogButtonBox *m_buttons;
};

#endif // BRUNRULEDIALOG_H
