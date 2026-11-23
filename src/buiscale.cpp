// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "buiscale.h"

#include <QApplication>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QStyledItemDelegate>
#include <QDialog>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QWizard>
#include <QEvent>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPointer>
#include <QRegularExpression>
#include <QScreen>
#include <QStyle>
#include <QTableView>
#include <QVariant>
#include <QWidget>
#include <QWizard>
#include <QWizardPage>

#include <algorithm>
#include <utility>

namespace {

// Dynamic properties of a bound widget
const char kRoleProperty[] = "bUiFontRole";
const char kStylesProperty[] = "bUiFontStyles";
// The row height this class gave a table
const char kRowHeightProperty[] = "bUiRowHeight";

// 10 pt at 96 dpi in pixels: the font the pixel measures were designed for
const qreal kDesignPixelSize = 10.0 * 96.0 / 72.0;

qreal logicalDpi()
{
    if (qGuiApp) {
        if (const QScreen *screen = QGuiApplication::primaryScreen()) {
            const qreal dpi = screen->logicalDotsPerInchY();
            if (dpi > 0) return dpi;
        }
    }
    return 96.0;
}

void multiplySize(QFont &font, qreal factor)
{
    if (font.pointSizeF() > 0) {
        font.setPointSizeF(qMax<qreal>(1.0, font.pointSizeF() * factor));
    } else if (font.pixelSize() > 0) {
        font.setPixelSize(qMax(1, qRound(font.pixelSize() * factor)));
    }
}

} // namespace

BUiScale::BUiScale()
    : QObject(nullptr)
{
}

BUiScale &BUiScale::instance()
{
    static BUiScale scale;
    return scale;
}

qreal BUiScale::roleFactor(Role role)
{
    switch (role) {
    case Tiny:     return 0.8;
    case Small:    return 0.9;
    case Normal:   return 1.0;
    case Large:    return 1.2;
    case Title:    return 1.4;
    case Headline: return 1.8;
    }
    return 1.0;
}

QFont BUiScale::scaled(const QFont &base, Role role, Styles styles)
{
    QFont font = base;
    if (styles.testFlag(Monospace)) {
        // The desktop's fixed-pitch family; the size stays that of the base
        const QFont fixed = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setFamilies(fixed.families());
        font.setStyleHint(QFont::Monospace);
        font.setFixedPitch(true);
    }
    multiplySize(font, roleFactor(role));
    if (styles.testFlag(Bold)) font.setBold(true);
    if (styles.testFlag(Italic)) font.setItalic(true);
    return font;
}

qreal BUiScale::scaleOf(const QFont &font)
{
    qreal pixels = 0;
    if (font.pointSizeF() > 0) {
        pixels = font.pointSizeF() * logicalDpi() / 72.0;
    } else if (font.pixelSize() > 0) {
        pixels = font.pixelSize();
    }
    return pixels > 0 ? pixels / kDesignPixelSize : 1.0;
}

int BUiScale::px(int designPixels, const QFont &font)
{
    if (designPixels <= 0) return designPixels;
    return qMax(1, qRound(designPixels * scaleOf(font)));
}

int BUiScale::px(int designPixels)
{
    return px(designPixels, instance().applicationFont());
}

int BUiScale::designPx(int pixels)
{
    if (pixels <= 0) return pixels;
    return qMax(1, qRound(pixels / scaleOf(instance().applicationFont())));
}

QString BUiScale::cssFontSize(Role role)
{
    const QFont font = instance().font(role);
    if (font.pointSizeF() > 0) {
        return QStringLiteral("font-size:%1pt").arg(font.pointSizeF(), 0, 'f', 1);
    }
    return QStringLiteral("font-size:%1px").arg(qMax(1, font.pixelSize()));
}

QString BUiScale::resolveCss(const QString &styleSheet)
{
    static const struct { const char *token; Role role; } tokens[] = {
        {"@tiny", Tiny}, {"@small", Small}, {"@normal", Normal},
        {"@large", Large}, {"@title", Title}, {"@headline", Headline},
    };
    QString resolved = styleSheet;
    for (const auto &entry : tokens) {
        const QString token = QLatin1String(entry.token);
        if (!resolved.contains(token)) continue;
        // "font-size:9.9pt" without the property name
        resolved.replace(token, cssFontSize(entry.role).section(QLatin1Char(':'), 1));
    }
    return resolved;
}

QString BUiScale::themeCss(const QString &styleSheet)
{
    // "font-size: 10pt;", "font-family: ...;" and the shorthand "font: ...;"
    // with an absolute value. Not "font-weight" or "font-style", and not a
    // role token ("font-size: @small"), which resolveCss() fills in.
    static const QRegularExpression absoluteFont(
        QStringLiteral("(?<![-\\w])font(?:-size|-family)?\\s*:(?!\\s*@)[^;}]*;?"),
        QRegularExpression::CaseInsensitiveOption);
    QString filtered = styleSheet;
    filtered.remove(absoluteFont);
    return resolveCss(filtered);
}

void BUiScale::setApplicationStyleSheet(const QString &styleSheet)
{
    auto *app = qobject_cast<QApplication *>(QCoreApplication::instance());
    if (!app) return;
    if (!m_filterInstalled) {
        app->installEventFilter(this);
        m_filterInstalled = true;
    }
    const bool wasApplying = std::exchange(m_applying, true);
    app->setStyleSheet(themeCss(styleSheet));
    // QApplication::setStyleSheet() sets a new style, and with it Qt takes
    // the per-class fonts of the platform theme again
    dropClassFonts();
    m_applying = wasApplying;
}

bool BUiScale::hasClassFonts() const
{
    // The classes Qt asks the platform theme for (QApplicationPrivate::
    // initializeWidgetFontHash()); without an entry the application font
    // is returned
    static const char *const classes[] = {
        "QMenu", "QMenuBar", "QMenuItem", "QMessageBox", "QLabel", "QTipLabel",
        "QStatusBar", "QMdiSubWindowTitleBar", "QDockWidgetTitle", "QPushButton",
        "QCheckBox", "QRadioButton", "QToolButton", "QAbstractItemView",
        "QListView", "QHeaderView", "QListBox", "QComboMenuItem",
        "QComboLineEdit", "QSmallFont", "QMiniFont",
    };
    const QFont app = QApplication::font();
    for (const char *name : classes) {
        if (QApplication::font(name) != app) return true;
    }
    return false;
}

void BUiScale::dropClassFonts()
{
    if (!qobject_cast<QApplication *>(QCoreApplication::instance()) || !hasClassFonts())
        return;
    const bool wasApplying = std::exchange(m_applying, true);
    // Setting the general font empties the per-class fonts
    QApplication::setFont(applicationFont());
    QCoreApplication::sendPostedEvents(nullptr, QEvent::ApplicationFontChange);
    rebindWidgets();
    propagateToWidgets();
    m_applying = wasApplying;
}

QString BUiScale::themeRules()
{
    // The values of the themes: 18 px indicators, 10 px above a group box
    // and 10 px inside it for its title
    if (px(18) == 18 && px(10) == 10) return QString();
    return QStringLiteral("QCheckBox::indicator, QRadioButton::indicator "
                          "{ width: %1px; height: %1px; }\n"
                          "QGroupBox { margin-top: %2px; padding-top: %2px; }\n")
        .arg(px(18)).arg(px(10));
}

QFont BUiScale::baseFont() const
{
    if (!m_baseKnown) {
        // The font the platform theme gave the application. Asked once,
        // before this class sets a font of its own.
        if (qobject_cast<QApplication *>(QCoreApplication::instance())) {
            m_baseFont = QApplication::font();
            m_desktopFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
            m_baseKnown = true;
        } else if (qGuiApp) {
            m_baseFont = QGuiApplication::font();
            m_baseKnown = true;
        } else {
            return QFont();
        }
    }
    return m_baseFont;
}

void BUiScale::setBaseFont(const QFont &font)
{
    m_baseFont = font;
    m_baseKnown = true;
    m_baseFixed = true;
    apply();
}

QFont BUiScale::applicationFont() const
{
    QFont font = baseFont();
    multiplySize(font, m_zoomPercent / 100.0);
    return font;
}

QFont BUiScale::font(Role role, Styles styles) const
{
    return scaled(applicationFont(), role, styles);
}

void BUiScale::setZoomPercent(int percent)
{
    const int clamped = clampZoom(percent);
    const bool differs = clamped != m_zoomPercent;
    m_zoomPercent = clamped;
    // Also when the value is the same: the first call at start-up makes the
    // application font an explicit one
    apply();
    if (differs) emit zoomChanged(m_zoomPercent);
}

void BUiScale::apply()
{
    if (qobject_cast<QApplication *>(QCoreApplication::instance())) {
        if (!m_filterInstalled) {
            qApp->installEventFilter(this);
            m_filterInstalled = true;
        }
        m_applying = true;
        QApplication::setFont(applicationFont());
        // Qt tells the open windows with posted events. Deliver them now:
        // whoever listens to changed() measures with the new font.
        QCoreApplication::sendPostedEvents(nullptr, QEvent::ApplicationFontChange);
        rebindWidgets();
        propagateToWidgets();

        // Column widths were given in pixels for the font so far
        const qreal scale = scaleOf(applicationFont());
        if (m_appliedScale > 0 && !qFuzzyCompare(scale, m_appliedScale)) {
            scaleColumnWidths(scale / m_appliedScale);
            scaleDialogs(scale / m_appliedScale);
        }
        m_appliedScale = scale;
        m_applying = false;
    }
    emit changed();
}

void BUiScale::rebindWidgets()
{
    const QWidgetList widgets = QApplication::allWidgets();
    for (QWidget *widget : widgets) {
        const QVariant role = widget->property(kRoleProperty);
        if (!role.isValid()) continue;
        widget->setFont(font(static_cast<Role>(role.toInt()),
                             Styles(widget->property(kStylesProperty).toInt())));
    }
}

void BUiScale::propagateToWidgets()
{
    // With a style sheet on the application Qt does not hand a new font
    // down from a widget to its children: a window gets the new font and
    // everything inside keeps the old one. Every widget is therefore told
    // itself, parents before children, so that a child also takes what it
    // inherits from a parent with a font of its own.
    const QWidgetList widgets = QApplication::allWidgets();
    QList<std::pair<int, QPointer<QWidget>>> ordered;
    ordered.reserve(widgets.size());
    for (QWidget *widget : widgets) {
        int depth = 0;
        for (const QWidget *parent = widget->parentWidget(); parent; parent = parent->parentWidget())
            ++depth;
        ordered.append({depth, QPointer<QWidget>(widget)});
    }
    std::stable_sort(ordered.begin(), ordered.end(),
                     [](const auto &a, const auto &b) { return a.first < b.first; });
    QEvent event(QEvent::ApplicationFontChange);
    for (const auto &entry : std::as_const(ordered)) {
        if (entry.second)
            QCoreApplication::sendEvent(entry.second, &event);
    }
}

void BUiScale::scaleColumnWidths(qreal ratio)
{
    // The columns of tables and trees whose width was set (by the code or
    // by the user) keep their share of the text: wider with a larger
    // font. Stretched columns and those fitted to their contents follow
    // by themselves.
    const QWidgetList widgets = QApplication::allWidgets();
    for (QWidget *widget : widgets) {
        auto *header = qobject_cast<QHeaderView *>(widget);
        if (!header || header->orientation() != Qt::Horizontal) continue;
        for (int i = 0; i < header->count(); ++i) {
            const QHeaderView::ResizeMode mode = header->sectionResizeMode(i);
            if (mode != QHeaderView::Interactive && mode != QHeaderView::Fixed) continue;
            if (header->isSectionHidden(i)) continue;
            if (header->stretchLastSection() && i == header->count() - 1) continue;
            header->resizeSection(i, qMax(header->minimumSectionSize(),
                                          qRound(header->sectionSize(i) * ratio)));
        }
    }
}

void BUiScale::scaleDialogs(qreal ratio)
{
    // A dialog was given its size in pixels for the font so far (resize()
    // with px() when it was made, or by the user). It keeps its share of
    // the text, as the columns do; never beyond the screen. Message boxes
    // and dialogs of a fixed size measure themselves.
    const QWidgetList windows = QApplication::topLevelWidgets();
    for (QWidget *window : windows) {
        auto *dialog = qobject_cast<QDialog *>(window);
        if (!dialog || qobject_cast<QMessageBox *>(dialog)) continue;
        const QLayout *layout = dialog->layout();
        if ((layout && layout->sizeConstraint() == QLayout::SetFixedSize)
            || dialog->minimumSize() == dialog->maximumSize())
            continue;

        QRect available;
        if (const QScreen *screen = dialog->screen())
            available = screen->availableGeometry();

        // A minimum size in pixels is the dialog's own business: set in
        // track(), it follows by itself; the one of a layout is measured
        // again with the new font
        QSize wanted(qRound(dialog->width() * ratio), qRound(dialog->height() * ratio));
        if (available.isValid()) wanted = wanted.boundedTo(available.size());
        dialog->resize(wanted);

        // An open dialog that grew stays on its screen
        if (dialog->isVisible() && available.isValid()) {
            QRect frame = dialog->frameGeometry();
            if (!available.contains(frame)) {
                frame.moveRight(qMin(frame.right(), available.right()));
                frame.moveBottom(qMin(frame.bottom(), available.bottom()));
                frame.moveLeft(qMax(frame.left(), available.left()));
                frame.moveTop(qMax(frame.top(), available.top()));
                dialog->move(frame.topLeft());
            }
        }
    }
}

bool BUiScale::isWizardTitle(const QLabel *label)
{
    // Inside a QWizard, outside its pages, bold: the logo, the watermark
    // and the subtitle are not
    if (!qobject_cast<const QWizard *>(label->window()) || !label->font().bold())
        return false;
    for (const QWidget *w = label->parentWidget(); w && !w->isWindow(); w = w->parentWidget()) {
        if (qobject_cast<const QWizardPage *>(w)) return false;
    }
    return true;
}

void BUiScale::fitRowHeight(QTableView *table)
{
    // The height of the rows of a table is asked from the style once, when
    // the table is made: it does not follow a font that changes later, and
    // some styles give a fixed one. Asked again here, and never lower than
    // a line of the table's font with some air. A height that the code of
    // a table chose itself is left alone.
    QHeaderView *rows = table->verticalHeader();
    if (!rows) return;
    const int current = rows->defaultSectionSize();
    const QVariant set = rows->property(kRowHeightProperty);
    const int styleHeight = qMax(rows->minimumSectionSize(),
        rows->style()->pixelMetric(QStyle::PM_HeaderDefaultSectionSizeVertical, nullptr, rows));
    // Qt's own value differs from the style's by the font it was made with
    const bool untouched = set.isValid() ? set.toInt() == current
                                         : qAbs(current - styleHeight) <= styleHeight / 2;
    if (!untouched) return;

    const int wanted = qMax(styleHeight,
                            table->fontMetrics().height() + px(8, table->font()));
    rows->setProperty(kRowHeightProperty, wanted);
    if (wanted != current)
        rows->setDefaultSectionSize(wanted);
}

void BUiScale::bind(QWidget *widget, Role role, Styles styles)
{
    if (!widget) return;
    widget->setProperty(kRoleProperty, static_cast<int>(role));
    widget->setProperty(kStylesProperty, static_cast<int>(styles));
    widget->setFont(instance().font(role, styles));
}

void BUiScale::track(QObject *context, const std::function<void()> &apply)
{
    if (!context || !apply) return;
    apply();
    connect(&instance(), &BUiScale::changed, context, apply);
}

namespace {

/// Cancel (and Close, every button of the reject role) leftmost in a row of
/// dialog buttons, in every dialog alike: the styles put it on the right
/// (Windows, KDE, GNOME) or in the middle (macOS)
void cancelToTheLeft(QDialogButtonBox *box)
{
    auto *layout = qobject_cast<QBoxLayout *>(box->layout());
    if (!layout || box->orientation() != Qt::Horizontal)
        return;
    int position = 0;
    for (QAbstractButton *button : box->buttons()) {
        if (box->buttonRole(button) != QDialogButtonBox::RejectRole)
            continue;
        layout->removeWidget(button);
        layout->insertWidget(position++, button);
    }
    if (position == 0)
        return;
    // The others to the right of a stretch, if the style left none between
    for (int i = position; i < layout->count(); ++i) {
        if (layout->itemAt(i)->spacerItem())
            return;
        if (layout->itemAt(i)->widget() && layout->itemAt(i)->widget()->isVisible())
            break;
    }
    layout->insertStretch(position, 1);
}

/// The same in a wizard: Cancel left, the steps right
void wizardCancelLeft(QWizard *wizard)
{
    if (wizard->property("buiscaleCancelLeft").toBool())
        return;
    wizard->setProperty("buiscaleCancelLeft", true);
    QList<QWizard::WizardButton> buttons{QWizard::CancelButton, QWizard::Stretch};
    if (wizard->options() & QWizard::HaveHelpButton) buttons << QWizard::HelpButton;
    if (wizard->options() & QWizard::HaveCustomButton1) buttons << QWizard::CustomButton1;
    if (wizard->options() & QWizard::HaveCustomButton2) buttons << QWizard::CustomButton2;
    if (wizard->options() & QWizard::HaveCustomButton3) buttons << QWizard::CustomButton3;
    buttons << QWizard::BackButton << QWizard::NextButton << QWizard::CommitButton << QWizard::FinishButton;
    wizard->setButtonLayout(buttons);
}

} // namespace

bool BUiScale::eventFilter(QObject *watched, QEvent *event)
{
    // Cancel on the left in every dialog, message box and wizard
    if (event->type() == QEvent::Show) {
        if (auto *box = qobject_cast<QDialogButtonBox *>(watched)) {
            // After the box has laid its buttons out for this show
            QPointer<QDialogButtonBox> guarded(box);
            QTimer::singleShot(0, box, [guarded]() {
                if (guarded) cancelToTheLeft(guarded);
            });
        } else if (auto *wizard = qobject_cast<QWizard *>(watched)) {
            wizardCancelLeft(wizard);
        } else if (auto *message = qobject_cast<QMessageBox *>(watched)) {
            // A message box measures itself before the interface font has
            // reached its label; on a real desktop it then showed only the
            // first wrapped line in a narrow window ("gRPC login failed:").
            // Measure again once the fonts are in place, wide enough for a
            // sentence, and let the text be selected and copied.
            QPointer<QMessageBox> guarded(message);
            QTimer::singleShot(0, message, [guarded]() {
                if (!guarded) return;
                if (auto *label = guarded->findChild<QLabel *>(QStringLiteral("qt_msgbox_label"))) {
                    label->setTextInteractionFlags(label->textInteractionFlags()
                                                   | Qt::TextSelectableByMouse);
                    const int wanted = qMin(label->fontMetrics().horizontalAdvance(label->text()),
                                            BUiScale::px(480));
                    if (label->minimumWidth() < wanted) label->setMinimumWidth(wanted);
                }
                guarded->adjustSize();
            });
        }
    }

    // Every table, when it is first shown and when its font changes
    if (event->type() == QEvent::Polish || event->type() == QEvent::FontChange) {
        if (auto *table = qobject_cast<QTableView *>(watched))
            fitRowHeight(table);
    }

    // Every push button: the theme's "min-width: 80px" becomes the button's
    // minimum, and a crowded layout squeezed it down to that, cutting its
    // text ("egend", "tualisier"). Never narrower than its text, unless
    // the code bounds its width itself (small browse buttons).
    if (event->type() == QEvent::Polish || event->type() == QEvent::FontChange
        || event->type() == QEvent::StyleChange) {
        if (auto *button = qobject_cast<QPushButton *>(watched)) {
            // After the style sheet has set its minimum (it polishes after
            // this filter has seen the event)
            QPointer<QPushButton> guarded(button);
            QTimer::singleShot(0, button, [guarded]() {
                if (!guarded || guarded->maximumWidth() != QWIDGETSIZE_MAX
                    || guarded->text().isEmpty()
                    || guarded->property("buiscaleMayShrink").toBool())
                    return;
                const int wanted = guarded->sizeHint().width();
                if (guarded->minimumWidth() < wanted)
                    guarded->setMinimumWidth(wanted);
            });
        }
    }

    // Every combo box: its open list measured its rows with the menu font
    // of the style (Qt's combo menu delegate) and drew them in the zoomed
    // font, so the texts lay on top of each other; a styled delegate
    // measures with the font it draws. Its width follows its widest entry
    // whenever the entries or the font change, not only at the first show.
    if (event->type() == QEvent::Polish) {
        if (auto *combo = qobject_cast<QComboBox *>(watched)) {
            if (!combo->property("buiscaleDelegate").toBool()) {
                combo->setProperty("buiscaleDelegate", true);
                // Only Qt's own delegates: a delegate of the code stays
                QAbstractItemDelegate *own = combo->itemDelegate();
                if (!own || own->inherits("QComboMenuDelegate") || own->inherits("QComboBoxDelegate"))
                    combo->setItemDelegate(new QStyledItemDelegate(combo));
                if (combo->sizeAdjustPolicy() == QComboBox::AdjustToContentsOnFirstShow)
                    combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
            }
        }
    }

    // A window about to be shown: if something set a style or a style
    // sheet without setApplicationStyleSheet(), Qt has taken the per-class
    // fonts of the platform theme again, and this window's buttons, labels
    // and lists would be made at the desktop's size, not the zoomed one
    if (event->type() == QEvent::Polish && !m_applying && watched->isWidgetType()
        && static_cast<QWidget *>(watched)->isWindow()) {
        dropClassFonts();
    }

    // The title of a wizard page as Qt shows it: a label of QWizard's own,
    // bold, and without a subtitle 4 pt larger than the wizard's font, set
    // whenever a page is laid out and not following the zoom. A heading:
    // Title, as the headings of the other dialogs
    if (event->type() == QEvent::Polish) {
        if (auto *label = qobject_cast<QLabel *>(watched)) {
            if (!label->property(kRoleProperty).isValid() && isWizardTitle(label))
                bind(label, Title, Bold);
        }
    }

    // A style sheet that is applied again (another theme) gives a widget
    // the font back that it had when the sheet was first applied: a bound
    // widget would fall back to an old size. Other code that gives a bound
    // widget another size or family (QWizard does for its titles) is
    // overruled as well; weight and slant set afterwards stay.
    if ((event->type() == QEvent::StyleChange || event->type() == QEvent::FontChange)
        && watched->isWidgetType()) {
        const QVariant role = watched->property(kRoleProperty);
        if (role.isValid()) {
            auto *widget = static_cast<QWidget *>(watched);
            QFont wanted = font(static_cast<Role>(role.toInt()),
                                Styles(widget->property(kStylesProperty).toInt()));
            if (event->type() == QEvent::FontChange) {
                const QFont current = widget->font();
                if (qFuzzyCompare(current.pointSizeF(), wanted.pointSizeF())
                    && current.pixelSize() == wanted.pixelSize()
                    && current.families() == wanted.families()) {
                    wanted = current;
                } else {
                    QFont kept = current;
                    kept.setFamilies(wanted.families());
                    if (wanted.pointSizeF() > 0) kept.setPointSizeF(wanted.pointSizeF());
                    else kept.setPixelSize(wanted.pixelSize());
                    wanted = kept;
                }
            }
            if (widget->font() != wanted)
                widget->setFont(wanted);
        }
    }

    // The desktop's font was changed while the application runs: take the
    // new one as base. Qt does not replace a font the application set.
    if (watched == qApp && !m_applying && !m_baseFixed && m_baseKnown
        && (event->type() == QEvent::ApplicationFontChange
            || event->type() == QEvent::ThemeChange)) {
        const QFont desktop = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
        if (desktop != m_desktopFont) {
            m_desktopFont = desktop;
            m_baseFont = desktop;
            apply();
        }
    }
    return QObject::eventFilter(watched, event);
}
