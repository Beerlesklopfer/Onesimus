#include "jobs/bjoblogdialog.h"
#include "buiscale.h"
#include "blogging.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QClipboard>

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
    resize(BUiScale::px(800), BUiScale::px(600));
    BUiScale::track(this, [this]() { setMinimumSize(BUiScale::px(600), BUiScale::px(400)); });

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
    // Shared job log view: per-pixel scrolling, follow-tail, live source
    m_logListView = new BJobLogView(this);
    m_logModel = new BJobLogModel(this);
    m_logListView->setModel(m_logModel);
    m_logListView->attach(m_director, m_logModel);

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
        BLOG_WARNING() << "BJobLogDialog: No Director connection available";
    }

    BLOG_DEBUG() << "BJobLogDialog: Requesting log for Job ID" << m_jobId;

    // The view loads the log once the dialog is shown and keeps a running
    // job's log up to date; replies for other jobs are ignored by its source
    m_logListView->showJob(m_jobId, BJobLog::isRunningJobStatus(m_job["jobstatus"].toString()));
}

void BJobLogDialog::onCopySelected()
{
    if (!m_logListView->selectionModel()->hasSelection()) {
        return;
    }

    // Selected lines in row order
    QApplication::clipboard()->setText(m_logListView->selectedText());
}

void BJobLogDialog::onSelectAll()
{
    m_logListView->selectAll();
}
