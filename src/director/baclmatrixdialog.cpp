#include "director/baclmatrixdialog.h"

#include "buiscale.h"

#include <QCheckBox>
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
    auto *how = new QLabel(this);
    how->setWordWrap(true);
    how->setText(tr("A double click (or Space) on ✓ denies it through the catalog ACL, on ⊘ allows "
                    "it again. Changed cells are underlined until they are applied. What the file ACL "
                    "denies is changed in the editor."));
    layout->addWidget(how);

    auto *bottom = new QHBoxLayout;
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
    updateButtons();

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
    if (column < 0 || column >= m_consoles.size()) return;
    const BConsoleAcl &console = m_consoles[column];
    const QString type = item->data(CellTypeRole).toString();
    const QString resource = item->data(ResourceRole).toString();
    const auto original = BAclMatrix::Cell(item->data(OriginalRole).toInt());
    QString tip = QStringLiteral("%1 — %2\n%3").arg(console.name, resource, item->data(ReasonRole).toString());

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
    if (m_applying || !item || column >= m_consoles.size() || !item->data(CellTypeRole).isValid())
        return false;
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
    m_apply->setEnabled(!m_applying && !m_pending.isEmpty());
    m_discard->setEnabled(!m_applying && !m_pending.isEmpty());
    m_refresh->setEnabled(!m_applying);
}

void BAclMatrixDialog::discard()
{
    m_pending.clear();
    rebuild();
    m_status->setText(tr("The changes were discarded."));
}

void BAclMatrixDialog::apply()
{
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
    m_table->setColumnCount(int(m_consoles.size()));
    for (int c = 0; c < m_consoles.size(); ++c) {
        const BConsoleAcl &console = m_consoles[c];
        auto *header = new QTableWidgetItem(console.locked ? console.name + QStringLiteral(" —")
                                                           : console.name);
        header->setToolTip(BAclMatrix::consoleSummary(console));
        m_table->setHorizontalHeaderItem(c, header);
    }

    int rowCount = 0;
    QList<QList<BAclMatrix::Row>> perType;
    const QList<BAclMatrix::Type> types = BAclMatrix::types();
    for (const BAclMatrix::Type &type : types) {
        perType.append(BAclMatrix::rows(m_consoles, type.key, m_known.value(type.key)));
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
