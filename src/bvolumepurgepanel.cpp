// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bvolumepurgepanel.h"

#include "bbyteformat.h"
#include "blabelstate.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>

namespace {

// Purging many volumes can take a while on a large catalog
const int DefaultReplyTimeoutMs = 600 * 1000;

enum Column { ColVolume, ColPool, ColStatus, ColJobs, ColFiles, ColSize, ColResult, ColumnCount };

} // namespace

BVolumePurgePanel::BVolumePurgePanel(QWidget *parent)
    : QGroupBox(tr("Purge volumes"), parent)
    , m_replyTimer(new QTimer(this))
    , m_replyTimeoutMs(DefaultReplyTimeoutMs)
{
    auto *layout = new QVBoxLayout(this);

    auto *note = new QLabel(tr("Purge removes all job and file records of the chosen volumes from "
                               "the catalog, whatever their retention periods say. "
                               "Click 'Preview' to see what the Director would purge."));
    note->setWordWrap(true);
    layout->addWidget(note);

    auto *poolRow = new QHBoxLayout();
    poolRow->addWidget(new QLabel(tr("Pool:")));
    m_poolCombo = new QComboBox();
    m_poolCombo->setObjectName(QStringLiteral("purgePoolCombo"));
    m_poolCombo->addItem(tr("All pools"), QVariant());
    poolRow->addWidget(m_poolCombo);
    poolRow->addStretch();
    layout->addLayout(poolRow);

    auto *statusRow = new QHBoxLayout();
    statusRow->addWidget(new QLabel(tr("Volume status:")));
    const QStringList defaults = BVolumePurge::defaultStatuses();
    for (const QString &status : BVolumePurge::allowedStatuses()) {
        // The status names are Bareos keywords and stay untranslated
        auto *check = new QCheckBox(status);
        check->setObjectName(QStringLiteral("purgeStatus_") + status);
        check->setProperty("volStatus", status);
        check->setChecked(defaults.contains(status));
        connect(check, &QCheckBox::toggled, this, &BVolumePurgePanel::onSelectionChanged);
        m_statusChecks.append(check);
        statusRow->addWidget(check);
    }
    statusRow->addStretch();
    layout->addLayout(statusRow);

    m_table = new QTableWidget(0, ColumnCount);
    m_table->setObjectName(QStringLiteral("purgeTable"));
    m_table->setHorizontalHeaderLabels({tr("Volume"), tr("Pool"), tr("Status"), tr("Jobs"),
                                        tr("Files"), tr("Size"), tr("Result")});
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_table);

    m_summaryLabel = new QLabel();
    m_summaryLabel->setObjectName(QStringLiteral("purgeSummary"));
    m_summaryLabel->setWordWrap(true);
    layout->addWidget(m_summaryLabel);

    auto *buttonRow = new QHBoxLayout();
    buttonRow->addStretch();
    m_previewButton = new QPushButton(tr("Preview"));
    m_previewButton->setObjectName(QStringLiteral("purgePreviewButton"));
    m_previewButton->setToolTip(tr("Ask the Director which volumes it would purge; nothing is changed"));
    connect(m_previewButton, &QPushButton::clicked, this, &BVolumePurgePanel::onPreviewClicked);
    buttonRow->addWidget(m_previewButton);
    m_purgeButton = new QPushButton(tr("Purge"));
    m_purgeButton->setObjectName(QStringLiteral("purgeButton"));
    m_purgeButton->setToolTip(tr("Purge the volumes of the preview (requires a preview first)"));
    connect(m_purgeButton, &QPushButton::clicked, this, &BVolumePurgePanel::onPurgeClicked);
    buttonRow->addWidget(m_purgeButton);
    layout->addLayout(buttonRow);

    connect(m_poolCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BVolumePurgePanel::onSelectionChanged);

    m_replyTimer->setSingleShot(true);
    connect(m_replyTimer, &QTimer::timeout, this, &BVolumePurgePanel::onReplyTimeout);

    // Without a confirmation function set by the caller: a plain question,
    // "No" is the default
    m_confirm = [this](const QString &title, const QString &text) {
        return QMessageBox::warning(this, title, text, QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No) == QMessageBox::Yes;
    };

    updateButtons();
}

void BVolumePurgePanel::setPoolNames(const QStringList &pools)
{
    const QVariant chosen = m_poolCombo->currentData();
    {
        const QSignalBlocker blocker(m_poolCombo);
        while (m_poolCombo->count() > 1) {
            m_poolCombo->removeItem(1);
        }
        for (const QString &pool : pools) {
            m_poolCombo->addItem(pool, pool);
        }
        const int index = chosen.isValid() ? m_poolCombo->findData(chosen) : 0;
        m_poolCombo->setCurrentIndex(index >= 0 ? index : 0);
    }
    if (m_poolCombo->currentData() != chosen) {
        onSelectionChanged();
    }
}

BVolumePurge::Selection BVolumePurgePanel::currentSelection() const
{
    BVolumePurge::Selection selection;
    const QVariant pool = m_poolCombo->currentData();
    selection.allPools = !pool.isValid();
    selection.pool = pool.toString();
    for (const QCheckBox *check : m_statusChecks) {
        if (check->isChecked()) {
            selection.volStatus.append(check->property("volStatus").toString());
        }
    }
    return selection;
}

void BVolumePurgePanel::setStatus(const QString &text, const char *state)
{
    m_summaryLabel->setText(text);
    bSetLabelState(m_summaryLabel, state);
}

void BVolumePurgePanel::updateButtons()
{
    const bool busy = isBusy();
    m_poolCombo->setEnabled(!busy);
    for (QCheckBox *check : m_statusChecks) {
        check->setEnabled(!busy);
    }
    m_previewButton->setEnabled(!busy);
    // Purge only what was previewed, and only if the preview found something
    m_purgeButton->setEnabled(!busy && m_previewValid && m_previewSelection == currentSelection()
                              && m_preview.purged > 0);
}

void BVolumePurgePanel::onSelectionChanged()
{
    if (isBusy()) {
        return;
    }
    if (m_previewValid) {
        m_previewValid = false;
        clearTable();
        setStatus(tr("The selection was changed. Click 'Preview' again."), "warning");
    }
    updateButtons();
}

void BVolumePurgePanel::onPreviewClicked()
{
    if (isBusy()) {
        return;
    }
    QString error;
    const QString line = BVolumePurge::buildCommand(currentSelection(), BVolumePurge::Mode::DryRun, &error);
    if (line.isEmpty()) {
        setStatus(error, "error");
        return;
    }
    m_previewValid = false;
    clearTable();
    send(Pending::Preview, line);
}

void BVolumePurgePanel::onPurgeClicked()
{
    if (isBusy() || !m_previewValid || m_previewSelection != currentSelection()) {
        updateButtons();
        return;
    }
    // The line is built from the previewed selection, not from the widgets
    QString error;
    const QString line = BVolumePurge::buildCommand(m_previewSelection, BVolumePurge::Mode::Purge, &error);
    if (line.isEmpty()) {
        setStatus(error, "error");
        return;
    }

    const QString where = m_previewSelection.allPools
                              ? tr("all pools")
                              : tr("pool '%1'").arg(m_previewSelection.pool);
    QString text = tr("Purge %n volume(s) in %1 with the status %2?", "", m_preview.purged)
                       .arg(where, m_previewSelection.volStatus.join(QStringLiteral(", ")));
    text += QStringLiteral("\n\n");
    text += tr("Purge ignores the retention periods. All job and file records on these volumes "
               "are removed from the catalog, and their backups can no longer be restored "
               "through the catalog. This cannot be undone.");
    text += QStringLiteral("\n\n");
    text += tr("The Director selects the volumes again when it purges; volumes that changed "
               "since the preview are purged or skipped according to their state then.");
    text += QStringLiteral("\n\n");
    text += tr("Command: %1").arg(line);

    if (!m_confirm || !m_confirm(tr("Confirm Purge"), text)) {
        return;
    }
    // The selection cannot change while the question is open, but be sure
    if (!m_previewValid || m_previewSelection != currentSelection()) {
        updateButtons();
        return;
    }
    // The preview is used up: the same preview is never executed twice
    m_previewValid = false;
    send(Pending::Purge, line);
}

void BVolumePurgePanel::send(Pending kind, const QString &commandLine)
{
    if (!m_sender) {
        setStatus(tr("Not connected to a Director."), "error");
        updateButtons();
        return;
    }
    m_pending = kind;
    m_pendingCommand = commandLine;
    setStatus(kind == Pending::Preview ? tr("Asking the Director for a preview...")
                                       : tr("The Director is purging..."),
              "info");
    updateButtons();
    emit busyChanged(true);
    m_replyTimer->start(m_replyTimeoutMs);
    m_sender(commandLine);
}

void BVolumePurgePanel::finishPending()
{
    m_replyTimer->stop();
    m_pending = Pending::None;
    m_pendingCommand.clear();
    updateButtons();
    emit busyChanged(false);
}

void BVolumePurgePanel::handleReply(const QString &reply)
{
    if (!isBusy()) {
        return;  // A late answer after the time limit, or one nobody asked for
    }
    const Pending kind = m_pending;
    const BVolumePurge::Mode expected =
        kind == Pending::Preview ? BVolumePurge::Mode::DryRun : BVolumePurge::Mode::Purge;

    BVolumePurge::Reply parsed = BVolumePurge::parseReply(reply);
    if (parsed.ok && parsed.mode != expected) {
        parsed.ok = false;
        parsed.error = tr("The Director answered in another mode than the one requested.");
    }

    // The table shows whatever rows came, also with an error
    showReply(parsed);

    if (!parsed.ok) {
        setStatus(kind == Pending::Preview
                      ? tr("Preview failed: %1").arg(parsed.error)
                      : tr("Purge failed: %1").arg(parsed.error),
                  "error");
        finishPending();
        return;
    }

    if (kind == Pending::Preview) {
        m_preview = parsed;
        m_previewSelection = currentSelection();
        m_previewValid = true;
        if (parsed.purged > 0) {
            setStatus(tr("Preview: %1 volumes selected, %2 would be purged, %3 skipped. "
                         "Click 'Purge' to purge them.")
                          .arg(parsed.selected).arg(parsed.purged).arg(parsed.skipped),
                      "success");
        } else {
            setStatus(tr("Preview: %1 volumes selected, none would be purged, %2 skipped.")
                          .arg(parsed.selected).arg(parsed.skipped),
                      "info");
        }
        finishPending();
    } else {
        setStatus(tr("Purge finished: %1 volumes selected, %2 purged, %3 skipped.")
                      .arg(parsed.selected).arg(parsed.purged).arg(parsed.skipped),
                  "success");
        finishPending();
        emit purgeFinished();
    }
}

void BVolumePurgePanel::handleFailure(const QString &reason)
{
    if (!isBusy()) {
        return;
    }
    const Pending kind = m_pending;
    setStatus(kind == Pending::Preview
                  ? tr("Preview failed: %1").arg(reason)
                  : tr("The result of the purge is unknown: %1. Run a preview to see the "
                       "current state.").arg(reason),
              "error");
    finishPending();
}

void BVolumePurgePanel::onReplyTimeout()
{
    handleFailure(tr("no answer from the Director within the time limit"));
}

void BVolumePurgePanel::clearTable()
{
    m_table->setRowCount(0);
}

void BVolumePurgePanel::showReply(const BVolumePurge::Reply &reply)
{
    clearTable();
    m_table->setRowCount(int(reply.volumes.size()));
    for (int row = 0; row < reply.volumes.size(); ++row) {
        const BVolumePurge::VolumeRow &volume = reply.volumes.at(row);

        QString result;
        switch (volume.action) {
        case BVolumePurge::Action::WouldPurge:
            result = tr("Would purge");
            break;
        case BVolumePurge::Action::Purged:
            result = tr("Purged");
            break;
        case BVolumePurge::Action::Skipped:
            result = volume.reason.isEmpty() ? tr("Skipped")
                                             : tr("Skipped: %1").arg(volume.reason);
            break;
        case BVolumePurge::Action::Unknown:
            result = volume.actionText;
            break;
        }

        auto number = [](qint64 value) {
            auto *item = new QTableWidgetItem(QString::number(value));
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            return item;
        };
        m_table->setItem(row, ColVolume, new QTableWidgetItem(volume.volume));
        m_table->setItem(row, ColPool, new QTableWidgetItem(volume.pool));
        m_table->setItem(row, ColStatus, new QTableWidgetItem(volume.volStatus));
        m_table->setItem(row, ColJobs, number(volume.jobs));
        m_table->setItem(row, ColFiles, number(volume.files));
        auto *size = new QTableWidgetItem(BByteFormat::format(volume.bytes));
        size->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_table->setItem(row, ColSize, size);
        m_table->setItem(row, ColResult, new QTableWidgetItem(result));
    }
    m_table->resizeColumnsToContents();
}
