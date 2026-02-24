/**
 * @file bprofilesettingsdialog.h
 * @brief Advanced settings dialog for connection profiles
 *
 * Provides UI for configuring advanced TLS settings, ACLs, and other
 * connection parameters that are not part of the basic wizard flow.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2025
 */

#ifndef BPROFILESETTINGSDIALOG_H
#define BPROFILESETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QDialogButtonBox>

#include "bconnectionprofile.h"

/**
 * @class BProfileSettingsDialog
 * @brief Dialog for advanced connection profile settings
 *
 * This dialog provides a tabbed interface for configuring:
 * - General settings (description, heartbeat)
 * - Advanced TLS settings (ciphers, protocols, CRL)
 * - Console ACLs (job, client, storage, etc.)
 *
 * The dialog operates on a copy of the profile and only applies
 * changes when the user clicks OK.
 */
class BProfileSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the advanced settings dialog
     * @param profile The connection profile to edit
     * @param parent Parent widget
     */
    explicit BProfileSettingsDialog(const BConnectionProfile &profile, QWidget *parent = nullptr);

    /**
     * @brief Returns the modified profile
     * @return The profile with applied changes
     */
    BConnectionProfile profile() const;

private slots:
    /**
     * @brief Validates and accepts the dialog
     */
    void onAccept();

    /**
     * @brief Resets all fields to their default values
     */
    void onResetDefaults();

private:
    /**
     * @brief Sets up the user interface
     */
    void setupUi();

    /**
     * @brief Creates the General tab
     * @return Widget containing general settings
     */
    QWidget* createGeneralTab();

    /**
     * @brief Creates the TLS tab
     * @return Widget containing TLS settings
     */
    QWidget* createTlsTab();

    /**
     * @brief Creates the ACL tab
     * @return Widget containing ACL settings
     */
    QWidget* createAclTab();

    /**
     * @brief Loads profile data into UI widgets
     */
    void loadProfile();

    /**
     * @brief Saves UI widget values to profile
     */
    void saveProfile();

    BConnectionProfile m_profile;       ///< Working copy of the profile

    // Tab widget
    QTabWidget *m_tabWidget;
    QDialogButtonBox *m_buttonBox;
    QPushButton *m_resetButton;

    // General tab widgets
    QLineEdit *m_descriptionEdit;
    QSpinBox *m_heartbeatSpin;
    QLineEdit *m_profileEdit;           ///< Director Profile reference

    // TLS tab widgets
    QCheckBox *m_tlsRequireCheck;
    QCheckBox *m_tlsAuthenticateCheck;
    QLineEdit *m_tlsCipherListEdit;
    QLineEdit *m_tlsCipherSuitesEdit;
    QLineEdit *m_tlsDhFileEdit;
    QLineEdit *m_tlsProtocolEdit;
    QTextEdit *m_tlsAllowedCnEdit;
    QLineEdit *m_tlsCrlFileEdit;
    QPushButton *m_browseDhFileButton;
    QPushButton *m_browseCrlFileButton;

    // ACL tab widgets
    QTextEdit *m_aclJobEdit;
    QTextEdit *m_aclClientEdit;
    QTextEdit *m_aclStorageEdit;
    QTextEdit *m_aclScheduleEdit;
    QTextEdit *m_aclPoolEdit;
    QTextEdit *m_aclFileSetEdit;
    QTextEdit *m_aclCatalogEdit;
    QTextEdit *m_aclCommandEdit;
    QTextEdit *m_aclWhereEdit;
    QTextEdit *m_aclPluginOptionsEdit;
};

#endif // BPROFILESETTINGSDIALOG_H
