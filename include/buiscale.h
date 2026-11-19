#ifndef BUISCALE_H
#define BUISCALE_H

#include <QFont>
#include <QObject>
#include <QString>
#include <functional>

class QLabel;
class QTableView;
class QWidget;

/**
 * @brief Font sizes and pixel measures of the interface
 *
 * The base is the font of the desktop (what the platform theme reports as
 * the general font), multiplied by the interface zoom. Nothing in the
 * application sets an absolute point or pixel size for text: every size is
 * a role relative to that base.
 *
 * - Fonts: font(role) for the application font at a role, scaled(font, role)
 *   relative to any font (a painted widget uses its own font()).
 * - Pixels: px(n) and px(n, font) turn a measure that was designed for a
 *   10 pt font at 96 dpi into one for the font in use.
 * - Zoom: 70 % to 250 % in steps of 10 %. setZoomPercent() sets the font of
 *   the application, sets the font of every bound widget again and emits
 *   changed(). The class does not store the factor; the main window reads
 *   and writes it through BSettings.
 *
 * A widget that takes its font from the application follows by itself. A
 * widget with a font of its own is bound with bind(); a measure in pixels
 * that has to follow is set in track(). Tables are looked after here: the
 * height of their rows follows their font (the style gives a fixed one),
 * and the widths of their columns are scaled with a change of the zoom.
 *
 * Windows of their own (dialogs, wizards, message boxes) take the same
 * font. Qt keeps a second set of fonts per widget class (QPushButton,
 * QLabel, QMessageBox, item views, menus, ...) that the platform theme
 * fills in: unzoomed desktop fonts. Qt fills it in again whenever the style
 * changes, that is on every QApplication::setStyleSheet(). A dialog made
 * afterwards would show its buttons and labels at the desktop's size and
 * its tabs and group boxes at the zoomed one. This class empties that set
 * after its own style sheet (setApplicationStyleSheet()) and, as a net for
 * any other path, when a window is polished. The size of an open dialog is
 * scaled with a change of the zoom.
 */
class BUiScale : public QObject
{
    Q_OBJECT

public:
    /// Text sizes relative to the base font
    enum Role {
        Tiny,       ///< 0.8: axis labels, marks inside painted cells
        Small,      ///< 0.9: hints, captions, secondary text
        Normal,     ///< 1.0: the base font
        Large,      ///< 1.2: section titles
        Title,      ///< 1.4: figures and page titles
        Headline    ///< 1.8: the one headline of a page
    };

    enum Style {
        Plain = 0x0,
        Bold = 0x1,
        Italic = 0x2,
        Monospace = 0x4     ///< The fixed-pitch font of the desktop
    };
    Q_DECLARE_FLAGS(Styles, Style)

    static constexpr int MinZoom = 70;
    static constexpr int MaxZoom = 250;
    static constexpr int ZoomStep = 10;
    static constexpr int DefaultZoom = 100;

    /// A zoom value inside the range, on a step
    static constexpr int clampZoom(int percent)
    {
        const int limited = percent < MinZoom ? MinZoom : (percent > MaxZoom ? MaxZoom : percent);
        return ((limited + ZoomStep / 2) / ZoomStep) * ZoomStep;
    }

    static qreal roleFactor(Role role);

    /// @p base at a role: same family, size multiplied by the role's factor
    static QFont scaled(const QFont &base, Role role, Styles styles = Plain);

    /// Size of @p font relative to 10 pt at 96 dpi
    static qreal scaleOf(const QFont &font);

    /// @p designPixels (meant for 10 pt at 96 dpi) for @p font; at least 1
    static int px(int designPixels, const QFont &font);

    /// The same for the application font
    static int px(int designPixels);

    /// The other way round: a measure on screen as one for 10 pt at 96 dpi.
    /// For what is stored (a column width) and has to fit any zoom later.
    static int designPx(int pixels);

    /// "font-size:9.9pt" for rich text, from the application font
    static QString cssFontSize(Role role);

    /**
     * @brief Fills in the sizes of a style sheet
     *
     * A style sheet names a size by its role: "font-size: @small;". The
     * tokens @tiny, @small, @normal, @large, @title and @headline become
     * the size of that role in points (application font). Call it again
     * after changed(), for instance in track().
     */
    static QString resolveCss(const QString &styleSheet);

    /**
     * @brief A theme (built in or the user's own) without sizes of its own
     *
     * Removes every "font-size", "font-family" and "font" declaration that
     * names an absolute value (pt, px, a family), so that a theme cannot
     * override the desktop's font and the interface zoom; then fills in the
     * role tokens with resolveCss(). Weight and slant ("font-weight",
     * "font-style") stay.
     */
    static QString themeCss(const QString &styleSheet);

    /**
     * @brief Sets the style sheet of the application, filtered by themeCss()
     *
     * Afterwards the per-class fonts Qt took from the platform theme are
     * dropped and every widget is given the interface font again. Does not
     * emit changed().
     */
    void setApplicationStyleSheet(const QString &styleSheet);

    /**
     * @brief Style sheet rules for what a theme gives in pixels
     *
     * A theme cannot name a size relative to the font. The few of its
     * measures that hold text or stand beside it (the title room of a
     * group box, the box of a check box) are given here for the
     * application font, to be appended to the theme. Empty at 10 pt.
     */
    static QString themeRules();

    static BUiScale &instance();

    /// The desktop's font, without the zoom
    QFont baseFont() const;

    /**
     * @brief Sets the base font instead of asking the desktop
     *
     * For tests and pictures that must not depend on the machine. The
     * desktop is not asked again afterwards.
     */
    void setBaseFont(const QFont &font);

    int zoomPercent() const { return m_zoomPercent; }

    /// Base font times zoom: the font of the application
    QFont applicationFont() const;

    /// The application font at a role
    QFont font(Role role, Styles styles = Plain) const;

    /**
     * @brief Gives a widget the font of a role, now and after every change
     *
     * For a widget that needs a size other than the application's. The
     * family and the size come from here; weight and slant only if asked
     * for in @p styles.
     */
    static void bind(QWidget *widget, Role role, Styles styles = Plain);

    /**
     * @brief Calls @p apply now and after every change of font or zoom
     *
     * For measures in pixels that follow the font (a maximum height, an
     * icon size). @p context ends the calls when it is destroyed.
     */
    static void track(QObject *context, const std::function<void()> &apply);

public slots:
    /// Clamped with clampZoom(); applies the font to the application
    void setZoomPercent(int percent);
    void zoomIn() { setZoomPercent(m_zoomPercent + ZoomStep); }
    void zoomOut() { setZoomPercent(m_zoomPercent - ZoomStep); }
    void resetZoom() { setZoomPercent(DefaultZoom); }

signals:
    /// The zoom has a new value
    void zoomChanged(int percent);
    /// The application font changed (zoom or base font)
    void changed();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    BUiScale();
    void apply();
    void rebindWidgets();
    void propagateToWidgets();
    bool hasClassFonts() const;
    void dropClassFonts();
    void scaleColumnWidths(qreal ratio);
    static void scaleDialogs(qreal ratio);
    static void fitRowHeight(QTableView *table);
    static bool isWizardTitle(const QLabel *label);

    mutable QFont m_baseFont;
    mutable QFont m_desktopFont;    ///< What the desktop reported when the base was taken
    mutable bool m_baseKnown = false;
    bool m_baseFixed = false;       ///< Set by setBaseFont(): the desktop is not asked
    bool m_applying = false;
    bool m_filterInstalled = false;
    int m_zoomPercent = DefaultZoom;
    qreal m_appliedScale = 0;       ///< scaleOf() of the font the application has
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BUiScale::Styles)

#endif // BUISCALE_H
