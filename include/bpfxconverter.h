#ifndef BPFXCONVERTER_H
#define BPFXCONVERTER_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

/**
 * @brief Dialog to convert PEM/DER certificates to PFX format
 *
 * This dialog helps Windows users convert separate PEM certificate
 * and key files into a single PFX (PKCS#12) file for use with Bareos TLS.
 *
 * Workflow:
 * 1. Select certificate file (.pem, .crt, .der)
 * 2. Select private key file (.pem, .key)
 * 3. Optionally select CA certificate
 * 4. Enter password for PFX protection
 * 5. Convert and save as .pfx file
 */
class BPFXConverter : public QDialog
{
    Q_OBJECT

public:
    explicit BPFXConverter(QWidget *parent = nullptr);

    QString getPFXFilePath() const { return m_pfxFilePath; }

private slots:
    void onBrowseCertificate();
    void onBrowsePrivateKey();
    void onBrowseCA();
    void onConvert();

private:
    // Input fields
    QLineEdit *m_certFileEdit;
    QPushButton *m_certBrowseButton;

    QLineEdit *m_keyFileEdit;
    QPushButton *m_keyBrowseButton;

    QLineEdit *m_caFileEdit;
    QPushButton *m_caBrowseButton;

    QLineEdit *m_passwordEdit;
    QLineEdit *m_confirmPasswordEdit;

    // Output
    QLineEdit *m_outputFileEdit;
    QPushButton *m_outputBrowseButton;

    QPushButton *m_convertButton;
    QTextEdit *m_logText;

    QString m_pfxFilePath;

    bool validateInputs();
    bool performConversion();
};

#endif // BPFXCONVERTER_H
