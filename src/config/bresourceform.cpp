/**
 * @file bresourceform.cpp
 * @brief Reusable schema-driven form widget for Bareos/Bacula resources
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */

#include "config/bresourceform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFont>
#include <QStyle>
#include <algorithm>

BResourceForm::BResourceForm(const QString &resourceType, QWidget *parent)
    : QWidget(parent)
    , m_resourceType(resourceType)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Scroll area for the form
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    mainLayout->addWidget(m_scrollArea);

    buildForm();
}

void BResourceForm::setGroupFilter(const QStringList &groups)
{
    if (m_groupFilter != groups) {
        m_groupFilter = groups;
        // Clear existing form
        m_fieldWidgets.clear();
        m_fieldLabels.clear();
        m_fieldDirectives.clear();
        m_advancedWidgets.clear();
        m_advancedLabels.clear();
        m_hasAdvancedDirectives = false;
        // Rebuild
        buildForm();
    }
}

void BResourceForm::setExistingResource(const BConfigResource &existing)
{
    m_existing = existing;
    // Rebuild form with existing values
    m_fieldWidgets.clear();
    m_fieldLabels.clear();
    m_fieldDirectives.clear();
    m_advancedWidgets.clear();
    m_advancedLabels.clear();
    m_hasAdvancedDirectives = false;
    buildForm();
}

BConfigResource BResourceForm::resource() const
{
    return m_resource;
}

void BResourceForm::setAdvancedVisible(bool visible)
{
    for (QWidget *w : m_advancedWidgets) {
        w->setVisible(visible);
    }
    for (QLabel *l : m_advancedLabels) {
        l->setVisible(visible);
    }
}

QVariant BResourceForm::directiveValue(const QString &name) const
{
    QWidget *widget = m_fieldWidgets.value(name);
    if (!widget) return QVariant();

    // Check for container widgets with embedded field widget
    QWidget *actualWidget = widget->property("fieldWidget").value<QWidget*>();
    if (actualWidget) {
        widget = actualWidget;
    }

    if (auto *lineEdit = qobject_cast<QLineEdit*>(widget)) {
        return lineEdit->text();
    } else if (auto *spinBox = qobject_cast<QSpinBox*>(widget)) {
        return spinBox->value();
    } else if (auto *checkBox = qobject_cast<QCheckBox*>(widget)) {
        return checkBox->isChecked();
    } else if (auto *comboBox = qobject_cast<QComboBox*>(widget)) {
        return comboBox->currentText();
    } else if (auto *textEdit = qobject_cast<QTextEdit*>(widget)) {
        return textEdit->toPlainText();
    }

    return QVariant();
}

void BResourceForm::setDirectiveValue(const QString &name, const QVariant &value)
{
    QWidget *widget = m_fieldWidgets.value(name);
    if (!widget) return;

    // Check for container widgets with embedded field widget
    QWidget *actualWidget = widget->property("fieldWidget").value<QWidget*>();
    if (actualWidget) {
        widget = actualWidget;
    }

    if (auto *lineEdit = qobject_cast<QLineEdit*>(widget)) {
        lineEdit->setText(value.toString());
    } else if (auto *spinBox = qobject_cast<QSpinBox*>(widget)) {
        spinBox->setValue(value.toInt());
    } else if (auto *checkBox = qobject_cast<QCheckBox*>(widget)) {
        checkBox->setChecked(value.toBool());
    } else if (auto *comboBox = qobject_cast<QComboBox*>(widget)) {
        comboBox->setCurrentText(value.toString());
    } else if (auto *textEdit = qobject_cast<QTextEdit*>(widget)) {
        textEdit->setPlainText(value.toString());
    }
}

void BResourceForm::buildForm()
{
    QWidget *formWidget = new QWidget(this);
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);
    formLayout->setContentsMargins(4, 4, 4, 4);

    // Ensure schema is loaded, then get directives
    BDirectiveSchema::instance().loadSchemas();
    QMap<QString, BDirective> directives = BDirectiveSchema::instance().directives(m_resourceType);

    if (directives.isEmpty()) {
        QLabel *noSchema = new QLabel(tr("No schema available for resource type '%1'").arg(m_resourceType), formWidget);
        noSchema->setStyleSheet("color: gray; font-style: italic;");
        formLayout->addWidget(noSchema);
        m_scrollArea->setWidget(formWidget);
        return;
    }

    // Sort directives: required first, then by group, then alphabetically
    QList<BDirective> sortedDirectives = directives.values();
    std::sort(sortedDirectives.begin(), sortedDirectives.end(),
              [](const BDirective &a, const BDirective &b) {
                  if (a.required != b.required) return a.required > b.required;
                  if (a.use != b.use) return a.use > b.use;
                  if (a.group != b.group) return a.group < b.group;
                  return a.name < b.name;
              });

    // Group directives by group field, filtering by platform
    QMap<QString, QList<BDirective>> groups;
    for (const BDirective &dir : sortedDirectives) {
        // Skip directives that don't apply to current platform
        if (!dir.appliesToCurrentPlatform()) {
            continue;
        }
        QString groupName = dir.group.isEmpty() ? tr("General") : dir.group;

        // Apply group filter if set
        if (!m_groupFilter.isEmpty() && !m_groupFilter.contains(groupName, Qt::CaseInsensitive)) {
            continue;
        }

        groups[groupName].append(dir);
    }

    // Ensure "General" group comes first
    QStringList groupOrder;
    if (groups.contains(tr("General"))) {
        groupOrder.append(tr("General"));
    }
    for (const QString &key : groups.keys()) {
        if (key != tr("General") && !groupOrder.contains(key)) {
            groupOrder.append(key);
        }
    }

    for (const QString &groupName : groupOrder) {
        const QList<BDirective> &groupDirectives = groups[groupName];

        QGroupBox *groupBox = new QGroupBox(groupName, formWidget);
        QFormLayout *form = new QFormLayout(groupBox);
        form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
        form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

        bool groupHasVisibleDirectives = false;
        bool groupIsAdvancedOnly = true;

        for (const BDirective &dir : groupDirectives) {
            // Skip directives that don't apply to current platform
            if (!dir.appliesToCurrentPlatform()) {
                continue;
            }

            // Get current value from existing resource
            BConfigValue currentValue;
            if (!m_existing.type().isEmpty()) {
                // Try exact match first, then case-insensitive
                if (m_existing.hasKey(dir.name)) {
                    currentValue = m_existing.value(dir.name);
                } else {
                    for (const QString &key : m_existing.keys()) {
                        if (key.compare(dir.name, Qt::CaseInsensitive) == 0) {
                            currentValue = m_existing.value(key);
                            break;
                        }
                    }
                }
            }

            // Create label
            QString labelText = dir.name;
            if (dir.required) {
                labelText = QString("<b>%1</b> *").arg(dir.name);
            }
            QLabel *label = new QLabel(labelText, groupBox);
            label->setTextFormat(Qt::RichText);
            if (!dir.description.isEmpty()) {
                label->setToolTip(dir.description);
            }

            // Create widget
            QWidget *widget = createWidgetForDirective(dir, currentValue);
            if (!widget) continue;

            // Store field widget and directive definition
            m_fieldWidgets[dir.name] = widget;
            m_fieldLabels[dir.name] = label;
            m_fieldDirectives[dir.name] = dir;

            // Connect signals for value changes
            connectFieldSignals(dir.name, widget);

            // Handle advanced (use=false) directives
            if (!dir.use) {
                m_advancedWidgets.append(widget);
                m_advancedLabels.append(label);
                widget->setVisible(false);
                label->setVisible(false);
                m_hasAdvancedDirectives = true;
            } else {
                groupHasVisibleDirectives = true;
                groupIsAdvancedOnly = false;
            }

            form->addRow(label, widget);
        }

        // If all directives in group are advanced, hide the group box too
        if (groupIsAdvancedOnly && !groupHasVisibleDirectives) {
            m_advancedWidgets.append(groupBox);
            groupBox->setVisible(false);
        }

        formLayout->addWidget(groupBox);
    }

    formLayout->addStretch();
    m_scrollArea->setWidget(formWidget);

    // Set up conditional visibility connections
    setupConditionalVisibility();
}

QWidget *BResourceForm::createWidgetForDirective(const BDirective &directive,
                                                  const BConfigValue &currentValue)
{
    QString currentStr = currentValue.isEmpty() ? QString() : currentValue.simpleValue();

    if (directive.type == "string") {
        // String with valid values → combo box
        if (!directive.validValues.isEmpty()) {
            QComboBox *combo = new QComboBox();
            combo->setEditable(true);
            combo->addItem(QString()); // empty option
            combo->addItems(directive.validValues);
            if (!currentStr.isEmpty()) {
                int idx = combo->findText(currentStr, Qt::MatchFixedString);
                if (idx >= 0) {
                    combo->setCurrentIndex(idx);
                } else {
                    combo->setCurrentText(currentStr);
                }
            }
            if (!directive.example.isEmpty()) {
                combo->lineEdit()->setPlaceholderText(directive.example);
            }
            combo->setToolTip(directive.description);
            return combo;
        }

        // Plain string
        QLineEdit *edit = new QLineEdit();
        if (!currentStr.isEmpty()) {
            edit->setText(currentStr);
        }
        if (!directive.example.isEmpty()) {
            edit->setPlaceholderText(directive.example);
        } else if (directive.defaultValue.isValid()) {
            edit->setPlaceholderText(directive.defaultValue.toString());
        }
        edit->setToolTip(directive.description);
        return edit;
    }

    if (directive.type == "integer") {
        QSpinBox *spin = new QSpinBox();
        spin->setMinimum(directive.minValue);
        spin->setMaximum(directive.maxValue > 0 ? directive.maxValue : 999999);
        if (directive.defaultValue.isValid()) {
            spin->setValue(directive.defaultValue.toInt());
        }
        if (!currentStr.isEmpty()) {
            bool ok;
            int val = currentStr.toInt(&ok);
            if (ok) spin->setValue(val);
        }
        spin->setToolTip(directive.description);
        return spin;
    }

    if (directive.type == "boolean") {
        QCheckBox *check = new QCheckBox();
        if (directive.defaultValue.isValid()) {
            check->setChecked(directive.defaultValue.toBool());
        }
        if (!currentStr.isEmpty()) {
            QString lower = currentStr.toLower();
            check->setChecked(lower == "yes" || lower == "true" || lower == "1");
        }
        check->setToolTip(directive.description);
        return check;
    }

    if (directive.type == "password") {
        QWidget *container = new QWidget();
        QHBoxLayout *hbox = new QHBoxLayout(container);
        hbox->setContentsMargins(0, 0, 0, 0);

        QLineEdit *edit = new QLineEdit();
        edit->setEchoMode(QLineEdit::Password);
        if (!currentStr.isEmpty()) {
            edit->setText(currentStr);
        }
        edit->setPlaceholderText(tr("Password"));
        edit->setToolTip(directive.description);
        hbox->addWidget(edit);

        QPushButton *toggleBtn = new QPushButton();
        toggleBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
        toggleBtn->setFixedWidth(30);
        toggleBtn->setCheckable(true);
        toggleBtn->setToolTip(tr("Show/Hide password"));
        connect(toggleBtn, &QPushButton::toggled, edit, [edit](bool checked) {
            edit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
        });
        hbox->addWidget(toggleBtn);

        // Store the line edit as the actual field widget
        container->setProperty("fieldWidget", QVariant::fromValue<QWidget*>(edit));
        return container;
    }

    if (directive.type == "file" || directive.type == "directory") {
        QWidget *container = new QWidget();
        QHBoxLayout *hbox = new QHBoxLayout(container);
        hbox->setContentsMargins(0, 0, 0, 0);

        QLineEdit *edit = new QLineEdit();
        if (!currentStr.isEmpty()) {
            edit->setText(currentStr);
        }
        if (!directive.example.isEmpty()) {
            edit->setPlaceholderText(directive.example);
        }
        edit->setToolTip(directive.description);
        hbox->addWidget(edit);

        QPushButton *browseBtn = new QPushButton(tr("..."));
        browseBtn->setFixedWidth(30);
        browseBtn->setToolTip(directive.type == "file" ? tr("Browse for file") : tr("Browse for directory"));
        connect(browseBtn, &QPushButton::clicked, edit, [edit, directive, this]() {
            QString path;
            if (directive.type == "file") {
                path = QFileDialog::getOpenFileName(this->window(), tr("Select File"), edit->text());
            } else {
                path = QFileDialog::getExistingDirectory(this->window(), tr("Select Directory"), edit->text());
            }
            if (!path.isEmpty()) {
                edit->setText(path);
            }
        });
        hbox->addWidget(browseBtn);

        container->setProperty("fieldWidget", QVariant::fromValue<QWidget*>(edit));
        return container;
    }

    if (directive.type == "resource_reference") {
        QComboBox *combo = new QComboBox();
        combo->setEditable(true);
        combo->addItem(QString()); // empty option
        if (!currentStr.isEmpty()) {
            combo->addItem(currentStr);
            combo->setCurrentText(currentStr);
        }
        if (!directive.referenceType.isEmpty()) {
            combo->lineEdit()->setPlaceholderText(tr("<%1 name>").arg(directive.referenceType));
        }
        combo->setToolTip(directive.description);
        return combo;
    }

    if (directive.type == "time" || directive.type == "size") {
        QLineEdit *edit = new QLineEdit();
        if (!currentStr.isEmpty()) {
            edit->setText(currentStr);
        }
        if (!directive.example.isEmpty()) {
            edit->setPlaceholderText(directive.example);
        } else if (directive.defaultValue.isValid()) {
            edit->setPlaceholderText(directive.defaultValue.toString());
        }
        edit->setToolTip(directive.description);
        return edit;
    }

    if (directive.type == "string_list" || directive.type == "addresses" || directive.type == "block") {
        QTextEdit *textEdit = new QTextEdit();
        textEdit->setMaximumHeight(80);
        if (!currentStr.isEmpty()) {
            textEdit->setPlainText(currentStr);
        } else if (currentValue.type() == BConfigValue::List) {
            textEdit->setPlainText(currentValue.listValue().join("\n"));
        }
        if (!directive.example.isEmpty()) {
            textEdit->setPlaceholderText(directive.example);
        }
        textEdit->setToolTip(directive.description);
        return textEdit;
    }

    // Fallback: plain QLineEdit for unknown types
    QLineEdit *edit = new QLineEdit();
    if (!currentStr.isEmpty()) {
        edit->setText(currentStr);
    }
    if (!directive.example.isEmpty()) {
        edit->setPlaceholderText(directive.example);
    }
    edit->setToolTip(directive.description);
    return edit;
}

void BResourceForm::connectFieldSignals(const QString &name, QWidget *widget)
{
    // Check for container widgets with embedded field widget
    QWidget *actualWidget = widget->property("fieldWidget").value<QWidget*>();
    if (actualWidget) {
        widget = actualWidget;
    }

    if (auto *lineEdit = qobject_cast<QLineEdit*>(widget)) {
        connect(lineEdit, &QLineEdit::textChanged, this, [this, name](const QString &text) {
            emit directiveValueChanged(name, text);
            emit valueChanged();
        });
    } else if (auto *spinBox = qobject_cast<QSpinBox*>(widget)) {
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, name](int value) {
            emit directiveValueChanged(name, value);
            emit valueChanged();
        });
    } else if (auto *checkBox = qobject_cast<QCheckBox*>(widget)) {
        connect(checkBox, &QCheckBox::toggled, this, [this, name](bool checked) {
            emit directiveValueChanged(name, checked);
            emit valueChanged();
        });
    } else if (auto *comboBox = qobject_cast<QComboBox*>(widget)) {
        connect(comboBox, &QComboBox::currentTextChanged, this, [this, name](const QString &text) {
            emit directiveValueChanged(name, text);
            emit valueChanged();
        });
    } else if (auto *textEdit = qobject_cast<QTextEdit*>(widget)) {
        connect(textEdit, &QTextEdit::textChanged, this, [this, name, textEdit]() {
            emit directiveValueChanged(name, textEdit->toPlainText());
            emit valueChanged();
        });
    }
}

void BResourceForm::collectValues()
{
    m_resource = BConfigResource(m_resourceType);

    // Preserve source file from existing resource
    if (!m_existing.type().isEmpty()) {
        m_resource.setSourceFile(m_existing.sourceFile());
    }

    for (auto it = m_fieldWidgets.begin(); it != m_fieldWidgets.end(); ++it) {
        const QString &name = it.key();
        QWidget *widget = it.value();

        // Check for container widgets with embedded field widget
        QWidget *actualWidget = widget->property("fieldWidget").value<QWidget*>();
        if (actualWidget) {
            widget = actualWidget;
        }

        QString value;

        if (auto *lineEdit = qobject_cast<QLineEdit*>(widget)) {
            value = lineEdit->text().trimmed();
        } else if (auto *spinBox = qobject_cast<QSpinBox*>(widget)) {
            value = QString::number(spinBox->value());
        } else if (auto *checkBox = qobject_cast<QCheckBox*>(widget)) {
            value = checkBox->isChecked() ? "yes" : "no";
        } else if (auto *comboBox = qobject_cast<QComboBox*>(widget)) {
            value = comboBox->currentText().trimmed();
        } else if (auto *textEdit = qobject_cast<QTextEdit*>(widget)) {
            value = textEdit->toPlainText().trimmed();
        }

        // Skip empty values
        if (value.isEmpty()) continue;

        // For boolean: skip if it matches the default
        BDirective dir = BDirectiveSchema::instance().directive(m_resourceType, name);
        if (dir.type == "boolean" && dir.defaultValue.isValid()) {
            QString defaultStr = dir.defaultValue.toBool() ? "yes" : "no";
            if (value == defaultStr && !m_existing.hasKey(name)) {
                continue;
            }
        }

        m_resource.setValue(name, BConfigValue(value));
    }

    // Set name from the Name directive
    QString nameValue = m_resource.simpleValue("Name");
    if (!nameValue.isEmpty()) {
        m_resource.setName(nameValue);
    }
}

void BResourceForm::setupConditionalVisibility()
{
    // Find all controlling directives and connect their signals
    QSet<QString> controllingDirectives;

    for (auto it = m_fieldDirectives.begin(); it != m_fieldDirectives.end(); ++it) {
        const BDirective &dir = it.value();
        if (dir.visibleWhen.isValid()) {
            controllingDirectives.insert(dir.visibleWhen.directive);
        }
    }

    // Connect signals from controlling widgets
    for (const QString &ctrlName : controllingDirectives) {
        QWidget *widget = m_fieldWidgets.value(ctrlName);
        if (!widget) continue;

        // Check for container widgets with embedded field widget
        QWidget *actualWidget = widget->property("fieldWidget").value<QWidget*>();
        if (actualWidget) {
            widget = actualWidget;
        }

        // Connect based on widget type
        if (auto *checkBox = qobject_cast<QCheckBox*>(widget)) {
            connect(checkBox, &QCheckBox::toggled, this, [this, ctrlName]() {
                updateDependentFields(ctrlName);
            });
        } else if (auto *comboBox = qobject_cast<QComboBox*>(widget)) {
            connect(comboBox, &QComboBox::currentTextChanged, this, [this, ctrlName]() {
                updateDependentFields(ctrlName);
            });
        } else if (auto *lineEdit = qobject_cast<QLineEdit*>(widget)) {
            connect(lineEdit, &QLineEdit::textChanged, this, [this, ctrlName]() {
                updateDependentFields(ctrlName);
            });
        }

        // Initialize visibility state
        updateDependentFields(ctrlName);
    }
}

void BResourceForm::updateDependentFields(const QString &controllingDirective)
{
    // Get current value of controlling directive
    QWidget *ctrlWidget = m_fieldWidgets.value(controllingDirective);
    if (!ctrlWidget) return;

    // Check for container widgets with embedded field widget
    QWidget *actualCtrlWidget = ctrlWidget->property("fieldWidget").value<QWidget*>();
    if (actualCtrlWidget) {
        ctrlWidget = actualCtrlWidget;
    }

    QVariant ctrlValue;
    if (auto *checkBox = qobject_cast<QCheckBox*>(ctrlWidget)) {
        ctrlValue = checkBox->isChecked();
    } else if (auto *comboBox = qobject_cast<QComboBox*>(ctrlWidget)) {
        ctrlValue = comboBox->currentText();
    } else if (auto *lineEdit = qobject_cast<QLineEdit*>(ctrlWidget)) {
        ctrlValue = lineEdit->text();
    } else if (auto *spinBox = qobject_cast<QSpinBox*>(ctrlWidget)) {
        ctrlValue = spinBox->value();
    }

    // Update all dependent fields
    for (auto it = m_fieldDirectives.begin(); it != m_fieldDirectives.end(); ++it) {
        const QString &fieldName = it.key();
        const BDirective &dir = it.value();

        if (!dir.visibleWhen.isValid())
            continue;
        if (dir.visibleWhen.directive != controllingDirective)
            continue;

        // Check if condition is met
        bool shouldBeEnabled = false;

        if (!dir.visibleWhen.values.isEmpty()) {
            // Check against list of values
            shouldBeEnabled = dir.visibleWhen.values.contains(ctrlValue);
        } else if (dir.visibleWhen.value.isValid()) {
            // Check against single value
            shouldBeEnabled = (ctrlValue == dir.visibleWhen.value);
        }

        // Update widget and label enabled state
        QWidget *depWidget = m_fieldWidgets.value(fieldName);
        QLabel *depLabel = m_fieldLabels.value(fieldName);

        if (depWidget) {
            depWidget->setEnabled(shouldBeEnabled);
        }
        if (depLabel) {
            depLabel->setEnabled(shouldBeEnabled);
        }
    }
}
