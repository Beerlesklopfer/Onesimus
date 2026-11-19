/**
 * @file bschedulepalette.cpp
 * @brief The palette of the schedule grid: brushes, eraser, paint mode
 */

#include "schedules/bschedulepalette.h"

#include "schedules/bschedulegriddialogs.h"
#include "schedules/bschedulegrideditor.h"
#include "schedules/bschedulegridwidget.h"
#include "schedules/model/bschedulegridtext.h"

#include <QButtonGroup>
#include <QEvent>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

#include <functional>

namespace {

/// Places its items in rows, as many as the width needs
class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(QWidget *parent, int spacing)
        : QLayout(parent), m_spacing(spacing)
    {
        setContentsMargins(0, 0, 0, 0);
    }

    ~FlowLayout() override
    {
        while (QLayoutItem *item = takeAt(0))
            delete item;
    }

    void addItem(QLayoutItem *item) override { m_items.append(item); }
    int count() const override { return int(m_items.size()); }
    QLayoutItem *itemAt(int index) const override { return m_items.value(index); }
    QLayoutItem *takeAt(int index) override
    {
        return index >= 0 && index < m_items.size() ? m_items.takeAt(index) : nullptr;
    }

    Qt::Orientations expandingDirections() const override { return {}; }
    bool hasHeightForWidth() const override { return true; }
    int heightForWidth(int width) const override { return place(QRect(0, 0, width, 0), false); }

    void setGeometry(const QRect &rect) override
    {
        QLayout::setGeometry(rect);
        place(rect, true);
    }

    QSize sizeHint() const override { return minimumSize(); }
    QSize minimumSize() const override
    {
        QSize size;
        for (const QLayoutItem *item : m_items)
            size = size.expandedTo(item->minimumSize());
        return size;
    }

private:
    int place(const QRect &rect, bool apply) const
    {
        int x = rect.x();
        int y = rect.y();
        int rowHeight = 0;
        for (QLayoutItem *item : m_items) {
            if (item->isEmpty())
                continue;
            const QSize size = item->sizeHint();
            if (x > rect.x() && x + size.width() > rect.right() + 1) {
                x = rect.x();
                y += rowHeight + m_spacing;
                rowHeight = 0;
            }
            if (apply)
                item->setGeometry(QRect(QPoint(x, y), size));
            x += size.width() + m_spacing;
            rowHeight = qMax(rowHeight, size.height());
        }
        return y + rowHeight - rect.y();
    }

    QList<QLayoutItem *> m_items;
    int m_spacing;
};

QToolButton *toolButton(QWidget *parent, bool checkable)
{
    auto *button = new QToolButton(parent);
    button->setCheckable(checkable);
    button->setAutoRaise(false);
    button->setFocusPolicy(Qt::StrongFocus);
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    // The theme marks the checked one
    button->setProperty("paletteButton", true);
    return button;
}

} // namespace

BSchedulePalette::BSchedulePalette(BScheduleGridEditor *editor, BScheduleGridWidget *grid,
                                   QWidget *parent)
    : QWidget(parent)
    , m_editor(editor)
    , m_grid(grid)
    , m_tools(new QWidget(this))
    , m_flow(new FlowLayout(m_tools, 6))
    , m_group(new QButtonGroup(this))
    , m_eraser(toolButton(m_tools, true))
    , m_newBrush(toolButton(m_tools, false))
    , m_paintMode(toolButton(m_tools, true))
    , m_keepRuns(toolButton(m_tools, true))
    , m_active(new QLabel(this))
{
    setAccessibleName(tr("Brushes"));
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->addWidget(m_tools);
    layout->addWidget(m_active);
    m_active->setTextFormat(Qt::PlainText);
    m_active->setWordWrap(true);
    m_active->setObjectName(QStringLiteral("schedulePaletteActive"));

    m_group->setExclusive(true);

    m_eraser->setText(tr("Eraser"));
    m_eraser->setAccessibleName(tr("Eraser"));
    m_eraser->setToolTip(tr("Removes the runs of the scope on display from cells. "
                            "Del erases the selected cells with any tool in hand."));

    m_newBrush->setText(tr("New brush..."));
    m_newBrush->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_newBrush->setToolTip(tr("Defines a brush: level, minute of the hour and overrides"));

    m_paintMode->setText(tr("Paint mode"));
    m_paintMode->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_paintMode->setToolTip(tr("On: pressing a cell and dragging over cells applies the tool at "
                               "once. Off: the mouse selects, and Space applies the tool to the "
                               "selection. The key P switches it in the grid."));

    m_keepRuns->setText(tr("Keep other runs"));
    m_keepRuns->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_keepRuns->setToolTip(tr("On: painting adds the brush to the runs a cell already has, so a "
                              "cell can have several runs in one hour. Off: the cell ends up with "
                              "the brush only. Shift+Space adds once without this switch."));

    connect(m_group, &QButtonGroup::idClicked, this, [this](int id) {
        if (id < 0)
            m_editor->setEraser();
        else
            m_editor->setActiveBrush(id);
    });
    connect(m_newBrush, &QToolButton::clicked, this, [this]() { openBrushDialog(); });
    connect(m_paintMode, &QToolButton::toggled, m_editor, &BScheduleGridEditor::setPaintMode);
    connect(m_keepRuns, &QToolButton::toggled, m_editor, &BScheduleGridEditor::setKeepOtherRuns);
    connect(m_editor, &BScheduleGridEditor::paintModeChanged, m_paintMode, &QToolButton::setChecked);
    connect(m_editor, &BScheduleGridEditor::keepOtherRunsChanged, m_keepRuns, &QToolButton::setChecked);
    connect(m_editor, &BScheduleGridEditor::paintModeChanged, this, &BSchedulePalette::updateActive);
    connect(m_editor, &BScheduleGridEditor::keepOtherRunsChanged, this, &BSchedulePalette::updateActive);
    connect(m_editor, &BScheduleGridEditor::brushesChanged, this, &BSchedulePalette::rebuild);
    connect(m_editor, &BScheduleGridEditor::toolChanged, this, &BSchedulePalette::updateActive);

    m_paintMode->setChecked(m_editor->paintMode());
    m_keepRuns->setChecked(m_editor->keepOtherRuns());
    rebuild();
}

QAbstractButton *BSchedulePalette::eraserButton() const { return m_eraser; }
QAbstractButton *BSchedulePalette::newBrushButton() const { return m_newBrush; }
QAbstractButton *BSchedulePalette::paintModeButton() const { return m_paintMode; }
QAbstractButton *BSchedulePalette::keepRunsButton() const { return m_keepRuns; }

QAbstractButton *BSchedulePalette::activeButton() const
{
    return m_group->checkedButton();
}

QString BSchedulePalette::activeText() const
{
    return m_active->text();
}

QString BSchedulePalette::brushLabel(int index) const
{
    const BGridBrush brush = m_editor->brushes().value(index);
    const QString tag = m_grid->brushTag(brush);
    const QString text = BScheduleGridText::brushText(brush);
    return tag.isEmpty() ? text : tr("%1 = %2").arg(tag, text);
}

void BSchedulePalette::rebuild()
{
    if (m_rebuilding)
        return;
    m_rebuilding = true;
    const bool hadFocus = m_tools->isAncestorOf(focusWidget()) && focusWidget()->hasFocus()
        && m_group->buttons().contains(qobject_cast<QAbstractButton *>(focusWidget()));

    // The layout is filled again in order: brushes, eraser, new brush, switches
    while (QLayoutItem *item = m_flow->takeAt(0))
        delete item;
    for (QAbstractButton *button : std::as_const(m_brushButtons)) {
        m_group->removeButton(button);
        // Not at once: the change may come from a key of the button
        button->hide();
        button->deleteLater();
    }
    m_brushButtons.clear();
    m_group->removeButton(m_eraser);

    const QColor frame = palette().color(QPalette::Mid);
    const qreal ratio = devicePixelRatioF();
    // As large as the grid draws a run with its font
    const QSize swatchSize = m_grid->swatchSize();
    for (QToolButton *button : {m_eraser, m_newBrush, m_paintMode, m_keepRuns})
        button->setIconSize(swatchSize);
    auto swatch = [&](const std::function<void(QPainter &, const QRect &)> &draw) {
        QPixmap pixmap(swatchSize * ratio);
        pixmap.setDevicePixelRatio(ratio);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        const QRect rect(QPoint(0, 0), swatchSize);
        draw(painter, rect);
        painter.setPen(frame);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect.adjusted(0, 0, -1, -1));
        return QIcon(pixmap);
    };

    const QList<BGridBrush> &brushes = m_editor->brushes();
    for (int i = 0; i < brushes.size(); ++i) {
        QToolButton *button = toolButton(m_tools, true);
        button->setIconSize(swatchSize);
        const QString label = brushLabel(i);
        button->setText(label);
        button->setAccessibleName(tr("Brush %1").arg(label));
        button->setToolTip(tr("Takes this brush in hand: %1. Space applies it to the selected cells.")
                               .arg(BScheduleGridText::brushText(brushes[i])));
        button->setIcon(swatch([&](QPainter &painter, const QRect &rect) {
            BGridRun run;
            run.overrides = brushes[i].overrides;
            run.minute = brushes[i].minute;
            m_grid->paintRun(painter, rect, run, true);
        }));
        m_group->addButton(button, i);
        m_brushButtons.append(button);
        m_flow->addWidget(button);
        button->show();
    }

    m_eraser->setIcon(swatch([&](QPainter &painter, const QRect &rect) {
        painter.fillRect(rect, m_grid->palette().color(QPalette::Base));
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(m_grid->palette().color(QPalette::Text), 1.5));
        const int dx = m_grid->px(9);
        const int dy = m_grid->px(5);
        painter.drawLine(rect.topLeft() + QPoint(dx, dy), rect.bottomRight() - QPoint(dx, dy));
        painter.drawLine(rect.topRight() + QPoint(-dx, dy), rect.bottomLeft() - QPoint(-dx, dy));
        painter.setRenderHint(QPainter::Antialiasing, false);
    }));
    m_group->addButton(m_eraser, -2);   // not -1: that id means "give me one"
    m_flow->addWidget(m_eraser);
    m_flow->addWidget(m_newBrush);
    m_flow->addWidget(m_paintMode);
    m_flow->addWidget(m_keepRuns);

    // The keyboard goes through them in the order they stand in
    QWidget *before = nullptr;
    for (int i = 0; i < m_flow->count(); ++i) {
        QWidget *widget = m_flow->itemAt(i)->widget();
        if (before && widget)
            setTabOrder(before, widget);
        before = widget;
    }

    m_rebuilding = false;
    m_flow->invalidate();
    updateGeometry();
    updateActive();
    if (hadFocus) {
        if (QAbstractButton *button = m_group->checkedButton())
            button->setFocus();
    }
}

void BSchedulePalette::updateActive()
{
    QAbstractButton *button = nullptr;
    if (m_editor->tool() == BScheduleGridEditor::Eraser)
        button = m_eraser;
    else
        button = m_brushButtons.value(m_editor->activeBrushIndex());
    if (button && !button->isChecked())
        button->setChecked(true);

    QString text;
    if (m_editor->tool() == BScheduleGridEditor::Eraser) {
        text = tr("In hand: the eraser.");
    } else if (m_editor->activeBrush()) {
        text = tr("In hand: the brush %1.").arg(brushLabel(m_editor->activeBrushIndex()));
    }
    text += QLatin1Char(' ');
    if (m_editor->paintMode()) {
        text += tr("Paint mode: press a cell or drag over cells to apply it.");
    } else {
        text += tr("Space applies it to the selected cells, Del erases them.");
    }
    if (m_editor->keepOtherRuns() && m_editor->tool() == BScheduleGridEditor::Brush)
        text += QLatin1Char(' ') + tr("Other runs of a painted cell stay.");
    m_active->setText(text);
}

BScheduleBrushDialog *BSchedulePalette::openBrushDialog()
{
    // One that was closed is on its way out
    if (m_dialog && m_dialog->isVisible()) {
        m_dialog->raise();
        m_dialog->activateWindow();
        return m_dialog;
    }
    auto *dialog = new BScheduleBrushDialog(window());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowModality(Qt::WindowModal);
    // From the brush in hand, or from the one that was in hand before the eraser
    const BGridBrush start = m_editor->brushes().value(m_editor->activeBrushIndex());
    dialog->setBrush(start);
    connect(dialog, &QDialog::accepted, this, [this, dialog]() {
        m_editor->setActiveBrush(dialog->brush());
    });
    m_dialog = dialog;
    dialog->open();
    return dialog;
}

void BSchedulePalette::refresh()
{
    rebuild();
}

void BSchedulePalette::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::PaletteChange || event->type() == QEvent::StyleChange
        || event->type() == QEvent::FontChange) {
        rebuild();
    }
}
