/**
 * @file bcertificategenerator.cpp
 * @brief TLS Certificate Generator Dialog implementation
 */

#include "bcertificategenerator.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>

BCertificateGenerator::BCertificateGenerator(QWidget *parent)
    : QDialog(parent)
    , m_success(false)
{
    setWindowTitle(tr("Generate TLS Certificates"));
    setMinimumSize(500, 450);
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
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/bareos-certs";
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
    m_logEdit->setMaximumHeight(120);
    m_logEdit->setVisible(false);
    mainLayout->addWidget(m_logEdit);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    m_generateButton = new QPushButton(tr("Generate Certificates"), this);
    m_generateButton->setMinimumHeight(35);
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

bool BCertificateGenerator::runOpenSSL(const QStringList &args, const QString &description)
{
    appendLog(description);

    QProcess process;
    process.start("openssl", args);

    if (!process.waitForStarted(5000)) {
        appendLog(tr("Failed to start OpenSSL"), true);
        return false;
    }

    if (!process.waitForFinished(30000)) {
        appendLog(tr("OpenSSL timed out"), true);
        return false;
    }

    if (process.exitCode() != 0) {
        QString error = QString::fromUtf8(process.readAllStandardError());
        appendLog(tr("OpenSSL error: %1").arg(error), true);
        return false;
    }

    return true;
}

bool BCertificateGenerator::fileExists(const QString &path) const
{
    return QFile::exists(path);
}

void BCertificateGenerator::onGenerate()
{
    m_success = false;
    m_logEdit->clear();
    m_logEdit->setVisible(true);
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_generateButton->setEnabled(false);
    m_statusLabel->setText(tr("Generating..."));
    m_statusLabel->setStyleSheet("color: black; font-weight: bold;");

    QString outputDir = m_outputDirEdit->text().trimmed();
    QString cn = m_commonNameEdit->text().trimmed();
    QString org = m_organizationEdit->text().trimmed();
    int validDays = m_validDaysEdit->text().toInt();

    if (outputDir.isEmpty() || cn.isEmpty()) {
        appendLog(tr("Please fill in all required fields"), true);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Error"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }

    if (validDays <= 0) validDays = 3650;

    // Create output directory
    QDir dir(outputDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            appendLog(tr("Failed to create output directory"), true);
            m_generateButton->setEnabled(true);
            m_statusLabel->setText(tr("Error"));
            m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
            return;
        }
    }

    // Define paths
    QString caKeyPath = outputDir + "/ca.key";
    m_caCertPath = outputDir + "/ca.pem";
    m_clientKeyPath = outputDir + "/client.key";
    QString clientCsrPath = outputDir + "/client.csr";
    m_clientCertPath = outputDir + "/client.pem";
    m_pfxPath = outputDir + "/client.pfx";

    // Check existing files
    if (!m_overwriteCheck->isChecked()) {
        if (fileExists(m_caCertPath) || fileExists(m_clientCertPath) || fileExists(m_clientKeyPath)) {
            appendLog(tr("Certificate files already exist. Enable 'Overwrite' to replace."), true);
            m_generateButton->setEnabled(true);
            m_statusLabel->setText(tr("Files exist"));
            m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
            return;
        }
    }

    QString subject = QString("/CN=%1/O=%2").arg(cn, org.isEmpty() ? "Onesimus" : org);

    m_progressBar->setValue(10);

    // Step 1: Generate CA private key
    if (!runOpenSSL({"genrsa", "-out", caKeyPath, "4096"},
                    tr("Generating CA private key..."))) {
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(25);

    // Step 2: Generate CA certificate
    if (!runOpenSSL({"req", "-new", "-x509", "-days", QString::number(validDays),
                     "-key", caKeyPath, "-out", m_caCertPath,
                     "-subj", "/CN=Onesimus-CA/O=" + (org.isEmpty() ? "Onesimus" : org)},
                    tr("Generating CA certificate..."))) {
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(40);

    // Step 3: Generate client private key
    if (!runOpenSSL({"genrsa", "-out", m_clientKeyPath, "4096"},
                    tr("Generating client private key..."))) {
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(55);

    // Step 4: Generate client CSR
    if (!runOpenSSL({"req", "-new", "-key", m_clientKeyPath, "-out", clientCsrPath,
                     "-subj", subject},
                    tr("Generating client certificate request..."))) {
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(70);

    // Step 5: Sign client certificate with CA
    if (!runOpenSSL({"x509", "-req", "-days", QString::number(validDays),
                     "-in", clientCsrPath, "-CA", m_caCertPath, "-CAkey", caKeyPath,
                     "-CAcreateserial", "-out", m_clientCertPath},
                    tr("Signing client certificate..."))) {
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(85);

    // Step 6: Generate PFX (Windows)
#ifdef Q_OS_WIN
    if (!runOpenSSL({"pkcs12", "-export", "-out", m_pfxPath,
                     "-inkey", m_clientKeyPath, "-in", m_clientCertPath,
                     "-certfile", m_caCertPath, "-passout", "pass:"},
                    tr("Creating PFX file..."))) {
        appendLog(tr("PFX creation failed (non-critical)"), true);
    }
#endif

    // Cleanup CSR
    QFile::remove(clientCsrPath);

    m_progressBar->setValue(100);
    m_success = true;

    appendLog(tr("Certificates generated successfully!"));
    appendLog(tr("CA Certificate: %1").arg(m_caCertPath));
    appendLog(tr("Client Certificate: %1").arg(m_clientCertPath));
    appendLog(tr("Private Key: %1").arg(m_clientKeyPath));
#ifdef Q_OS_WIN
    appendLog(tr("PFX File: %1").arg(m_pfxPath));
#endif

    m_statusLabel->setText(tr("Success!"));
    m_statusLabel->setStyleSheet("color: #008000; font-weight: bold;");
    m_generateButton->setEnabled(true);
    m_closeButton->setText(tr("Use Certificates"));

    // Change close behavior to accept
    disconnect(m_closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
}
