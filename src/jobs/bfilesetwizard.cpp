/**
 * @file bfilesetwizard.cpp
 * @brief Unified wizard for creating and editing FileSet resources
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */

#include "jobs/bfilesetwizard.h"
#include "jobs/fileset/bfilesetdocument.h"
#include "blogging.h"
#include "jobs/fileset/bincludeblockwidget.h"
#include "config/beditablelistwidget.h"
#include "config/beditablelistmodel.h"
#include "config/bconfigparser.h"
#include "director/bdirector.h"
#include "director/bresourcewidget.h"

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
#include <QFileDialog>
#include <private/qzipwriter_p.h>

// Static template storage
QJsonObject BFileSetWizard::s_templates;

// ============================================================================
// BFileSetWizard
// ============================================================================

BFileSetWizard::BFileSetWizard(BDirector *director, QWidget *parent)
    : QWizard(parent)
    , m_mode(NewMode)
    , m_director(director)
    , m_document(new BFileSetDocument(this))
{
    init();
}

BFileSetWizard::BFileSetWizard(BDirector *director, const QString &filesetName,
                               QWidget *parent)
    : QWizard(parent)
    , m_mode(EditMode)
    , m_director(director)
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
}

void BFileSetWizard::init()
{
    setWindowTitle(m_mode == NewMode ? tr("New FileSet Wizard") : tr("Edit FileSet"));
    setWizardStyle(QWizard::ModernStyle);
    setMinimumSize(800, 700);

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
    m_mixedLabel->setStyleSheet("color: #0066cc; font-weight: bold;");
    m_mixedLabel->setVisible(false);
    layout->addWidget(m_mixedLabel);

    // Description group
    QGroupBox *descGroup = new QGroupBox(tr("Template Description"));
    QVBoxLayout *descLayout = new QVBoxLayout(descGroup);

    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setWordWrap(true);
    descLayout->addWidget(m_descriptionLabel);

    m_platformLabel = new QLabel(this);
    m_platformLabel->setStyleSheet("color: gray; font-style: italic;");
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
    includeScroll->setMinimumHeight(100);
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
    excludeScroll->setMinimumHeight(100);
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

    // Clear existing data
    doc->clear();

    // Set basic properties from template
    doc->setName(templateObj["name"].toString());
    doc->setDescription(templateObj["description"].toString());

    // Extract fileset structure from template
    QJsonObject fileset = templateObj["fileset"].toObject();
    doc->setEnableVss(fileset["Enable VSS"].toBool(true));
    doc->setIgnoreFileSetChanges(fileset["Ignore FileSet Changes"].toBool(false));
    doc->setEnableSnapshot(fileset["Enable Snapshot"].toBool(false));

    // Load Include block from template
    QJsonObject include = fileset["Include"].toObject();
    BFileSetDocument::IncludeBlock *block = doc->addIncludeBlock();

    // Include paths from fileset.Include.File
    QJsonValue fileVal = include["File"];
    QStringList includePaths;
    if (fileVal.isArray()) {
        for (const QJsonValue &v : fileVal.toArray()) {
            includePaths.append(v.toString());
        }
    } else if (fileVal.isString()) {
        includePaths.append(fileVal.toString());
    }
    block->pathsModel->setItems(includePaths);

    // Load options from fileset.Include.Options
    QJsonObject opts = include["Options"].toObject();
    for (auto it = opts.begin(); it != opts.end(); ++it) {
        if (it.key().compare("Exclude", Qt::CaseInsensitive) == 0) {
            // Nested exclude in Options
            QJsonObject exclObj = it.value().toObject();
            QStringList exclFiles, exclWildDir, exclWildFile;

            // File exclusions
            QJsonValue filesVal = exclObj["File"];
            if (filesVal.isArray()) {
                for (const QJsonValue &v : filesVal.toArray()) {
                    exclFiles.append(v.toString());
                }
            } else if (filesVal.isString()) {
                exclFiles.append(filesVal.toString());
            }

            // WildDir patterns (directory patterns)
            QJsonValue wildDirVal = exclObj["WildDir"];
            if (wildDirVal.isArray()) {
                for (const QJsonValue &v : wildDirVal.toArray()) {
                    exclWildDir.append(v.toString());
                }
            } else if (wildDirVal.isString()) {
                exclWildDir.append(wildDirVal.toString());
            }

            // WildFile patterns (file patterns)
            QJsonValue wildFileVal = exclObj["WildFile"];
            if (wildFileVal.isArray()) {
                for (const QJsonValue &v : wildFileVal.toArray()) {
                    exclWildFile.append(v.toString());
                }
            } else if (wildFileVal.isString()) {
                exclWildFile.append(wildFileVal.toString());
            }

            block->excludeFilesModel->setItems(exclFiles);
            block->excludePatternsModel->setItems(exclWildDir);
            block->excludeWildFileModel->setItems(exclWildFile);
        } else {
            // Regular option
            QJsonValue val = it.value();
            if (val.isBool()) {
                block->options[it.key()] = val.toBool();
            } else if (val.isString()) {
                block->options[it.key()] = val.toString();
            }
        }
    }

    // Load global Exclude from fileset.Exclude
    QJsonObject exclude = fileset["Exclude"].toObject();
    QJsonValue exclFileVal = exclude["File"];
    QStringList excludePaths;
    if (exclFileVal.isArray()) {
        for (const QJsonValue &v : exclFileVal.toArray()) {
            excludePaths.append(v.toString());
        }
    } else if (exclFileVal.isString()) {
        excludePaths.append(exclFileVal.toString());
    }
    doc->excludePathModel()->setItems(excludePaths);

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
    dialog.setMinimumSize(500, 600);

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
        item->setToolTip(QString("%1\nPlatform: %2").arg(desc, platform));

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
    m_filesetCombo->setMinimumWidth(250);
    m_filesetCombo->setPlaceholderText(tr("Loading FileSets..."));

    m_loadingLabel = new QLabel(this);
    m_loadingLabel->setStyleSheet("color: gray; font-style: italic;");

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

    m_enableSnapshotCheck = new QCheckBox(tr("Enable Snapshot"), this);
    m_enableSnapshotCheck->setToolTip(tr("Enable snapshot support (Linux LVM/btrfs)"));
    globalLayout->addWidget(m_enableSnapshotCheck);

    globalLayout->addStretch();
    mainLayout->addWidget(globalGroup);

    // Execute option
    m_executeCheck = new QCheckBox(tr("Execute 'configure add fileset' on Director"), this);
    m_executeCheck->setChecked(true);
    mainLayout->addWidget(m_executeCheck);

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

    // Always update from document (template may have changed)
    m_nameEdit->setText(doc->name().replace(" ", ""));
    m_descriptionEdit->setText(doc->description());
    m_enableVssCheck->setChecked(doc->enableVss());
    m_ignoreChangesCheck->setChecked(doc->ignoreFileSetChanges());
    m_enableSnapshotCheck->setChecked(doc->enableSnapshot());
    m_executeCheck->setChecked(wiz->executeOnFinish());
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
    return !m_nameEdit->text().trimmed().isEmpty();
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
    doc->setEnableSnapshot(m_enableSnapshotCheck->isChecked());

    wiz->setExecuteOnFinish(m_executeCheck->isChecked());
}

void BFileSetSettingsPage::fetchFileSets()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz || !wiz->director()) {
        m_loadingLabel->setText(tr("No Director connection"));
        return;
    }

    // Connect to Director's fileSetsResult signal
    connect(wiz->director(), &BDirector::jsonResult,
            this, [this](BDirector::Command cmd, const QString &jsonData) {
        if (cmd != BDirector::Command::DotFilesets) return;

        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonArray filesets = doc.object()["result"].toObject()["filesets"].toArray();
        onFileSetsLoaded(filesets);

        // Disconnect after receiving
        BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult, this, nullptr);
        }
    });

    // Send command to fetch filesets
    wiz->director()->doSend(BDirector::Command::DotFilesets);
}

void BFileSetSettingsPage::onFileSetsLoaded(const QJsonArray &filesets)
{
    m_filesetCombo->clear();
    m_filesetCombo->addItem(tr("-- Select FileSet --"), QString());

    for (const QJsonValue &fs : filesets) {
        QJsonObject fsObj = fs.toObject();
        QString name = fsObj["name"].toString();
        if (!name.isEmpty()) {
            m_filesetCombo->addItem(name, name);
        }
    }

    m_loadingLabel->setText(QString("(%1 FileSets)").arg(filesets.count()));

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

    loadFileSetFromDirector(filesetName);
}

void BFileSetSettingsPage::loadFileSetFromDirector(const QString &filesetName)
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz || !wiz->director()) return;

    m_loadingLabel->setText(tr("Loading %1...").arg(filesetName));

    // Handler for parsing "show fileset" response (works for both JSON and text)
    auto parseResponse = [this, filesetName](const QString &response, bool isJson) {
        BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
        if (!wiz) return;

        BFileSetDocument *document = wiz->document();
        document->clear();
        document->setName(filesetName);  // Use the selected name

        if (isJson) {
            // Parse JSON response
            QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject result = doc.object()["result"].toObject();
            QJsonArray filesets = result["filesets"].toArray();

            if (!filesets.isEmpty()) {
                QJsonObject fsObj = filesets.first().toObject();

                document->setDescription(fsObj["description"].toString());
                document->setEnableVss(fsObj["enablevss"].toBool(true));
                document->setIgnoreFileSetChanges(fsObj["ignorefilesetchanges"].toBool(false));

                // Load Include blocks
                QJsonArray includes = fsObj["include"].toArray();
                for (const QJsonValue &inc : includes) {
                    QJsonObject incObj = inc.toObject();
                    BFileSetDocument::IncludeBlock *block = document->addIncludeBlock();

                    QJsonArray files = incObj["file"].toArray();
                    QStringList paths;
                    for (const QJsonValue &f : files) {
                        paths.append(f.toString());
                    }
                    block->pathsModel->setItems(paths);

                    QJsonObject opts = incObj["options"].toObject();
                    for (auto it = opts.begin(); it != opts.end(); ++it) {
                        block->options[it.key()] = it.value().toVariant();
                    }
                }

                // Load Exclude
                QJsonArray excludes = fsObj["exclude"].toArray();
                QStringList excludePaths;
                for (const QJsonValue &exc : excludes) {
                    QJsonObject excObj = exc.toObject();
                    QJsonArray files = excObj["file"].toArray();
                    for (const QJsonValue &f : files) {
                        excludePaths.append(f.toString());
                    }
                }
                document->excludePathModel()->setItems(excludePaths);
            }
        } else {
            // Parse text response from "show fileset"
            // The "show fileset" command returns a different format than config files
            // Try to parse it, but the format may need conversion
            BLOG_DEBUG() << "FileSet text response:" << response.left(500);

            BConfigParser parser;
            if (parser.parseString(response)) {
                QList<BConfigResource> resources = parser.resources();
                BLOG_DEBUG() << "Parsed" << resources.count() << "resources";
                for (const BConfigResource &res : resources) {
                    BLOG_DEBUG() << "Resource type:" << res.type();
                    if (res.type().compare("FileSet", Qt::CaseInsensitive) == 0) {
                        document->loadFromResource(res);
                        break;
                    }
                }
            } else {
                BLOG_DEBUG() << "BConfigParser failed to parse response";
            }
        }

        // If document is still empty after parsing, add a default include block
        if (document->includeBlockCount() == 0) {
            BLOG_DEBUG() << "Adding default include block (parsing may have failed)";
            document->addIncludeBlock();
        }

        // Update UI
        m_nameEdit->setText(document->name());
        m_descriptionEdit->setText(document->description());
        m_enableVssCheck->setChecked(document->enableVss());
        m_ignoreChangesCheck->setChecked(document->ignoreFileSetChanges());
        m_enableSnapshotCheck->setChecked(document->enableSnapshot());

        m_loadingLabel->setText(tr("Loaded"));
    };

    // Connect to Director's JSON response
    connect(wiz->director(), &BDirector::jsonResult,
            this, [this, parseResponse](BDirector::Command cmd, const QString &jsonData) {
        if (cmd != BDirector::Command::ShowFileset) return;

        BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult, this, nullptr);
            disconnect(wiz->director(), &BDirector::textResult, this, nullptr);
        }

        parseResponse(jsonData, true);
    });

    // Connect to Director's text response (fallback)
    connect(wiz->director(), &BDirector::textResult,
            this, [this, parseResponse](BDirector::Command cmd, const QString &textData) {
        if (cmd != BDirector::Command::ShowFileset) return;

        BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
        if (wiz && wiz->director()) {
            disconnect(wiz->director(), &BDirector::jsonResult, this, nullptr);
            disconnect(wiz->director(), &BDirector::textResult, this, nullptr);
        }

        parseResponse(textData, false);
    });

    // Send command to fetch specific fileset
    wiz->director()->doSend(BDirector::Command::ShowFileset, filesetName);
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
    infoLabel->setStyleSheet("color: gray; font-style: italic;");
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
    , m_timeoutTimer(new QTimer(this))
{
    setTitle(tr("Preview & Execute"));
    setSubTitle(tr("Review the generated FileSet configuration and execute on the Director."));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Resource preview (use Preview toggle to see raw config)
    m_filesetWidget = new BResourceWidget("FileSet", nullptr, this);
    mainLayout->addWidget(m_filesetWidget, 1);  // stretch factor 1: takes available space

    // Configure command header row (label + copy button)
    QHBoxLayout *cmdHeaderLayout = new QHBoxLayout();
    cmdHeaderLayout->setContentsMargins(0, 4, 0, 0);

    QLabel *cmdLabel = new QLabel(tr("Command:"), this);
    cmdLabel->setStyleSheet("font-weight: bold;");
    cmdHeaderLayout->addWidget(cmdLabel);

    cmdHeaderLayout->addStretch();

    m_copyButton = new QPushButton(tr("Copy configuration"), this);
    cmdHeaderLayout->addWidget(m_copyButton);

    mainLayout->addLayout(cmdHeaderLayout);

    // Configure command text
    m_commandEdit = new QTextEdit(this);
    m_commandEdit->setReadOnly(true);
    m_commandEdit->setFixedHeight(104);
    m_commandEdit->setFont(QFont("Consolas", 9));
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
    m_progressBar->setMaximumWidth(100);
    bottomLayout->addWidget(m_progressBar);

    bottomLayout->addStretch();

    mainLayout->addLayout(bottomLayout);

    // Timeout timer (30 seconds)
    m_timeoutTimer->setSingleShot(true);
    m_timeoutTimer->setInterval(30000);

    connect(m_copyButton, &QPushButton::clicked, this, &BFileSetPreviewPage::onCopyConfig);
    connect(m_timeoutTimer, &QTimer::timeout, this, &BFileSetPreviewPage::onConfigureTimeout);
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
    QString config = doc->toConfigText();

    // Build configure command (single-line format for bconsole)
    QString command = doc->toConfigureCommand();

    // Display in UI
    m_commandEdit->setPlainText(command);

    // Parse and display in resource widget
    BConfigParser parser;
    if (parser.parseString(config)) {
        QList<BConfigResource> resources = parser.resources();
        if (!resources.isEmpty()) {
            m_filesetWidget->setResources(resources);
        }
    }

    // Show/hide command row based on execute option
    bool showCmd = wiz->executeOnFinish();
    m_commandEdit->setVisible(showCmd);
    m_copyButton->setVisible(showCmd);

    // Validation
    BFileSetDocument::ValidationResult validation = doc->validate();

    if (!validation.errors.isEmpty()) {
        m_validationLabel->setText(QString("<span style='color: red;'>%1</span>")
            .arg(validation.errors.join("<br>")));
    } else if (!validation.warnings.isEmpty()) {
        m_validationLabel->setText(QString("<span style='color: orange;'>%1</span>")
            .arg(validation.warnings.join("<br>")));
    } else {
        m_validationLabel->setText(QString("<span style='color: green;'>%1</span>")
            .arg(tr("Configuration is valid")));
    }
}

bool BFileSetPreviewPage::validatePage()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return true;

    // If execute option is unchecked, just close the wizard
    if (!wiz->executeOnFinish()) {
        return true;
    }

    // If already executed successfully, allow closing
    if (m_executed) {
        return true;
    }

    // Start async execution and keep wizard open until complete
    executeConfigureCommand();
    return false;
}

void BFileSetPreviewPage::executeConfigureCommand()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz || !wiz->director()) return;

    m_statusLabel->setText(tr("Executing configure command..."));
    m_progressBar->setVisible(true);
    m_timeoutTimer->start();

    // Connect to director signals
    connect(wiz->director(), &BDirector::jsonResult,
            this, &BFileSetPreviewPage::onJsonResponse);
    connect(wiz->director(), &BDirector::textResult,
            this, &BFileSetPreviewPage::onCommandResponse);

    // Send configure command (full FileSet definition)
    QString command = wiz->document()->toConfigureCommand();
    wiz->director()->doSend(BDirector::Command::Configure, command);
}

void BFileSetPreviewPage::onJsonResponse(BDirector::Command cmd, const QString &jsonData)
{
    if (cmd != BDirector::Command::Configure) return;

    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    m_timeoutTimer->stop();
    m_progressBar->setVisible(false);

    // Disconnect signals
    disconnect(wiz->director(), &BDirector::jsonResult,
               this, &BFileSetPreviewPage::onJsonResponse);
    disconnect(wiz->director(), &BDirector::textResult,
               this, &BFileSetPreviewPage::onCommandResponse);

    // Parse response
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject result = doc.object()["result"].toObject();
    QJsonObject configureObj = result["configure"].toObject();

    if (configureObj.contains("add")) {
        m_statusLabel->setText(QString("<span style='color: green;'>%1</span>")
            .arg(tr("FileSet created successfully!")));
        m_executed = true;

        // Trigger reload
        wiz->director()->doSend(BDirector::Command::Reload);

        // Auto-close wizard after brief delay so user can see the success message
        QTimer::singleShot(1500, this, [this]() {
            if (wizard()) wizard()->accept();
        });
    } else {
        QString error = result["error"].toString();
        if (error.isEmpty()) error = tr("Unknown error");
        m_statusLabel->setText(QString("<span style='color: red;'>%1: %2</span>")
            .arg(tr("Error")).arg(error));
    }
}

void BFileSetPreviewPage::onCommandResponse(BDirector::Command cmd, const QString &response)
{
    if (cmd != BDirector::Command::Configure) return;

    m_timeoutTimer->stop();
    m_progressBar->setVisible(false);

    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (wiz && wiz->director()) {
        disconnect(wiz->director(), &BDirector::jsonResult,
                   this, &BFileSetPreviewPage::onJsonResponse);
        disconnect(wiz->director(), &BDirector::textResult,
                   this, &BFileSetPreviewPage::onCommandResponse);
    }

    // Handle text response
    if (response.contains("Created") || response.contains("success", Qt::CaseInsensitive)) {
        m_statusLabel->setText(QString("<span style='color: green;'>%1</span>")
            .arg(tr("FileSet created successfully!")));
        m_executed = true;

        // Auto-close wizard after brief delay so user can see the success message
        QTimer::singleShot(1500, this, [this]() {
            if (wizard()) wizard()->accept();
        });
    } else {
        // Show the Director's error response
        m_statusLabel->setText(QString("<span style='color: red;'>%1: %2</span>")
            .arg(tr("Error")).arg(response.trimmed()));
    }
}

void BFileSetPreviewPage::onConfigureTimeout()
{
    m_progressBar->setVisible(false);

    int ret = QMessageBox::question(this, tr("Timeout"),
        tr("The configure command timed out.\nDo you want to retry?"),
        QMessageBox::Retry | QMessageBox::Cancel);

    if (ret == QMessageBox::Retry) {
        executeConfigureCommand();
    } else {
        m_statusLabel->setText(tr("Command timed out"));
    }
}

void BFileSetPreviewPage::onCopyConfig()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    QApplication::clipboard()->setText(wiz->document()->toConfigText());
    m_statusLabel->setText(tr("Configuration copied to clipboard"));
}

void BFileSetPreviewPage::onExportZip()
{
    BFileSetWizard *wiz = qobject_cast<BFileSetWizard*>(wizard());
    if (!wiz) return;

    BFileSetDocument *doc = wiz->document();
    QString config = doc->toConfigText();

    // Ask user for save location
    QString filter = tr("ZIP Archives (*.zip);;Configuration Files (*.conf);;All Files (*)");
    QString selectedFilter;
    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Export FileSet Configuration"),
        QString("%1.zip").arg(doc->name()),
        filter,
        &selectedFilter);

    if (filePath.isEmpty()) return;

    // Determine export format based on selected filter or extension
    bool exportAsZip = selectedFilter.contains("zip", Qt::CaseInsensitive) ||
                       filePath.endsWith(".zip", Qt::CaseInsensitive);

    if (exportAsZip) {
        // Ensure .zip extension
        if (!filePath.endsWith(".zip", Qt::CaseInsensitive)) {
            filePath += ".zip";
        }

        // Create ZIP using Qt's QZipWriter
        QZipWriter zip(filePath);
        if (zip.status() != QZipWriter::NoError) {
            QMessageBox::warning(this, tr("Export Error"),
                tr("Cannot create zip file: %1").arg(filePath));
            return;
        }

        // Create directory structure
        QString dirPath = QString("etc/bareos/bareos-dir.d/fileset");
        zip.addDirectory("etc");
        zip.addDirectory("etc/bareos");
        zip.addDirectory("etc/bareos/bareos-dir.d");
        zip.addDirectory(dirPath);

        // Add FileSet configuration file
        QString confPath = QString("%1/%2.conf").arg(dirPath, doc->name());
        zip.addFile(confPath, config.toUtf8());

        // Add README
        QString readme = QString(
            "Bareos FileSet Configuration: %1\n"
            "=========================================\n\n"
            "Contents:\n"
            "  etc/bareos/bareos-dir.d/fileset/%1.conf\n\n"
            "Installation:\n"
            "  1. Extract this archive to / (root) on the Director server\n"
            "     Or copy the .conf file to /etc/bareos/bareos-dir.d/fileset/\n"
            "  2. Reload the Director: systemctl reload bareos-dir\n\n"
            "Generated by Onesimus\n").arg(doc->name());
        zip.addFile("README.txt", readme.toUtf8());

        zip.close();

        if (zip.status() != QZipWriter::NoError) {
            QMessageBox::warning(this, tr("Export Error"),
                tr("Error writing zip file"));
            return;
        }

        m_statusLabel->setText(tr("Exported to: %1").arg(filePath));
    } else {
        // Export as plain .conf file
        if (!filePath.endsWith(".conf", Qt::CaseInsensitive)) {
            filePath += ".conf";
        }

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Export Error"),
                tr("Cannot write file: %1").arg(filePath));
            return;
        }

        file.write(config.toUtf8());
        file.close();

        m_statusLabel->setText(tr("Exported to: %1").arg(filePath));
    }
}
