/**
 * @file bschedulegridwidget.cpp
 * @brief The schedule grid: painting, selection, editing, tooltips, list of runs
 */

#include "schedules/bschedulegridwidget.h"

#include "schedules/bschedulegridaccessible.h"
#include "schedules/bschedulegrideditor.h"
#include "schedules/model/bschedulegridtext.h"

#include <QAccessible>
#include <QFrame>
#include <QHeaderView>
#include <QHelpEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QToolTip>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace {

const int MaxStripes = 3;
const int LoadBarHeight = 3;
const int RuleBandHeight = 22;

QColor mix(const QColor &a, const QColor &b, double share)
{
    return QColor(qRound(a.red() + (b.red() - a.red()) * share),
                  qRound(a.green() + (b.green() - a.green()) * share),
                  qRound(a.blue() + (b.blue() - a.blue()) * share));
}

QString runLineText(const BScheduleGridModel &model, int runIndex)
{
    for (const BGridRunLineText &line : model.runLines()) {
        if (line.runIndex == runIndex)
            return line.text;
    }
    return QString();
}

} // namespace

BScheduleGridWidget::BScheduleGridWidget(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    BScheduleGridAccessible::install();

    setFocusPolicy(Qt::StrongFocus);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    viewport()->setAutoFillBackground(false);
    setAccessibleName(tr("Schedule grid"));
    updateLayout();
}

BScheduleGridWidget::~BScheduleGridWidget()
{
    if (m_details)
        delete m_details.data();
}

void BScheduleGridWidget::setModel(BScheduleGridModel *model)
{
    if (model == m_model)
        return;
    if (m_model)
        disconnect(m_model, nullptr, this, nullptr);
    m_model = model;
    if (m_model) {
        connect(m_model, &BScheduleGridModel::scheduleReset, this, [this]() {
            // What was highlighted belongs to the lines as they were
            m_highlightedRun = -1;
            m_highlightedRule = -1;
            onModelReset();
        });
        connect(m_model, &BScheduleGridModel::cellsChanged, this, &BScheduleGridWidget::onCellsEdited);
        connect(m_model, &BScheduleGridModel::scopesChanged, this, &BScheduleGridWidget::onCellsChanged);
        connect(m_model, &BScheduleGridModel::runLinesChanged, this, &BScheduleGridWidget::onCellsChanged);
        connect(m_model, &BScheduleGridModel::activeScopeChanged, this, &BScheduleGridWidget::onModelReset);
        connect(m_model, &BScheduleGridModel::calendarRulesChanged, this, &BScheduleGridWidget::onModelReset);
        connect(m_model, &BScheduleGridModel::firstDayOfWeekChanged, this, &BScheduleGridWidget::onModelReset);
        connect(m_model, &BScheduleGridModel::selectionChanged,
                this, &BScheduleGridWidget::onSelectionChanged);
    }
    m_current = BGridPos();
    onModelReset();
}

void BScheduleGridWidget::setEditor(BScheduleGridEditor *editor)
{
    if (editor == m_editor)
        return;
    if (m_editor)
        disconnect(m_editor, nullptr, this, nullptr);
    m_editor = editor;
    m_drag = NoDrag;
    m_stroke = BGridCellSet();
    if (m_editor) {
        connect(m_editor, &BScheduleGridEditor::brushesChanged,
                this, &BScheduleGridWidget::onCellsChanged);
        connect(m_editor, &BScheduleGridEditor::toolChanged, this, &BScheduleGridWidget::onToolChanged);
        connect(m_editor, &BScheduleGridEditor::paintModeChanged,
                this, &BScheduleGridWidget::updateCursor);
    }
    rebuildCaches();
    updateCursor();
    viewport()->update();
}

void BScheduleGridWidget::setLevelColorSource(const LevelColorSource &source)
{
    m_levelColors = source;
    viewport()->update();
}

QColor BScheduleGridWidget::levelFill(const LevelColorSource &source, const QPalette &palette,
                                      const std::optional<QString> &level)
{
    const QString code = BScheduleGridText::levelColorCode(level);
    QColor color;
    if (!code.isEmpty()) {
        if (source) {
            color = source(code);
        } else if (code == QLatin1String("F")) {
            color = QColor(200, 220, 255);
        } else if (code == QLatin1String("I")) {
            color = QColor(200, 255, 200);
        } else if (code == QLatin1String("D")) {
            color = QColor(255, 240, 200);
        } else if (code == QLatin1String("V")) {
            color = QColor(230, 200, 255);
        }
    }
    // No level on the Run line (the job's level applies) or a level without a
    // colour: neutral, as in the timeline
    if (!color.isValid())
        color = palette.color(QPalette::Mid);
    return color;
}

QColor BScheduleGridWidget::levelFill(const std::optional<QString> &level) const
{
    return levelFill(m_levelColors, palette(), level);
}

QColor BScheduleGridWidget::inkOn(const QColor &background)
{
    const double luminance = 0.299 * background.red() + 0.587 * background.green()
                             + 0.114 * background.blue();
    return luminance > 140.0 ? QColor(Qt::black) : QColor(Qt::white);
}

// --- Model changes ------------------------------------------------------------

void BScheduleGridWidget::rebuildCaches()
{
    m_brushes.clear();
    m_brushTags.clear();
    for (QList<int> &rules : m_rulesAtHour)
        rules.clear();
    if (!m_model)
        return;
    // With an editor the brushes it offers come first and keep their place,
    // so a tag does not change while the schedule is edited
    if (m_editor && m_editor->model() == m_model)
        m_brushes = m_editor->brushes();
    for (const BGridBrush &brush : m_model->brushes()) {
        if (!m_brushes.contains(brush))
            m_brushes.append(brush);
    }
    m_brushTags = BScheduleGridText::brushTags(m_brushes);
    const QList<BGridCalendarRule> &rules = m_model->calendarRules();
    for (int i = 0; i < rules.size(); ++i) {
        for (int hour = 0; hour < BGridCellSet::Columns; ++hour) {
            if (rules[i].spec.mask.hour & (1u << hour))
                m_rulesAtHour[size_t(hour)].append(i);
        }
    }
}

void BScheduleGridWidget::onModelReset()
{
    if (m_details)
        m_details->close();
    m_drag = NoDrag;
    m_stroke = BGridCellSet();
    rebuildCaches();
    updateLayout();
    viewport()->update();
    notifyAccessibleReset();
}

void BScheduleGridWidget::onCellsChanged()
{
    // Scopes, Run lines or brushes changed: what cells say did not, or
    // cellsChanged() tells which
    rebuildCaches();
    viewport()->update();
}

void BScheduleGridWidget::onCellsEdited(const QList<BGridPos> &cells)
{
    rebuildCaches();
    viewport()->update();
    notifyAccessibleCells(cells);
}

void BScheduleGridWidget::onToolChanged()
{
    viewport()->update();
    notifyAccessibleTool();
}

void BScheduleGridWidget::updateCursor()
{
    if (m_editor && m_editor->paintMode())
        viewport()->setCursor(Qt::CrossCursor);
    else
        viewport()->unsetCursor();
}

void BScheduleGridWidget::onSelectionChanged()
{
    viewport()->update();
    notifyAccessibleSelection();
}

QString BScheduleGridWidget::brushTag(const BGridBrush &brush) const
{
    const int index = int(m_brushes.indexOf(brush));
    return index < 0 ? QString() : m_brushTags.value(index);
}

QList<int> BScheduleGridWidget::rulesAtHour(int hour) const
{
    if (hour < 0 || hour >= BGridCellSet::Columns)
        return {};
    return m_rulesAtHour[size_t(hour)];
}

// --- Layout -------------------------------------------------------------------

QFont BScheduleGridWidget::smallFont() const
{
    QFont small = font();
    if (small.pointSizeF() > 0)
        small.setPointSizeF(qMax(6.5, small.pointSizeF() * 0.75));
    else
        small.setPixelSize(qMax(9, small.pixelSize() * 3 / 4));
    return small;
}

QString BScheduleGridWidget::dayHeaderText(int row) const
{
    if (!m_model)
        return QString();
    return BScheduleGridText::dayName(m_model->dayOfWeekOfRow(row));
}

void BScheduleGridWidget::updateLayout()
{
    QFont bold = font();
    bold.setBold(true);
    const QFontMetrics metrics(bold);
    int headerWidth = metrics.horizontalAdvance(tr("Calendar rules"));
    for (int day = Qt::Monday; day <= Qt::Sunday; ++day) {
        headerWidth = qMax(headerWidth, metrics.horizontalAdvance(
                                            BScheduleGridText::dayName(Qt::DayOfWeek(day))));
    }
    m_geometry.setHeaderSize(headerWidth + 16, metrics.height() + 8);
    m_geometry.setRuleBandHeight(m_model && !m_model->calendarRules().isEmpty() ? RuleBandHeight : 0);
    m_geometry.setViewportSize(viewport()->size());

    const QSize range = m_geometry.scrollRange();
    horizontalScrollBar()->setRange(0, range.width());
    horizontalScrollBar()->setPageStep(m_geometry.scrollAreaRect().width());
    horizontalScrollBar()->setSingleStep(m_geometry.cellWidth());
    verticalScrollBar()->setRange(0, range.height());
    verticalScrollBar()->setPageStep(m_geometry.scrollAreaRect().height());
    verticalScrollBar()->setSingleStep(m_geometry.cellHeight());
    m_geometry.setScrollOffset(QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value()));
}

QSize BScheduleGridWidget::sizeHint() const
{
    return m_geometry.minimumContentSize() + QSize(2 * frameWidth(), 2 * frameWidth());
}

QSize BScheduleGridWidget::minimumSizeHint() const
{
    // Smaller than the content: then the cells scroll under the headers
    return QSize(m_geometry.rowHeaderWidth() + 8 * BScheduleGridGeometry::MinCellWidth,
                 m_geometry.columnHeaderHeight() + 4 * BScheduleGridGeometry::MinCellHeight);
}

void BScheduleGridWidget::resizeEvent(QResizeEvent *event)
{
    QAbstractScrollArea::resizeEvent(event);
    updateLayout();
}

void BScheduleGridWidget::scrollContentsBy(int, int)
{
    m_geometry.setScrollOffset(QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value()));
    viewport()->update();
}

void BScheduleGridWidget::changeEvent(QEvent *event)
{
    QAbstractScrollArea::changeEvent(event);
    if (event->type() == QEvent::FontChange || event->type() == QEvent::StyleChange
        || event->type() == QEvent::LanguageChange) {
        updateLayout();
        viewport()->update();
    } else if (event->type() == QEvent::PaletteChange) {
        viewport()->update();
    }
}

void BScheduleGridWidget::ensureCellVisible(const BGridPos &cell)
{
    const QRect rect = m_geometry.cellRect(cell.row, cell.column);
    const QRect area = m_geometry.scrollAreaRect();
    int dx = 0;
    int dy = 0;
    if (rect.left() < area.left())
        dx = rect.left() - area.left();
    else if (rect.right() > area.right())
        dx = rect.right() - area.right();
    if (rect.top() < area.top())
        dy = rect.top() - area.top();
    else if (rect.bottom() > area.bottom())
        dy = rect.bottom() - area.bottom();
    if (dx)
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + dx);
    if (dy)
        verticalScrollBar()->setValue(verticalScrollBar()->value() + dy);
}

// --- Current cell and highlights ------------------------------------------------

void BScheduleGridWidget::setCurrentCell(const BGridPos &cell)
{
    BGridPos bounded;
    bounded.row = qBound(0, cell.row, BGridCellSet::Rows - 1);
    bounded.column = qBound(0, cell.column, BGridCellSet::Columns - 1);
    if (bounded == m_current)
        return;
    m_current = bounded;
    ensureCellVisible(m_current);
    viewport()->update();
    emit currentCellChanged(m_current.row, m_current.column);
    notifyAccessibleFocus();
}

void BScheduleGridWidget::setHighlightedRun(int runIndex)
{
    if (runIndex == m_highlightedRun)
        return;
    m_highlightedRun = runIndex;
    viewport()->update();
}

void BScheduleGridWidget::setHighlightedRule(int ruleIndex)
{
    if (ruleIndex == m_highlightedRule)
        return;
    m_highlightedRule = ruleIndex;
    viewport()->update();
}

// --- Painting -------------------------------------------------------------------

QColor BScheduleGridWidget::gridLineColor() const
{
    return mix(palette().color(QPalette::Base), palette().color(QPalette::Text), 0.28);
}

QColor BScheduleGridWidget::selectionOutlineColor() const
{
    // The highlight colour of a dark theme is too dark for a line on its base
    const QColor highlight = palette().color(QPalette::Highlight);
    const bool darkBase = palette().color(QPalette::Base).lightness() < 128;
    if (darkBase)
        return highlight.lightness() < 150 ? highlight.lighter(210) : highlight;
    return highlight.lightness() > 150 ? highlight.darker(160) : highlight;
}

void BScheduleGridWidget::paintWarningMark(QPainter &painter, const QRect &rect, const QColor &ink) const
{
    // A triangle with an exclamation mark, 11 by 10 pixels
    const int left = rect.left() + 1;
    const int bottom = rect.bottom() - 1;
    QPainterPath triangle;
    triangle.moveTo(left, bottom);
    triangle.lineTo(left + 10, bottom);
    triangle.lineTo(left + 5, bottom - 9);
    triangle.closeSubpath();
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(ink, 1));
    painter.setBrush(QColor(0xf2, 0xb1, 0x1a));
    painter.drawPath(triangle);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(left + 5, bottom - 6, left + 5, bottom - 3);
    painter.drawPoint(left + 5, bottom - 1);
    painter.restore();
}

void BScheduleGridWidget::paintRun(QPainter &painter, const QRect &rect, const BGridRun &run,
                                   bool inScope, const QString &scopeBadge, int loadBar) const
{
    const QColor base = palette().color(QPalette::Base);
    const QColor text = palette().color(QPalette::Text);
    const QColor fill = levelFill(run.level());

    painter.save();
    painter.setClipRect(rect, Qt::IntersectClip);

    QColor ink;
    if (inScope) {
        painter.fillRect(rect, fill);
        ink = inkOn(fill);
    } else {
        painter.fillRect(rect, base);
        const QRect frame = rect.adjusted(0, 0, -1, -1);
        if (run.sharesDatesWithActiveScope) {
            // May run on a day of the active scope: tinted, hatched rising,
            // solid frame
            QColor tint = fill;
            tint.setAlpha(80);
            painter.fillRect(rect, tint);
            painter.fillRect(rect, QBrush(mix(fill, text, 0.5), Qt::BDiagPattern));
            painter.setPen(QPen(mix(fill, text, 0.5), 1, Qt::SolidLine));
            ink = text;
        } else {
            // Never on a day of the active scope: no tint, hatched falling,
            // dashed frame
            painter.fillRect(rect, QBrush(mix(base, text, 0.38), Qt::FDiagPattern));
            painter.setPen(QPen(mix(base, text, 0.55), 1, Qt::DashLine));
            ink = mix(text, base, 0.2);
        }
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(frame);
    }

    // The minute: a tick at minute/60 across the stripe
    const int tickX = rect.left() + qBound(0, qRound(run.minute / 60.0 * rect.width()),
                                           qMax(0, rect.width() - 2));
    painter.fillRect(QRect(tickX, rect.top(), 2, 5), ink);

    QFont letterFont = font();
    letterFont.setBold(true);
    const QFont small = smallFont();
    QFont smallBold = small;
    smallBold.setBold(true);
    const QFontMetrics smallMetrics(small);

    const QString letter = BScheduleGridText::levelLetter(run.level());
    if (QFontMetrics(letterFont).horizontalAdvance(letter) > rect.width() - 2)
        letterFont = smallBold;
    const QFontMetrics letterMetrics(letterFont);

    // Below the letter: the minute, or for a ghost its scope
    QString below;
    if (inScope) {
        if (run.minute != 0)
            below = QStringLiteral(":%1").arg(run.minute, 2, 10, QLatin1Char('0'));
    } else if (!run.withoutTime) {
        below = scopeBadge;
    }
    const int belowHeight = smallMetrics.height();
    const bool showBelow = !below.isEmpty()
        && smallMetrics.horizontalAdvance(below) <= rect.width() - 2
        && rect.height() >= letterMetrics.height() + belowHeight + loadBar + 6;

    QRect letterRect = rect.adjusted(0, 5, 0, -loadBar);
    if (showBelow)
        letterRect.setBottom(rect.bottom() - loadBar - belowHeight);
    if (!inScope) {
        // A plain patch under the letter keeps it readable on the hatching
        QRect patch(0, 0, letterMetrics.horizontalAdvance(letter) + 4, letterMetrics.height());
        patch.moveCenter(letterRect.center());
        painter.fillRect(patch.intersected(rect.adjusted(1, 1, -1, -1)), base);
    }
    painter.setFont(letterFont);
    painter.setPen(ink);
    painter.drawText(letterRect, Qt::AlignCenter, letter);

    if (showBelow) {
        const QRect belowRect(rect.left(), rect.bottom() - loadBar - belowHeight,
                              rect.width(), belowHeight);
        if (!inScope) {
            QRect patch(0, 0, smallMetrics.horizontalAdvance(below) + 2, belowHeight);
            patch.moveCenter(belowRect.center());
            painter.fillRect(patch.intersected(rect.adjusted(1, 1, -1, -1)), base);
        }
        painter.setFont(small);
        painter.drawText(belowRect, Qt::AlignCenter, below);
    }

    // The tag of a brush with overrides, top right
    const QString tag = brushTag(run.brush());
    if (!tag.isEmpty()) {
        const QFontMetrics tagMetrics(smallBold);
        QRect tagRect(0, 0, tagMetrics.horizontalAdvance(tag) + 4, tagMetrics.height());
        tagRect.moveTopRight(rect.topRight());
        painter.fillRect(tagRect, inScope ? ink : text);
        painter.setFont(smallBold);
        painter.setPen(inScope ? fill : base);
        painter.drawText(tagRect, Qt::AlignCenter, tag);
    }

    if (run.withoutTime)
        paintWarningMark(painter, rect.adjusted(0, 0, 0, -loadBar), ink);

    painter.restore();
}

void BScheduleGridWidget::paintRuleMark(QPainter &painter, const QRect &rect, const QString &text,
                                        const std::optional<QString> &level, bool highlighted) const
{
    QFont bold = smallFont();
    bold.setBold(true);
    const QFontMetrics metrics(bold);
    QRect pill(0, 0, qMin(rect.width() - 2, metrics.horizontalAdvance(text) + 10),
               qMin(rect.height() - 4, metrics.height() + 2));
    pill.moveCenter(rect.center());
    const QColor fill = levelFill(level);

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(palette().color(QPalette::Text), highlighted ? 2 : 1));
    painter.setBrush(fill);
    painter.drawRoundedRect(QRectF(pill).adjusted(0.5, 0.5, -0.5, -0.5),
                            pill.height() / 2.0, pill.height() / 2.0);
    painter.setFont(bold);
    painter.setPen(inkOn(fill));
    painter.drawText(pill, Qt::AlignCenter, text);
    painter.restore();
}

void BScheduleGridWidget::paintHeaders(QPainter &painter) const
{
    const QColor text = palette().color(QPalette::Text);
    const QColor headerFill = palette().color(QPalette::AlternateBase);
    const QColor selectedFill = mix(headerFill, palette().color(QPalette::Highlight), 0.7);
    const QColor selectedText = inkOn(selectedFill);
    const QColor line = gridLineColor();
    const BGridSelection selection = m_model ? m_model->selection() : BGridSelection();

    QFont plain = font();
    QFont bold = font();
    bold.setBold(true);

    // Column headers: the hours
    painter.save();
    painter.setClipRect(QRect(m_geometry.rowHeaderWidth(), 0,
                              m_geometry.scrollAreaRect().width(), m_geometry.columnHeaderHeight()));
    for (int column = 0; column < BGridCellSet::Columns; ++column) {
        const QRect rect = m_geometry.columnHeaderRect(column);
        const bool selected = selection.isColumnSelected(column);
        painter.fillRect(rect, selected ? selectedFill : headerFill);
        painter.setPen(line);
        painter.drawLine(rect.topRight(), rect.bottomRight());
        painter.drawLine(rect.bottomLeft(), rect.bottomRight());
        painter.setFont(selected || (hasFocus() && column == m_current.column) ? bold : plain);
        painter.setPen(selected ? selectedText : text);
        painter.drawText(rect, Qt::AlignCenter,
                         QStringLiteral("%1").arg(column, 2, 10, QLatin1Char('0')));
    }
    painter.restore();

    // Row headers: the weekdays, and the label of the band of calendar rules
    painter.save();
    painter.setClipRect(QRect(0, m_geometry.columnHeaderHeight(), m_geometry.rowHeaderWidth(),
                              m_geometry.scrollAreaRect().height()));
    for (int row = 0; row < BGridCellSet::Rows; ++row) {
        const QRect rect = m_geometry.rowHeaderRect(row);
        const bool selected = selection.isRowSelected(row);
        painter.fillRect(rect, selected ? selectedFill : headerFill);
        painter.setPen(line);
        painter.drawLine(rect.topRight(), rect.bottomRight());
        painter.drawLine(rect.bottomLeft(), rect.bottomRight());
        painter.setFont(selected || (hasFocus() && row == m_current.row) ? bold : plain);
        painter.setPen(selected ? selectedText : text);
        painter.drawText(rect.adjusted(8, 0, -4, 0), Qt::AlignLeft | Qt::AlignVCenter,
                         dayHeaderText(row));
    }
    if (m_geometry.hasRuleBand()) {
        const QRect rect = m_geometry.ruleHeaderRect();
        QFont italic = font();
        italic.setItalic(true);
        painter.setFont(italic);
        painter.setPen(text);
        painter.drawText(rect.adjusted(8, 0, -4, 0), Qt::AlignLeft | Qt::AlignVCenter,
                         tr("Calendar rules"));
    }
    painter.restore();

    // The corner: selects all
    const QRect corner = m_geometry.cornerRect();
    painter.fillRect(corner, headerFill);
    painter.setPen(line);
    painter.drawLine(corner.topRight(), corner.bottomRight());
    painter.drawLine(corner.bottomLeft(), corner.bottomRight());
    QPainterPath mark;
    mark.moveTo(corner.right() - 4, corner.bottom() - 12);
    mark.lineTo(corner.right() - 4, corner.bottom() - 4);
    mark.lineTo(corner.right() - 12, corner.bottom() - 4);
    mark.closeSubpath();
    painter.fillPath(mark, mix(headerFill, text, 0.55));
}

void BScheduleGridWidget::paintCell(QPainter &painter, int row, int column, double maxLoad) const
{
    const QRect rect = m_geometry.cellRect(row, column);
    const QRect inner = rect.adjusted(0, 0, -1, -1);    // without the grid lines right and below
    const QColor base = palette().color(QPalette::Base);
    const QColor text = palette().color(QPalette::Text);

    const BGridCell cell = m_model->cell(row, column);
    const int loadBar = cell.expectedConcurrentRuns ? LoadBarHeight : 0;

    struct Stripe
    {
        const BGridRun *run;
        bool inScope;
    };
    QList<Stripe> stripes;
    for (const BGridRun &run : cell.inScope)
        stripes.append(Stripe{&run, true});
    for (const BGridRun &run : cell.outOfScope)
        stripes.append(Stripe{&run, false});

    const int shown = int(qMin(qsizetype(MaxStripes), stripes.size()));
    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    int x = inner.left();
    for (int i = 0; i < shown; ++i) {
        const int width = (inner.width() - (x - inner.left())) / (shown - i);
        const QRect stripe(x, inner.top(), width, inner.height());
        const Stripe &s = stripes[i];
        const QString badge = s.inScope
            ? QString() : BScheduleGridText::scopeBadge(scopes.value(s.run->scopeIndex).scope);
        paintRun(painter, stripe, *s.run, s.inScope, badge, loadBar);
        if (i > 0) {
            // A line between stripes: two runs of one level are two runs
            painter.setPen(mix(base, text, 0.45));
            painter.drawLine(stripe.left(), stripe.top(), stripe.left(), stripe.bottom());
        }
        x += width;
    }

    QFont smallBold = smallFont();
    smallBold.setBold(true);

    // More runs than stripes
    if (stripes.size() > shown) {
        const QString more = QStringLiteral("+%1").arg(stripes.size() - shown);
        const QFontMetrics metrics(smallBold);
        QRect badge(0, 0, metrics.horizontalAdvance(more) + 4, metrics.height());
        badge.moveBottomRight(inner.bottomRight() - QPoint(0, loadBar));
        painter.fillRect(badge, text);
        painter.setFont(smallBold);
        painter.setPen(base);
        painter.drawText(badge, Qt::AlignCenter, more);
    }

    // The load of a later step: a thin bar at the bottom
    if (cell.expectedConcurrentRuns) {
        const QRect track(inner.left(), inner.bottom() - LoadBarHeight + 1, inner.width(), LoadBarHeight);
        painter.fillRect(track, mix(base, text, 0.22));
        const double share = maxLoad > 0 ? qBound(0.0, *cell.expectedConcurrentRuns / maxLoad, 1.0) : 0.0;
        painter.fillRect(QRect(track.left(), track.top(), qRound(track.width() * share), track.height()),
                         mix(base, text, 0.9));
    }
}

void BScheduleGridWidget::paintRuleBand(QPainter &painter) const
{
    if (!m_geometry.hasRuleBand() || !m_model)
        return;
    const QColor base = palette().color(QPalette::Base);
    const QColor text = palette().color(QPalette::Text);
    const QRect first = m_geometry.ruleCellRect(0);
    const QRect last = m_geometry.ruleCellRect(BGridCellSet::Columns - 1);
    const QRect band = first.united(last).adjusted(0, 0, -1, -1);

    // One strip without cell borders: these are not cells
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(mix(base, text, 0.5), 1, Qt::DotLine));
    painter.setBrush(palette().color(QPalette::AlternateBase));
    painter.drawRoundedRect(QRectF(band).adjusted(0.5, 0.5, -0.5, -0.5), 5, 5);
    painter.restore();

    const QList<BGridCalendarRule> &rules = m_model->calendarRules();
    for (int column = 0; column < BGridCellSet::Columns; ++column) {
        const QList<int> &here = m_rulesAtHour[size_t(column)];
        if (here.isEmpty())
            continue;
        QString mark = BScheduleGridText::ruleTag(here.first());
        if (here.size() > 1)
            mark += QStringLiteral("+%1").arg(here.size() - 1);
        paintRuleMark(painter, m_geometry.ruleCellRect(column), mark,
                      rules[here.first()].spec.overrides.level,
                      here.contains(m_highlightedRule));
    }
}

void BScheduleGridWidget::paintOutline(QPainter &painter, const BGridCellSet &cells, const QPen &pen) const
{
    // Only the edges between a cell of the set and one outside it
    painter.save();
    painter.setPen(pen);
    const int w = pen.width();
    for (const BGridPos &pos : cells.cells()) {
        const QRect r = m_geometry.cellRect(pos.row, pos.column).adjusted(0, 0, -1, -1);
        const int inset = w / 2;
        if (!cells.contains(pos.row - 1, pos.column))
            painter.drawLine(r.left(), r.top() + inset, r.right(), r.top() + inset);
        if (!cells.contains(pos.row + 1, pos.column))
            painter.drawLine(r.left(), r.bottom() - inset, r.right(), r.bottom() - inset);
        if (!cells.contains(pos.row, pos.column - 1))
            painter.drawLine(r.left() + inset, r.top(), r.left() + inset, r.bottom());
        if (!cells.contains(pos.row, pos.column + 1))
            painter.drawLine(r.right() - inset, r.top(), r.right() - inset, r.bottom());
    }
    painter.restore();
}

void BScheduleGridWidget::paintSelection(QPainter &painter) const
{
    const BGridCellSet &selected = m_model->selection().cells();
    if (selected.isEmpty())
        return;
    QColor wash = palette().color(QPalette::Highlight);
    wash.setAlpha(85);
    for (const BGridPos &pos : selected.cells())
        painter.fillRect(m_geometry.cellRect(pos.row, pos.column).adjusted(0, 0, -1, -1), wash);
    QPen pen(selectionOutlineColor(), 2);
    pen.setCapStyle(Qt::FlatCap);
    paintOutline(painter, selected, pen);
}

void BScheduleGridWidget::paintStroke(QPainter &painter) const
{
    if (m_stroke.isEmpty() || !m_editor)
        return;
    const QColor base = palette().color(QPalette::Base);
    const QColor text = palette().color(QPalette::Text);
    const std::optional<BGridBrush> brush = m_editor->activeBrush();
    for (const BGridPos &pos : m_stroke.cells()) {
        const QRect inner = m_geometry.cellRect(pos.row, pos.column).adjusted(0, 0, -1, -1);
        painter.save();
        painter.setOpacity(0.85);
        if (brush) {
            // What the cell will show
            BGridRun run;
            run.overrides = brush->overrides;
            run.minute = brush->minute;
            paintRun(painter, inner, run, true);
        } else {
            // The eraser: the cell goes empty, marked with a cross
            painter.fillRect(inner, base);
            painter.setPen(QPen(mix(base, text, 0.6), 1));
            painter.drawLine(inner.topLeft(), inner.bottomRight());
            painter.drawLine(inner.topRight(), inner.bottomLeft());
        }
        painter.restore();
    }
    QPen under(base, 2);
    paintOutline(painter, m_stroke, under);
    QPen over(selectionOutlineColor(), 2, Qt::CustomDashLine);
    over.setDashPattern({2, 2});
    over.setCapStyle(Qt::FlatCap);
    paintOutline(painter, m_stroke, over);
}

void BScheduleGridWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());
    const QColor base = palette().color(QPalette::Base);
    const QColor text = palette().color(QPalette::Text);
    painter.fillRect(viewport()->rect(), palette().color(QPalette::Window));

    if (m_model) {
        painter.save();
        painter.setClipRect(m_geometry.scrollAreaRect());

        const QRect cells = m_geometry.cellsRect();
        painter.fillRect(cells, base);

        double maxLoad = 1.0;
        for (int row = 0; row < BGridCellSet::Rows; ++row) {
            for (int column = 0; column < BGridCellSet::Columns; ++column) {
                const BGridCell cell = m_model->cell(row, column);
                if (cell.expectedConcurrentRuns)
                    maxLoad = qMax(maxLoad, *cell.expectedConcurrentRuns);
            }
        }
        for (int row = 0; row < BGridCellSet::Rows; ++row) {
            for (int column = 0; column < BGridCellSet::Columns; ++column)
                paintCell(painter, row, column, maxLoad);
        }

        // Grid lines on top of the fills, so that neighbours stay apart
        painter.setPen(gridLineColor());
        for (int row = 0; row < BGridCellSet::Rows; ++row) {
            const QRect r = m_geometry.cellRect(row, 0);
            painter.drawLine(cells.left(), r.bottom(), cells.right(), r.bottom());
        }
        for (int column = 0; column < BGridCellSet::Columns; ++column) {
            const QRect r = m_geometry.cellRect(0, column);
            // Every sixth hour a stronger line, to find the hour faster
            painter.setPen((column + 1) % 6 == 0 ? mix(base, text, 0.55) : gridLineColor());
            painter.drawLine(r.right(), cells.top(), r.right(), cells.bottom());
        }

        paintSelection(painter);
        paintStroke(painter);

        if (m_highlightedRun >= 0) {
            int scope = -1;
            const BGridCellSet cellsOfRun = m_model->cellsOfRun(m_highlightedRun, &scope);
            if (scope == m_model->activeScopeIndex()) {
                // Two dashed lines, one in each of the two extreme colours:
                // visible on every fill
                QPen under(base, 2);
                paintOutline(painter, cellsOfRun, under);
                QPen over(text, 2, Qt::CustomDashLine);
                over.setDashPattern({3, 3});
                over.setCapStyle(Qt::FlatCap);
                paintOutline(painter, cellsOfRun, over);
            }
        }

        if (hasFocus()) {
            const QRect r = m_geometry.cellRect(m_current.row, m_current.column).adjusted(2, 2, -3, -3);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(base, 1));
            painter.drawRect(r);
            painter.setPen(QPen(text, 1, Qt::DotLine));
            painter.drawRect(r);
        }

        paintRuleBand(painter);
        painter.restore();
    }

    paintHeaders(painter);
}

// --- Selection with mouse and keyboard ---------------------------------------------

template <typename Change>
void BScheduleGridWidget::changeSelection(Change change)
{
    if (!m_model)
        return;
    BGridSelection selection = m_model->selection();
    change(selection);
    m_model->setSelection(selection);
}

void BScheduleGridWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !m_model) {
        QAbstractScrollArea::mousePressEvent(event);
        return;
    }
    const BScheduleGridGeometry::Hit hit = m_geometry.hitTest(event->position().toPoint());
    const bool ctrl = event->modifiers() & Qt::ControlModifier;
    const bool shift = event->modifiers() & Qt::ShiftModifier;
    const bool hasAnchor = m_model->selection().anchor().has_value();
    m_drag = NoDrag;
    m_dragKeep = ctrl;
    m_dragMoved = false;
    m_pressPlain = !ctrl && !shift;

    switch (hit.part) {
    case BScheduleGridGeometry::Cell:
        if (m_editor && m_editor->paintMode() && m_pressPlain) {
            // Paint mode: the tool goes on every cell passed until the release
            m_drag = DragPaint;
            m_stroke = BGridCellSet();
            m_stroke.insert(hit.row, hit.column);
            m_pressCell = BGridPos{hit.row, hit.column};
            m_dragCell = m_pressCell;
            setCurrentCell(m_pressCell);
            viewport()->update();
            break;
        }
        changeSelection([&](BGridSelection &s) {
            if (shift)
                s.extendTo(hit.row, hit.column, ctrl);
            else if (ctrl)
                s.toggleCell(hit.row, hit.column);
            else
                s.selectCell(hit.row, hit.column);
        });
        m_drag = DragCells;
        m_pressCell = BGridPos{hit.row, hit.column};
        m_dragCell = m_pressCell;
        setCurrentCell(m_pressCell);
        break;
    case BScheduleGridGeometry::RowHeader:
        changeSelection([&](BGridSelection &s) {
            if (shift && hasAnchor) {
                // Whole days, also if the anchor was set by a cell: from
                // the day of the anchor to this one
                s.selectRow(s.anchor()->row, ctrl ? BGridSelection::Add : BGridSelection::Replace);
                s.extendTo(hit.row, BGridCellSet::Columns - 1, ctrl);
            } else {
                s.selectRow(hit.row, ctrl ? BGridSelection::Toggle : BGridSelection::Replace);
            }
        });
        m_drag = DragRows;
        m_dragCell = BGridPos{hit.row, 0};
        setCurrentCell(BGridPos{hit.row, m_current.column});
        break;
    case BScheduleGridGeometry::ColumnHeader:
        changeSelection([&](BGridSelection &s) {
            if (shift && hasAnchor) {
                // Whole hours, from the hour of the anchor to this one
                s.selectColumn(s.anchor()->column, ctrl ? BGridSelection::Add : BGridSelection::Replace);
                s.extendTo(BGridCellSet::Rows - 1, hit.column, ctrl);
            } else {
                s.selectColumn(hit.column, ctrl ? BGridSelection::Toggle : BGridSelection::Replace);
            }
        });
        m_drag = DragColumns;
        m_dragCell = BGridPos{0, hit.column};
        setCurrentCell(BGridPos{m_current.row, hit.column});
        break;
    case BScheduleGridGeometry::Corner:
        changeSelection([](BGridSelection &s) { s.selectAll(); });
        break;
    case BScheduleGridGeometry::RuleCell: {
        // The first rule of the hour; a click on a chosen one the next
        const QList<int> rules = rulesAtHour(hit.column);
        if (!rules.isEmpty()) {
            const int chosen = int(rules.indexOf(m_highlightedRule));
            emit calendarRuleActivated(rules[(chosen + 1) % rules.size()]);
        }
        break;
    }
    case BScheduleGridGeometry::RuleHeader:
    case BScheduleGridGeometry::Outside:
        break;
    }
    event->accept();
}

void BScheduleGridWidget::extendStroke(const BGridPos &from, const BGridPos &to)
{
    // Every cell on the way, also if the mouse was faster than its events
    const int steps = qMax(qAbs(to.row - from.row), qAbs(to.column - from.column));
    for (int i = 1; i <= steps; ++i) {
        m_stroke.insert(from.row + qRound(double(to.row - from.row) * i / steps),
                        from.column + qRound(double(to.column - from.column) * i / steps));
    }
}

void BScheduleGridWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drag == NoDrag || !(event->buttons() & Qt::LeftButton) || !m_model) {
        QAbstractScrollArea::mouseMoveEvent(event);
        return;
    }
    const BGridPos cell = m_geometry.nearestCell(event->position().toPoint());
    switch (m_drag) {
    case DragPaint:
        if (cell != m_dragCell) {
            extendStroke(m_dragCell, cell);
            m_dragCell = cell;
            m_dragMoved = true;
            setCurrentCell(cell);
            viewport()->update();
        }
        break;
    case DragCells:
        if (cell != m_dragCell) {
            m_dragCell = cell;
            m_dragMoved = true;
            changeSelection([&](BGridSelection &s) { s.extendTo(cell.row, cell.column, m_dragKeep); });
            setCurrentCell(cell);
        }
        break;
    case DragRows:
        if (cell.row != m_dragCell.row) {
            m_dragCell = cell;
            m_dragMoved = true;
            changeSelection([&](BGridSelection &s) {
                s.extendTo(cell.row, BGridCellSet::Columns - 1, m_dragKeep);
            });
            setCurrentCell(BGridPos{cell.row, m_current.column});
        }
        break;
    case DragColumns:
        if (cell.column != m_dragCell.column) {
            m_dragCell = cell;
            m_dragMoved = true;
            changeSelection([&](BGridSelection &s) {
                s.extendTo(BGridCellSet::Rows - 1, cell.column, m_dragKeep);
            });
            setCurrentCell(BGridPos{m_current.row, cell.column});
        }
        break;
    case NoDrag:
        break;
    }
    event->accept();
}

void BScheduleGridWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        QAbstractScrollArea::mouseReleaseEvent(event);
        return;
    }
    const Drag drag = m_drag;
    m_drag = NoDrag;
    if (drag == DragPaint) {
        // The whole stroke is one edit
        const BGridCellSet stroke = m_stroke;
        m_stroke = BGridCellSet();
        viewport()->update();
        if (m_editor && !stroke.isEmpty())
            m_editor->applyTool(stroke);
    }
    event->accept();
}

void BScheduleGridWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_model) {
        const BScheduleGridGeometry::Hit hit = m_geometry.hitTest(event->position().toPoint());
        if (hit.part == BScheduleGridGeometry::Cell) {
            // Not in paint mode: there the second press of a double click
            // is another dab of paint
            if (m_editor && m_editor->paintMode()) {
                mousePressEvent(event);
                return;
            }
            showCellDetails(hit.row, hit.column);
            event->accept();
            return;
        }
        if (hit.part == BScheduleGridGeometry::RuleCell) {
            const QList<int> rules = rulesAtHour(hit.column);
            if (!rules.isEmpty()) {
                emit calendarRuleEditRequested(rules.contains(m_highlightedRule) ? m_highlightedRule
                                                                                 : rules.first());
            }
            event->accept();
            return;
        }
    }
    QAbstractScrollArea::mouseDoubleClickEvent(event);
}

void BScheduleGridWidget::moveCurrent(const BGridPos &cell, Qt::KeyboardModifiers modifiers)
{
    BGridPos target;
    target.row = qBound(0, cell.row, BGridCellSet::Rows - 1);
    target.column = qBound(0, cell.column, BGridCellSet::Columns - 1);
    const bool shift = modifiers & Qt::ShiftModifier;
    changeSelection([&](BGridSelection &s) {
        if (shift) {
            // Without an anchor the rectangle starts where the keyboard was
            if (!s.anchor())
                s.selectCell(m_current.row, m_current.column);
            s.extendTo(target.row, target.column, false);
        } else {
            s.selectCell(target.row, target.column);
        }
    });
    setCurrentCell(target);
}

void BScheduleGridWidget::keyPressEvent(QKeyEvent *event)
{
    if (!m_model) {
        event->ignore();
        return;
    }
    const Qt::KeyboardModifiers modifiers = event->modifiers();
    const bool ctrl = modifiers & Qt::ControlModifier;
    const int lastRow = BGridCellSet::Rows - 1;
    const int lastColumn = BGridCellSet::Columns - 1;

    if (event->matches(QKeySequence::SelectAll)) {
        changeSelection([](BGridSelection &s) { s.selectAll(); });
        event->accept();
        return;
    }

    switch (event->key()) {
    case Qt::Key_Left:
        moveCurrent(BGridPos{m_current.row, m_current.column - 1}, modifiers);
        break;
    case Qt::Key_Right:
        moveCurrent(BGridPos{m_current.row, m_current.column + 1}, modifiers);
        break;
    case Qt::Key_Up:
        moveCurrent(BGridPos{m_current.row - 1, m_current.column}, modifiers);
        break;
    case Qt::Key_Down:
        moveCurrent(BGridPos{m_current.row + 1, m_current.column}, modifiers);
        break;
    case Qt::Key_Home:
        moveCurrent(BGridPos{ctrl ? 0 : m_current.row, 0}, modifiers);
        break;
    case Qt::Key_End:
        moveCurrent(BGridPos{ctrl ? lastRow : m_current.row, lastColumn}, modifiers);
        break;
    case Qt::Key_PageUp:
        moveCurrent(BGridPos{0, m_current.column}, modifiers);
        break;
    case Qt::Key_PageDown:
        moveCurrent(BGridPos{lastRow, m_current.column}, modifiers);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        showCellDetails(m_current.row, m_current.column);
        break;
    case Qt::Key_Escape:
        if (m_drag == DragPaint) {
            // The stroke is given up
            m_drag = NoDrag;
            m_stroke = BGridCellSet();
            viewport()->update();
            break;
        }
        if (m_highlightedRun < 0 && m_highlightedRule < 0) {
            event->ignore();
            return;
        }
        setHighlightedRun(-1);
        setHighlightedRule(-1);
        break;
    case Qt::Key_Space:
        if (!m_editor) {
            event->ignore();
            return;
        }
        if ((modifiers & Qt::ShiftModifier) && m_editor->activeBrush())
            m_editor->paintBeside(m_model->selection().cells(), *m_editor->activeBrush());
        else
            m_editor->applyToolToSelection();
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        if (!m_editor) {
            event->ignore();
            return;
        }
        m_editor->eraseSelection();
        break;
    default: {
        // The level keys and paint mode; not with Ctrl or Alt, which belong
        // to shortcuts
        const bool plain = !(modifiers & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier));
        const QString level = BScheduleGridEditor::levelOfKey(event->key());
        if (m_editor && plain && !level.isEmpty()) {
            m_editor->paintLevel(level);
            break;
        }
        if (m_editor && plain && event->key() == Qt::Key_P) {
            m_editor->setPaintMode(!m_editor->paintMode());
            break;
        }
        // Not QAbstractScrollArea: it would scroll on keys this grid does not use
        event->ignore();
        return;
    }
    }
    event->accept();
}

void BScheduleGridWidget::focusInEvent(QFocusEvent *event)
{
    QAbstractScrollArea::focusInEvent(event);
    viewport()->update();
    notifyAccessibleFocus();
}

void BScheduleGridWidget::focusOutEvent(QFocusEvent *event)
{
    QAbstractScrollArea::focusOutEvent(event);
    viewport()->update();
}

// --- Tooltips ------------------------------------------------------------------------

QString BScheduleGridWidget::cellToolTip(int row, int column) const
{
    if (!m_model || row < 0 || row >= BGridCellSet::Rows || column < 0 || column >= BGridCellSet::Columns)
        return QString();
    const BGridCell cell = m_model->cell(row, column);
    const QList<BGridScopeInfo> &scopes = m_model->scopes();

    QString text = QStringLiteral("<b>%1</b>").arg(
        tr("%1, %2 to %3").arg(BScheduleGridText::dayName(m_model->dayOfWeekOfRow(row)),
                               BScheduleGridText::timeText(column, 0),
                               BScheduleGridText::timeText(column, 59)).toHtmlEscaped());

    auto line = [&](const BGridRun &run) {
        QString entry = BScheduleGridText::runText(run, column).toHtmlEscaped();
        const QString tag = brushTag(run.brush());
        if (!tag.isEmpty())
            entry = QStringLiteral("[%1] ").arg(tag) + entry;
        if (run.withoutTime)
            entry += QStringLiteral(" (") + tr("the Run line names no time").toHtmlEscaped()
                     + QLatin1Char(')');
        entry += QStringLiteral("<br>&nbsp;&nbsp;&nbsp;")
                 + tr("Run line %1: %2").arg(run.runIndex + 1)
                       .arg(runLineText(*m_model, run.runIndex)).toHtmlEscaped();
        return entry;
    };

    if (cell.inScope.isEmpty()) {
        text += QStringLiteral("<br>")
                + tr("No run in the scope \"%1\"")
                      .arg(BScheduleGridText::scopeLabel(m_model->activeScope())).toHtmlEscaped();
    }
    for (const BGridRun &run : cell.inScope)
        text += QStringLiteral("<br>") + line(run);

    if (!cell.outOfScope.isEmpty()) {
        text += QStringLiteral("<br><i>%1</i>").arg(tr("Other scopes:").toHtmlEscaped());
        for (const BGridRun &run : cell.outOfScope) {
            const QString scope = BScheduleGridText::scopeLabel(scopes.value(run.scopeIndex).scope);
            const QString relation = run.sharesDatesWithActiveScope
                ? tr("%1: may run on the same day as this scope").arg(scope)
                : tr("%1: never on the same day as this scope").arg(scope);
            text += QStringLiteral("<br>") + line(run)
                    + QStringLiteral("<br>&nbsp;&nbsp;&nbsp;") + relation.toHtmlEscaped();
        }
        if (m_editor) {
            text += QStringLiteral("<br>")
                    + tr("Painting or erasing here changes only the scope \"%1\"; the runs of "
                         "other scopes stay.")
                          .arg(BScheduleGridText::scopeLabel(m_model->activeScope())).toHtmlEscaped();
        }
    }
    if (cell.expectedConcurrentRuns) {
        text += QStringLiteral("<br>")
                + tr("Expected load: %1 runs at the same time")
                      .arg(QString::number(*cell.expectedConcurrentRuns, 'f', 1)).toHtmlEscaped();
    }
    return text;
}

QString BScheduleGridWidget::toolTipAt(const QPoint &viewportPos) const
{
    if (!m_model)
        return QString();
    const BScheduleGridGeometry::Hit hit = m_geometry.hitTest(viewportPos);
    switch (hit.part) {
    case BScheduleGridGeometry::Cell:
        return cellToolTip(hit.row, hit.column);
    case BScheduleGridGeometry::RowHeader:
        return tr("%1: click to select the whole day. Ctrl adds or removes it, "
                  "Shift extends the selection from its anchor.")
            .arg(BScheduleGridText::dayName(m_model->dayOfWeekOfRow(hit.row))).toHtmlEscaped();
    case BScheduleGridGeometry::ColumnHeader:
        return tr("%1 to %2: click to select this hour on every day. Ctrl adds or removes it, "
                  "Shift extends the selection from its anchor.")
            .arg(BScheduleGridText::timeText(hit.column, 0),
                 BScheduleGridText::timeText(hit.column, 59)).toHtmlEscaped();
    case BScheduleGridGeometry::Corner:
        return tr("Click to select all days and hours (Ctrl+A)").toHtmlEscaped();
    case BScheduleGridGeometry::RuleHeader:
        return tr("Run lines by day of the month or week of the year do not belong to a weekday. "
                  "They are marked here at their hours and listed under \"Calendar rules\"; "
                  "this band cannot be selected or painted. A double click on a mark opens "
                  "the form of its rule.").toHtmlEscaped();
    case BScheduleGridGeometry::RuleCell: {
        const QList<int> here = rulesAtHour(hit.column);
        if (here.isEmpty())
            return QString();
        const QList<BGridCalendarRule> &rules = m_model->calendarRules();
        QStringList lines;
        for (int index : here) {
            lines.append(QStringLiteral("<b>%1</b> %2<br>&nbsp;&nbsp;&nbsp;%3")
                             .arg(BScheduleGridText::ruleTag(index),
                                  rules[index].text.toHtmlEscaped(),
                                  BScheduleGridText::ruleReading(rules[index]).toHtmlEscaped()));
        }
        return lines.join(QStringLiteral("<br>"));
    }
    case BScheduleGridGeometry::Outside:
        break;
    }
    return QString();
}

bool BScheduleGridWidget::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        auto *help = static_cast<QHelpEvent *>(event);
        const QString text = toolTipAt(help->pos());
        if (text.isEmpty()) {
            QToolTip::hideText();
            event->ignore();
        } else {
            // The rectangle keeps the tip while the mouse stays on this part
            const BScheduleGridGeometry::Hit hit = m_geometry.hitTest(help->pos());
            QRect rect;
            if (hit.part == BScheduleGridGeometry::Cell)
                rect = m_geometry.cellRect(hit.row, hit.column);
            else if (hit.part == BScheduleGridGeometry::RowHeader)
                rect = m_geometry.rowHeaderRect(hit.row);
            else if (hit.part == BScheduleGridGeometry::ColumnHeader)
                rect = m_geometry.columnHeaderRect(hit.column);
            else if (hit.part == BScheduleGridGeometry::RuleCell)
                rect = m_geometry.ruleCellRect(hit.column);
            QToolTip::showText(help->globalPos(), text, viewport(), rect);
        }
        return true;
    }
    return QAbstractScrollArea::viewportEvent(event);
}

// --- The list of runs of a cell ----------------------------------------------------

QWidget *BScheduleGridWidget::cellDetails() const
{
    return m_details && m_details->isVisible() ? m_details.data() : nullptr;
}

bool BScheduleGridWidget::showCellDetails(int row, int column)
{
    if (!m_model || row < 0 || row >= BGridCellSet::Rows || column < 0 || column >= BGridCellSet::Columns)
        return false;
    const BGridCell cell = m_model->cell(row, column);
    if (cell.isEmpty())
        return false;
    if (m_details)
        delete m_details.data();

    auto *frame = new QFrame(this, Qt::Popup);
    frame->setObjectName(QStringLiteral("scheduleGridCellDetails"));
    frame->setAttribute(Qt::WA_DeleteOnClose);
    frame->setFrameShape(QFrame::StyledPanel);
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    auto *title = new QLabel(frame);
    title->setTextFormat(Qt::PlainText);
    QFont bold = title->font();
    bold.setBold(true);
    title->setFont(bold);
    title->setText(tr("%1, %2 to %3: %n run(s)", "", int(cell.inScope.size() + cell.outOfScope.size()))
                       .arg(BScheduleGridText::dayName(m_model->dayOfWeekOfRow(row)),
                            BScheduleGridText::timeText(column, 0),
                            BScheduleGridText::timeText(column, 59)));
    layout->addWidget(title);

    auto *tree = new QTreeWidget(frame);
    tree->setObjectName(QStringLiteral("scheduleGridCellRuns"));
    tree->setAccessibleName(tr("Runs of the cell"));
    tree->setRootIsDecorated(false);
    tree->setUniformRowHeights(true);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    tree->setHeaderLabels({tr("Time"), tr("Level"), tr("Overrides"), tr("Scope"), tr("Run line")});

    const QList<BGridScopeInfo> &scopes = m_model->scopes();
    auto add = [&](const BGridRun &run, bool inScope) {
        auto *item = new QTreeWidgetItem(tree);
        item->setText(0, BScheduleGridText::timeText(column, run.minute));
        item->setText(1, BScheduleGridText::levelName(run.level()));
        QString overrides = BScheduleGridText::overridesText(run.overrides);
        const QString tag = brushTag(run.brush());
        if (!tag.isEmpty())
            overrides = QStringLiteral("[%1] ").arg(tag) + overrides;
        if (run.withoutTime) {
            overrides += (overrides.isEmpty() ? QString() : QStringLiteral("; "))
                         + tr("no time in the Run line");
        }
        item->setText(2, overrides);
        QString scope = BScheduleGridText::scopeLabel(scopes.value(run.scopeIndex).scope);
        if (!inScope) {
            scope = run.sharesDatesWithActiveScope
                ? tr("%1 (other scope, may run on the same day)").arg(scope)
                : tr("%1 (other scope, never on the same day)").arg(scope);
            QFont italic = item->font(0);
            italic.setItalic(true);
            for (int i = 0; i < 5; ++i)
                item->setFont(i, italic);
        }
        item->setText(3, scope);
        item->setText(4, tr("%1: %2").arg(run.runIndex + 1).arg(runLineText(*m_model, run.runIndex)));
        item->setData(0, Qt::UserRole, run.runIndex);
    };
    for (const BGridRun &run : cell.inScope)
        add(run, true);
    for (const BGridRun &run : cell.outOfScope)
        add(run, false);
    for (int i = 0; i < tree->columnCount(); ++i)
        tree->resizeColumnToContents(i);
    tree->setCurrentItem(tree->topLevelItem(0));
    layout->addWidget(tree);

    auto *hint = new QLabel(tr("Enter or a double click shows the Run line in the list. Esc closes."), frame);
    hint->setTextFormat(Qt::PlainText);
    layout->addWidget(hint);

    connect(tree, &QTreeWidget::itemActivated, this, [this](QTreeWidgetItem *item) {
        emit runActivated(item->data(0, Qt::UserRole).toInt());
    });

    // Wide enough for the columns, as high as the rows, within reason
    int width = 2 * tree->frameWidth() + 24;
    for (int i = 0; i < tree->columnCount(); ++i)
        width += tree->columnWidth(i);
    const int rows = tree->topLevelItemCount();
    const int rowHeight = qMax(18, tree->sizeHintForRow(0));
    tree->setMinimumWidth(qMin(width, 900));
    tree->setFixedHeight(tree->header()->sizeHint().height() + qMin(rows, 12) * rowHeight
                         + 2 * tree->frameWidth() + 4);

    m_details = frame;
    frame->adjustSize();
    const QRect rect = m_geometry.cellRect(row, column);
    frame->move(viewport()->mapToGlobal(rect.bottomLeft() + QPoint(0, 2)));
    frame->show();
    tree->setFocus();
    return true;
}

// --- Accessibility -------------------------------------------------------------------
//
// The events are sent whether or not assistive technology is listening:
// they are few and cheap, the platform drops them if nobody listens, and
// QAccessible::isActive() cannot be switched on by a test.

void BScheduleGridWidget::notifyAccessibleFocus()
{
#if QT_CONFIG(accessibility)
    if (!hasFocus())
        return;
    QAccessibleEvent event(this, QAccessible::Focus);
    event.setChild(BScheduleGridAccessible::childIndex(m_current.row, m_current.column));
    QAccessible::updateAccessibility(&event);
#endif
}

void BScheduleGridWidget::notifyAccessibleSelection()
{
#if QT_CONFIG(accessibility)
    QAccessibleEvent event(this, QAccessible::SelectionWithin);
    QAccessible::updateAccessibility(&event);
#endif
}

void BScheduleGridWidget::notifyAccessibleReset()
{
#if QT_CONFIG(accessibility)
    QAccessibleTableModelChangeEvent event(this, QAccessibleTableModelChangeEvent::ModelReset);
    QAccessible::updateAccessibility(&event);
#endif
}

void BScheduleGridWidget::notifyAccessibleCells(const QList<BGridPos> &cells)
{
#if QT_CONFIG(accessibility)
    if (cells.isEmpty())
        return;
    // The part of the table that changed, then every cell that says
    // something else now
    int top = BGridCellSet::Rows;
    int bottom = -1;
    int left = BGridCellSet::Columns;
    int right = -1;
    for (const BGridPos &pos : cells) {
        top = qMin(top, pos.row);
        bottom = qMax(bottom, pos.row);
        left = qMin(left, pos.column);
        right = qMax(right, pos.column);
    }
    QAccessibleTableModelChangeEvent changed(this, QAccessibleTableModelChangeEvent::DataChanged);
    changed.setFirstRow(top);
    changed.setLastRow(bottom);
    changed.setFirstColumn(left);
    changed.setLastColumn(right);
    QAccessible::updateAccessibility(&changed);
    for (const BGridPos &pos : cells) {
        QAccessibleEvent name(this, QAccessible::NameChanged);
        name.setChild(BScheduleGridAccessible::childIndex(pos.row, pos.column));
        QAccessible::updateAccessibility(&name);
    }
#else
    Q_UNUSED(cells);
#endif
}

void BScheduleGridWidget::notifyAccessibleTool()
{
#if QT_CONFIG(accessibility)
    // The tool in hand is part of the description of the table
    QAccessibleEvent event(this, QAccessible::DescriptionChanged);
    QAccessible::updateAccessibility(&event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    if (m_editor) {
        QAccessibleAnnouncementEvent announcement(this, tr("Tool: %1").arg(m_editor->toolText()));
        QAccessible::updateAccessibility(&announcement);
    }
#endif
#endif
}
