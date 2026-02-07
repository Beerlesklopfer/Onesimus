/**
 * @file bcertificategenerator.cpp
 * @brief TLS Certificate Generator Dialog implementation
 *
 * Uses OpenSSL library API directly for certificate generation.
 * Uses BIO API instead of FILE* to avoid applink issues on Windows.
 */

#include "bcertificategenerator.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pkcs12.h>
#include <openssl/bio.h>

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

QString BCertificateGenerator::getOpenSSLError()
{
    char buf[256];
    unsigned long err = ERR_get_error();
    if (err != 0) {
        ERR_error_string_n(err, buf, sizeof(buf));
        return QString::fromLatin1(buf);
    }
    return QString();
}

bool BCertificateGenerator::fileExists(const QString &path) const
{
    return QFile::exists(path);
}

EVP_PKEY* BCertificateGenerator::generateRSAKey(int bits)
{
    EVP_PKEY *pkey = nullptr;
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

    if (!ctx) {
        appendLog(tr("Failed to create key context: %1").arg(getOpenSSLError()), true);
        return nullptr;
    }

    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        appendLog(tr("Failed to init keygen: %1").arg(getOpenSSLError()), true);
        EVP_PKEY_CTX_free(ctx);
        return nullptr;
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
        appendLog(tr("Failed to set key bits: %1").arg(getOpenSSLError()), true);
        EVP_PKEY_CTX_free(ctx);
        return nullptr;
    }

    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        appendLog(tr("Failed to generate key: %1").arg(getOpenSSLError()), true);
        EVP_PKEY_CTX_free(ctx);
        return nullptr;
    }

    EVP_PKEY_CTX_free(ctx);
    return pkey;
}

X509* BCertificateGenerator::generateCertificate(EVP_PKEY *pkey, const QString &cn,
                                                  const QString &org, int validDays,
                                                  bool isCa, X509 *issuerCert,
                                                  EVP_PKEY *issuerKey)
{
    X509 *cert = X509_new();
    if (!cert) {
        appendLog(tr("Failed to create X509: %1").arg(getOpenSSLError()), true);
        return nullptr;
    }

    // Set version to X509v3
    if (X509_set_version(cert, 2) != 1) {
        appendLog(tr("Failed to set X509 version"), true);
        X509_free(cert);
        return nullptr;
    }

    // Generate random serial number
    ASN1_INTEGER *serial = ASN1_INTEGER_new();
    if (!serial) {
        appendLog(tr("Failed to create serial number"), true);
        X509_free(cert);
        return nullptr;
    }
    ASN1_INTEGER_set(serial, (long)time(nullptr) + (isCa ? 0 : 1));  // Different serial for CA vs client
    X509_set_serialNumber(cert, serial);
    ASN1_INTEGER_free(serial);

    // Set validity period
    X509_gmtime_adj(X509_getm_notBefore(cert), 0);
    X509_gmtime_adj(X509_getm_notAfter(cert), (long)60 * 60 * 24 * validDays);

    // Set public key
    if (X509_set_pubkey(cert, pkey) != 1) {
        appendLog(tr("Failed to set public key"), true);
        X509_free(cert);
        return nullptr;
    }

    // Create and set subject name
    X509_NAME *name = X509_NAME_new();
    if (!name) {
        appendLog(tr("Failed to create X509_NAME"), true);
        X509_free(cert);
        return nullptr;
    }

    QByteArray cnBytes = cn.toUtf8();
    QByteArray orgBytes = org.toUtf8();

    // Add Organization first, then Common Name (order matters for display)
    if (X509_NAME_add_entry_by_txt(name, "O", MBSTRING_UTF8,
            (const unsigned char*)orgBytes.constData(), -1, -1, 0) != 1) {
        appendLog(tr("Failed to add Organization to subject: %1").arg(getOpenSSLError()), true);
        X509_NAME_free(name);
        X509_free(cert);
        return nullptr;
    }

    if (X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_UTF8,
            (const unsigned char*)cnBytes.constData(), -1, -1, 0) != 1) {
        appendLog(tr("Failed to add Common Name to subject: %1").arg(getOpenSSLError()), true);
        X509_NAME_free(name);
        X509_free(cert);
        return nullptr;
    }

    // Set subject name on certificate
    if (X509_set_subject_name(cert, name) != 1) {
        appendLog(tr("Failed to set subject name: %1").arg(getOpenSSLError()), true);
        X509_NAME_free(name);
        X509_free(cert);
        return nullptr;
    }

    // Set issuer name
    if (issuerCert) {
        // Client cert: issuer is the CA
        if (X509_set_issuer_name(cert, X509_get_subject_name(issuerCert)) != 1) {
            appendLog(tr("Failed to set issuer name: %1").arg(getOpenSSLError()), true);
            X509_NAME_free(name);
            X509_free(cert);
            return nullptr;
        }
    } else {
        // Self-signed CA: issuer is same as subject
        if (X509_set_issuer_name(cert, name) != 1) {
            appendLog(tr("Failed to set issuer name: %1").arg(getOpenSSLError()), true);
            X509_NAME_free(name);
            X509_free(cert);
            return nullptr;
        }
    }

    X509_NAME_free(name);  // Certificate has its own copy now

    // Add extensions for CA certificate
    if (isCa) {
        X509V3_CTX ctx;
        X509V3_set_ctx_nodb(&ctx);
        X509V3_set_ctx(&ctx, cert, cert, nullptr, nullptr, 0);

        X509_EXTENSION *ext = X509V3_EXT_conf_nid(nullptr, &ctx,
            NID_basic_constraints, const_cast<char*>("critical,CA:TRUE"));
        if (ext) {
            X509_add_ext(cert, ext, -1);
            X509_EXTENSION_free(ext);
        }

        ext = X509V3_EXT_conf_nid(nullptr, &ctx,
            NID_key_usage, const_cast<char*>("critical,keyCertSign,cRLSign"));
        if (ext) {
            X509_add_ext(cert, ext, -1);
            X509_EXTENSION_free(ext);
        }
    }

    // Sign the certificate
    EVP_PKEY *signingKey = issuerKey ? issuerKey : pkey;
    if (!X509_sign(cert, signingKey, EVP_sha256())) {
        appendLog(tr("Failed to sign certificate: %1").arg(getOpenSSLError()), true);
        X509_free(cert);
        return nullptr;
    }

    return cert;
}

bool BCertificateGenerator::savePrivateKey(EVP_PKEY *pkey, const QString &path)
{
    // Use BIO API instead of FILE* to avoid applink issues on Windows
    QByteArray pathBytes = path.toLocal8Bit();
    BIO *bio = BIO_new_file(pathBytes.constData(), "wb");
    if (!bio) {
        appendLog(tr("Cannot open file for writing: %1").arg(path), true);
        return false;
    }

    int result = PEM_write_bio_PrivateKey(bio, pkey, nullptr, nullptr, 0, nullptr, nullptr);
    BIO_free(bio);

    if (!result) {
        appendLog(tr("Failed to write private key: %1").arg(getOpenSSLError()), true);
        return false;
    }

    return true;
}

bool BCertificateGenerator::saveCertificate(X509 *cert, const QString &path)
{
    // Use BIO API instead of FILE* to avoid applink issues on Windows
    QByteArray pathBytes = path.toLocal8Bit();
    BIO *bio = BIO_new_file(pathBytes.constData(), "wb");
    if (!bio) {
        appendLog(tr("Cannot open file for writing: %1").arg(path), true);
        return false;
    }

    int result = PEM_write_bio_X509(bio, cert);
    BIO_free(bio);

    if (!result) {
        appendLog(tr("Failed to write certificate: %1").arg(getOpenSSLError()), true);
        return false;
    }

    return true;
}

bool BCertificateGenerator::createPFX(EVP_PKEY *pkey, X509 *cert, X509 *caCert,
                                       const QString &path, const QString &password)
{
    Q_UNUSED(caCert)  // TODO: Add CA to chain if needed

    PKCS12 *p12 = PKCS12_create(
        password.isEmpty() ? "" : password.toUtf8().constData(),
        "Onesimus Client Certificate",
        pkey,
        cert,
        nullptr,  // CA stack
        0, 0, 0, 0, 0
    );

    if (!p12) {
        appendLog(tr("Failed to create PKCS12: %1").arg(getOpenSSLError()), true);
        return false;
    }

    // Use BIO API instead of FILE* to avoid applink issues on Windows
    QByteArray pathBytes = path.toLocal8Bit();
    BIO *bio = BIO_new_file(pathBytes.constData(), "wb");
    if (!bio) {
        appendLog(tr("Cannot open file for writing: %1").arg(path), true);
        PKCS12_free(p12);
        return false;
    }

    int result = i2d_PKCS12_bio(bio, p12);
    BIO_free(bio);
    PKCS12_free(p12);

    if (!result) {
        appendLog(tr("Failed to write PFX file: %1").arg(getOpenSSLError()), true);
        return false;
    }

    return true;
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
    if (org.isEmpty()) org = "Onesimus";

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

    m_progressBar->setValue(10);

    // Step 1: Generate CA private key
    appendLog(tr("Generating CA private key (4096 bit RSA)..."));
    EVP_PKEY *caKey = generateRSAKey(4096);
    if (!caKey) {
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(25);

    // Step 2: Save CA key
    appendLog(tr("Saving CA private key..."));
    if (!savePrivateKey(caKey, caKeyPath)) {
        EVP_PKEY_free(caKey);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }

    // Step 3: Generate CA certificate (self-signed)
    appendLog(tr("Generating CA certificate..."));
    X509 *caCert = generateCertificate(caKey, "Onesimus-CA", org, validDays, true, nullptr, nullptr);
    if (!caCert) {
        EVP_PKEY_free(caKey);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }

    if (!saveCertificate(caCert, m_caCertPath)) {
        X509_free(caCert);
        EVP_PKEY_free(caKey);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(45);

    // Step 4: Generate client private key
    appendLog(tr("Generating client private key (4096 bit RSA)..."));
    EVP_PKEY *clientKey = generateRSAKey(4096);
    if (!clientKey) {
        X509_free(caCert);
        EVP_PKEY_free(caKey);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(60);

    // Step 5: Save client key
    appendLog(tr("Saving client private key..."));
    if (!savePrivateKey(clientKey, m_clientKeyPath)) {
        EVP_PKEY_free(clientKey);
        X509_free(caCert);
        EVP_PKEY_free(caKey);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }

    // Step 6: Generate client certificate (signed by CA)
    appendLog(tr("Generating client certificate..."));
    X509 *clientCert = generateCertificate(clientKey, cn, org, validDays, false, caCert, caKey);
    if (!clientCert) {
        EVP_PKEY_free(clientKey);
        X509_free(caCert);
        EVP_PKEY_free(caKey);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }

    if (!saveCertificate(clientCert, m_clientCertPath)) {
        X509_free(clientCert);
        EVP_PKEY_free(clientKey);
        X509_free(caCert);
        EVP_PKEY_free(caKey);
        m_generateButton->setEnabled(true);
        m_statusLabel->setText(tr("Failed"));
        m_statusLabel->setStyleSheet("color: #c00000; font-weight: bold;");
        return;
    }
    m_progressBar->setValue(80);

    // Step 7: Generate PFX file (Windows)
#ifdef Q_OS_WIN
    appendLog(tr("Creating PFX file for Windows..."));
    if (!createPFX(clientKey, clientCert, caCert, m_pfxPath, QString())) {
        appendLog(tr("PFX creation failed (non-critical)"), true);
    }
#endif

    // Cleanup OpenSSL objects
    X509_free(clientCert);
    EVP_PKEY_free(clientKey);
    X509_free(caCert);
    EVP_PKEY_free(caKey);

    m_progressBar->setValue(100);
    m_success = true;

    appendLog(tr("Certificates generated successfully!"));
    appendLog(tr("CA Certificate: %1").arg(m_caCertPath));
    appendLog(tr("Client Certificate: %1").arg(m_clientCertPath));
    appendLog(tr("Private Key: %1").arg(m_clientKeyPath));
#ifdef Q_OS_WIN
    appendLog(tr("PFX File: %1").arg(m_pfxPath));
    appendLog(tr("PFX Password: (empty - no password)"));
#endif

    m_statusLabel->setText(tr("Success!"));
    m_statusLabel->setStyleSheet("color: #008000; font-weight: bold;");
    m_generateButton->setEnabled(true);
    m_closeButton->setText(tr("Use Certificates"));

    // Change close behavior to accept
    disconnect(m_closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
}
