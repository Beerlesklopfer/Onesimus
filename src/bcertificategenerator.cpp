// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bcertificategenerator.cpp
 * @brief TLS Certificate Generator Dialog implementation
 *
 * The OpenSSL work is in bpki.cpp: key files are created owner-only, the CA
 * key is kept only encrypted with a passphrase, the PFX only with a password.
 */

#include "bcertificategenerator.h"
#include "buiscale.h"
#include "blabelstate.h"
#include "bpki.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>


BCertificateGenerator::BCertificateGenerator(QWidget *parent)
    : QDialog(parent)
    , m_success(false)
{
    setWindowTitle(tr("Generate TLS Certificates"));
    BUiScale::track(this, [this]() { setMinimumSize(BUiScale::px(500), BUiScale::px(450)); });
    setupUI();
}

BCertificateGenerator::~BCertificateGenerator() = default;

void BCertificateGenerator::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Info
    auto *infoLabel = new QLabel(tr(
        "Generate TLS certificates for secure Bareos communication.\n"
        "This will create a CA certificate, client certificate, and private key."
    ), this);
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    // Settings group
    auto *settingsGroup = new QGroupBox(tr("Certificate Settings"), this);
    auto *formLayout = new QFormLayout(settingsGroup);

    // Output directory
    auto *dirLayout = new QHBoxLayout();
    m_outputDirEdit = new QLineEdit(this);
    // Below the application's data directory, not in the home directory,
    // where every other user could list it
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/certs";
    m_outputDirEdit->setText(defaultDir);
    m_browseButton = new QPushButton(tr("Browse..."), this);
    dirLayout->addWidget(m_outputDirEdit);
    dirLayout->addWidget(m_browseButton);
    formLayout->addRow(tr("Output Directory:"), dirLayout);

    // Common Name
    m_commonNameEdit = new QLineEdit(this);
    m_commonNameEdit->setText("onesimus-client");
    m_commonNameEdit->setPlaceholderText(tr("e.g., onesimus-client"));
    formLayout->addRow(tr("Common Name (CN):"), m_commonNameEdit);

    // Organization
    m_organizationEdit = new QLineEdit(this);
    m_organizationEdit->setText("Onesimus");
    m_organizationEdit->setPlaceholderText(tr("e.g., My Company"));
    formLayout->addRow(tr("Organization (O):"), m_organizationEdit);

    // Valid days
    m_validDaysEdit = new QLineEdit(this);
    m_validDaysEdit->setText("3650");
    m_validDaysEdit->setPlaceholderText(tr("Days (e.g., 3650 = 10 years)"));
    formLayout->addRow(tr("Valid for (days):"), m_validDaysEdit);

    // CA key: kept only encrypted; without a passphrase it is discarded
    // after signing (a new CA is made next time)
    m_caPassphraseEdit = new QLineEdit(this);
    m_caPassphraseEdit->setEchoMode(QLineEdit::Password);
    m_caPassphraseEdit->setPlaceholderText(tr("Empty: the CA key is not kept"));
    m_caPassphraseEdit->setToolTip(tr("With a passphrase the CA key is saved encrypted (ca.key) "
                                      "to sign more certificates later. Without one it is not saved."));
    formLayout->addRow(tr("CA key passphrase:"), m_caPassphraseEdit);

    // PFX password: a PFX file is written only with a password
    m_pfxPasswordEdit = new QLineEdit(this);
    m_pfxPasswordEdit->setEchoMode(QLineEdit::Password);
#ifdef Q_OS_WIN
    m_pfxPasswordEdit->setPlaceholderText(tr("Required: protects client.pfx"));
#else
    m_pfxPasswordEdit->setPlaceholderText(tr("Optional: also writes client.pfx"));
#endif
    formLayout->addRow(tr("PFX password:"), m_pfxPasswordEdit);
    m_pfxConfirmEdit = new QLineEdit(this);
    m_pfxConfirmEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow(tr("Confirm PFX password:"), m_pfxConfirmEdit);

    // Overwrite
    m_overwriteCheck = new QCheckBox(tr("Overwrite existing files"), this);
    formLayout->addRow("", m_overwriteCheck);

    mainLayout->addWidget(settingsGroup);

    // Progress
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("font-weight: bold;");
    mainLayout->addWidget(m_statusLabel);

    // Log
    m_logEdit = new QTextEdit(this);
    m_logEdit->setReadOnly(true);
    BUiScale::track(m_logEdit, [this]() { m_logEdit->setMaximumHeight(BUiScale::px(120)); });
    m_logEdit->setVisible(false);
    mainLayout->addWidget(m_logEdit);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    m_generateButton = new QPushButton(tr("Generate Certificates"), this);
    BUiScale::track(m_generateButton, [this]() { m_generateButton->setMinimumHeight(BUiScale::px(35)); });
    m_closeButton = new QPushButton(tr("Close"), this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_generateButton);
    buttonLayout->addWidget(m_closeButton);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(m_browseButton, &QPushButton::clicked, this, &BCertificateGenerator::onBrowseOutput);
    connect(m_generateButton, &QPushButton::clicked, this, &BCertificateGenerator::onGenerate);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::reject);
}

void BCertificateGenerator::onBrowseOutput()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Output Directory"),
        m_outputDirEdit->text());
    if (!dir.isEmpty()) {
        m_outputDirEdit->setText(dir);
    }
}

void BCertificateGenerator::appendLog(const QString &msg, bool error)
{
    QString color = error ? "#c00000" : "#333333";
    m_logEdit->append(QString("<span style='color:%1;'>%2</span>").arg(color, msg));
}

bool BCertificateGenerator::fileExists(const QString &path) const
{
    return QFile::exists(path);
}

void BCertificateGenerator::fail(const QString &message)
{
    if (!message.isEmpty()) {
        appendLog(message, true);
    }
    m_generateButton->setEnabled(true);
    m_statusLabel->setText(tr("Failed"));
    bSetLabelState(m_statusLabel, "error");
}

void BCertificateGenerator::onGenerate()
{
    m_success = false;
    m_pfxPath.clear();
    m_pfxPassword.clear();
    m_logEdit->clear();
    m_logEdit->setVisible(true);
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_generateButton->setEnabled(false);
    m_statusLabel->setText(tr("Generating..."));
    bSetLabelState(m_statusLabel, "");   // theme text colour; bold from the label's style sheet

    QString outputDir = m_outputDirEdit->text().trimmed();
    QString cn = m_commonNameEdit->text().trimmed();
    QString org = m_organizationEdit->text().trimmed();
    int validDays = m_validDaysEdit->text().toInt();
    const QByteArray caPassphrase = m_caPassphraseEdit->text().toUtf8();
    const QString pfxPassword = m_pfxPasswordEdit->text();

    if (outputDir.isEmpty() || cn.isEmpty()) {
        appendLog(tr("Please fill in all required fields"), true);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Error"));
        bSetLabelState(m_statusLabel, "error");
        return;
    }
    if (pfxPassword != m_pfxConfirmEdit->text()) {
        appendLog(tr("The PFX passwords do not match."), true);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Error"));
        bSetLabelState(m_statusLabel, "error");
        return;
    }
#ifdef Q_OS_WIN
    // On Windows the connection uses the PFX file, which needs a password
    if (pfxPassword.isEmpty()) {
        appendLog(tr("Please enter a password to protect the PFX file."), true);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Error"));
        bSetLabelState(m_statusLabel, "error");
        return;
    }
#endif

    if (validDays <= 0) validDays = 3650;
    if (org.isEmpty()) org = "Onesimus";

    // Create output directory; a new one only for its owner
    QDir dir(outputDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            appendLog(tr("Failed to create output directory"), true);
            m_generateButton->setEnabled(true);
            m_statusLabel->setText(tr("Error"));
            bSetLabelState(m_statusLabel, "error");
            return;
        }
        QFile::setPermissions(outputDir, QFileDevice::ReadOwner | QFileDevice::WriteOwner
                                             | QFileDevice::ExeOwner);
    }

    // Define paths
    QString caKeyPath = outputDir + "/ca.key";
    m_caCertPath = outputDir + "/ca.pem";
    m_clientKeyPath = outputDir + "/client.key";
    m_clientCertPath = outputDir + "/client.pem";
    const QString pfxPath = outputDir + "/client.pfx";

    // Check existing files
    if (!m_overwriteCheck->isChecked()) {
        if (fileExists(m_caCertPath) || fileExists(m_clientCertPath) || fileExists(m_clientKeyPath)
            || (!caPassphrase.isEmpty() && fileExists(caKeyPath))
            || (!pfxPassword.isEmpty() && fileExists(pfxPath))) {
            appendLog(tr("Certificate files already exist. Enable 'Overwrite' to replace."), true);
            m_generateButton->setEnabled(true);
            m_statusLabel->setText(tr("Files exist"));
            bSetLabelState(m_statusLabel, "error");
            return;
        }
    }

    m_progressBar->setValue(10);
    QString error;

    // Step 1: CA key and self-signed CA certificate
    appendLog(tr("Generating CA private key (4096 bit RSA)..."));
    BPki::KeyPtr caKey = BPki::generateRsaKey(4096, &error);
    if (!caKey) {
        fail(error);
        return;
    }
    m_progressBar->setValue(25);

    appendLog(tr("Generating CA certificate..."));
    BPki::CertificateRequest caRequest;
    caRequest.commonName = QStringLiteral("Onesimus-CA");
    caRequest.organization = org;
    caRequest.validDays = validDays;
    caRequest.isCa = true;
    BPki::CertPtr caCert = BPki::makeCertificate(caKey.get(), caRequest, nullptr, nullptr, &error);
    if (!caCert || !BPki::writeCertificate(caCert.get(), m_caCertPath, &error)) {
        fail(error);
        return;
    }

    // Step 2: the CA key only encrypted, or not at all
    if (!caPassphrase.isEmpty()) {
        appendLog(tr("Saving CA private key (encrypted with the passphrase)..."));
        if (!BPki::writePrivateKey(caKey.get(), caKeyPath, caPassphrase, &error)) {
            fail(error);
            return;
        }
    } else {
        appendLog(tr("No CA key passphrase: the CA private key is not saved."));
    }
    m_progressBar->setValue(45);

    // Step 3: client key, owner-only and unencrypted (the TLS connection
    // loads it without asking)
    appendLog(tr("Generating client private key (4096 bit RSA)..."));
    BPki::KeyPtr clientKey = BPki::generateRsaKey(4096, &error);
    if (!clientKey) {
        fail(error);
        return;
    }
    m_progressBar->setValue(60);

    appendLog(tr("Saving client private key..."));
    if (!BPki::writePrivateKey(clientKey.get(), m_clientKeyPath, QByteArray(), &error)) {
        fail(error);
        return;
    }

    // Step 4: client certificate signed by the CA
    appendLog(tr("Generating client certificate..."));
    BPki::CertificateRequest request;
    request.commonName = cn;
    request.organization = org;
    request.validDays = validDays;
    BPki::CertPtr clientCert = BPki::makeCertificate(clientKey.get(), request, caCert.get(),
                                                     caKey.get(), &error);
    if (!clientCert || !BPki::writeCertificate(clientCert.get(), m_clientCertPath, &error)) {
        fail(error);
        return;
    }
    m_progressBar->setValue(80);

    // Step 5: PFX file, only with a password
    if (!pfxPassword.isEmpty()) {
        appendLog(tr("Creating PFX file..."));
        if (!BPki::writePkcs12(clientKey.get(), clientCert.get(), caCert.get(), pfxPath,
                               pfxPassword.toUtf8(), &error)) {
#ifdef Q_OS_WIN
            fail(error);
            return;
#else
            appendLog(tr("PFX creation failed: %1").arg(error), true);
#endif
        } else {
            m_pfxPath = pfxPath;
            m_pfxPassword = pfxPassword;
        }
    }

    m_progressBar->setValue(100);
    m_success = true;

    appendLog(tr("Certificates generated successfully!"));
    appendLog(tr("CA Certificate: %1").arg(m_caCertPath));
    if (!caPassphrase.isEmpty()) {
        appendLog(tr("CA Key (encrypted): %1").arg(caKeyPath));
    }
    appendLog(tr("Client Certificate: %1").arg(m_clientCertPath));
    appendLog(tr("Private Key: %1").arg(m_clientKeyPath));
    if (!m_pfxPath.isEmpty()) {
        appendLog(tr("PFX File: %1").arg(m_pfxPath));
    }

    m_statusLabel->setText(tr("Success!"));
    bSetLabelState(m_statusLabel, "success");
    m_generateButton->setEnabled(true);
    m_closeButton->setText(tr("Use Certificates"));

    // Change close behavior to accept
    disconnect(m_closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
}
