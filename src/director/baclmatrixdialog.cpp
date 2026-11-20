#include "director/baclmatrixdialog.h"

#include "buiscale.h"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QRegularExpressionValidator>
#include <QDialogButtonBox>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

namespace {

constexpr int TypeRole = Qt::UserRole;          ///< On the vertical header item: ACL key
constexpr int GroupRole = Qt::UserRole + 1;     ///< true for the row of a type's title
constexpr int DiffersRole = Qt::UserRole + 2;
// On a cell
constexpr int CellTypeRole = Qt::UserRole + 3;
constexpr int ResourceRole = Qt::UserRole + 4;
constexpr int OriginalRole = Qt::UserRole + 5;  ///< BAclMatrix::Cell as the Director reported it
constexpr int ReasonRole = Qt::UserRole + 6;

QString symbol(BAclMatrix::Cell cell)
{
    switch (cell) {
    case BAclMatrix::Cell::Allowed: return QStringLiteral("✓");            // ✓
    case BAclMatrix::Cell::Denied: return QStringLiteral("✗");             // ✗
    case BAclMatrix::Cell::NotListed: return QStringLiteral("·");          // ·
    case BAclMatrix::Cell::NarrowedByCatalog: return QStringLiteral("⊘");  // ⊘
    case BAclMatrix::Cell::Locked: return QStringLiteral("—");             // —
    }
    return QString();
}

QColor colour(BAclMatrix::Cell cell, const QPalette &palette)
{
    switch (cell) {
    case BAclMatrix::Cell::Allowed: return QColor(0x3a, 0x9d, 0x4f);
    case BAclMatrix::Cell::Denied: return QColor(0xd9, 0x53, 0x4f);
    case BAclMatrix::Cell::NarrowedByCatalog: return QColor(0xe6, 0x95, 0x00);
    case BAclMatrix::Cell::NotListed:
    case BAclMatrix::Cell::Locked: break;
    }
    return palette.color(QPalette::PlaceholderText);
}

bool confirmDialog(QWidget *parent, const QString &summary)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(BAclMatrixDialog::tr("Change the catalog ACL"));
    auto *layout = new QVBoxLayout(&dialog);
    auto *intro = new QLabel(BAclMatrixDialog::tr("These changes of the catalog ACL are sent to the Director, "
                                                  "one after the other. The file ACL is not changed. The first "
                                                  "change the Director refuses stops the rest."), &dialog);
    intro->setWordWrap(true);
    layout->addWidget(intro);
    auto *text = new QPlainTextEdit(summary, &dialog);
    text->setReadOnly(true);
    text->setLineWrapMode(QPlainTextEdit::NoWrap);
    text->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    text->setMinimumSize(BUiScale::px(560), BUiScale::px(260));
    layout->addWidget(text, 1);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, &dialog);
    buttons->addButton(BAclMatrixDialog::tr("Apply"), QDialogButtonBox::AcceptRole);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    return dialog.exec() == QDialog::Accepted;
}

/// A password for a new console: 24 characters that need no escaping in
/// a configuration file or on a command line
QString newPassword()
{
    static const QString chars = QStringLiteral("ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789-_.");
    QString password;
    for (int i = 0; i < 24; ++i)
        password.append(chars.at(QRandomGenerator::system()->bounded(int(chars.size()))));
    return password;
}

bool consoleFormDialog(QWidget *parent, const QStringList &profiles, const QStringList &taken,
                       QString *name, QString *password, QString *profile)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(BAclMatrixDialog::tr("New console"));
    auto *layout = new QVBoxLayout(&dialog);
    auto *intro = new QLabel(BAclMatrixDialog::tr(
        "The Director creates a Console resource with this name, password and profile (configure "
        "add console). The profile's ACLs are the frame of the console; the matrix can narrow them "
        "further through the catalog ACL afterwards."), &dialog);
    intro->setWordWrap(true);
    layout->addWidget(intro);
    auto *form = new QFormLayout;
    auto *nameEdit = new QLineEdit(&dialog);
    // Bareos resource names: letters, digits and _ - . : (no blanks: the
    // command line would need quotes the Director's configure does not take)
    nameEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression(QStringLiteral("[A-Za-z0-9_.:-]{1,127}")), nameEdit));
    nameEdit->setPlaceholderText(BAclMatrixDialog::tr("e.g. backup-viewer"));
    form->addRow(BAclMatrixDialog::tr("&Name:"), nameEdit);
    auto *passwordRow = new QHBoxLayout;
    auto *passwordEdit = new QLineEdit(newPassword(), &dialog);
    passwordEdit->setReadOnly(true);
    passwordEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    passwordRow->addWidget(passwordEdit, 1);
    auto *again = new QPushButton(BAclMatrixDialog::tr("New"), &dialog);
    again->setToolTip(BAclMatrixDialog::tr("Generates another password"));
    QObject::connect(again, &QPushButton::clicked, passwordEdit, [passwordEdit]() { passwordEdit->setText(newPassword()); });
    passwordRow->addWidget(again);
    auto *copy = new QPushButton(BAclMatrixDialog::tr("Copy"), &dialog);
    copy->setToolTip(BAclMatrixDialog::tr("Copies the password to the clipboard"));
    QObject::connect(copy, &QPushButton::clicked, passwordEdit, [passwordEdit]() {
        QApplication::clipboard()->setText(passwordEdit->text());
    });
    passwordRow->addWidget(copy);
    form->addRow(BAclMatrixDialog::tr("&Password:"), passwordRow);
    auto *profileCombo = new QComboBox(&dialog);
    profileCombo->setEditable(true);
    profileCombo->addItems(profiles);
    const int op = profiles.indexOf(QStringLiteral("operator"));
    if (op >= 0) profileCombo->setCurrentIndex(op);
    profileCombo->setToolTip(BAclMatrixDialog::tr("The Profile resource whose ACLs the console gets"));
    form->addRow(BAclMatrixDialog::tr("P&rofile:"), profileCombo);
    layout->addLayout(form);
    auto *note = new QLabel(BAclMatrixDialog::tr(
        "Copy the password now: whoever uses the console needs it, and it cannot be read back here."), &dialog);
    note->setWordWrap(true);
    layout->addWidget(note);
    auto *problem = new QLabel(&dialog);
    problem->setWordWrap(true);
    layout->addWidget(problem);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, &dialog);
    QPushButton *create = buttons->addButton(BAclMatrixDialog::tr("Create"), QDialogButtonBox::AcceptRole);
    auto check = [&]() {
        const QString n = nameEdit->text().trimmed();
        QString why;
        if (n.isEmpty()) why = BAclMatrixDialog::tr("Enter a name.");
        else if (taken.contains(n, Qt::CaseInsensitive)) why = BAclMatrixDialog::tr("A console of this name exists.");
        else if (profileCombo->currentText().trimmed().isEmpty()) why = BAclMatrixDialog::tr("Choose a profile.");
        problem->setText(why);
        create->setEnabled(why.isEmpty());
    };
    QObject::connect(nameEdit, &QLineEdit::textChanged, &dialog, check);
    QObject::connect(profileCombo, &QComboBox::currentTextChanged, &dialog, check);
    check();
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    *name = nameEdit->text().trimmed();
    *password = passwordEdit->text();
    *profile = profileCombo->currentText().trimmed();
    return true;
}

void textDialog(QWidget *parent, const QString &text)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(BAclMatrixDialog::tr("Proposed configuration text"));
    auto *layout = new QVBoxLayout(&dialog);
    auto *intro = new QLabel(BAclMatrixDialog::tr(
        "Profiles are changed in the configuration files of the Director only. Put this text in place "
        "of the Profile resource in its file (the path is a suggestion), check it with the Director's "
        "test (-t) and reload; then Refresh shows the new state. Comments of the old resource are not "
        "in this text."), &dialog);
    intro->setWordWrap(true);
    layout->addWidget(intro);
    auto *edit = new QPlainTextEdit(text, &dialog);
    edit->setReadOnly(true);
    edit->setLineWrapMode(QPlainTextEdit::NoWrap);
    edit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    edit->setMinimumSize(BUiScale::px(620), BUiScale::px(320));
    layout->addWidget(edit, 1);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    QPushButton *copy = buttons->addButton(BAclMatrixDialog::tr("Copy"), QDialogButtonBox::ActionRole);
    QObject::connect(copy, &QPushButton::clicked, &dialog, [text]() { QApplication::clipboard()->setText(text); });
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    dialog.exec();
}

bool profileFormDialog(QWidget *parent, const QStringList &profiles, QString *name,
                       QString *description, QString *copyOf)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(BAclMatrixDialog::tr("New profile"));
    auto *layout = new QVBoxLayout(&dialog);
    auto *intro = new QLabel(BAclMatrixDialog::tr(
        "The new profile is a proposal in the matrix: change its cells, then \"Config text...\" shows its "
        "Profile resource to put into the Director's configuration. Nothing is sent."), &dialog);
    intro->setWordWrap(true);
    layout->addWidget(intro);
    auto *form = new QFormLayout;
    auto *nameEdit = new QLineEdit(&dialog);
    nameEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression(QStringLiteral("[A-Za-z0-9_.:-]{1,127}")), nameEdit));
    nameEdit->setPlaceholderText(BAclMatrixDialog::tr("e.g. restore-only"));
    form->addRow(BAclMatrixDialog::tr("&Name:"), nameEdit);
    auto *descriptionEdit = new QLineEdit(&dialog);
    form->addRow(BAclMatrixDialog::tr("&Description:"), descriptionEdit);
    auto *copyCombo = new QComboBox(&dialog);
    copyCombo->addItem(BAclMatrixDialog::tr("(empty: nothing allowed)"), QString());
    for (const QString &profile : profiles) copyCombo->addItem(profile, profile);
    copyCombo->setToolTip(BAclMatrixDialog::tr("The ACL entries the new profile starts with"));
    form->addRow(BAclMatrixDialog::tr("&Start as a copy of:"), copyCombo);
    layout->addLayout(form);
    auto *problem = new QLabel(&dialog);
    problem->setWordWrap(true);
    layout->addWidget(problem);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, &dialog);
    QPushButton *create = buttons->addButton(BAclMatrixDialog::tr("Add to the matrix"), QDialogButtonBox::AcceptRole);
    auto check = [&]() {
        const QString n = nameEdit->text().trimmed();
        QString why;
        if (n.isEmpty()) why = BAclMatrixDialog::tr("Enter a name.");
        else if (profiles.contains(n, Qt::CaseInsensitive)) why = BAclMatrixDialog::tr("A profile of this name exists.");
        problem->setText(why);
        create->setEnabled(why.isEmpty());
    };
    QObject::connect(nameEdit, &QLineEdit::textChanged, &dialog, check);
    check();
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    *name = nameEdit->text().trimmed();
    *description = descriptionEdit->text().trimmed();
    *copyOf = copyCombo->currentData().toString();
    return true;
}

QString listText(const QStringList &list)
{
    return list.isEmpty() ? BAclMatrixDialog::tr("(no entries: no restriction)") : list.join(QStringLiteral(", "));
}

/// Space on a cell turns it, like a double click
class SpaceFilter : public QObject
{
public:
    SpaceFilter(BAclMatrixDialog *dialog, QTableWidget *table) : QObject(table), m_dialog(dialog), m_table(table) {}
    bool eventFilter(QObject *, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress && static_cast<QKeyEvent *>(event)->key() == Qt::Key_Space
            && m_table->currentItem()) {
            m_dialog->toggle(m_table->currentRow(), m_table->currentColumn());
            return true;
        }
        return false;
    }

private:
    BAclMatrixDialog *m_dialog;
    QTableWidget *m_table;
};

} // namespace

BAclMatrixDialog::BAclMatrixDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("ACL matrix of the consoles"));
    setObjectName(QStringLiteral("aclMatrix"));
    auto *layout = new QVBoxLayout(this);

    auto *row = new QHBoxLayout;
    m_viewCombo = new QComboBox(this);
    m_viewCombo->addItem(tr("Consoles"));
    m_viewCombo->addItem(tr("Profiles"));
    m_viewCombo->setToolTip(tr("Consoles: what each console may use, the catalog ACL can be changed. "
                               "Profiles: the ACLs of the Profile resources, changed as proposed "
                               "configuration text."));
    connect(m_viewCombo, &QComboBox::currentIndexChanged, this, [this](int index) {
        setView(index == 1 ? View::Profiles : View::Consoles);
    });
    row->addWidget(m_viewCombo);
    m_filter = new QLineEdit(this);
    m_filter->setPlaceholderText(tr("Filter resources"));
    m_filter->setClearButtonEnabled(true);
    m_filter->setAccessibleName(tr("Filter resources"));
    connect(m_filter, &QLineEdit::textChanged, this, &BAclMatrixDialog::applyFilter);
    row->addWidget(m_filter, 1);
    m_differing = new QCheckBox(tr("Only rows where the consoles differ"), this);
    connect(m_differing, &QCheckBox::toggled, this, &BAclMatrixDialog::applyFilter);
    row->addWidget(m_differing);
    m_refresh = new QPushButton(tr("Refresh"), this);
    m_refresh->setToolTip(tr("Reads the ACLs of every console again (.acl list)"));
    connect(m_refresh, &QPushButton::clicked, this, [this]() {
        m_status->setText(tr("Reading the ACLs of the consoles..."));
        emit refreshRequested();
    });
    row->addWidget(m_refresh);
    m_newConsole = new QPushButton(tr("New console..."), this);
    m_newConsole->setToolTip(tr("Creates a console on the Director with a password and a profile "
                                "(configure add console); it appears as a new column"));
    connect(m_newConsole, &QPushButton::clicked, this, &BAclMatrixDialog::newConsole);
    row->addWidget(m_newConsole);
    m_newProfile = new QPushButton(tr("New profile..."), this);
    m_newProfile->setToolTip(tr("Adds a proposed profile as a new column; \"Config text...\" shows its "
                                "Profile resource to put into the configuration"));
    connect(m_newProfile, &QPushButton::clicked, this, &BAclMatrixDialog::newProfile);
    row->addWidget(m_newProfile);
    layout->addLayout(row);

    m_table = new QTableWidget(this);
    m_table->setAccessibleName(tr("ACL matrix: resources by console"));
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_table->setWordWrap(false);
    // The names of the resources are the row headers: they stay in place
    // when the consoles scroll sideways
    m_table->verticalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(m_table, &QTableWidget::cellDoubleClicked, this, [this](int r, int c) { toggle(r, c); });
    m_table->installEventFilter(new SpaceFilter(this, m_table));
    layout->addWidget(m_table, 1);

    auto *legend = new QLabel(this);
    legend->setWordWrap(true);
    legend->setText(tr("✓ allowed   ✗ denied by an entry with \"!\"   · not allowed   "
                       "⊘ allowed by the file ACL, narrowed by the catalog ACL   "
                       "— console locked. A cell's tool tip says why; a console's tool tip "
                       "has its profiles, its catalog ACL and its Where and Plugin Options ACLs."));
    layout->addWidget(legend);
    m_how = new QLabel(this);
    m_how->setWordWrap(true);
    layout->addWidget(m_how);

    auto *bottom = new QHBoxLayout;
    // Cancel on the left, as in every dialog
    auto *cancel = new QPushButton(tr("Cancel"), this);
    cancel->setToolTip(tr("Closes the matrix; changes that were not applied are discarded"));
    connect(cancel, &QPushButton::clicked, this, &BAclMatrixDialog::reject);
    bottom->addWidget(cancel, 0, Qt::AlignTop);
    m_status = new QLabel(this);
    m_status->setWordWrap(true);
    m_status->setTextInteractionFlags(Qt::TextSelectableByMouse);
    bottom->addWidget(m_status, 1);
    m_discard = new QPushButton(tr("Discard"), this);
    m_discard->setToolTip(tr("Forgets the changes that were not applied"));
    connect(m_discard, &QPushButton::clicked, this, &BAclMatrixDialog::discard);
    bottom->addWidget(m_discard, 0, Qt::AlignTop);
    m_apply = new QPushButton(tr("Apply..."), this);
    m_apply->setToolTip(tr("Shows every change of the catalog ACL once and sends them after your "
                           "confirmation (.acl set / .acl delete)"));
    connect(m_apply, &QPushButton::clicked, this, &BAclMatrixDialog::apply);
    bottom->addWidget(m_apply, 0, Qt::AlignTop);
    layout->addLayout(bottom);
    setView(View::Consoles);

    resize(BUiScale::px(900), BUiScale::px(640));
}

void BAclMatrixDialog::setKnownNames(const QHash<QString, QStringList> &names)
{
    m_known = names;
    if (!m_consoles.isEmpty()) rebuild();
}

void BAclMatrixDialog::setReply(const QString &reply)
{
    QList<BConsoleAcl> consoles;
    QString error;
    if (!BAclMatrix::parse(reply, &consoles, &error)) {
        setFailure(error);
        return;
    }
    m_consoles = consoles;
    // A change that the Director has now is no longer pending
    for (auto it = m_pending.begin(); it != m_pending.end();) {
        bool same = true;
        for (const BConsoleAcl &console : m_consoles) {
            if (console.name == it.key().first)
                same = BAclMatrix::catalogList(console, it.key().second) == it.value();
        }
        it = same ? m_pending.erase(it) : std::next(it);
    }
    rebuild();
    bool effective = false;
    for (const BConsoleAcl &console : m_consoles) {
        if (!console.effective.isEmpty()) effective = true;
    }
    const QString counted = effective
        ? tr("%n console(s). Allowed is what the Director decided (file ACL and catalog ACL).", "",
             int(m_consoles.size()))
        : tr("%n console(s). This Director reports the file ACL only; the GUI decided the cells "
             "from its entries.", "", int(m_consoles.size()));
    m_status->setText(m_afterRefresh.isEmpty() ? counted : m_afterRefresh + QLatin1Char(' ') + counted);
    m_afterRefresh.clear();
}

void BAclMatrixDialog::setProfilesReply(const QString &reply)
{
    QList<BConsoleAcl> profiles;
    QString error;
    if (!BAclMatrix::parseProfiles(reply, &profiles, &error)) {
        if (m_view == View::Profiles) m_status->setText(tr("The profiles could not be read: %1").arg(error));
        return;
    }
    // Proposed profiles stay until the Director has one of their name
    for (const BConsoleAcl &mine : std::as_const(m_profiles)) {
        if (!mine.proposed) continue;
        bool there = false;
        for (const BConsoleAcl &p : profiles) there = there || p.name.compare(mine.name, Qt::CaseInsensitive) == 0;
        if (!there) profiles.append(mine);
    }
    m_profiles = profiles;
    // A proposal the Director has now is no longer pending
    for (auto it = m_profilePending.begin(); it != m_profilePending.end();) {
        bool same = false;
        for (const BConsoleAcl &profile : m_profiles) {
            if (profile.name != it.key()) continue;
            same = true;
            for (auto t = it->cbegin(); t != it->cend(); ++t) {
                if (BAclMatrix::fileList(profile, t.key()) != t.value()) same = false;
            }
        }
        it = same ? m_profilePending.erase(it) : std::next(it);
    }
    if (m_view == View::Profiles) {
        rebuild();
        m_status->setText(tr("%n profile(s). Profiles are changed in the configuration files: "
                             "\"Config text...\" proposes the new text.", "", int(m_profiles.size())));
    }
}

void BAclMatrixDialog::setView(View view)
{
    const int index = view == View::Profiles ? 1 : 0;
    if (m_viewCombo->currentIndex() != index) {
        m_viewCombo->setCurrentIndex(index);    // comes back here
        return;
    }
    m_view = view;
    m_newConsole->setVisible(view == View::Consoles);
    m_newProfile->setVisible(view == View::Profiles);
    m_apply->setText(view == View::Profiles ? tr("Config text...") : tr("Apply..."));
    m_apply->setToolTip(view == View::Profiles
        ? tr("Shows the new text of every changed profile, to put into the Director's configuration")
        : tr("Shows every change of the catalog ACL once and sends them after your "
             "confirmation (.acl set / .acl delete)"));
    m_how->setText(view == View::Profiles
        ? tr("A double click (or Space) on a cell turns it in the proposal: ✓ becomes denied, "
             "· or ✗ allowed. Changed cells are underlined; \"Config text...\" shows the new "
             "Profile resources. Nothing is sent: a person puts the text in.")
        : tr("A double click (or Space) on ✓ denies it through the catalog ACL, on ⊘ allows "
             "it again. Changed cells are underlined until they are applied. What the file ACL "
             "denies is changed in the editor."));
    rebuild();
    if (view == View::Profiles && m_profiles.isEmpty())
        m_status->setText(tr("No profiles read yet: Refresh reads them."));
}

int BAclMatrixDialog::pendingCount() const
{
    if (m_view == View::Consoles) return int(m_pending.size());
    int n = int(m_profilePending.size());
    for (const BConsoleAcl &profile : m_profiles) {
        if (profile.proposed && !m_profilePending.contains(profile.name)) ++n;
    }
    return n;
}

void BAclMatrixDialog::newProfile()
{
    QStringList names;
    for (const BConsoleAcl &profile : m_profiles) names.append(profile.name);
    QString name, description, copyOf;
    const ProfileForm form = m_profileForm ? m_profileForm : profileFormDialog;
    if (!form(this, names, &name, &description, &copyOf))
        return;
    BConsoleAcl profile;
    profile.name = name;
    profile.description = description;
    profile.proposed = true;
    for (const BConsoleAcl &source : std::as_const(m_profiles)) {
        if (source.name != copyOf) continue;
        for (auto it = source.file.cbegin(); it != source.file.cend(); ++it) {
            QList<BAclEntry> entries;
            for (const BAclEntry &entry : it.value())
                entries.append(BAclEntry{entry.value, tr("Profile %1").arg(name)});
            profile.file.insert(it.key(), entries);
        }
    }
    m_profiles.append(profile);
    rebuild();
    m_status->setText(tr("Profile %1 is proposed: change its cells, then \"Config text...\" shows it.").arg(name));
}

QStringList BAclMatrixDialog::currentProfileList(const QString &profile, const QString &type) const
{
    const auto it = m_profilePending.constFind(profile);
    if (it != m_profilePending.constEnd() && it->contains(type)) return it->value(type);
    for (const BConsoleAcl &p : m_profiles) {
        if (p.name == profile) return BAclMatrix::fileList(p, type);
    }
    return {};
}

QString BAclMatrixDialog::profileSummary(const BConsoleAcl &profile) const
{
    QStringList usedBy;
    for (const BConsoleAcl &console : m_consoles) {
        if (console.profiles.contains(profile.name)) usedBy.append(console.name);
    }
    QStringList lines;
    lines.append(tr("Profile %1").arg(profile.name));
    if (profile.proposed) lines.append(tr("Proposed: not on the Director yet."));
    if (!profile.description.isEmpty()) lines.append(profile.description);
    lines.append(usedBy.isEmpty() ? tr("Used by no console") : tr("Used by: %1").arg(usedBy.join(QStringLiteral(", "))));
    for (const BAclMatrix::Type &type : BAclMatrix::patternTypes()) {
        const QStringList values = BAclMatrix::fileList(profile, type.key);
        lines.append(tr("%1: %2").arg(type.title, values.isEmpty() ? tr("none") : values.join(QStringLiteral(", "))));
    }
    return lines.join(QLatin1Char('\n'));
}

void BAclMatrixDialog::showProfileTexts()
{
    QString text;
    QStringList names = m_profilePending.keys();
    for (const BConsoleAcl &profile : m_profiles) {
        if (profile.proposed && !names.contains(profile.name)) names.append(profile.name);
    }
    std::sort(names.begin(), names.end());
    for (const QString &name : names) {
        for (const BConsoleAcl &profile : m_profiles) {
            if (profile.name != name) continue;
            text += QStringLiteral("# /etc/bareos/bareos-dir.d/profile/%1.conf\n").arg(name)
                    + BAclMatrix::profileText(profile, m_profilePending.value(name)) + QLatin1Char('\n');
        }
    }
    const TextView view = m_textView ? m_textView : textDialog;
    view(this, text);
    m_status->setText(tr("The proposal stays until the Director has it: after the text is in place and "
                         "the Director reloaded, Refresh shows it."));
}

void BAclMatrixDialog::setFailure(const QString &message)
{
    m_status->setText(tr("The ACLs could not be read: %1").arg(message));
}

QStringList BAclMatrixDialog::currentList(const QString &console, const QString &type) const
{
    const auto it = m_pending.constFind(Key(console, type));
    if (it != m_pending.constEnd()) return it.value();
    for (const BConsoleAcl &c : m_consoles) {
        if (c.name == console) return BAclMatrix::catalogList(c, type);
    }
    return {};
}

void BAclMatrixDialog::paintCell(QTableWidgetItem *item)
{
    const int column = item->column();
    if (column < 0 || column >= columns().size()) return;
    const BConsoleAcl &console = columns()[column];
    const QString type = item->data(CellTypeRole).toString();
    const QString resource = item->data(ResourceRole).toString();
    const auto original = BAclMatrix::Cell(item->data(OriginalRole).toInt());
    QString tip = QStringLiteral("%1 — %2\n%3").arg(console.name, resource, item->data(ReasonRole).toString());
    if (m_view == View::Profiles) {
        BAclMatrix::Cell shown = original;
        const auto pending = m_profilePending.constFind(console.name);
        if (pending != m_profilePending.constEnd() && pending->contains(type)) {
            QList<BAclEntry> entries;
            for (const QString &value : pending->value(type)) entries.append(BAclEntry{value, QString()});
            shown = BAclMatrix::fileDecision(entries, resource);
        }
        const bool changed = shown != original;
        item->setText(symbol(shown));
        item->setForeground(colour(shown, palette()));
        QFont font = m_table->font();
        font.setUnderline(changed);
        font.setBold(changed);
        item->setFont(font);
        tip += QLatin1Char('\n') + (changed ? (shown == BAclMatrix::Cell::Allowed ? tr("In the proposal: allowed.")
                                                                                  : tr("In the proposal: denied."))
                                             : tr("Double click: change it in the proposed configuration text."));
        item->setToolTip(tip);
        item->setData(Qt::AccessibleTextRole, tip);
        return;
    }

    BAclMatrix::Cell shown = original;
    const auto pending = m_pending.constFind(Key(console.name, type));
    if (pending != m_pending.constEnd()
        && (original == BAclMatrix::Cell::Allowed || original == BAclMatrix::Cell::NarrowedByCatalog)) {
        shown = pending->isEmpty() || BAclMatrix::listAllows(*pending, resource)
            ? BAclMatrix::Cell::Allowed : BAclMatrix::Cell::NarrowedByCatalog;
    }
    const bool changed = shown != original;
    item->setText(symbol(shown));
    item->setForeground(colour(shown, palette()));
    QFont font = m_table->font();
    font.setUnderline(changed);
    font.setBold(changed);
    item->setFont(font);
    if (changed)
        tip += QLatin1Char('\n') + (shown == BAclMatrix::Cell::Allowed
                                        ? tr("Not applied yet: allowed again through the catalog ACL.")
                                        : tr("Not applied yet: denied through the catalog ACL."));
    else if (BAclMatrix::editable(console, original))
        tip += QLatin1Char('\n') + (original == BAclMatrix::Cell::Allowed
                                        ? tr("Double click: deny it through the catalog ACL.")
                                        : tr("Double click: allow it again."));
    item->setToolTip(tip);
    item->setData(Qt::AccessibleTextRole, tip);
}

bool BAclMatrixDialog::toggle(int row, int column)
{
    QTableWidgetItem *item = m_table->item(row, column);
    if (m_applying || !item || column >= columns().size() || !item->data(CellTypeRole).isValid())
        return false;
    if (m_view == View::Profiles) {
        const BConsoleAcl &profile = m_profiles[column];
        const QString type = item->data(CellTypeRole).toString();
        const QString resource = item->data(ResourceRole).toString();
        const QStringList now = currentProfileList(profile.name, type);
        const QStringList next = BAclMatrix::fileListAfter(now, resource, !BAclMatrix::listAllows(now, resource));
        QHash<QString, QStringList> lists = m_profilePending.value(profile.name);
        if (next == BAclMatrix::fileList(profile, type)) lists.remove(type);
        else lists.insert(type, next);
        if (lists.isEmpty()) m_profilePending.remove(profile.name);
        else m_profilePending.insert(profile.name, lists);
        for (int r = 0; r < m_table->rowCount(); ++r) {
            QTableWidgetItem *cell = m_table->item(r, column);
            if (cell && cell->data(CellTypeRole).toString() == type) paintCell(cell);
        }
        updateButtons();
        return true;
    }
    const BConsoleAcl &console = m_consoles[column];
    const auto original = BAclMatrix::Cell(item->data(OriginalRole).toInt());
    if (!BAclMatrix::editable(console, original)) {
        m_status->setText(original == BAclMatrix::Cell::Locked
            ? tr("Console %1 is locked: its catalog ACL cannot be changed until it can be read.").arg(console.name)
            : tr("%1 is not allowed by the file ACL of console %2: that is changed in the editor, "
                 "not here.").arg(item->data(ResourceRole).toString(), console.name));
        return false;
    }
    const QString type = item->data(CellTypeRole).toString();
    const QString resource = item->data(ResourceRole).toString();
    const QStringList now = currentList(console.name, type);
    const bool allowedNow = now.isEmpty() || BAclMatrix::listAllows(now, resource);
    const QStringList next = BAclMatrix::catalogAfter(now, resource, !allowedNow);
    if (next == BAclMatrix::catalogList(console, type))
        m_pending.remove(Key(console.name, type));
    else
        m_pending.insert(Key(console.name, type), next);
    // A list holds every resource of its type: the whole column may change
    for (int r = 0; r < m_table->rowCount(); ++r) {
        QTableWidgetItem *cell = m_table->item(r, column);
        if (cell && cell->data(CellTypeRole).toString() == type) paintCell(cell);
    }
    updateButtons();
    return true;
}

void BAclMatrixDialog::updateButtons()
{
    m_apply->setEnabled(!m_applying && pendingCount() > 0);
    m_discard->setEnabled(!m_applying && pendingCount() > 0);
    m_refresh->setEnabled(!m_applying);
}

void BAclMatrixDialog::reject()
{
    if (!m_pending.isEmpty() && !m_applying) {
        const auto answer = QMessageBox::question(
            this, tr("Discard the changes?"),
            tr("%n change(s) of the catalog ACL were not applied. Discard them and close?", "",
               int(m_pending.size())),
            QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
        if (answer != QMessageBox::Discard)
            return;
    }
    const View shown = m_view;
    m_view = View::Profiles;
    const int proposals = pendingCount();
    m_view = shown;
    if (proposals > 0) {
        const auto answer = QMessageBox::question(
            this, tr("Discard the proposals?"),
            tr("%n profile proposal(s) are only in the matrix. Discard them and close?", "", proposals),
            QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
        if (answer != QMessageBox::Discard)
            return;
    }
    m_pending.clear();
    m_profilePending.clear();
    m_profiles.erase(std::remove_if(m_profiles.begin(), m_profiles.end(),
                                    [](const BConsoleAcl &p) { return p.proposed; }), m_profiles.end());
    rebuild();
    QDialog::reject();
}

QStringList BAclMatrixDialog::knownProfiles() const
{
    QStringList profiles;
    for (const BConsoleAcl &profile : m_profiles) {
        if (!profile.proposed) profiles.append(profile.name);   // a console needs one the Director has
    }
    for (const BConsoleAcl &console : m_consoles) {
        for (const QString &profile : console.profiles) {
            if (!profiles.contains(profile)) profiles.append(profile);
        }
    }
    if (!profiles.contains(QStringLiteral("operator"))) profiles.append(QStringLiteral("operator"));
    std::sort(profiles.begin(), profiles.end());
    return profiles;
}

void BAclMatrixDialog::newConsole()
{
    if (!m_creating.isEmpty()) return;
    QStringList taken;
    for (const BConsoleAcl &console : m_consoles) taken.append(console.name);
    QString name, password, profile;
    const ConsoleForm form = m_consoleForm ? m_consoleForm : consoleFormDialog;
    if (!form(this, knownProfiles(), taken, &name, &password, &profile))
        return;
    m_creating = name;
    m_newConsole->setEnabled(false);
    m_status->setText(tr("Creating console %1...").arg(name));
    emit createConsoleRequested(name, password, profile);
}

void BAclMatrixDialog::setCreateResult(bool success, const QString &message)
{
    if (m_creating.isEmpty()) return;
    const QString name = m_creating;
    m_creating.clear();
    m_newConsole->setEnabled(true);
    if (!success) {
        m_status->setText(tr("Console %1 was not created: %2").arg(name, message.isEmpty() ? tr("refused") : message));
        return;
    }
    m_afterRefresh = tr("Console %1 was created.").arg(name);
    m_status->setText(m_afterRefresh);
    emit refreshRequested();
}

void BAclMatrixDialog::discard()
{
    if (m_view == View::Profiles) {
        m_profilePending.clear();
        m_profiles.erase(std::remove_if(m_profiles.begin(), m_profiles.end(),
                                        [](const BConsoleAcl &p) { return p.proposed; }), m_profiles.end());
    } else {
        m_pending.clear();
    }
    rebuild();
    m_status->setText(tr("The changes were discarded."));
}

void BAclMatrixDialog::apply()
{
    if (m_view == View::Profiles) {
        if (pendingCount() > 0) showProfileTexts();
        return;
    }
    if (m_applying || m_pending.isEmpty()) return;
    QString summary;
    m_steps.clear();
    QList<Key> keys = m_pending.keys();
    std::sort(keys.begin(), keys.end());
    for (const Key &key : keys) {
        const QStringList next = m_pending.value(key);
        for (const QString &entry : next) {
            if (entry.contains(QLatin1Char(','))) {
                m_status->setText(tr("Nothing was sent: the entry \"%1\" contains a comma, which the "
                                     "Director cannot store in a catalog ACL.").arg(entry));
                return;
            }
        }
        QString old;
        for (const BConsoleAcl &console : m_consoles) {
            if (console.name == key.first) old = listText(BAclMatrix::catalogList(console, key.second));
        }
        const QString command = BAclMatrix::changeCommand(key.first, key.second, next);
        summary += tr("Console %1, %2:").arg(key.first, key.second) + QLatin1Char('\n')
                   + tr("  now: %1").arg(old) + QLatin1Char('\n')
                   + tr("  new: %1").arg(listText(next)) + QLatin1Char('\n')
                   + QStringLiteral("  ") + command + QStringLiteral("\n\n");
        m_steps.append({command, tr("%1 of %2").arg(key.second, key.first)});
    }
    const Confirm confirm = m_confirm ? m_confirm : confirmDialog;
    if (!confirm(this, summary)) {
        m_steps.clear();
        m_status->setText(tr("Nothing was sent: the changes were not confirmed."));
        return;
    }
    m_applying = true;
    m_done.clear();
    updateButtons();
    sendNext();
}

void BAclMatrixDialog::sendNext()
{
    if (m_steps.isEmpty()) {
        finishApply(tr("Changed: %1.").arg(m_done.join(QStringLiteral(", "))), true);
        return;
    }
    m_status->setText(tr("Changing %1...").arg(m_steps.first().second));
    emit changeRequested(m_steps.first().first);
}

void BAclMatrixDialog::setChangeReply(const QString &reply)
{
    if (!m_applying || m_steps.isEmpty()) return;
    QString message;
    if (!BAclMatrix::changeSucceeded(reply, &message)) {
        setChangeFailure(message.isEmpty() ? tr("refused") : message);
        return;
    }
    m_done.append(m_steps.takeFirst().second);
    sendNext();
}

void BAclMatrixDialog::setChangeFailure(const QString &message)
{
    if (!m_applying || m_steps.isEmpty()) return;
    const QString what = m_steps.first().second;
    const QString before = m_done.isEmpty() ? tr("nothing") : m_done.join(QStringLiteral(", "));
    finishApply(tr("Changing %1 failed: %2. Changed before: %3. Nothing after it was sent.")
                    .arg(what, message, before), true);
}

void BAclMatrixDialog::finishApply(const QString &message, bool reload)
{
    m_applying = false;
    m_steps.clear();
    m_pending.clear();
    updateButtons();
    m_status->setText(message);
    if (reload) {
        m_afterRefresh = message;
        emit refreshRequested();
    }
}

void BAclMatrixDialog::rebuild()
{
    m_table->clear();
    const QList<BConsoleAcl> &cols = columns();
    m_table->setColumnCount(int(cols.size()));
    for (int c = 0; c < cols.size(); ++c) {
        const BConsoleAcl &column = cols[c];
        auto *header = new QTableWidgetItem(column.locked ? column.name + QStringLiteral(" —")
                                            : column.proposed ? tr("%1 (new)").arg(column.name)
                                                              : column.name);
        header->setToolTip(m_view == View::Profiles ? profileSummary(column) : BAclMatrix::consoleSummary(column));
        m_table->setHorizontalHeaderItem(c, header);
    }

    int rowCount = 0;
    QList<QList<BAclMatrix::Row>> perType;
    const QList<BAclMatrix::Type> types = BAclMatrix::types();
    for (const BAclMatrix::Type &type : types) {
        QStringList known = m_known.value(type.key);
        if (m_view == View::Profiles) {
            // The same resources as in the console view
            for (const BAclMatrix::Row &row : BAclMatrix::rows(m_consoles, type.key, known))
                known.append(row.resource);
        }
        perType.append(BAclMatrix::rows(cols, type.key, known));
        rowCount += 1 + int(perType.last().size());
    }
    m_table->setRowCount(rowCount);

    QFont bold = m_table->font();
    bold.setBold(true);
    int r = 0;
    for (int t = 0; t < types.size(); ++t) {
        const BAclMatrix::Type &type = types[t];
        auto *title = new QTableWidgetItem(tr("%1 (%2)").arg(type.title).arg(perType[t].size()));
        title->setFont(bold);
        title->setData(TypeRole, type.key);
        title->setData(GroupRole, true);
        m_table->setVerticalHeaderItem(r, title);
        if (m_table->columnCount() > 1)
            m_table->setSpan(r, 0, 1, m_table->columnCount());
        ++r;
        for (const BAclMatrix::Row &row : perType[t]) {
            auto *name = new QTableWidgetItem(QStringLiteral("  ") + row.resource);
            name->setData(TypeRole, type.key);
            name->setData(GroupRole, false);
            name->setData(DiffersRole, row.differs);
            name->setToolTip(row.resource);
            m_table->setVerticalHeaderItem(r, name);
            for (int c = 0; c < row.cells.size(); ++c) {
                auto *cell = new QTableWidgetItem;
                cell->setTextAlignment(Qt::AlignCenter);
                cell->setData(CellTypeRole, type.key);
                cell->setData(ResourceRole, row.resource);
                cell->setData(OriginalRole, int(row.cells[c]));
                cell->setData(ReasonRole, row.reasons[c]);
                m_table->setItem(r, c, cell);
                paintCell(cell);
            }
            ++r;
        }
    }
    applyFilter();
    updateButtons();
}

void BAclMatrixDialog::applyFilter()
{
    const QString text = m_filter->text().trimmed();
    const bool onlyDiffering = m_differing->isChecked();
    int groupRow = -1;
    bool groupHasVisible = false;
    auto closeGroup = [&]() {
        if (groupRow >= 0) m_table->setRowHidden(groupRow, !groupHasVisible);
    };
    for (int r = 0; r < m_table->rowCount(); ++r) {
        QTableWidgetItem *header = m_table->verticalHeaderItem(r);
        if (!header) continue;
        if (header->data(GroupRole).toBool()) {
            closeGroup();
            groupRow = r;
            groupHasVisible = false;
            continue;
        }
        const bool visible = (text.isEmpty() || header->text().contains(text, Qt::CaseInsensitive))
                             && (!onlyDiffering || header->data(DiffersRole).toBool());
        m_table->setRowHidden(r, !visible);
        groupHasVisible = groupHasVisible || visible;
    }
    closeGroup();
}
