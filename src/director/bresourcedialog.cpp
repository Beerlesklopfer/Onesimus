/**
 * @file bresourcedialog.cpp
 * @brief Dynamic schema-driven dialog for editing Bareos/Bacula resources
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "director/bresourcedialog.h"
#include "config/bresourceform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTimer>
#include <private/qzipwriter_p.h>

BResourceDialog::BResourceDialog(const QString &resourceType,
                                 const BConfigResource &existing,
                                 QWidget *parent)
    : QDialog(parent)
    , m_resourceType(resourceType)
{
    buildUi(existing);
}

BResourceDialog::BResourceDialog(const QString &resourceType,
                                 const QStringList &names,
                                 QWidget *parent)
    : QDialog(parent)
    , m_resourceType(resourceType)
{
    BConfigResource initial;
    if (!names.isEmpty()) {
        initial = BConfigResource(resourceType, names.first());
    }
    buildUi(initial);

    // Insert selector combo at position 0 (before top bar)
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        QHBoxLayout *selectorLayout = new QHBoxLayout();
        QLabel *selectorLabel = new QLabel(tr("Select %1:").arg(resourceType), this);
        selectorLayout->addWidget(selectorLabel);

        m_selectorCombo = new QComboBox(this);
        m_selectorCombo->addItems(names);
        m_selectorCombo->setMinimumWidth(250);
        selectorLayout->addWidget(m_selectorCombo, 1);
        selectorLayout->addStretch();

        mainLayout->insertLayout(0, selectorLayout);

        connect(m_selectorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &BResourceDialog::onResourceSelected);
    }
}

void BResourceDialog::buildUi(const BConfigResource &existing)
{
    setWindowTitle(tr("Edit %1 Resource").arg(m_resourceType));
    setMinimumSize(600, 500);
    resize(700, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Top bar with advanced toggle and save buttons
    QHBoxLayout *topBar = new QHBoxLayout();

    // Advanced toggle
    m_advancedToggle = new QCheckBox(tr("Show advanced directives"), this);
    m_advancedToggle->setChecked(false);
    connect(m_advancedToggle, &QCheckBox::toggled,
            this, &BResourceDialog::onShowAdvanced);
    topBar->addWidget(m_advancedToggle);

    topBar->addStretch();

    // Save buttons
    m_saveConfButton = new QPushButton(tr("Save .conf"), this);
    m_saveConfButton->setIcon(QIcon::fromTheme("document-save"));
    connect(m_saveConfButton, &QPushButton::clicked, this, &BResourceDialog::onSaveConf);
    topBar->addWidget(m_saveConfButton);

    m_saveZipButton = new QPushButton(tr("Save ZIP"), this);
    m_saveZipButton->setIcon(QIcon::fromTheme("package-x-generic"));
    connect(m_saveZipButton, &QPushButton::clicked, this, &BResourceDialog::onSaveZip);
    topBar->addWidget(m_saveZipButton);

    mainLayout->addLayout(topBar);

    // Resource form
    m_form = new BResourceForm(m_resourceType, this);
    if (!existing.type().isEmpty()) {
        m_form->setExistingResource(existing);
    }
    mainLayout->addWidget(m_form);

    // Hide advanced toggle if no advanced directives
    if (!m_form->hasAdvancedDirectives()) {
        m_advancedToggle->setVisible(false);
    }

    // Button box
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &BResourceDialog::onAccepted);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(m_buttonBox);
}

BConfigResource BResourceDialog::resource() const
{
    return m_form->resource();
}

void BResourceDialog::setReferenceData(const QMap<QString, QStringList> &referenceData)
{
    m_form->setReferenceData(referenceData);
}

void BResourceDialog::onShowAdvanced(bool checked)
{
    m_form->setAdvancedVisible(checked);
}

void BResourceDialog::onAccepted()
{
    m_form->collectValues();
    accept();
}

void BResourceDialog::onResourceSelected(int index)
{
    if (!m_selectorCombo || index < 0) return;

    QString name = m_selectorCombo->currentText();
    BConfigResource res(m_resourceType, name);
    m_form->setExistingResource(res);
}

// Helper: Convert resource to Bareos config format
static QString resourceToConf(const BConfigResource &resource)
{
    QString conf;
    conf += QString("%1 {\n").arg(resource.type());
    conf += QString("  Name = \"%1\"\n").arg(resource.name());

    for (const QString &key : resource.keys()) {
        if (key.toLower() == "name") continue;  // Already emitted above

        BConfigValue val = resource.value(key);
        switch (val.type()) {
        case BConfigValue::Simple: {
            QString v = val.simpleValue();
            // Quote strings, but not numbers/booleans/time periods
            bool needsQuotes = true;
            bool isNumeric = false;
            v.toLongLong(&isNumeric);
            if (isNumeric) needsQuotes = false;
            if (v.toLower() == "yes" || v.toLower() == "no") needsQuotes = false;
            if (v.startsWith("[md5]")) needsQuotes = false;

            if (needsQuotes)
                conf += QString("  %1 = \"%2\"\n").arg(key, v);
            else
                conf += QString("  %1 = %2\n").arg(key, v);
            break;
        }
        case BConfigValue::List:
            for (const QString &item : val.listValue())
                conf += QString("  %1 = \"%2\"\n").arg(key, item);
            break;
        case BConfigValue::Block:
            // Nested blocks not fully supported yet
            break;
        }
    }
    conf += QString("}\n");
    return conf;
}

void BResourceDialog::onSaveConf()
{
    m_form->collectValues();
    BConfigResource res = m_form->resource();

    QString name = res.name();
    if (name.isEmpty()) name = m_resourceType.toLower();

    QString defaultName = QString("%1.conf").arg(name);

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Save %1 Configuration").arg(m_resourceType), defaultName,
        tr("Configuration Files (*.conf);;All Files (*)"));
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Save Failed"),
                             tr("Could not write to %1").arg(filePath));
        return;
    }

    file.write(resourceToConf(res).toUtf8());
    file.close();

    m_saveConfButton->setText(tr("Saved!"));
    QTimer::singleShot(2000, this, [this]() {
        m_saveConfButton->setText(tr("Save .conf"));
    });
}

void BResourceDialog::onSaveZip()
{
    m_form->collectValues();
    BConfigResource res = m_form->resource();

    QString name = res.name();
    if (name.isEmpty()) name = m_resourceType.toLower();

    QString defaultName = QString("%1-%2.zip").arg(m_resourceType.toLower(), name);

    QString filePath = QFileDialog::getSaveFileName(
        this, tr("Save %1 Configuration as ZIP").arg(m_resourceType), defaultName,
        tr("ZIP Archives (*.zip);;All Files (*)"));
    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".zip", Qt::CaseInsensitive))
        filePath += ".zip";

    QZipWriter zip(filePath);
    if (zip.status() != QZipWriter::NoError) {
        QMessageBox::warning(this, tr("Save Failed"),
                             tr("Could not create ZIP file: %1").arg(filePath));
        return;
    }

    // Create Bareos directory structure based on resource type
    QString confDir;
    if (m_resourceType == "Client") {
        confDir = "etc/bareos/bareos-dir.d/client";
    } else if (m_resourceType == "Job" || m_resourceType == "JobDefs") {
        confDir = "etc/bareos/bareos-dir.d/job";
    } else if (m_resourceType == "FileSet") {
        confDir = "etc/bareos/bareos-dir.d/fileset";
    } else if (m_resourceType == "Schedule") {
        confDir = "etc/bareos/bareos-dir.d/schedule";
    } else if (m_resourceType == "Pool") {
        confDir = "etc/bareos/bareos-dir.d/pool";
    } else if (m_resourceType == "Storage") {
        confDir = "etc/bareos/bareos-dir.d/storage";
    } else if (m_resourceType == "Console") {
        confDir = "etc/bareos/bareos-dir.d/console";
    } else if (m_resourceType == "Director") {
        confDir = "etc/bareos/bareos-dir.d/director";
    } else {
        confDir = QString("etc/bareos/bareos-dir.d/%1").arg(m_resourceType.toLower());
    }

    // Create directory hierarchy
    QStringList parts = confDir.split('/');
    QString path;
    for (const QString &part : parts) {
        if (!path.isEmpty()) path += '/';
        path += part;
        zip.addDirectory(path);
    }

    // Add config file
    QString confPath = QString("%1/%2.conf").arg(confDir, name);
    zip.addFile(confPath, resourceToConf(res).toUtf8());

    zip.close();

    m_saveZipButton->setText(tr("Saved!"));
    QTimer::singleShot(2000, this, [this]() {
        m_saveZipButton->setText(tr("Save ZIP"));
    });
}
