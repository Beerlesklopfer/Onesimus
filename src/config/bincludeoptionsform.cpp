/**
 * @file bincludeoptionsform.cpp
 * @brief Schema-driven form for FileSet Include Options
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */

#include "config/bincludeoptionsform.h"
#include "config/beditablelistwidget.h"
#include "config/beditablelistmodel.h"
#include "blogging.h"
#include <QVBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QGroupBox>

// Static schema storage
QJsonObject BIncludeOptionsForm::s_schema;

BIncludeOptionsForm::BIncludeOptionsForm(Mode mode, QWidget *parent)
    : QWidget(parent)
    , m_mode(mode)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    mainLayout->addWidget(m_scrollArea);

    buildForm();
}

bool BIncludeOptionsForm::shouldShowOption(const QString &name, const QJsonObject &def) const
{
    Q_UNUSED(def)

    if (m_mode == IncludeMode) {
        // Show all options in Include mode
        return true;
    }

    // Exclude mode - only show options relevant to Exclude blocks
    // Exclude blocks typically support: wild, wildfile, wilddir, regex
    static QStringList excludeOptions = {
        "wild", "wildfile", "wilddir", "regex"
    };

    return excludeOptions.contains(name.toLower());
}

QJsonObject BIncludeOptionsForm::loadSchema()
{
    if (!s_schema.isEmpty()) {
        return s_schema;
    }

    QFile file(":/directives/directives/fileset.json");
    if (!file.open(QIODevice::ReadOnly)) {
        BLOG_WARNING() << "Failed to load fileset.json schema";
        return QJsonObject();
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        BLOG_WARNING() << "Failed to parse fileset.json:" << error.errorString();
        return QJsonObject();
    }

    s_schema = doc.object()["include_options"].toObject();
    return s_schema;
}

void BIncludeOptionsForm::buildForm()
{
    QWidget *formWidget = new QWidget(this);
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);
    formLayout->setContentsMargins(4, 4, 4, 4);

    QJsonObject schema = loadSchema();
    if (schema.isEmpty()) {
        QLabel *errorLabel = new QLabel(tr("Failed to load include options schema"), formWidget);
        errorLabel->setStyleSheet("color: red;");
        formLayout->addWidget(errorLabel);
        m_scrollArea->setWidget(formWidget);
        return;
    }

    // Group options by category
    // - Signature & Compression (string types with values)
    // - Filesystem behavior (general booleans)
    // - Time options (atime-related)
    // - Platform-specific (unix/windows)

    QMap<QString, QList<QString>> groups;
    groups["Signature & Compression"] = QStringList();
    groups["Filesystem"] = QStringList();
    groups["Time & Integrity"] = QStringList();
    groups["Platform-specific"] = QStringList();

    for (auto it = schema.begin(); it != schema.end(); ++it) {
        QString name = it.key();
        QJsonObject def = it.value().toObject();
        m_optionDefs[name] = def;

        // Skip options not relevant for current mode
        if (!shouldShowOption(name, def)) {
            continue;
        }

        QString type = def["type"].toString();
        QString platform = def["platform"].toString();

        if (type == "string" && def.contains("values")) {
            groups["Signature & Compression"].append(name);
        } else if (!platform.isEmpty() || type == "string_list") {
            // Platform-specific options and list types (like fstype)
            groups["Platform-specific"].append(name);
        } else if (name.contains("atime", Qt::CaseInsensitive) ||
                   name.contains("mtime", Qt::CaseInsensitive) ||
                   name == "checkfilechanges") {
            groups["Time & Integrity"].append(name);
        } else {
            groups["Filesystem"].append(name);
        }
    }

    // Create group boxes
    QStringList groupOrder = {"Signature & Compression", "Filesystem", "Time & Integrity", "Platform-specific"};

    for (const QString &groupName : groupOrder) {
        const QList<QString> &options = groups[groupName];
        if (options.isEmpty()) continue;

        QGroupBox *groupBox = new QGroupBox(tr(groupName.toUtf8().constData()), formWidget);
        QGridLayout *grid = new QGridLayout(groupBox);
        grid->setSpacing(6);

        int row = 0, col = 0;
        int maxCols = (groupName == "Signature & Compression") ? 2 : 3;

        for (const QString &optName : options) {
            QJsonObject def = m_optionDefs[optName];
            QWidget *widget = createWidgetForOption(optName, def);
            if (!widget) continue;

            m_widgets[optName] = widget;
            connectWidgetSignals(optName, widget);

            // For combo boxes and list widgets, add with label spanning full width
            if (qobject_cast<QComboBox*>(widget)) {
                QString labelText = optName.left(1).toUpper() + optName.mid(1) + ":";
                QLabel *label = new QLabel(labelText, groupBox);
                label->setToolTip(def["description"].toString());

                // Combo box: label and widget side by side
                QHBoxLayout *hbox = new QHBoxLayout();
                hbox->addWidget(label);
                hbox->addWidget(widget);
                hbox->addStretch();
                grid->addLayout(hbox, row, col, 1, maxCols);
                row++;
                col = 0;
            } else if (qobject_cast<BEditableListWidget*>(widget)) {
                // List widget already has title in compact mode, add directly
                if (col > 0) { row++; col = 0; }  // Start new row
                grid->addWidget(widget, row, 0, 1, maxCols);
                row++;
            } else if (qobject_cast<QLineEdit*>(widget) || qobject_cast<QSpinBox*>(widget)) {
                // Line edit or spin box: label and widget side by side
                QString labelText = optName.left(1).toUpper() + optName.mid(1) + ":";
                QLabel *label = new QLabel(labelText, groupBox);
                label->setToolTip(def["description"].toString());

                if (col > 0) { row++; col = 0; }  // Start new row
                QHBoxLayout *hbox = new QHBoxLayout();
                hbox->addWidget(label);
                hbox->addWidget(widget, 1);
                grid->addLayout(hbox, row, 0, 1, maxCols);
                row++;
            } else {
                grid->addWidget(widget, row, col);
                col++;
                if (col >= maxCols) {
                    col = 0;
                    row++;
                }
            }
        }

        formLayout->addWidget(groupBox);
    }

    formLayout->addStretch();
    m_scrollArea->setWidget(formWidget);
}

QWidget *BIncludeOptionsForm::createWidgetForOption(const QString &name, const QJsonObject &def)
{
    QString type = def["type"].toString();
    QString description = def["description"].toString();
    QString platform = def["platform"].toString();

    // Build display name from option name
    QString displayName = name;
    // Convert camelCase to readable: "onefs" -> "One FS", "noatime" -> "No Atime"
    if (name == "onefs") displayName = "One FS";
    else if (name == "noatime") displayName = "No Atime";
    else if (name == "keepatime") displayName = "Keep Atime";
    else if (name == "mtimeonly") displayName = "Mtime Only";
    else if (name == "readfifo") displayName = "Read FIFO";
    else if (name == "hardlinks") displayName = "Hard Links";
    else if (name == "xattr") displayName = "XAttr Support";
    else if (name == "acl") displayName = "ACL Support";
    else if (name == "vss") displayName = "VSS";
    else if (name == "checkfilechanges") displayName = "Check File Changes";
    else if (name == "honornodumpflag") displayName = "Honor No Dump Flag";
    else displayName = name.left(1).toUpper() + name.mid(1);

    // Add platform indicator
    if (platform == "unix") {
        displayName += " (Unix)";
    } else if (platform == "windows") {
        displayName += " (Windows)";
    }

    if (type == "boolean") {
        QCheckBox *check = new QCheckBox(displayName);
        check->setToolTip(description);
        check->setChecked(def["default"].toBool(false));
        return check;
    } else if (type == "string" && def.contains("values")) {
        QComboBox *combo = new QComboBox();
        combo->setToolTip(description);

        // Add "None" option for optional fields like compression
        if (name == "compression") {
            combo->addItem("None");
        }

        QJsonArray values = def["values"].toArray();
        for (const QJsonValue &v : values) {
            combo->addItem(v.toString());
        }

        // Set default
        QString defaultVal = def["default"].toString();
        if (!defaultVal.isEmpty()) {
            int idx = combo->findText(defaultVal);
            if (idx >= 0) {
                combo->setCurrentIndex(idx);
            }
        }
        return combo;
    } else if (type == "string_list") {
        // List of strings (e.g., fstype)
        BEditableListWidget *listWidget = new BEditableListWidget();
        listWidget->setToolTip(description);
        listWidget->setTitle(displayName);
        listWidget->setCompactMode(true);  // Compact mode with +/- in title
        listWidget->setMaximumHeight(100);

        // Set placeholder based on example if available
        if (def.contains("example")) {
            QJsonArray examples = def["example"].toArray();
            if (!examples.isEmpty()) {
                listWidget->setPlaceholderText(examples.first().toString());
            }
        }

        return listWidget;
    } else if (type == "string") {
        // Plain string (e.g., regex, wild, exclude_dir_containing)
        QLineEdit *edit = new QLineEdit();
        edit->setToolTip(description);
        if (def.contains("default")) {
            edit->setText(def["default"].toString());
        }
        edit->setPlaceholderText(description);
        return edit;
    } else if (type == "integer") {
        // Integer (e.g., strip_path)
        QSpinBox *spin = new QSpinBox();
        spin->setToolTip(description);
        spin->setMinimum(0);
        spin->setMaximum(999);
        if (def.contains("default")) {
            spin->setValue(def["default"].toInt(0));
        }
        return spin;
    }

    return nullptr;
}

void BIncludeOptionsForm::connectWidgetSignals(const QString &name, QWidget *widget)
{
    if (auto *check = qobject_cast<QCheckBox*>(widget)) {
        connect(check, &QCheckBox::toggled, this, [this, name](bool checked) {
            emit optionChanged(name, checked);
            emit valueChanged();
        });
    } else if (auto *combo = qobject_cast<QComboBox*>(widget)) {
        connect(combo, &QComboBox::currentTextChanged, this, [this, name](const QString &text) {
            emit optionChanged(name, text);
            emit valueChanged();
        });
    } else if (auto *listWidget = qobject_cast<BEditableListWidget*>(widget)) {
        connect(listWidget->model(), &BEditableListModel::modelModified, this, [this, name, listWidget]() {
            emit optionChanged(name, listWidget->model()->items());
            emit valueChanged();
        });
    } else if (auto *edit = qobject_cast<QLineEdit*>(widget)) {
        connect(edit, &QLineEdit::textChanged, this, [this, name](const QString &text) {
            emit optionChanged(name, text);
            emit valueChanged();
        });
    } else if (auto *spin = qobject_cast<QSpinBox*>(widget)) {
        connect(spin, &QSpinBox::valueChanged, this, [this, name](int value) {
            emit optionChanged(name, value);
            emit valueChanged();
        });
    }
}

QVariant BIncludeOptionsForm::optionValue(const QString &name) const
{
    QWidget *widget = m_widgets.value(name);
    if (!widget) return QVariant();

    if (auto *check = qobject_cast<QCheckBox*>(widget)) {
        return check->isChecked();
    } else if (auto *combo = qobject_cast<QComboBox*>(widget)) {
        return combo->currentText();
    } else if (auto *listWidget = qobject_cast<BEditableListWidget*>(widget)) {
        return listWidget->model()->items();
    } else if (auto *edit = qobject_cast<QLineEdit*>(widget)) {
        return edit->text();
    } else if (auto *spin = qobject_cast<QSpinBox*>(widget)) {
        return spin->value();
    }
    return QVariant();
}

void BIncludeOptionsForm::setOptionValue(const QString &name, const QVariant &value)
{
    QWidget *widget = m_widgets.value(name);
    if (!widget) return;

    if (auto *check = qobject_cast<QCheckBox*>(widget)) {
        check->setChecked(value.toBool());
    } else if (auto *combo = qobject_cast<QComboBox*>(widget)) {
        int idx = combo->findText(value.toString());
        if (idx >= 0) {
            combo->setCurrentIndex(idx);
        } else {
            combo->setCurrentText(value.toString());
        }
    } else if (auto *listWidget = qobject_cast<BEditableListWidget*>(widget)) {
        listWidget->model()->setItems(value.toStringList());
    } else if (auto *edit = qobject_cast<QLineEdit*>(widget)) {
        edit->setText(value.toString());
    } else if (auto *spin = qobject_cast<QSpinBox*>(widget)) {
        spin->setValue(value.toInt());
    }
}

QMap<QString, QVariant> BIncludeOptionsForm::allValues() const
{
    QMap<QString, QVariant> values;
    for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        values[it.key()] = optionValue(it.key());
    }
    return values;
}

void BIncludeOptionsForm::setValues(const QMap<QString, QVariant> &values)
{
    // Defensive copy: the caller may pass a reference to block->options which
    // gets replaced by the signal chain (setOptionValue → widget change →
    // valueChanged → onOptionChanged → saveOptionsToBlock). Without a copy,
    // the iterator would be invalidated mid-loop causing a crash.
    const QMap<QString, QVariant> localCopy = values;

    // Block signals to prevent the feedback loop during bulk loading.
    // Each setOptionValue triggers widget signals → valueChanged → save,
    // which is wasteful during loading and was the root cause of the crash.
    blockSignals(true);

    for (auto it = localCopy.constBegin(); it != localCopy.constEnd(); ++it) {
        setOptionValue(it.key(), it.value());
    }

    blockSignals(false);
    // No valueChanged() emit here — we're loading data, not user-editing.
}

void BIncludeOptionsForm::resetToDefaults()
{
    for (auto it = m_optionDefs.begin(); it != m_optionDefs.end(); ++it) {
        QString name = it.key();
        QJsonObject def = it.value();

        if (def["type"].toString() == "boolean") {
            setOptionValue(name, def["default"].toBool(false));
        } else if (def.contains("default")) {
            setOptionValue(name, def["default"].toString());
        }
    }
}

QString BIncludeOptionsForm::directiveName(const QString &optionName)
{
    // Map internal lowercase names to Bareos directive names.
    // Includes both schema keys ("xattr") and Bareos JSON keys ("xattrsupport").
    static QMap<QString, QString> nameMap = {
        {"signature", "Signature"},
        {"compression", "Compression"},
        {"onefs", "OneFS"},
        {"xattr", "XAttr Support"},
        {"xattrsupport", "XAttr Support"},
        {"acl", "ACL Support"},
        {"aclsupport", "ACL Support"},
        {"vss", "VSS"},
        {"portable", "Portable"},
        {"recurse", "Recurse"},
        {"sparse", "Sparse"},
        {"hardlinks", "Hardlinks"},
        {"readfifo", "Read Fifo"},
        {"noatime", "No Atime"},
        {"keepatime", "Keep Atime"},
        {"mtimeonly", "Mtime Only"},
        {"checkfilechanges", "Check File Changes"},
        {"honornodumpflag", "Honor NoDump Flag"},
        {"fstype", "FS Type"},
        {"drivetype", "Drive Type"},
        {"exclude_dir_containing", "Exclude Dir Containing"},
        {"strip_path", "Strip Path"},
        {"regex", "Regex"},
        {"regexdir", "RegexDir"},
        {"regexfile", "RegexFile"},
        {"wild", "Wild"},
        {"wildfile", "WildFile"},
        {"wilddir", "WildDir"}
    };

    return nameMap.value(optionName.toLower(), optionName);
}

QString BIncludeOptionsForm::toConfigText(const QString &indent,
                                          const QStringList &excludeFiles,
                                          const QStringList &excludePatterns) const
{
    QString config;
    QTextStream out(&config);

    QMap<QString, QVariant> values = allValues();

    for (auto it = values.begin(); it != values.end(); ++it) {
        QString name = it.key();
        QVariant value = it.value();

        // Skip "None" compression (means no compression directive)
        if (name == "compression" && value.toString() == "None") {
            continue;
        }

        // Skip default values to keep config minimal
        QJsonObject def = m_optionDefs.value(name);
        if (!def.isEmpty()) {
            QString type = def["type"].toString();
            if (type == "boolean") {
                bool defaultVal = def["default"].toBool(false);
                if (value.toBool() == defaultVal) {
                    continue;  // Skip if same as default
                }
            } else if (type == "string") {
                QString defaultVal = def["default"].toString();
                if (value.toString() == defaultVal || value.toString().isEmpty()) {
                    continue;  // Skip if same as default or empty
                }
            } else if (type == "integer") {
                int defaultVal = def["default"].toInt(0);
                if (value.toInt() == defaultVal) {
                    continue;  // Skip if same as default
                }
            } else if (type == "string_list") {
                QStringList list = value.toStringList();
                if (list.isEmpty()) {
                    continue;  // Skip if empty
                }
            }
        }

        QString directive = directiveName(name);

        if (value.typeId() == QMetaType::Bool) {
            out << indent << directive << " = " << (value.toBool() ? "yes" : "no") << "\n";
        } else if (value.typeId() == QMetaType::QStringList) {
            // Output each item as separate directive
            QStringList list = value.toStringList();
            for (const QString &item : list) {
                out << indent << directive << " = " << item << "\n";
            }
        } else if (value.typeId() == QMetaType::Int) {
            out << indent << directive << " = " << value.toInt() << "\n";
        } else {
            out << indent << directive << " = " << value.toString() << "\n";
        }
    }

    // Output nested Exclude block if there are excludes
    if (!excludeFiles.isEmpty() || !excludePatterns.isEmpty()) {
        out << indent << "Exclude {\n";
        for (const QString &file : excludeFiles) {
            out << indent << "  File = \"" << file << "\"\n";
        }
        for (const QString &pattern : excludePatterns) {
            out << indent << "  WildDir = \"" << pattern << "\"\n";
        }
        out << indent << "}\n";
    }

    return config;
}
