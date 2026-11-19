/**
 * @file bschedulepersistence.cpp
 * @brief Writing schedules to the Director: change, create, delete
 */

#include "schedules/bschedulepersistence.h"

#include "schedules/model/bschedulegridmodel.h"
#include "config/bresourcepersistence.h"

// A schedule report is a resource report with the schedule's own fields:
// the kinds and operations are the same, in the same order
static_assert(int(BScheduleReport::Written) == int(BResourceReport::Written)
              && int(BScheduleReport::Locked) == int(BResourceReport::Locked)
              && int(BScheduleReport::NotAvailable) == int(BResourceReport::NotAvailable)
              && int(BScheduleReport::Failed) == int(BResourceReport::Failed),
              "BScheduleReport::Kind follows BResourceReport::Kind");
static_assert(int(BScheduleReview::Update) == int(BResourceReview::Update)
              && int(BScheduleReview::Create) == int(BResourceReview::Create)
              && int(BScheduleReview::Delete) == int(BResourceReview::Delete),
              "BScheduleReview::Operation follows BResourceReview::Operation");

namespace {

using Operation = BScheduleReview::Operation;

QStringList refTexts(const QList<BConfig::ResourceRef> &refs)
{
    return BResourcePersistence::refTexts(refs);
}

} // namespace

BSchedulePersistence::BSchedulePersistence(BConfigClient *client, BScheduleSaveUi *ui, QObject *parent)
    : QObject(parent)
    , m_client(client)
    , m_ui(ui)
{
    qRegisterMetaType<BScheduleReport>();
}

void BSchedulePersistence::setBusy(bool busy)
{
    if (m_busy == busy)
        return;
    m_busy = busy;
    emit busyChanged(busy);
}

// ============================================================================
// Raw text
// ============================================================================

void BSchedulePersistence::prepare(const QString &scheduleName)
{
    if (!m_client || scheduleName.isEmpty())
        return;
    if (m_prepared && m_prepared->name == scheduleName)
        return;
    m_client->call(BConfig::Request::read(BConfig::ResourceType::Schedule, scheduleName), this,
                   [this, scheduleName](const BConfig::Reply &reply) {
        // The integrity check: the text is what the SHA-256 says
        if (!reply.ok() || BConfig::sha256Hex(reply.text) != reply.sha256)
            return;
        m_prepared = Prepared{scheduleName, reply.text, reply.sha256, reply.file, reply.sharedFile};
    });
}

QString BSchedulePersistence::preparedText(const QString &scheduleName) const
{
    return (m_prepared && m_prepared->name == scheduleName) ? m_prepared->text : QString();
}

void BSchedulePersistence::forget()
{
    m_prepared.reset();
}

void BSchedulePersistence::withRaw(const QString &name, const QList<BRunLine> &loaded,
                                   std::function<void(const Prepared &raw)> next, Operation operation)
{
    // The text read when editing began, if it still says what the GUI
    // loaded: its SHA-256 makes a change made on the Director since then a
    // CONFLICT. Otherwise the GUI has loaded a newer state: read again.
    if (m_prepared && m_prepared->name == name
        && BScheduleResource::matchesLoaded(BScheduleResource::parse(m_prepared->text), loaded)) {
        const Prepared raw = *m_prepared;
        next(raw);
        return;
    }
    m_client->call(BConfig::Request::read(BConfig::ResourceType::Schedule, name), this,
                   [this, name, loaded, next, operation](const BConfig::Reply &reply) {
        if (!reply.ok()) {
            finish(reportOf(reply, operation, name));
            return;
        }
        if (BConfig::sha256Hex(reply.text) != reply.sha256) {
            BScheduleReport report;
            report.kind = BScheduleReport::Failed;
            report.operation = operation;
            report.scheduleName = name;
            report.title = tr("The text of schedule %1 arrived changed").arg(name);
            report.text = tr("The SHA-256 of the text the Director sent does not match the text. Nothing "
                             "was written.");
            finish(report);
            return;
        }
        const Prepared raw{name, reply.text, reply.sha256, reply.file, reply.sharedFile};
        QString error;
        if (!BScheduleResource::matchesLoaded(BScheduleResource::parse(raw.text), loaded, &error)) {
            m_prepared.reset();
            BScheduleReport report;
            report.kind = BScheduleReport::FileDiffers;
            report.operation = operation;
            report.scheduleName = name;
            report.title = tr("Schedule %1 has changed on the Director").arg(name);
            report.text = tr("The configuration file of the schedule does not say what was loaded "
                             "here, so the change was not sent. Reload the schedule and make the "
                             "change again. %1").arg(error);
            report.currentText = raw.text;
            finish(report);
            return;
        }
        m_prepared = raw;
        next(raw);
    });
}

// ============================================================================
// Update
// ============================================================================

bool BSchedulePersistence::saveRuns(const BSchedule &loaded, const QList<BScheduleResource::Line> &lines,
                                    const QStringList &jobs, const QString &note)
{
    if (m_busy)
        return false;
    BScheduleReport refused;
    refused.operation = Operation::Update;
    refused.scheduleName = loaded.name;
    for (int i = 0; i < loaded.runs.size(); ++i) {
        if (!loaded.runs[i].understood()) {
            refused.kind = BScheduleReport::Locked;
            refused.title = tr("Schedule %1 cannot be written").arg(loaded.name);
            refused.text = tr("Run line %1 is not understood by this application (%2). A schedule with "
                              "such a line is not changed here; change it in its configuration file.")
                               .arg(i + 1).arg(loaded.runs[i].parseError);
            finish(refused);
            return false;
        }
    }
    if (!m_client) {
        refused.kind = BScheduleReport::NotAvailable;
        refused.title = tr("Schedule %1 was not changed").arg(loaded.name);
        refused.text = tr("There is no connection to the configuration methods of the Director.");
        finish(refused);
        return false;
    }
    setBusy(true);
    withRaw(loaded.name, loaded.runs, [this, loaded, lines, jobs, note](const Prepared &raw) {
        updateStep(loaded, lines, jobs, note, raw);
    }, Operation::Update);
    return true;
}

void BSchedulePersistence::updateStep(const BSchedule &loaded, const QList<BScheduleResource::Line> &lines,
                                      const QStringList &jobs, const QString &note, const Prepared &raw)
{
    const QString name = loaded.name;
    QString newText;
    QString error;
    if (!BScheduleResource::rewrite(raw.text, lines, &newText, &error)) {
        BScheduleReport report;
        report.kind = BScheduleReport::Failed;
        report.scheduleName = name;
        report.title = tr("Schedule %1 was not changed").arg(name);
        report.text = tr("The change could not be written into the configuration text safely: %1").arg(error);
        finish(report);
        return;
    }
    if (newText == raw.text) {
        BScheduleReport report;
        report.kind = BScheduleReport::Cancelled;
        report.scheduleName = name;
        report.title = tr("Nothing to write");
        report.text = tr("The Run lines are the ones the Director has.");
        finish(report, false);
        return;
    }

    // What the review shows
    BScheduleReview review;
    review.operation = Operation::Update;
    review.scheduleName = name;
    review.file = raw.file;
    review.sharedFile = raw.sharedFile;
    review.jobs = jobs;
    review.oldText = raw.text;
    review.newText = newText;
    review.note = note;
    const BScheduleResource::Parsed parsed = BScheduleResource::parse(raw.text);
    QList<BScheduleReview::Row::State> oldStates(parsed.runs.size(), BScheduleReview::Row::Removed);
    for (const BScheduleResource::Line &line : lines) {
        BScheduleReview::Row row;
        if (line.origin >= 0 && line.keep) {
            row.text = parsed.runs[line.origin].value;
            row.state = BScheduleReview::Row::Untouched;
            oldStates[line.origin] = BScheduleReview::Row::Untouched;
        } else if (line.origin >= 0) {
            row.text = line.text;
            row.state = BScheduleReview::Row::Changed;
            oldStates[line.origin] = BScheduleReview::Row::Changed;
        } else {
            row.text = line.text;
            row.state = BScheduleReview::Row::New;
        }
        review.newRows.append(row);
    }
    for (int i = 0; i < parsed.runs.size(); ++i)
        review.oldRows.append(BScheduleReview::Row{parsed.runs[i].value, oldStates[i]});

    checkUpdate(review, raw);
}

void BSchedulePersistence::checkUpdate(const BScheduleReview &review, const Prepared &raw)
{
    const BConfig::Request check = BConfig::Request::update(BConfig::ResourceType::Schedule,
                                                            review.scheduleName, raw.sha256,
                                                            review.newText, true);
    m_client->call(check, this, [this, review, raw](const BConfig::Reply &checked) {
        if (!checked.ok()) {
            BScheduleReport report = reportOf(checked, Operation::Update, review.scheduleName);
            report.validatedOnly = true;
            report.ourText = review.newText;
            finish(report);
            return;
        }
        BScheduleReview shown = review;
        shown.referencedBy = refTexts(checked.referencedBy);
        QString edited;
        const BScheduleSaveUi::Decision decision = m_ui ? m_ui->decide(shown, &edited)
                                                        : BScheduleSaveUi::Cancel;
        if (decision == BScheduleSaveUi::Recheck) {
            BScheduleReview next;
            if (!editedReview(shown, edited, &next))
                return;
            if (next.newText == raw.text) {
                BScheduleReport report;
                report.kind = BScheduleReport::Cancelled;
                report.scheduleName = review.scheduleName;
                report.title = tr("Nothing to write");
                report.text = tr("The edited text is the one the Director has.");
                finish(report, false);
                return;
            }
            checkUpdate(next, raw);   // the edited text is checked and shown again
            return;
        }
        if (decision != BScheduleSaveUi::Write) {
            BScheduleReport report;
            report.kind = BScheduleReport::Cancelled;
            report.scheduleName = review.scheduleName;
            report.title = tr("Schedule %1 was not changed").arg(review.scheduleName);
            report.text = tr("Nothing was written.");
            finish(report, false);
            return;
        }
        const BConfig::Request write = BConfig::Request::update(BConfig::ResourceType::Schedule,
                                                                review.scheduleName, raw.sha256,
                                                                review.newText, false);
        m_client->call(write, this, [this, review](const BConfig::Reply &written) {
            BScheduleReport report = reportOf(written, Operation::Update, review.scheduleName);
            report.ourText = review.newText;
            if (written.ok()) {
                if (BConfig::sha256Hex(written.text) == written.sha256 && !written.text.isEmpty()) {
                    m_prepared = Prepared{review.scheduleName, written.text, written.sha256,
                                          written.file, written.sharedFile};
                } else {
                    m_prepared.reset();
                }
            }
            finish(report);
        });
    });
}

bool BSchedulePersistence::editedReview(const BScheduleReview &shown, const QString &edited,
                                        BScheduleReview *next)
{
    // The editor shows line ends as lines; a CRLF text stays one
    QString text = edited;
    if (shown.newText.contains(QLatin1String("\r\n"))) {
        text.replace(QLatin1String("\r\n"), QLatin1String("\n"));
        text.replace(QLatin1String("\n"), QLatin1String("\r\n"));
    }

    auto refuse = [&](const QString &why) {
        BScheduleReport report;
        report.kind = BScheduleReport::Failed;
        report.operation = shown.operation;
        report.scheduleName = shown.scheduleName;
        report.title = shown.operation == Operation::Create
                           ? tr("Schedule %1 was not created").arg(shown.scheduleName)
                           : tr("Schedule %1 was not changed").arg(shown.scheduleName);
        report.text = tr("The edited text was not sent to the Director: %1").arg(why);
        report.ourText = text;
        finish(report);
        return false;
    };

    const BScheduleResource::Parsed parsed = BScheduleResource::parse(text);
    if (parsed.name != shown.scheduleName)
        return refuse(tr("the name of the schedule has to stay %1.").arg(shown.scheduleName));
    QStringList newValues;
    QString error;
    if (!BScheduleResource::runValues(text, &newValues, &error))
        return refuse(error);
    QStringList oldValues;
    if (!shown.oldText.isEmpty())
        BScheduleResource::runValues(shown.oldText, &oldValues);

    *next = shown;
    next->newText = text;
    next->referencedBy.clear();
    const QString editedNote = tr("The text was edited in the review; the Director checked the edited text.");
    next->note = shown.note.contains(editedNote) ? shown.note
                 : shown.note.isEmpty()          ? editedNote
                                                 : shown.note + QLatin1Char(' ') + editedNote;
    // Line by line, in order: what the edit changed, added, removed
    next->newRows.clear();
    for (int i = 0; i < newValues.size(); ++i) {
        const auto state = i >= oldValues.size()          ? BScheduleReview::Row::New
                           : oldValues[i] == newValues[i] ? BScheduleReview::Row::Untouched
                                                          : BScheduleReview::Row::Changed;
        next->newRows.append(BScheduleReview::Row{newValues[i], state});
    }
    if (shown.operation == Operation::Update) {
        next->oldRows.clear();
        for (int i = 0; i < oldValues.size(); ++i) {
            const auto state = i >= newValues.size()          ? BScheduleReview::Row::Removed
                               : oldValues[i] == newValues[i] ? BScheduleReview::Row::Untouched
                                                              : BScheduleReview::Row::Changed;
            next->oldRows.append(BScheduleReview::Row{oldValues[i], state});
        }
    }
    return true;
}

// ============================================================================
// Delete
// ============================================================================

bool BSchedulePersistence::deleteSchedule(const QString &scheduleName, const QStringList &jobs)
{
    if (m_busy)
        return false;
    if (!m_client) {
        BScheduleReport report;
        report.kind = BScheduleReport::NotAvailable;
        report.operation = Operation::Delete;
        report.scheduleName = scheduleName;
        report.title = tr("Schedule %1 was not deleted").arg(scheduleName);
        report.text = tr("There is no connection to the configuration methods of the Director.");
        finish(report);
        return false;
    }
    setBusy(true);
    // Always the state of now: deleting needs no loaded Run lines
    m_client->call(BConfig::Request::read(BConfig::ResourceType::Schedule, scheduleName), this,
                   [this, scheduleName, jobs](const BConfig::Reply &read) {
        if (!read.ok()) {
            finish(reportOf(read, Operation::Delete, scheduleName));
            return;
        }
        const QString sha = read.sha256;
        BScheduleReview review;
        review.operation = Operation::Delete;
        review.scheduleName = scheduleName;
        review.file = read.file;
        review.sharedFile = read.sharedFile;
        review.jobs = jobs;
        review.oldText = read.text;
        QStringList values;
        if (BScheduleResource::runValues(read.text, &values)) {
            for (const QString &value : std::as_const(values))
                review.oldRows.append(BScheduleReview::Row{value, BScheduleReview::Row::Removed});
        }
        m_client->call(BConfig::Request::remove(BConfig::ResourceType::Schedule, scheduleName, sha, true), this,
                       [this, review, sha](const BConfig::Reply &checked) {
            if (!checked.ok()) {
                BScheduleReport report = reportOf(checked, Operation::Delete, review.scheduleName);
                report.validatedOnly = true;
                finish(report);
                return;
            }
            BScheduleReview shown = review;
            shown.referencedBy = refTexts(checked.referencedBy);
            if (!m_ui || !m_ui->confirm(shown)) {
                BScheduleReport report;
                report.kind = BScheduleReport::Cancelled;
                report.operation = Operation::Delete;
                report.scheduleName = review.scheduleName;
                report.title = tr("Schedule %1 was not deleted").arg(review.scheduleName);
                report.text = tr("Nothing was written.");
                finish(report, false);
                return;
            }
            m_client->call(BConfig::Request::remove(BConfig::ResourceType::Schedule, review.scheduleName,
                                                    sha, false), this,
                           [this, review](const BConfig::Reply &deleted) {
                if (deleted.ok())
                    m_prepared.reset();
                finish(reportOf(deleted, Operation::Delete, review.scheduleName));
            });
        });
    });
    return true;
}

// ============================================================================
// Create
// ============================================================================

bool BSchedulePersistence::createSchedule(const QString &scheduleName, const QString &text)
{
    if (m_busy)
        return false;
    if (!m_client) {
        BScheduleReport report;
        report.kind = BScheduleReport::NotAvailable;
        report.operation = Operation::Create;
        report.scheduleName = scheduleName;
        report.title = tr("Schedule %1 was not created").arg(scheduleName);
        report.text = tr("There is no connection to the configuration methods of the Director.");
        finish(report);
        return false;
    }
    setBusy(true);
    BScheduleReview review;
    review.operation = Operation::Create;
    review.scheduleName = scheduleName;
    review.newText = text;
    QStringList values;
    if (BScheduleResource::runValues(text, &values)) {
        for (const QString &value : std::as_const(values))
            review.newRows.append(BScheduleReview::Row{value, BScheduleReview::Row::New});
    }
    checkCreate(review);
    return true;
}

void BSchedulePersistence::checkCreate(const BScheduleReview &review)
{
    m_client->call(BConfig::Request::create(BConfig::ResourceType::Schedule, review.newText, true), this,
                   [this, review](const BConfig::Reply &checked) {
        if (!checked.ok()) {
            BScheduleReport report = reportOf(checked, Operation::Create, review.scheduleName);
            report.validatedOnly = true;
            report.ourText = review.newText;
            finish(report);
            return;
        }
        BScheduleReview shown = review;
        shown.file = checked.file;
        QString edited;
        const BScheduleSaveUi::Decision decision = m_ui ? m_ui->decide(shown, &edited)
                                                        : BScheduleSaveUi::Cancel;
        if (decision == BScheduleSaveUi::Recheck) {
            BScheduleReview next;
            if (editedReview(shown, edited, &next))
                checkCreate(next);
            return;
        }
        if (decision != BScheduleSaveUi::Write) {
            BScheduleReport report;
            report.kind = BScheduleReport::Cancelled;
            report.operation = Operation::Create;
            report.scheduleName = review.scheduleName;
            report.title = tr("Schedule %1 was not created").arg(review.scheduleName);
            report.text = tr("Nothing was written.");
            finish(report, false);
            return;
        }
        m_client->call(BConfig::Request::create(BConfig::ResourceType::Schedule, review.newText, false), this,
                       [this, review](const BConfig::Reply &created) {
            BScheduleReport report = reportOf(created, Operation::Create, review.scheduleName);
            report.ourText = review.newText;
            finish(report);
        });
    });
}

// ============================================================================
// Reports
// ============================================================================

BScheduleReport BSchedulePersistence::reportOf(const BConfig::Reply &reply, Operation operation,
                                               const QString &name) const
{
    // The status texts are those of every resource (BResourcePersistence)
    const BResourceReport generic = BResourcePersistence::reportOf(
        reply, BResourceReview::Operation(int(operation)), BConfig::ResourceType::Schedule, name);
    BScheduleReport report;
    report.kind = BScheduleReport::Kind(int(generic.kind));
    report.operation = operation;
    report.scheduleName = name;
    report.title = generic.title;
    report.text = generic.text;
    report.directorMessage = generic.directorMessage;
    report.problem = generic.problem;
    report.currentText = generic.currentText;
    report.referencedBy = generic.referencedBy;
    report.followups = generic.followups;
    report.reloaded = generic.reloaded;
    return report;
}

void BSchedulePersistence::finish(const BScheduleReport &report, bool ask)
{
    setBusy(false);
    BScheduleSaveUi::Answer answer = BScheduleSaveUi::Close;
    if (ask && m_ui)
        answer = m_ui->report(report);
    emit finished(report);
    if (report.kind == BScheduleReport::Written
        || ((report.kind == BScheduleReport::Conflict || report.kind == BScheduleReport::FileDiffers)
            && answer == BScheduleSaveUi::Reload)) {
        if (report.kind != BScheduleReport::Written)
            m_prepared.reset();
        emit reloadRequested(report.scheduleName);
    }
}

// ============================================================================
// Lines of a change
// ============================================================================

bool BSchedulePersistence::linesFromModel(const BScheduleGridModel &model, QList<BScheduleResource::Line> *lines,
                                          QString *error)
{
    QString reason;
    if (!model.canBeWritten(&reason)) {
        if (error)
            *error = reason;
        return false;
    }
    const BGridSnapshot snapshot = model.snapshot();
    QList<BScheduleResource::Line> result;
    for (int i = 0; i < snapshot.runs.size(); ++i) {
        const BRunLine &run = snapshot.runs[i];
        const int origin = snapshot.origins.value(i, -1);
        BScheduleResource::Line line;
        line.origin = origin;
        line.keep = origin >= 0 && !run.dirty;
        if (!line.keep) {
            QString lineError;
            line.text = BRunWriter::lineText(run, &lineError);
            if (line.text.isEmpty()) {
                if (error)
                    *error = tr("Run line %1 cannot be written: %2").arg(i + 1).arg(lineError);
                return false;
            }
        }
        result.append(line);
    }
    if (lines)
        *lines = result;
    return true;
}

QList<BScheduleResource::Line> BSchedulePersistence::linesFromDrag(const BSchedule &loaded, int draggedRun,
                                                                   const QString &newText)
{
    QList<BScheduleResource::Line> lines;
    for (int i = 0; i < loaded.runs.size(); ++i) {
        BScheduleResource::Line line;
        line.origin = i;
        line.keep = i != draggedRun;
        if (!line.keep)
            line.text = newText;
        lines.append(line);
    }
    return lines;
}
