// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bthirdpartynotices.h"

#include "buiscale.h"

#include <QComboBox>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QLabel>
#include <QRegularExpression>
#include <QTextBrowser>
#include <QUrl>
#include <QVBoxLayout>

namespace {
const QString kRoot = QStringLiteral(":/legal/");
const QString kNotices = QStringLiteral("THIRD-PARTY-NOTICES.md");
} // namespace

QStringList BThirdPartyNoticesDialog::documents()
{
    QStringList docs;
    if (QFile::exists(kRoot + kNotices))
        docs << kNotices;
    if (QFile::exists(kRoot + QStringLiteral("LICENSE")))
        docs << QStringLiteral("LICENSE");
    const QStringList texts = QDir(kRoot + QStringLiteral("LICENSES"))
                                  .entryList({QStringLiteral("*.txt")}, QDir::Files, QDir::Name);
    for (const QString &text : texts)
        docs << QStringLiteral("LICENSES/") + text;
    return docs;
}

QString BThirdPartyNoticesDialog::documentText(const QString &document)
{
    QFile file(kRoot + document);
    if (!file.open(QIODevice::ReadOnly))
        return QString();
    return QString::fromUtf8(file.readAll());
}

QString BThirdPartyNoticesDialog::documentForLink(const QUrl &link)
{
    if (!link.isRelative() || link.path().isEmpty())
        return QString();
    QString path = QDir::cleanPath(link.path());
    if (path.startsWith(QLatin1String("./")))
        path = path.mid(2);
    return documents().contains(path) ? path : QString();
}

BThirdPartyNoticesDialog::BThirdPartyNoticesDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Third-party licences"));
    BUiScale::track(this, [this]() { resize(BUiScale::px(760), BUiScale::px(620)); });

    auto *layout = new QVBoxLayout(this);

    auto *pickRow = new QHBoxLayout();
    auto *pickLabel = new QLabel(tr("Document:"), this);
    m_documents = new QComboBox(this);
    pickLabel->setBuddy(m_documents);
    for (const QString &doc : documents()) {
        const QString title = doc == kNotices ? tr("Third-party notices")
                              : doc == QLatin1String("LICENSE") ? tr("Onesimus licence (AGPL-3.0)")
                                                                : doc;
        m_documents->addItem(title, doc);
    }
    pickRow->addWidget(pickLabel);
    pickRow->addWidget(m_documents, 1);
    layout->addLayout(pickRow);

    m_text = new QTextBrowser(this);
    m_text->setOpenLinks(false);
    layout->addWidget(m_text, 1);
    connect(m_text, &QTextBrowser::anchorClicked, this, &BThirdPartyNoticesDialog::onAnchorClicked);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    connect(m_documents, &QComboBox::currentIndexChanged, this, [this](int index) {
        showDocument(m_documents->itemData(index).toString());
    });
    if (m_documents->count() > 0)
        showDocument(m_documents->itemData(0).toString());
    else
        m_text->setPlainText(tr("The third-party notices are missing from this build."));
}

QString BThirdPartyNoticesDialog::currentDocument() const
{
    return m_documents->currentData().toString();
}

bool BThirdPartyNoticesDialog::showDocument(const QString &document)
{
    const int index = m_documents->findData(document);
    if (index < 0)
        return false;
    if (m_documents->currentIndex() != index) {
        m_documents->setCurrentIndex(index); // shows it through currentIndexChanged
        return true;
    }
    QString text = documentText(document);
    if (document.endsWith(QLatin1String(".md"))) {
        // The SPDX header lines are HTML comments: not part of the notices
        static const QRegularExpression spdx(QStringLiteral("^<!--[^\\n]*-->\\n"),
                                             QRegularExpression::MultilineOption);
        text.remove(spdx);
        m_text->setFont(font());
        m_text->setMarkdown(text);
    } else {
        // The licence texts are laid out for a fixed-pitch font; the size
        // stays that of the interface (zoom)
        QFont fixed = font();
        fixed.setFamilies(QFontDatabase::systemFont(QFontDatabase::FixedFont).families());
        fixed.setStyleHint(QFont::TypeWriter);
        m_text->setFont(fixed);
        m_text->setPlainText(text);
    }
    return true;
}

void BThirdPartyNoticesDialog::onAnchorClicked(const QUrl &link)
{
    const QString doc = documentForLink(link);
    if (!doc.isEmpty()) {
        showDocument(doc);
        return;
    }
    if (!link.isRelative())
        QDesktopServices::openUrl(link);
}
