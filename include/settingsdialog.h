#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>
#include <QSettings>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>
#include "bdirector.h"

namespace Ui {
class SettingsDialog;
}

/**
 * @brief Moderner Settings Dialog mit Kategorien-Navigation
 * 
 * Design: Industriell-dunkel mit klaren Kategorien
 * Layout: Sidebar-Navigation + Content-Bereich
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the settings dialog
     * @param director Pointer to BDirector (for connection settings)
     * @param availableLevels List of available backup levels from Director (code, name pairs)
     * @param parent Parent widget
     */
    explicit SettingsDialog(BDirector *director,
                           const QList<QPair<QString, QString>> &availableLevels = QList<QPair<QString, QString>>(),
                           QWidget *parent = nullptr);
    ~SettingsDialog();

    // Einstellungen laden/speichern
    void loadSettings();
    void saveSettings();

signals:
    void settingsChanged();

private slots:
    void onCategoryChanged(int index);
    void onApplyClicked();
    void onCancelClicked();
    void onResetToDefaultsClicked();
    void onBrowseCACert();
    void onBrowseClientCert();
    void onBrowseClientKey();
    void onExportSettings();
    void onImportSettings();
    void onClearStoredConnections();

    // Connection Profile Management
    void onProfileSelectionChanged();
    void onAddProfile();
    void onEditProfile();
    void onDeleteProfile();
    void onDuplicateProfile();
    void saveCurrentProfile();
    void onChooseColorFull();
    void onChooseColorIncremental();
    void onChooseColorDifferential();
    void onChooseColorVirtualFull();

    // Status Bar Color slots
    void onChooseColorConnected();
    void onChooseColorDisconnected();

private:
    void setupUI();
    void createSidebar();
    void createContentPages();
    void createConnectionPage();
    void createAppearancePage();
    void createBehaviorPage();
    void createAdvancedPage();
    void applyModernStyle();
    
    Ui::SettingsDialog *ui;
    BDirector *m_director;
    
    // UI-Komponenten
    QListWidget *m_categoryList;
    QStackedWidget *m_contentStack;
    QPushButton *m_applyButton;
    QPushButton *m_cancelButton;
    QPushButton *m_resetButton;
    
    // Settings-Seiten
    QWidget *m_connectionPage;
    QWidget *m_appearancePage;
    QWidget *m_behaviorPage;
    QWidget *m_advancedPage;
    
    // Connection Profile Management
    QListWidget *m_profileList;
    QPushButton *m_addProfileButton;
    QPushButton *m_editProfileButton;
    QPushButton *m_deleteProfileButton;
    QPushButton *m_duplicateProfileButton;
    QWidget *m_profileDetailsWidget;
    QString m_currentProfileId;

    // Verbindungseinstellungen (for current profile)
    QLineEdit *m_profileNameEdit;
    QLineEdit *m_hostEdit;
    QSpinBox *m_portSpin;
    QLineEdit *m_directorEdit;
    QLineEdit *m_consoleEdit;
    QLineEdit *m_passwordEdit;
    QCheckBox *m_savePasswordCheck;
    QCheckBox *m_autoConnectCheck;
    QSpinBox *m_connectionTimeoutSpin;

    // TLS-Einstellungen (checkable GroupBox mit RadioButtons)
    QGroupBox *m_tlsGroupBox;
    QRadioButton *m_tlsLegacyRadio;
    QRadioButton *m_tlsPSKRadio;
    QRadioButton *m_tlsCertificateRadio;
    QWidget *m_certWidget;          ///< Container for certificate fields
    QLineEdit *m_caCertEdit;
    QLineEdit *m_clientCertEdit;
    QLineEdit *m_clientKeyEdit;
    QCheckBox *m_verifyPeerCheck;
    
    // Appearance-Einstellungen
    QComboBox *m_themeCombo;
    QSpinBox *m_fontSizeSpin;
    QCheckBox *m_animationsCheck;
    QCheckBox *m_compactModeCheck;
    QComboBox *m_languageCombo;

    // Level Color Buttons
    QPushButton *m_colorButtonFull;
    QPushButton *m_colorButtonIncremental;
    QPushButton *m_colorButtonDifferential;
    QPushButton *m_colorButtonVirtualFull;

    // Status Bar Color Buttons
    QPushButton *m_colorButtonConnected;
    QPushButton *m_colorButtonDisconnected;
    
    // Behavior-Einstellungen
    QCheckBox *m_confirmJobCancelCheck;
    QCheckBox *m_confirmJobStartCheck;
    QCheckBox *m_autoRefreshCheck;
    QSpinBox *m_refreshIntervalSpin;
    QSpinBox *m_maxJobsDisplaySpin;

    // Visible Backup Levels (dynamic from Director)
    QList<QPair<QString, QString>> m_availableLevels;  ///< Available levels (name, name) from Director
    QMap<QString, QCheckBox*> m_levelCheckboxes;       ///< Dynamic checkboxes for each level
    QVBoxLayout *m_levelsLayout;                       ///< Layout for level checkboxes
    
    // Advanced-Einstellungen
    QCheckBox *m_debugLoggingCheck;
    QLineEdit *m_logFileEdit;
    QSpinBox *m_maxLogSizeSpin;
    QCheckBox *m_enableTooltipsCheck;
    QComboBox *m_backupSystemCombo;
};

#endif // SETTINGSDIALOG_H
