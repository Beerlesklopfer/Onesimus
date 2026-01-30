/**
 * @file bconnectionwizard.h
 * @brief Director Configuration Wizard (Q&A Style)
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

/**
 * @class BConnectionWizard
 * @brief Q&A wizard for director configuration
 *
 * Pages:
 *   1. Welcome
 *   2. Server (Host + Port)
 *   3. Credentials (Director, Console, Password)
 *   4. AuthMethod
 *   5. TLS (only if cert auth selected)
 *   6. Test
 *   7. ProfileName
 */
class BConnectionWizard : public QWizard
{
    Q_OBJECT

public:
    enum PageId {
        Page_Welcome,
        Page_Server,
        Page_Credentials,
        Page_AuthMethod,
        Page_TLS,
        Page_Test,
        Page_ConsoleSetup,
        Page_ProfileName
    };

    explicit BConnectionWizard(QWidget *parent = nullptr);
    ~BConnectionWizard() override;

    BConnectionProfile profile() const;
    void setProfile(const BConnectionProfile &profile);

private:
    BConnectionProfile m_profile;
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

class ServerPage : public QAPage
{
    Q_OBJECT
public:
    explicit ServerPage(QWidget *parent = nullptr);
private:
    QLineEdit *m_hostEdit;
    QSpinBox *m_portSpin;
};

class CredentialsPage : public QAPage
{
    Q_OBJECT
public:
    explicit CredentialsPage(QWidget *parent = nullptr);
private:
    QLineEdit *m_directorEdit;
    QLineEdit *m_consoleEdit;
    QLineEdit *m_passwordEdit;
    QCheckBox *m_saveCheck;
};

class AuthMethodPage : public QAPage
{
    Q_OBJECT
public:
    explicit AuthMethodPage(QWidget *parent = nullptr);
    int nextId() const override;
    QString authMethod() const;
private:
    QRadioButton *m_pskRadio;
    QRadioButton *m_legacyRadio;
    QRadioButton *m_certRadio;
    QButtonGroup *m_group;
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
    QRadioButton *m_useCurrentRadio;
    QRadioButton *m_selectExistingRadio;
    QRadioButton *m_createNewRadio;
    QButtonGroup *m_group;
    QComboBox *m_consoleCombo;
    QLineEdit *m_newNameEdit;
    QLineEdit *m_newPasswordEdit;
    QPushButton *m_generatePasswordButton;
    QPushButton *m_refreshButton;
    QLabel *m_statusLabel;
    BareosDirector *m_director;
    bool m_consolesLoaded;
    void loadConsoles();
    void generatePassword();
    void createConsole();
private slots:
    void onSelectionChanged();
    void onRefreshClicked();
    void onGeneratePasswordClicked();
    void onJsonResponse(const QString &cmd, const QString &json);
};

class ProfileNamePage : public QAPage
{
    Q_OBJECT
public:
    explicit ProfileNamePage(QWidget *parent = nullptr);
    void initializePage() override;
private:
    QLineEdit *m_edit;
    QCheckBox *m_defaultCheck;
    QCheckBox *m_connectCheck;
};

#endif // BCONNECTIONWIZARD_H
