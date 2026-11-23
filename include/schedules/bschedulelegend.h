// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSCHEDULELEGEND_H
#define BSCHEDULELEGEND_H

#include <QFrame>
#include <QPixmap>
#include <QString>

/**
 * @file bschedulelegend.h
 * @brief What the marks of the schedule timeline mean
 *
 * A popup under the "Legend" button of the schedule view: every mark the
 * timeline draws (level colours, measured and estimated bars, the 90th
 * percentile, hourglass, wait strip, question mark, "»", the open diamond
 * of an unused schedule, the measured runs, the line of now), drawn the way the timeline
 * draws it, with one sentence each. The pictures follow the palette and
 * the font size (BUiScale).
 */
class BScheduleLegend : public QFrame
{
    Q_OBJECT

public:
    enum class Mark {
        Full, Differential, Incremental, VirtualFull,
        Estimated, Unmeasured, Percentile90,
        WaitCertain, WaitUncertain, WaitStrip, MayWait, StartsLater,
        Unused, Waits, Measured, MeasuredFailed, Now,
    };

    explicit BScheduleLegend(QWidget *parent = nullptr);

    /// The picture of a mark, as the timeline draws it
    QPixmap picture(Mark mark) const;
    /// Its sentence
    static QString text(Mark mark);
    static QList<Mark> marks();

    /// Shows the legend as a popup below @p anchor
    void popupBelow(QWidget *anchor);

private:
    void build();
};

#endif // BSCHEDULELEGEND_H
