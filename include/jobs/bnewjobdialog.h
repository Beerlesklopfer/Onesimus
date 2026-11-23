// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BNEWJOBDIALOG_H
#define BNEWJOBDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>
#include <QTabWidget>
#include "director/bdirector.h"
#include "jobs/brunjobcommand.h"

class BJobWidget;  // Forward declaration

/**
 * @brief Dialog for creating and running new backup jobs
 * @since 1.0
 *
 * Inspired by Baculum's job creation interface, this dialog provides
 * a structured way to configure and run backup jobs with all necessary
 * parameters.
 *
 * @note Now gets configuration data from BJobWidget instead of loading it directly
 * @since 2.8
 */
class BNewJobDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BNewJobDialog(BJobWidget *jobWidget, BDirector *director, QWidget *parent = nullptr);
    ~BNewJobDialog();

    /**
     * @brief Get the job command built from the current input
     * @return Complete run command, empty if the input is not valid
     */
    QString getJobCommand() const;

private slots:
    void onJobChanged(int index);
    void onClientChanged(int index);
    void onLevelChanged(int index);
    void onRunClicked();
    void onEstimateClicked();
    void onRefreshData();
    void onJsonResponse(BDirector::Command cmd, const QString &jsonData);
    void onDotJobsReceived(const QString &jsonData);
    void onDotClientsReceived(const QString &jsonData);
    void onDotDefaultsReceived(const QString &jsonData);
    // Note: Fileset, Storage, Pool data now comes from JobWidget

private:
    void setupUI();
    void createBasicTab();
    void createAdvancedTab();
    void loadConfigurationDataFromJobWidget();
    void updateJobDefaults();
    void buildRunCommand();
    void updateButtonState();
    BRunJobCommand::Params currentParams() const;
    static QString commandErrorText(const BRunJobCommand::Result &result);

    BJobWidget *m_jobWidget;
    BDirector *m_director;

    // UI Components - Basic Tab
    QTabWidget *m_tabWidget;
    QComboBox *m_jobCombo;
    QComboBox *m_clientCombo;
    QComboBox *m_filesetCombo;
    QComboBox *m_poolCombo;
    QComboBox *m_storageCombo;
    QComboBox *m_levelCombo;
    QSpinBox *m_prioritySpin;

    // UI Components - Advanced Tab
    QLineEdit *m_whenEdit;
    QCheckBox *m_bootstrapCheck;
    QLineEdit *m_bootstrapEdit;
    QCheckBox *m_replaceCheck;
    QComboBox *m_replaceCombo;
    QTextEdit *m_commandPreview = nullptr;

    // Status
    QLabel *m_statusLabel;
    QPushButton *m_runButton = nullptr;
    QPushButton *m_estimateButton;

    // Data storage
    QStringList m_jobNames;
    QStringList m_clientNames;
    QStringList m_filesetNames;
    QStringList m_poolNames;
    QStringList m_storageNames;

    bool m_dataLoaded;

    // Command as shown in the preview; rebuilt on every input change and before sending
    QString m_command;
    QString m_commandError;         ///< Not empty: the input cannot be sent
    bool m_priorityEdited = false;  ///< Priority is only sent once the user has set it
};

#endif // BNEWJOBDIALOG_H
