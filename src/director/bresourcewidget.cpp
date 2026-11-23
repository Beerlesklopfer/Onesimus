// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bresourcewidget.cpp
 * @brief Implementation of base resource widget
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2025
 */

#include "director/bresourcewidget.h"
#include "buiscale.h"
#include "director/bresourcedialog.h"
#include "bcapabilitynotice.h"
#include "config/bdirectiveschema.h"
#include "config/bresourcechanges.h"
#include "config/bresourcepersistence.h"
#include "config/bresourcereviewdialog.h"
#include "config/bresourcetext.h"
#include "director/bdirectorconfigclient.h"
#include "schedules/model/brunspec.h"
#include "version.h"
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <private/qzipwriter_p.h>

BResourceWidget::BResourceWidget(const QString &resourceType, BDirector *director, QWidget *parent)
    : QWidget(parent)
    , m_director(director)
    , m_resourceType(resourceType)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Top bar: count label + edit button
    QHBoxLayout *topBar = new QHBoxLayout();
    m_countLabel = new QLabel(this);
    m_countLabel->setStyleSheet("font-weight: bold; padding: 4px;");
    topBar->addWidget(m_countLabel);
    topBar->addStretch();

    m_previewToggle = new QPushButton(tr("Preview"), this);
    m_previewToggle->setCheckable(true);
    m_previewToggle->setToolTip(tr("Toggle config file preview"));
    connect(m_previewToggle, &QPushButton::toggled, this, [this](bool) {
        BConfigResource res = selectedResource();
        if (!res.type().isEmpty()) {
            if (m_previewToggle->isChecked())
                updateConfigPreview(res);
            else
                updateDetails(res);
        }
    });
    topBar->addWidget(m_previewToggle);

    m_editButton = new QPushButton(tr("Edit..."), this);
    m_editButton->setEnabled(false);
    // Only Job and JobDefs have a write path (raw text over gRPC). Every
    // other type would be changed in this list only, with lists and nested
    // blocks lost, so it is not offered (backlog P1-26).
    if (!hasWritePath()) {
        m_editButton->setToolTip(tr("Only Job and JobDefs resources can be changed here; they are written "
                                    "to the Director. Change this resource in its configuration file."));
    }
    connect(m_editButton, &QPushButton::clicked,
            this, &BResourceWidget::onEditResource);
    topBar->addWidget(m_editButton);

    m_exportButton = new QPushButton(tr("Export .conf"), this);
    m_exportButton->setEnabled(false);
    connect(m_exportButton, &QPushButton::clicked,
            this, &BResourceWidget::exportToConf);
    topBar->addWidget(m_exportButton);

    m_exportZipButton = new QPushButton(tr("Export ZIP"), this);
    m_exportZipButton->setEnabled(false);
    connect(m_exportZipButton, &QPushButton::clicked,
            this, &BResourceWidget::exportToZip);
    topBar->addWidget(m_exportZipButton);

    layout->addLayout(topBar);

    // Splitter for tree and details
    m_splitter = new QSplitter(Qt::Horizontal, this);

    // Tree widget
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Source File")});
    m_treeWidget->setAlternatingRowColors(true);
    m_treeWidget->setRootIsDecorated(false);
    m_treeWidget->header()->setStretchLastSection(true);
    m_treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_splitter->addWidget(m_treeWidget);

    // Details text edit
    m_detailsEdit = new QTextEdit(this);
    m_detailsEdit->setReadOnly(true);
    BUiScale::bind(m_detailsEdit, BUiScale::Normal, BUiScale::Monospace);
    m_splitter->addWidget(m_detailsEdit);

    m_splitter->setSizes({300, 400});
    layout->addWidget(m_splitter);

    connect(m_treeWidget, &QTreeWidget::itemSelectionChanged,
            this, &BResourceWidget::onItemSelectionChanged);

    updateCountLabel();
}

BResourceWidget::~BResourceWidget()
{
}

void BResourceWidget::setResources(const QList<BConfigResource> &resources)
{
    m_resources = resources;
    populateTree();
    updateCountLabel();

    bool hasResources = !m_resources.isEmpty();
    m_exportButton->setEnabled(hasResources);
    m_exportZipButton->setEnabled(hasResources);
}

void BResourceWidget::selectResource(const QString &name)
{
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = m_treeWidget->topLevelItem(i);
        if (item && item->text(0) == name) {
            m_treeWidget->setCurrentItem(item);
            m_treeWidget->scrollToItem(item);
            return;
        }
    }
}

void BResourceWidget::updateCountLabel()
{
    m_countLabel->setText(tr("%1: %2 resource(s)")
                              .arg(m_resourceType)
                              .arg(m_resources.size()));
}

BConfigResource BResourceWidget::selectedResource() const
{
    QList<QTreeWidgetItem*> selected = m_treeWidget->selectedItems();
    if (selected.isEmpty()) {
        return BConfigResource();
    }

    int index = selected.first()->data(0, Qt::UserRole).toInt();
    if (index >= 0 && index < m_resources.size()) {
        return m_resources.at(index);
    }
    return BConfigResource();
}

void BResourceWidget::onItemSelectionChanged()
{
    BConfigResource resource = selectedResource();
    if (!resource.type().isEmpty()) {
        m_editButton->setEnabled(hasWritePath());
        if (m_previewToggle->isChecked())
            updateConfigPreview(resource);
        else
            updateDetails(resource);
        emit resourceSelected(resource);
    } else {
        m_editButton->setEnabled(false);
        m_detailsEdit->clear();
        emit selectionCleared();
    }
}

bool BResourceWidget::hasWritePath() const
{
    return m_resourceType == QLatin1String("Job") || m_resourceType == QLatin1String("JobDefs");
}

void BResourceWidget::onEditResource()
{
    BConfigResource res = selectedResource();
    if (res.type().isEmpty() || !hasWritePath()) return;

    BResourceDialog dlg(m_resourceType, res, this);
    if (!m_referenceData.isEmpty()) {
        dlg.setReferenceData(m_referenceData);
    }
    if (dlg.exec() != QDialog::Accepted)
        return;
    BConfigResource modified = dlg.resource();
    if (modified.name().isEmpty())
        modified.setName(res.name());

    // Job and JobDefs go to the Director; the list changes once it is
    // written. Without a Director nothing is written and nothing changes.
    if (!writeEdited(res, modified, dlg.formDirectives())) {
        QMessageBox::information(this, tr("%1 %2 was not changed").arg(m_resourceType, res.name()),
                                 tr("There is no Director connection to write the change to. Nothing was changed."));
    }
}

bool BResourceWidget::writeEdited(const BConfigResource &before, const BConfigResource &after,
                                  const QStringList &formDirectives)
{
    BConfig::ResourceType type;
    BCapabilities::Function function;
    if (m_resourceType == QLatin1String("Job")) {
        type = BConfig::ResourceType::Job;
        function = BCapabilities::Function::ModifyJob;
    } else if (m_resourceType == QLatin1String("JobDefs")) {
        type = BConfig::ResourceType::JobDefs;
        function = BCapabilities::Function::ModifyJobDefs;
    } else {
        return false;
    }
    // The configuration import dialog has no Director: nothing to write to
    if (!m_director)
        return false;

    const QString name = before.name();
    const QString title = tr("%1 %2 was not changed").arg(m_resourceType, name);

    // BNET, or a session without UpdateResource: the notice, nothing written
    if (!BCapabilityNotice::check(this, m_director, function, BCapabilities::Request::Explicit))
        return true;

    const QString typeName = m_resourceType;
    const BResourceChanges changes = BResourceChanges::between(
        before, after, formDirectives, [typeName](const QString &directive) {
            const QVariant def = BDirectiveSchema::instance().directive(typeName, directive).defaultValue;
            if (!def.isValid())
                return QString();
            if (def.typeId() == QMetaType::Bool)
                return def.toBool() ? QStringLiteral("yes") : QStringLiteral("no");
            return def.toString();
        });
    QString skippedNote;
    if (!changes.skipped.isEmpty()) {
        skippedNote = tr("Not written, because the form cannot write lists or nested blocks: %1. "
                         "Change them in the configuration file.").arg(changes.skipped.join(QStringLiteral(", ")));
    }
    if (changes.isEmpty()) {
        QMessageBox::information(this, title,
                                 skippedNote.isEmpty() ? tr("Nothing was changed.")
                                                       : tr("Nothing that can be written was changed. %1").arg(skippedNote));
        return true;
    }

    if (!m_persistence) {
        m_saveDialogs = new BResourceSaveDialogs(this);
        m_persistence = new BResourcePersistence(new BDirectorConfigClient(m_director, this), m_saveDialogs, this);
        connect(m_persistence, &BResourcePersistence::finished, this, &BResourceWidget::onWriteFinished);
        connect(m_persistence, &BResourcePersistence::reloadRequested, this,
                [this](BConfig::ResourceType rtype, const QString &) {
            if (!m_director || !m_director->isConnected())
                return;
            m_director->doSend(rtype == BConfig::ResourceType::JobDefs ? BDirector::Command::ShowJobDefs
                                                                        : BDirector::Command::ShowJobs);
        });
    }
    if (m_persistence->isBusy()) {
        QMessageBox::information(this, title, tr("Another change is still being written. Try again when it is done."));
        return true;
    }

    m_pendingEdit = after;
    m_persistence->read(type, name, this,
                        [this, type, name, title, changes, skippedNote](
                            const BConfig::Reply &reply, const std::optional<BResourcePersistence::Raw> &raw) {
        if (!raw) {
            const QString why = reply.ok()
                ? tr("The text the Director sent does not match its SHA-256.")
                : BResourcePersistence::reportOf(reply, BResourceReview::Update, type, name).text;
            QMessageBox::warning(this, title, tr("%1 Nothing was written.").arg(why));
            return;
        }
        QString newText;
        QString error;
        if (!BResourceText::edit(raw->text, changes.changes, &newText, &error)) {
            QMessageBox::warning(this, title,
                                 tr("The change cannot be written into the text of the resource safely: %1 "
                                    "Nothing was written.").arg(error));
            return;
        }
        BResourceReview review;
        review.changes = changes.descriptions;
        review.note = skippedNote;
        m_persistence->update(type, name, *raw, newText, review);
    });
    return true;
}

void BResourceWidget::onWriteFinished(const BResourceReport &report)
{
    if (report.kind != BResourceReport::Written || report.name != m_pendingEdit.name())
        return;
    // Show what was written; the Director's own state comes with the reload
    for (int i = 0; i < m_resources.size(); ++i) {
        if (m_resources[i].name() != report.name)
            continue;
        BConfigResource merged = m_resources[i];
        for (const QString &key : m_pendingEdit.keys())
            merged.setValue(key, m_pendingEdit.value(key));
        m_resources[i] = merged;
        QTreeWidgetItem *current = m_treeWidget->currentItem();
        if (current && current->data(0, Qt::UserRole).toInt() == i)
            updateDetails(merged);
        emit resourceModified(merged);
        break;
    }
}

void BResourceWidget::populateTree()
{
    m_treeWidget->clear();

    for (int i = 0; i < m_resources.size(); ++i) {
        const BConfigResource &res = m_resources.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, res.name());
        item->setText(1, QFileInfo(res.sourceFile()).fileName());
        item->setData(0, Qt::UserRole, i);
        item->setIcon(0, resourceIcon());
    }

    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));
    }
}

void BResourceWidget::updateDetails(const BConfigResource &resource)
{
    QString text;
    text += QString("<h3>%1: %2</h3>").arg(resource.type(), resource.name());
    text += QString("<p><i>Source: %1</i></p>").arg(resource.sourceFile());
    text += "<hr>";
    text += "<pre>";

    for (const QString &key : resource.keys()) {
        BConfigValue value = resource.value(key);
        text += QString("<b>%1</b> = %2\n").arg(key, formatValue(value));
    }

    text += "</pre>";
    m_detailsEdit->setHtml(text);
}

void BResourceWidget::updateConfigPreview(const BConfigResource &resource)
{
    m_detailsEdit->setPlainText(resourceToConf(resource));
}

QString BResourceWidget::formatValue(const BConfigValue &value, int indent) const
{
    QString indentStr(indent * 2, ' ');

    switch (value.type()) {
    case BConfigValue::Simple:
        return value.simpleValue();

    case BConfigValue::List: {
        QStringList items = value.listValue();
        if (items.size() == 1) {
            return items.first();
        }
        QString result = "\n";
        for (const QString &item : items) {
            result += QString("%1  - %2\n").arg(indentStr, item);
        }
        return result;
    }

    case BConfigValue::Block: {
        QString result = "{\n";
        QMap<QString, BConfigValue> block = value.blockValue();
        for (auto it = block.begin(); it != block.end(); ++it) {
            result += QString("%1  %2 = %3\n")
                          .arg(indentStr, it.key(), formatValue(it.value(), indent + 1));
        }
        result += indentStr + "}";
        return result;
    }

    case BConfigValue::BlockList: {
        QString result;
        for (const auto &block : value.blockListValue()) {
            result += "{\n";
            for (auto it = block.begin(); it != block.end(); ++it) {
                result += QString("%1  %2 = %3\n")
                              .arg(indentStr, it.key(), formatValue(it.value(), indent + 1));
            }
            result += indentStr + "}\n";
        }
        return result;
    }
    }
    return QString();
}

QIcon BResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("folder");
}

// ============================================================================
// Export methods
// ============================================================================

static QString daemonDirForType(const QString &resourceType)
{
    QString t = resourceType.toLower();

    // File Daemon types
    if (t == "filedaemon")
        return QStringLiteral("bareos-fd.d");

    // Storage Daemon types
    if (t == "device" || t == "autochanger" || t == "ndmp")
        return QStringLiteral("bareos-sd.d");

    // Director types (default)
    return QStringLiteral("bareos-dir.d");
}

static QString confSubdirForType(const QString &resourceType)
{
    QString t = resourceType.toLower();

    // FileDaemon resources go into the "client" subdirectory
    if (t == "filedaemon")
        return QStringLiteral("client");

    return t;
}

static QString confPathForResource(const BConfigResource &resource)
{
    // If an explicit source path is provided, use it directly
    if (!resource.sourceFile().isEmpty())
        return resource.sourceFile();

    return QString("etc/bareos/%1/%2/%3.conf")
        .arg(daemonDirForType(resource.type()),
             confSubdirForType(resource.type()),
             resource.name());
}

/**
 * @brief Capitalize a Bareos directive key to proper Title Case
 *
 * Handles common acronyms: TLS, FD, SD, CA, ACL, NDC, IP.
 * Example: "tls enable" → "TLS Enable", "fd port" → "FD Port"
 */
static QString capitalizeDirective(const QString &key)
{
    static const QSet<QString> acronyms = {
        "tls", "fd", "sd", "ca", "acl", "ndc", "ip", "pki"
    };

    QStringList words = key.split(' ', Qt::SkipEmptyParts);
    for (QString &word : words) {
        if (acronyms.contains(word.toLower())) {
            word = word.toUpper();
        } else {
            word[0] = word[0].toUpper();
        }
    }
    return words.join(' ');
}

/// Why a schedule cannot be written: one of its Run lines is not understood
static QString scheduleRunProblem(const BConfigResource &resource)
{
    if (resource.type().compare("Schedule", Qt::CaseInsensitive) != 0)
        return QString();
    for (const QString &key : resource.keys()) {
        if (key.compare("run", Qt::CaseInsensitive) != 0)
            continue;
        const BConfigValue val = resource.value(key);
        QStringList reported;
        if (val.type() == BConfigValue::Simple)
            reported.append(val.simpleValue());
        else if (val.type() == BConfigValue::List)
            reported = val.listValue();
        QString error;
        if (!BRunWriter::runTexts(reported, nullptr, &error))
            return error;
    }
    return QString();
}

QString BResourceWidget::resourceToConf(const BConfigResource &resource) const
{
    QString conf;

    // A schedule with a Run line that is not understood is not written; the
    // file says so in its place
    const QString runProblem = scheduleRunProblem(resource);
    if (!runProblem.isEmpty()) {
        conf += "# " + tr("Schedule \"%1\" is not written here.").arg(resource.name()) + "\n";
        conf += "# " + runProblem + "\n";
        return conf;
    }
    const bool isSchedule = (resource.type().compare("Schedule", Qt::CaseInsensitive) == 0);

    // Section banner: ################ CLIENT ################
    QString label = QString(" %1 ").arg(resource.type().toUpper());
    int pad = qMax(0, (50 - label.length()) / 2);
    conf += QString("#").repeated(pad) + label + QString("#").repeated(50 - pad - label.length()) + "\n";

    conf += QString("%1 {\n").arg(resource.type());
    conf += QString("  Name = \"%1\"\n").arg(resource.name());

    // Bareos quoting rules (docs.bareos.org):
    // - Numbers must NEVER be quoted, even with units (e.g. "365 days")
    // - Time periods must NOT be quoted
    // - Boolean yes/no must NOT be quoted
    // - [md5] prefixed passwords must NOT be quoted
    static const QRegularExpression timePeriodRx(
        R"(^\d+\s*(?:seconds?|secs?|s|minutes?|mins?|hours?|days?|weeks?|months?|quarters?|years?)(?:\s+\d+\s*(?:seconds?|secs?|s|minutes?|mins?|hours?|days?|weeks?|months?|quarters?|years?))*$)",
        QRegularExpression::CaseInsensitiveOption);

    // Recursive helper to format a BConfigValue at a given indent level
    std::function<void(const QString &, const BConfigValue &, int)> formatValue;
    formatValue = [&](const QString &key, const BConfigValue &val, int indent) {
        QString pad = QString("  ").repeated(indent);
        QString directive = capitalizeDirective(key);

        // Run lines of a schedule are not strings: Bareos does not read a
        // quoted Run line. Understood (checked above), they go out as they are.
        if (isSchedule && indent == 1 && key.compare("run", Qt::CaseInsensitive) == 0
            && (val.type() == BConfigValue::Simple || val.type() == BConfigValue::List)) {
            const QStringList reported = (val.type() == BConfigValue::Simple)
                ? QStringList(val.simpleValue()) : val.listValue();
            QStringList runs;
            BRunWriter::runTexts(reported, &runs);
            for (const QString &run : runs)
                conf += QString("%1%2 = %3\n").arg(pad, directive, run);
            return;
        }

        switch (val.type()) {
        case BConfigValue::Simple: {
            QString v = val.simpleValue();
            bool isNumeric = false;
            v.toLongLong(&isNumeric);
            bool isTimePeriod = timePeriodRx.match(v).hasMatch();

            if (v.startsWith('"') || isNumeric || v == "yes" || v == "no"
                || v.contains('=') || isTimePeriod || v.startsWith("[md5]"))
                conf += QString("%1%2 = %3\n").arg(pad, directive, v);
            else
                conf += QString("%1%2 = \"%3\"\n").arg(pad, directive, v);
            break;
        }
        case BConfigValue::List:
            for (const QString &item : val.listValue())
                conf += QString("%1%2 = \"%3\"\n").arg(pad, directive, item);
            break;
        case BConfigValue::Block: {
            conf += QString("%1%2 {\n").arg(pad, directive);
            QMap<QString, BConfigValue> block = val.blockValue();
            for (auto it = block.begin(); it != block.end(); ++it)
                formatValue(it.key(), it.value(), indent + 1);
            conf += QString("%1}\n").arg(pad);
            break;
        }
        case BConfigValue::BlockList:
            for (const auto &block : val.blockListValue()) {
                conf += QString("%1%2 {\n").arg(pad, directive);
                for (auto it = block.begin(); it != block.end(); ++it)
                    formatValue(it.key(), it.value(), indent + 1);
                conf += QString("%1}\n").arg(pad);
            }
            break;
        }
    };

    for (const QString &key : resource.keys()) {
        if (key == "name") continue;  // Already emitted above
        formatValue(key, resource.value(key), 1);
    }

    conf += "}\n";
    return conf;
}

void BResourceWidget::exportToConf()
{
    const QList<BConfigResource> &exportRes =
        m_exportResources.isEmpty() ? m_resources : m_exportResources;
    if (exportRes.isEmpty()) return;

    QString baseName = m_exportName.isEmpty()
        ? (exportRes.size() == 1 ? exportRes.first().name() : m_resourceType)
        : m_exportName;
    QString defaultName = QString("%1.conf").arg(baseName);

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Export %1 Configuration").arg(m_resourceType), defaultName,
        tr("Configuration Files (*.conf);;All Files (*)"));
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Export Failed"),
                             tr("Could not write to %1").arg(filePath));
        return;
    }

    // Group resources by daemon side (bareos-fd.d vs bareos-dir.d)
    QString currentGroup;
    for (const BConfigResource &res : exportRes) {
        QString group;
        QString src = res.sourceFile();
        if (src.contains("bareos-fd.d"))
            group = "FILE DAEMON (Client Side)";
        else if (src.contains("bareos-dir.d"))
            group = "DIRECTOR (Server Side)";
        else if (src.contains("bareos-sd.d"))
            group = "STORAGE DAEMON";

        if (!group.isEmpty() && group != currentGroup) {
            if (!currentGroup.isEmpty())
                file.write("\n");
            QString banner = QString("# %1\n# %2\n\n")
                .arg(QString("=").repeated(50), group);
            file.write(banner.toUtf8());
            currentGroup = group;
        }

        file.write(resourceToConf(res).toUtf8());
        file.write("\n");
    }
    file.close();
}

void BResourceWidget::exportToZip()
{
    const QList<BConfigResource> &exportRes =
        m_exportResources.isEmpty() ? m_resources : m_exportResources;
    if (exportRes.isEmpty()) return;

    QString baseName = m_exportName.isEmpty()
        ? (exportRes.size() == 1
            ? QString("%1-%2").arg(exportRes.first().type().toLower(), exportRes.first().name())
            : m_resourceType.toLower())
        : m_exportName;
    QString defaultName = QString("%1.zip").arg(baseName);

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Export %1 Configuration as ZIP").arg(m_resourceType), defaultName,
        tr("ZIP Archives (*.zip);;All Files (*)"));
    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".zip", Qt::CaseInsensitive))
        filePath += ".zip";

    QZipWriter zip(filePath);
    if (zip.status() != QZipWriter::NoError) {
        QMessageBox::warning(this, tr("Export Failed"),
                             tr("Could not create ZIP file: %1").arg(filePath));
        return;
    }

    // Collect all conf paths and build directory hierarchy
    QSet<QString> directories;
    for (const BConfigResource &res : exportRes) {
        QString confPath = confPathForResource(res);
        QString dir = QFileInfo(confPath).path();
        while (!dir.isEmpty() && dir != ".") {
            directories.insert(dir);
            dir = QFileInfo(dir).path();
        }
    }

    // Create directory hierarchy
    QStringList sortedDirs = directories.values();
    sortedDirs.sort();
    for (const QString &d : sortedDirs)
        zip.addDirectory(d);

    // Add all config files
    for (const BConfigResource &res : exportRes) {
        zip.addFile(confPathForResource(res), resourceToConf(res).toUtf8());
    }

    // Collect and bundle TLS certificate files referenced in resources
    // Scan for TLS file directives, read from local filesystem, add to ZIP
    static const QStringList tlsFileKeys = {
        "tls ca certificate file", "tls certificate", "tls key"
    };
    QSet<QString> addedTlsFiles;
    QStringList tlsFileNotes;
    for (const BConfigResource &res : exportRes) {
        for (const QString &tlsKey : tlsFileKeys) {
            BConfigValue val = res.value(tlsKey);
            if (val.type() != BConfigValue::Simple)
                continue;
            QString absPath = val.simpleValue();
            if (absPath.isEmpty() || !absPath.startsWith('/'))
                continue;

            // Derive ZIP-relative path: /etc/bareos/tls/ca.pem → etc/bareos/tls/ca.pem
            QString zipPath = absPath.startsWith('/') ? absPath.mid(1) : absPath;

            if (addedTlsFiles.contains(zipPath))
                continue;

            QFile tlsFile(absPath);
            if (tlsFile.open(QIODevice::ReadOnly)) {
                // Ensure parent directories exist in ZIP
                QString dir = QFileInfo(zipPath).path();
                while (!dir.isEmpty() && dir != "." && !directories.contains(dir)) {
                    directories.insert(dir);
                    zip.addDirectory(dir);
                    dir = QFileInfo(dir).path();
                }
                zip.addFile(zipPath, tlsFile.readAll());
                tlsFile.close();
                addedTlsFiles.insert(zipPath);
                tlsFileNotes += QString("- `%1` (from %2)").arg(zipPath, absPath);
            }
        }
    }

    // Generate README.md
    QString readme;
    readme += QString("# %1 Configuration Export\n\n").arg(baseName);
    readme += QString("Exported: %1\n").arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    readme += QString("Generator: Onesimus %1\n\n").arg(VERSION_FULL);
    readme += "## Configuration Files\n\n";
    for (const BConfigResource &res : exportRes) {
        readme += QString("- `%1` (%2: %3)\n")
            .arg(confPathForResource(res), res.type(), res.name());
    }
    if (!addedTlsFiles.isEmpty()) {
        readme += "\n## TLS Certificate Files\n\n";
        for (const QString &note : tlsFileNotes)
            readme += note + "\n";
        readme += "\n**Note:** Client certificate and key must be generated for this host.\n";
        readme += "Use the same CA to sign both Director and FileDaemon certificates.\n";
    }
    readme += "\n## Deployment\n\n";
    readme += "Copy the `etc/` directory tree to the target machine's root (`/`):\n\n";
    readme += "```\nsudo cp -r etc/ /\nsudo chown -R bareos:bareos /etc/bareos/\nsudo systemctl restart bareos-fd\n```\n";
    readme += "\n**Note:** Review and update passwords marked `CHANGE_ME` before deployment.\n";
    zip.addFile("README.md", readme.toUtf8());

    zip.close();
}
