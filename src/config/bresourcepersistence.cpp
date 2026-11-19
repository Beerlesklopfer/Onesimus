/**
 * @file bresourcepersistence.cpp
 * @brief Writing one configuration resource to the Director as raw text
 */

#include "config/bresourcepersistence.h"

using Operation = BResourceReview::Operation;

BResourcePersistence::BResourcePersistence(BConfigClient *client, BResourceSaveUi *ui, QObject *parent)
    : QObject(parent)
    , m_client(client)
    , m_ui(ui)
{
    qRegisterMetaType<BResourceReport>();
}

void BResourcePersistence::setBusy(bool busy)
{
    if (m_busy == busy)
        return;
    m_busy = busy;
    emit busyChanged(busy);
}

// ============================================================================
// Words
// ============================================================================

QString BResourcePersistence::noun(BConfig::ResourceType type)
{
    switch (type) {
    case BConfig::ResourceType::Schedule: return tr("schedule");
    case BConfig::ResourceType::Job: return tr("job");
    case BConfig::ResourceType::JobDefs: return tr("JobDefs resource");
    case BConfig::ResourceType::Client: return tr("client");
    case BConfig::ResourceType::FileSet: return tr("FileSet");
    case BConfig::ResourceType::Pool: return tr("pool");
    case BConfig::ResourceType::Storage: return tr("storage");
    default: break;
    }
    return tr("resource");
}

QString BResourcePersistence::notDoneTitle(Operation operation, BConfig::ResourceType type, const QString &name)
{
    const QString kind = BConfig::typeName(type);
    switch (operation) {
    case Operation::Create: return tr("%1 %2 was not created").arg(kind, name);
    case Operation::Delete: return tr("%1 %2 was not deleted").arg(kind, name);
    case Operation::Update: break;
    }
    return tr("%1 %2 was not changed").arg(kind, name);
}

QStringList BResourcePersistence::refTexts(const QList<BConfig::ResourceRef> &refs)
{
    QStringList texts;
    for (const BConfig::ResourceRef &ref : refs)
        texts.append(QStringLiteral("%1 %2").arg(BConfig::typeName(ref.type), ref.name));
    return texts;
}

// ============================================================================
// Read
// ============================================================================

void BResourcePersistence::read(BConfig::ResourceType type, const QString &name, QObject *context,
                                RawCallback callback)
{
    if (!m_client) {
        BConfig::Reply reply;
        reply.status = BConfig::Status::NotAvailable;
        if (callback)
            callback(reply, std::nullopt);
        return;
    }
    m_client->call(BConfig::Request::read(type, name), context, [callback](const BConfig::Reply &reply) {
        if (!callback)
            return;
        // The integrity check: the text is what the SHA-256 says
        if (reply.ok() && BConfig::sha256Hex(reply.text) == reply.sha256)
            callback(reply, Raw{reply.text, reply.sha256, reply.file, reply.sharedFile});
        else
            callback(reply, std::nullopt);
    });
}

// ============================================================================
// Flow
// ============================================================================

bool BResourcePersistence::refuse(Operation operation, BConfig::ResourceType type, const QString &name)
{
    if (m_busy)
        return true;
    if (m_client)
        return false;
    BResourceReport report;
    report.kind = BResourceReport::NotAvailable;
    report.operation = operation;
    report.type = type;
    report.name = name;
    report.title = notDoneTitle(operation, type, name);
    report.text = tr("There is no connection to the configuration methods of the Director. Nothing was sent.");
    finish(report);
    return true;
}

BResourceReport BResourcePersistence::cancelled(const BResourceReview &review) const
{
    BResourceReport report;
    report.kind = BResourceReport::Cancelled;
    report.operation = review.operation;
    report.type = review.type;
    report.name = review.name;
    report.title = notDoneTitle(review.operation, review.type, review.name);
    report.text = tr("Nothing was written.");
    return report;
}

bool BResourcePersistence::update(BConfig::ResourceType type, const QString &name, const Raw &base,
                                  const QString &newText, const BResourceReview &reviewIn)
{
    if (refuse(Operation::Update, type, name))
        return false;

    BResourceReview review = reviewIn;
    review.operation = Operation::Update;
    review.type = type;
    review.name = name;
    review.file = base.file;
    review.sharedFile = base.sharedFile;
    review.oldText = base.text;
    review.newText = newText;

    if (newText == base.text) {
        BResourceReport report = cancelled(review);
        report.title = tr("Nothing to write");
        report.text = tr("The text is the one the Director has.");
        finish(report, false);
        return true;
    }
    if (base.sha256.isEmpty() || BConfig::sha256Hex(base.text) != base.sha256) {
        BResourceReport report;
        report.kind = BResourceReport::Failed;
        report.operation = Operation::Update;
        report.type = type;
        report.name = name;
        report.title = notDoneTitle(Operation::Update, type, name);
        report.text = tr("The text the change was made in does not match its SHA-256. Reload the %1 and "
                         "make the change again.").arg(noun(type));
        finish(report);
        return true;
    }

    setBusy(true);
    const QString sha = base.sha256;
    m_client->call(BConfig::Request::update(type, name, sha, newText, true), this,
                   [this, review, sha](const BConfig::Reply &checked) {
        if (!checked.ok()) {
            BResourceReport report = reportOf(checked, Operation::Update, review.type, review.name);
            report.validatedOnly = true;
            report.ourText = review.newText;
            finish(report);
            return;
        }
        BResourceReview shown = review;
        shown.referencedBy = refTexts(checked.referencedBy);
        if (!m_ui || !m_ui->confirm(shown)) {
            finish(cancelled(review), false);
            return;
        }
        m_client->call(BConfig::Request::update(review.type, review.name, sha, review.newText, false), this,
                       [this, review](const BConfig::Reply &written) {
            BResourceReport report = reportOf(written, Operation::Update, review.type, review.name);
            report.ourText = review.newText;
            finish(report);
        });
    });
    return true;
}

bool BResourcePersistence::create(BConfig::ResourceType type, const QString &name, const QString &text,
                                  const BResourceReview &reviewIn)
{
    if (refuse(Operation::Create, type, name))
        return false;
    setBusy(true);
    BResourceReview review = reviewIn;
    review.operation = Operation::Create;
    review.type = type;
    review.name = name;
    review.oldText.clear();
    review.newText = text;
    m_client->call(BConfig::Request::create(type, text, true), this, [this, review](const BConfig::Reply &checked) {
        if (!checked.ok()) {
            BResourceReport report = reportOf(checked, Operation::Create, review.type, review.name);
            report.validatedOnly = true;
            report.ourText = review.newText;
            finish(report);
            return;
        }
        BResourceReview shown = review;
        shown.file = checked.file;
        if (!m_ui || !m_ui->confirm(shown)) {
            finish(cancelled(review), false);
            return;
        }
        m_client->call(BConfig::Request::create(review.type, review.newText, false), this,
                       [this, review](const BConfig::Reply &created) {
            BResourceReport report = reportOf(created, Operation::Create, review.type, review.name);
            report.ourText = review.newText;
            finish(report);
        });
    });
    return true;
}

bool BResourcePersistence::remove(BConfig::ResourceType type, const QString &name, const BResourceReview &reviewIn)
{
    if (refuse(Operation::Delete, type, name))
        return false;
    setBusy(true);
    // Always the state of now: what is deleted is what the review shows
    m_client->call(BConfig::Request::read(type, name), this, [this, type, name, reviewIn](const BConfig::Reply &read) {
        if (!read.ok()) {
            finish(reportOf(read, Operation::Delete, type, name));
            return;
        }
        const QString sha = read.sha256;
        BResourceReview review = reviewIn;
        review.operation = Operation::Delete;
        review.type = type;
        review.name = name;
        review.file = read.file;
        review.sharedFile = read.sharedFile;
        review.oldText = read.text;
        review.newText.clear();
        m_client->call(BConfig::Request::remove(type, name, sha, true), this,
                       [this, review, sha](const BConfig::Reply &checked) {
            if (!checked.ok()) {
                BResourceReport report = reportOf(checked, Operation::Delete, review.type, review.name);
                report.validatedOnly = true;
                finish(report);
                return;
            }
            BResourceReview shown = review;
            shown.referencedBy = refTexts(checked.referencedBy);
            if (!m_ui || !m_ui->confirm(shown)) {
                finish(cancelled(review), false);
                return;
            }
            m_client->call(BConfig::Request::remove(review.type, review.name, sha, false), this,
                           [this, review](const BConfig::Reply &deleted) {
                finish(reportOf(deleted, Operation::Delete, review.type, review.name));
            });
        });
    });
    return true;
}

// ============================================================================
// Reports
// ============================================================================

BResourceReport BResourcePersistence::reportOf(const BConfig::Reply &reply, Operation operation,
                                               BConfig::ResourceType type, const QString &name)
{
    BResourceReport report;
    report.operation = operation;
    report.type = type;
    report.name = name;
    report.directorMessage = reply.message;
    report.problem = reply.problem;
    report.referencedBy = refTexts(reply.referencedBy);
    report.followups = reply.followups;
    report.reloaded = reply.reloaded;
    report.title = notDoneTitle(operation, type, name);

    const QString kind = BConfig::typeName(type);
    const QString thing = noun(type);
    switch (reply.status) {
    case BConfig::Status::Ok:
        report.kind = BResourceReport::Written;
        report.title = operation == Operation::Create ? tr("%1 %2 created").arg(kind, name)
                     : operation == Operation::Delete ? tr("%1 %2 deleted").arg(kind, name)
                                                      : tr("%1 %2 changed").arg(kind, name);
        report.text = reply.reloaded
            ? tr("The Director wrote the configuration and reloaded it.")
            : tr("The Director wrote the configuration but did not report a reload; the change takes "
                 "effect with its next reload.");
        if (!reply.followups.isEmpty())
            report.text += QLatin1Char(' ') + tr("The reload does not adjust everything; the Director names "
                                                 "these follow-up commands:");
        break;
    case BConfig::Status::Conflict:
        report.kind = BResourceReport::Conflict;
        report.title = tr("%1 %2 was changed on the Director").arg(kind, name);
        report.text = tr("The Director has another state of the %1 than the one this change started "
                         "from. Nothing was written and nothing is overwritten. Compare the two texts, "
                         "reload the %1 and make the change again.").arg(thing);
        report.currentText = reply.text;
        break;
    case BConfig::Status::Invalid:
        report.kind = BResourceReport::Invalid;
        report.text = tr("The Director checked the new configuration and rejected it. Nothing was written.");
        break;
    case BConfig::Status::Referenced:
        report.kind = BResourceReport::Referenced;
        report.text = tr("The %1 is still used. The Director does not delete a %1 that other resources "
                         "name; change them first. Nothing was deleted.").arg(thing);
        break;
    case BConfig::Status::Denied:
        report.kind = BResourceReport::Denied;
        report.text = type == BConfig::ResourceType::JobDefs
            ? tr("The console of this session may not change this %1 (the ACL of its resource type). "
                 "Nothing was written.").arg(thing)
            : tr("The console of this session may not change this %1 (its %2 ACL). Nothing was "
                 "written.").arg(thing, kind);
        break;
    case BConfig::Status::NotFound:
        report.kind = BResourceReport::NotFound;
        report.text = tr("The Director has no %1 of this name (any more). Nothing was written.").arg(thing);
        break;
    case BConfig::Status::Exists:
        report.kind = BResourceReport::Exists;
        report.text = tr("The Director already has a resource of this name. Nothing was written.");
        break;
    case BConfig::Status::ReloadFailed:
        report.kind = BResourceReport::ReloadFailed;
        report.text = tr("The Director wrote the file, but reloading the configuration failed, so it put "
                         "the old file back. The %1 is as it was.").arg(thing);
        break;
    case BConfig::Status::NotAvailable:
        report.kind = BResourceReport::NotAvailable;
        report.text = reply.rpcMessage.isEmpty()
            ? tr("This connection has no configuration methods. Nothing was sent.")
            : reply.rpcMessage;
        break;
    case BConfig::Status::RpcError:
        report.kind = BResourceReport::Failed;
        report.text = reply.rpcCode == 7
            ? tr("The Director refused the call (%1). The console needs the command ACL \"configure\".")
                  .arg(reply.rpcMessage)
            : tr("The call failed (%1). Nothing was written.").arg(reply.rpcMessage);
        break;
    case BConfig::Status::Unspecified:
        report.kind = BResourceReport::Failed;
        report.text = tr("The Director sent no status. Nothing is known to have been written.");
        break;
    }
    return report;
}

void BResourcePersistence::finish(const BResourceReport &report, bool ask)
{
    setBusy(false);
    BResourceSaveUi::Answer answer = BResourceSaveUi::Close;
    if (ask && m_ui)
        answer = m_ui->report(report);
    emit finished(report);
    if (report.kind == BResourceReport::Written
        || ((report.kind == BResourceReport::Conflict || report.kind == BResourceReport::FileDiffers)
            && answer == BResourceSaveUi::Reload)) {
        emit reloadRequested(report.type, report.name);
    }
}
