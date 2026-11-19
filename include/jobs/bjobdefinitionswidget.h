#ifndef BJOBDEFINITIONSWIDGET_H
#define BJOBDEFINITIONSWIDGET_H

#include <QAbstractTableModel>
#include <QHash>
#include <QJsonObject>
#include <QMap>
#include <QPointer>
#include <QWidget>

#include <functional>
#include <optional>

#include "bcapabilities.h"
#include "config/bresourcepersistence.h"
#include "config/bresourcetext.h"
#include "jobs/bjobdefinitions.h"

class BResourceSaveDialogs;
class QCheckBox;
class QComboBox;
class QHeaderView;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class QSortFilterProxyModel;
class QSplitter;
class QTableView;
class QTableWidget;
class QTabWidget;
class QTimer;

/**
 * @file bjobdefinitionswidget.h
 * @brief The Job Definitions tab: the Job and JobDefs resources of the
 *        Director, what each directive is set to and where, and changing,
 *        creating, copying and deleting them (backlog P3-18)
 */

/**
 * @brief The table of Job and JobDefs resources
 *
 * One row per resource; a value a job inherits from its JobDefs is shown in
 * italics with the JobDefs in the tool tip. "Enabled" is a status chip
 * (BStatusDelegate). The sort role (Qt::UserRole) sorts the priority by its
 * number and text without regard to case.
 */
class BJobDefinitionsTableModel : public QAbstractTableModel
{
public:
    enum Column {
        ColName, ColKind, ColJobDefs, ColClient, ColFileSet, ColSchedule, ColPool, ColStorage,
        ColLevel, ColPriority, ColEnabled, ColumnCount
    };
    static constexpr int KindRole = Qt::UserRole + 1;   ///< int(BJobDefinitions::Kind)
    static constexpr int NameRole = Qt::UserRole + 2;

    explicit BJobDefinitionsTableModel(QObject *parent = nullptr);

    void setDefinitions(const BJobDefinitions *definitions);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /// The row of a resource; -1 if it is not in the table
    int rowOf(BJobDefinitions::Kind kind, const QString &name) const;

    /// The normalized directive a column shows ("" for name and kind)
    static QString keyOf(int column);

private:
    struct Row
    {
        BJobDefinitions::Kind kind = BJobDefinitions::Kind::Job;
        QString name;
        QHash<int, BJobDefinitions::Value> values;   ///< by column
    };
    QList<Row> m_rows;
};

/**
 * @brief The Job Definitions tab
 *
 * Left the table, right the selected resource:
 *  - "Directives": every directive with its value and where it is set:
 *    this resource, a JobDefs (named), or nowhere (the default applies);
 *  - "Edit": the common directives with pick lists of the names the
 *    application loaded; an empty field means "not set here", so the value
 *    of the JobDefs applies;
 *  - "Text": the resource as it stands in its file (ReadResource), and, on
 *    request, edited as text for everything the form does not have.
 *
 * Saving writes only the edited directives into that text
 * (BResourceText::edit(): every other line byte for byte), lets the
 * Director check it, shows the diff and the jobs the change applies to, and
 * writes it after "Write" (BResourcePersistence). Delete refuses what is
 * still referenced and lists the referrers. A copy is created disabled
 * unless the user says otherwise, so that it does not start on the
 * original's schedule.
 *
 * Changing and deleting need the configuration methods of the Onesimus
 * Director over gRPC, for the type, in this session (GetConfigCapabilities,
 * which reflects the ACLs of the console). Where they are missing the tab
 * is read-only, the gate shows the capability notice and nothing is sent.
 * Creating uses CreateResource where offered, else asks for the job wizard
 * ("configure add"). Nothing here starts or changes a running job.
 */
class BJobDefinitionsWidget : public QWidget
{
    Q_OBJECT

public:
    using Kind = BJobDefinitions::Kind;

    explicit BJobDefinitionsWidget(QWidget *parent = nullptr);
    ~BJobDefinitionsWidget() override;

    // --- Wiring -------------------------------------------------------------

    void setConfigClient(BConfigClient *client);
    BConfigClient *configClient() const { return m_client; }
    /// The dialogs of a write; tests answer them (nullptr: the real dialogs)
    void setSaveUi(BResourceSaveUi *ui);
    BResourcePersistence *persistence() const { return m_persistence; }

    /// Asked before a function is carried out; shows the capability notice
    /// where the Director cannot. Without a gate the capabilities decide.
    using Gate = std::function<bool(QWidget *parent, BCapabilities::Function function)>;
    void setGate(Gate gate) { m_gate = std::move(gate); }

    /// The names for the pick lists, by type: "Client", "FileSet", "Pool",
    /// "Storage", "Schedule", "Messages", "JobDefs"; asked when shown
    using NameSource = std::function<QMap<QString, QStringList>()>;
    void setNameSource(NameSource source) { m_names = std::move(source); }

    /// What a new resource or a copy is to be
    struct NewResource
    {
        QString name;
        QList<BResourceText::Change> directives;   ///< Without Name; for a copy only extra changes
    };
    /// Asks for a new resource (@p copyOf empty) or the name of a copy;
    /// std::nullopt: cancelled. Tests answer it; the default is a dialog.
    using NewResourceUi = std::function<std::optional<NewResource>(Kind kind, const QString &copyOf)>;
    void setNewResourceUi(NewResourceUi ui) { m_newUi = std::move(ui); }

    /// Asked before unsaved changes are dropped; true: drop them
    using DiscardUi = std::function<bool(const QString &resource)>;
    void setDiscardUi(DiscardUi ui) { m_discardUi = std::move(ui); }

    // --- Data ---------------------------------------------------------------

    /// The resources as loaded ("show jobs", "show jobdefs")
    void setResources(const QMap<QString, QJsonObject> &jobs, const QMap<QString, QJsonObject> &jobDefs);
    const BJobDefinitions &definitions() const { return m_definitions; }

    // --- State (also for tests) ---------------------------------------------

    bool selectResource(Kind kind, const QString &name);
    bool hasSelection() const { return !m_name.isEmpty(); }
    Kind currentKind() const { return m_kind; }
    QString currentName() const { return m_name; }
    bool isModified() const;
    /// The raw text is loaded and understood: the form can be edited
    bool isEditable() const;
    /// Why the selected resource cannot be changed here ("" if it can)
    QString readOnlyReason() const { return m_readOnlyReason; }
    /// The raw text is being read from the Director
    bool isLoadingText() const { return m_rawLoading; }

    /// The editor of a directive of the form ("client", "priority", ...)
    QWidget *fieldEditor(const QString &key) const;
    /// Sets a field as the user would; "" = not set here
    bool setFieldValue(const QString &key, const QString &value);
    QString fieldValue(const QString &key) const;
    /// Edit the text directly (replaces the form for this save)
    void setTextEditing(bool on);
    void setEditedText(const QString &text);

    QTableView *table() const { return m_table; }
    BJobDefinitionsTableModel *tableModel() const { return m_model; }
    QTableWidget *directivesView() const { return m_directives; }
    QPlainTextEdit *textView() const { return m_text; }
    QPushButton *saveButton() const { return m_saveButton; }
    QPushButton *deleteButton() const { return m_deleteButton; }
    QPushButton *copyButton() const { return m_copyButton; }

public slots:
    void save();
    void revert();
    void newResource(BJobDefinitions::Kind kind);
    void copyCurrent();
    void deleteCurrent();

signals:
    void statusMessageChanged(const QString &message);
    /// The user asked to read the Job and JobDefs resources again
    void refreshRequested();
    /// A write went through: the definitions and the schedule views are to
    /// be read again
    void definitionsChanged(BConfig::ResourceType type, const QString &name);
    /// Create without CreateResource: the job wizard ("configure add")
    void wizardRequested(BJobDefinitions::Kind kind);

private:
    struct Field
    {
        QString key;            ///< normalized
        QString directive;      ///< spelling for a new line ("FileSet")
        QString names;          ///< pick list type ("Client"), "" for none
        QStringList fixed;      ///< fixed choices (Type, Level)
        bool boolean = false;
        bool number = false;
        QComboBox *combo = nullptr;
        QLineEdit *line = nullptr;
        QLabel *note = nullptr;
    };

    void setupUi();
    void setupFields(QWidget *page);
    void onSelectionChanged();
    void showResource(Kind kind, const QString &name);
    void clearDetail();
    void loadRaw(bool keepEdits);
    void fillDirectives();
    void fillForm(bool values);
    void fillPickLists();
    void updateAccess();
    void updateButtons();
    void onPersistenceFinished(const BResourceReport &report);
    void onReloadRequested(BConfig::ResourceType type, const QString &name);
    bool allowed(BCapabilities::Function function);
    bool supports(BConfig::Operation operation) const;
    bool supports(Kind kind, BConfig::Operation operation) const;
    QString currentFieldValue(const Field &field) const;
    QString baselineValue(const QString &key) const;
    QList<BResourceText::Change> pendingChanges(QStringList *described) const;
    QString editedTextAsSent() const;
    bool confirmDiscard();
    void saveHeader();
    std::optional<NewResource> askNewResource(Kind kind, const QString &copyOf);

    static BConfig::ResourceType typeOf(Kind kind);
    static QString kindWord(Kind kind);

    BJobDefinitions m_definitions;
    BJobDefinitionsTableModel *m_model = nullptr;
    QSortFilterProxyModel *m_proxy = nullptr;
    QTableView *m_table = nullptr;
    QSplitter *m_splitter = nullptr;
    QLineEdit *m_filter = nullptr;
    QComboBox *m_kindFilter = nullptr;
    QPushButton *m_refreshButton = nullptr;
    QPushButton *m_newJobButton = nullptr;
    QPushButton *m_newJobDefsButton = nullptr;
    QPushButton *m_copyButton = nullptr;
    QPushButton *m_deleteButton = nullptr;

    QLabel *m_title = nullptr;
    QLabel *m_subtitle = nullptr;
    QLabel *m_access = nullptr;
    QTabWidget *m_tabs = nullptr;
    QTableWidget *m_directives = nullptr;
    QWidget *m_formPage = nullptr;
    QList<Field> m_fields;
    QPlainTextEdit *m_text = nullptr;
    QCheckBox *m_textEditing = nullptr;
    QPushButton *m_saveButton = nullptr;
    QPushButton *m_revertButton = nullptr;
    QTimer *m_headerTimer = nullptr;

    QPointer<BConfigClient> m_client;
    BResourcePersistence *m_persistence = nullptr;
    BResourceSaveDialogs *m_saveDialogs = nullptr;
    Gate m_gate;
    NameSource m_names;
    NewResourceUi m_newUi;
    DiscardUi m_discardUi;

    // The selected resource
    Kind m_kind = Kind::Job;
    QString m_name;
    std::optional<BResourcePersistence::Raw> m_raw;
    BResourceText::Parsed m_parsed;
    QString m_rawError;             ///< Why the raw text is not there
    bool m_rawLoading = false;
    QString m_readOnlyReason;
    QHash<QString, QString> m_baseline;     ///< key -> value set here (form baseline)
    bool m_filling = false;
    bool m_resetAfterWrite = false;
    std::optional<std::pair<Kind, QString>> m_selectAfterReload;
    quint64 m_readSerial = 0;
};

#endif // BJOBDEFINITIONSWIDGET_H
