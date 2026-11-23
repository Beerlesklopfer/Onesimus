// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bfilesetwizard.cpp
 * @brief Unified wizard for creating and editing FileSet resources
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "jobs/bfilesetwizard.h"
#include "bstatuschip.h"
#include "blabelstate.h"
#include "buiscale.h"
#include "jobs/fileset/bfilesetdocument.h"
#include "bcapabilitynotice.h"
#include "blogging.h"
#include "bquote.h"
#include "config/bresourcepersistence.h"
#include "config/bresourcereviewdialog.h"
#include "director/bdirectorconfigclient.h"
#include "jobs/fileset/bincludeblockwidget.h"
#include "config/beditablelistwidget.h"
#include "config/beditablelistmodel.h"
#include "config/bconfigparser.h"
#include "director/bdirector.h"
#include "director/bresourcewidget.h"
#include "models/bresourcemodels.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QScrollArea>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>

// Static template storage
QJsonObject BFileSetWizard::s_templates;

// ============================================================================
// BFileSetWizard
// ============================================================================

BFileSetWizard::BFileSetWizard(BDirector *director, BFilesetModel *filesetModel,
                               QWidget *parent)
    : QWizard(parent)
    , m_mode(NewMode)
    , m_director(director)
    , m_filesetModel(filesetModel)
    , m_document(new BFileSetDocument(this))
{
    init();
}

BFileSetWizard::BFileSetWizard(BDirector *director, const QString &filesetName,
                               BFilesetModel *filesetModel, QWidget *parent)
    : QWizard(parent)
    , m_mode(EditMode)
    , m_director(director)
    , m_filesetModel(filesetModel)
    , m_document(new BFileSetDocument(this))
    , m_originalName(filesetName)
{
    init();

    // Load existing FileSet from Director
    // The document will be populated when settings page initializes
    m_document->setName(filesetName);
}

BFileSetWizard::~BFileSetWizard()
{
    delete m_saveDialogs;
}

bool BFileSetWizard::canUseConfigMethod(BConfig::Operation operation) const
{
    return m_director && m_director->isConnected()
        && m_director->transport() == BDirector::Transport::Grpc
        && m_director->configCapabilities().supports(BConfig::ResourceType::FileSet, operation);
}

BResourcePersistence *BFileSetWizard::persistence()
{
    if (!m_persistence && m_director) {
        m_saveDialogs = new BResourceSaveDialogs(this);
        m_persistence = new BResourcePersistence(new BDirectorConfigClient(m_director, this), m_saveDialogs, this);
    }
    return m_persistence;
}

void BFileSetWizard::init()
{
    setWindowTitle(m_mode == NewMode ? tr("New FileSet Wizard") : tr("Edit FileSet"));
    setWizardStyle(QWizard::ModernStyle);
    BUiScale::track(this, [this]() { setMinimumSize(BUiScale::px(800), BUiScale::px(700)); });

    // Load templates if not already loaded
    if (s_templates.isEmpty()) {
        s_templates = loadTemplates();
    }

    // Add pages based on mode
    if (m_mode == NewMode) {
        setPage(Page_Template, new BFileSetTemplatePage());
    }
    setPage(Page_Settings, new BFileSetSettingsPage());
    setPage(Page_Include, new BFileSetIncludePage());
    setPage(Page_Exclude, new BFileSetExcludePage());
    setPage(Page_Preview, new BFileSetPreviewPage());

    // Start page depends on mode
    if (m_mode == EditMode) {
        setStartId(Page_Settings);
    }

    setButtonText(QWizard::FinishButton, tr("Finish"));
}

QJsonObject BFileSetWizard::loadTemplates()
{
    QJsonObject templates;

    // List of template files to load
    QStringList templateFiles = {
        ":/templates/filesets/templates/filesets/empty_custom.json",
        ":/templates/filesets/templates/filesets/linux_full_system.json",
        ":/templates/filesets/templates/filesets/linux_home.json",
        ":/templates/filesets/templates/filesets/linux_etc.json",
        ":/templates/filesets/templates/filesets/linux_docker.json",
        ":/templates/filesets/templates/filesets/linux_nextcloud.json",
        ":/templates/filesets/templates/filesets/linux_gitlab.json",
        ":/templates/filesets/templates/filesets/linux_webserver.json",
        ":/templates/filesets/templates/filesets/linux_mail.json",
        ":/templates/filesets/templates/filesets/linux_samba.json",
        ":/templates/filesets/templates/filesets/linux_proxmox.json",
        ":/templates/filesets/templates/filesets/linux_kubernetes.json",
        ":/templates/filesets/templates/filesets/linux_mysql.json",
        ":/templates/filesets/templates/filesets/linux_postgresql.json",
        ":/templates/filesets/templates/filesets/linux_mongodb.json",
        ":/templates/filesets/templates/filesets/windows_user_data.json",
        ":/templates/filesets/templates/filesets/windows_domain_controller.json",
        ":/templates/filesets/templates/filesets/windows_file_server.json",
        ":/templates/filesets/templates/filesets/windows_sql_server.json",
        ":/templates/filesets/templates/filesets/windows_web_server_iis.json",
        ":/templates/filesets/templates/filesets/macos_user.json"
    };

    for (const QString &path : templateFiles) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            continue;
        }

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error != QJsonParseError::NoError) {
            BLOG_WARNING() << "Failed to parse template" << path << ":" << error.errorString();
            continue;
        }

        QJsonObject templateObj = doc.object();
        QString name = templateObj["name"].toString();
        if (name.isEmpty()) continue;

        templates[name] = templateObj;
    }

    return templates;
}

// ============================================================================
// Page 1: Template Selection
// ============================================================================

BFileSetTemplatePage::BFileSetTemplatePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Select FileSet Template"));
    setSubTitle(tr("Choose a preset template to start with, or mix multiple templates together."));

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Template selection with Mix button
    QHBoxLayout *templateLayout = new QHBoxLayout();
    QFormLayout *formLayout = new QFormLayout();

    m_templateCombo = new QComboBox(this);
    formLayout->addRow(tr("Template:"), m_templateCombo);
    templateLayout->addLayout(formLayout, 1);

    m_mixButton = new QPushButton(tr("Mix Templates..."), this);
    m_mixButton->setIcon(QIcon::fromTheme("list-add"));
    m_mixButton->setToolTip(tr("Combine paths from multiple templates"));
    templateLayout->addWidget(m_mixButton);

    layout->addLayout(templateLayout);

    // Mixed templates indicator
    m_mixedLabel = new QLabel(this);
    m_mixedLabel->setStyleSheet("font-weight: bold;");
    bSetLabelState(m_mixedLabel, "");
    m_mixedLabel->setVisible(false);
    layout->addWidget(m_mixedLabel);

    // Description group
    QGroupBox *descGroup = new QGroupBox(tr("Template Description"));
    QVBoxLayout *descLayout = new QVBoxLayout(descGroup);

    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setWordWrap(true);
    descLayout->addWidget(m_descriptionLabel);

    m_platformLabel = new QLabel(this);
    m_platformLabel->setStyleSheet("font-style: italic;");
    bSetLabelState(m_platformLabel, "muted");
    descLayout->addWidget(m_platformLabel);

    layout->addWidget(descGroup);

    // Preview group with scroll areas
    QGroupBox *previewGroup = new QGroupBox(tr("Paths Preview"));
    QHBoxLayout *previewMainLayout = new QHBoxLayout(previewGroup);

    // Include preview with scroll
    QVBoxLayout *includeLayout = new QVBoxLayout();
    includeLayout->addWidget(new QLabel(tr("<b>Include:</b>")));
    QScrollArea *includeScroll = new QScrollArea(this);
    includeScroll->setWidgetResizable(true);
    BUiScale::track(includeScroll, [includeScroll]() { includeScroll->setMinimumHeight(BUiScale::px(100)); });
    m_includePreview = new QLabel(this);
    m_includePreview->setWordWrap(true);
    m_includePreview->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    includeScroll->setWidget(m_includePreview);
    includeLayout->addWidget(includeScroll);
    previewMainLayout->addLayout(includeLayout);

    // Exclude preview with scroll
    QVBoxLayout *excludeLayout = new QVBoxLayout();
    excludeLayout->addWidget(new QLabel(tr("<b>Exclude:</b>")));
    QScrollArea *excludeScroll = new QScrollArea(this);
    excludeScroll->setWidgetResizable(true);
    BUiScale::track(excludeScroll, [excludeScroll]() { excludeScroll->setMinimumHeight(BUiScale::px(100)); });
    m_excludePreview = new QLabel(this);
    m_excludePreview->setWordWrap(true);
    m_excludePreview->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    excludeScroll->setWidget(m_excludePreview);
    excludeLayout->addWidget(excludeScroll);
    previewMainLayout->addLayout(excludeLayout);

    layout->addWidget(previewGroup);
    layout->addStretch();

    connect(m_templateCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BFileSetTemplatePage::onTemplateChanged);
    connect(m_mixButton, &QPushButton::clicked, this, &BFileSetTemplatePage::onMixTemplates);
}

void BFileSetTemplatePage::initializePage()
{
    m_templateCombo->clear();

    QJsonObject templates = BFileSetWizard::loadTemplates();
    for (auto it = templates.begin(); it != templates.end(); ++it) {
        m_templateCombo->addItem(it.key(), it.key());
    }

    // Select "Empty (Custom)" by default
    int customIndex = m_templateCombo->findText("Empty (Custom)");
    if (customIndex >= 0) {
        m_templateCombo->setCurrentIndex(customIndex);
    } else if (m_templateCombo->count() > 0) {
        m_templateCombo->setCurrentIndex(0);
    }

    onTemplateChanged(m_templateCombo->currentIndex());
}

bool BFileSetTemplatePage::isComplete() const
{
    return m_templateCombo->currentIndex() >= 0;
}

void BFileSetTemplatePage::onTemplateChanged(int index)
{
    if (index < 0) return;

    QString templateName = m_templateCombo->currentData().toString();
    loadTemplateIntoDocument(templateName);

    // Clear mixed templates when selecting a single template
    m_mixedTemplates.clear();
    m_mixedLabel->setVisible(false);

    updatePreview();
    emit completeChanged();
}

void BFileSetTemplatePage::loadTemplateIntoDocument(const QString &templateName)
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    QJsonObject templates = BFileSetWizard::loadTemplates();
    QJsonObject templateObj = templates[templateName].toObject();
    BFileSetDocument *doc = wiz->document();

    // The FileSet part of the template, with its own Name ("LinuxHome");
    // the template's title is only the fallback
    doc->loadFromTemplate(templateObj["fileset"].toObject());
    if (doc->name().isEmpty())
        doc->setName(templateObj["name"].toString().remove(QLatin1Char(' ')));
    if (doc->description().isEmpty())
        doc->setDescription(templateObj["description"].toString());

    doc->markClean();

    // Update description labels
    m_descriptionLabel->setText(templateObj["description"].toString());

    QString platform = templateObj["platform"].toString();
    if (platform == "windows") {
        m_platformLabel->setText(tr("Platform: Windows"));
    } else if (platform == "linux") {
        m_platformLabel->setText(tr("Platform: Linux/Unix"));
    } else if (platform == "macos") {
        m_platformLabel->setText(tr("Platform: macOS"));
    } else {
        m_platformLabel->setText(tr("Platform: Any"));
    }
}

void BFileSetTemplatePage::onMixTemplates()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    QJsonObject templates = BFileSetWizard::loadTemplates();

    // Create a dialog with checkable list
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Mix Templates"));
    BUiScale::track(&dialog, [&dialog]() { dialog.setMinimumSize(BUiScale::px(500), BUiScale::px(600)); });

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel(tr("Select templates to combine:"), &dialog);
    layout->addWidget(label);

    // Splitter for list and preview
    QSplitter *splitter = new QSplitter(Qt::Vertical, &dialog);

    QListWidget *listWidget = new QListWidget(&dialog);
    listWidget->setSelectionMode(QAbstractItemView::NoSelection);

    for (auto it = templates.begin(); it != templates.end(); ++it) {
        QListWidgetItem *item = new QListWidgetItem(it.key());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(m_mixedTemplates.contains(it.key()) ? Qt::Checked : Qt::Unchecked);
        item->setData(Qt::UserRole, it.key());

        QJsonObject tmpl = it.value().toObject();
        QString platform = tmpl["platform"].toString();
        QString desc = tmpl["description"].toString();
        item->setToolTip(tr("%1\nPlatform: %2").arg(desc, platform));

        listWidget->addItem(item);
    }
    splitter->addWidget(listWidget);

    // Preview of combined paths
    QGroupBox *previewGroup = new QGroupBox(tr("Combined Paths Preview"), &dialog);
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);
    QTextEdit *previewEdit = new QTextEdit(&dialog);
    previewEdit->setReadOnly(true);
    previewLayout->addWidget(previewEdit);
    splitter->addWidget(previewGroup);

    splitter->setSizes({300, 250});
    layout->addWidget(splitter);

    // Update preview when checkboxes change
    auto updateMixPreview = [&]() {
        QStringList includePaths, excludePaths;
        for (int i = 0; i < listWidget->count(); ++i) {
            QListWidgetItem *item = listWidget->item(i);
            if (item->checkState() == Qt::Checked) {
                QString name = item->data(Qt::UserRole).toString();
                QJsonObject tmpl = templates[name].toObject();
                QJsonObject fileset = tmpl["fileset"].toObject();
                QJsonObject include = fileset["Include"].toObject();

                // Include paths
                QJsonValue fileVal = include["File"];
                if (fileVal.isArray()) {
                    for (const QJsonValue &v : fileVal.toArray()) {
                        if (!includePaths.contains(v.toString()))
                            includePaths.append(v.toString());
                    }
                } else if (fileVal.isString()) {
                    if (!includePaths.contains(fileVal.toString()))
                        includePaths.append(fileVal.toString());
                }

                // Exclude paths
                QJsonObject exclude = fileset["Exclude"].toObject();
                QJsonValue exFileVal = exclude["File"];
                if (exFileVal.isArray()) {
                    for (const QJsonValue &v : exFileVal.toArray()) {
                        if (!excludePaths.contains(v.toString()))
                            excludePaths.append(v.toString());
                    }
                } else if (exFileVal.isString()) {
                    if (!excludePaths.contains(exFileVal.toString()))
                        excludePaths.append(exFileVal.toString());
                }
            }
        }
        QString preview = QString("<b>Include (%1):</b><br>%2<br><br><b>Exclude (%3):</b><br>%4")
            .arg(includePaths.count())
            .arg(includePaths.isEmpty() ? tr("(none)") : includePaths.join("<br>"))
            .arg(excludePaths.count())
            .arg(excludePaths.isEmpty() ? tr("(none)") : excludePaths.join("<br>"));
        previewEdit->setHtml(preview);
    };

    connect(listWidget, &QListWidget::itemChanged, [&](QListWidgetItem *) {
        updateMixPreview();
    });
    updateMixPreview();

    // Buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);

    if (dialog.exec() != QDialog::Accepted) return;

    // Collect selected templates and merge into document
    m_mixedTemplates.clear();
    BFileSetDocument *doc = wiz->document();

    // Clear existing include blocks
    while (doc->includeBlockCount() > 0) {
        doc->removeIncludeBlock(0);
    }

    // Create a single merged include block
    BFileSetDocument::IncludeBlock *mergedBlock = doc->addIncludeBlock();
    QStringList excludePaths;

    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem *item = listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            QString name = item->data(Qt::UserRole).toString();
            m_mixedTemplates.append(name);

            QJsonObject tmpl = templates[name].toObject();
            QJsonObject fileset = tmpl["fileset"].toObject();
            QJsonObject include = fileset["Include"].toObject();

            // Add include paths
            QJsonValue fileVal = include["File"];
            if (fileVal.isArray()) {
                for (const QJsonValue &v : fileVal.toArray()) {
                    QString path = v.toString();
                    if (!mergedBlock->pathsModel->items().contains(path)) {
                        mergedBlock->pathsModel->addItem(path);
                    }
                }
            } else if (fileVal.isString()) {
                QString path = fileVal.toString();
                if (!mergedBlock->pathsModel->items().contains(path)) {
                    mergedBlock->pathsModel->addItem(path);
                }
            }

            // Collect exclude paths
            QJsonObject exclude = fileset["Exclude"].toObject();
            QJsonValue exFileVal = exclude["File"];
            if (exFileVal.isArray()) {
                for (const QJsonValue &v : exFileVal.toArray()) {
                    if (!excludePaths.contains(v.toString()))
                        excludePaths.append(v.toString());
                }
            } else if (exFileVal.isString()) {
                if (!excludePaths.contains(exFileVal.toString()))
                    excludePaths.append(exFileVal.toString());
            }
        }
    }

    // Set global excludes
    doc->excludePathModel()->setItems(excludePaths);

    // Update wizard data
    if (!m_mixedTemplates.isEmpty()) {
        doc->setName(tr("Mixed_%1").arg(m_mixedTemplates.count()));
        doc->setDescription(tr("Combined FileSet from %1 templates").arg(m_mixedTemplates.count()));

        m_mixedLabel->setText(tr("Mixed: %1 templates selected").arg(m_mixedTemplates.count()));
        m_mixedLabel->setVisible(true);

        m_descriptionLabel->setText(doc->description());
        m_platformLabel->setText(tr("Templates: %1").arg(m_mixedTemplates.join(", ")));
    } else {
        m_mixedLabel->setVisible(false);
    }

    updatePreview();
    emit completeChanged();
}

void BFileSetTemplatePage::updatePreview()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    BFileSetDocument *doc = wiz->document();

    // Collect all include paths
    QStringList includePaths;
    for (int i = 0; i < doc->includeBlockCount(); ++i) {
        BFileSetDocument::IncludeBlock *block = doc->includeBlock(i);
        includePaths.append(block->pathsModel->items());
    }

    m_includePreview->setText(includePaths.isEmpty()
        ? tr("(none)")
        : includePaths.join("\n"));

    m_excludePreview->setText(doc->excludePathModel()->items().isEmpty()
        ? tr("(none)")
        : doc->excludePathModel()->items().join("\n"));
}

// ============================================================================
// Page 2: FileSet Settings
// ============================================================================

BFileSetSettingsPage::BFileSetSettingsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("FileSet Settings"));
    setSubTitle(tr("Configure the FileSet name and basic options."));

    setupUi();
}

void BFileSetSettingsPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // FileSet selection (Edit mode only - hidden in New mode)
    m_filesetSelectWidget = new QWidget(this);
    QHBoxLayout *selectLayout = new QHBoxLayout(m_filesetSelectWidget);
    selectLayout->setContentsMargins(0, 0, 0, 8);

    QLabel *selectLabel = new QLabel(tr("Select FileSet:"), this);
    m_filesetCombo = new QComboBox(this);
    BUiScale::track(m_filesetCombo, [this]() { m_filesetCombo->setMinimumWidth(BUiScale::px(250)); });
    m_filesetCombo->setPlaceholderText(tr("Loading FileSets..."));

    m_loadingLabel = new QLabel(this);
    m_loadingLabel->setStyleSheet("font-style: italic;");
    bSetLabelState(m_loadingLabel, "muted");

    selectLayout->addWidget(selectLabel);
    selectLayout->addWidget(m_filesetCombo, 1);
    selectLayout->addWidget(m_loadingLabel);
    selectLayout->addStretch();

    m_filesetSelectWidget->setVisible(false);  // Hidden by default, shown in Edit mode
    mainLayout->addWidget(m_filesetSelectWidget);

    connect(m_filesetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BFileSetSettingsPage::onFileSetSelected);

    // Basic settings
    QGroupBox *basicGroup = new QGroupBox(tr("Basic Settings"));
    QFormLayout *basicForm = new QFormLayout(basicGroup);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText(tr("e.g., LinuxFullSystem"));
    basicForm->addRow(tr("Name:"), m_nameEdit);

    m_descriptionEdit = new QLineEdit(this);
    m_descriptionEdit->setPlaceholderText(tr("Optional description"));
    basicForm->addRow(tr("Description:"), m_descriptionEdit);

    mainLayout->addWidget(basicGroup);

    // Global FileSet options
    QGroupBox *globalGroup = new QGroupBox(tr("Global Options"));
    QHBoxLayout *globalLayout = new QHBoxLayout(globalGroup);

    m_enableVssCheck = new QCheckBox(tr("Enable VSS"), this);
    m_enableVssCheck->setToolTip(tr("Enable Volume Shadow Copy Service (Windows)"));
    globalLayout->addWidget(m_enableVssCheck);

    m_ignoreChangesCheck = new QCheckBox(tr("Ignore FileSet Changes"), this);
    m_ignoreChangesCheck->setToolTip(tr("Don't trigger Full backup when FileSet changes"));
    globalLayout->addWidget(m_ignoreChangesCheck);

    globalLayout->addStretch();
    mainLayout->addWidget(globalGroup);

    mainLayout->addStretch();

    connect(m_nameEdit, &QLineEdit::textChanged, this, &QWizardPage::completeChanged);
}

void BFileSetSettingsPage::initializePage()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    BFileSetDocument *doc = wiz->document();

    if (!m_initialized) {
        // Show FileSet selection in Edit mode
        if (wiz->mode() == BFileSetWizard::EditMode) {
            m_filesetSelectWidget->setVisible(true);
            m_loadingLabel->setText(tr("Loading..."));
            fetchFileSets();
        }

        m_initialized = true;
    }

    // Always update from document (template may have changed). The name as
    // it is: blanks are allowed, and removing them in edit mode would make
    // a new FileSet
    m_nameEdit->setText(doc->name());
    m_descriptionEdit->setText(doc->description());
    m_enableVssCheck->setChecked(doc->enableVss());
    m_ignoreChangesCheck->setChecked(doc->ignoreFileSetChanges());
}

void BFileSetSettingsPage::cleanupPage()
{
    collectFormValues();
}

bool BFileSetSettingsPage::validatePage()
{
    collectFormValues();
    return true;
}

bool BFileSetSettingsPage::isComplete() const
{
    return BQuote::isValidName(m_nameEdit->text().trimmed());
}

void BFileSetSettingsPage::collectFormValues()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    BFileSetDocument *doc = wiz->document();

    doc->setName(m_nameEdit->text().trimmed());
    doc->setDescription(m_descriptionEdit->text().trimmed());
    doc->setEnableVss(m_enableVssCheck->isChecked());
    doc->setIgnoreFileSetChanges(m_ignoreChangesCheck->isChecked());
}

void BFileSetSettingsPage::fetchFileSets()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    // Model-driven: use already-populated BFilesetModel if available
    BFilesetModel *model = wiz->filesetModel();
    if (model && model->rowCount() > 0) {
        onFileSetsLoaded(model->filesetNames());
    } else if (wiz->director()) {
        // Fetch fileset names from Director
        m_loadingLabel->setText(tr("Loading..."));
        auto conn = std::make_shared<QMetaObject::Connection>();
        *conn = connect(wiz->director(), &BDirector::jsonResult,
                this, [this, conn](BDirector::Command cmd, const QString &jsonData) {
            if (cmd != BDirector::Command::DotFilesets) return;

            QObject::disconnect(*conn);

            QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
            QJsonArray filesets = doc.object()["result"].toObject()["filesets"].toArray();
            QStringList names;
            for (const QJsonValue &fs : filesets) {
                QString name = fs.toObject()["name"].toString();
                if (!name.isEmpty()) names.append(name);
            }
            onFileSetsLoaded(names);
        });

        wiz->director()->doSend(BDirector::Command::DotFilesets);
    } else {
        m_loadingLabel->setText(tr("No Director connection"));
        return;
    }

    // Also fetch full fileset configs for model-based editing
    if (model && !model->hasFilesetConfigs() && wiz->director()) {
        requestShowFilesets();
    }
}

void BFileSetSettingsPage::requestShowFilesets()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz || !wiz->director() || !wiz->filesetModel()) return;

    auto jsonConn = std::make_shared<QMetaObject::Connection>();
    auto textConn = std::make_shared<QMetaObject::Connection>();

    *jsonConn = connect(wiz->director(), &BDirector::jsonResult,
            this, [this, jsonConn, textConn](BDirector::Command cmd, const QString &jsonData) {
        if (cmd != BDirector::Command::ShowFilesets) return;

        QObject::disconnect(*jsonConn);
        QObject::disconnect(*textConn);

        BFileSetWizard *w = qobject_cast<BFileSetWizard*>(wizard());
        if (w && w->filesetModel()) {
            w->filesetModel()->parseShowFilesets(jsonData);
            BLOG_DEBUG() << "FileSet configs loaded from Director (JSON)";

            // Reload the currently selected fileset now that configs are available
            QString selected = m_filesetCombo->currentData().toString();
            if (!selected.isEmpty()) {
                loadFileSetFromModel(selected);
            }
        }
    });

    *textConn = connect(wiz->director(), &BDirector::textResult,
            this, [this, jsonConn, textConn](BDirector::Command cmd, const QString &) {
        if (cmd != BDirector::Command::ShowFilesets) return;

        QObject::disconnect(*jsonConn);
        QObject::disconnect(*textConn);

        BLOG_DEBUG() << "show filesets returned text (not supported, need .api 2)";
    });

    wiz->director()->doSend(BDirector::Command::ShowFilesets);
}

void BFileSetSettingsPage::onFileSetsLoaded(const QStringList &names)
{
    m_filesetCombo->clear();
    m_filesetCombo->addItem(tr("-- Select FileSet --"), QString());

    for (const QString &name : names) {
        m_filesetCombo->addItem(name, name);
    }

    m_loadingLabel->setText(tr("(%1 FileSets)").arg(names.count()));

    // If we have a preset name, select it
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (wiz && !wiz->document()->name().isEmpty()) {
        int idx = m_filesetCombo->findData(wiz->document()->name());
        if (idx >= 0) {
            m_filesetCombo->setCurrentIndex(idx);
        }
    }
}

void BFileSetSettingsPage::onFileSetSelected(int index)
{
    if (index <= 0) return;  // Skip "Select FileSet" placeholder

    QString filesetName = m_filesetCombo->itemData(index).toString();
    if (filesetName.isEmpty()) return;

    // Try model-based loading first (from cached "show filesets" data)
    loadFileSetFromModel(filesetName);
}

void BFileSetSettingsPage::loadFileSetFromModel(const QString &filesetName)
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;
    wiz->setRaw(std::nullopt);

    // The text of the resource where the Director offers it: every
    // directive, also those "show filesets" does not report
    if (wiz->canUseConfigMethod(BConfig::Operation::ReadRaw) && wiz->persistence()) {
        m_loadingLabel->setText(tr("Loading..."));
        wiz->persistence()->read(BConfig::ResourceType::FileSet, filesetName, this,
                                 [this, filesetName](const BConfig::Reply &,
                                                     const std::optional<BResourcePersistence::Raw> &raw) {
            BFileSetWizard *w = qobject_cast<BFileSetWizard*>(wizard());
            if (!w) return;
            QString error;
            if (raw && w->document()->loadFromConfigText(raw->text, &error)) {
                w->setRaw(raw);
                showDocument();
                m_loadingLabel->setText(tr("Loaded"));
                return;
            }
            BLOG_WARNING() << "FileSet" << filesetName << "text not read:" << error;
            loadFileSetFromJson(filesetName);
        });
        return;
    }
    loadFileSetFromJson(filesetName);
}

void BFileSetSettingsPage::showDocument()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;
    BFileSetDocument *doc = wiz->document();
    m_nameEdit->setText(doc->name());
    m_descriptionEdit->setText(doc->description());
    m_enableVssCheck->setChecked(doc->enableVss());
    m_ignoreChangesCheck->setChecked(doc->ignoreFileSetChanges());
}

void BFileSetSettingsPage::loadFileSetFromJson(const QString &filesetName)
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    BFilesetModel *model = wiz->filesetModel();
    if (model && model->hasFilesetConfigs()) {
        QJsonObject fsObj = model->filesetConfig(filesetName);
        if (!fsObj.isEmpty()) {
            BFileSetDocument *doc = wiz->document();
            doc->loadFromBareosJson(fsObj);
            showDocument();

            // "show filesets" does not report these two directives
            m_loadingLabel->setText(tr("Loaded from \"show filesets\": Enable VSS and Ignore FileSet "
                                       "Changes are not reported there; check them."));
            BLOG_DEBUG() << "FileSet" << filesetName << "loaded from model cache"
                         << "(includes:" << doc->includeBlockCount() << ")";
            return;
        }
    }

    // Model not ready yet — add default block so wizard is usable
    BLOG_WARNING() << "FileSet" << filesetName << "not found in model cache";
    BFileSetDocument *doc = wiz->document();
    doc->clear();
    doc->setName(filesetName);
    doc->addIncludeBlock();
    m_nameEdit->setText(filesetName);
    m_loadingLabel->setText(tr("Config not cached"));
}

// ============================================================================
// Page 3: Include Blocks
// ============================================================================

BFileSetIncludePage::BFileSetIncludePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Include Blocks"));
    setSubTitle(tr("Configure which files and directories to include in the backup."));

    setupUi();
}

void BFileSetIncludePage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Header row with title and Add button
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addStretch();

    m_addButton = new QPushButton(tr("+ Add Include Block"), this);
    m_addButton->setIcon(QIcon::fromTheme("list-add"));
    headerLayout->addWidget(m_addButton);

    mainLayout->addLayout(headerLayout);

    // Scrollable area for include blocks
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_container = new QWidget();
    m_blocksLayout = new QVBoxLayout(m_container);
    m_blocksLayout->setContentsMargins(0, 0, 0, 0);
    m_blocksLayout->addStretch();

    m_scrollArea->setWidget(m_container);
    mainLayout->addWidget(m_scrollArea);

    connect(m_addButton, &QPushButton::clicked, this, &BFileSetIncludePage::onAddIncludeBlock);
}

void BFileSetIncludePage::initializePage()
{
    // Always rebuild to pick up changes from template or edit selection
    rebuildIncludeBlockWidgets();
    m_initialized = true;
}

void BFileSetIncludePage::cleanupPage()
{
    // Nothing to clean up - widgets remain valid
}

void BFileSetIncludePage::onAddIncludeBlock()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    wiz->document()->addIncludeBlock();
    rebuildIncludeBlockWidgets();
}

void BFileSetIncludePage::onRemoveIncludeBlock(int index)
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    wiz->document()->removeIncludeBlock(index);
    rebuildIncludeBlockWidgets();
}

void BFileSetIncludePage::rebuildIncludeBlockWidgets()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    BFileSetDocument *doc = wiz->document();

    // Clear existing widgets
    for (BIncludeBlockWidget *widget : m_includeWidgets) {
        m_blocksLayout->removeWidget(widget);
        delete widget;
    }
    m_includeWidgets.clear();

    // Remove stretch
    QLayoutItem *item;
    while ((item = m_blocksLayout->takeAt(0)) != nullptr) {
        delete item;
    }

    // Create widget for each include block
    for (int i = 0; i < doc->includeBlockCount(); ++i) {
        BIncludeBlockWidget *widget = new BIncludeBlockWidget(doc, i, m_container);
        connect(widget, &BIncludeBlockWidget::removeRequested,
                this, &BFileSetIncludePage::onRemoveIncludeBlock);

        m_blocksLayout->addWidget(widget);
        m_includeWidgets.append(widget);
    }

    // Add stretch at the end
    m_blocksLayout->addStretch();
}

// ============================================================================
// Page 4: Global Exclude
// ============================================================================

BFileSetExcludePage::BFileSetExcludePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Global Exclude"));
    setSubTitle(tr("Configure paths and patterns to exclude from all Include blocks."));

    setupUi();
}

void BFileSetExcludePage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Global exclude paths
    QGroupBox *excludeGroup = new QGroupBox(tr("Exclude Paths"));
    QVBoxLayout *excludeLayout = new QVBoxLayout(excludeGroup);

    m_excludeWidget = new BEditableListWidget(this);
    m_excludeWidget->setTitle(tr("Global Exclude"));
    m_excludeWidget->setPlaceholderText(tr("Enter path to exclude..."));
    m_excludeWidget->setAddButtonText(tr("+ Add Path"));

    excludeLayout->addWidget(m_excludeWidget);
    mainLayout->addWidget(excludeGroup);

    QLabel *infoLabel = new QLabel(
        tr("These paths will be excluded from all backup jobs using this FileSet.\n"
           "Use absolute paths (e.g., /tmp, /var/cache) or patterns."), this);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("font-style: italic;");
    bSetLabelState(infoLabel, "muted");
    mainLayout->addWidget(infoLabel);

    mainLayout->addStretch();
}

void BFileSetExcludePage::initializePage()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    if (!m_initialized) {
        BFileSetDocument *doc = wiz->document();

        // Connect model and undo stack
        m_excludeWidget->setModel(doc->excludePathModel());
        m_excludeWidget->setUndoStack(doc->undoStack());

        m_initialized = true;
    }
}

// ============================================================================
// Page 5: Preview & Execute
// ============================================================================

BFileSetPreviewPage::BFileSetPreviewPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Preview & Deploy"));
    setSubTitle(tr("Review the generated FileSet configuration. Finish sends it to the Director where the "
                   "Director accepts FileSets; otherwise copy it for manual deployment."));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Resource preview (use Preview toggle to see raw config)
    m_filesetWidget = new BResourceWidget("FileSet", nullptr, this);
    mainLayout->addWidget(m_filesetWidget, 1);  // stretch factor 1: takes available space

    // Configuration header row (label + action buttons)
    QHBoxLayout *cmdHeaderLayout = new QHBoxLayout();
    cmdHeaderLayout->setContentsMargins(0, 4, 0, 0);

    QLabel *cmdLabel = new QLabel(tr("Configuration:"), this);
    cmdLabel->setStyleSheet("font-weight: bold;");
    cmdHeaderLayout->addWidget(cmdLabel);

    cmdHeaderLayout->addStretch();

    m_copyButton = new QPushButton(tr("Copy configuration"), this);
    cmdHeaderLayout->addWidget(m_copyButton);

    mainLayout->addLayout(cmdHeaderLayout);

    // Configure command text
    m_commandEdit = new QTextEdit(this);
    m_commandEdit->setReadOnly(true);
    BUiScale::track(m_commandEdit, [this]() { m_commandEdit->setMinimumHeight(BUiScale::px(160)); });
    BUiScale::bind(m_commandEdit, BUiScale::Normal, BUiScale::Monospace);
    m_commandEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    m_commandEdit->setStyleSheet("QTextEdit { background: palette(base); border: 1px solid palette(mid); padding: 2px; }");
    mainLayout->addWidget(m_commandEdit);

    // Validation + status row
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(0, 0, 0, 0);

    m_validationLabel = new QLabel(this);
    bottomLayout->addWidget(m_validationLabel);

    m_statusLabel = new QLabel(this);
    bottomLayout->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    BUiScale::track(m_progressBar, [this]() { m_progressBar->setMaximumWidth(BUiScale::px(100)); });
    bottomLayout->addWidget(m_progressBar);

    bottomLayout->addStretch();

    mainLayout->addLayout(bottomLayout);

    connect(m_copyButton, &QPushButton::clicked, this, &BFileSetPreviewPage::onCopyConfig);
}

void BFileSetPreviewPage::initializePage()
{
    generateConfig();
}

void BFileSetPreviewPage::generateConfig()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    BFileSetDocument *doc = wiz->document();

    // Generate configuration from document
    QString textError;
    QString config = doc->toConfigText(&textError);

    const bool canSend = wiz->canUseConfigMethod(updatesExisting() ? BConfig::Operation::UpdateRaw
                                                                     : BConfig::Operation::CreateRaw);
    QString notice;
    if (canSend) {
        notice = updatesExisting()
            ? tr("# Finish writes this text over the FileSet %1 on the Director, after the\n"
                 "# Director has checked it and you have confirmed the change.\n\n").arg(wiz->originalName())
            : tr("# Finish creates this FileSet on the Director, after the Director has\n"
                 "# checked it and you have confirmed it.\n\n");
    } else {
        notice = tr(
            "# NOTE: Bareos Director does not support 'configure add fileset' with\n"
            "# nested Include/Exclude/Options blocks (confirmed Bareos limitation).\n"
            "# Deploy this configuration manually:\n"
            "#   1. Copy to /etc/bareos/bareos-dir.d/fileset/%1.conf\n"
            "#   2. Run: systemctl reload bareos-dir\n"
            "#\n"
            "# Use 'Copy configuration' to copy the text below.\n\n")
            .arg(doc->name());
    }
    m_commandEdit->setPlainText(notice + (config.isEmpty() ? QStringLiteral("# ") + textError : config));

    // Parse and display in resource widget
    BConfigParser parser;
    if (parser.parseString(config)) {
        QList<BConfigResource> resources = parser.resources();
        if (!resources.isEmpty()) {
            m_filesetWidget->setResources(resources);
        }
    }

    // Always show command area (contains config text now)
    m_commandEdit->setVisible(true);
    m_copyButton->setVisible(true);

    // Validation
    BFileSetDocument::ValidationResult validation = doc->validate();

    if (!validation.errors.isEmpty()) {
        m_validationLabel->setText(BStatus::textSpan(BStatus::Tone::Error, validation.errors.join("<br>")));
    } else if (!validation.warnings.isEmpty()) {
        m_validationLabel->setText(BStatus::textSpan(BStatus::Tone::Warning, validation.warnings.join("<br>")));
    } else {
        m_validationLabel->setText(BStatus::textSpan(BStatus::Tone::Success, tr("Configuration is valid")));
    }
}

bool BFileSetPreviewPage::updatesExisting() const
{
    auto *wiz = qobject_cast<BFileSetWizard *>(wizard());
    // Only a FileSet loaded from its text, under its old name: a renamed
    // one is created as a new FileSet
    return wiz && wiz->mode() == BFileSetWizard::EditMode && wiz->raw()
        && wiz->document()->name() == wiz->originalName();
}

bool BFileSetPreviewPage::validatePage()
{
    if (m_executed) return true;
    if (m_waiting) return false;

    auto *wiz = qobject_cast<BFileSetWizard *>(wizard());
    if (!wiz) return true;

    const BFileSetDocument::ValidationResult validation = wiz->document()->validate();
    if (!validation.valid) {
        QMessageBox::warning(this, tr("FileSet not complete"), validation.errors.join(QLatin1Char('\n')));
        return false;
    }

    if (deploy())
        return false;   // the wizard closes when the Director has written it

    // Finishing the wizard is where the user expects the FileSet to reach
    // the Director. Say once per session why it does not.
    BCapabilityNotice::check(this, wiz->director(), BCapabilities::Function::DeployFileSet,
                             BCapabilities::Request::Implicit);

    int ret = QMessageBox::question(this, tr("Close Wizard"),
        tr("The configuration has not been sent to the Director.\n"
           "Use 'Copy configuration' to save it before closing.\n\n"
           "Do you want to close the wizard?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    return (ret == QMessageBox::Yes);
}

bool BFileSetPreviewPage::deploy()
{
    auto *wiz = qobject_cast<BFileSetWizard *>(wizard());
    if (!wiz) return false;
    const bool update = updatesExisting();
    if (!wiz->canUseConfigMethod(update ? BConfig::Operation::UpdateRaw : BConfig::Operation::CreateRaw))
        return false;
    BResourcePersistence *persistence = wiz->persistence();
    if (!persistence) return false;
    if (persistence->isBusy()) {
        m_statusLabel->setText(tr("Another change is still being written. Try again when it is done."));
        return true;
    }

    BFileSetDocument *doc = wiz->document();
    QString error;
    const QString text = doc->toConfigText(&error);
    if (text.isEmpty()) {
        m_statusLabel->setText(BStatus::textSpan(BStatus::Tone::Error, error.toHtmlEscaped()));
        return true;
    }

    if (!m_finishedConnection) {
        m_finishedConnection = connect(persistence, &BResourcePersistence::finished, this,
                                       [this](const BResourceReport &report) {
            if (!m_waiting) return;
            m_waiting = false;
            m_progressBar->setVisible(false);
            if (report.kind == BResourceReport::Written) {
                // The flow has told the user already (its report)
                m_executed = true;
                if (QWizard *w = wizard())
                    w->accept();
            } else if (report.kind == BResourceReport::Cancelled) {
                m_statusLabel->setText(tr("Nothing was written."));
            } else {
                m_statusLabel->setText(BStatus::textSpan(
                    BStatus::Tone::Error,
                    QString(report.title + QStringLiteral(": ") + report.text).left(600).toHtmlEscaped()));
            }
        });
    }

    BResourceReview review;
    m_waiting = true;
    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("The Director checks the FileSet..."));
    bool started = false;
    if (update) {
        review.note = tr("The wizard writes the whole FileSet as it generates it: every directive is "
                         "kept, comments and the layout of the file are not.");
        started = persistence->update(BConfig::ResourceType::FileSet, doc->name(), *wiz->raw(), text, review);
    } else {
        review.note = tr("A new FileSet. No job uses it until a job names it.");
        started = persistence->create(BConfig::ResourceType::FileSet, doc->name(), text, review);
    }
    if (!started && m_waiting) {
        // Refused at once; the report (if any) came through finished()
        m_waiting = false;
        m_progressBar->setVisible(false);
    }
    return true;
}

void BFileSetPreviewPage::onCopyConfig()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    QApplication::clipboard()->setText(wiz->document()->toConfigText());
    m_statusLabel->setText(tr("Configuration copied to clipboard"));
}
