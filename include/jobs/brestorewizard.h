/**
 * @file brestorewizard.h
 * @brief Restore Wizard for BVFS-based file restore
 *
 * Multi-page wizard: SelectJob → Browse → Options → Preview → Execute
 */

#ifndef BRESTOREWIZARD_H
#define BRESTOREWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QJsonObject>
#include <QDateTime>
#include <QTreeView>
#include <QTableView>
#include <QSplitter>
#include <QProgressBar>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDateTimeEdit>
#include <QTextEdit>
#include <QTimer>

#include "director/bdirector.h"
#include "jobs/bbvfsmodel.h"

// ============================================================================
// Data struct — preserves wizard state across page navigation
// ============================================================================

struct BRestoreWizardData {
    // Source (set on construction, from context menu selection)
    quint64 jobId = 0;
    QString jobName;
    QString sourceClient;
    QString sourceFileSet;

    // Restore scope (set by SelectJobPage)
    QString selectedClient;
    QString selectedFileSet;
    bool allRelatedJobs = true;
    QDateTime beforeDate;
    QString resolvedBvfsJobIds;

    // File selection (set by BrowsePage)
    QStringList selectedFileIds;
    QList<int> selectedDirIds;

    // Cached resource lists (loaded by SelectJobPage, shared with OptionsPage)
    QStringList clientNames;

    // Restore options (set by OptionsPage)
    QString targetClient;
    QString restoreWhere = "/tmp/bareos-restores";
    QString replacePolicy = "always";

    // Generated (set by PreviewPage)
    QString restoreTableName;
    QString bvfsRestoreCommand;
    QString restoreCommand;

    // Execution state
    bool restoreTableCreated = false;
    bool restoreSucceeded = false;
};

// ============================================================================
// Wizard
// ============================================================================

class BRestoreWizard : public QWizard
{
    Q_OBJECT

public:
    enum PageId {
        Page_SelectJob,
        Page_Browse,
        Page_Options,
        Page_Preview,
        Page_Execute
    };

    explicit BRestoreWizard(const QJsonObject &job, BDirector *director,
                            QWidget *parent = nullptr);
    ~BRestoreWizard() override;

    BRestoreWizardData *wizardData() { return &m_data; }
    BBvfsModel *bvfsModel() { return m_bvfsModel; }
    BDirector *director() { return m_director; }
    bool restoreSucceeded() const { return m_data.restoreSucceeded; }

protected:
    void reject() override;

private:
    BRestoreWizardData m_data;
    BBvfsModel *m_bvfsModel;
    BDirector *m_director;
};

// ============================================================================
// Page 1: Select Restore Job conditions
// ============================================================================

class BRestoreSelectJobPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BRestoreSelectJobPage(QWidget *parent = nullptr);
    void initializePage() override;
    bool validatePage() override;

private slots:
    void onScopeChanged();
    void onClientsResponse(BDirector::Command cmd, const QString &jsonData);
    void onFileSetsResponse(BDirector::Command cmd, const QString &jsonData);
    void onJobDetailResponse(BDirector::Command cmd, const QString &jsonData);
    void onLoadTimeout();

private:
    void preselectFileSet();

    QLabel *m_sourceLabel;
    QComboBox *m_clientCombo;
    QComboBox *m_fileSetCombo;
    QRadioButton *m_allRelatedRadio;
    QRadioButton *m_singleJobRadio;
    QButtonGroup *m_scopeGroup;
    QDateTimeEdit *m_beforeDateEdit;
    QLabel *m_errorLabel;
    QTimer *m_loadTimeoutTimer;
    bool m_clientsLoaded = false;
    bool m_fileSetsLoaded = false;
    bool m_jobDetailQueried = false;
};

// ============================================================================
// Page 2: Browse files with checkboxes
// ============================================================================

class BRestoreBrowsePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BRestoreBrowsePage(QWidget *parent = nullptr);
    void initializePage() override;
    bool isComplete() const override;
    bool validatePage() override;

private slots:
    void onTreeItemClicked(const QModelIndex &proxyIndex);
    void onTreeItemExpanded(const QModelIndex &proxyIndex);
    void onTreeCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void onFileListDoubleClicked(const QModelIndex &index);
    void onLoadingStarted();
    void onLoadingFinished();
    void onSelectionCountChanged(int count);

private:
    void showDirectoryInList(const QModelIndex &dirProxyIndex);

    QTreeView *m_treeView;
    QTableView *m_listView;
    QSplitter *m_splitter;
    QProgressBar *m_progressBar;
    QLabel *m_infoLabel;
    QLabel *m_selectionLabel;
    BBvfsDirFilterProxy *m_dirProxy = nullptr;
    QSortFilterProxyModel *m_listSortProxy = nullptr;
    bool m_loaded = false;
    bool m_initialExpandDone = false;
    QString m_loadedClient;
    QString m_loadedFileSet;
    bool m_loadedAllRelated = true;
};

// ============================================================================
// Page 3: Restore options
// ============================================================================

class BRestoreOptionsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BRestoreOptionsPage(QWidget *parent = nullptr);
    void initializePage() override;
    bool isComplete() const override;
    bool validatePage() override;

private slots:
    void updateReplacePolicyHelp(int index);

private:
    QLabel *m_sourceLabel;
    QComboBox *m_targetClientCombo;
    QLineEdit *m_restoreWhereEdit;
    QComboBox *m_replacePolicyCombo;
    QLabel *m_replacePolicyHelpLabel;
};

// ============================================================================
// Page 4: Command preview
// ============================================================================

class BRestorePreviewPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BRestorePreviewPage(QWidget *parent = nullptr);
    void initializePage() override;
    void cleanupPage() override;
    bool isComplete() const override;

private slots:
    void onHelpAllResponse(BDirector::Command cmd, const QString &jsonData);
    void onAuthCheckTimeout();

private:
    void disconnectAuth();
    void finishAuthChecks();
    void setAuthLabel(QLabel *label, bool authorized, const QString &detail = QString());

    QLabel *m_summaryLabel;
    QTextEdit *m_commandPreview;

    // Permission check UI
    QLabel *m_authRestoreLabel = nullptr;
    QLabel *m_authClientLabel = nullptr;
    QLabel *m_authWhereLabel = nullptr;
    QLabel *m_authStatusLabel = nullptr;  ///< Shows progress/timeout info
    QTimer *m_authTimeoutTimer = nullptr;
    bool m_authDone = false;
};

// ============================================================================
// Page 5: Execute restore
// ============================================================================

class BRestoreExecutePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BRestoreExecutePage(QWidget *parent = nullptr);
    void initializePage() override;
    bool isComplete() const override;

private slots:
    void onJsonResponse(BDirector::Command cmd, const QString &jsonData);
    void onCommandResponse(BDirector::Command cmd, const QString &response);
    void onTimeout();

private:
    enum ExecutionState {
        Idle,
        SendingBvfsRestore,
        SendingRestore,
        SendingCleanup,
        Completed,
        Failed
    };

    void advanceState();
    void sendBvfsRestore();
    void sendRestore();
    void sendCleanup();
    void markCompleted();
    void markFailed(const QString &reason);
    void disconnectDirector();
    void appendLog(const QString &msg, bool isError = false);
    void setStatus(const QString &msg);

    QProgressBar *m_progressBar;
    QTextEdit *m_logEdit;
    QLabel *m_statusLabel;
    QLabel *m_hintLabel;
    QTimer *m_timeoutTimer = nullptr;
    ExecutionState m_state = Idle;
    QString m_restoreJobId;
};

#endif // BRESTOREWIZARD_H
