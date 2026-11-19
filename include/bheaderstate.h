#ifndef BHEADERSTATE_H
#define BHEADERSTATE_H

#include <QByteArray>
#include <QString>

class QHeaderView;

/**
 * @brief Saved state of a table header: order, widths, hidden columns, sort column
 *
 * The tables keep QHeaderView::saveState() in BSettings together with the
 * interface scale the widths were saved at (BUiScale::px(1000)). Widths
 * saved at another interface zoom are scaled to the current one when they
 * are restored. After a restore every column is resizable by the user and
 * the last visible column takes the remaining width.
 */
namespace BHeaderState {

/// Scale the widths are measured in now: pixels per 1000 design pixels
int currentScale();

/// Makes every section resizable by the user and stretches the last visible one
void makeInteractive(QHeaderView *header);

/// Scales every section from @p fromScale to @p toScale (no signals)
void scaleSections(QHeaderView *header, int fromScale, int toScale);

/**
 * @brief Restores @p state saved at @p savedScale
 * @return false if @p state is empty or not a state of this header
 */
bool restore(QHeaderView *header, const QByteArray &state, int savedScale);

enum class Source {
    Saved,      ///< From the state stored in BSettings
    Legacy,     ///< From the old BColumnConfiguration group, which is removed now
    Nothing     ///< Nothing stored: the header keeps its defaults
};

/**
 * @brief restore(), or once the old per-column BColumnConfiguration group
 *
 * Older versions stored order, widths (for 10 pt) and visibility of a table
 * in the group @p legacyKey of QSettings(organization, application). When
 * nothing is in BSettings yet, that group is applied and then removed, so
 * that there is a single source from then on. The caller saves the header
 * to BSettings right after a Legacy result.
 */
Source restoreOrMigrate(QHeaderView *header, const QByteArray &state, int savedScale,
                        const QString &legacyKey);

} // namespace BHeaderState

#endif // BHEADERSTATE_H
