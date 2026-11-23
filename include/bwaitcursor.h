// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BWAITCURSOR_H
#define BWAITCURSOR_H

#include <QGuiApplication>

/**
 * @brief The wait cursor of one long operation, at most once on the stack
 *
 * QGuiApplication keeps override cursors on a stack: every
 * setOverrideCursor() needs exactly one restoreOverrideCursor(). The
 * connection of the main window starts and ends on many paths (connect,
 * Reconnect, auto-connect, a new pin, errors, disconnect); show() and
 * hide() may be called on any of them as often as they like and leave the
 * stack balanced: it pops only the cursor it pushed (others, such as a
 * dialog's, are expected to nest inside and be gone by then).
 */
class BWaitCursor
{
public:
    BWaitCursor() = default;
    BWaitCursor(const BWaitCursor &) = delete;
    BWaitCursor &operator=(const BWaitCursor &) = delete;
    ~BWaitCursor() { hide(); }

    void show()
    {
        if (m_shown)
            return;
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
        m_shown = true;
    }

    void hide()
    {
        if (!m_shown)
            return;
        QGuiApplication::restoreOverrideCursor();
        m_shown = false;
    }

    bool isShown() const { return m_shown; }

private:
    bool m_shown = false;
};

#endif // BWAITCURSOR_H
