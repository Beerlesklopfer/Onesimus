#ifndef BLABELSTATE_H
#define BLABELSTATE_H

#include <QStyle>
#include <QVariant>
#include <QWidget>

/**
 * @brief Gives a label one of the themes' semantic colours
 *
 * The themes (resources/themes/dark.qss, light.qss) define
 * QLabel[state="success|info|warning|error|muted"] with a colour that is
 * readable on that theme's background. Code names the meaning and leaves the
 * colour to the theme; a colour set from code fits one theme at best.
 * An empty state returns to the theme's normal text colour.
 *
 * The widget's own style sheet must not set a colour, or it wins over the theme.
 */
inline void bSetLabelState(QWidget *label, const char *state)
{
    const QString name = QString::fromLatin1(state);
    label->setProperty("state", name.isEmpty() ? QVariant() : QVariant(name));
    // Property selectors are evaluated when the widget is polished
    label->style()->unpolish(label);
    label->style()->polish(label);
    label->update();
}

#endif // BLABELSTATE_H
