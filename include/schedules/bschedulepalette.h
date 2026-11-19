#ifndef BSCHEDULEPALETTE_H
#define BSCHEDULEPALETTE_H

#include <QList>
#include <QPointer>
#include <QWidget>

class QAbstractButton;
class QButtonGroup;
class QLabel;
class QLayout;
class QToolButton;
class BScheduleBrushDialog;
class BScheduleGridEditor;
class BScheduleGridWidget;

/**
 * @brief The palette of the schedule grid: the legend of brushes, made to choose from
 *
 * One button per brush the editor offers, drawn as the grid draws a run of
 * it and spelled out ("A = Full at :00, pool Monthly"), then the eraser and
 * "New brush...". The button of the tool in hand is checked; a line below
 * names the tool in words, because a checked button is told by colour.
 * Two switches follow: paint mode (dragging applies the tool) and "Keep
 * other runs" (painting adds to what a cell has).
 *
 * The buttons flow into as many rows as the width needs. They are ordinary
 * buttons: Tab reaches them, the arrow keys move among the tools, Space
 * presses, and a screen reader hears their names and which one is checked.
 *
 * The palette draws the brushes with the grid it is given, so both always
 * agree; it does not own the grid or the editor.
 */
class BSchedulePalette : public QWidget
{
    Q_OBJECT

public:
    BSchedulePalette(BScheduleGridEditor *editor, BScheduleGridWidget *grid, QWidget *parent = nullptr);

    QList<QAbstractButton *> brushButtons() const { return m_brushButtons; }
    QAbstractButton *eraserButton() const;
    QAbstractButton *newBrushButton() const;
    QAbstractButton *paintModeButton() const;
    QAbstractButton *keepRunsButton() const;

    /// The button of the tool in hand
    QAbstractButton *activeButton() const;

    /// The line that names the tool in hand
    QString activeText() const;

    /**
     * @brief Opens the "New brush" form
     *
     * It starts from the brush in hand. Accepting it offers the brush and
     * takes it in hand. The form is window modal and not waited for.
     */
    BScheduleBrushDialog *openBrushDialog();

    /// Draws the buttons again: the colours of the levels changed
    void refresh();

protected:
    void changeEvent(QEvent *event) override;

private:
    void rebuild();
    void updateActive();
    QString brushLabel(int index) const;

    BScheduleGridEditor *m_editor;
    BScheduleGridWidget *m_grid;

    QWidget *m_tools;
    QLayout *m_flow;
    QButtonGroup *m_group;
    QList<QAbstractButton *> m_brushButtons;
    QToolButton *m_eraser;
    QToolButton *m_newBrush;
    QToolButton *m_paintMode;
    QToolButton *m_keepRuns;
    QLabel *m_active;
    QPointer<BScheduleBrushDialog> m_dialog;
    bool m_rebuilding = false;
};

#endif // BSCHEDULEPALETTE_H
