#ifndef BNEWCLIENTDIALOG_H
#define BNEWCLIENTDIALOG_H

#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QProgressBar>
#include <QTimer>
#include "director/bdirector.h"
#include "director/bresourcewidget.h"
#include "config/bconfigparser.h"
#include "config/bdirectiveschema.h"

// Forward declaration
class BResourceForm;

/**
 * @brief Data shared across all wizard pages
 * @since 2.8
 */
struct BNewClientWizardData {
    // Page 1 - Director (auto-populated from connection)
    QString directorName;
    QString directorAddress;
    int directorPort = 9101;
    QString tlsMode;        // "TLS-PSK" or "TLS Certificate"

    // Page 2 - Client settings (user input)
    QString clientName;
    QString clientAddress;
    int fdPort = 9102;
    QString password;       // cleartext (in wizard only)
    QString passwordHash;   // MD5 hex
    bool executeConfigureCmd = true;

    // TLS settings (platform-specific)
    bool tlsEnable = true;
    bool tlsRequire = false;  // false for PSK, true for x509
    // Windows: PFX file
    QString tlsCertificateFile;  // PFX path (Windows only)
    // Linux/Mac: PEM files
    QString tlsCaCertificateFile;  // CA cert (Unix)
    QString tlsCertificate;        // Client cert (Unix)
    QString tlsKey;                // Private key (Unix)

    // NAT/Passive mode settings
    bool passive = false;
    int connectionFromClientWait = 1800;  // seconds

    // Page 3 - Generated config files (FD-side)
    QString directorConf;   // FD-side: bareos-fd.d/director/<dir>.conf
    QString clientConf;     // FD-side: bareos-fd.d/client/myself.conf
    QString messagesConf;   // FD-side: bareos-fd.d/messages/Standard.conf

    // Director-side
    QString dirClientConf;  // Director-side: bareos-dir.d/client/<name>.conf
    QString configureCommand; // Director-side: configure add client ...

    // Validation results
    QStringList validationErrors;
    QStringList validationWarnings;
};

/**
 * @brief Wizard for adding a new client — generates FD config files + ZIP export
 *
 * Pages:
 *   1. Director Info — auto-populated from active connection
 *   2. Password & Settings — client name, address, port, password
 *   3. Preview & Export — resource preview with validation, clipboard, ZIP
 *
 * Uses BDirectiveSchema for validation and BConfigParser for parsing
 * generated configs into BConfigResource objects for structured display.
 *
 * @since 2.8
 */
class BNewClientWizard : public QWizard
{
    Q_OBJECT

public:
    enum PageId {
        Page_Director,
        Page_Settings,
        Page_Preview
    };

    explicit BNewClientWizard(BDirector *director, QWidget *parent = nullptr);

    BDirector *director() const { return m_director; }
    BNewClientWizardData &data() { return m_data; }
    const BNewClientWizardData &data() const { return m_data; }

private:
    BDirector *m_director;
    BNewClientWizardData m_data;
};

// ============================================================================
// Page 1: Director Info
// ============================================================================

class BNewClientDirectorPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BNewClientDirectorPage(QWidget *parent = nullptr);
    void initializePage() override;
    bool isComplete() const override;

private:
    QLabel *m_directorNameLabel;
    QLabel *m_directorAddressLabel;
    QLabel *m_directorPortLabel;
    QLabel *m_tlsModeLabel;
    QLabel *m_errorLabel;
    bool m_dataValid = false;
};

// ============================================================================
// Page 2: Password & Settings
// ============================================================================

class BNewClientSettingsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BNewClientSettingsPage(QWidget *parent = nullptr);
    void initializePage() override;
    void cleanupPage() override;
    bool isComplete() const override;

private slots:
    void onGeneratePassword();
    void onPasswordChanged(const QString &text);
    void onResourceFormChanged();
    void onPassiveChanged(bool checked);

private:
    void collectFormValues();
    void restoreFormValues();

    // Basic settings
    QLineEdit *m_nameEdit;
    QLineEdit *m_addressEdit;
    QSpinBox *m_portSpin;
    QLineEdit *m_passwordEdit;
    QPushButton *m_generateButton;
    QLabel *m_md5Label;
    QCheckBox *m_executeCheck;

    // TLS settings via schema-driven form
    BResourceForm *m_resourceForm;

    // NAT/Passive mode (simple checkbox + spinner)
    QCheckBox *m_passiveCheck;
    QWidget *m_waitContainer;
    QSpinBox *m_waitSpin;

    bool m_initialized = false;
};

// ============================================================================
// Page 3: Preview & Export
// ============================================================================

class BNewClientPreviewPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BNewClientPreviewPage(QWidget *parent = nullptr);
    void initializePage() override;
    bool validatePage() override;

private slots:
    void onCopyCurrentTab();
    void onCopyAll();
    void onExportZip();
    void onJsonResponse(BDirector::Command cmd, const QString &jsonData);
    void onCommandResponse(BDirector::Command cmd, const QString &response);
    void onConfigureTimeout();

private:
    void generateConfigs();
    void validateConfigs();
    void executeConfigureCommand();
    void parseAndDisplayResources();
    void updateRawText();

    QSplitter *m_mainSplitter;
    QTabWidget *m_tabWidget;
    BResourceWidget *m_fdDirectorWidget;
    BResourceWidget *m_fdClientWidget;
    BResourceWidget *m_fdMessagesWidget;
    BResourceWidget *m_dirClientWidget;
    QTextEdit *m_rawTextEdit;
    QGroupBox *m_cmdGroup;
    QTextEdit *m_commandEdit;
    QLabel *m_validationLabel;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    QPushButton *m_exportButton;
    QTimer *m_timeoutTimer;
    bool m_executed = false;
    bool m_waitingForReload = false;
};

#endif // BNEWCLIENTDIALOG_H
