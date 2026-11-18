#include "jobs/bjoblogview.h"
#include "jobs/bjoblogsource.h"
#include <QPainter>
#include <QScrollBar>
#include <algorithm>

BJobLogView::BJobLogView(QWidget *parent)
    : QListView(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setWordWrap(false);
    // Per pixel: a row can never be scrolled out of reach. Do not enable
    // uniform item sizes, that clips the row width (no horizontal scrolling).
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setFont(QFont("Courier", 9));

    // Prevent the global QSS hover rule from overriding the model's
    // BackgroundRole colours (error/warning lines)
    setStyleSheet("QListView::item:hover { background: transparent; }");
}

void BJobLogView::attach(BDirector *director, BJobLogModel *logModel)
{
    delete m_source;
    m_source = nullptr;
    m_director = director;
    m_logModel = logModel;
}

void BJobLogView::ensureSource()
{
    // Job messages are pushed over gRPC only; BNET has to poll the catalog
    const bool wantPush = m_director->transport() == BDirector::Transport::Grpc;
    if (m_source && m_sourceIsPush == wantPush) {
        return;
    }
    delete m_source;
    m_sourceIsPush = wantPush;
    if (wantPush) {
        m_source = new BPushJobLogSource(m_logModel, this);
    } else {
        m_source = new BPollingJobLogSource(m_logModel, this);
    }

    BDirector *director = m_director;
    connect(m_source, &BJobLogSource::fetchRequested, director, [director](quint64 jobId) {
        QMetaObject::invokeMethod(director, "doSend", Qt::QueuedConnection,
                                  Q_ARG(BDirector::Command, BDirector::Command::ListJobId),
                                  Q_ARG(QString, QString::number(jobId)));
    });
    // The source is the context object: the connections end with it
    connect(director, &BDirector::jobLogResult, m_source, &BJobLogSource::catalogReply);
    connect(director, &BDirector::jobMessage, m_source,
            [source = m_source](quint64 jobId, const QString &, const QString &text) {
                source->jobMessage(jobId, text);
            });
    connect(director, &BDirector::jobStatusEvent, m_source,
            [source = m_source](quint64 jobId, const QString &, const QString &status) {
                source->jobStatusEvent(jobId, status);
            });

    connect(m_source, &BJobLogSource::updated, this, [this](quint64 jobId) {
        setPlaceholderText(m_logModel && m_logModel->rowCount() == 0
                               ? tr("No log entries for this job.") : QString());
        emit logUpdated(jobId);
    });
    connect(m_source, &BJobLogSource::failed, this, [this](quint64 jobId, const QString &error) {
        setPlaceholderText(tr("Could not load the job log: %1").arg(error));
        emit logFailed(jobId, error);
    });
    connect(m_source, &BJobLogSource::finished, this, [this](quint64 jobId) {
        if (jobId == m_jobId) {
            m_running = false;
            m_complete = true;
        }
        emit logFinished(jobId);
    });
}

void BJobLogView::showJob(quint64 jobId, bool running)
{
    if (jobId == 0) {
        stopJob();
        return;
    }
    if (jobId == m_jobId && m_logModel && m_logModel->jobId() == jobId) {
        return;   // already shown (and fed, if visible)
    }

    if (m_source) {
        m_source->stop();
    }
    m_jobId = jobId;
    m_running = running;
    m_complete = false;

    if (!m_director || !m_logModel) {
        setPlaceholderText(tr("No connection to the Director."));
        return;
    }

    setPlaceholderText(tr("Loading job log..."));
    m_logModel->setJob(jobId);
    if (isVisible()) {
        startSource();
    }
}

void BJobLogView::startSource()
{
    if (m_jobId == 0 || !m_director || !m_logModel) {
        return;
    }
    ensureSource();
    m_source->start(m_jobId, m_running);
}

void BJobLogView::stopJob()
{
    m_jobId = 0;
    m_running = false;
    m_complete = false;
    if (m_source) {
        m_source->stop();
    }
}

void BJobLogView::notifyJobEnded(quint64 jobId)
{
    if (m_source) {
        m_source->jobEnded(jobId);
    }
}

bool BJobLogView::isLive() const
{
    return m_source && m_source->isLive();
}

void BJobLogView::setPlaceholderText(const QString &text)
{
    if (m_placeholder != text) {
        m_placeholder = text;
        viewport()->update();
    }
}

QString BJobLogView::selectedText() const
{
    if (!selectionModel()) {
        return QString();
    }
    QModelIndexList selected = selectionModel()->selectedIndexes();
    std::sort(selected.begin(), selected.end(),
              [](const QModelIndex &a, const QModelIndex &b) { return a.row() < b.row(); });

    QStringList lines;
    lines.reserve(selected.size());
    for (const QModelIndex &index : std::as_const(selected)) {
        lines.append(index.data(Qt::DisplayRole).toString());
    }
    return lines.join(QLatin1Char('\n'));
}

// ----------------------------------------------------------------------------
// Follow the tail
// ----------------------------------------------------------------------------

void BJobLogView::reset()
{
    QListView::reset();
    m_forceBottom = true;
}

void BJobLogView::setModel(QAbstractItemModel *model)
{
    disconnect(m_aboutToInsert);
    QListView::setModel(model);
    if (model) {
        m_aboutToInsert = connect(model, &QAbstractItemModel::rowsAboutToBeInserted,
                                  this, &BJobLogView::sampleFollowTail);
    }
}

void BJobLogView::sampleFollowTail()
{
    // Sample before the insert: a user who scrolled up is not pulled down
    const QScrollBar *bar = verticalScrollBar();
    m_followTail = m_forceBottom || BJobLog::isAtBottom(bar->value(), bar->maximum());
}

void BJobLogView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    QListView::rowsInserted(parent, start, end);
    m_forceBottom = false;
    if (m_followTail) {
        scrollToBottom();
    }
}

// ----------------------------------------------------------------------------
// Events
// ----------------------------------------------------------------------------

void BJobLogView::paintEvent(QPaintEvent *event)
{
    QListView::paintEvent(event);

    if (m_placeholder.isEmpty() || !model() || model()->rowCount() > 0) {
        return;
    }
    QPainter painter(viewport());
    painter.setFont(font());
    painter.setPen(palette().color(QPalette::PlaceholderText));
    painter.drawText(viewport()->rect().adjusted(6, 4, -6, -4),
                     Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap, m_placeholder);
}

void BJobLogView::showEvent(QShowEvent *event)
{
    QListView::showEvent(event);
    // (Re)start: first show of a job requested while hidden, or resume of a
    // log that was not complete when the view was hidden
    if (m_jobId != 0 && !m_complete && !(m_source && m_source->isActive())) {
        startSource();
    }
}

void BJobLogView::hideEvent(QHideEvent *event)
{
    if (m_source) {
        m_source->stop();
    }
    QListView::hideEvent(event);
}
