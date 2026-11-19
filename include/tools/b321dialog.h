#ifndef B321DIALOG_H
#define B321DIALOG_H

#include <QDateTime>
#include <QDialog>
#include <QJsonObject>
#include <QMap>
#include <QPointer>
#include <QStringList>

#include <functional>

#include "director/bdirector.h"
#include "tools/b321verifier.h"

class QAction;
class QCheckBox;
class QLabel;
class QListWidget;
class QPushButton;
class QTableWidget;
class QTextBrowser;
class QTreeWidget;
class QSplitter;

/**
 * @brief Tools > 3-2-1 Verifier: does each backup job follow the 3-2-1 rule?
 *
 * A window of its own (Tools menu) rather than a panel of the Jobs or the
 * Storage tab: the check needs the jobs, their copies, the volumes and the
 * storages at once, and belongs to neither tab alone.
 *
 * One row per Job resource of type Backup with three chips (copies x/3,
 * media types y/2, off site z/1; green only for a met criterion, red for a
 * failed one, grey for unknown), the verdict and the main reason. The detail
 * pane lists the chain a restore would use, its copies and where they lie,
 * with the reasons, suggestions and hints of B321Verifier.
 *
 * Read only. Refresh sends three read commands, all complete Bareos 25
 * commands that the Onesimus Director knows too:
 *  - "llist jobs days=3650 reverse limit=20000" (Command::LlistJobs, the job
 *    records with PriorJobId, pool, client and FileSet),
 *  - "list jobmedia" (Command::ListJobMedia, JobId and volume of every
 *    JobMedia record),
 *  - "llist volumes" (Command::LlistVolumes, the volume list the Storage tab
 *    shows: MediaType, storage, status, LocationId).
 * A volume list the Storage tab loads is taken as well.
 *
 * The user's off-site markings (per Director profile and storage), "unmarked
 * storages are on site" and "count the original data as a copy" are kept in
 * BSettings.
 */
class B321Dialog : public QDialog
{
    Q_OBJECT

public:
    /// What the main window knows and the verifier needs, read at each refresh
    struct Context
    {
        QMap<QString, QJsonObject> jobs;     ///< "show jobs", by name
        QMap<QString, QJsonObject> jobDefs;  ///< "show jobdefs", by name
        QStringList storages;                ///< configured storages (".storages")
        QString profileId;                   ///< Director profile of the connection
    };
    using ContextProvider = std::function<Context()>;

    enum Column {
        JobColumn = 0,
        ClientColumn,
        CopiesColumn,
        MediaColumn,
        OffSiteColumn,
        VerdictColumn,
        ReasonColumn,
        ColumnCount
    };

    /// Job window and row limit of the job request ("<days>,<limit>")
    static constexpr int HistoryDays = 3650;
    static constexpr int HistoryLimit = 20000;

    explicit B321Dialog(ContextProvider context, QWidget *parent = nullptr);
    ~B321Dialog() override;

    /**
     * @brief The Tools menu entry: opens the one window, or raises it
     *
     * The window is made on first use, wired to the Director (its replies
     * in, sendCommand() out) and deleted when it is closed. Defined in
     * b321action.cpp, so that the dialog itself needs no Director.
     */
    static QAction *createAction(QWidget *parent, BDirector *director, ContextProvider context);

    /// Whether there is a connection; without one Refresh sends nothing
    void setConnected(bool connected);

    /// The Job resources of type Backup in "show jobs" (type from the JobDefs if needed)
    static QList<B321Verifier::Definition> backupDefinitions(const QMap<QString, QJsonObject> &jobs,
                                                             const QMap<QString, QJsonObject> &jobDefs);

    /// Fixes the present (tests and pictures); invalid = now
    void setReferenceTime(const QDateTime &now);

    QTableWidget *table() const { return m_table; }
    QTreeWidget *detailTree() const { return m_tree; }
    QTextBrowser *detailText() const { return m_text; }
    QListWidget *offSiteList() const { return m_offSiteList; }
    QCheckBox *countOriginalBox() const { return m_countOriginal; }
    QCheckBox *unmarkedOnSiteBox() const { return m_unmarkedOnSite; }
    QLabel *stateLabel() const { return m_stateLabel; }
    const QList<B321Verifier::Result> &results() const { return m_results; }

    /// The args of the job request: "<HistoryDays>,<HistoryLimit>"
    static QString jobRequestArgs();

public slots:
    /// Reads the context and asks the Director for jobs, job media and volumes
    void refresh();

    /// Reply to Command::LlistJobs (BDirector::jobHistoryResult)
    void processJobHistory(const QString &args, const QString &reply);

    /// Replies to Command::ListJobMedia, LlistVolumes and ListVolumes (BDirector::jsonResult)
    void processJsonResult(BDirector::Command cmd, const QString &json);

    /// A command of this window got no reply (BDirector::commandFailed)
    void processCommandFailed(BDirector::Command cmd, const QString &args, const QString &error);

signals:
    /// A read command to send (connected to the Director when there is one)
    void sendCommand(BDirector::Command cmd, const QString &args);

private:
    void setupUi();
    void readContext();
    void fillOffSiteList();
    void recompute();
    void fillTable();
    void showDetails();
    void updateState();
    void saveHeader();
    QStringList knownStorages() const;

    ContextProvider m_contextProvider;
    bool m_connected = false;
    QString m_profileId;
    QStringList m_configuredStorages;
    QDateTime m_referenceTime;

    B321Verifier::Input m_input;
    QList<B321Verifier::Result> m_results;

    bool m_jobsPending = false;
    bool m_mediaPending = false;
    bool m_volumesPending = false;
    QStringList m_errors;

    QLabel *m_stateLabel = nullptr;
    QPushButton *m_refreshButton = nullptr;
    QTableWidget *m_table = nullptr;
    QTreeWidget *m_tree = nullptr;
    QTextBrowser *m_text = nullptr;
    QListWidget *m_offSiteList = nullptr;
    QCheckBox *m_countOriginal = nullptr;
    QCheckBox *m_unmarkedOnSite = nullptr;
    QSplitter *m_mainSplitter = nullptr;
    QSplitter *m_leftSplitter = nullptr;
};

#endif // B321DIALOG_H
