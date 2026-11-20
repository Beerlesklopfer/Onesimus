#ifndef BACLMATRIXDIALOG_H
#define BACLMATRIXDIALOG_H

#include <QDialog>
#include <QHash>
#include <QPair>
#include <QStringList>

#include <functional>

#include "director/baclmatrix.h"

class QCheckBox;
class QComboBox;
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

    /// The reply of "show profiles"
    void setProfilesReply(const QString &reply);

    /// Consoles (the catalog ACL can be changed) or profiles (file only:
    /// "Config text..." proposes the new Profile resources to copy)
    enum class View { Consoles, Profiles };
    void setView(View view);
    View view() const { return m_view; }

    /// Shows proposed configuration text; the default is a dialog with Copy (tests read it)
    using TextView = std::function<void(QWidget *parent, const QString &text)>;
    void setTextView(TextView view) { m_textView = std::move(view); }

    /// The reply to the change sent last (changeRequested())
    void setChangeReply(const QString &reply);
    /// The change sent last did not get a reply
    void setChangeFailure(const QString &message);

    /// The answer to createConsoleRequested()
    void setCreateResult(bool success, const QString &message);

    /**
     * @brief Asks for the new console: name, password and profile
     * @return false if cancelled; the default shows a dialog (tests answer it)
     */
    using ConsoleForm = std::function<bool(QWidget *parent, const QStringList &profiles,
                                           const QStringList &taken, QString *name,
                                           QString *password, QString *profile)>;
    void setConsoleForm(ConsoleForm form) { m_consoleForm = std::move(form); }

    /**
     * @brief Asks for a new profile: name, description and the profile it
     *        starts as a copy of (empty: no entries, nothing allowed)
     * @return false if cancelled; the default shows a dialog (tests answer it)
     */
    using ProfileForm = std::function<bool(QWidget *parent, const QStringList &profiles,
                                           QString *name, QString *description, QString *copyOf)>;
    void setProfileForm(ProfileForm form) { m_profileForm = std::move(form); }
    /// "New profile...": a proposed column; its text comes with "Config text..."
    void newProfile();
    /// "New console...": asks for it and emits createConsoleRequested()
    void newConsole();
    /// The profiles the consoles of the last reply name
    QStringList knownProfiles() const;

    /// Asks before anything is written; the default shows a dialog (tests answer it)
    using Confirm = std::function<bool(QWidget *parent, const QString &summary)>;
    void setConfirm(Confirm confirm) { m_confirm = std::move(confirm); }

    /// Turns the cell of a console and a resource; false if it cannot be changed
    bool toggle(int row, int column);
    /// Changes not applied in the current view: catalog lists by (console,
    /// type), or profiles with changed lists
    int pendingCount() const;
    /// Shows the changes once and, confirmed, sends them
    void apply();
    void discard();
    /// Cancel: closes; changes that were not applied are discarded after a question
    void reject() override;

    /// The consoles of the last reply
    const QList<BConsoleAcl> &consoles() const { return m_consoles; }
    QTableWidget *table() const { return m_table; }

signals:
    void refreshRequested();
    /// Send this ".acl set/delete" line; the answer comes with setChangeReply()
    void changeRequested(const QString &command);
    /// Create this console (configure add console); the answer comes with setCreateResult()
    void createConsoleRequested(const QString &name, const QString &password, const QString &profile);

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
    /// The file list of a profile and type with the pending changes
    QStringList currentProfileList(const QString &profile, const QString &type) const;
    const QList<BConsoleAcl> &columns() const { return m_view == View::Profiles ? m_profiles : m_consoles; }
    void showProfileTexts();
    QString profileSummary(const BConsoleAcl &profile) const;

    QList<BConsoleAcl> m_consoles;
    QHash<QString, QStringList> m_known;
    QHash<Key, QStringList> m_pending;
    View m_view = View::Consoles;
    QList<BConsoleAcl> m_profiles;
    QHash<QString, QHash<QString, QStringList>> m_profilePending;   ///< profile -> type -> entries
    TextView m_textView;
    QList<QPair<QString, QString>> m_steps;    ///< command, what it changes
    QStringList m_done;
    bool m_applying = false;
    QString m_afterRefresh;     ///< The outcome of the last change, kept over the refresh
    Confirm m_confirm;
    ConsoleForm m_consoleForm;
    ProfileForm m_profileForm;
    QString m_creating;         ///< The console being created; empty: none

    QComboBox *m_viewCombo = nullptr;
    QLabel *m_how = nullptr;
    QLineEdit *m_filter = nullptr;
    QCheckBox *m_differing = nullptr;
    QPushButton *m_refresh = nullptr;
    QPushButton *m_apply = nullptr;
    QPushButton *m_discard = nullptr;
    QPushButton *m_newConsole = nullptr;
    QPushButton *m_newProfile = nullptr;
    QLabel *m_status = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // BACLMATRIXDIALOG_H
