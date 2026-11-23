// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bpfxconverter.cpp
 * @brief PEM/DER to PFX Converter Dialog
 */

#include "bpfxconverter.h"
#include "buiscale.h"
#include "bpki.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QPushButton>
#include <QDialogButtonBox>

BPFXConverter::BPFXConverter(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Convert to PFX Format"));
    BUiScale::track(this, [this]() { setMinimumWidth(BUiScale::px(600)); });
    BUiScale::track(this, [this]() { setMinimumHeight(BUiScale::px(500)); });

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Info label
    QLabel *infoLabel = new QLabel(
        tr("Convert separate PEM/DER certificate and key files into a single PFX file for Windows.\n"
           "PFX files are password-protected and contain both certificate and private key."));
    infoLabel->setWordWrap(true);
    // No background from code: the light blue box had no text colour of its
    // own and was unreadable with the dark theme's light text
    infoLabel->setStyleSheet("QLabel { padding: 10px; }");
    mainLayout->addWidget(infoLabel);

    // Input group
    QGroupBox *inputGroup = new QGroupBox(tr("Input Files"));
    QFormLayout *inputLayout = new QFormLayout(inputGroup);

    // Certificate file
    QHBoxLayout *certLayout = new QHBoxLayout();
    m_certFileEdit = new QLineEdit();
    m_certFileEdit->setPlaceholderText(tr("Select certificate file..."));
    m_certBrowseButton = new QPushButton(tr("Browse..."));
    connect(m_certBrowseButton, &QPushButton::clicked, this, &BPFXConverter::onBrowseCertificate);
    certLayout->addWidget(m_certFileEdit);
    certLayout->addWidget(m_certBrowseButton);
    inputLayout->addRow(tr("Certificate:"), certLayout);

    // Private key file
    QHBoxLayout *keyLayout = new QHBoxLayout();
    m_keyFileEdit = new QLineEdit();
    m_keyFileEdit->setPlaceholderText(tr("Select private key file..."));
    m_keyBrowseButton = new QPushButton(tr("Browse..."));
    connect(m_keyBrowseButton, &QPushButton::clicked, this, &BPFXConverter::onBrowsePrivateKey);
    keyLayout->addWidget(m_keyFileEdit);
    keyLayout->addWidget(m_keyBrowseButton);
    inputLayout->addRow(tr("Private Key:"), keyLayout);

    // CA certificate (optional)
    QHBoxLayout *caLayout = new QHBoxLayout();
    m_caFileEdit = new QLineEdit();
    m_caFileEdit->setPlaceholderText(tr("Optional: CA certificate"));
    m_caBrowseButton = new QPushButton(tr("Browse..."));
    connect(m_caBrowseButton, &QPushButton::clicked, this, &BPFXConverter::onBrowseCA);
    caLayout->addWidget(m_caFileEdit);
    caLayout->addWidget(m_caBrowseButton);
    inputLayout->addRow(tr("CA Cert (Optional):"), caLayout);

    mainLayout->addWidget(inputGroup);

    // Password group
    QGroupBox *passwordGroup = new QGroupBox(tr("PFX Password Protection"));
    QFormLayout *passwordLayout = new QFormLayout(passwordGroup);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText(tr("Enter password for PFX file..."));
    passwordLayout->addRow(tr("Password:"), m_passwordEdit);

    m_confirmPasswordEdit = new QLineEdit();
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setPlaceholderText(tr("Confirm password..."));
    passwordLayout->addRow(tr("Confirm:"), m_confirmPasswordEdit);

    mainLayout->addWidget(passwordGroup);

    // Output group
    QGroupBox *outputGroup = new QGroupBox(tr("Output"));
    QFormLayout *outputLayout = new QFormLayout(outputGroup);

    QHBoxLayout *outputFileLayout = new QHBoxLayout();
    m_outputFileEdit = new QLineEdit();
    m_outputFileEdit->setPlaceholderText(tr("Select where to save PFX file..."));
    m_outputBrowseButton = new QPushButton(tr("Browse..."));
    connect(m_outputBrowseButton, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getSaveFileName(this,
            tr("Save PFX File"),
            QString(),
            tr("PFX Files (*.pfx);;All Files (*)"));
        if (!file.isEmpty()) {
            if (!file.endsWith(".pfx", Qt::CaseInsensitive)) {
                file += ".pfx";
            }
            m_outputFileEdit->setText(file);
        }
    });
    outputFileLayout->addWidget(m_outputFileEdit);
    outputFileLayout->addWidget(m_outputBrowseButton);
    outputLayout->addRow(tr("Save As:"), outputFileLayout);

    mainLayout->addWidget(outputGroup);

    // Log output
    QLabel *logLabel = new QLabel(tr("Conversion Log:"));
    mainLayout->addWidget(logLabel);

    m_logText = new QTextEdit();
    m_logText->setReadOnly(true);
    BUiScale::track(m_logText, [this]() { m_logText->setMaximumHeight(BUiScale::px(150)); });
    BUiScale::bind(m_logText, BUiScale::Normal, BUiScale::Monospace);
    mainLayout->addWidget(m_logText);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox();
    m_convertButton = new QPushButton(tr("Convert to PFX"));
    m_convertButton->setDefault(true);
    connect(m_convertButton, &QPushButton::clicked, this, &BPFXConverter::onConvert);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

    buttonBox->addButton(m_convertButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttonBox);
}

void BPFXConverter::onBrowseCertificate()
{
    QString file = QFileDialog::getOpenFileName(this,
        tr("Select Certificate File"),
        QString(),
        tr("Certificates (*.pem *.crt *.cert *.der);;All Files (*)"));
    if (!file.isEmpty()) {
        m_certFileEdit->setText(file);
        m_logText->append(tr("✓ Certificate selected: %1").arg(file));
    }
}

void BPFXConverter::onBrowsePrivateKey()
{
    QString file = QFileDialog::getOpenFileName(this,
        tr("Select Private Key File"),
        QString(),
        tr("Private Keys (*.pem *.key);;All Files (*)"));
    if (!file.isEmpty()) {
        m_keyFileEdit->setText(file);
        m_logText->append(tr("✓ Private key selected: %1").arg(file));
    }
}

void BPFXConverter::onBrowseCA()
{
    QString file = QFileDialog::getOpenFileName(this,
        tr("Select CA Certificate"),
        QString(),
        tr("Certificates (*.pem *.crt *.cert);;All Files (*)"));
    if (!file.isEmpty()) {
        m_caFileEdit->setText(file);
        m_logText->append(tr("✓ CA certificate selected: %1").arg(file));
    }
}

bool BPFXConverter::validateInputs()
{
    // Check certificate file
    if (m_certFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Input"),
            tr("Please select a certificate file."));
        return false;
    }

    // Check private key file
    if (m_keyFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Input"),
            tr("Please select a private key file."));
        return false;
    }

    // Check password
    if (m_passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Input"),
            tr("Please enter a password to protect the PFX file."));
        return false;
    }

    // Check password confirmation
    if (m_passwordEdit->text() != m_confirmPasswordEdit->text()) {
        QMessageBox::warning(this, tr("Password Mismatch"),
            tr("The passwords do not match. Please try again."));
        return false;
    }

    // Check output file
    if (m_outputFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Output"),
            tr("Please specify where to save the PFX file."));
        return false;
    }

    return true;
}

void BPFXConverter::onConvert()
{
    m_logText->clear();
    m_logText->append(tr("========================================"));
    m_logText->append(tr("Starting PFX Conversion"));
    m_logText->append(tr("========================================"));

    if (!validateInputs()) {
        return;
    }

    if (performConversion()) {
        m_logText->append(tr("========================================"));
        m_logText->append(tr("✓ Conversion successful!"));
        m_logText->append(tr("PFX file created: %1").arg(m_outputFileEdit->text()));
        m_logText->append(tr("========================================"));

        m_pfxFilePath = m_outputFileEdit->text();

        QMessageBox::information(this, tr("Success"),
            tr("PFX file created successfully!\n\n%1\n\n"
               "You can now use this file in the certificate settings.")
            .arg(m_pfxFilePath));

        accept();
    }
}

bool BPFXConverter::performConversion()
{
    auto readFile = [this](const QString &path, const QString &what, QByteArray *data) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            m_logText->append(tr("✗ ERROR: Cannot open %1: %2").arg(what, file.errorString()));
            QMessageBox::critical(this, tr("Error"),
                tr("Cannot open %1:\n%2").arg(what, path));
            return false;
        }
        *data = file.readAll();
        return true;
    };

    QByteArray certData, keyData, caData;
    m_logText->append(tr("Loading certificate from: %1").arg(m_certFileEdit->text()));
    if (!readFile(m_certFileEdit->text(), tr("the certificate file"), &certData)) {
        return false;
    }
    m_logText->append(tr("Loading private key from: %1").arg(m_keyFileEdit->text()));
    if (!readFile(m_keyFileEdit->text(), tr("the private key file"), &keyData)) {
        return false;
    }
    const QString caPath = m_caFileEdit->text().trimmed();
    if (!caPath.isEmpty()) {
        if (!readFile(caPath, tr("the CA certificate file"), &caData)) {
            return false;
        }
    }

    // Writes the file owner-only and reads it back with the password before
    // it reports success
    m_logText->append(tr("Creating PFX (PKCS#12) file..."));
    QString error;
    if (!BPki::convertToPkcs12(certData, keyData, caData, m_outputFileEdit->text(),
                               m_passwordEdit->text().toUtf8(), &error)) {
        m_logText->append(tr("✗ ERROR: %1").arg(error));
        QMessageBox::critical(this, tr("Error"),
            tr("The PFX file was not created.\n\n%1").arg(error));
        return false;
    }
    return true;
}
