/**
 * @file bjobdefinitionswidget.cpp
 * @brief The Job Definitions tab: Job and JobDefs resources, their
 *        directives and where they are set, and writing them
 */

#include "jobs/bjobdefinitionswidget.h"

#include "bheaderstate.h"
#include "bstatuschip.h"
#include "bstatusdelegate.h"
#include "buiscale.h"
#include "config/bresourcereviewdialog.h"
#include "config/bsettings.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIntValidator>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QSortFilterProxyModel>
#include <QSplitter>
#include <QTableView>
#include <QTableWidget>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>

namespace {

using Kind = BJobDefinitions::Kind;

bool isNo(const QString &text)
{
    const QString value = text.trimmed().toLower();
    return value == QLatin1String("no") || value == QLatin1String("false") || value == QLatin1String("0");
}

/// The table filtered by kind and by a text in any column
class DefinitionsFilter : public QSortFilterProxyModel
{
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

    void setKindFilter(int kind)
    {
        m_kind = kind;
        invalidateFilter();
    }
    void setText(const QString &text)
    {
        m_text = text.trimmed();
        invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int row, const QModelIndex &parent) const override
    {
        const QAbstractItemModel *model = sourceModel();
        if (m_kind >= 0
            && model->index(row, 0, parent).data(BJobDefinitionsTableModel::KindRole).toInt() != m_kind)
            return false;
        if (m_text.isEmpty())
            return true;
        for (int column = 0; column < model->columnCount(parent); ++column) {
            if (model->index(row, column, parent).data().toString().contains(m_text, Qt::CaseInsensitive))
                return true;
        }
        return false;
    }

private:
    int m_kind = -1;
    QString m_text;
};

} // namespace

// ============================================================================
// BJobDefinitionsTableModel
// ============================================================================

BJobDefinitionsTableModel::BJobDefinitionsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QString BJobDefinitionsTableModel::keyOf(int column)
{
    switch (column) {
    case ColJobDefs: return QStringLiteral("jobdefs");
    case ColClient: return QStringLiteral("client");
    case ColFileSet: return QStringLiteral("fileset");
    case ColSchedule: return QStringLiteral("schedule");
    case ColPool: return QStringLiteral("pool");
    case ColStorage: return QStringLiteral("storage");
    case ColLevel: return QStringLiteral("level");
    case ColPriority: return QStringLiteral("priority");
    case ColEnabled: return QStringLiteral("enabled");
    default: break;
    }
    return QString();
}

void BJobDefinitionsTableModel::setDefinitions(const BJobDefinitions *definitions)
{
    beginResetModel();
    m_rows.clear();
    if (definitions) {
        for (const BJobDefinitions::Resource &resource : definitions->resources()) {
            Row row;
            row.kind = resource.kind;
            row.name = resource.name;
            for (int column = ColJobDefs; column < ColumnCount; ++column)
                row.values.insert(column, definitions->value(resource.kind, resource.name, keyOf(column)));
            m_rows.append(row);
        }
    }
    endResetModel();
}

int BJobDefinitionsTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : int(m_rows.size());
}

int BJobDefinitionsTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : ColumnCount;
}

int BJobDefinitionsTableModel::rowOf(Kind kind, const QString &name) const
{
    for (int i = 0; i < m_rows.size(); ++i) {
        if (m_rows[i].kind == kind && m_rows[i].name == name)
            return i;
    }
    return -1;
}

QVariant BJobDefinitionsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    switch (section) {
    case ColName: return BJobDefinitionsWidget::tr("Name");
    case ColKind: return BJobDefinitionsWidget::tr("Resource");
    case ColJobDefs: return BJobDefinitionsWidget::tr("JobDefs");
    case ColClient: return BJobDefinitionsWidget::tr("Client");
    case ColFileSet: return BJobDefinitionsWidget::tr("FileSet");
    case ColSchedule: return BJobDefinitionsWidget::tr("Schedule");
    case ColPool: return BJobDefinitionsWidget::tr("Pool");
    case ColStorage: return BJobDefinitionsWidget::tr("Storage");
    case ColLevel: return BJobDefinitionsWidget::tr("Level");
    case ColPriority: return BJobDefinitionsWidget::tr("Priority");
    case ColEnabled: return BJobDefinitionsWidget::tr("Enabled");
    default: break;
    }
    return QVariant();
}

QVariant BJobDefinitionsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_rows.size())
        return QVariant();
    const Row &row = m_rows[index.row()];
    const int column = index.column();

    if (role == KindRole)
        return int(row.kind);
    if (role == NameRole)
        return row.name;

    if (column == ColName || column == ColKind) {
        const QString text = column == ColName ? row.name
                           : (row.kind == Kind::Job ? QStringLiteral("Job") : QStringLiteral("JobDefs"));
        if (role == Qt::DisplayRole || role == Qt::AccessibleTextRole)
            return text;
        if (role == Qt::UserRole)
            return text.toLower();
        return QVariant();
    }

    const BJobDefinitions::Value value = row.values.value(column);
    if (column == ColEnabled) {
        const bool enabled = !value.set || !isNo(value.text);
        switch (role) {
        case Qt::DisplayRole:
            return enabled ? BJobDefinitionsWidget::tr("Enabled") : BJobDefinitionsWidget::tr("Disabled");
        case Qt::AccessibleTextRole:
            return enabled ? BJobDefinitionsWidget::tr("Enabled") : BJobDefinitionsWidget::tr("Disabled, the "
                                                                                              "scheduler does not "
                                                                                              "start it");
        case BStatus::ToneRole:
            return int(enabled ? BStatus::Tone::Info : BStatus::Tone::Neutral);
        case BStatus::CodeRole:
            return enabled ? QStringLiteral("E") : QStringLiteral("D");
        case Qt::UserRole:
            return enabled ? 1 : 0;
        default:
            break;
        }
    }

    switch (role) {
    case Qt::DisplayRole:
    case Qt::AccessibleTextRole:
        if (column == ColEnabled)
            break;
        return value.text;
    case Qt::UserRole:
        if (column == ColPriority)
            return value.set ? value.text.toInt() : 10;
        return value.text.toLower();
    case Qt::FontRole:
        if (value.set && !value.here) {
            QFont font;
            font.setItalic(true);
            return font;
        }
        break;
    case Qt::ToolTipRole:
        if (value.here)
            return BJobDefinitionsWidget::tr("Set in this %1").arg(row.kind == Kind::Job ? QStringLiteral("Job")
                                                                                         : QStringLiteral("JobDefs"));
        if (value.set)
            return BJobDefinitionsWidget::tr("Inherited from JobDefs %1").arg(value.from);
        if (value.uncertain)
            return BJobDefinitionsWidget::tr("Not known: a JobDefs of the chain was not reported");
        return BJobDefinitionsWidget::tr("Not set: the default of the Director applies");
    default:
        break;
    }
    return QVariant();
}

// ============================================================================
// The dialog for a new resource or a copy
// ============================================================================

namespace {

class NewResourceDialog : public QDialog
{
public:
    NewResourceDialog(Kind kind, const QString &copyOf, const QMap<QString, QStringList> &names,
                      const QStringList &taken, QWidget *parent)
        : QDialog(parent)
        , m_kind(kind)
        , m_copy(!copyOf.isEmpty())
        , m_taken(taken)
    {
        const QString word = kind == Kind::Job ? QStringLiteral("Job") : QStringLiteral("JobDefs");
        setWindowTitle(m_copy ? BJobDefinitionsWidget::tr("Copy %1 %2").arg(word, copyOf)
                              : BJobDefinitionsWidget::tr("New %1").arg(word));
        auto *layout = new QVBoxLayout(this);
        auto *form = new QFormLayout();
        m_name = new QLineEdit(this);
        m_name->setText(m_copy ? copyOf + QStringLiteral("-copy") : QString());
        form->addRow(BJobDefinitionsWidget::tr("&Name:"), m_name);

        if (!m_copy) {
            auto addCombo = [&](const QString &label, const QString &key, const QString &directive,
                                const QStringList &choices) {
                auto *combo = new QComboBox(this);
                combo->setEditable(true);
                combo->addItem(QString());
                combo->addItems(choices);
                combo->setCurrentIndex(0);
                form->addRow(label, combo);
                m_combos.append({directive, combo});
                Q_UNUSED(key);
            };
            if (kind == Kind::Job)
                addCombo(BJobDefinitionsWidget::tr("&JobDefs:"), QStringLiteral("jobdefs"), QStringLiteral("JobDefs"),
                         names.value(QStringLiteral("JobDefs")));
            addCombo(BJobDefinitionsWidget::tr("&Type:"), QStringLiteral("type"), QStringLiteral("Type"),
                     {QStringLiteral("Backup"), QStringLiteral("Restore"), QStringLiteral("Verify"),
                      QStringLiteral("Admin"), QStringLiteral("Copy"), QStringLiteral("Migrate"),
                      QStringLiteral("Consolidate")});
            addCombo(BJobDefinitionsWidget::tr("&Level:"), QStringLiteral("level"), QStringLiteral("Level"),
                     {QStringLiteral("Full"), QStringLiteral("Incremental"), QStringLiteral("Differential"),
                      QStringLiteral("VirtualFull")});
            addCombo(BJobDefinitionsWidget::tr("&Client:"), QStringLiteral("client"), QStringLiteral("Client"),
                     names.value(QStringLiteral("Client")));
            addCombo(BJobDefinitionsWidget::tr("&FileSet:"), QStringLiteral("fileset"), QStringLiteral("FileSet"),
                     names.value(QStringLiteral("FileSet")));
            addCombo(BJobDefinitionsWidget::tr("&Schedule:"), QStringLiteral("schedule"), QStringLiteral("Schedule"),
                     names.value(QStringLiteral("Schedule")));
            addCombo(BJobDefinitionsWidget::tr("St&orage:"), QStringLiteral("storage"), QStringLiteral("Storage"),
                     names.value(QStringLiteral("Storage")));
            addCombo(BJobDefinitionsWidget::tr("&Pool:"), QStringLiteral("pool"), QStringLiteral("Pool"),
                     names.value(QStringLiteral("Pool")));
            addCombo(BJobDefinitionsWidget::tr("&Messages:"), QStringLiteral("messages"), QStringLiteral("Messages"),
                     names.value(QStringLiteral("Messages")));
        }
        layout->addLayout(form);

        if (kind == Kind::Job) {
            m_disabled = new QCheckBox(BJobDefinitionsWidget::tr("Create it &disabled (Enabled = no)"), this);
            m_disabled->setChecked(true);
            m_disabled->setToolTip(BJobDefinitionsWidget::tr("A new job with a schedule is started by the "
                                                             "scheduler of the Director. Created disabled, it "
                                                             "does not run until you enable it."));
            layout->addWidget(m_disabled);
        }
        if (m_copy) {
            layout->addWidget(BResourceReviewDialog::wrapped(
                BJobDefinitionsWidget::tr("The copy is the text of %1 with the new name, in a file of its own. "
                                          "Everything else stays as it is.").arg(copyOf), this));
        } else {
            layout->addWidget(BResourceReviewDialog::wrapped(
                BJobDefinitionsWidget::tr("Fields left empty are not written; a job takes them from its "
                                          "JobDefs. The Director checks the new resource before anything is "
                                          "written."), this));
        }
        m_error = new QLabel(this);
        m_error->setWordWrap(true);
        m_error->setTextFormat(Qt::PlainText);
        layout->addWidget(m_error);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        buttons->button(QDialogButtonBox::Ok)->setText(BJobDefinitionsWidget::tr("&Check and review..."));
        connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
            const QString error = check();
            m_error->setText(error);
            if (error.isEmpty())
                accept();
        });
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttons);
        resize(BUiScale::px(460), sizeHint().height());
    }

    QString check() const
    {
        static const QRegularExpression badRx(QStringLiteral("[\"\\\\/\\x00-\\x1f]"));
        const QString name = m_name->text().trimmed();
        if (name.isEmpty())
            return BJobDefinitionsWidget::tr("A name is needed.");
        if (badRx.match(name).hasMatch() || name.contains(QLatin1String("..")))
            return BJobDefinitionsWidget::tr("The name must not contain quotes, slashes, backslashes or \"..\".");
        if (m_taken.contains(name))
            return BJobDefinitionsWidget::tr("A resource of this name exists already.");
        return QString();
    }

    BJobDefinitionsWidget::NewResource result() const
    {
        BJobDefinitionsWidget::NewResource out;
        out.name = m_name->text().trimmed();
        for (const auto &entry : m_combos) {
            const QString value = entry.second->currentText().trimmed();
            if (!value.isEmpty())
                out.directives.append(BResourceText::Change::set(entry.first, value));
        }
        if (m_disabled && m_disabled->isChecked())
            out.directives.append(BResourceText::Change::set(QStringLiteral("Enabled"), QStringLiteral("no")));
        return out;
    }

private:
    Kind m_kind;
    bool m_copy;
    QStringList m_taken;
    QLineEdit *m_name = nullptr;
    QCheckBox *m_disabled = nullptr;
    QLabel *m_error = nullptr;
    QList<std::pair<QString, QComboBox *>> m_combos;
};

} // namespace

// ============================================================================
// BJobDefinitionsWidget: construction
// ============================================================================

BConfig::ResourceType BJobDefinitionsWidget::typeOf(Kind kind)
{
    return kind == Kind::Job ? BConfig::ResourceType::Job : BConfig::ResourceType::JobDefs;
}

QString BJobDefinitionsWidget::kindWord(Kind kind)
{
    return kind == Kind::Job ? QStringLiteral("Job") : QStringLiteral("JobDefs");
}

BJobDefinitionsWidget::BJobDefinitionsWidget(QWidget *parent)
    : QWidget(parent)
{
    m_saveDialogs = new BResourceSaveDialogs(this);
    m_persistence = new BResourcePersistence(nullptr, m_saveDialogs, this);
    connect(m_persistence, &BResourcePersistence::finished, this, &BJobDefinitionsWidget::onPersistenceFinished);
    connect(m_persistence, &BResourcePersistence::reloadRequested, this, &BJobDefinitionsWidget::onReloadRequested);
    connect(m_persistence, &BResourcePersistence::busyChanged, this, [this]() { updateButtons(); });
    setupUi();
    clearDetail();
}

BJobDefinitionsWidget::~BJobDefinitionsWidget()
{
    delete m_saveDialogs;
}

void BJobDefinitionsWidget::setupUi()
{
    auto *layout = new QVBoxLayout(this);

    // --- Tool row ---
    auto *tools = new QHBoxLayout();
    m_refreshButton = new QPushButton(QIcon(QStringLiteral(":/icons/icons/refresh.svg")), tr("&Refresh"), this);
    m_refreshButton->setToolTip(tr("Reads the Job and JobDefs resources from the Director again"));
    m_newJobButton = new QPushButton(QIcon(QStringLiteral(":/icons/icons/add.svg")), tr("New &Job..."), this);
    m_newJobDefsButton = new QPushButton(QIcon(QStringLiteral(":/icons/icons/add.svg")), tr("New Job&Defs..."), this);
    m_copyButton = new QPushButton(QIcon(QStringLiteral(":/icons/icons/copy.svg")), tr("&Copy..."), this);
    m_deleteButton = new QPushButton(QIcon(QStringLiteral(":/icons/icons/delete.svg")), tr("De&lete..."), this);
    m_filter = new QLineEdit(this);
    m_filter->setPlaceholderText(tr("Filter"));
    m_filter->setClearButtonEnabled(true);
    m_filter->setAccessibleName(tr("Filter the resources"));
    m_kindFilter = new QComboBox(this);
    m_kindFilter->addItem(tr("Jobs and JobDefs"), -1);
    m_kindFilter->addItem(tr("Jobs"), int(Kind::Job));
    m_kindFilter->addItem(tr("JobDefs"), int(Kind::JobDefs));
    m_kindFilter->setAccessibleName(tr("Show"));
    tools->addWidget(m_refreshButton);
    tools->addSpacing(BUiScale::px(12));
    tools->addWidget(m_newJobButton);
    tools->addWidget(m_newJobDefsButton);
    tools->addWidget(m_copyButton);
    tools->addWidget(m_deleteButton);
    tools->addStretch(1);
    tools->addWidget(m_kindFilter);
    tools->addWidget(m_filter);
    layout->addLayout(tools);

    // --- Table ---
    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_model = new BJobDefinitionsTableModel(this);
    auto *proxy = new DefinitionsFilter(this);
    m_proxy = proxy;
    m_proxy->setSourceModel(m_model);
    m_proxy->setSortRole(Qt::UserRole);
    m_table = new QTableView(m_splitter);
    m_table->setModel(m_proxy);
    m_table->setAccessibleName(tr("Job and JobDefs resources"));
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setSortingEnabled(true);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setItemDelegateForColumn(BJobDefinitionsTableModel::ColEnabled, new BStatusDelegate(m_table));
    QHeaderView *header = m_table->horizontalHeader();
    header->setSectionsMovable(true);
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setStretchLastSection(true);
    header->setMinimumSectionSize(BUiScale::px(40));
    m_table->setColumnWidth(BJobDefinitionsTableModel::ColName, BUiScale::px(180));
    m_table->setColumnWidth(BJobDefinitionsTableModel::ColKind, BUiScale::px(80));
    m_table->setColumnWidth(BJobDefinitionsTableModel::ColPriority, BUiScale::px(60));
    m_table->setColumnWidth(BJobDefinitionsTableModel::ColEnabled, BUiScale::px(100));
    m_table->sortByColumn(BJobDefinitionsTableModel::ColName, Qt::AscendingOrder);

    // Order, widths, hidden columns and the sort column, in BSettings
    m_headerTimer = new QTimer(this);
    m_headerTimer->setSingleShot(true);
    m_headerTimer->setInterval(500);
    connect(m_headerTimer, &QTimer::timeout, this, &BJobDefinitionsWidget::saveHeader);
    connect(header, &QHeaderView::sectionResized, m_headerTimer, qOverload<>(&QTimer::start));
    connect(header, &QHeaderView::sectionMoved, m_headerTimer, qOverload<>(&QTimer::start));
    connect(header, &QHeaderView::sortIndicatorChanged, m_headerTimer, qOverload<>(&QTimer::start));
    if (BHeaderState::restore(header, BSettings::instance().jobDefinitionsHeaderState(),
                              BSettings::instance().jobDefinitionsHeaderScale())) {
        m_table->sortByColumn(header->sortIndicatorSection(), header->sortIndicatorOrder());
    }

    // --- Detail ---
    auto *detail = new QWidget(m_splitter);
    auto *detailLayout = new QVBoxLayout(detail);
    detailLayout->setContentsMargins(BUiScale::px(6), 0, 0, 0);
    m_title = new QLabel(detail);
    m_title->setTextFormat(Qt::PlainText);
    BUiScale::bind(m_title, BUiScale::Large, BUiScale::Bold);
    m_subtitle = BResourceReviewDialog::wrapped(QString(), detail);
    m_access = BResourceReviewDialog::wrapped(QString(), detail);
    m_access->setObjectName(QStringLiteral("jobDefinitionsAccess"));
    detailLayout->addWidget(m_title);
    detailLayout->addWidget(m_subtitle);
    detailLayout->addWidget(m_access);

    m_tabs = new QTabWidget(detail);
    m_directives = new QTableWidget(0, 3, m_tabs);
    m_directives->setHorizontalHeaderLabels({tr("Directive"), tr("Value"), tr("Set in")});
    m_directives->setAccessibleName(tr("Directives and where they are set"));
    m_directives->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_directives->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_directives->verticalHeader()->setVisible(false);
    m_directives->horizontalHeader()->setStretchLastSection(true);
    m_directives->setColumnWidth(0, BUiScale::px(190));
    m_directives->setColumnWidth(1, BUiScale::px(220));
    m_tabs->addTab(m_directives, tr("Directives"));

    auto *scroll = new QScrollArea(m_tabs);
    scroll->setWidgetResizable(true);
    m_formPage = new QWidget(scroll);
    setupFields(m_formPage);
    scroll->setWidget(m_formPage);
    m_tabs->addTab(scroll, tr("Edit"));

    auto *textPage = new QWidget(m_tabs);
    auto *textLayout = new QVBoxLayout(textPage);
    m_text = new QPlainTextEdit(textPage);
    m_text->setReadOnly(true);
    m_text->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_text->setAccessibleName(tr("Resource text"));
    BUiScale::bind(m_text, BUiScale::Normal, BUiScale::Monospace);
    m_textEditing = new QCheckBox(tr("Edit the &text directly (for directives the form does not have)"), textPage);
    m_textEditing->setToolTip(tr("The text is sent as you write it, after the Director has checked it and "
                                 "you have seen the difference. The fields of the form are not used then."));
    textLayout->addWidget(m_text, 1);
    textLayout->addWidget(m_textEditing);
    m_tabs->addTab(textPage, tr("Text"));
    detailLayout->addWidget(m_tabs, 1);

    auto *saveRow = new QHBoxLayout();
    m_revertButton = new QPushButton(tr("Re&vert"), detail);
    m_revertButton->setToolTip(tr("Drops the changes made here"));
    m_saveButton = new QPushButton(QIcon(QStringLiteral(":/icons/icons/save.svg")), tr("&Save..."), detail);
    m_saveButton->setToolTip(tr("Lets the Director check the change, shows it, and writes it after you confirm"));
    saveRow->addStretch(1);
    saveRow->addWidget(m_revertButton);
    saveRow->addWidget(m_saveButton);
    detailLayout->addLayout(saveRow);

    m_splitter->addWidget(m_table);
    m_splitter->addWidget(detail);
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 2);
    const QByteArray splitterState = BSettings::instance().jobDefinitionsSplitterState();
    if (!splitterState.isEmpty())
        m_splitter->restoreState(splitterState);
    connect(m_splitter, &QSplitter::splitterMoved, this, [this]() {
        BSettings::instance().setJobDefinitionsSplitterState(m_splitter->saveState());
    });
    layout->addWidget(m_splitter, 1);

    // --- Connections ---
    connect(m_refreshButton, &QPushButton::clicked, this, &BJobDefinitionsWidget::refreshRequested);
    connect(m_newJobButton, &QPushButton::clicked, this, [this]() { newResource(Kind::Job); });
    connect(m_newJobDefsButton, &QPushButton::clicked, this, [this]() { newResource(Kind::JobDefs); });
    connect(m_copyButton, &QPushButton::clicked, this, &BJobDefinitionsWidget::copyCurrent);
    connect(m_deleteButton, &QPushButton::clicked, this, &BJobDefinitionsWidget::deleteCurrent);
    connect(m_saveButton, &QPushButton::clicked, this, &BJobDefinitionsWidget::save);
    connect(m_revertButton, &QPushButton::clicked, this, &BJobDefinitionsWidget::revert);
    connect(m_filter, &QLineEdit::textChanged, this, [proxy](const QString &text) { proxy->setText(text); });
    connect(m_kindFilter, &QComboBox::currentIndexChanged, this, [this, proxy]() {
        proxy->setKindFilter(m_kindFilter->currentData().toInt());
    });
    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &BJobDefinitionsWidget::onSelectionChanged);
    connect(m_textEditing, &QCheckBox::toggled, this, [this](bool on) {
        if (m_filling)
            return;
        m_text->setReadOnly(!on);
        if (!on && m_raw) {
            // Back to the form: the text is the Director's again
            m_filling = true;
            QString shown = m_raw->text;
            shown.replace(QLatin1String("\r\n"), QLatin1String("\n"));
            m_text->setPlainText(shown);
            m_filling = false;
        }
        updateAccess();
        updateButtons();
    });
    connect(m_text, &QPlainTextEdit::textChanged, this, [this]() {
        if (!m_filling)
            updateButtons();
    });
}

void BJobDefinitionsWidget::setupFields(QWidget *page)
{
    auto *form = new QFormLayout(page);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    auto add = [&](const QString &key, const QString &directive, const QString &label, const QString &names,
                   const QStringList &fixed = QStringList(), bool boolean = false, bool number = false) {
        Field field;
        field.key = key;
        field.directive = directive;
        field.names = names;
        field.fixed = fixed;
        field.boolean = boolean;
        field.number = number;
        auto *cell = new QWidget(page);
        auto *cellLayout = new QVBoxLayout(cell);
        cellLayout->setContentsMargins(0, 0, 0, 0);
        cellLayout->setSpacing(BUiScale::px(1));
        if (number || (names.isEmpty() && fixed.isEmpty() && !boolean)) {
            field.line = new QLineEdit(cell);
            if (number)
                field.line->setValidator(new QIntValidator(1, 1000000, field.line));
            field.line->setAccessibleName(directive);
            cellLayout->addWidget(field.line);
            connect(field.line, &QLineEdit::textChanged, this, [this]() {
                if (!m_filling)
                    updateButtons();
            });
        } else {
            field.combo = new QComboBox(cell);
            field.combo->setEditable(!boolean);
            field.combo->setAccessibleName(directive);
            field.combo->setInsertPolicy(QComboBox::NoInsert);
            if (boolean) {
                field.combo->addItem(tr("(not set here)"), QString());
                field.combo->addItem(tr("yes"), QStringLiteral("yes"));
                field.combo->addItem(tr("no"), QStringLiteral("no"));
            }
            cellLayout->addWidget(field.combo);
            connect(field.combo, &QComboBox::currentTextChanged, this, [this]() {
                if (!m_filling)
                    updateButtons();
            });
        }
        field.note = new QLabel(cell);
        field.note->setTextFormat(Qt::PlainText);
        field.note->setWordWrap(true);
        field.note->setEnabled(false);      // drawn in the disabled text colour of the theme
        BUiScale::bind(field.note, BUiScale::Small);
        cellLayout->addWidget(field.note);
        form->addRow(label, cell);
        m_fields.append(field);
    };
    add(QStringLiteral("jobdefs"), QStringLiteral("JobDefs"), tr("JobDefs:"), QStringLiteral("JobDefs"));
    add(QStringLiteral("type"), QStringLiteral("Type"), tr("Type:"), QString(),
        {QStringLiteral("Backup"), QStringLiteral("Restore"), QStringLiteral("Verify"), QStringLiteral("Admin"),
         QStringLiteral("Copy"), QStringLiteral("Migrate"), QStringLiteral("Consolidate")});
    add(QStringLiteral("level"), QStringLiteral("Level"), tr("Level:"), QString(),
        {QStringLiteral("Full"), QStringLiteral("Incremental"), QStringLiteral("Differential"),
         QStringLiteral("VirtualFull"), QStringLiteral("Base"), QStringLiteral("InitCatalog"),
         QStringLiteral("Catalog"), QStringLiteral("VolumeToCatalog"), QStringLiteral("DiskToCatalog")});
    add(QStringLiteral("client"), QStringLiteral("Client"), tr("Client:"), QStringLiteral("Client"));
    add(QStringLiteral("fileset"), QStringLiteral("FileSet"), tr("FileSet:"), QStringLiteral("FileSet"));
    add(QStringLiteral("schedule"), QStringLiteral("Schedule"), tr("Schedule:"), QStringLiteral("Schedule"));
    add(QStringLiteral("storage"), QStringLiteral("Storage"), tr("Storage:"), QStringLiteral("Storage"));
    add(QStringLiteral("pool"), QStringLiteral("Pool"), tr("Pool:"), QStringLiteral("Pool"));
    add(QStringLiteral("messages"), QStringLiteral("Messages"), tr("Messages:"), QStringLiteral("Messages"));
    add(QStringLiteral("priority"), QStringLiteral("Priority"), tr("Priority:"), QString(), QStringList(), false, true);
    add(QStringLiteral("enabled"), QStringLiteral("Enabled"), tr("Enabled:"), QString(), QStringList(), true);
    add(QStringLiteral("description"), QStringLiteral("Description"), tr("Description:"), QString());
}

void BJobDefinitionsWidget::saveHeader()
{
    BSettings::instance().setJobDefinitionsHeaderState(m_table->horizontalHeader()->saveState());
    BSettings::instance().setJobDefinitionsHeaderScale(BHeaderState::currentScale());
}

// ============================================================================
// Wiring and data
// ============================================================================

void BJobDefinitionsWidget::setConfigClient(BConfigClient *client)
{
    if (m_client)
        disconnect(m_client, nullptr, this, nullptr);
    m_client = client;
    m_persistence->setClient(client);
    if (client) {
        connect(client, &BConfigClient::capabilitiesChanged, this, [this]() {
            // Another session: what was read with the old rights is not kept
            if (hasSelection() && !isModified())
                showResource(m_kind, m_name);
            else
                updateAccess();
            updateButtons();
        });
    }
    if (hasSelection() && !isModified())
        showResource(m_kind, m_name);
    updateButtons();
}

void BJobDefinitionsWidget::setSaveUi(BResourceSaveUi *ui)
{
    m_persistence->setUi(ui ? ui : m_saveDialogs);
}

void BJobDefinitionsWidget::setResources(const QMap<QString, QJsonObject> &jobs,
                                         const QMap<QString, QJsonObject> &jobDefs)
{
    std::optional<std::pair<Kind, QString>> keep;
    if (m_selectAfterReload)
        keep = m_selectAfterReload;
    else if (hasSelection())
        keep = std::make_pair(m_kind, m_name);

    m_definitions.setResources(jobs, jobDefs);
    const bool modified = isModified();
    {
        const QSignalBlocker blocker(m_table->selectionModel());
        m_model->setDefinitions(&m_definitions);
    }

    if (keep && m_definitions.contains(keep->first, keep->second)) {
        const bool same = hasSelection() && keep->first == m_kind && keep->second == m_name;
        m_selectAfterReload.reset();
        {
            const QSignalBlocker blocker(m_table->selectionModel());
            selectResource(keep->first, keep->second);
        }
        if (!same || !modified)
            showResource(keep->first, keep->second);
        else
            fillDirectives();   // the edits stay; the inherited values are new
    } else {
        // A resource that was just created and is not reported yet is
        // waited for at the next load
        if (!m_selectAfterReload || !hasSelection() || !m_definitions.contains(m_kind, m_name)) {
            clearDetail();
        } else {
            const QSignalBlocker blocker(m_table->selectionModel());
            selectResource(m_kind, m_name);
        }
    }
    updateButtons();
}

bool BJobDefinitionsWidget::selectResource(Kind kind, const QString &name)
{
    const int row = m_model->rowOf(kind, name);
    if (row < 0)
        return false;
    const QModelIndex index = m_proxy->mapFromSource(m_model->index(row, 0));
    if (!index.isValid())
        return false;
    m_table->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    m_table->setCurrentIndex(index);
    m_table->scrollTo(index);
    return true;
}

void BJobDefinitionsWidget::onSelectionChanged()
{
    const QModelIndexList rows = m_table->selectionModel()->selectedRows();
    if (rows.isEmpty())
        return;
    const Kind kind = Kind(rows.first().data(BJobDefinitionsTableModel::KindRole).toInt());
    const QString name = rows.first().data(BJobDefinitionsTableModel::NameRole).toString();
    if (hasSelection() && kind == m_kind && name == m_name)
        return;
    if (isModified() && !confirmDiscard()) {
        const QSignalBlocker blocker(m_table->selectionModel());
        selectResource(m_kind, m_name);
        return;
    }
    showResource(kind, name);
}

bool BJobDefinitionsWidget::confirmDiscard()
{
    const QString resource = QStringLiteral("%1 %2").arg(kindWord(m_kind), m_name);
    if (m_discardUi)
        return m_discardUi(resource);
    return QMessageBox::question(this, tr("Unsaved changes"),
                                 tr("%1 has changes that were not written. Drop them?").arg(resource),
                                 QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel)
        == QMessageBox::Discard;
}

// ============================================================================
// Detail
// ============================================================================

bool BJobDefinitionsWidget::supports(Kind kind, BConfig::Operation operation) const
{
    return m_client && m_client->capabilities().supports(typeOf(kind), operation);
}

bool BJobDefinitionsWidget::supports(BConfig::Operation operation) const
{
    return hasSelection() && supports(m_kind, operation);
}

void BJobDefinitionsWidget::clearDetail()
{
    m_name.clear();
    m_raw.reset();
    m_parsed = BResourceText::Parsed();
    m_rawError.clear();
    m_rawLoading = false;
    m_baseline.clear();
    ++m_readSerial;
    m_title->setText(tr("No resource selected"));
    m_subtitle->setText(tr("Choose a Job or JobDefs in the table to see its directives and where they are set."));
    m_filling = true;
    m_directives->setRowCount(0);
    m_text->clear();
    m_textEditing->setChecked(false);
    m_text->setReadOnly(true);
    for (Field &field : m_fields) {
        if (field.combo && field.boolean) {
            field.combo->setCurrentIndex(0);
        } else if (field.combo) {
            field.combo->setCurrentIndex(-1);
            field.combo->setEditText(QString());
        }
        if (field.line)
            field.line->clear();
        field.note->clear();
    }
    m_filling = false;
    updateAccess();
    updateButtons();
}

void BJobDefinitionsWidget::showResource(Kind kind, const QString &name)
{
    m_kind = kind;
    m_name = name;
    m_raw.reset();
    m_parsed = BResourceText::Parsed();
    m_rawError.clear();
    m_resetAfterWrite = false;

    m_title->setText(QStringLiteral("%1 %2").arg(kindWord(kind), name));
    if (kind == Kind::Job) {
        const QStringList chain = m_definitions.chain(kind, name);
        m_subtitle->setText(chain.isEmpty() ? tr("Uses no JobDefs: every directive is set here or has its default.")
                                            : tr("Inherits from JobDefs %1.").arg(chain.join(QStringLiteral(" → "))));
    } else {
        const QStringList jobs = m_definitions.jobsUsing(name);
        QString text = jobs.isEmpty() ? tr("No job uses this JobDefs.")
                                      : tr("Used by %n job(s): %1.", "", int(jobs.size()))
                                            .arg(jobs.join(QLatin1String(", ")));
        const QStringList chain = m_definitions.chain(kind, name);
        if (!chain.isEmpty())
            text += QLatin1Char(' ') + tr("Inherits from JobDefs %1.").arg(chain.join(QStringLiteral(" → ")));
        m_subtitle->setText(text);
    }

    m_filling = true;
    m_textEditing->setChecked(false);
    m_text->setReadOnly(true);
    m_text->setPlainText(QString());
    m_filling = false;

    fillPickLists();
    fillDirectives();
    fillForm(true);
    loadRaw(false);
    updateAccess();
    updateButtons();
}

void BJobDefinitionsWidget::loadRaw(bool keepEdits)
{
    if (!hasSelection())
        return;
    if (!supports(BConfig::Operation::ReadRaw)) {
        m_rawLoading = false;
        m_rawError.clear();
        updateAccess();
        return;
    }
    m_rawLoading = true;
    const quint64 serial = ++m_readSerial;
    const Kind kind = m_kind;
    const QString name = m_name;
    m_persistence->read(typeOf(kind), name, this,
                        [this, serial, kind, name, keepEdits](const BConfig::Reply &reply,
                                                             const std::optional<BResourcePersistence::Raw> &raw) {
        if (serial != m_readSerial || kind != m_kind || name != m_name)
            return;     // another resource is on display by now
        m_rawLoading = false;
        if (!raw) {
            m_raw.reset();
            m_parsed = BResourceText::Parsed();
            m_rawError = reply.ok() ? tr("the text the Director sent does not match its SHA-256")
                                    : BResourcePersistence::reportOf(reply, BResourceReview::Update, typeOf(kind),
                                                                     name).text;
            updateAccess();
            updateButtons();
            return;
        }
        m_raw = raw;
        m_parsed = BResourceText::parse(raw->text);
        m_rawError.clear();
        m_filling = true;
        if (!keepEdits || !m_textEditing->isChecked()) {
            QString shown = raw->text;
            shown.replace(QLatin1String("\r\n"), QLatin1String("\n"));
            m_text->setPlainText(shown);
        }
        m_filling = false;
        fillDirectives();
        fillForm(!keepEdits);
        updateAccess();
        updateButtons();
    });
}

void BJobDefinitionsWidget::fillDirectives()
{
    if (!hasSelection())
        return;
    const QList<BJobDefinitions::Value> values = m_raw && m_parsed.ok
        ? m_definitions.resolveWithText(m_kind, m_name, m_parsed)
        : m_definitions.values(m_kind, m_name);
    m_directives->setRowCount(0);
    m_directives->setRowCount(int(values.size()));
    const QString here = tr("this %1").arg(kindWord(m_kind));
    for (int i = 0; i < values.size(); ++i) {
        const BJobDefinitions::Value &value = values[i];
        auto *label = new QTableWidgetItem(value.label);
        auto *text = new QTableWidgetItem(value.set ? value.text : QString());
        QString where;
        if (value.here)
            where = here;
        else if (value.set)
            where = tr("JobDefs %1").arg(value.from);
        else if (value.uncertain)
            where = tr("not known (a JobDefs was not reported)");
        else
            where = tr("not set (default)");
        auto *source = new QTableWidgetItem(where);
        if (value.set && !value.here) {
            QFont font = text->font();
            font.setItalic(true);
            text->setFont(font);
            source->setFont(font);
        }
        if (!value.set) {
            label->setForeground(palette().color(QPalette::Disabled, QPalette::Text));
            source->setForeground(palette().color(QPalette::Disabled, QPalette::Text));
        }
        text->setToolTip(value.text);
        m_directives->setItem(i, 0, label);
        m_directives->setItem(i, 1, text);
        m_directives->setItem(i, 2, source);
    }
}

void BJobDefinitionsWidget::fillPickLists()
{
    const QMap<QString, QStringList> names = m_names ? m_names() : QMap<QString, QStringList>();
    m_filling = true;
    for (Field &field : m_fields) {
        if (!field.combo || field.boolean)
            continue;
        const QString current = field.combo->currentText();
        field.combo->clear();
        field.combo->addItem(QString());
        QStringList choices = field.fixed;
        if (!field.names.isEmpty()) {
            choices = names.value(field.names);
            if (field.key == QLatin1String("jobdefs")) {
                choices = m_definitions.jobDefsNames();
                if (m_kind == Kind::JobDefs)
                    choices.removeAll(m_name);
            }
        }
        choices.removeDuplicates();
        choices.sort(Qt::CaseInsensitive);
        if (!field.fixed.isEmpty())
            choices = field.fixed;
        field.combo->addItems(choices);
        field.combo->setEditText(current);
    }
    m_filling = false;
}

void BJobDefinitionsWidget::fillForm(bool values)
{
    m_baseline.clear();
    const bool fromText = m_raw && m_parsed.ok;
    const QList<BJobDefinitions::Value> resolved = fromText
        ? m_definitions.resolveWithText(m_kind, m_name, m_parsed)
        : m_definitions.values(m_kind, m_name);
    QHash<QString, BJobDefinitions::Value> byKey;
    for (const BJobDefinitions::Value &value : resolved)
        byKey.insert(value.key, value);

    m_filling = true;
    for (Field &field : m_fields) {
        const BJobDefinitions::Value value = byKey.value(field.key);
        QString here;
        if (fromText) {
            const int index = m_parsed.find(field.key);
            here = index >= 0 ? m_parsed.directives[index].value : QString();
        } else if (value.here) {
            here = value.text;
        }
        m_baseline.insert(field.key, here);

        QString note;
        if (value.here && !fromText)
            note = tr("Set here (as the Director reports it)");
        else if (!value.here && value.set)
            note = tr("Inherited: %1 (JobDefs %2). Leave empty to keep it.").arg(value.text, value.from);
        else if (!value.set)
            note = value.uncertain ? tr("Not known: a JobDefs of the chain was not reported.")
                                   : tr("Not set: the default of the Director applies.");
        if (fromText && (m_parsed.opaqueKeys.contains(field.key) || m_parsed.find(field.key) == -2))
            note = tr("Written in a way the form cannot change (several times, a list, or with other "
                      "directives on its line). Use the text.");
        field.note->setText(note);
        field.note->setVisible(!note.isEmpty());

        const QString placeholder = (!value.here && value.set) ? value.text : QString();
        if (field.line) {
            field.line->setPlaceholderText(placeholder);
            if (values)
                field.line->setText(here);
        } else if (field.boolean) {
            if (values) {
                const QString wanted = here.isEmpty() ? QString() : (isNo(here) ? QStringLiteral("no")
                                                                                : QStringLiteral("yes"));
                field.combo->setCurrentIndex(qMax(0, field.combo->findData(wanted)));
            }
            field.combo->setItemText(0, placeholder.isEmpty() ? tr("(not set here)")
                                                              : tr("(not set here: %1)").arg(placeholder));
        } else {
            if (field.combo->lineEdit())
                field.combo->lineEdit()->setPlaceholderText(placeholder);
            if (values)
                field.combo->setEditText(here);
        }
    }
    m_filling = false;
}

bool BJobDefinitionsWidget::isEditable() const
{
    return hasSelection() && m_raw && m_parsed.ok && supports(BConfig::Operation::UpdateRaw);
}

void BJobDefinitionsWidget::updateAccess()
{
    QString reason;
    if (!hasSelection()) {
        reason.clear();
    } else if (!supports(BConfig::Operation::UpdateRaw)) {
        const BConfig::Capabilities caps = m_client ? m_client->capabilities() : BConfig::Capabilities();
        if (!caps.known) {
            reason = tr("Read-only on this connection: changing Job and JobDefs resources needs the "
                        "configuration methods of the gRPC interface of the Onesimus Director. New Job and "
                        "New JobDefs use the job wizard (\"configure add\").");
        } else {
            reason = tr("Read-only: this session is not offered changes of %1 resources (the console needs the "
                        "command ACL \"configure\" and the ACL of the resource type).").arg(kindWord(m_kind));
        }
    } else if (m_rawLoading) {
        reason = tr("Reading the text of the resource from the Director...");
    } else if (!m_rawError.isEmpty()) {
        reason = tr("Read-only: the text of the resource could not be read: %1").arg(m_rawError);
    } else if (m_raw && !m_parsed.ok) {
        reason = tr("The form is locked: the text of the resource cannot be changed field by field safely "
                    "(%1). Edit the text directly instead.").arg(m_parsed.error);
    }
    m_readOnlyReason = reason;
    m_access->setText(reason);
    m_access->setVisible(!reason.isEmpty());

    const bool textMode = m_textEditing->isChecked();
    const bool formEditable = isEditable() && !textMode;
    for (Field &field : m_fields) {
        const bool locked = m_parsed.ok
            && (m_parsed.opaqueKeys.contains(field.key) || m_parsed.find(field.key) == -2);
        // A JobDefs does not name itself; its own JobDefs chain is allowed
        QWidget *editor = field.combo ? static_cast<QWidget *>(field.combo) : field.line;
        editor->setEnabled(formEditable && !locked);
    }
    const bool canText = hasSelection() && m_raw && supports(BConfig::Operation::UpdateRaw);
    m_textEditing->setEnabled(canText);
    m_text->setReadOnly(!(canText && textMode));
}

void BJobDefinitionsWidget::updateButtons()
{
    const bool busy = m_persistence->isBusy();
    const bool modified = isModified();
    m_saveButton->setEnabled(!busy && modified && hasSelection() && m_raw.has_value());
    m_revertButton->setEnabled(!busy && modified);
    m_deleteButton->setEnabled(!busy && hasSelection());
    const bool canCopy = hasSelection() && supports(BConfig::Operation::CreateRaw)
                      && supports(BConfig::Operation::ReadRaw);
    m_copyButton->setEnabled(!busy && canCopy);
    m_copyButton->setToolTip(canCopy ? tr("Creates a resource with the text of this one and a new name")
                                     : tr("Copying needs the configuration methods of the gRPC interface of the "
                                          "Onesimus Director (read and create) for this resource type"));
    m_newJobButton->setEnabled(!busy);
    m_newJobDefsButton->setEnabled(!busy);
    m_refreshButton->setEnabled(!busy);
}

// ============================================================================
// Fields
// ============================================================================

QWidget *BJobDefinitionsWidget::fieldEditor(const QString &key) const
{
    for (const Field &field : m_fields) {
        if (field.key == key)
            return field.combo ? static_cast<QWidget *>(field.combo) : field.line;
    }
    return nullptr;
}

QString BJobDefinitionsWidget::currentFieldValue(const Field &field) const
{
    if (field.line)
        return field.line->text().trimmed();
    if (field.boolean)
        return field.combo->currentData().toString();
    return field.combo->currentText().trimmed();
}

QString BJobDefinitionsWidget::fieldValue(const QString &key) const
{
    for (const Field &field : m_fields) {
        if (field.key == key)
            return currentFieldValue(field);
    }
    return QString();
}

bool BJobDefinitionsWidget::setFieldValue(const QString &key, const QString &value)
{
    for (Field &field : m_fields) {
        if (field.key != key)
            continue;
        if (field.line) {
            field.line->setText(value);
        } else if (field.boolean) {
            const int index = field.combo->findData(value);
            if (index < 0)
                return false;
            field.combo->setCurrentIndex(index);
        } else {
            field.combo->setEditText(value);
        }
        updateButtons();
        return true;
    }
    return false;
}

QString BJobDefinitionsWidget::baselineValue(const QString &key) const
{
    return m_baseline.value(key);
}

void BJobDefinitionsWidget::setTextEditing(bool on)
{
    m_textEditing->setChecked(on);
}

void BJobDefinitionsWidget::setEditedText(const QString &text)
{
    m_text->setPlainText(text);
}

QString BJobDefinitionsWidget::editedTextAsSent() const
{
    // The text is shown with "\n"; it is sent in the line ends of the file
    QString text = m_text->toPlainText();
    if (m_raw && m_parsed.lineEnd == QLatin1String("\r\n")) {
        text.replace(QLatin1String("\r\n"), QLatin1String("\n"));
        text.replace(QLatin1Char('\n'), QLatin1String("\r\n"));
    }
    return text;
}

QList<BResourceText::Change> BJobDefinitionsWidget::pendingChanges(QStringList *described) const
{
    QList<BResourceText::Change> changes;
    for (const Field &field : m_fields) {
        const QString now = currentFieldValue(field);
        const QString before = baselineValue(field.key);
        if (now == before)
            continue;
        if (now.isEmpty())
            changes.append(BResourceText::Change::unset(field.directive));
        else
            changes.append(BResourceText::Change::set(field.directive, now));
        if (described) {
            described->append(tr("%1: %2 → %3")
                                  .arg(field.directive,
                                       before.isEmpty() ? tr("(not set here)") : before,
                                       now.isEmpty() ? tr("(not set here, inherited)") : now));
        }
    }
    return changes;
}

bool BJobDefinitionsWidget::isModified() const
{
    if (!hasSelection())
        return false;
    if (m_textEditing->isChecked() && m_raw)
        return editedTextAsSent() != m_raw->text;
    if (!m_raw)
        return false;   // nothing can be edited without the text
    return !pendingChanges(nullptr).isEmpty();
}

// ============================================================================
// Actions
// ============================================================================

bool BJobDefinitionsWidget::allowed(BCapabilities::Function function)
{
    if (m_gate)
        return m_gate(this, function);
    const BCapabilities::Entry *entry = BCapabilities::entry(function);
    if (!entry)
        return false;
    // Without a gate: what the session's capabilities say; creating has the
    // wizard where they do not list it
    if (entry->bareos && entry->onesimus == BCapabilities::Support::Yes)
        return true;
    return m_client && m_client->capabilities().supports(entry->liveType, entry->liveOperation);
}

void BJobDefinitionsWidget::revert()
{
    if (!hasSelection())
        return;
    m_filling = true;
    m_textEditing->setChecked(false);
    if (m_raw) {
        QString shown = m_raw->text;
        shown.replace(QLatin1String("\r\n"), QLatin1String("\n"));
        m_text->setPlainText(shown);
    }
    m_filling = false;
    fillForm(true);
    updateAccess();
    updateButtons();
}

void BJobDefinitionsWidget::save()
{
    if (!hasSelection() || !isModified())
        return;
    const QString what = QStringLiteral("%1 %2").arg(kindWord(m_kind), m_name);
    const BCapabilities::Function function = m_kind == Kind::Job ? BCapabilities::Function::ModifyJob
                                                                 : BCapabilities::Function::ModifyJobDefs;
    if (!allowed(function) || !supports(BConfig::Operation::UpdateRaw) || !m_raw) {
        emit statusMessageChanged(tr("%1 was not written: changing it needs the configuration methods of the "
                                     "gRPC interface of the Onesimus Director. Nothing was sent.").arg(what));
        return;
    }
    if (m_persistence->isBusy()) {
        emit statusMessageChanged(tr("Another change is being written."));
        return;
    }

    QString newText;
    QStringList described;
    if (m_textEditing->isChecked()) {
        newText = editedTextAsSent();
        described.append(tr("The text as edited"));
    } else {
        QString error;
        const QList<BResourceText::Change> changes = pendingChanges(&described);
        if (!BResourceText::edit(m_raw->text, changes, &newText, &error)) {
            emit statusMessageChanged(tr("%1 cannot be written: %2").arg(what, error));
            QMessageBox::warning(this, tr("%1 was not written").arg(what),
                                 tr("The change could not be written into the text of the resource safely: "
                                    "%1\n\nNothing was sent.").arg(error));
            return;
        }
    }

    BResourceReview review;
    review.changes = described;
    if (m_kind == Kind::JobDefs) {
        review.affected = m_definitions.usersOf(m_name);
        review.affectedLabel = tr("Jobs and JobDefs that use this JobDefs");
        review.note = tr("A JobDefs gives its values to every job that does not set them itself; the change "
                         "applies to those of the %n resource(s) below.", "", int(review.affected.size()));
    } else {
        review.affected = {QStringLiteral("Job %1").arg(m_name)};
        review.affectedLabel = tr("Jobs affected");
        review.note = tr("Lines that were not edited stay as they are in the file.");
    }
    emit statusMessageChanged(tr("The Director checks the change of %1...").arg(what));
    m_persistence->update(typeOf(m_kind), m_name, *m_raw, newText, review);
}

void BJobDefinitionsWidget::deleteCurrent()
{
    if (!hasSelection())
        return;
    const QString what = QStringLiteral("%1 %2").arg(kindWord(m_kind), m_name);
    const BCapabilities::Function function = m_kind == Kind::Job ? BCapabilities::Function::DeleteJob
                                                                 : BCapabilities::Function::DeleteJobDefs;
    if (!allowed(function) || !supports(BConfig::Operation::Delete)) {
        emit statusMessageChanged(tr("%1 was not deleted: deleting it needs the configuration methods of the "
                                     "gRPC interface of the Onesimus Director. Nothing was sent.").arg(what));
        return;
    }
    if (m_persistence->isBusy()) {
        emit statusMessageChanged(tr("Another change is being written."));
        return;
    }
    BResourceReview review;
    if (m_kind == Kind::JobDefs) {
        review.affected = m_definitions.usersOf(m_name);
        review.affectedLabel = tr("Jobs and JobDefs that use this JobDefs");
    } else {
        review.affected = {QStringLiteral("Job %1").arg(m_name)};
        review.affectedLabel = tr("Jobs affected");
        review.note = tr("The job's records in the catalog stay; only its definition is removed. A running "
                         "job is not touched.");
    }
    emit statusMessageChanged(tr("The Director checks whether %1 can be deleted...").arg(what));
    m_persistence->remove(typeOf(m_kind), m_name, review);
}

std::optional<BJobDefinitionsWidget::NewResource> BJobDefinitionsWidget::askNewResource(Kind kind,
                                                                                        const QString &copyOf)
{
    if (m_newUi)
        return m_newUi(kind, copyOf);
    QMap<QString, QStringList> names = m_names ? m_names() : QMap<QString, QStringList>();
    names.insert(QStringLiteral("JobDefs"), m_definitions.jobDefsNames());
    QStringList taken = m_definitions.jobNames();
    taken.append(m_definitions.jobDefsNames());
    NewResourceDialog dialog(kind, copyOf, names, taken, this);
    if (dialog.exec() != QDialog::Accepted)
        return std::nullopt;
    return dialog.result();
}

void BJobDefinitionsWidget::newResource(Kind kind)
{
    const BCapabilities::Function function = kind == Kind::Job ? BCapabilities::Function::CreateJob
                                                               : BCapabilities::Function::CreateJobDefs;
    if (!supports(kind, BConfig::Operation::CreateRaw)) {
        // No CreateResource: the job wizard and "configure add", where the
        // Director has it
        if (allowed(function))
            emit wizardRequested(kind);
        return;
    }
    if (m_persistence->isBusy()) {
        emit statusMessageChanged(tr("Another change is being written."));
        return;
    }
    const std::optional<NewResource> wanted = askNewResource(kind, QString());
    if (!wanted || wanted->name.isEmpty())
        return;
    QList<BResourceText::Change> directives;
    directives.append(BResourceText::Change::set(QStringLiteral("Name"), wanted->name));
    directives.append(wanted->directives);
    const QString text = BResourceText::createText(kindWord(kind), directives);
    BResourceReview review;
    review.note = kind == Kind::Job ? tr("A new job. The Director checks that everything it needs is set, here "
                                         "or in its JobDefs.")
                                    : tr("A new JobDefs. No job uses it until a job names it.");
    m_selectAfterReload = std::make_pair(kind, wanted->name);
    emit statusMessageChanged(tr("The Director checks the new %1 %2...").arg(kindWord(kind), wanted->name));
    if (!m_persistence->create(typeOf(kind), wanted->name, text, review))
        m_selectAfterReload.reset();
}

void BJobDefinitionsWidget::copyCurrent()
{
    if (!hasSelection())
        return;
    const Kind kind = m_kind;
    const QString source = m_name;
    if (!supports(BConfig::Operation::CreateRaw) || !supports(BConfig::Operation::ReadRaw)) {
        emit statusMessageChanged(tr("%1 %2 was not copied: copying needs the configuration methods of the gRPC "
                                     "interface of the Onesimus Director. Nothing was sent.")
                                      .arg(kindWord(kind), source));
        return;
    }
    if (m_persistence->isBusy()) {
        emit statusMessageChanged(tr("Another change is being written."));
        return;
    }
    const std::optional<NewResource> wanted = askNewResource(kind, source);
    if (!wanted || wanted->name.isEmpty())
        return;
    const NewResource copy = *wanted;
    // The text of now, not the one on display: the copy is what the Director has
    m_persistence->read(typeOf(kind), source, this,
                        [this, kind, source, copy](const BConfig::Reply &reply,
                                                   const std::optional<BResourcePersistence::Raw> &raw) {
        if (!raw) {
            const BResourceReport report = BResourcePersistence::reportOf(reply, BResourceReview::Create,
                                                                          typeOf(kind), copy.name);
            emit statusMessageChanged(tr("%1 %2 was not copied: %3").arg(kindWord(kind), source, report.text));
            return;
        }
        QList<BResourceText::Change> changes;
        changes.append(BResourceText::Change::set(QStringLiteral("Name"), copy.name));
        changes.append(copy.directives);
        QString text;
        QString error;
        if (!BResourceText::edit(raw->text, changes, &text, &error)) {
            emit statusMessageChanged(tr("%1 %2 was not copied: %3").arg(kindWord(kind), source, error));
            return;
        }
        BResourceReview review;
        review.note = tr("A copy of %1 %2 with the new name.").arg(kindWord(kind), source);
        QStringList described;
        for (const BResourceText::Change &change : std::as_const(changes))
            described.append(QStringLiteral("%1 = %2").arg(change.key, change.value));
        review.changes = described;
        m_selectAfterReload = std::make_pair(kind, copy.name);
        emit statusMessageChanged(tr("The Director checks the copy %1...").arg(copy.name));
        if (!m_persistence->create(typeOf(kind), copy.name, text, review))
            m_selectAfterReload.reset();
    });
}

// ============================================================================
// After a write
// ============================================================================

void BJobDefinitionsWidget::onPersistenceFinished(const BResourceReport &report)
{
    if (report.kind == BResourceReport::Written) {
        QString text = report.title + QLatin1Char('.');
        if (report.reloaded)
            text += QLatin1Char(' ') + tr("The Director reloaded its configuration.");
        if (!report.followups.isEmpty())
            text += QLatin1Char(' ') + tr("Follow-up commands: %1").arg(report.followups.join(QLatin1String("; ")));
        emit statusMessageChanged(text);
        if (report.operation == BResourceReview::Update && report.name == m_name && typeOf(m_kind) == report.type)
            m_resetAfterWrite = true;
        if (report.operation == BResourceReview::Delete && report.name == m_name)
            clearDetail();
        emit definitionsChanged(report.type, report.name);
        return;
    }
    if (report.operation == BResourceReview::Create)
        m_selectAfterReload.reset();
    emit statusMessageChanged(report.title + QLatin1String(". ") + report.text);
    updateButtons();
}

void BJobDefinitionsWidget::onReloadRequested(BConfig::ResourceType type, const QString &name)
{
    if (!hasSelection() || typeOf(m_kind) != type || name != m_name)
        return;
    // After a write the form follows the new text; after a conflict the
    // edits stay and are written into the newer text by the next save
    const bool keep = !m_resetAfterWrite;
    m_resetAfterWrite = false;
    if (!keep) {
        m_filling = true;
        m_textEditing->setChecked(false);
        m_filling = false;
    }
    loadRaw(keep);
}
