/**
 * @file bfilesetdocument.cpp
 * @brief Pure data model for FileSet editing with undo/redo support
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */

#include "jobs/fileset/bfilesetdocument.h"
#include "config/bconfigparser.h"
#include <QJsonArray>

BFileSetDocument::BFileSetDocument(QObject *parent)
    : QObject(parent)
    , m_undoStack(new QUndoStack(this))
    , m_excludePathModel(new BEditableListModel(this))
{
    m_excludePathModel->setUndoStack(m_undoStack);

    connect(m_excludePathModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);

    connect(m_undoStack, &QUndoStack::cleanChanged, this, [this](bool clean) {
        emit modifiedChanged(!clean);
    });
}

BFileSetDocument::~BFileSetDocument()
{
    qDeleteAll(m_includeBlocks);
}

// ============================================================================
// Basic Properties
// ============================================================================

QString BFileSetDocument::name() const
{
    return m_name;
}

void BFileSetDocument::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged(m_name);
        emit documentChanged();
    }
}

QString BFileSetDocument::description() const
{
    return m_description;
}

void BFileSetDocument::setDescription(const QString &desc)
{
    if (m_description != desc) {
        m_description = desc;
        emit descriptionChanged(m_description);
        emit documentChanged();
    }
}

bool BFileSetDocument::enableVss() const
{
    return m_enableVss;
}

void BFileSetDocument::setEnableVss(bool enable)
{
    if (m_enableVss != enable) {
        m_enableVss = enable;
        emit documentChanged();
    }
}

bool BFileSetDocument::ignoreFileSetChanges() const
{
    return m_ignoreFileSetChanges;
}

void BFileSetDocument::setIgnoreFileSetChanges(bool ignore)
{
    if (m_ignoreFileSetChanges != ignore) {
        m_ignoreFileSetChanges = ignore;
        emit documentChanged();
    }
}

bool BFileSetDocument::enableSnapshot() const
{
    return m_enableSnapshot;
}

void BFileSetDocument::setEnableSnapshot(bool enable)
{
    if (m_enableSnapshot != enable) {
        m_enableSnapshot = enable;
        emit documentChanged();
    }
}

// ============================================================================
// Include Block Management
// ============================================================================

int BFileSetDocument::includeBlockCount() const
{
    return m_includeBlocks.count();
}

BFileSetDocument::IncludeBlock *BFileSetDocument::includeBlock(int index)
{
    if (index < 0 || index >= m_includeBlocks.count()) {
        return nullptr;
    }
    return m_includeBlocks.at(index);
}

const BFileSetDocument::IncludeBlock *BFileSetDocument::includeBlock(int index) const
{
    if (index < 0 || index >= m_includeBlocks.count()) {
        return nullptr;
    }
    return m_includeBlocks.at(index);
}

BFileSetDocument::IncludeBlock *BFileSetDocument::addIncludeBlock()
{
    IncludeBlock *block = new IncludeBlock();
    block->id = m_nextBlockId++;

    // Create models for this block
    block->pathsModel = new BEditableListModel(this);
    block->pathsModel->setUndoStack(m_undoStack);

    block->excludeFilesModel = new BEditableListModel(this);
    block->excludeFilesModel->setUndoStack(m_undoStack);

    block->excludePatternsModel = new BEditableListModel(this);
    block->excludePatternsModel->setUndoStack(m_undoStack);

    block->excludeWildFileModel = new BEditableListModel(this);
    block->excludeWildFileModel->setUndoStack(m_undoStack);

    // Initialize default options
    initDefaultOptions(block);

    // Connect signals
    connectBlockSignals(block);

    int index = m_includeBlocks.count();
    m_includeBlocks.append(block);
    emit includeBlockAdded(index);
    emit documentChanged();

    return block;
}

void BFileSetDocument::removeIncludeBlock(int index)
{
    if (index < 0 || index >= m_includeBlocks.count()) {
        return;
    }

    IncludeBlock *block = m_includeBlocks.takeAt(index);

    delete block->pathsModel;
    delete block->excludeFilesModel;
    delete block->excludePatternsModel;
    delete block->excludeWildFileModel;
    delete block;

    emit includeBlockRemoved(index);
    emit documentChanged();
}

// ============================================================================
// Global Exclude
// ============================================================================

BEditableListModel *BFileSetDocument::excludePathModel() const
{
    return m_excludePathModel;
}

// ============================================================================
// Undo/Redo
// ============================================================================

QUndoStack *BFileSetDocument::undoStack() const
{
    return m_undoStack;
}

bool BFileSetDocument::isModified() const
{
    return !m_undoStack->isClean();
}

void BFileSetDocument::markClean()
{
    m_undoStack->setClean();
}

// ============================================================================
// Serialization
// ============================================================================

void BFileSetDocument::loadFromResource(const BConfigResource &resource)
{
    clear();

    m_name = resource.simpleValue("Name");
    m_description = resource.simpleValue("Description");
    m_enableVss = resource.simpleValue("EnableVSS", "yes").toLower() == "yes";
    m_ignoreFileSetChanges = resource.simpleValue("IgnoreFileSetChanges", "no").toLower() == "yes";
    m_enableSnapshot = resource.simpleValue("EnableSnapshot", "no").toLower() == "yes";

    // Load Include blocks - BConfigResource stores nested structures
    BConfigValue includeValue = resource.value("Include");
    if (includeValue.type() == BConfigValue::Block) {
        IncludeBlock *block = addIncludeBlock();

        // Load paths from block
        QMap<QString, BConfigValue> incBlock = includeValue.blockValue();
        if (incBlock.contains("File")) {
            BConfigValue fileValue = incBlock.value("File");
            if (fileValue.type() == BConfigValue::List) {
                block->pathsModel->setItems(fileValue.listValue());
            } else if (fileValue.type() == BConfigValue::Simple) {
                block->pathsModel->setItems({fileValue.simpleValue()});
            }
        }

        // Load options
        if (incBlock.contains("Options")) {
            BConfigValue optsValue = incBlock.value("Options");
            if (optsValue.type() == BConfigValue::Block) {
                QMap<QString, BConfigValue> opts = optsValue.blockValue();
                for (auto it = opts.begin(); it != opts.end(); ++it) {
                    if (it.key().compare("Exclude", Qt::CaseInsensitive) == 0) {
                        // Nested exclude within Options
                        if (it.value().type() == BConfigValue::Block) {
                            QMap<QString, BConfigValue> exclBlock = it.value().blockValue();
                            if (exclBlock.contains("File")) {
                                BConfigValue exclFiles = exclBlock.value("File");
                                if (exclFiles.type() == BConfigValue::List) {
                                    block->excludeFilesModel->setItems(exclFiles.listValue());
                                } else if (exclFiles.type() == BConfigValue::Simple) {
                                    block->excludeFilesModel->setItems({exclFiles.simpleValue()});
                                }
                            }
                            if (exclBlock.contains("WildDir")) {
                                BConfigValue patterns = exclBlock.value("WildDir");
                                if (patterns.type() == BConfigValue::List) {
                                    block->excludePatternsModel->setItems(patterns.listValue());
                                } else if (patterns.type() == BConfigValue::Simple) {
                                    block->excludePatternsModel->setItems({patterns.simpleValue()});
                                }
                            }
                        }
                    } else {
                        // Regular option
                        if (it.value().type() == BConfigValue::Simple) {
                            block->options[it.key()] = it.value().simpleValue();
                        }
                    }
                }
            }
        }
    }

    // Load global Exclude
    BConfigValue excludeValue = resource.value("Exclude");
    if (excludeValue.type() == BConfigValue::Block) {
        QMap<QString, BConfigValue> exclBlock = excludeValue.blockValue();
        if (exclBlock.contains("File")) {
            BConfigValue fileValue = exclBlock.value("File");
            if (fileValue.type() == BConfigValue::List) {
                m_excludePathModel->setItems(fileValue.listValue());
            } else if (fileValue.type() == BConfigValue::Simple) {
                m_excludePathModel->setItems({fileValue.simpleValue()});
            }
        }
    }

    markClean();
    emit documentChanged();
}

void BFileSetDocument::loadFromJson(const QJsonObject &preset)
{
    clear();

    m_name = preset.value("name").toString();
    m_description = preset.value("description").toString();
    m_enableVss = preset.value("enableVss").toBool(true);
    m_ignoreFileSetChanges = preset.value("ignoreFileSetChanges").toBool(false);
    m_enableSnapshot = preset.value("enableSnapshot").toBool(false);

    // Load Include blocks
    QJsonArray includes = preset.value("include").toArray();
    for (const QJsonValue &inc : includes) {
        QJsonObject incObj = inc.toObject();
        IncludeBlock *block = addIncludeBlock();

        // Load paths
        QJsonArray files = incObj.value("files").toArray();
        QStringList paths;
        for (const QJsonValue &f : files) {
            paths.append(f.toString());
        }
        block->pathsModel->setItems(paths);

        // Load options
        QJsonObject opts = incObj.value("options").toObject();
        for (auto it = opts.begin(); it != opts.end(); ++it) {
            if (it.key() == "excludeFiles") {
                QStringList exclFiles;
                for (const QJsonValue &f : it.value().toArray()) {
                    exclFiles.append(f.toString());
                }
                block->excludeFilesModel->setItems(exclFiles);
            } else if (it.key() == "excludePatterns") {
                QStringList patterns;
                for (const QJsonValue &p : it.value().toArray()) {
                    patterns.append(p.toString());
                }
                block->excludePatternsModel->setItems(patterns);
            } else {
                block->options[it.key()] = it.value().toVariant();
            }
        }
    }

    // Load global exclude
    QJsonArray excludes = preset.value("exclude").toArray();
    QStringList exclPaths;
    for (const QJsonValue &excl : excludes) {
        exclPaths.append(excl.toString());
    }
    m_excludePathModel->setItems(exclPaths);

    markClean();
    emit documentChanged();
}

QString BFileSetDocument::toConfigText() const
{
    QString config;
    QTextStream out(&config);

    out << "FileSet {\n";
    out << "  Name = \"" << m_name << "\"\n";

    if (!m_description.isEmpty()) {
        out << "  Description = \"" << m_description << "\"\n";
    }

    if (m_enableVss) {
        out << "  Enable VSS = yes\n";
    }

    if (m_ignoreFileSetChanges) {
        out << "  Ignore FileSet Changes = yes\n";
    }

    if (m_enableSnapshot) {
        out << "  Enable Snapshot = yes\n";
    }

    // Include blocks
    for (const IncludeBlock *block : m_includeBlocks) {
        out << "  Include {\n";

        // Options first
        out << "    Options {\n";
        for (auto it = block->options.begin(); it != block->options.end(); ++it) {
            out << "      " << formatOption(it.key(), it.value()) << "\n";
        }

        // Nested exclude in Options
        if (block->excludeFilesModel->rowCount() > 0 ||
            block->excludePatternsModel->rowCount() > 0 ||
            block->excludeWildFileModel->rowCount() > 0) {
            out << "      Exclude {\n";
            for (const QString &f : block->excludeFilesModel->items()) {
                out << "        File = \"" << f << "\"\n";
            }
            for (const QString &p : block->excludePatternsModel->items()) {
                out << "        WildDir = \"" << p << "\"\n";
            }
            for (const QString &p : block->excludeWildFileModel->items()) {
                out << "        WildFile = \"" << p << "\"\n";
            }
            out << "      }\n";
        }
        out << "    }\n";

        // File entries
        for (const QString &path : block->pathsModel->items()) {
            out << "    File = \"" << path << "\"\n";
        }

        out << "  }\n";
    }

    // Global Exclude block
    if (m_excludePathModel->rowCount() > 0) {
        out << "  Exclude {\n";
        for (const QString &path : m_excludePathModel->items()) {
            out << "    File = \"" << path << "\"\n";
        }
        out << "  }\n";
    }

    out << "}\n";

    return config;
}

QString BFileSetDocument::toConfigureCommand() const
{
    QStringList parts;
    parts << "configure add fileset";
    parts << QString("name=\"%1\"").arg(m_name);

    if (!m_description.isEmpty()) {
        parts << QString("description=\"%1\"").arg(m_description);
    }

    if (m_enableVss) {
        parts << "enablevss=yes";
    }

    if (m_ignoreFileSetChanges) {
        parts << "ignorefilesetchanges=yes";
    }

    if (m_enableSnapshot) {
        parts << "enablesnapshot=yes";
    }

    // Include blocks
    for (const IncludeBlock *block : m_includeBlocks) {
        QStringList includeParts;
        includeParts << "include {";

        // Options
        QStringList optParts;
        optParts << "options {";

        for (auto it = block->options.begin(); it != block->options.end(); ++it) {
            QString key = it.key().toLower().replace(" ", "");
            QVariant val = it.value();

            if (val.typeId() == QMetaType::Bool) {
                optParts << QString("%1=%2").arg(key, val.toBool() ? "yes" : "no");
            } else {
                optParts << QString("%1=\"%2\"").arg(key, val.toString());
            }
        }

        // Nested exclude in Options
        if (block->excludeFilesModel->rowCount() > 0 ||
            block->excludePatternsModel->rowCount() > 0 ||
            block->excludeWildFileModel->rowCount() > 0) {

            QStringList exclParts;
            exclParts << "exclude {";

            for (const QString &f : block->excludeFilesModel->items()) {
                exclParts << QString("file=\"%1\"").arg(f);
            }
            for (const QString &p : block->excludePatternsModel->items()) {
                exclParts << QString("wilddir=\"%1\"").arg(p);
            }
            for (const QString &p : block->excludeWildFileModel->items()) {
                exclParts << QString("wildfile=\"%1\"").arg(p);
            }
            exclParts << "}";
            optParts << exclParts.join(" ");
        }

        optParts << "}";
        includeParts << optParts.join(" ");

        // File entries
        for (const QString &path : block->pathsModel->items()) {
            includeParts << QString("file=\"%1\"").arg(path);
        }

        includeParts << "}";
        parts << includeParts.join(" ");
    }

    // Global Exclude block
    if (m_excludePathModel->rowCount() > 0) {
        QStringList exclParts;
        exclParts << "exclude {";
        for (const QString &path : m_excludePathModel->items()) {
            exclParts << QString("file=\"%1\"").arg(path);
        }
        exclParts << "}";
        parts << exclParts.join(" ");
    }

    return parts.join(" ");
}

QJsonObject BFileSetDocument::toJson() const
{
    QJsonObject json;

    json["name"] = m_name;
    json["description"] = m_description;
    json["enableVss"] = m_enableVss;
    json["ignoreFileSetChanges"] = m_ignoreFileSetChanges;
    json["enableSnapshot"] = m_enableSnapshot;

    // Include blocks
    QJsonArray includes;
    for (const IncludeBlock *block : m_includeBlocks) {
        QJsonObject incObj;

        // Files
        QJsonArray files;
        for (const QString &path : block->pathsModel->items()) {
            files.append(path);
        }
        incObj["files"] = files;

        // Options
        QJsonObject opts;
        for (auto it = block->options.begin(); it != block->options.end(); ++it) {
            opts[it.key()] = QJsonValue::fromVariant(it.value());
        }

        // Nested excludes
        QJsonArray exclFiles;
        for (const QString &f : block->excludeFilesModel->items()) {
            exclFiles.append(f);
        }
        if (!exclFiles.isEmpty()) {
            opts["excludeFiles"] = exclFiles;
        }

        QJsonArray exclPatterns;
        for (const QString &p : block->excludePatternsModel->items()) {
            exclPatterns.append(p);
        }
        if (!exclPatterns.isEmpty()) {
            opts["excludePatterns"] = exclPatterns;
        }

        incObj["options"] = opts;
        includes.append(incObj);
    }
    json["include"] = includes;

    // Global exclude
    QJsonArray excludes;
    for (const QString &path : m_excludePathModel->items()) {
        excludes.append(path);
    }
    json["exclude"] = excludes;

    return json;
}

// ============================================================================
// Validation
// ============================================================================

BFileSetDocument::ValidationResult BFileSetDocument::validate() const
{
    ValidationResult result;
    result.valid = true;

    // Name is required
    if (m_name.isEmpty()) {
        result.valid = false;
        result.errors.append(tr("FileSet name is required"));
    }

    // At least one Include block with paths
    if (m_includeBlocks.isEmpty()) {
        result.valid = false;
        result.errors.append(tr("At least one Include block is required"));
    } else {
        bool hasAnyPath = false;
        for (int i = 0; i < m_includeBlocks.count(); ++i) {
            const IncludeBlock *block = m_includeBlocks.at(i);
            if (block->pathsModel->rowCount() > 0) {
                hasAnyPath = true;
            } else {
                result.warnings.append(tr("Include block %1 has no file paths").arg(i + 1));
            }
        }
        if (!hasAnyPath) {
            result.valid = false;
            result.errors.append(tr("At least one Include block must have file paths"));
        }
    }

    return result;
}

void BFileSetDocument::clear()
{
    m_name.clear();
    m_description.clear();
    m_enableVss = true;
    m_ignoreFileSetChanges = false;
    m_enableSnapshot = false;

    // Clear include blocks
    qDeleteAll(m_includeBlocks);
    m_includeBlocks.clear();

    // Clear global exclude
    m_excludePathModel->setItems(QStringList());

    // Clear undo stack
    m_undoStack->clear();

    m_nextBlockId = 1;

    emit documentChanged();
}

// ============================================================================
// Private Methods
// ============================================================================

void BFileSetDocument::connectBlockSignals(IncludeBlock *block)
{
    connect(block->pathsModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);
    connect(block->excludeFilesModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);
    connect(block->excludePatternsModel, &BEditableListModel::modelModified,
            this, &BFileSetDocument::onAnyModelChanged);
}

void BFileSetDocument::onAnyModelChanged()
{
    emit documentChanged();
}

QString BFileSetDocument::formatOption(const QString &key, const QVariant &value) const
{
    // Format option based on type
    if (value.typeId() == QMetaType::Bool) {
        return QString("%1 = %2").arg(key, value.toBool() ? "yes" : "no");
    } else if (value.typeId() == QMetaType::Int || value.typeId() == QMetaType::LongLong) {
        return QString("%1 = %2").arg(key).arg(value.toLongLong());
    } else {
        // String or other - quote it
        return QString("%1 = \"%2\"").arg(key, value.toString());
    }
}

void BFileSetDocument::initDefaultOptions(IncludeBlock *block)
{
    // Set common default options
    block->options["Signature"] = "SHA1";
    block->options["Compression"] = "LZ4";
    block->options["OneFS"] = true;
    block->options["AclSupport"] = true;
    block->options["XattrSupport"] = true;
}
