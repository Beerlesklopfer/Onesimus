#ifndef BLEVELCOLORS_H
#define BLEVELCOLORS_H

#include <QColor>
#include <QString>
#include "config/bsettings.h"

/**
 * @brief Utility class for backup level color coding
 * @since 1.0
 *
 * Provides consistent color coding for backup levels across the application.
 * Colors are used in table views, checkboxes, and other UI elements.
 * Colors can be customized in the settings dialog.
 */
class BLevelColors
{
public:
    /**
     * @brief Returns the background color for a given backup level
     * @param level The backup level code (F, I, D, etc.)
     * @return QColor for the level, or invalid color if unknown
     */
    static QColor getLevelColor(const QString &level)
    {
        return BSettings::instance().levelColor(level);
    }

    /**
     * @brief Returns the display name for a backup level
     * @param level The backup level code (F, I, D, etc.)
     * @return Human-readable name
     */
    static QString getLevelName(const QString &level)
    {
        if (level == "F") return "Full";
        if (level == "I") return "Incremental";
        if (level == "D") return "Differential";
        if (level == "V") return "Virtual Full";

        return level;  // Return code as-is if unknown
    }

    /**
     * @brief Returns the short display name for a backup level
     * @param level The backup level code (F, I, D, etc.)
     * @return Short name (1-4 characters)
     */
    static QString getLevelShortName(const QString &level)
    {
        if (level == "F") return "Full";
        if (level == "I") return "Incr";
        if (level == "D") return "Diff";
        if (level == "V") return "VFull";

        return level;
    }

    /**
     * @brief Returns black or white depending on background luminance
     * @param bg The background color
     * @return QColor(Qt::black) for light backgrounds, QColor(Qt::white) for dark ones
     */
    static QColor contrastColor(const QColor &bg)
    {
        double lum = 0.299 * bg.red() + 0.587 * bg.green() + 0.114 * bg.blue();
        return (lum > 140.0) ? QColor(Qt::black) : QColor(Qt::white);
    }

    /**
     * @brief Generates QSS rules for QCheckBox[level="X"] selectors
     *
     * Called by applyTheme() and appended to the global stylesheet so that
     * level-filter checkboxes pick up the correct background/foreground colors
     * from BSettings without any inline setStyleSheet() in widget code.
     *
     * @return QSS string with one rule per known backup level
     */
    static QString generateLevelQss()
    {
        const QStringList levels = {"F", "I", "D", "V"};
        QString qss;
        for (const QString &level : levels) {
            QColor bg = BSettings::instance().levelColor(level);
            if (!bg.isValid()) continue;
            QColor fg = contrastColor(bg);
            qss += QString(
                "QCheckBox[level=\"%1\"] {"
                " background-color: rgb(%2,%3,%4);"
                " color: rgb(%5,%6,%7);"
                " }\n"
            ).arg(level)
             .arg(bg.red()).arg(bg.green()).arg(bg.blue())
             .arg(fg.red()).arg(fg.green()).arg(fg.blue());
        }
        return qss;
    }
};

#endif // BLEVELCOLORS_H
