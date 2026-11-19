/**
 * @file bschedulelegend.cpp
 * @brief What the marks of the schedule timeline mean
 *
 * The pictures repeat the drawing code of BScheduleGanttWidget::drawJobBar()
 * and drawUnusedMark() in small; a change of a mark there belongs here too.
 */

#include "schedules/bschedulelegend.h"
#include "buiscale.h"
#include "jobs/blevelcolors.h"

#include <QGridLayout>
#include <QGuiApplication>
#include <QLabel>
#include <QPainter>
#include <QScreen>

BScheduleLegend::BScheduleLegend(QWidget *parent)
    : QFrame(parent, Qt::Popup)
{
    setFrameShape(QFrame::StyledPanel);
    setAttribute(Qt::WA_DeleteOnClose);
    build();
}

QList<BScheduleLegend::Mark> BScheduleLegend::marks()
{
    return {Mark::Full, Mark::Differential, Mark::Incremental, Mark::VirtualFull,
            Mark::Estimated, Mark::Unmeasured, Mark::Percentile90,
            Mark::WaitCertain, Mark::WaitUncertain, Mark::WaitStrip, Mark::MayWait, Mark::StartsLater,
            Mark::Unused, Mark::Now};
}

QString BScheduleLegend::text(Mark mark)
{
    switch (mark) {
    case Mark::Full:
        return tr("Full backup. The length of the bar is the median duration of the job's measured runs at this level.");
    case Mark::Differential:
        return tr("Differential backup.");
    case Mark::Incremental:
        return tr("Incremental backup.");
    case Mark::VirtualFull:
        return tr("Virtual full backup.");
    case Mark::Estimated:
        return tr("Pale and dashed, \"~\" in front of the letter: no run at this level was measured; "
                  "the length is estimated from the job's runs at other levels.");
    case Mark::Unmeasured:
        return tr("Dashed outline with \"?\": the job never ran, so there is no duration. Only the start is shown.");
    case Mark::Percentile90:
        return tr("Lighter extension with a line at its end: nine of ten runs ended by then (90th percentile).");
    case Mark::WaitCertain:
        return tr("Filled hourglass: the run will wait. The coloured line behind it is the scheduled time.");
    case Mark::WaitUncertain:
        return tr("Hourglass outline: the run may wait, the prediction is not certain (see the list of predicted waits).");
    case Mark::WaitStrip:
        return tr("Hatched strip: the wait, from the scheduled time to the predicted start. A dashed outline: not certain.");
    case Mark::MayWait:
        return tr("\"?\" in front of a bar: the prediction for this run is not certain, it may have to wait.");
    case Mark::StartsLater:
        return tr("\"»\" at the right edge: the run starts after the last day on display.");
    case Mark::Unused:
        return tr("Open diamond: no job uses this schedule. It marks where a run would start.");
    case Mark::Now:
        return tr("Red line: now.");
    }
    return QString();
}

QPixmap BScheduleLegend::picture(Mark mark) const
{
    const qreal dpr = devicePixelRatioF();
    const int w = BUiScale::px(56);
    const int h = BUiScale::px(22);
    QPixmap pixmap(QSize(w, h) * dpr);
    pixmap.setDevicePixelRatio(dpr);
    pixmap.fill(palette().base().color());

    QPainter p(&pixmap);
    p.setRenderHint(QPainter::Antialiasing);
    const QColor ink = palette().text().color();
    const QRect bar(BUiScale::px(18), BUiScale::px(3), BUiScale::px(30), h - BUiScale::px(6));
    QFont bold = BUiScale::scaled(font(), BUiScale::Small, BUiScale::Bold);
    p.setFont(bold);

    auto level = [&](const QString &code) {
        const QColor fill = BLevelColors::getLevelColor(code);
        p.setPen(Qt::NoPen);
        p.setBrush(fill);
        p.drawRoundedRect(bar, 3, 3);
        p.setPen(Qt::white);
        p.drawText(bar, Qt::AlignCenter, code);
    };
    auto hourglass = [&](bool filled) {
        const QRectF glass(bar.left() - BUiScale::px(11) + 0.5, bar.top() + BUiScale::px(1) + 0.5,
                           BUiScale::px(9), bar.height() - BUiScale::px(2) - 1);
        QPolygonF poly;
        poly << glass.topLeft() << glass.topRight() << glass.center()
             << glass.bottomRight() << glass.bottomLeft() << glass.center();
        QColor fill = ink;
        fill.setAlpha(215);
        p.setPen(QPen(ink, 1));
        p.setBrush(filled ? QBrush(fill) : QBrush(Qt::NoBrush));
        p.drawPolygon(poly);
        const QColor level = BLevelColors::getLevelColor(QStringLiteral("I"));
        p.setPen(QPen(level, 2));
        p.drawLine(QPointF(glass.right() + 1.5, bar.top()), QPointF(glass.right() + 1.5, bar.bottom()));
    };

    switch (mark) {
    case Mark::Full:         level(QStringLiteral("F")); break;
    case Mark::Differential: level(QStringLiteral("D")); break;
    case Mark::Incremental:  level(QStringLiteral("I")); break;
    case Mark::VirtualFull:  level(QStringLiteral("V")); break;
    case Mark::Estimated: {
        const QColor fill = BLevelColors::getLevelColor(QStringLiteral("F"));
        QColor pale = fill;
        pale.setAlpha(110);
        p.setPen(QPen(fill, 1.5, Qt::DashLine));
        p.setBrush(pale);
        p.drawRoundedRect(bar.adjusted(0, 0, -1, -1), 3, 3);
        p.setPen(ink);
        p.drawText(bar, Qt::AlignCenter, QStringLiteral("~F"));
        break;
    }
    case Mark::Unmeasured: {
        const QColor fill = BLevelColors::getLevelColor(QStringLiteral("F"));
        const QRect small(bar.left(), bar.top(), BUiScale::px(14), bar.height());
        p.setPen(QPen(fill, 1.5, Qt::DashLine));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(small.adjusted(0, 0, -1, -1), 3, 3);
        p.setPen(ink);
        p.drawText(small, Qt::AlignCenter, QStringLiteral("?"));
        break;
    }
    case Mark::Percentile90: {
        const QColor fill = BLevelColors::getLevelColor(QStringLiteral("F"));
        const QRect median(bar.left(), bar.top(), BUiScale::px(18), bar.height());
        const QRect tail(median.right(), bar.top(), bar.right() - median.right(), bar.height());
        p.setPen(Qt::NoPen);
        p.setBrush(fill);
        p.drawRoundedRect(median, 3, 3);
        QColor light = fill;
        light.setAlpha(60);
        p.setBrush(light);
        p.drawRect(tail);
        p.setPen(QPen(fill, 2));
        p.drawLine(tail.right(), bar.top(), tail.right(), bar.bottom());
        break;
    }
    case Mark::WaitCertain:
        hourglass(true);
        level(QStringLiteral("I"));
        break;
    case Mark::WaitUncertain:
        hourglass(false);
        level(QStringLiteral("I"));
        break;
    case Mark::WaitStrip: {
        const QRect strip(BUiScale::px(4), bar.top() + BUiScale::px(4), BUiScale::px(26),
                          bar.height() - BUiScale::px(8));
        QColor hatch = ink;
        hatch.setAlpha(170);
        p.setPen(Qt::NoPen);
        p.setBrush(QBrush(hatch, Qt::BDiagPattern));
        p.drawRect(strip);
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(ink, 1));
        p.drawRect(strip.adjusted(0, 0, -1, -1));
        const QColor fill = BLevelColors::getLevelColor(QStringLiteral("I"));
        p.setPen(Qt::NoPen);
        p.setBrush(fill);
        p.drawRoundedRect(QRect(strip.right() + 1, bar.top(), w - strip.right() - BUiScale::px(4), bar.height()), 3, 3);
        break;
    }
    case Mark::MayWait:
        p.setPen(ink);
        p.drawText(QRect(bar.left() - BUiScale::px(12), bar.top(), BUiScale::px(10), bar.height()),
                   Qt::AlignCenter, QStringLiteral("?"));
        level(QStringLiteral("I"));
        break;
    case Mark::StartsLater: {
        const QRect strip(BUiScale::px(4), bar.top() + BUiScale::px(4), w - BUiScale::px(8),
                          bar.height() - BUiScale::px(8));
        QColor hatch = ink;
        hatch.setAlpha(170);
        p.setPen(Qt::NoPen);
        p.setBrush(QBrush(hatch, Qt::BDiagPattern));
        p.drawRect(strip);
        const QRect markRect(w - BUiScale::px(16), bar.top(), BUiScale::px(12), bar.height());
        p.fillRect(markRect.adjusted(0, BUiScale::px(2), 0, -BUiScale::px(2)), palette().base());
        p.setPen(ink);
        p.drawText(markRect, Qt::AlignCenter, QStringLiteral("»"));
        break;
    }
    case Mark::Unused: {
        const qreal half = (bar.height() - 1) / 2.0;
        const QPointF c(bar.left() + BUiScale::px(8) + 0.5, bar.center().y() + 0.5);
        QPolygonF diamond;
        diamond << QPointF(c.x(), c.y() - half) << QPointF(c.x() + half, c.y())
                << QPointF(c.x(), c.y() + half) << QPointF(c.x() - half, c.y());
        const QColor base = palette().base().color();
        const QColor muted((ink.red() * 3 + base.red() * 2) / 5, (ink.green() * 3 + base.green() * 2) / 5,
                           (ink.blue() * 3 + base.blue() * 2) / 5);
        p.setPen(QPen(muted, 1.2));
        p.setBrush(Qt::NoBrush);
        p.drawPolygon(diamond);
        break;
    }
    case Mark::Now: {
        const int x = w / 2;
        p.setPen(QPen(Qt::red, 2));
        p.drawLine(x, 0, x, h);
        QPolygonF triangle;
        triangle << QPointF(x - BUiScale::px(4), 0) << QPointF(x + BUiScale::px(4), 0)
                 << QPointF(x, BUiScale::px(6));
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::red);
        p.drawPolygon(triangle);
        break;
    }
    }
    return pixmap;
}

void BScheduleLegend::build()
{
    auto *grid = new QGridLayout(this);
    grid->setHorizontalSpacing(BUiScale::px(10));
    auto *title = new QLabel(tr("<b>What the marks mean</b>"), this);
    grid->addWidget(title, 0, 0, 1, 2);
    int row = 1;
    for (Mark mark : marks()) {
        auto *icon = new QLabel(this);
        icon->setPixmap(picture(mark));
        auto *label = new QLabel(text(mark), this);
        label->setWordWrap(true);
        label->setMinimumWidth(BUiScale::px(380));
        grid->addWidget(icon, row, 0, Qt::AlignTop);
        grid->addWidget(label, row, 1);
        ++row;
    }
}

void BScheduleLegend::popupBelow(QWidget *anchor)
{
    adjustSize();
    QPoint pos = anchor->mapToGlobal(QPoint(0, anchor->height()));
    if (const QScreen *screen = anchor->screen()) {
        const QRect area = screen->availableGeometry();
        pos.setX(qBound(area.left(), pos.x(), area.right() - width()));
        if (pos.y() + height() > area.bottom())
            pos.setY(qMax(area.top(), anchor->mapToGlobal(QPoint(0, 0)).y() - height()));
    }
    move(pos);
    show();
}
