#ifndef BCONFIGIMPORTDIALOG_H
#define BCONFIGIMPORTDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QProgressBar>
#include <QSpinBox>
#include <QTemporaryDir>
#include "config/bconfigparser.h"
#include "director/bresourcewidgets.h"

/**
 * @file bconfigimportdialog.h
 * @brief Tabbed dialog for importing Bareos/Bacula Director configuration
 *
 * This dialog allows users to import configuration from:
 * - A directory tree (e.g., /etc/bareos/bareos-dir.d/)
 * - A ZIP archive containing configuration files
 *
 * Each resource type is displayed in its own tab with specialized widgets.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */
class BConfigImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BConfigImportDialog(QWidget *parent = nullptr);
    ~BConfigImportDialog();

    /**
     * @brief Get the selected Director resource
     */
    BConfigResource selectedDirector() const;

    /**
     * @brief Get the selected Console resource
     */
    BConfigResource selectedConsole() const;

    /**
     * @brief Check if user wants to create a connection profile
     */
    bool createConnectionProfile() const;

signals:
    /**
     * @brief Emitted when import is complete and accepted
     * @param director The selected Director resource
     * @param console The selected Console resource
     * @param host The hostname/address to connect to
     * @param port The port number
     */
    void configurationImported(const BConfigResource &director, const BConfigResource &console,
                               const QString &host, int port);

private slots:
    void onBrowseDirectory();
    void onBrowseZipFile();
    void onParse();
    void onResourceParsed(const QString &type, const QString &name);
    void onParsingComplete(int resourceCount);
    void onParsingError(const QString &error);
    void onDirectorSelected(const BConfigResource &resource);
    void onConsoleSelected(const BConfigResource &resource);
    void onAccept();

private:
    void setupUI();
    void setupTabs();
    void populateTabs();
    void updateConnectionPreview();
    void validateSelection();
    void cleanupTempDir();

    /**
     * @brief Validate parsed resources against directive schemas
     * @return List of validation warnings/errors (empty if all valid)
     */
    QStringList validateResources() const;

    /**
     * @brief Display validation results in the validation label
     */
    void updateValidationDisplay();

    // Source selection
    QLineEdit *m_pathEdit;
    QPushButton *m_browseDirectoryButton;
    QPushButton *m_browseZipButton;
    QPushButton *m_parseButton;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;

    // Tab widget
    QTabWidget *m_tabWidget;

    // Resource widgets (one per tab)
    BDirectorResourceWidget *m_directorWidget;
    BConsoleResourceWidget *m_consoleWidget;
    BClientResourceWidget *m_clientWidget;
    BJobResourceWidget *m_jobWidget;
    BStorageResourceWidget *m_storageWidget;
    BFileSetResourceWidget *m_fileSetWidget;
    BPoolResourceWidget *m_poolWidget;
    BScheduleResourceWidget *m_scheduleWidget;
    BMessagesResourceWidget *m_messagesWidget;
    BCatalogResourceWidget *m_catalogWidget;

    // Connection selection
    QGroupBox *m_connectionGroup;
    QComboBox *m_directorCombo;
    QComboBox *m_consoleCombo;
    QLineEdit *m_addressEdit;       ///< Remote address/hostname to connect to
    QSpinBox *m_portSpinBox;        ///< Port number (default 9101)
    QCheckBox *m_createProfileCheck;

    // Preview
    QGroupBox *m_previewGroup;
    QLabel *m_previewDirectorLabel;
    QLabel *m_previewAddressLabel;
    QLabel *m_previewPortLabel;
    QLabel *m_previewConsoleLabel;
    QLabel *m_previewTlsLabel;

    // Validation feedback
    QLabel *m_validationLabel;

    // Buttons
    QPushButton *m_importButton;
    QPushButton *m_cancelButton;

    // Parser
    BConfigParser *m_parser;

    // Temp directory for ZIP extraction
    QTemporaryDir *m_tempDir;
};

#endif // BCONFIGIMPORTDIALOG_H
