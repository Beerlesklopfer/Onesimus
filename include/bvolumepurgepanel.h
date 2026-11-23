// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BVOLUMEPURGEPANEL_H
#define BVOLUMEPURGEPANEL_H

#include <QGroupBox>
#include <QList>
#include <QStringList>

#include <functional>

#include "bvolumepurge.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTimer;

/**
 * @brief Purge the volumes of a pool or of all pools (Onesimus Director)
 *
 * Part of the database cleanup dialog. The user chooses the pool and the
 * volume statuses, "Preview" sends the dryrun form and shows per volume
 * what the Director would do. Only after a preview of exactly the current
 * selection is "Purge" enabled; it asks for confirmation and sends the
 * "yes" form. Any change of the selection invalidates the preview.
 *
 * The panel knows no Director: the command line leaves through the sender
 * function, the reply comes back through handleReply(). The confirmation is
 * a function too, so that tests can answer it.
 */
class BVolumePurgePanel : public QGroupBox
{
    Q_OBJECT

public:
    using Sender = std::function<void(const QString &commandLine)>;
    /// Returns true if the user agrees; @p text says what will happen
    using Confirm = std::function<bool(const QString &title, const QString &text)>;

    explicit BVolumePurgePanel(QWidget *parent = nullptr);

    void setSender(Sender sender) { m_sender = std::move(sender); }
    void setConfirm(Confirm confirm) { m_confirm = std::move(confirm); }
    void setReplyTimeout(int milliseconds) { m_replyTimeoutMs = milliseconds; }

    /// The pools offered besides "All pools"; keeps the choice if it is still there
    void setPoolNames(const QStringList &pools);

    /// The selection the widgets show now
    BVolumePurge::Selection currentSelection() const;

    /// Waiting for the Director's answer
    bool isBusy() const { return m_pending != Pending::None; }
    /// A preview of the current selection is shown and can be executed
    bool hasValidPreview() const { return m_previewValid; }

    /// The Director's answer to the command sent last (ignored when nothing is awaited)
    void handleReply(const QString &reply);
    /// The command sent last has no answer (connection lost, time limit)
    void handleFailure(const QString &reason);

signals:
    void busyChanged(bool busy);
    /// After the Director answered a "yes" command without an error
    void purgeFinished();

private slots:
    void onPreviewClicked();
    void onPurgeClicked();
    void onSelectionChanged();
    void onReplyTimeout();

private:
    enum class Pending { None, Preview, Purge };

    void send(Pending kind, const QString &commandLine);
    void finishPending();
    void showReply(const BVolumePurge::Reply &reply);
    void clearTable();
    void updateButtons();
    void setStatus(const QString &text, const char *state);

    QComboBox *m_poolCombo;
    QList<QCheckBox *> m_statusChecks;
    QPushButton *m_previewButton;
    QPushButton *m_purgeButton;
    QTableWidget *m_table;
    QLabel *m_summaryLabel;
    QTimer *m_replyTimer;

    Sender m_sender;
    Confirm m_confirm;
    int m_replyTimeoutMs;

    Pending m_pending = Pending::None;
    QString m_pendingCommand;
    // The preview the "Purge" button executes: valid only for this selection
    bool m_previewValid = false;
    BVolumePurge::Selection m_previewSelection;
    BVolumePurge::Reply m_preview;
};

#endif // BVOLUMEPURGEPANEL_H
