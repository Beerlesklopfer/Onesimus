// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BRESOURCEREVIEWDIALOG_H
#define BRESOURCEREVIEWDIALOG_H

#include <QDialog>
#include <QPointer>

#include "config/bresourcepersistence.h"

class QLabel;
class QListWidget;
class QPlainTextEdit;
class QPushButton;
class QTabWidget;

/**
 * @file bresourcereviewdialog.h
 * @brief The dialogs of writing a resource: the review before, the report after
 */

/**
 * @brief The review before a resource is written
 *
 * The text before and after as a line diff (a removed line is marked "-",
 * an added one "+", and coloured), the edited directives in words, the
 * resources the change applies to (a JobDefs: every job that uses it), what
 * the Director names as referencing it, and the file. "Write to the
 * Director" is never the default button: Enter cancels.
 */
class BResourceReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BResourceReviewDialog(const BResourceReview &review, QWidget *parent = nullptr);

    QPlainTextEdit *diffView() const { return m_diff; }
    QListWidget *affectedList() const { return m_affected; }
    QPushButton *writeButton() const { return m_write; }

    /// The diff as text: "  same", "- removed", "+ added", one line each
    static QString diffText(const QString &oldText, const QString &newText);

    // --- Shared with the schedule dialogs ------------------------------------

    /// A read-only monospace view of a resource text (CRLF shown as lines)
    static QPlainTextEdit *textView(const QString &text, QWidget *parent);
    /// A selectable plain-text label that wraps
    static QLabel *wrapped(const QString &text, QWidget *parent);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void colourDiff();

    QString m_oldText;
    QString m_newText;
    QPlainTextEdit *m_diff = nullptr;
    QListWidget *m_affected = nullptr;
    QPushButton *m_write = nullptr;
};

/**
 * @brief What came of a write: success with follow-ups, or why not
 *
 * A conflict shows the text the Director has next to the text that was to
 * be written and offers "Reload"; nothing is ever written from here. An
 * invalid configuration names file, line and column; a refused delete lists
 * the resources that still refer to it.
 */
class BResourceReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BResourceReportDialog(const BResourceReport &report, QWidget *parent = nullptr);

    bool reloadChosen() const { return m_reload; }
    QPushButton *reloadButton() const { return m_reloadButton; }
    QString problemText() const;

    /// "File ..., line 3, column 11: ..." or empty
    static QString problemLine(const std::optional<BConfig::Problem> &problem);

private:
    bool m_reload = false;
    QPushButton *m_reloadButton = nullptr;
    QLabel *m_problem = nullptr;
};

/**
 * @brief BResourceSaveUi with the two dialogs, modal over a widget
 *
 * A write that went through without follow-ups and with a reload is not
 * shown in a dialog; the view says so in its status line.
 */
class BResourceSaveDialogs : public BResourceSaveUi
{
public:
    explicit BResourceSaveDialogs(QWidget *parent) : m_parent(parent) {}

    bool confirm(const BResourceReview &review) override;
    Answer report(const BResourceReport &report) override;

    /// Does a report need a dialog?
    static bool needsDialog(const BResourceReport &report);

private:
    QPointer<QWidget> m_parent;
};

#endif // BRESOURCEREVIEWDIALOG_H
