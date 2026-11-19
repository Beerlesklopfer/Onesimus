#include "tools/b321dialog.h"

#include "bheaderstate.h"
#include "blabelstate.h"
#include "bsorttableitem.h"
#include "bstatuschip.h"
#include "bstatusdelegate.h"
#include "buiscale.h"
#include "config/bsettings.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonValue>
#include <QLabel>
#include <QListWidget>
#include <QLocale>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSplitter>
#include <QTableWidget>
#include <QTextBrowser>
#include <QTimer>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace {

using State = B321Verifier::State;

/// Index of the row's result in m_results (column 0); sorting moves rows
constexpr int ResultIndexRole = Qt::UserRole + 10;

BStatus::Tone toneOf(State state)
{
    switch (state) {
    case State::Met: return BStatus::Tone::Success;
    case State::Failed: return BStatus::Tone::Error;
    case State::Unknown: break;
    }
    return BStatus::Tone::Neutral;
}

QString codeOf(State state)
{
    switch (state) {
    case State::Met: return QStringLiteral("\u2713");
    case State::Failed: return QStringLiteral("\u2717");
    case State::Unknown: break;
    }
    return QStringLiteral("?");
}

State stateOf(B321Verifier::Verdict verdict)
{
    switch (verdict) {
    case B321Verifier::Verdict::Passes: return State::Met;
    case B321Verifier::Verdict::Fails: return State::Failed;
    case B321Verifier::Verdict::Unknown: break;
    }
    return State::Unknown;
}

/// A chip cell: code, short text, tone; the screen reader gets @p spoken
QTableWidgetItem *chipItem(State state, const QString &text, const QString &spoken, qint64 sortKey)
{
    auto *item = new BSortTableItem(text, sortKey);
    item->setData(BStatus::CodeRole, codeOf(state));
    item->setData(BStatus::ToneRole, int(toneOf(state)));
    item->setData(Qt::AccessibleTextRole, spoken);
    item->setToolTip(spoken);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}

/// Sort key: failed first, then unknown, then met; then the count
qint64 sortKeyOf(State state, int count)
{
    const int rank = state == State::Failed ? 0 : (state == State::Unknown ? 1 : 2);
    return qint64(rank) * 1000 + count;
}

QString dateText(const QDateTime &when)
{
    return when.isValid() ? QLocale::c().toString(when, QStringLiteral("yyyy-MM-dd HH:mm"))
                          : B321Dialog::tr("unknown");
}

/// A reference in "show jobs" is a name or {"name": ...}
QString nameOf(const QJsonValue &value)
{
    if (value.isObject()) return value.toObject().value(QLatin1String("name")).toString().trimmed();
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        return array.isEmpty() ? QString() : nameOf(array.first());
    }
    return value.toString().trimmed();
}

QString escaped(const QString &text)
{
    return text.toHtmlEscaped();
}

QString placementText(const B321Verifier::Placement &p)
{
    if (!p.known) return B321Dialog::tr("%1 (not in the volume list)").arg(p.volume);
    QStringList parts;
    parts.append(p.mediaType.isEmpty() ? B321Dialog::tr("media type unknown") : p.mediaType);
    parts.append(p.storage.isEmpty() ? B321Dialog::tr("storage unknown") : p.storage);
    if (!p.status.isEmpty()) parts.append(p.status);
    switch (p.source) {
    case B321Verifier::OffSiteSource::Marking: parts.append(B321Dialog::tr("off site (marked)")); break;
    case B321Verifier::OffSiteSource::Location:
        parts.append(B321Dialog::tr("off site (location %1)").arg(p.locationId.value_or(0)));
        break;
    case B321Verifier::OffSiteSource::UnmarkedOnSite: parts.append(B321Dialog::tr("on site")); break;
    case B321Verifier::OffSiteSource::None: parts.append(B321Dialog::tr("off site unknown")); break;
    }
    return B321Dialog::tr("%1 (%2)").arg(p.volume, parts.join(QStringLiteral(", ")));
}

} // namespace

B321Dialog::B321Dialog(ContextProvider context, QWidget *parent)
    : QDialog(parent)
    , m_contextProvider(std::move(context))
{
    setWindowTitle(tr("3-2-1 Verifier"));
    setAccessibleName(tr("3-2-1 Verifier"));
    setAccessibleDescription(tr("Checks for every backup job whether there are 3 copies of the data, "
                                "on 2 media types, with 1 copy off site. Read only."));
    setSizeGripEnabled(true);
    m_input.countOriginalData = BSettings::instance().verify321CountOriginalData();
    setupUi();
    readContext();
    fillOffSiteList();
    recompute();
}

B321Dialog::~B321Dialog() = default;

void B321Dialog::setupUi()
{
    auto *layout = new QVBoxLayout(this);

    // State of the data, and Refresh
    auto *top = new QHBoxLayout();
    m_stateLabel = new QLabel(this);
    m_stateLabel->setWordWrap(true);
    m_stateLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_stateLabel->setAccessibleName(tr("Data state"));
    top->addWidget(m_stateLabel, 1);
    m_refreshButton = new QPushButton(tr("&Refresh"), this);
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    m_refreshButton->setToolTip(tr("Reload the jobs, the volumes of the jobs and the volume list "
                                   "(read only)"));
    top->addWidget(m_refreshButton);
    layout->addLayout(top);

    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_leftSplitter = new QSplitter(Qt::Vertical, m_mainSplitter);

    // The jobs
    auto *tablePane = new QWidget(m_leftSplitter);
    auto *tableLayout = new QVBoxLayout(tablePane);
    tableLayout->setContentsMargins(0, 0, 0, 0);
    auto *tableLabel = new QLabel(tr("&Backup jobs:"), tablePane);
    tableLayout->addWidget(tableLabel);
    m_table = new QTableWidget(tablePane);
    m_table->setColumnCount(ColumnCount);
    m_table->setHorizontalHeaderLabels({tr("Job"), tr("Client"), tr("Copies"), tr("Media Types"),
                                        tr("Off Site"), tr("Verdict"), tr("Reason")});
    m_table->horizontalHeaderItem(CopiesColumn)->setToolTip(
        tr("Copies of the data a restore would use: the chain, its complete copies and, if set, "
           "the original data. 3 are needed."));
    m_table->horizontalHeaderItem(MediaColumn)->setToolTip(
        tr("Distinct media types of the volumes of the chain and its complete copies. 2 are needed."));
    m_table->horizontalHeaderItem(OffSiteColumn)->setToolTip(
        tr("Copies off site, by your marking of the storages or a location of the volume. "
           "1 is needed."));
    tableLabel->setBuddy(m_table);
    m_table->setAccessibleName(tr("3-2-1 check per backup job"));
    m_table->setAccessibleDescription(tr("One row per backup job: copies, media types, off site, "
                                         "verdict and reason. The details of the selected job follow "
                                         "below the table."));
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->setWordWrap(false);
    m_table->setTextElideMode(Qt::ElideRight);
    m_table->setTabKeyNavigation(false);
    m_table->verticalHeader()->setVisible(false);
    auto *delegate = new BStatusDelegate(m_table);
    for (int column : {int(CopiesColumn), int(MediaColumn), int(OffSiteColumn), int(VerdictColumn)})
        m_table->setItemDelegateForColumn(column, delegate);
    m_table->setColumnWidth(JobColumn, BUiScale::px(170));
    m_table->setColumnWidth(ClientColumn, BUiScale::px(140));
    m_table->setColumnWidth(CopiesColumn, BUiScale::px(80));
    m_table->setColumnWidth(MediaColumn, BUiScale::px(100));
    m_table->setColumnWidth(OffSiteColumn, BUiScale::px(100));
    m_table->setColumnWidth(VerdictColumn, BUiScale::px(100));
    BHeaderState::makeInteractive(m_table->horizontalHeader());
    if (BHeaderState::restore(m_table->horizontalHeader(), BSettings::instance().verify321HeaderState(),
                              BSettings::instance().verify321HeaderScale())) {
        // restore() leaves the sort indicator; the rows follow it once filled
    } else {
        m_table->horizontalHeader()->setSortIndicator(VerdictColumn, Qt::AscendingOrder);
    }
    m_table->setSortingEnabled(true);
    tableLayout->addWidget(m_table);

    // Details of the selected job
    auto *detailPane = new QWidget(m_leftSplitter);
    auto *detailLayout = new QHBoxLayout(detailPane);
    detailLayout->setContentsMargins(0, 0, 0, 0);
    auto *treeBox = new QVBoxLayout();
    auto *treeLabel = new QLabel(tr("C&hain and copies:"), detailPane);
    treeBox->addWidget(treeLabel);
    m_tree = new QTreeWidget(detailPane);
    m_tree->setHeaderHidden(true);
    m_tree->setColumnCount(1);
    m_tree->setAccessibleName(tr("Chain and copies of the selected job"));
    m_tree->setAccessibleDescription(tr("The jobs a restore would use and the copies of them, with the "
                                        "volumes they lie on"));
    // Long lines scroll sideways instead of being cut; the full line is the tool tip too
    m_tree->setTextElideMode(Qt::ElideNone);
    m_tree->header()->setStretchLastSection(false);
    m_tree->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    treeLabel->setBuddy(m_tree);
    treeBox->addWidget(m_tree);
    detailLayout->addLayout(treeBox, 1);
    auto *textBox = new QVBoxLayout();
    auto *textLabel = new QLabel(tr("&Why, and what to do:"), detailPane);
    textBox->addWidget(textLabel);
    m_text = new QTextBrowser(detailPane);
    m_text->setOpenLinks(false);
    m_text->setAccessibleName(tr("Reasons, suggestions and hints for the selected job"));
    textLabel->setBuddy(m_text);
    textBox->addWidget(m_text);
    detailLayout->addLayout(textBox, 1);

    m_leftSplitter->setStretchFactor(0, 1);
    m_leftSplitter->setStretchFactor(1, 1);

    // Settings: off-site marks and the count of the original data
    auto *settingsBox = new QGroupBox(tr("Settings"), m_mainSplitter);
    auto *settingsLayout = new QVBoxLayout(settingsBox);
    m_countOriginal = new QCheckBox(tr("Count the &original data as a copy"), settingsBox);
    m_countOriginal->setToolTip(tr("The 3-2-1 rule is usually stated as the production data plus "
                                   "2 backups. Off: 3 backups are needed."));
    m_countOriginal->setChecked(m_input.countOriginalData);
    settingsLayout->addWidget(m_countOriginal);

    auto *offSiteLabel = new QLabel(tr("O&ff-site storages:"), settingsBox);
    settingsLayout->addWidget(offSiteLabel);
    auto *offSiteHelp = new QLabel(tr("Bareos does not record where a storage stands. Tick the "
                                      "storages that are at another site."), settingsBox);
    offSiteHelp->setWordWrap(true);
    BUiScale::bind(offSiteHelp, BUiScale::Small);
    bSetLabelState(offSiteHelp, "muted");
    settingsLayout->addWidget(offSiteHelp);
    m_offSiteList = new QListWidget(settingsBox);
    m_offSiteList->setAccessibleName(tr("Off-site storages"));
    m_offSiteList->setAccessibleDescription(tr("A checked storage is off site. Space toggles."));
    offSiteLabel->setBuddy(m_offSiteList);
    settingsLayout->addWidget(m_offSiteList, 1);
    m_unmarkedOnSite = new QCheckBox(tr("&Unmarked storages are on site"), settingsBox);
    m_unmarkedOnSite->setToolTip(tr("Without this, a storage that is not ticked is \"unknown\", "
                                    "not \"on site\"."));
    settingsLayout->addWidget(m_unmarkedOnSite);

    m_mainSplitter->addWidget(m_leftSplitter);
    m_mainSplitter->addWidget(settingsBox);
    m_mainSplitter->setStretchFactor(0, 4);
    m_mainSplitter->setStretchFactor(1, 1);
    layout->addWidget(m_mainSplitter, 1);

    auto *closeButton = new QPushButton(tr("&Close"), this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
    auto *bottom = new QHBoxLayout();
    bottom->addStretch();
    bottom->addWidget(closeButton);
    layout->addLayout(bottom);

    BUiScale::track(this, [this]() {
        m_mainSplitter->setSizes({BUiScale::px(860), BUiScale::px(260)});
        m_leftSplitter->setSizes({BUiScale::px(300), BUiScale::px(360)});
    });
    resize(BUiScale::px(1180), BUiScale::px(720));

    setTabOrder(m_refreshButton, m_table);
    setTabOrder(m_table, m_tree);
    setTabOrder(m_tree, m_text);
    setTabOrder(m_text, m_countOriginal);
    setTabOrder(m_countOriginal, m_offSiteList);
    setTabOrder(m_offSiteList, m_unmarkedOnSite);
    setTabOrder(m_unmarkedOnSite, closeButton);

    connect(m_refreshButton, &QPushButton::clicked, this, &B321Dialog::refresh);
    connect(m_table, &QTableWidget::currentCellChanged, this, [this]() { showDetails(); });
    connect(m_countOriginal, &QCheckBox::toggled, this, [this](bool checked) {
        BSettings::instance().setVerify321CountOriginalData(checked);
        m_input.countOriginalData = checked;
        recompute();
    });
    connect(m_unmarkedOnSite, &QCheckBox::toggled, this, [this](bool checked) {
        BSettings::instance().setVerify321UnmarkedOnSite(m_profileId, checked);
        m_input.unmarkedAreOnSite = checked;
        recompute();
    });
    connect(m_offSiteList, &QListWidget::itemChanged, this, [this](QListWidgetItem *item) {
        const QString storage = item->data(Qt::UserRole).toString();
        const bool offSite = item->checkState() == Qt::Checked;
        BSettings::instance().setVerify321OffSite(m_profileId, storage, offSite);
        item->setData(Qt::AccessibleTextRole, offSite ? tr("%1, off site").arg(storage)
                                                      : tr("%1, not marked off site").arg(storage));
        if (offSite)
            m_input.offSiteStorages.insert(storage);
        else
            m_input.offSiteStorages.remove(storage);
        recompute();
    });

    // The header layout is kept like that of the other tables
    auto *saveTimer = new QTimer(this);
    saveTimer->setSingleShot(true);
    saveTimer->setInterval(500);
    connect(saveTimer, &QTimer::timeout, this, &B321Dialog::saveHeader);
    QHeaderView *header = m_table->horizontalHeader();
    connect(header, &QHeaderView::sectionResized, saveTimer, qOverload<>(&QTimer::start));
    connect(header, &QHeaderView::sectionMoved, saveTimer, qOverload<>(&QTimer::start));
    connect(header, &QHeaderView::sortIndicatorChanged, saveTimer, qOverload<>(&QTimer::start));
}

void B321Dialog::saveHeader()
{
    BSettings::instance().setVerify321HeaderState(m_table->horizontalHeader()->saveState());
    BSettings::instance().setVerify321HeaderScale(BHeaderState::currentScale());
}

QString B321Dialog::jobRequestArgs()
{
    return QStringLiteral("%1,%2").arg(HistoryDays).arg(HistoryLimit);
}

void B321Dialog::setReferenceTime(const QDateTime &now)
{
    m_referenceTime = now;
    recompute();
}

void B321Dialog::setConnected(bool connected)
{
    m_connected = connected;
    if (!connected) {
        m_jobsPending = m_mediaPending = m_volumesPending = false;
    }
    updateState();
}

QList<B321Verifier::Definition> B321Dialog::backupDefinitions(const QMap<QString, QJsonObject> &jobs,
                                                              const QMap<QString, QJsonObject> &jobDefs)
{
    QList<B321Verifier::Definition> definitions;
    for (auto it = jobs.constBegin(); it != jobs.constEnd(); ++it) {
        const QJsonObject &job = it.value();
        // A directive of the job, or of the JobDefs it names
        const QJsonObject defs = jobDefs.value(nameOf(job.value(QLatin1String("jobdefs"))));
        auto directive = [&](const char *key) {
            const QJsonValue own = job.value(QLatin1String(key));
            return own.isUndefined() || own.isNull() ? defs.value(QLatin1String(key)) : own;
        };
        if (nameOf(directive("type")).compare(QLatin1String("Backup"), Qt::CaseInsensitive) != 0)
            continue;
        B321Verifier::Definition definition;
        definition.name = it.key();
        definition.client = nameOf(directive("client"));
        definition.fileset = nameOf(directive("fileset"));
        definitions.append(definition);
    }
    std::sort(definitions.begin(), definitions.end(),
              [](const B321Verifier::Definition &a, const B321Verifier::Definition &b) {
                  return a.name.compare(b.name, Qt::CaseInsensitive) < 0;
              });
    return definitions;
}

void B321Dialog::readContext()
{
    if (!m_contextProvider) return;
    const Context context = m_contextProvider();
    m_profileId = context.profileId;
    m_configuredStorages = context.storages;
    m_input.definitions = backupDefinitions(context.jobs, context.jobDefs);
    m_input.definitionsKnown = !context.jobs.isEmpty();

    m_input.unmarkedAreOnSite = BSettings::instance().verify321UnmarkedOnSite(m_profileId);
    const QSignalBlocker blocker(m_unmarkedOnSite);
    m_unmarkedOnSite->setChecked(m_input.unmarkedAreOnSite);
}

QStringList B321Dialog::knownStorages() const
{
    QStringList storages = m_configuredStorages;
    for (const BVolume &volume : m_input.volumes) {
        if (!volume.storage.isEmpty() && !storages.contains(volume.storage))
            storages.append(volume.storage);
    }
    std::sort(storages.begin(), storages.end(), [](const QString &a, const QString &b) {
        return a.compare(b, Qt::CaseInsensitive) < 0;
    });
    storages.removeDuplicates();
    return storages;
}

void B321Dialog::fillOffSiteList()
{
    const QSignalBlocker blocker(m_offSiteList);
    const QString current = m_offSiteList->currentItem()
        ? m_offSiteList->currentItem()->data(Qt::UserRole).toString() : QString();
    m_offSiteList->clear();
    m_input.offSiteStorages.clear();
    const BSettings &settings = BSettings::instance();
    const QStringList storages = knownStorages();
    for (const QString &storage : storages) {
        auto *item = new QListWidgetItem(storage, m_offSiteList);
        item->setData(Qt::UserRole, storage);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        const bool offSite = settings.verify321OffSite(m_profileId, storage);
        item->setCheckState(offSite ? Qt::Checked : Qt::Unchecked);
        item->setData(Qt::AccessibleTextRole, offSite ? tr("%1, off site").arg(storage)
                                                      : tr("%1, not marked off site").arg(storage));
        if (offSite) m_input.offSiteStorages.insert(storage);
        if (storage == current) m_offSiteList->setCurrentItem(item);
    }
    if (storages.isEmpty()) {
        auto *item = new QListWidgetItem(tr("No storage known yet"), m_offSiteList);
        item->setFlags(Qt::NoItemFlags);
    }
}

void B321Dialog::refresh()
{
    readContext();
    fillOffSiteList();
    m_errors.clear();
    if (m_connected) {
        m_jobsPending = m_mediaPending = m_volumesPending = true;
        emit sendCommand(BDirector::Command::LlistJobs, jobRequestArgs());
        emit sendCommand(BDirector::Command::ListJobMedia, QString());
        emit sendCommand(BDirector::Command::LlistVolumes, QString());
    }
    recompute();
}

void B321Dialog::processJobHistory(const QString &args, const QString &reply)
{
    // The schedule view asks with its own window; only this window's request counts
    if (args != jobRequestArgs()) return;
    m_jobsPending = false;
    QString error;
    const QList<B321Verifier::Job> jobs = B321Verifier::parseJobs(reply.toUtf8(), &error);
    if (!error.isEmpty()) {
        m_errors.append(tr("Jobs: %1").arg(error));
        m_input.jobsKnown = false;
        m_input.jobs.clear();
    } else {
        m_input.jobs = jobs;
        m_input.jobsKnown = true;
        m_input.historyDays = HistoryDays;
        m_input.jobsTruncated = jobs.size() >= HistoryLimit;
    }
    recompute();
}

void B321Dialog::processJsonResult(BDirector::Command cmd, const QString &json)
{
    if (cmd == BDirector::Command::ListJobMedia) {
        m_mediaPending = false;
        QString error;
        const QHash<quint64, QStringList> media = B321Verifier::parseJobMedia(json.toUtf8(), &error);
        if (!error.isEmpty()) {
            m_errors.append(tr("Volumes of the jobs: %1").arg(error));
            m_input.jobMediaKnown = false;
            m_input.jobMedia.clear();
        } else {
            m_input.jobMedia = media;
            m_input.jobMediaKnown = true;
        }
        recompute();
        return;
    }
    if (cmd == BDirector::Command::LlistVolumes || cmd == BDirector::Command::ListVolumes) {
        m_volumesPending = false;
        QString error;
        const BVolumeList volumes = BVolumeList::fromJson(json.toUtf8(), QStringList(), &error);
        if (!error.isEmpty() && volumes.isEmpty()) {
            m_errors.append(tr("Volume list: %1").arg(error));
            return;
        }
        m_input.volumes = volumes.volumes();
        m_input.volumesKnown = true;
        fillOffSiteList();
        recompute();
    }
}

void B321Dialog::processCommandFailed(BDirector::Command cmd, const QString &args, const QString &error)
{
    const QString text = error.simplified();
    if (cmd == BDirector::Command::LlistJobs && args == jobRequestArgs()) {
        m_jobsPending = false;
        m_errors.append(tr("Jobs: %1").arg(text));
    } else if (cmd == BDirector::Command::ListJobMedia) {
        m_mediaPending = false;
        m_errors.append(tr("Volumes of the jobs: %1").arg(text));
    } else if (cmd == BDirector::Command::LlistVolumes && m_volumesPending) {
        m_volumesPending = false;
        m_errors.append(tr("Volume list: %1").arg(text));
    } else {
        return;
    }
    recompute();
}

void B321Dialog::recompute()
{
    m_input.now = m_referenceTime.isValid() ? m_referenceTime : B321Verifier::wallClockNow();
    m_results = (m_input.jobsKnown || m_input.definitionsKnown) ? B321Verifier::verify(m_input)
                                                                : QList<B321Verifier::Result>();
    fillTable();
    updateState();
}

void B321Dialog::fillTable()
{
    QString selected;
    if (const QTableWidgetItem *item = m_table->item(m_table->currentRow(), JobColumn))
        selected = item->text();

    m_table->setSortingEnabled(false);
    m_table->setRowCount(0);
    m_table->setRowCount(int(m_results.size()));
    for (int row = 0; row < m_results.size(); ++row) {
        const B321Verifier::Result &r = m_results.at(row);

        auto *jobItem = new QTableWidgetItem(r.job.name);
        jobItem->setData(ResultIndexRole, row);
        jobItem->setData(Qt::AccessibleTextRole,
                         tr("%1: %2. %3").arg(r.job.name, B321Verifier::verdictText(r.verdict), r.summary));
        m_table->setItem(row, JobColumn, jobItem);
        m_table->setItem(row, ClientColumn, new QTableWidgetItem(r.job.client));

        const QString copies = tr("%1/3").arg(r.copies);
        m_table->setItem(row, CopiesColumn,
                         chipItem(r.copiesState, copies,
                                  tr("Copies: %1 of 3, %2").arg(r.copies)
                                      .arg(B321Verifier::stateText(r.copiesState)),
                                  sortKeyOf(r.copiesState, r.copies)));
        const QString media = r.mediaState == State::Unknown && r.mediaTypes == 0
            ? tr("?/2") : tr("%1/2").arg(r.mediaTypes);
        m_table->setItem(row, MediaColumn,
                         chipItem(r.mediaState, media,
                                  tr("Media types: %1 of 2%2, %3")
                                      .arg(r.mediaTypes)
                                      .arg(r.mediaTypeNames.isEmpty()
                                               ? QString()
                                               : QStringLiteral(" (%1)").arg(r.mediaTypeNames.join(QStringLiteral(", "))),
                                           B321Verifier::stateText(r.mediaState)),
                                  sortKeyOf(r.mediaState, r.mediaTypes)));
        const QString offSite = r.offSiteState == State::Unknown ? tr("unknown")
                                                                 : tr("%1/1").arg(r.offSiteCopies);
        m_table->setItem(row, OffSiteColumn,
                         chipItem(r.offSiteState, offSite,
                                  r.offSiteState == State::Unknown
                                      ? tr("Off site: unknown")
                                      : tr("Off site: %1 of 1, %2").arg(r.offSiteCopies)
                                            .arg(B321Verifier::stateText(r.offSiteState)),
                                  sortKeyOf(r.offSiteState, r.offSiteCopies)));
        const State verdictState = stateOf(r.verdict);
        m_table->setItem(row, VerdictColumn,
                         chipItem(verdictState, B321Verifier::verdictText(r.verdict),
                                  tr("Verdict: %1").arg(B321Verifier::verdictText(r.verdict)),
                                  sortKeyOf(verdictState, 0)));
        auto *reasonItem = new QTableWidgetItem(r.summary);
        reasonItem->setToolTip(r.summary);
        m_table->setItem(row, ReasonColumn, reasonItem);
    }
    m_table->setSortingEnabled(true);

    int newRow = m_results.isEmpty() ? -1 : 0;
    for (int row = 0; row < m_table->rowCount(); ++row) {
        if (m_table->item(row, JobColumn)->text() == selected) newRow = row;
    }
    if (newRow >= 0) {
        const QSignalBlocker blocker(m_table);
        m_table->setCurrentCell(newRow, JobColumn);
    }
    showDetails();
}

void B321Dialog::showDetails()
{
    m_tree->clear();
    const QTableWidgetItem *item = m_table->item(m_table->currentRow(), JobColumn);
    if (!item) {
        m_text->setHtml(QStringLiteral("<p>%1</p>").arg(escaped(tr("No job selected."))));
        return;
    }
    const int index = item->data(ResultIndexRole).toInt();
    if (index < 0 || index >= m_results.size()) return;
    const B321Verifier::Result &r = m_results.at(index);

    QHash<quint64, B321Verifier::Job> jobsById;
    for (const B321Verifier::Job &job : m_input.jobs) jobsById.insert(job.jobId, job);

    auto volumesOf = [this](quint64 jobId) {
        QStringList texts;
        if (!m_input.jobMediaKnown) return tr("volumes not loaded");
        const QStringList names = m_input.jobMedia.value(jobId);
        if (names.isEmpty()) return tr("no volume recorded");
        QHash<QString, const BVolume *> byName;
        for (const BVolume &volume : m_input.volumes) byName.insert(volume.name, &volume);
        for (const QString &name : names) {
            const BVolume *volume = byName.value(name, nullptr);
            texts.append(volume ? tr("%1 (%2, %3, %4)").arg(name, volume->mediaType, volume->storage,
                                                           volume->status)
                                : tr("%1 (not in the volume list)").arg(name));
        }
        return texts.join(QStringLiteral("; "));
    };

    for (const B321Verifier::CopySet &set : r.sets) {
        QString title;
        if (set.original) {
            title = tr("Chain (the backup itself): %n job(s)", nullptr, int(set.jobIds.size()));
        } else if (set.complete) {
            title = tr("Copy in pool %1: complete").arg(set.pool.isEmpty() ? tr("(not reported)") : set.pool);
        } else {
            title = tr("Copy in pool %1: partial, %2 of %3 jobs")
                        .arg(set.pool.isEmpty() ? tr("(not reported)") : set.pool)
                        .arg(set.covered.size()).arg(set.covered.size() + set.missing.size());
        }
        QStringList facts;
        facts.append(set.mediaTypes.isEmpty() ? tr("media type unknown") : set.mediaTypes.join(QStringLiteral(", ")));
        if (!set.storages.isEmpty()) facts.append(set.storages.join(QStringLiteral(", ")));
        facts.append(B321Verifier::offSiteText(set.offSite));
        if (set.written.isValid())
            facts.append(set.original ? tr("last job ended %1").arg(dateText(set.written))
                                      : tr("copied %1").arg(dateText(set.written)));
        auto *top = new QTreeWidgetItem(m_tree, {tr("%1 \u2014 %2").arg(title, facts.join(QStringLiteral(", ")))});
        top->setToolTip(0, top->text(0));
        for (qsizetype i = 0; i < set.jobIds.size(); ++i) {
            const quint64 id = set.jobIds.at(i);
            const B321Verifier::Job job = jobsById.value(id);
            QString text;
            if (set.original) {
                text = tr("%1 %2, started %3: %4").arg(B321Verifier::levelName(job.level)).arg(id)
                           .arg(dateText(job.started), volumesOf(id));
                if (r.migratedFrom.contains(id))
                    text += tr(" (migrated from job %1)").arg(r.migratedFrom.value(id).jobId);
            } else {
                const quint64 member = set.covered.value(i);
                const B321Verifier::Job original = jobsById.value(member);
                text = tr("Copy %1 of %2 %3, copied %4: %5").arg(id)
                           .arg(B321Verifier::levelName(original.level)).arg(member)
                           .arg(dateText(job.reallyEnded), volumesOf(id));
            }
            auto *child = new QTreeWidgetItem(top, {text});
            child->setToolTip(0, text);
        }
        for (quint64 missing : set.missing) {
            const B321Verifier::Job original = jobsById.value(missing);
            new QTreeWidgetItem(top, {tr("No copy of %1 %2").arg(B321Verifier::levelName(original.level))
                                          .arg(missing)});
        }
        top->setExpanded(true);
    }
    if (r.sets.isEmpty()) {
        new QTreeWidgetItem(m_tree, {tr("No restorable chain in the job history")});
    }

    QString html;
    html += QStringLiteral("<p><b>%1</b></p>")
                .arg(escaped(tr("%1: %2").arg(r.job.name, B321Verifier::verdictText(r.verdict))));
    auto list = [&](const QString &heading, const QStringList &lines) {
        if (lines.isEmpty()) return;
        html += QStringLiteral("<p><b>%1</b></p><ul>").arg(escaped(heading));
        for (const QString &line : lines) html += QStringLiteral("<li>%1</li>").arg(escaped(line));
        html += QStringLiteral("</ul>");
    };
    list(tr("Why"), r.reasons);
    list(tr("What to do"), r.suggestions);
    list(tr("Hints (not part of the verdict)"), r.hints);
    m_text->setHtml(html);
}

void B321Dialog::updateState()
{
    QString text;
    const char *state = "muted";
    if (!m_errors.isEmpty()) {
        text = tr("Not everything could be loaded: %1").arg(m_errors.join(QStringLiteral(" \u2022 ")));
        state = "warning";
    } else if (m_jobsPending || m_mediaPending || m_volumesPending) {
        QStringList waiting;
        if (m_jobsPending) waiting.append(tr("jobs"));
        if (m_mediaPending) waiting.append(tr("volumes of the jobs"));
        if (m_volumesPending) waiting.append(tr("volume list"));
        text = tr("Loading: %1 \u2026").arg(waiting.join(QStringLiteral(", ")));
        state = "info";
    } else if (!m_input.jobsKnown) {
        text = m_connected ? tr("No job data yet. Refresh loads it (read only).")
                           : tr("Not connected to a Director.");
    } else {
        int passes = 0, fails = 0, unknown = 0;
        for (const B321Verifier::Result &r : std::as_const(m_results)) {
            if (r.verdict == B321Verifier::Verdict::Passes) ++passes;
            else if (r.verdict == B321Verifier::Verdict::Fails) ++fails;
            else ++unknown;
        }
        text = tr("%n job(s) checked: %1 pass, %2 fail, %3 unknown. ", nullptr, int(m_results.size()))
                   .arg(passes).arg(fails).arg(unknown)
             + tr("From %n job record(s) of the last %1 days", nullptr, int(m_input.jobs.size())).arg(HistoryDays)
             + (m_input.volumesKnown ? tr(", %n volume(s)", nullptr, int(m_input.volumes.size())) : QString())
             + (m_input.jobMediaKnown ? tr(", the volumes of %n job(s)", nullptr, int(m_input.jobMedia.size()))
                                      : QString())
             + tr(". Read only.");
        state = fails > 0 ? "warning" : (unknown > 0 ? "info" : "success");
    }
    m_stateLabel->setText(text);
    bSetLabelState(m_stateLabel, state);
    m_refreshButton->setEnabled(m_connected);
}
