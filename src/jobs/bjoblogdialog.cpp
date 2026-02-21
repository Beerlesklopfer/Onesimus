#include "jobs/bjoblogdialog.h"
#include "blogging.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QClipboard>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QTimer>

BJobLogDialog::BJobLogDialog(const QJsonObject &job, BDirector *director, QWidget *parent)
    : QDialog(parent)
    , m_job(job)
    , m_director(director)
    , m_jobId(job["jobid"].toString().toULongLong())
{
    setupUI();
    loadJobLog();
}

void BJobLogDialog::setupUI()
{
    // Erstelle aussagekräftigen Titel
    QString jobName = m_job["name"].toString();
    QString jobId = m_job["jobid"].toString();
    QString client = m_job["client"].toString();

    setWindowTitle(QString("Job-Log: %1 (ID: %2) - %3")
                       .arg(jobName)
                       .arg(jobId)
                       .arg(client));

    // Setze minimale Fenstergröße
    resize(800, 600);
    setMinimumSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Info-Label
    QLabel *infoLabel = new QLabel(this);
    QString status = m_job["jobstatus"].toString();
    QString statusText;
    if (status == "T") statusText = tr("OK") + " ✓";
    else if (status == "W") statusText = tr("Warning") + " ⚠";
    else if (status == "F") statusText = tr("Failed") + " ✗";
    else if (status == "E") statusText = tr("Error") + " ✗";
    else statusText = status;

    infoLabel->setText(QString("<b>Status:</b> %1 | <b>Start:</b> %2")
                           .arg(statusText)
                           .arg(m_job["starttime"].toString()));
    mainLayout->addWidget(infoLabel);

    // Log-Ansicht
    m_logListView = new QListView(this);
    m_logModel = new BJobLogModel(this);
    m_logListView->setModel(m_logModel);
    m_logListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_logListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_logListView->setAlternatingRowColors(true);
    m_logListView->setWordWrap(false);

    // Setze Monospace-Font für bessere Lesbarkeit
    QFont logFont("Courier", 9);
    m_logListView->setFont(logFont);

    // Prevent global QSS hover rule from overriding model BackgroundRole colors
    // (status lines use colored backgrounds for Error/Warning/Fatal)
    m_logListView->setStyleSheet(
        "QListView::item:hover { background: transparent; }");

    // Zeige Loading-Nachricht
    m_logModel->setLogLines({tr("Lade Job-Log...")});

    mainLayout->addWidget(m_logListView);

    // Button-Leiste
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_copyButton = new QPushButton(tr("Auswahl kopieren"), this);
    m_copyButton->setIcon(QIcon::fromTheme("edit-copy"));
    m_copyButton->setEnabled(false);
    connect(m_copyButton, &QPushButton::clicked, this, &BJobLogDialog::onCopySelected);
    buttonLayout->addWidget(m_copyButton);

    m_selectAllButton = new QPushButton(tr("Alles auswählen"), this);
    m_selectAllButton->setIcon(QIcon::fromTheme("edit-select-all"));
    connect(m_selectAllButton, &QPushButton::clicked, this, &BJobLogDialog::onSelectAll);
    buttonLayout->addWidget(m_selectAllButton);

    buttonLayout->addStretch();

    // Schließen-Button
    QDialogButtonBox *dialogButtons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::accept);
    buttonLayout->addWidget(dialogButtons);

    mainLayout->addLayout(buttonLayout);

    // Enable copy button when selection changes
    connect(m_logListView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this]() {
                m_copyButton->setEnabled(m_logListView->selectionModel()->hasSelection());
            });
}

void BJobLogDialog::loadJobLog()
{
    if (!m_director) {
        m_logModel->setLogLines({tr("Keine Verbindung zum Director verfügbar.")});
        BLOG_WARNING() << "BJobLogDialog: No Director connection available";
        return;
    }

    // Connect to Director jsonResult signal (job log is JSON data)
    // Use Qt::UniqueConnection to avoid duplicates
    connect(m_director, &BDirector::jsonResult,
            this, &BJobLogDialog::onJobLogReceived,
            Qt::UniqueConnection);

    BLOG_DEBUG() << "BJobLogDialog: Requesting log for Job ID" << m_jobId;

    // Request job log using queued connection (thread-safe)
    QMetaObject::invokeMethod(m_director, "doSend",
                              Qt::QueuedConnection,
                              Q_ARG(BDirector::Command, BDirector::Command::ListJobId),
                              Q_ARG(QString, QString::number(m_jobId)));
}

void BJobLogDialog::onJobLogReceived(BDirector::Command cmd, const QString &response)
{
    // Enum-based routing: only process ListJobId responses
    if (cmd != BDirector::Command::ListJobId) return;

    if (response.isEmpty()) {
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        return;  // Not valid JSON
    }

    if (!doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject result = root["result"].toObject();

    // Verify "joblog" key in result as a safety check
    if (!result.contains("joblog")) {
        return;  // Not a job log response
    }

    // Parse and display the log
    m_logModel->parseJsonResponse(response);
    m_logListView->scrollToBottom();

    // Disconnect after receiving response (we only need it once)
    disconnect(m_director, &BDirector::jsonResult,
              this, &BJobLogDialog::onJobLogReceived);
}

void BJobLogDialog::onCopySelected()
{
    QModelIndexList selectedIndexes = m_logListView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        return;
    }

    // Build text from selected lines
    QStringList selectedLines;
    for (const QModelIndex &index : selectedIndexes) {
        QString line = m_logModel->data(index, Qt::DisplayRole).toString();
        selectedLines.append(line);
    }

    // Copy to clipboard
    QString text = selectedLines.join("\n");
    QApplication::clipboard()->setText(text);

    BLOG_DEBUG() << "BJobLogDialog: Copied" << selectedLines.size() << "lines to clipboard";
}

void BJobLogDialog::onSelectAll()
{
    m_logListView->selectAll();
}
