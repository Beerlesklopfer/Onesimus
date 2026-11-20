#ifndef BACLMATRIXDIALOG_H
#define BACLMATRIXDIALOG_H

#include <QDialog>
#include <QHash>
#include <QPair>
#include <QStringList>

#include <functional>

#include "director/baclmatrix.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;
class QTableWidgetItem;

/**
 * @brief The ACLs of every console side by side, with changes of the
 *        catalog ACL
 *
 * One column per console, as many as the Director reports: a console added
 * on the Director appears after the next refresh. The rows are the
 * resources, grouped by type of ACL (BAclMatrix::types()); their names stay
 * in place when the table scrolls sideways. A cell says whether the console
 * may use the resource; its tool tip says why.
 *
 * Changes: a double click (or Space) on a cell that is allowed denies it
 * through the catalog ACL, on a cell the catalog ACL narrows allows it
 * again (BAclMatrix::catalogAfter()). What the file ACL denies is changed
 * in the editor, not here. Changed cells are underlined until "Apply..."
 * shows every change once (old and new catalog list per console and type)
 * and, confirmed, sends one ".acl set/delete" after the other; the first
 * that the Director refuses stops the rest. Then the ACLs are read again.
 *
 * The dialog sends nothing itself: refreshRequested() and
 * changeRequested() ask the owner, setReply(), setChangeReply() and the
 * failure setters bring the answers.
 */
class BAclMatrixDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BAclMatrixDialog(QWidget *parent = nullptr);

    /// Names of resources the GUI loaded, by ACL key ("jobacl"): rows even
    /// where no console may use them
    void setKnownNames(const QHash<QString, QStringList> &names);

    /// The reply of ".acl list"
    void setReply(const QString &reply);
    /// ".acl list" did not get a reply
    void setFailure(const QString &message);

    /// The reply to the change sent last (changeRequested())
    void setChangeReply(const QString &reply);
    /// The change sent last did not get a reply
    void setChangeFailure(const QString &message);

    /// Asks before anything is written; the default shows a dialog (tests answer it)
    using Confirm = std::function<bool(QWidget *parent, const QString &summary)>;
    void setConfirm(Confirm confirm) { m_confirm = std::move(confirm); }

    /// Turns the cell of a console and a resource; false if it cannot be changed
    bool toggle(int row, int column);
    /// The catalog lists that differ from the Director's, by (console, type)
    int pendingCount() const { return int(m_pending.size()); }
    /// Shows the changes once and, confirmed, sends them
    void apply();
    void discard();

    /// The consoles of the last reply
    const QList<BConsoleAcl> &consoles() const { return m_consoles; }
    QTableWidget *table() const { return m_table; }

signals:
    void refreshRequested();
    /// Send this ".acl set/delete" line; the answer comes with setChangeReply()
    void changeRequested(const QString &command);

private:
    using Key = QPair<QString, QString>;    ///< console, type

    void rebuild();
    void applyFilter();
    void updateButtons();
    void paintCell(QTableWidgetItem *item);
    void sendNext();
    void finishApply(const QString &message, bool reload);
    /// The catalog list of a console and type with the pending changes
    QStringList currentList(const QString &console, const QString &type) const;

    QList<BConsoleAcl> m_consoles;
    QHash<QString, QStringList> m_known;
    QHash<Key, QStringList> m_pending;
    QList<QPair<QString, QString>> m_steps;    ///< command, what it changes
    QStringList m_done;
    bool m_applying = false;
    QString m_afterRefresh;     ///< The outcome of the last change, kept over the refresh
    Confirm m_confirm;

    QLineEdit *m_filter = nullptr;
    QCheckBox *m_differing = nullptr;
    QPushButton *m_refresh = nullptr;
    QPushButton *m_apply = nullptr;
    QPushButton *m_discard = nullptr;
    QLabel *m_status = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // BACLMATRIXDIALOG_H
