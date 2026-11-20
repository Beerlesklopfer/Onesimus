#include "director/baclmatrixdialog.h"

#include "buiscale.h"

#include <QCheckBox>
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
    layout->addWidget(m_table, 1);

    auto *legend = new QLabel(this);
    legend->setWordWrap(true);
    legend->setText(tr("✓ allowed   ✗ denied by an entry with \"!\"   · not allowed   "
                       "⊘ allowed by the file ACL, narrowed by the catalog ACL   "
                       "— console locked. A cell's tool tip says why; a console's tool tip "
                       "has its profiles, its catalog ACL and its Where and Plugin Options ACLs."));
    layout->addWidget(legend);

    m_status = new QLabel(this);
    m_status->setWordWrap(true);
    m_status->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(m_status);

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
    rebuild();
    bool effective = false;
    for (const BConsoleAcl &console : m_consoles) {
        if (!console.effective.isEmpty()) effective = true;
    }
    m_status->setText(effective
        ? tr("%n console(s). Allowed is what the Director decided (file ACL and catalog ACL).", "",
             int(m_consoles.size()))
        : tr("%n console(s). This Director reports the file ACL only; the GUI decided the cells "
             "from its entries.", "", int(m_consoles.size())));
}

void BAclMatrixDialog::setFailure(const QString &message)
{
    m_status->setText(tr("The ACLs could not be read: %1").arg(message));
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
                auto *cell = new QTableWidgetItem(symbol(row.cells[c]));
                cell->setTextAlignment(Qt::AlignCenter);
                cell->setForeground(colour(row.cells[c], palette()));
                cell->setToolTip(QStringLiteral("%1 — %2\n%3")
                                     .arg(m_consoles[c].name, row.resource, row.reasons[c]));
                cell->setData(Qt::AccessibleTextRole, row.reasons[c]);
                m_table->setItem(r, c, cell);
            }
            ++r;
        }
    }
    applyFilter();
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
