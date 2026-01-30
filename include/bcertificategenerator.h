/**
 * @file bcertificategenerator.h
 * @brief TLS Certificate Generator Dialog
 *
 * Generates TLS certificates using OpenSSL for Bareos communication.
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

/**
 * @class BCertificateGenerator
 * @brief Dialog for generating TLS certificates
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
    QString pfxPath() const { return m_pfxPath; }

    /**
     * @brief Check if generation was successful
     */
    bool isSuccessful() const { return m_success; }

private:
    void setupUI();
    void appendLog(const QString &msg, bool error = false);
    bool runOpenSSL(const QStringList &args, const QString &description);
    bool fileExists(const QString &path) const;

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
    bool m_success;
};

#endif // BCERTIFICATEGENERATOR_H
