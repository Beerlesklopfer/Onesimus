#include "bstatusdelegate.h"

#include "buiscale.h"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <QStyle>

// ============================================================================
// BStatusPalette
// ============================================================================

BStatusPalette::BStatusPalette()
    : QWidget(nullptr)
{
    setObjectName(QStringLiteral("BStatusPalette"));
    setAttribute(Qt::WA_DontShowOnScreen);
    setAttribute(Qt::WA_QuitOnClose, false);
}

BStatusPalette *BStatusPalette::instance()
{
    static QPointer<BStatusPalette> palette;
    if (!palette) {
        if (!qobject_cast<QApplication *>(QCoreApplication::instance())) {
            return nullptr;
        }
        palette = new BStatusPalette();
        // Gone with the application, not before
        QObject::connect(qApp, &QCoreApplication::aboutToQuit, palette.data(), &QObject::deleteLater);
        palette->ensurePolished();
    }
    return palette;
}

void BStatusPalette::refresh()
{
    BStatusPalette *palette = instance();
    if (!palette) {
        return;
    }
    // A theme without tokens falls back to the built-in values
    BStatus::resetColors();
    palette->style()->unpolish(palette);
    palette->style()->polish(palette);
}

// ============================================================================
// BStatusDelegate
// ============================================================================

namespace {

QColor contrastText(const QColor &background)
{
    const double luminance = 0.299 * background.red() + 0.587 * background.green()
                             + 0.114 * background.blue();
    return luminance > 140.0 ? QColor(Qt::black) : QColor(Qt::white);
}

struct ChipLayout {
    QRect chip;
    QRect code;
    QRect text;
    QString elidedText;
};

ChipLayout layoutChip(const QStyleOptionViewItem &option, const QString &code, const QString &text)
{
    QFont bold = option.font;
    bold.setBold(true);
    const QFontMetrics fm(option.font);
    const QFontMetrics boldFm(bold);

    const int margin = BUiScale::px(4, option.font);
    const int padding = BUiScale::px(6, option.font);
    const int gap = code.isEmpty() || text.isEmpty() ? 0 : BUiScale::px(5, option.font);
    const int height = qMin(option.rect.height() - 2, fm.height() + BUiScale::px(4, option.font));

    const int codeWidth = code.isEmpty() ? 0 : boldFm.horizontalAdvance(code);
    const int available = option.rect.width() - 2 * margin - 2 * padding - codeWidth - gap;
    ChipLayout layout;
    layout.elidedText = fm.elidedText(text, Qt::ElideRight, qMax(0, available));
    const int textWidth = layout.elidedText.isEmpty() ? 0 : fm.horizontalAdvance(layout.elidedText);
    // The chip fills the cell (maintainer's wish): the column width is the
    // chip width, so all chips of a column line up as one band
    const int width = option.rect.width() - 2 * margin;

    layout.chip = QRect(option.rect.left() + margin,
                        option.rect.top() + (option.rect.height() - height) / 2,
                        qMax(0, width), qMax(0, height));
    layout.code = QRect(layout.chip.left() + padding, layout.chip.top(), codeWidth, layout.chip.height());
    // Justified: the code sits at the left edge of the chip, the word at the
    // right edge, so the chips of a column read like a table (maintainer's wish)
    layout.text = QRect(layout.chip.right() - padding - textWidth + 1, layout.chip.top(),
                        textWidth, layout.chip.height());
    if (layout.text.left() <= layout.code.right() + gap) {
        layout.text.moveLeft(layout.code.right() + 1 + gap);
    }
    return layout;
}

} // namespace

BStatusDelegate::BStatusDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

BStatusDelegate::Colors BStatusDelegate::chipColors(const QModelIndex &index)
{
    Colors colors;
    const QVariant fill = index.data(BStatus::ChipColorRole);
    if (fill.canConvert<QColor>() && fill.value<QColor>().isValid()) {
        colors.background = fill.value<QColor>();
        colors.foreground = contrastText(colors.background);
        colors.border = colors.background.darker(135);
        return colors;
    }
    const QVariant tone = index.data(BStatus::ToneRole);
    if (!tone.isValid()) {
        return colors;
    }
    // Make sure the theme's tokens were read
    BStatusPalette::instance();
    const auto t = static_cast<BStatus::Tone>(tone.toInt());
    colors.background = BStatus::color(t, BStatus::Part::Background);
    colors.foreground = BStatus::color(t, BStatus::Part::Foreground);
    colors.border = BStatus::color(t, BStatus::Part::Accent);
    return colors;
}

void BStatusDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    const Colors colors = chipColors(index);
    if (!colors.background.isValid()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    const QString text = opt.text;
    const QString code = index.data(BStatus::CodeRole).toString();

    // Background, selection and focus as for any other cell
    opt.text.clear();
    opt.icon = QIcon();
    opt.features &= ~QStyleOptionViewItem::HasDecoration;
    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

    const ChipLayout layout = layoutChip(opt, code, text);
    if (layout.chip.width() <= 0 || layout.chip.height() <= 0) {
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    const bool selected = opt.state & QStyle::State_Selected;
    const qreal radius = layout.chip.height() / 2.0;
    // On a selected row the frame takes the selection's text colour, so the
    // chip stands apart from the highlight and the selection stays visible
    QPen pen(selected ? opt.palette.color(QPalette::HighlightedText) : colors.border);
    pen.setWidthF(selected ? qMax(1.5, BUiScale::px(2, opt.font) * 0.75) : 1.0);
    painter->setPen(pen);
    painter->setBrush(colors.background);
    painter->drawRoundedRect(QRectF(layout.chip).adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);

    painter->setPen(colors.foreground);
    if (!code.isEmpty()) {
        QFont bold = opt.font;
        bold.setBold(true);
        painter->setFont(bold);
        painter->drawText(layout.code, Qt::AlignCenter, code);
    }
    if (!layout.elidedText.isEmpty()) {
        painter->setFont(opt.font);
        painter->drawText(layout.text, Qt::AlignLeft | Qt::AlignVCenter, layout.elidedText);
    }
    painter->restore();
}

QSize BStatusDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize base = QStyledItemDelegate::sizeHint(option, index);
    if (!chipColors(index).background.isValid()) {
        return base;
    }
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QFont bold = opt.font;
    bold.setBold(true);
    const QString code = index.data(BStatus::CodeRole).toString();
    const QFontMetrics fm(opt.font);
    const int codeWidth = code.isEmpty() ? 0 : QFontMetrics(bold).horizontalAdvance(code);
    const int textWidth = opt.text.isEmpty() ? 0 : fm.horizontalAdvance(opt.text);
    const int gap = code.isEmpty() || opt.text.isEmpty() ? 0 : BUiScale::px(5, opt.font);
    const int width = 2 * BUiScale::px(4, opt.font) + 2 * BUiScale::px(6, opt.font)
                      + codeWidth + gap + textWidth;
    const int height = fm.height() + BUiScale::px(8, opt.font);
    return QSize(width, qMax(base.height(), height));
}
