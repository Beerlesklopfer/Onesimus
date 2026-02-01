/**
 * @file bconnectionwizard.h
 * @brief Director Connection Wizard (Q&A Style)
 */

#ifndef BCONNECTIONWIZARD_H
#define BCONNECTIONWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QSpinBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressBar>
#include <QTimer>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QComboBox>

#include "bconnectionprofile.h"

class BareosDirector;
class QSqlDatabase;

/**
 * @struct BConnectionWizardData
 * @brief Struct to preserve wizard data across page navigation
 *
 * This struct is used to store wizard input values so they persist
 * even when navigating back and forth between pages. MainWindow holds
 * a pointer to this struct until the wizard completes successfully.
 */
struct BConnectionWizardData {
    // Server page
    QString host;
    int port = 9101;

    // Credentials page
    QString directorName;
    QString consoleName;
    QString password;
    bool savePassword = true;

    // Auth method page
    QString authMethod = "psk";  // "psk", "cert", or "legacy"

    // TLS page (for cert mode)
    QString tlsCaCertFile;
    QString tlsCertFile;
    QString tlsKeyFile;

    // Profile page
    QString profileName;
    bool setAsDefault = true;
    bool connectNow = true;
};

/**
 * @class BConnectionWizard
 * @brief Q&A wizard for director connection configuration
 *
 * Pages:
 *   1. Welcome
 *   2. Server (Host + Port)
 *   3. Credentials (Director, Console, Password)
 *   4. AuthMethod
 *   5. TLS (only if cert auth selected)
 *   6. Test
 *   7. ProfileName
 *
 * The wizard uses a BConnectionWizardData pointer to preserve data across
 * page navigation. MainWindow owns this data and clears it on success.
 */
class BConnectionWizard : public QWizard
{
    Q_OBJECT

public:
    enum PageId {
        Page_Welcome,
        Page_TemplateSelection,
        Page_Server,
        Page_Credentials,
        Page_AuthMethod,
        Page_TLS,
        Page_ConfigPreview,
        Page_Test,
        Page_ConsoleSetup,
        Page_ProfileName
    };

    /**
     * @brief Detected server capabilities
     */
    struct ServerCapabilities {
        bool checked = false;           ///< Has capability check been done?
        bool reachable = false;         ///< Is server reachable on the port?
        bool supportsPSK = false;       ///< Does server support TLS-PSK?
        bool supportsCert = false;      ///< Does server support TLS with certificates?
        bool supportsLegacy = false;    ///< Does server support legacy (no TLS)?
        QString detectedVersion;        ///< Detected Bareos/Bacula version
        QString lastError;              ///< Last error during detection
    };

    /**
     * @brief Construct wizard with external data storage
     * @param wizardData Pointer to data struct owned by caller (e.g., MainWindow)
     * @param db Database connection for loading/saving Directors (optional)
     * @param parent Parent widget
     */
    explicit BConnectionWizard(BConnectionWizardData *wizardData = nullptr,
                              QSqlDatabase *db = nullptr,
                              QWidget *parent = nullptr);
    ~BConnectionWizard() override;

    BConnectionProfile profile() const;
    void setProfile(const BConnectionProfile &profile);

    /**
     * @brief Save connection to database
     * @param db Database connection
     * @return Director ID on success, -1 on failure
     *
     * Saves the wizard data to the directors table using BDirectorModel.
     * Returns the new Director ID which can be used to mark as default.
     */
    int saveToDatabase(QSqlDatabase &db);

    ServerCapabilities capabilities() const { return m_capabilities; }
    void setCapabilities(const ServerCapabilities &caps) { m_capabilities = caps; }

    /// Access the wizard data struct
    BConnectionWizardData *wizardData() const { return m_wizardData; }

    /// Access the database connection
    QSqlDatabase *database() const { return m_database; }

private:
    BConnectionProfile m_profile;
    ServerCapabilities m_capabilities;
    BConnectionWizardData *m_wizardData;  ///< External data storage (not owned)
    QSqlDatabase *m_database;  ///< Database connection (not owned)
};

// Base class for Q&A pages
class QAPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit QAPage(const QString &question, QWidget *parent = nullptr);
protected:
    QLabel *m_questionLabel;
    QLabel *m_hintLabel;
    QVBoxLayout *m_layout;
    void setHint(const QString &hint);
};

// Pages
class WelcomePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit WelcomePage(QWidget *parent = nullptr);
};

class TemplateSelectionPage : public QAPage
{
    Q_OBJECT
public:
    explicit TemplateSelectionPage(QWidget *parent = nullptr);
    void initializePage() override;
    int nextId() const override;
    bool isComplete() const override;
private:
    QRadioButton *m_newConnectionRadio;
    QRadioButton *m_fromTemplateRadio;
    QRadioButton *m_importZipRadio;
    QButtonGroup *m_group;
    QComboBox *m_templateCombo;
    QPushButton *m_refreshButton;
    QPushButton *m_browseZipButton;
    QLineEdit *m_zipFileEdit;
    QLabel *m_templateDetailsLabel;
    void loadTemplates();
    void loadTemplateDetails(int directorId);
private slots:
    void onSelectionChanged();
    void onTemplateSelected(int index);
    void onRefreshClicked();
    void onBrowseZipClicked();
};

class ServerPage : public QAPage
{
    Q_OBJECT
public:
    explicit ServerPage(QWidget *parent = nullptr);
    bool isComplete() const override;
    bool validatePage() override;
private:
    QLineEdit *m_hostEdit;
    QSpinBox *m_portSpin;
    QPushButton *m_checkButton;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    bool m_checkInProgress;
    bool m_checkCompleted;
    void checkCapabilities();
    void onCheckComplete(bool reachable);
private slots:
    void onCheckClicked();
};

class CredentialsPage : public QAPage
{
    Q_OBJECT
public:
    explicit CredentialsPage(QWidget *parent = nullptr);
    bool isComplete() const override;
    bool validatePage() override;
private:
    QLineEdit *m_directorEdit;
    QLineEdit *m_consoleEdit;
    QLineEdit *m_passwordEdit;
    QLabel *m_md5Label;
    QCheckBox *m_saveCheck;
};

class AuthMethodPage : public QAPage
{
    Q_OBJECT
public:
    explicit AuthMethodPage(QWidget *parent = nullptr);
    void initializePage() override;
    int nextId() const override;
    QString authMethod() const;
private:
    QRadioButton *m_pskRadio;
    QRadioButton *m_legacyRadio;
    QRadioButton *m_certRadio;
    QButtonGroup *m_group;
    QLabel *m_capabilityLabel;
    void updateCapabilityHints();
private slots:
    void onSelectionChanged();
};

class TLSPage : public QAPage
{
    Q_OBJECT
public:
    explicit TLSPage(QWidget *parent = nullptr);
    bool validatePage() override;
private:
    QLineEdit *m_caCertEdit;
    QLineEdit *m_clientCertEdit;
    QLineEdit *m_clientKeyEdit;
    QPushButton *m_caBrowseButton;
    QPushButton *m_clientCertBrowseButton;
    QPushButton *m_clientKeyBrowseButton;
    QPushButton *m_generateButton;
private slots:
    void browseCaCert();
    void browseClientCert();
    void browseClientKey();
    void generateCertificates();
    void convertToPFX();
};

class ConfigPreviewPage : public QAPage
{
    Q_OBJECT
public:
    explicit ConfigPreviewPage(QWidget *parent = nullptr);
    void initializePage() override;
    int nextId() const override;
private:
    QTextEdit *m_consoleConfigEdit;
    QTextEdit *m_directorConfigEdit;
    QPushButton *m_copyConsoleButton;
    QPushButton *m_copyDirectorButton;
    void updateConfigs();
private slots:
    void onCopyConsoleConfig();
    void onCopyDirectorConfig();
};

class TestPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit TestPage(QWidget *parent = nullptr);
    ~TestPage() override;
    void initializePage() override;
    void cleanupPage() override;
    bool isComplete() const override;
    int nextId() const override;
private:
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    QTextEdit *m_logEdit;
    QPushButton *m_testButton;
    QTimer *m_timeoutTimer;
    BareosDirector *m_testDirector;
    bool m_testSucceeded;
    bool m_testRunning;
    void startTest();
    void stopTest();
    void appendLog(const QString &msg, bool error = false);
private slots:
    void onTestClicked();
    void onStateChanged(int oldState, int newState);
    void onAuthResult(bool success, const QString &msg);
    void onProtocolError(const QString &error);
    void onResourcesLoaded();
    void onTimeout();
};

class ConsoleSetupPage : public QAPage
{
    Q_OBJECT
public:
    explicit ConsoleSetupPage(QWidget *parent = nullptr);
    void initializePage() override;
    bool validatePage() override;
    bool isComplete() const override;
private:
    QRadioButton *m_modifyExistingRadio;
    QRadioButton *m_createNewRadio;
    QButtonGroup *m_group;
    QComboBox *m_consoleCombo;
    QTextEdit *m_consoleDetails;
    QLineEdit *m_newNameEdit;
    QLineEdit *m_newPasswordEdit;
    QPushButton *m_generatePasswordButton;
    QPushButton *m_refreshButton;
    QLabel *m_statusLabel;
    BareosDirector *m_director;
    bool m_consolesLoaded;
    void loadConsoles();
    void loadConsoleDetails(const QString &name);
    void generatePassword();
    void createConsole();
private slots:
    void onSelectionChanged();
    void onRefreshClicked();
    void onConsoleSelected(int index);
    void onGeneratePasswordClicked();
    void onJsonResponse(const QString &cmd, const QString &json);
};

class ProfileNamePage : public QAPage
{
    Q_OBJECT
public:
    explicit ProfileNamePage(QWidget *parent = nullptr);
    void initializePage() override;
    bool validatePage() override;
private slots:
    void onAdvancedSettingsClicked();
    void onExportConfigClicked();
private:
    QLineEdit *m_edit;
    QCheckBox *m_defaultCheck;
    QCheckBox *m_connectCheck;
    QPushButton *m_advancedButton;
    QPushButton *m_exportButton;
};

#endif // BCONNECTIONWIZARD_H
