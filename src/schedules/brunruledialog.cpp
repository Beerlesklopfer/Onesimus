// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file brunruledialog.cpp
 * @brief The form for a calendar rule
 */

#include "schedules/brunruledialog.h"
#include "buiscale.h"

#include "schedules/bschedulegriddialogs.h"
#include "schedules/model/bschedulegridmodel.h"
#include "schedules/model/bschedulegridtext.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <QVBoxLayout>

namespace {

/**
 * The keyboard in a block of mask buttons: Tab reaches the block once (its
 * first button) and leaves it again; the arrow keys, Home and End move
 * inside it. Space toggles, as on every button.
 */
class MaskKeys : public QObject
{
public:
    MaskKeys(const QList<QAbstractButton *> &buttons, int perRow, QObject *parent)
        : QObject(parent), m_buttons(buttons), m_perRow(perRow)
    {
        for (int i = 0; i < m_buttons.size(); ++i) {
            m_buttons[i]->setFocusPolicy(i == 0 ? Qt::StrongFocus : Qt::ClickFocus);
            m_buttons[i]->installEventFilter(this);
        }
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() != QEvent::KeyPress)
            return false;
        const int index = int(m_buttons.indexOf(static_cast<QAbstractButton *>(watched)));
        if (index < 0)
            return false;
        const int count = int(m_buttons.size());
        int target = index;
        switch (static_cast<QKeyEvent *>(event)->key()) {
        case Qt::Key_Left: target = index - 1; break;
        case Qt::Key_Right: target = index + 1; break;
        case Qt::Key_Up: target = index - m_perRow; break;
        case Qt::Key_Down: target = index + m_perRow; break;
        case Qt::Key_Home: target = 0; break;
        case Qt::Key_End: target = count - 1; break;
        default: return false;
        }
        if (target >= 0 && target < count && m_buttons[target]->isEnabled())
            m_buttons[target]->setFocus(Qt::TabFocusReason);
        return true;
    }

private:
    QList<QAbstractButton *> m_buttons;
    int m_perRow;
};

quint64 bitsOf(const QList<QAbstractButton *> &buttons)
{
    quint64 bits = 0;
    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons[i]->isChecked())
            bits |= Q_UINT64_C(1) << i;
    }
    return bits;
}

void setBits(const QList<QAbstractButton *> &buttons, quint64 bits)
{
    for (int i = 0; i < buttons.size(); ++i)
        buttons[i]->setChecked(bits & (Q_UINT64_C(1) << i));
}

} // namespace

BRunRuleDialog::BRunRuleDialog(QWidget *parent)
    : QDialog(parent)
    , m_overrides(new BRunOverridesEditor(this))
    , m_daysBox(new QGroupBox(tr("Only on these da&ys of the month"), this))
    , m_weeksBox(new QGroupBox(tr("Only in these &weeks of the year"), this))
    , m_last(new QCheckBox(tr("last"), this))
    , m_everyHour(new QCheckBox(tr("&Every hour"), this))
    , m_minute(new QSpinBox(this))
    , m_runText(new QLabel(this))
    , m_reading(new QLabel(this))
    , m_error(new QLabel(this))
    , m_buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    setWindowTitle(tr("Calendar rule"));
    auto *layout = new QVBoxLayout(this);

    auto *intro = new QLabel(tr("A calendar rule runs by day of the month or by week of the year. "
                                "It does not belong to a weekday and is not painted in the grid."),
                             this);
    intro->setWordWrap(true);
    layout->addWidget(intro);

    // --- What runs ---
    auto *whatBox = new QGroupBox(tr("What runs"), this);
    auto *whatLayout = new QVBoxLayout(whatBox);
    whatLayout->addWidget(m_overrides);
    layout->addWidget(whatBox);

    // --- Days of the month and weeks of the year, side by side ---
    auto *calendarRow = new QHBoxLayout();
    m_daysBox->setCheckable(true);
    m_daysBox->setToolTip(tr("Not checked: every day of the month"));
    auto *daysLayout = new QGridLayout(m_daysBox);
    daysLayout->setSpacing(2);
    m_days = addMaskButtons(daysLayout, 31, 8,
                            [](int i) { return QString::number(i + 1); },
                            [](int i) { return tr("Day %1").arg(i + 1); });
    addQuickButtons(daysLayout, 4, m_days, 1);
    calendarRow->addWidget(m_daysBox);

    m_weeksBox->setCheckable(true);
    m_weeksBox->setToolTip(tr("Not checked: every week of the year. The weeks are numbered w00 to "
                              "w53 as in a Run line."));
    auto *weeksLayout = new QGridLayout(m_weeksBox);
    weeksLayout->setSpacing(2);
    m_woys = addMaskButtons(weeksLayout, 54, 9,
                            [](int i) { return QStringLiteral("%1").arg(i, 2, 10, QLatin1Char('0')); },
                            [](int i) { return tr("Week %1").arg(i); });
    addQuickButtons(weeksLayout, 6, m_woys, 0);
    calendarRow->addWidget(m_weeksBox);
    layout->addLayout(calendarRow);

    // --- The other restrictions of a Run line ---
    auto *alsoBox = new QGroupBox(tr("And only"), this);
    auto *alsoLayout = new QGridLayout(alsoBox);
    alsoLayout->addWidget(new QLabel(tr("on these weekdays:"), alsoBox), 0, 0);
    auto *wdayRow = new QWidget(alsoBox);
    m_wdayLayout = new QGridLayout(wdayRow);
    m_wdayLayout->setContentsMargins(0, 0, 0, 0);
    m_wdayLayout->setHorizontalSpacing(14);
    for (int weekday = 0; weekday < 7; ++weekday) {
        const Qt::DayOfWeek day = weekday == 0 ? Qt::Sunday : Qt::DayOfWeek(weekday);
        auto *box = new QCheckBox(BScheduleGridText::shortDayName(day), wdayRow);
        box->setAccessibleName(BScheduleGridText::dayName(day));
        box->setChecked(true);
        m_wdays.append(box);
        connect(box, &QCheckBox::toggled, this, &BRunRuleDialog::updatePreview);
    }
    layoutWeekdays();
    alsoLayout->addWidget(wdayRow, 0, 1);

    alsoLayout->addWidget(new QLabel(tr("in these weeks of the month:"), alsoBox), 1, 0);
    auto *womRow = new QWidget(alsoBox);
    auto *womLayout = new QHBoxLayout(womRow);
    womLayout->setContentsMargins(0, 0, 0, 0);
    womLayout->setSpacing(14);
    for (int week = 0; week < 5; ++week) {
        BDateTimeMask mask = BDateTimeMask::all();
        mask.wom = quint8(1u << week);
        auto *box = new QCheckBox(BRunWriter::weekOfMonthText(mask), womRow);
        box->setChecked(true);
        m_woms.append(box);
        womLayout->addWidget(box);
        connect(box, &QCheckBox::toggled, this, &BRunRuleDialog::updatePreview);
    }
    m_last->setParent(womRow);
    m_last->setToolTip(tr("The last seven days of a month"));
    womLayout->addWidget(m_last);
    womLayout->addStretch(1);
    connect(m_last, &QCheckBox::toggled, this, &BRunRuleDialog::updatePreview);
    alsoLayout->addWidget(womRow, 1, 1);

    alsoLayout->addWidget(new QLabel(tr("in these months:"), alsoBox), 2, 0, Qt::AlignTop);
    auto *monthRow = new QWidget(alsoBox);
    auto *monthLayout = new QGridLayout(monthRow);
    monthLayout->setContentsMargins(0, 0, 0, 0);
    monthLayout->setHorizontalSpacing(14);
    for (int month = 0; month < 12; ++month) {
        auto *box = new QCheckBox(BScheduleGridText::monthName(month + 1), monthRow);
        box->setChecked(true);
        m_months.append(box);
        monthLayout->addWidget(box, month / 6, month % 6);
        connect(box, &QCheckBox::toggled, this, &BRunRuleDialog::updatePreview);
    }
    alsoLayout->addWidget(monthRow, 2, 1);
    alsoLayout->setColumnStretch(1, 1);
    layout->addWidget(alsoBox);

    // --- Time ---
    auto *timeBox = new QGroupBox(tr("Time"), this);
    auto *timeLayout = new QGridLayout(timeBox);
    timeLayout->setSpacing(2);
    m_hours = addMaskButtons(timeLayout, 24, 12,
                             [](int i) { return QStringLiteral("%1").arg(i, 2, 10, QLatin1Char('0')); },
                             [](int i) { return tr("%1 hours").arg(i); });
    auto *timeRow = new QHBoxLayout();
    timeRow->setSpacing(8);
    m_everyHour->setToolTip(tr("The run starts in every hour of the day (\"hourly\")"));
    timeRow->addWidget(m_everyHour);
    auto *minuteLabel = new QLabel(tr("&Minute of the hour:"), timeBox);
    m_minute->setRange(0, 59);
    m_minute->setPrefix(QStringLiteral(":"));
    m_minute->setAccessibleName(tr("Minute of the hour"));
    minuteLabel->setBuddy(m_minute);
    timeRow->addWidget(minuteLabel);
    timeRow->addWidget(m_minute);
    timeRow->addStretch(1);
    timeLayout->addLayout(timeRow, 2, 0, 1, 12);
    layout->addWidget(timeBox);

    // --- The line it makes ---
    BUiScale::bind(m_runText, BUiScale::Normal, BUiScale::Monospace);
    m_runText->setTextFormat(Qt::PlainText);
    m_runText->setWordWrap(true);
    m_runText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_runText->setAccessibleName(tr("Run line"));
    m_reading->setTextFormat(Qt::PlainText);
    m_reading->setWordWrap(true);
    m_error->setTextFormat(Qt::PlainText);
    m_error->setWordWrap(true);
    m_error->setObjectName(QStringLiteral("scheduleGridNotice"));
    layout->addWidget(m_runText);
    layout->addWidget(m_reading);
    layout->addWidget(m_error);
    layout->addWidget(m_buttons);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_overrides, &BRunOverridesEditor::changed, this, &BRunRuleDialog::updatePreview);
    connect(m_daysBox, &QGroupBox::toggled, this, &BRunRuleDialog::updatePreview);
    connect(m_weeksBox, &QGroupBox::toggled, this, &BRunRuleDialog::updatePreview);
    connect(m_minute, &QSpinBox::valueChanged, this, &BRunRuleDialog::updatePreview);
    connect(m_everyHour, &QCheckBox::toggled, this, [this](bool on) {
        for (QAbstractButton *button : std::as_const(m_hours))
            button->setEnabled(!on);
        updatePreview();
    });

    // A new rule: on the 1st at 21:00
    BRunSpec first;
    first.mask.mday = 1u;
    first.mask.hour = 1u << 21;
    setSpec(first);
}

QList<QAbstractButton *> BRunRuleDialog::addMaskButtons(QGridLayout *layout, int count, int perRow,
                                                        const std::function<QString(int)> &text,
                                                        const std::function<QString(int)> &name)
{
    QList<QAbstractButton *> buttons;
    const int width = fontMetrics().horizontalAdvance(QStringLiteral("00")) + 16;
    for (int i = 0; i < count; ++i) {
        auto *button = new QToolButton(layout->parentWidget());
        button->setCheckable(true);
        button->setText(text(i));
        button->setAccessibleName(name(i));
        button->setMinimumWidth(width);
        // The theme draws a checked one filled
        button->setProperty("maskButton", true);
        layout->addWidget(button, i / perRow, i % perRow);
        buttons.append(button);
        connect(button, &QToolButton::toggled, this, &BRunRuleDialog::updatePreview);
    }
    new MaskKeys(buttons, perRow, this);
    return buttons;
}

void BRunRuleDialog::addQuickButtons(QGridLayout *layout, int row,
                                     const QList<QAbstractButton *> &buttons, int firstNumber)
{
    auto *quick = new QHBoxLayout();
    quick->setSpacing(6);
    auto add = [&](const QString &text, const std::function<bool(int)> &checked) {
        auto *button = new QPushButton(text, layout->parentWidget());
        quick->addWidget(button);
        connect(button, &QPushButton::clicked, this, [buttons, checked, firstNumber]() {
            for (int i = 0; i < buttons.size(); ++i)
                buttons[i]->setChecked(checked(i + firstNumber));
        });
    };
    add(tr("Odd"), [](int number) { return number % 2 == 1; });
    add(tr("Even"), [](int number) { return number % 2 == 0; });
    add(tr("None"), [](int) { return false; });
    quick->addStretch(1);
    layout->addLayout(quick, row, 0, 1, layout->columnCount());
}

void BRunRuleDialog::layoutWeekdays()
{
    for (QCheckBox *box : std::as_const(m_wdays))
        m_wdayLayout->removeWidget(box);
    for (int position = 0; position < 7; ++position) {
        // Qt counts Monday = 1 ... Sunday = 7, Bareos Sunday = 0
        const int weekday = (int(m_firstDay) % 7 + position) % 7;
        m_wdayLayout->addWidget(m_wdays[weekday], 0, position);
    }
    m_wdayLayout->setColumnStretch(7, 1);
    // The keyboard follows the row
    for (int position = 1; position < 7; ++position) {
        setTabOrder(m_wdays[(int(m_firstDay) % 7 + position - 1) % 7],
                    m_wdays[(int(m_firstDay) % 7 + position) % 7]);
    }
}

void BRunRuleDialog::setFirstDayOfWeek(Qt::DayOfWeek day)
{
    if (day < Qt::Monday || day > Qt::Sunday || day == m_firstDay)
        return;
    m_firstDay = day;
    layoutWeekdays();
}

void BRunRuleDialog::setSpec(const BRunSpec &spec)
{
    const QSignalBlocker blocker(this);
    m_overrides->setOverrides(spec.overrides);

    const quint32 mdays = spec.mask.mday & BDateTimeMask::AllMdays;
    m_daysBox->setChecked(mdays != BDateTimeMask::AllMdays);
    setBits(m_days, mdays != BDateTimeMask::AllMdays ? mdays : 0);

    const quint64 woys = spec.mask.woy & BDateTimeMask::AllWoys;
    m_weeksBox->setChecked(woys != BDateTimeMask::AllWoys);
    setBits(m_woys, woys != BDateTimeMask::AllWoys ? woys : 0);

    for (int weekday = 0; weekday < 7; ++weekday)
        m_wdays[weekday]->setChecked(spec.mask.wday & (1u << weekday));
    for (int week = 0; week < 5; ++week)
        m_woms[week]->setChecked(spec.mask.wom & (1u << week));
    m_last->setChecked(spec.mask.last7DaysOfMonth);
    for (int month = 0; month < 12; ++month)
        m_months[month]->setChecked(spec.mask.month & (1u << month));

    const quint32 hours = spec.mask.hour & BDateTimeMask::AllHours;
    m_everyHour->setChecked(hours == BDateTimeMask::AllHours);
    setBits(m_hours, hours != BDateTimeMask::AllHours ? hours : 0);
    for (QAbstractButton *button : std::as_const(m_hours))
        button->setEnabled(hours != BDateTimeMask::AllHours);
    m_minute->setValue(spec.minute);
    updatePreview();
}

BRunSpec BRunRuleDialog::spec() const
{
    BRunSpec result;
    result.overrides = m_overrides->overrides();
    result.minute = m_minute->value();
    result.mask.mday = m_daysBox->isChecked() ? quint32(bitsOf(m_days)) : BDateTimeMask::AllMdays;
    result.mask.woy = m_weeksBox->isChecked() ? bitsOf(m_woys) : BDateTimeMask::AllWoys;
    result.mask.hour = m_everyHour->isChecked() ? BDateTimeMask::AllHours : quint32(bitsOf(m_hours));
    result.mask.wday = 0;
    for (int weekday = 0; weekday < 7; ++weekday) {
        if (m_wdays[weekday]->isChecked())
            result.mask.wday |= quint8(1u << weekday);
    }
    result.mask.wom = 0;
    for (int week = 0; week < 5; ++week) {
        if (m_woms[week]->isChecked())
            result.mask.wom |= quint8(1u << week);
    }
    result.mask.last7DaysOfMonth = m_last->isChecked();
    result.mask.month = 0;
    for (int month = 0; month < 12; ++month) {
        if (m_months[month]->isChecked())
            result.mask.month |= quint16(1u << month);
    }
    return result;
}

bool BRunRuleDialog::isValid(QString *error) const
{
    if (error)
        error->clear();
    const BRunSpec made = spec();
    if (!BScheduleGridModel::isCalendarRule(made)) {
        if (error) {
            *error = tr("Choose days of the month or weeks of the year. A run without them "
                        "belongs to the grid and is painted there.");
        }
        return false;
    }
    BRunLine line;
    line.spec = made;
    line.dirty = true;
    QString reason;
    if (BRunWriter::lineText(line, &reason).isEmpty()) {
        if (error)
            *error = reason;
        return false;
    }
    return true;
}

QString BRunRuleDialog::runText() const
{
    return m_runText->text();
}

QString BRunRuleDialog::readingText() const
{
    return m_reading->text();
}

void BRunRuleDialog::updatePreview()
{
    QString error;
    const bool ok = isValid(&error);
    const BRunSpec made = spec();
    const QString text = BRunWriter::write(made);
    m_runText->setText(text.isEmpty() ? QString() : tr("Run = %1").arg(text));
    m_runText->setVisible(!text.isEmpty());

    QString reading;
    if (ok) {
        // As the grid reads the line that is written
        const BRunLine parsed = BRunParser::parse(text);
        if (parsed.spec) {
            BGridCalendarRule rule;
            rule.spec = *parsed.spec;
            rule.text = text;
            rule.byDayOfMonth = (made.mask.mday & BDateTimeMask::AllMdays) != BDateTimeMask::AllMdays;
            rule.byWeekOfYear = (made.mask.woy & BDateTimeMask::AllWoys) != BDateTimeMask::AllWoys;
            rule.withoutTime = !parsed.spec->haveHour;
            reading = tr("%1 %2").arg(BScheduleGridText::levelName(made.overrides.level),
                                      BScheduleGridText::ruleReading(rule));
        }
    }
    m_reading->setText(reading);
    m_reading->setVisible(!reading.isEmpty());
    m_error->setText(error);
    m_error->setVisible(!ok);
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(ok);
}
