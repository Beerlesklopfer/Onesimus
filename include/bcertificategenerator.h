// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bcertificategenerator.h
 * @brief TLS Certificate Generator Dialog
 *
 * Generates TLS certificates using OpenSSL library API for Bareos communication.
 * On Linux: CA cert, client cert, private key (PEM)
 * On Windows: Additionally creates PFX file
 */

#ifndef BCERTIFICATEGENERATOR_H
#define BCERTIFICATEGENERATOR_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressBar>
#include <QLabel>
#include <QCheckBox>

// Forward declarations for OpenSSL types
/**
 * @class BCertificateGenerator
 * @brief Dialog for generating TLS certificates using OpenSSL library
 */
class BCertificateGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit BCertificateGenerator(QWidget *parent = nullptr);
    ~BCertificateGenerator() override;

    /**
     * @brief Get generated certificate paths
     */
    QString caCertPath() const { return m_caCertPath; }
    QString clientCertPath() const { return m_clientCertPath; }
    QString clientKeyPath() const { return m_clientKeyPath; }
    QString pfxPath() const { return m_pfxPath; }   ///< empty when no PFX was written
    QString pfxPassword() const { return m_pfxPassword; }

    /**
     * @brief Check if generation was successful
     */
    bool isSuccessful() const { return m_success; }

private:
    void setupUI();
    void appendLog(const QString &msg, bool error = false);
    bool fileExists(const QString &path) const;
    void fail(const QString &message);

private slots:
    void onBrowseOutput();
    void onGenerate();

private:
    // UI
    QLineEdit *m_outputDirEdit;
    QPushButton *m_browseButton;
    QLineEdit *m_commonNameEdit;
    QLineEdit *m_organizationEdit;
    QLineEdit *m_validDaysEdit;
    QLineEdit *m_caPassphraseEdit;
    QLineEdit *m_pfxPasswordEdit;
    QLineEdit *m_pfxConfirmEdit;
    QCheckBox *m_overwriteCheck;
    QPushButton *m_generateButton;
    QPushButton *m_closeButton;
    QTextEdit *m_logEdit;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;

    // Results
    QString m_caCertPath;
    QString m_clientCertPath;
    QString m_clientKeyPath;
    QString m_pfxPath;
    QString m_pfxPassword;
    bool m_success;
};

#endif // BCERTIFICATEGENERATOR_H
