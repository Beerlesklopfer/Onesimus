// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bschedulegrideditor.cpp
 * @brief Editing a schedule in the grid: the tool in hand and the undo stack
 */

#include "schedules/bschedulegrideditor.h"

#include "schedules/model/bschedulegridtext.h"

#include <QUndoCommand>
#include <QUndoStack>

namespace {

/// An edit of Run lines: the lines before and after it
class RunLinesCommand : public QUndoCommand
{
public:
    RunLinesCommand(BScheduleGridEditor *editor, const BGridChange &change, const QString &text,
                    const std::optional<BGridScope> &scope)
        : QUndoCommand(text), m_editor(editor), m_before(change.before), m_after(change.after),
          m_scope(scope)
    {
    }

    void undo() override
    {
        apply(m_before);
        emit m_editor->edited(BScheduleGridEditor::tr("Undone: %1").arg(text()));
    }

    void redo() override
    {
        // The edit has been made when the command is pushed
        if (m_first) {
            m_first = false;
            return;
        }
        apply(m_after);
        emit m_editor->edited(BScheduleGridEditor::tr("Redone: %1").arg(text()));
    }

private:
    void apply(const BGridSnapshot &snapshot)
    {
        BScheduleGridModel *model = m_editor->model();
        model->restore(snapshot);
        // What changed is where the edit was made
        if (m_scope) {
            const int index = model->indexOfScope(*m_scope);
            if (index >= 0)
                model->setActiveScope(index);
        }
    }

    BScheduleGridEditor *m_editor;
    BGridSnapshot m_before;
    BGridSnapshot m_after;
    std::optional<BGridScope> m_scope;
    bool m_first = true;
};

/// A scope without Run lines comes or goes
class ScopeCommand : public QUndoCommand
{
public:
    ScopeCommand(BScheduleGridEditor *editor, const BGridScope &scope, bool add, const QString &text)
        : QUndoCommand(text), m_editor(editor), m_scope(scope), m_add(add),
          m_shownBefore(editor->model()->activeScope())
    {
    }

    void undo() override
    {
        change(!m_add);
        emit m_editor->edited(BScheduleGridEditor::tr("Undone: %1").arg(text()));
    }

    void redo() override
    {
        change(m_add);
        if (m_first)
            m_first = false;
        else
            emit m_editor->edited(BScheduleGridEditor::tr("Redone: %1").arg(text()));
    }

private:
    void change(bool add)
    {
        BScheduleGridModel *model = m_editor->model();
        if (add) {
            const int index = model->addScope(m_scope);
            // A removed scope comes back as it went: shown, if it was
            if (index >= 0 && (m_add || m_shownBefore == m_scope))
                model->setActiveScope(index);
        } else {
            model->removeScope(model->indexOfScope(m_scope));
            const int before = model->indexOfScope(m_shownBefore);
            if (m_add && before >= 0)
                model->setActiveScope(before);
        }
    }

    BScheduleGridEditor *m_editor;
    BGridScope m_scope;
    bool m_add;
    BGridScope m_shownBefore;
    bool m_first = true;
};

} // namespace

BScheduleGridEditor::BScheduleGridEditor(BScheduleGridModel *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_undo(new QUndoStack(this))
{
    connect(m_model, &BScheduleGridModel::scheduleReset, this, &BScheduleGridEditor::onScheduleReset);
    connect(m_model, &BScheduleGridModel::runLinesChanged, this, &BScheduleGridEditor::collectBrushes);
    onScheduleReset();
}

void BScheduleGridEditor::onScheduleReset()
{
    m_undo->clear();
    m_lastError.clear();
    m_brushes = m_model->brushes();
    if (m_brushes.isEmpty()) {
        // Nothing to pick up from the schedule: the three usual levels
        for (const char *level : {"Full", "Differential", "Incremental"}) {
            BGridBrush brush;
            brush.overrides.level = QString::fromLatin1(level);
            m_brushes.append(brush);
        }
    }
    m_tool = Brush;
    m_activeBrush = 0;
    emit brushesChanged();
    emit toolChanged();
}

void BScheduleGridEditor::collectBrushes()
{
    bool grown = false;
    for (const BGridBrush &brush : m_model->brushes()) {
        if (!m_brushes.contains(brush)) {
            m_brushes.append(brush);
            grown = true;
        }
    }
    if (grown)
        emit brushesChanged();
}

// --- The tool -------------------------------------------------------------------

int BScheduleGridEditor::addBrush(const BGridBrush &brush)
{
    int index = int(m_brushes.indexOf(brush));
    if (index < 0) {
        m_brushes.append(brush);
        index = int(m_brushes.size()) - 1;
        emit brushesChanged();
    }
    return index;
}

std::optional<BGridBrush> BScheduleGridEditor::activeBrush() const
{
    if (m_tool != Brush || m_activeBrush < 0 || m_activeBrush >= m_brushes.size())
        return std::nullopt;
    return m_brushes[m_activeBrush];
}

void BScheduleGridEditor::setActiveBrush(int index)
{
    if (index < 0 || index >= m_brushes.size())
        return;
    if (m_tool == Brush && index == m_activeBrush)
        return;
    m_tool = Brush;
    m_activeBrush = index;
    emit toolChanged();
}

void BScheduleGridEditor::setActiveBrush(const BGridBrush &brush)
{
    setActiveBrush(addBrush(brush));
}

void BScheduleGridEditor::setEraser()
{
    if (m_tool == Eraser)
        return;
    m_tool = Eraser;
    emit toolChanged();
}

QString BScheduleGridEditor::toolText() const
{
    if (m_tool == Eraser)
        return tr("Eraser");
    const std::optional<BGridBrush> brush = activeBrush();
    return brush ? BScheduleGridText::brushText(*brush) : QString();
}

BGridBrush BScheduleGridEditor::levelBrush(const QString &level) const
{
    BGridBrush brush;
    brush.overrides.level = level;
    if (m_activeBrush >= 0 && m_activeBrush < m_brushes.size())
        brush.minute = m_brushes[m_activeBrush].minute;
    return brush;
}

QString BScheduleGridEditor::levelOfKey(int key)
{
    switch (key) {
    case Qt::Key_F: return QStringLiteral("Full");
    case Qt::Key_D: return QStringLiteral("Differential");
    case Qt::Key_I: return QStringLiteral("Incremental");
    case Qt::Key_V: return QStringLiteral("VirtualFull");
    default: break;
    }
    return QString();
}

void BScheduleGridEditor::setPaintMode(bool on)
{
    if (on == m_paintMode)
        return;
    m_paintMode = on;
    emit paintModeChanged(on);
}

void BScheduleGridEditor::setKeepOtherRuns(bool on)
{
    if (on == m_keepOtherRuns)
        return;
    m_keepOtherRuns = on;
    emit keepOtherRunsChanged(on);
}

// --- Edits ----------------------------------------------------------------------

bool BScheduleGridEditor::isLocked() const
{
    return !m_model->notUnderstood().isEmpty();
}

bool BScheduleGridEditor::refuseLocked()
{
    if (!isLocked())
        return false;
    m_lastError = tr("This schedule contains a Run line that is not understood. It is locked: "
                     "nothing in it can be changed here.");
    emit editFailed(m_lastError);
    return true;
}

bool BScheduleGridEditor::pushChange(const BGridChange &change, const QString &text,
                                     bool inActiveScope)
{
    if (!change.ok) {
        m_lastError = change.error;
        emit editFailed(m_lastError);
        return false;
    }
    m_lastError.clear();
    if (change.isNoop())
        return true;
    std::optional<BGridScope> scope;
    if (inActiveScope)
        scope = m_model->activeScope();
    m_undo->push(new RunLinesCommand(this, change, text, scope));
    emit edited(text);
    return true;
}

QString BScheduleGridEditor::cellCountText(const BGridCellSet &cells) const
{
    QString text = tr("%n cell(s)", "", cells.count());
    if (!m_model->activeScope().isEveryWeek()) {
        text = tr("%1 of the scope %2").arg(text, BScheduleGridText::scopeLabel(m_model->activeScope()));
    }
    return text;
}

bool BScheduleGridEditor::paint(const BGridCellSet &cells, const BGridBrush &brush)
{
    if (refuseLocked())
        return false;
    const BScheduleGridModel::PaintMode mode = m_keepOtherRuns ? BScheduleGridModel::AddRun
                                                               : BScheduleGridModel::ReplaceRuns;
    return pushChange(m_model->applyBrush(cells, brush, mode),
                      tr("Paint %1 on %2").arg(BScheduleGridText::brushText(brush), cellCountText(cells)),
                      true);
}

bool BScheduleGridEditor::paintBeside(const BGridCellSet &cells, const BGridBrush &brush)
{
    if (refuseLocked())
        return false;
    return pushChange(m_model->applyBrush(cells, brush, BScheduleGridModel::AddRun),
                      tr("Add %1 to %2").arg(BScheduleGridText::brushText(brush), cellCountText(cells)),
                      true);
}

bool BScheduleGridEditor::erase(const BGridCellSet &cells)
{
    if (refuseLocked())
        return false;
    return pushChange(m_model->erase(cells), tr("Erase %1").arg(cellCountText(cells)), true);
}

bool BScheduleGridEditor::applyTool(const BGridCellSet &cells)
{
    if (refuseLocked())
        return false;
    if (cells.isEmpty()) {
        m_lastError = tr("No cell is selected.");
        emit editFailed(m_lastError);
        return false;
    }
    if (m_tool == Eraser)
        return erase(cells);
    const std::optional<BGridBrush> brush = activeBrush();
    if (!brush) {
        m_lastError = tr("No brush is chosen.");
        emit editFailed(m_lastError);
        return false;
    }
    return paint(cells, *brush);
}

bool BScheduleGridEditor::applyToolToSelection()
{
    return applyTool(m_model->selection().cells());
}

bool BScheduleGridEditor::eraseSelection()
{
    if (refuseLocked())
        return false;
    const BGridCellSet cells = m_model->selection().cells();
    if (cells.isEmpty()) {
        m_lastError = tr("No cell is selected.");
        emit editFailed(m_lastError);
        return false;
    }
    return erase(cells);
}

bool BScheduleGridEditor::paintLevel(const QString &level)
{
    if (refuseLocked())
        return false;
    const BGridBrush brush = levelBrush(level);
    setActiveBrush(brush);
    const BGridCellSet cells = m_model->selection().cells();
    if (cells.isEmpty()) {
        m_lastError.clear();
        return true;    // The brush is in hand; nothing to paint
    }
    return paint(cells, brush);
}

int BScheduleGridEditor::addScope(const BGridScope &scope)
{
    if (refuseLocked())
        return -1;
    if (!scope.isValid()) {
        m_lastError = tr("A scope needs at least one month, and a week of the month or \"last\".");
        emit editFailed(m_lastError);
        return -1;
    }
    m_lastError.clear();
    const int existing = m_model->indexOfScope(scope);
    if (existing >= 0) {
        m_model->setActiveScope(existing);
        return existing;
    }
    const QString text = tr("Add the scope %1").arg(BScheduleGridText::scopeLabel(scope));
    m_undo->push(new ScopeCommand(this, scope, true, text));
    emit edited(text);
    return m_model->indexOfScope(scope);
}

bool BScheduleGridEditor::removeScope(int index)
{
    if (refuseLocked())
        return false;
    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    if (index < 0 || index >= scopes.size())
        return false;
    const BGridScopeInfo info = scopes[index];
    if (info.scope.isEveryWeek()) {
        m_lastError = tr("The scope \"%1\" is always there.").arg(BScheduleGridText::scopeLabel(info.scope));
        emit editFailed(m_lastError);
        return false;
    }
    if (info.runCount > 0) {
        m_lastError = tr("The scope %1 still has Run lines. Erase its cells first.")
                          .arg(BScheduleGridText::scopeLabel(info.scope));
        emit editFailed(m_lastError);
        return false;
    }
    m_lastError.clear();
    const QString text = tr("Remove the scope %1").arg(BScheduleGridText::scopeLabel(info.scope));
    m_undo->push(new ScopeCommand(this, info.scope, false, text));
    emit edited(text);
    return true;
}

bool BScheduleGridEditor::addCalendarRule(const BRunSpec &spec)
{
    if (refuseLocked())
        return false;
    return pushChange(m_model->addCalendarRule(spec), tr("Add a calendar rule"), false);
}

bool BScheduleGridEditor::changeCalendarRule(int ruleIndex, const BRunSpec &spec)
{
    if (refuseLocked())
        return false;
    const QList<BGridCalendarRule> &rules = m_model->calendarRules();
    if (ruleIndex < 0 || ruleIndex >= rules.size())
        return false;
    return pushChange(m_model->changeCalendarRule(rules[ruleIndex].runIndex, spec),
                      tr("Change the calendar rule %1").arg(BScheduleGridText::ruleTag(ruleIndex)),
                      false);
}

bool BScheduleGridEditor::removeCalendarRule(int ruleIndex)
{
    if (refuseLocked())
        return false;
    const QList<BGridCalendarRule> &rules = m_model->calendarRules();
    if (ruleIndex < 0 || ruleIndex >= rules.size())
        return false;
    return pushChange(m_model->removeCalendarRule(rules[ruleIndex].runIndex),
                      tr("Remove the calendar rule %1").arg(BScheduleGridText::ruleTag(ruleIndex)),
                      false);
}

void BScheduleGridEditor::undo()
{
    m_undo->undo();
}

void BScheduleGridEditor::redo()
{
    m_undo->redo();
}

void BScheduleGridEditor::revertAll()
{
    m_undo->setIndex(0);
}
