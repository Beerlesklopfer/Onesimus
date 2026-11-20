#ifndef BSCHEDULEPERSISTENCE_H
#define BSCHEDULEPERSISTENCE_H

#include <QList>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>

#include <optional>

#include "director/bconfigclient.h"
#include "schedules/model/brunspec.h"
#include "schedules/model/bscheduleresource.h"

class BScheduleGridModel;

/**
 * @file bschedulepersistence.h
 * @brief Writing schedules to the Director: change, create, delete, each
 *        checked, shown and confirmed first
 *
 * Step 7 of the schedule plan, on the configuration methods of the Onesimus
 * Director (BConfigClient: ReadResource, CreateResource, UpdateResource,
 * DeleteResource). Every write goes the same way:
 *
 *   1. the raw text of the resource and its SHA-256 (ReadResource): the one
 *      read when the schedule was opened for editing (prepare()), or read
 *      now; it has to say the Run lines the GUI loaded, else nothing is
 *      written and the user is asked to reload;
 *   2. the new text: the change written into the raw text
 *      (BScheduleResource::rewrite()), untouched lines byte for byte;
 *   3. the same request with validate_only: the Director checks the whole
 *      new configuration and writes nothing;
 *   4. the review (BScheduleSaveUi::confirm()): old and new Run lines, the
 *      jobs that use the schedule, what the Director names as referencing
 *      it, the resource text;
 *   5. only after "Write": the request itself, with the SHA-256 of step 1.
 *
 * In the review the text after writing may be edited (Update, Create). An
 * edited text goes back to step 3: the Director checks it, the review shows
 * it again, and only a "Write" in that review sends it, still with the
 * SHA-256 of step 1.
 *
 * Every status of the Director is reported (BScheduleSaveUi::report()).
 * CONFLICT never leads to a second attempt with the new SHA-256: the user
 * sees both texts and can reload. A delete that is REFERENCED shows the
 * resources and does nothing.
 */

/// What the review shows before anything is written
struct BScheduleReview
{
    enum Operation { Update, Create, Delete };

    struct Row
    {
        enum State { Untouched, Changed, New, Removed };
        QString text;
        State state = Untouched;
    };

    Operation operation = Update;
    QString scheduleName;
    QString file;               ///< Path on the Director, if known
    bool sharedFile = false;    ///< The file holds other resources as well
    QList<Row> oldRows;         ///< Run lines as they are in the file; Changed and Removed marked
    QList<Row> newRows;         ///< Run lines after the change; Changed and New marked
    QStringList jobs;           ///< Jobs that use the schedule (job index of the GUI)
    QStringList referencedBy;   ///< "Job forgejo", as the Director names them
    QString oldText;            ///< The resource text now (empty for Create)
    QString newText;            ///< The resource text after the change (empty for Delete)
    QString note;               ///< What the change is, in a sentence (a drag, a grid edit)
};

/// The outcome of a write, or why there was none
struct BScheduleReport
{
    enum Kind {
        Written,        ///< Done; reloaded says whether the Director reloaded
        Cancelled,      ///< The user did not confirm; nothing was written
        Conflict,       ///< The Director has another state than the one the change started from
        FileDiffers,    ///< The file does not say what the GUI loaded (found before sending anything)
        Invalid,        ///< The Director rejected the new configuration; problem says where
        Referenced,     ///< Delete refused: still in use
        Denied,         ///< Resource ACL
        NotFound,
        Exists,         ///< Create: the name is taken
        ReloadFailed,   ///< Written, the reload failed, the Director put the old file back
        Locked,         ///< The schedule has a Run line the GUI does not understand
        NotAvailable,   ///< No configuration methods on this connection
        Failed,         ///< Anything else (gRPC error, unreadable text, ...)
    };

    Kind kind = Failed;
    BScheduleReview::Operation operation = BScheduleReview::Update;
    QString scheduleName;
    QString title;              ///< One line
    QString text;               ///< What happened and what it means, in plain words
    QString directorMessage;    ///< The Director's own message, if any
    std::optional<BConfig::Problem> problem;
    QString ourText;            ///< The text the GUI wanted to write
    QString currentText;        ///< The text the Director has (CONFLICT, FileDiffers)
    QStringList referencedBy;
    QStringList followups;
    bool reloaded = false;
    bool validatedOnly = false; ///< The status came from the check, nothing was sent for real
};

/**
 * @brief The dialogs of the flow, behind an interface (tests answer them)
 */
class BScheduleSaveUi
{
public:
    virtual ~BScheduleSaveUi() = default;

    /// The review; true only for "Write"
    virtual bool confirm(const BScheduleReview &review) = 0;

    /// What the review with an editable text asks for
    enum Decision { Cancel, Write, Recheck };
    /**
     * @brief The review, in which the text after writing may be edited
     * @param edited For Recheck: the text the user wrote instead. It is
     *        never written as it is: the Director checks it first and the
     *        review is shown again.
     *
     * By default the review without editing (confirm()).
     */
    virtual Decision decide(const BScheduleReview &review, QString *edited)
    {
        Q_UNUSED(edited);
        return confirm(review) ? Write : Cancel;
    }

    enum Answer { Close, Reload };
    /// A report; Reload is offered for Conflict and FileDiffers
    virtual Answer report(const BScheduleReport &report) = 0;
};

class BSchedulePersistence : public QObject
{
    Q_OBJECT

public:
    BSchedulePersistence(BConfigClient *client, BScheduleSaveUi *ui, QObject *parent = nullptr);

    /// The client may change with the connection
    void setClient(BConfigClient *client) { m_client = client; }
    BConfigClient *client() const { return m_client; }
    void setUi(BScheduleSaveUi *ui) { m_ui = ui; }

    /// A write is under way; another one is refused until it ends
    bool isBusy() const { return m_busy; }

    /**
     * @brief Reads the raw text of a schedule that is about to be edited
     *
     * Its SHA-256 is the one a later save sends, so that a change made on
     * the Director in between is a CONFLICT. Nothing is reported; a save
     * reads again if this did not work.
     */
    void prepare(const QString &scheduleName);
    /// The raw text kept for a schedule (empty if none)
    QString preparedText(const QString &scheduleName) const;
    void forget();

    /**
     * @brief Writes new Run lines of an existing schedule (UpdateResource)
     * @param loaded The schedule as the GUI loaded it
     * @param lines  The Run lines after the change (BScheduleResource::Line)
     * @param jobs   The jobs that use it, for the review
     * @param note   A sentence for the review
     * @return false if refused at once (busy, locked); a report says why
     */
    bool saveRuns(const BSchedule &loaded, const QList<BScheduleResource::Line> &lines,
                  const QStringList &jobs, const QString &note = QString(),
                  const std::optional<QString> &description = std::nullopt);

    /// Deletes a schedule (DeleteResource), after a check and the review
    bool deleteSchedule(const QString &scheduleName, const QStringList &jobs);

    /// Creates a schedule from its text (CreateResource), after a check and the review
    bool createSchedule(const QString &scheduleName, const QString &text);

    // --- Building the Run lines of a change ---------------------------------

    /// The lines of the grid model as they are now: kept, changed, new
    static bool linesFromModel(const BScheduleGridModel &model, QList<BScheduleResource::Line> *lines,
                               QString *error = nullptr);

    /**
     * @brief The lines after a drag: every line kept but the dragged one,
     *        which is written anew at its new time
     * @param loaded     The schedule as loaded
     * @param draggedRun Index of the dragged line
     * @param newText    Its new text (BScheduleDragPlan::Plan::generatedRuns)
     */
    static QList<BScheduleResource::Line> linesFromDrag(const BSchedule &loaded, int draggedRun,
                                                        const QString &newText);

signals:
    /// The flow is over (written or not); after Written the schedule is to be read again
    void finished(const BScheduleReport &report);
    /// The user asked to read the schedule again (after a conflict), or it was written
    void reloadRequested(const QString &scheduleName);
    void busyChanged(bool busy);

private:
    struct Prepared
    {
        QString name;
        QString text;
        QString sha256;
        QString file;
        bool sharedFile = false;
    };

    void setBusy(bool busy);
    void finish(const BScheduleReport &report, bool ask = true);
    BScheduleReport reportOf(const BConfig::Reply &reply, BScheduleReview::Operation operation,
                             const QString &name) const;
    void withRaw(const QString &name, const QList<BRunLine> &loaded,
                 std::function<void(const Prepared &raw)> next, BScheduleReview::Operation operation);
    void updateStep(const BSchedule &loaded, const QList<BScheduleResource::Line> &lines,
                    const QStringList &jobs, const QString &note, const Prepared &raw,
                    const std::optional<QString> &description);
    /// Steps 3 to 5 of an update; again for an edited text
    void checkUpdate(const BScheduleReview &review, const Prepared &raw);
    /// The same for a create
    void checkCreate(const BScheduleReview &review);
    /// The review of a text edited in the review; false (and a report) if it cannot be checked
    bool editedReview(const BScheduleReview &shown, const QString &edited, BScheduleReview *next);

    QPointer<BConfigClient> m_client;
    BScheduleSaveUi *m_ui = nullptr;
    bool m_busy = false;
    std::optional<Prepared> m_prepared;
};

Q_DECLARE_METATYPE(BScheduleReport)

#endif // BSCHEDULEPERSISTENCE_H
