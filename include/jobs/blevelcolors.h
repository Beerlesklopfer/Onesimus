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
};

#endif // BLEVELCOLORS_H
