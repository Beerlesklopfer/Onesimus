// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BTHIRDPARTYNOTICES_H
#define BTHIRDPARTYNOTICES_H

#include <QDialog>
#include <QString>
#include <QStringList>

class QComboBox;
class QTextBrowser;
class QUrl;

/**
 * @brief The third-party notices and licence texts, from the Qt resources
 *
 * CMakeLists.txt compiles THIRD-PARTY-NOTICES.md, LICENSE and the *.txt files in LICENSES
 * into the program under :/legal/ (the same files every package ships). The
 * dialog shows one document at a time: the notices as Markdown, a licence as
 * plain text. A link in the notices to a licence file (LICENSES/MIT.txt)
 * opens that document; web and mail links go to the desktop.
 */
class BThirdPartyNoticesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BThirdPartyNoticesDialog(QWidget *parent = nullptr);

    /// Resource paths relative to :/legal/, the notices first
    static QStringList documents();
    /// The text of a document (relative to :/legal/); empty when missing
    static QString documentText(const QString &document);
    /// The document a link in the notices refers to, or an empty string
    static QString documentForLink(const QUrl &link);

    /// The document on show (relative to :/legal/)
    QString currentDocument() const;
    /// Shows a document; false when it is not one of documents()
    bool showDocument(const QString &document);

private:
    void onAnchorClicked(const QUrl &link);

    QComboBox *m_documents = nullptr;
    QTextBrowser *m_text = nullptr;
};

#endif // BTHIRDPARTYNOTICES_H
