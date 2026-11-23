// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSETTINGSDIALOG_H
#define BSETTINGSDIALOG_H

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
#include "director/bdirector.h"
#include "bconnectionprofile.h"
#include <QColor>
#include <QMap>

namespace Ui {
class BSettingsDialog;
}

/**
 * @brief Moderner Settings Dialog mit Kategorien-Navigation
 * 
 * Design: Industriell-dunkel mit klaren Kategorien
 * Layout: Sidebar-Navigation + Content-Bereich
 */
class BSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the settings dialog
     * @param director Pointer to BDirector (for connection settings)
     * @param availableLevels List of available backup levels from Director (code, name pairs)
     * @param parent Parent widget
     */
    explicit BSettingsDialog(BDirector *director,
                           const QList<QPair<QString, QString>> &availableLevels = QList<QPair<QString, QString>>(),
                           QWidget *parent = nullptr);
    ~BSettingsDialog();

    // Einstellungen laden/speichern
    void loadSettings();
    /// Writes everything the dialog shows, the profiles included. Nothing
    /// is written before: Cancel leaves the stored settings as they were.
    void saveSettings();

    /**
     * @brief Resets the stored settings to their defaults and reloads the dialog
     * @param includeProfiles false: the connection profiles are kept
     */
    void resetToDefaults(bool includeProfiles);

    /// Writes the profiles of the dialog (with the edits not saved yet) to
    /// @p path; secrets only with @p includeSecrets
    bool exportProfiles(const QString &path, bool includeSecrets, QString *error = nullptr);

    /// Reads profiles from @p path into the dialog; saved with the dialog.
    /// @return Number of profiles read, 0 on error
    int importProfiles(const QString &path, QString *error = nullptr);

    /// The profiles as the dialog holds them (not saved yet)
    QList<BConnectionProfile> workingProfiles() const { return m_profiles; }

public slots:
    void reject() override;

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
    void onTransportChanged();          ///< BNET/gRPC: enables gRPC port, hides BNET TLS options
    void onForgetGrpcCertificate();     ///< Clears the pinned gRPC certificate fingerprint
    void onChooseColorFull();
    void onChooseColorIncremental();
    void onChooseColorDifferential();
    void onChooseColorVirtualFull();

    // Status Bar Color slots
    void onChooseColorConnected();
    void onChooseColorDisconnected();

    // PFX Converter (Windows only)
    void onConvertToPFX();

private:
    void setupUI();
    void createSidebar();
    void createContentPages();
    void createConnectionPage();
    void createAppearancePage();
    void createBehaviorPage();
    void createAdvancedPage();
    void applyModernStyle();
    void saveCurrentProfileSilent();  ///< Takes the form into the working copy of the current profile
    int profileRow(const QString &profileId) const;  ///< Row of a profile in the list, -1 if none
    int profileIndex(const QString &profileId) const; ///< Index in m_profiles, -1 if none
    void addProfileToList(const BConnectionProfile &profile, bool select);
    void showColor(QPushButton *button, const QColor &color);
    void chooseLevelColor(const QString &level, QPushButton *button, const QString &title);
    void updateGrpcFingerprintDisplay(); ///< Shows the pinned fingerprint (short, full as tooltip)

    Ui::BSettingsDialog *ui;
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
    QPushButton *m_deleteProfileButton;
    QPushButton *m_duplicateProfileButton;
    QWidget *m_profileDetailsWidget;
    QString m_currentProfileId;
    // Working copies: written by saveSettings() only, so Cancel reverts them
    QList<BConnectionProfile> m_profiles;
    QString m_lastUsedProfileId;
    QMap<QString, QColor> m_levelColors;   ///< Level code ("F", "I", ...) -> colour
    QColor m_connectedColor;
    QColor m_disconnectedColor;

    // Verbindungseinstellungen (for current profile)
    QLineEdit *m_profileNameEdit;
    QLineEdit *m_hostEdit;
    QSpinBox *m_portSpin;
    QComboBox *m_serverPlatformCombo;
    QLineEdit *m_directorEdit;
    QLineEdit *m_consoleEdit;
    QLineEdit *m_passwordEdit;
    QLabel *m_md5Label;
    QCheckBox *m_savePasswordCheck;
    QCheckBox *m_autoConnectCheck;

    // Protocol (BNET or gRPC with the Onesimus Director)
    QComboBox *m_transportCombo;
    QSpinBox *m_grpcPortSpin;
    QLabel *m_grpcFingerprintLabel;     ///< Pinned certificate fingerprint (read-only)
    QPushButton *m_forgetGrpcCertButton;
    QLabel *m_grpcInfoLabel;            ///< Hint shown instead of the BNET TLS options
    QString m_grpcCertFingerprint;      ///< Working copy of the pinned fingerprint

    // TLS-Einstellungen
    QGroupBox *m_tlsGroupBox;
    QCheckBox *m_useCertificatesCheck;  ///< Use x509 certificates instead of PSK
    QWidget *m_certWidget;              ///< Container for certificate fields
    QLineEdit *m_caCertEdit;
    QLineEdit *m_clientCertEdit;
    QLineEdit *m_clientKeyEdit;
    QCheckBox *m_verifyPeerCheck;
    QLineEdit *m_cipherListEdit;        ///< PSK cipher list (colon-separated)

    // Appearance-Einstellungen
    QSpinBox *m_uiZoomSpin;     ///< Interface zoom in percent
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
    QSpinBox *m_clientStaleHoursSpin;
    QSpinBox *m_scheduleHistoryDaysSpin;
    QCheckBox *m_jobsNewestFirstCheck;

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

#endif // BSETTINGSDIALOG_H
