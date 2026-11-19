#ifndef BRESOURCEPERSISTENCE_H
#define BRESOURCEPERSISTENCE_H

#include <QList>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>

#include <functional>
#include <optional>

#include "director/bconfigclient.h"

/**
 * @file bresourcepersistence.h
 * @brief Writing one configuration resource to the Director as raw text:
 *        change, create, delete, each checked, shown and confirmed first
 *
 * The write path of schedule plan step 7, for any resource type the
 * configuration methods of the Onesimus Director offer (BConfigClient:
 * ReadResource, CreateResource, UpdateResource, DeleteResource). Every write
 * goes the same way:
 *
 *   1. the raw text of the resource and its SHA-256, as read when editing
 *      began (ReadResource); the caller has written its change into that
 *      text (BResourceText::edit());
 *   2. the same request with validate_only: the Director checks the whole
 *      new configuration and writes nothing;
 *   3. the review (BResourceSaveUi::confirm()): the text before and after as
 *      a line diff, the resources the change applies to, what the Director
 *      names as referencing the resource;
 *   4. only after "Write": the request itself, with the SHA-256 of step 1.
 *
 * Every status of the Director is reported (BResourceSaveUi::report()).
 * CONFLICT never leads to a second attempt with the new SHA-256: the user
 * sees both texts and can reload. A delete that is REFERENCED shows the
 * referring resources and changes nothing. reportOf() is also the status
 * text of BSchedulePersistence.
 */

/// What the review shows before anything is written
struct BResourceReview
{
    enum Operation { Update, Create, Delete };

    Operation operation = Update;
    BConfig::ResourceType type = BConfig::ResourceType::Unspecified;
    QString name;
    QString file;               ///< Path on the Director, if known
    bool sharedFile = false;    ///< The file holds other resources as well
    QString oldText;            ///< The resource text now (empty for Create)
    QString newText;            ///< The resource text after the change (empty for Delete)
    QStringList changes;        ///< "Client: a-fd -> b-fd", one per edited directive (may be empty)
    QStringList affected;       ///< What the change applies to ("Job nightly"), from what the GUI loaded
    QString affectedLabel;      ///< Heading of that list ("Jobs that use this JobDefs")
    QStringList referencedBy;   ///< "Job forgejo", as the Director names them
    QString note;               ///< What the change is, in a sentence
};

/// The outcome of a write, or why there was none
struct BResourceReport
{
    /// The same kinds, in the same order, as BScheduleReport::Kind
    enum Kind {
        Written,        ///< Done; reloaded says whether the Director reloaded
        Cancelled,      ///< The user did not confirm, or there was nothing to write
        Conflict,       ///< The Director has another state than the one the change started from
        FileDiffers,    ///< The file does not say what the GUI loaded (found before sending anything)
        Invalid,        ///< The Director rejected the new configuration; problem says where
        Referenced,     ///< Delete refused: still in use
        Denied,         ///< Resource ACL
        NotFound,
        Exists,         ///< Create: the name is taken
        ReloadFailed,   ///< Written, the reload failed, the Director put the old file back
        Locked,         ///< The resource cannot be edited safely here
        NotAvailable,   ///< No configuration methods on this connection
        Failed,         ///< Anything else (gRPC error, unreadable text, ...)
    };

    Kind kind = Failed;
    BResourceReview::Operation operation = BResourceReview::Update;
    BConfig::ResourceType type = BConfig::ResourceType::Unspecified;
    QString name;
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
class BResourceSaveUi
{
public:
    virtual ~BResourceSaveUi() = default;

    /// The review; true only for "Write"
    virtual bool confirm(const BResourceReview &review) = 0;

    enum Answer { Close, Reload };
    /// A report; Reload is offered for Conflict and FileDiffers
    virtual Answer report(const BResourceReport &report) = 0;
};

class BResourcePersistence : public QObject
{
    Q_OBJECT

public:
    BResourcePersistence(BConfigClient *client, BResourceSaveUi *ui, QObject *parent = nullptr);

    void setClient(BConfigClient *client) { m_client = client; }
    BConfigClient *client() const { return m_client; }
    void setUi(BResourceSaveUi *ui) { m_ui = ui; }

    /// A write is under way; another one is refused until it ends
    bool isBusy() const { return m_busy; }

    /// The raw text of a resource, its SHA-256 checked
    struct Raw
    {
        QString text;
        QString sha256;
        QString file;
        bool sharedFile = false;
    };
    using RawCallback = std::function<void(const BConfig::Reply &reply, const std::optional<Raw> &raw)>;

    /**
     * @brief Reads a resource (ReadResource); nothing is reported
     *
     * @p callback gets the reply and, if it is OK and its text matches its
     * SHA-256, the raw text. Runs while @p context lives.
     */
    void read(BConfig::ResourceType type, const QString &name, QObject *context, RawCallback callback);

    /**
     * @brief Writes a changed resource (UpdateResource)
     * @param base     The raw text the change was written into, with its SHA-256
     * @param newText  The text after the change
     * @param review   What the review shows besides the texts (affected,
     *                 changes, note); type, name and texts are filled in here
     * @return false if refused at once (busy, no client); a report says why
     */
    bool update(BConfig::ResourceType type, const QString &name, const Raw &base, const QString &newText,
                const BResourceReview &review = BResourceReview());

    /// Creates a resource from its text (CreateResource), after a check and the review
    bool create(BConfig::ResourceType type, const QString &name, const QString &text,
                const BResourceReview &review = BResourceReview());

    /// Deletes a resource (DeleteResource) as it is now, after a check and the review
    bool remove(BConfig::ResourceType type, const QString &name,
                const BResourceReview &review = BResourceReview());

    // --- Words --------------------------------------------------------------

    /// "schedule", "job", "JobDefs resource", ... for running text
    static QString noun(BConfig::ResourceType type);
    /// "Schedule %1 was not changed" and the like
    static QString notDoneTitle(BResourceReview::Operation operation, BConfig::ResourceType type,
                                const QString &name);
    /// The report of a reply of the Director (also used by BSchedulePersistence)
    static BResourceReport reportOf(const BConfig::Reply &reply, BResourceReview::Operation operation,
                                    BConfig::ResourceType type, const QString &name);
    /// "Job forgejo"
    static QStringList refTexts(const QList<BConfig::ResourceRef> &refs);

signals:
    /// The flow is over (written or not)
    void finished(const BResourceReport &report);
    /// The resource is to be read again: it was written, or the user asked
    /// for it after a conflict
    void reloadRequested(BConfig::ResourceType type, const QString &name);
    void busyChanged(bool busy);

private:
    bool refuse(BResourceReview::Operation operation, BConfig::ResourceType type, const QString &name);
    void setBusy(bool busy);
    void finish(const BResourceReport &report, bool ask = true);
    BResourceReport cancelled(const BResourceReview &review) const;

    QPointer<BConfigClient> m_client;
    BResourceSaveUi *m_ui = nullptr;
    bool m_busy = false;
};

Q_DECLARE_METATYPE(BResourceReport)

#endif // BRESOURCEPERSISTENCE_H
