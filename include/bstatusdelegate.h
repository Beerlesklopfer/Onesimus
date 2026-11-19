#ifndef BSTATUSDELEGATE_H
#define BSTATUSDELEGATE_H

#include <QColor>
#include <QStyledItemDelegate>
#include <QWidget>

#include "bstatuschip.h"

/**
 * @brief Carrier of the themes' status colours
 *
 * A hidden widget that the themes style like any other:
 *
 *     BStatusPalette { qproperty-successBackground: #1d3b24; ... }
 *
 * When the theme's style sheet polishes it, every property is handed to
 * BStatus::setColor(), where delegates, models and dialogs read it. One
 * instance exists per application; refresh() after a new theme was applied.
 * Without a QApplication nothing is created and the built-in values apply.
 */
class BStatusPalette : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor neutralBackground READ neutralBackground WRITE setNeutralBackground)
    Q_PROPERTY(QColor neutralForeground READ neutralForeground WRITE setNeutralForeground)
    Q_PROPERTY(QColor neutralAccent READ neutralAccent WRITE setNeutralAccent)
    Q_PROPERTY(QColor successBackground READ successBackground WRITE setSuccessBackground)
    Q_PROPERTY(QColor successForeground READ successForeground WRITE setSuccessForeground)
    Q_PROPERTY(QColor successAccent READ successAccent WRITE setSuccessAccent)
    Q_PROPERTY(QColor warningBackground READ warningBackground WRITE setWarningBackground)
    Q_PROPERTY(QColor warningForeground READ warningForeground WRITE setWarningForeground)
    Q_PROPERTY(QColor warningAccent READ warningAccent WRITE setWarningAccent)
    Q_PROPERTY(QColor errorBackground READ errorBackground WRITE setErrorBackground)
    Q_PROPERTY(QColor errorForeground READ errorForeground WRITE setErrorForeground)
    Q_PROPERTY(QColor errorAccent READ errorAccent WRITE setErrorAccent)
    Q_PROPERTY(QColor runningBackground READ runningBackground WRITE setRunningBackground)
    Q_PROPERTY(QColor runningForeground READ runningForeground WRITE setRunningForeground)
    Q_PROPERTY(QColor runningAccent READ runningAccent WRITE setRunningAccent)
    Q_PROPERTY(QColor waitingBackground READ waitingBackground WRITE setWaitingBackground)
    Q_PROPERTY(QColor waitingForeground READ waitingForeground WRITE setWaitingForeground)
    Q_PROPERTY(QColor waitingAccent READ waitingAccent WRITE setWaitingAccent)
    Q_PROPERTY(QColor infoBackground READ infoBackground WRITE setInfoBackground)
    Q_PROPERTY(QColor infoForeground READ infoForeground WRITE setInfoForeground)
    Q_PROPERTY(QColor infoAccent READ infoAccent WRITE setInfoAccent)

public:
    /// The application's instance, created and polished on first use;
    /// nullptr without a QApplication
    static BStatusPalette *instance();

    /// Reads the tokens again from the current style sheet (after a theme change)
    static void refresh();

#define BSTATUS_PALETTE_ACCESSORS(name, Name, tone)                                              \
    QColor name##Background() const { return BStatus::color(tone, BStatus::Part::Background); } \
    QColor name##Foreground() const { return BStatus::color(tone, BStatus::Part::Foreground); } \
    QColor name##Accent() const { return BStatus::color(tone, BStatus::Part::Accent); }         \
    void set##Name##Background(const QColor &c) { BStatus::setColor(tone, BStatus::Part::Background, c); } \
    void set##Name##Foreground(const QColor &c) { BStatus::setColor(tone, BStatus::Part::Foreground, c); } \
    void set##Name##Accent(const QColor &c) { BStatus::setColor(tone, BStatus::Part::Accent, c); }

    BSTATUS_PALETTE_ACCESSORS(neutral, Neutral, BStatus::Tone::Neutral)
    BSTATUS_PALETTE_ACCESSORS(success, Success, BStatus::Tone::Success)
    BSTATUS_PALETTE_ACCESSORS(warning, Warning, BStatus::Tone::Warning)
    BSTATUS_PALETTE_ACCESSORS(error, Error, BStatus::Tone::Error)
    BSTATUS_PALETTE_ACCESSORS(running, Running, BStatus::Tone::Running)
    BSTATUS_PALETTE_ACCESSORS(waiting, Waiting, BStatus::Tone::Waiting)
    BSTATUS_PALETTE_ACCESSORS(info, Info, BStatus::Tone::Info)
#undef BSTATUS_PALETTE_ACCESSORS

private:
    BStatusPalette();
};

/**
 * @brief Paints a status cell as a rounded chip: code, short word, semantic colour
 *
 * Reads from the model:
 *  - Qt::DisplayRole: the short word
 *  - BStatus::CodeRole: the letter or sign drawn in bold before the word
 *  - BStatus::ToneRole: the tone (colour from the theme's tokens), or
 *  - BStatus::ChipColorRole: an explicit fill (the text gets black or white)
 * A cell with neither tone nor fill is painted as a normal cell.
 *
 * The cell's background, the selection and the focus frame are drawn by the
 * style as usual; the chip sits on top and gets a frame in the selection's
 * text colour when its row is selected. What a screen reader announces is
 * the model's Qt::AccessibleTextRole, which the models set to the full word.
 * Sizes follow the font (BUiScale).
 */
class BStatusDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit BStatusDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    struct Colors {
        QColor background;
        QColor foreground;
        QColor border;
    };

    /// Colours of the chip of @p index; invalid background if the cell has no chip
    static Colors chipColors(const QModelIndex &index);
};

#endif // BSTATUSDELEGATE_H
