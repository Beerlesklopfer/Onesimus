#ifndef BACLMATRIXDIALOG_H
#define BACLMATRIXDIALOG_H

#include <QDialog>
#include <QHash>
#include <QStringList>

#include "director/baclmatrix.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

/**
 * @brief The ACLs of every console side by side (read only)
 *
 * One column per console, as many as the Director reports: a console added
 * on the Director appears after the next refresh. The rows are the
 * resources, grouped by type of ACL (BAclMatrix::types()); their names stay
 * in place when the table scrolls sideways. A cell says whether the console
 * may use the resource; its tool tip says why (the entry that decides and
 * where it comes from, or the catalog ACL that narrows it). The column's
 * tool tip has the profiles, the state of the catalog ACL and the Where and
 * Plugin Options ACLs.
 *
 * The dialog sends nothing itself: refreshRequested() asks the owner to send
 * ".acl list", and setReply() / setFailure() bring the answer.
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
    /// The command did not get a reply
    void setFailure(const QString &message);

    /// The consoles of the last reply
    const QList<BConsoleAcl> &consoles() const { return m_consoles; }
    QTableWidget *table() const { return m_table; }

signals:
    void refreshRequested();

private:
    void rebuild();
    void applyFilter();

    QList<BConsoleAcl> m_consoles;
    QHash<QString, QStringList> m_known;
    QLineEdit *m_filter = nullptr;
    QCheckBox *m_differing = nullptr;
    QPushButton *m_refresh = nullptr;
    QLabel *m_status = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // BACLMATRIXDIALOG_H
