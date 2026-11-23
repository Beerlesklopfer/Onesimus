// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bschedulegriddialogs.cpp
 * @brief The small forms of the schedule grid: overrides, a new brush, a new scope
 */

#include "schedules/bschedulegriddialogs.h"

#include "schedules/model/bschedulegridtext.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

namespace {

std::optional<QString> textOf(const QLineEdit *edit)
{
    const QString text = edit->text().trimmed();
    return text.isEmpty() ? std::nullopt : std::optional<QString>(text);
}

void setText(QLineEdit *edit, const std::optional<QString> &value)
{
    edit->setText(value ? *value : QString());
}

/// "not set", "yes", "no"
QComboBox *yesNoCombo(QWidget *parent)
{
    auto *combo = new QComboBox(parent);
    combo->addItem(BRunOverridesEditor::tr("not set"));
    combo->addItem(BRunOverridesEditor::tr("yes"));
    combo->addItem(BRunOverridesEditor::tr("no"));
    return combo;
}

std::optional<bool> yesNoOf(const QComboBox *combo)
{
    if (combo->currentIndex() == 1)
        return true;
    if (combo->currentIndex() == 2)
        return false;
    return std::nullopt;
}

void setYesNo(QComboBox *combo, const std::optional<bool> &value)
{
    combo->setCurrentIndex(!value ? 0 : (*value ? 1 : 2));
}

QStringList &knownPools()
{
    static QStringList names;
    return names;
}

QStringList &knownStorages()
{
    static QStringList names;
    return names;
}

/// A pick list: "not set" and the names the Director reported
QComboBox *nameCombo(const QStringList &names, const QString &what, QWidget *parent)
{
    auto *combo = new QComboBox(parent);
    combo->setEditable(false);
    // Wide enough for its names; a name the Director did not report adds
    // a remark that has to be readable as well
    int longest = 0;
    for (const QString &name : names)
        longest = qMax(longest, int(name.size()));
    combo->setMinimumContentsLength(qBound(22, longest + 2, 40));
    combo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    combo->addItem(BRunOverridesEditor::tr("(job's setting)"), QString());
    for (const QString &name : names)
        combo->addItem(name, name);
    combo->setToolTip(names.isEmpty()
        ? BRunOverridesEditor::tr("The Director has not reported any %1 yet; refresh the view after "
                                  "connecting. Not set: the setting of the job applies.").arg(what)
        : BRunOverridesEditor::tr("The %1 the Director reported. Not set: the setting of the job applies.")
              .arg(what));
    return combo;
}

std::optional<QString> nameOf(const QComboBox *combo)
{
    const QString name = combo->currentData().toString();
    return name.isEmpty() ? std::nullopt : std::optional<QString>(name);
}

} // namespace

// ============================================================================
// BRunOverridesEditor
// ============================================================================

BRunOverridesEditor::BRunOverridesEditor(QWidget *parent)
    : QWidget(parent)
    , m_level(new QComboBox(this))
    , m_pool(nameCombo(knownPools(), tr("pools"), this))
    , m_fullPool(nameCombo(knownPools(), tr("pools"), this))
    , m_incrementalPool(nameCombo(knownPools(), tr("pools"), this))
    , m_differentialPool(nameCombo(knownPools(), tr("pools"), this))
    , m_nextPool(nameCombo(knownPools(), tr("pools"), this))
    , m_storage(nameCombo(knownStorages(), tr("storages"), this))
    , m_messages(new QLineEdit(this))
    , m_maxRunSchedTime(new QLineEdit(this))
    , m_havePriority(new QCheckBox(tr("set to"), this))
    , m_priority(new QSpinBox(this))
    , m_spoolData(yesNoCombo(this))
    , m_accurate(yesNoCombo(this))
{
    // The level of the job first, then the usual four, then the rest
    m_level->addItem(tr("Level of the job"), QString());
    QStringList levels{QStringLiteral("Full"), QStringLiteral("Differential"),
                       QStringLiteral("Incremental"), QStringLiteral("VirtualFull")};
    for (const QString &level : BRunParser::levelNames()) {
        if (!levels.contains(level))
            levels.append(level);
    }
    for (const QString &level : std::as_const(levels))
        m_level->addItem(level, level);
    m_level->setToolTip(tr("The level the Run line names. \"Level of the job\": the line names "
                           "none and the level of the job applies."));

    m_priority->setRange(1, 9999);
    m_priority->setValue(10);
    m_priority->setEnabled(false);
    m_priority->setAccessibleName(tr("Priority"));
    m_havePriority->setAccessibleName(tr("Override the priority"));
    m_maxRunSchedTime->setPlaceholderText(tr("a time period, e.g. 8 hours"));
    const QString optional = tr("Empty: the setting of the job applies");
    for (QLineEdit *edit : {m_messages, m_maxRunSchedTime}) {
        edit->setToolTip(optional);
        edit->setClearButtonEnabled(true);
        connect(edit, &QLineEdit::textChanged, this, &BRunOverridesEditor::changed);
    }
    for (QComboBox *combo : {m_pool, m_fullPool, m_incrementalPool, m_differentialPool, m_nextPool,
                             m_storage}) {
        connect(combo, &QComboBox::currentIndexChanged, this, &BRunOverridesEditor::changed);
    }

    auto *priorityRow = new QWidget(this);
    auto *priorityLayout = new QHBoxLayout(priorityRow);
    priorityLayout->setContentsMargins(0, 0, 0, 0);
    priorityLayout->addWidget(m_havePriority);
    priorityLayout->addWidget(m_priority, 1);

    auto *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    int row = 0;
    auto add = [&](int column, const QString &label, QWidget *field) {
        auto *text = new QLabel(label, this);
        text->setBuddy(field);
        layout->addWidget(text, row, column * 2);
        layout->addWidget(field, row, column * 2 + 1);
    };
    add(0, tr("&Level:"), m_level);
    add(1, tr("&Storage:"), m_storage);
    ++row;
    add(0, tr("&Pool:"), m_pool);
    add(1, tr("&Messages:"), m_messages);
    ++row;
    add(0, tr("&Full pool:"), m_fullPool);
    add(1, tr("P&riority:"), priorityRow);
    ++row;
    add(0, tr("&Incremental pool:"), m_incrementalPool);
    add(1, tr("Sp&ool data:"), m_spoolData);
    ++row;
    add(0, tr("&Differential pool:"), m_differentialPool);
    add(1, tr("&Accurate:"), m_accurate);
    ++row;
    add(0, tr("&Next pool:"), m_nextPool);
    add(1, tr("Max run sched &time:"), m_maxRunSchedTime);
    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(3, 1);
    layout->setColumnMinimumWidth(1, 140);
    layout->setColumnMinimumWidth(3, 140);

    connect(m_level, &QComboBox::currentIndexChanged, this, &BRunOverridesEditor::changed);
    connect(m_spoolData, &QComboBox::currentIndexChanged, this, &BRunOverridesEditor::changed);
    connect(m_accurate, &QComboBox::currentIndexChanged, this, &BRunOverridesEditor::changed);
    connect(m_priority, &QSpinBox::valueChanged, this, &BRunOverridesEditor::changed);
    connect(m_havePriority, &QCheckBox::toggled, this, [this](bool on) {
        m_priority->setEnabled(on);
        emit changed();
    });
}

void BRunOverridesEditor::setOverrides(const BRunOverrides &overrides)
{
    const QSignalBlocker blocker(this);
    int level = overrides.level ? m_level->findData(*overrides.level) : 0;
    if (level < 0) {
        // A level this list does not know is kept as it is
        m_level->addItem(*overrides.level, *overrides.level);
        level = m_level->count() - 1;
    }
    m_level->setCurrentIndex(level);
    choose(m_pool, overrides.pool.value_or(QString()));
    choose(m_fullPool, overrides.fullPool.value_or(QString()));
    choose(m_incrementalPool, overrides.incrementalPool.value_or(QString()));
    choose(m_differentialPool, overrides.differentialPool.value_or(QString()));
    choose(m_nextPool, overrides.nextPool.value_or(QString()));
    choose(m_storage, overrides.storage.value_or(QString()));
    setText(m_messages, overrides.messages);
    setText(m_maxRunSchedTime, overrides.maxRunSchedTime);
    m_havePriority->setChecked(overrides.priority.has_value());
    m_priority->setEnabled(overrides.priority.has_value());
    if (overrides.priority)
        m_priority->setValue(*overrides.priority);
    setYesNo(m_spoolData, overrides.spoolData);
    setYesNo(m_accurate, overrides.accurate);
}

BRunOverrides BRunOverridesEditor::overrides() const
{
    BRunOverrides overrides;
    const QString level = m_level->currentData().toString();
    if (!level.isEmpty())
        overrides.level = level;
    overrides.pool = nameOf(m_pool);
    overrides.fullPool = nameOf(m_fullPool);
    overrides.incrementalPool = nameOf(m_incrementalPool);
    overrides.differentialPool = nameOf(m_differentialPool);
    overrides.nextPool = nameOf(m_nextPool);
    overrides.storage = nameOf(m_storage);
    overrides.messages = textOf(m_messages);
    overrides.maxRunSchedTime = textOf(m_maxRunSchedTime);
    if (m_havePriority->isChecked())
        overrides.priority = m_priority->value();
    overrides.spoolData = yesNoOf(m_spoolData);
    overrides.accurate = yesNoOf(m_accurate);
    return overrides;
}

void BRunOverridesEditor::setResourceNames(const QStringList &pools, const QStringList &storages)
{
    knownPools() = pools;
    knownStorages() = storages;
}

QStringList BRunOverridesEditor::poolNames()
{
    return knownPools();
}

QStringList BRunOverridesEditor::storageNames()
{
    return knownStorages();
}

void BRunOverridesEditor::choose(QComboBox *combo, const QString &name)
{
    if (name.isEmpty()) {
        combo->setCurrentIndex(0);
        return;
    }
    int index = combo->findData(name);
    if (index < 0) {
        // Named by the Run line, not reported by the Director: kept as it is
        const QString text = tr("%1 (not reported)").arg(name);
        combo->addItem(text, name);
        combo->setItemData(combo->count() - 1, tr("%1 is named by the Run line, but the Director did not report "
                                                  "it. It is kept as it is.").arg(name), Qt::ToolTipRole);
        combo->setMinimumContentsLength(qMax(combo->minimumContentsLength(), qMin(int(text.size()) + 2, 48)));
        index = combo->count() - 1;
    }
    combo->setCurrentIndex(index);
}

// ============================================================================
// BScheduleBrushDialog
// ============================================================================

BScheduleBrushDialog::BScheduleBrushDialog(QWidget *parent)
    : QDialog(parent)
    , m_overrides(new BRunOverridesEditor(this))
    , m_minute(new QSpinBox(this))
    , m_preview(new QLabel(this))
    , m_error(new QLabel(this))
    , m_buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    setWindowTitle(tr("New brush"));
    auto *layout = new QVBoxLayout(this);

    auto *intro = new QLabel(tr("A brush is what painting puts into a cell: a level, the minute "
                                "of the hour and, if wanted, settings that replace those of the job."),
                             this);
    intro->setWordWrap(true);
    layout->addWidget(intro);

    auto *minuteRow = new QFormLayout();
    m_minute->setRange(0, 59);
    m_minute->setPrefix(QStringLiteral(":"));
    m_minute->setAccessibleName(tr("Minute of the hour"));
    m_minute->setToolTip(tr("The run starts at this minute of every painted hour"));
    minuteRow->addRow(tr("Minute of the &hour:"), m_minute);
    layout->addLayout(minuteRow);

    auto *box = new QGroupBox(tr("Level and overrides"), this);
    auto *boxLayout = new QVBoxLayout(box);
    boxLayout->addWidget(m_overrides);
    layout->addWidget(box);

    m_preview->setTextFormat(Qt::PlainText);
    m_preview->setWordWrap(true);
    m_preview->setObjectName(QStringLiteral("scheduleGridPreview"));
    layout->addWidget(m_preview);
    m_error->setTextFormat(Qt::PlainText);
    m_error->setWordWrap(true);
    m_error->setObjectName(QStringLiteral("scheduleGridNotice"));
    layout->addWidget(m_error);
    layout->addWidget(m_buttons);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_overrides, &BRunOverridesEditor::changed, this, &BScheduleBrushDialog::updatePreview);
    connect(m_minute, &QSpinBox::valueChanged, this, &BScheduleBrushDialog::updatePreview);
    updatePreview();
}

void BScheduleBrushDialog::setBrush(const BGridBrush &brush)
{
    m_overrides->setOverrides(brush.overrides);
    m_minute->setValue(brush.minute);
    updatePreview();
}

BGridBrush BScheduleBrushDialog::brush() const
{
    return BGridBrush{m_overrides->overrides(), m_minute->value()};
}

bool BScheduleBrushDialog::isValid(QString *error) const
{
    // As a line it would make: Monday at midnight
    BRunLine line;
    BRunSpec spec;
    spec.overrides = m_overrides->overrides();
    spec.minute = m_minute->value();
    spec.mask.wday = 1u << 1;
    spec.mask.hour = 1u;
    line.spec = spec;
    line.dirty = true;
    QString reason;
    const bool ok = !BRunWriter::lineText(line, &reason).isEmpty();
    if (error)
        *error = ok ? QString() : reason;
    return ok;
}

void BScheduleBrushDialog::updatePreview()
{
    QString error;
    const bool ok = isValid(&error);
    m_preview->setText(tr("Brush: %1").arg(BScheduleGridText::brushText(brush())));
    m_error->setText(error);
    m_error->setVisible(!ok);
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(ok);
}

// ============================================================================
// BScheduleScopeDialog
// ============================================================================

BScheduleScopeDialog::BScheduleScopeDialog(const QList<BGridScopeInfo> &existing, QWidget *parent)
    : QDialog(parent)
    , m_existing(existing)
    , m_last(new QCheckBox(tr("&last (the last seven days of a month)"), this))
    , m_note(new QLabel(this))
    , m_buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    setWindowTitle(tr("Add scope"));
    auto *layout = new QVBoxLayout(this);

    auto *intro = new QLabel(tr("A scope is the part of the calendar a grid is painted for: "
                                "weeks of the month, and months if not all. What is painted in "
                                "it becomes Run lines such as \"1st sat at 21:00\"."), this);
    intro->setWordWrap(true);
    layout->addWidget(intro);

    auto *weeksBox = new QGroupBox(tr("Weeks of the month"), this);
    auto *weeksLayout = new QGridLayout(weeksBox);
    for (int week = 0; week < 5; ++week) {
        BDateTimeMask mask = BDateTimeMask::all();
        mask.wom = quint8(1u << week);
        auto *box = new QCheckBox(QStringLiteral("&%1").arg(BRunWriter::weekOfMonthText(mask)), weeksBox);
        box->setToolTip(tr("Days %1 to %2 of a month").arg(week * 7 + 1).arg(qMin(week * 7 + 7, 31)));
        m_weeks.append(box);
        weeksLayout->addWidget(box, 0, week);
        connect(box, &QCheckBox::toggled, this, &BScheduleScopeDialog::updateState);
    }
    m_last->setParent(weeksBox);
    weeksLayout->addWidget(m_last, 1, 0, 1, 5);
    connect(m_last, &QCheckBox::toggled, this, &BScheduleScopeDialog::updateState);
    layout->addWidget(weeksBox);

    auto *monthsBox = new QGroupBox(tr("Months"), this);
    auto *monthsLayout = new QGridLayout(monthsBox);
    for (int month = 0; month < 12; ++month) {
        auto *box = new QCheckBox(BScheduleGridText::monthName(month + 1), monthsBox);
        box->setChecked(true);
        m_months.append(box);
        monthsLayout->addWidget(box, month / 4, month % 4);
        connect(box, &QCheckBox::toggled, this, &BScheduleScopeDialog::updateState);
    }
    auto *allMonths = new QPushButton(tr("&All months"), monthsBox);
    monthsLayout->addWidget(allMonths, 3, 0);
    connect(allMonths, &QPushButton::clicked, this, [this]() {
        for (QCheckBox *box : std::as_const(m_months))
            box->setChecked(true);
    });
    layout->addWidget(monthsBox);

    m_note->setTextFormat(Qt::PlainText);
    m_note->setWordWrap(true);
    layout->addWidget(m_note);
    layout->addWidget(m_buttons);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    m_weeks.first()->setChecked(true);
    updateState();
}

void BScheduleScopeDialog::setScope(const BGridScope &scope)
{
    for (int week = 0; week < 5; ++week)
        m_weeks[week]->setChecked(scope.weeksOfMonth & (1u << week));
    m_last->setChecked(scope.last);
    for (int month = 0; month < 12; ++month)
        m_months[month]->setChecked(scope.months & (1u << month));
    updateState();
}

BGridScope BScheduleScopeDialog::scope() const
{
    BGridScope scope;
    scope.weeksOfMonth = 0;
    scope.months = 0;
    for (int week = 0; week < 5; ++week) {
        if (m_weeks[week]->isChecked())
            scope.weeksOfMonth |= quint8(1u << week);
    }
    scope.last = m_last->isChecked();
    for (int month = 0; month < 12; ++month) {
        if (m_months[month]->isChecked())
            scope.months |= quint16(1u << month);
    }
    return scope;
}

QString BScheduleScopeDialog::noteText() const
{
    return m_note->text();
}

void BScheduleScopeDialog::updateState()
{
    const BGridScope chosen = scope();
    bool ok = chosen.isValid();
    QString note;
    if (chosen.months == 0) {
        note = tr("Choose at least one month.");
    } else if (!ok) {
        note = tr("Choose at least one week of the month, or \"last\".");
    } else {
        bool exists = false;
        for (const BGridScopeInfo &info : std::as_const(m_existing))
            exists = exists || info.scope == chosen;
        const QString label = BScheduleGridText::scopeLabel(chosen);
        note = exists ? tr("The scope \"%1\" exists already; it will be shown.").arg(label)
                      : tr("New scope: %1").arg(label);
    }
    m_note->setText(note);
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(ok);
}
