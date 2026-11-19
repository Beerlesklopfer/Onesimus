#ifndef BSCHEDULEGRIDDIALOGS_H
#define BSCHEDULEGRIDDIALOGS_H

#include <QDialog>
#include <QList>
#include <QWidget>

#include "schedules/model/bschedulegridmodel.h"

class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QSpinBox;

/**
 * @file bschedulegriddialogs.h
 * @brief The small forms of the schedule grid: overrides, a new brush, a new scope
 *
 * None of them talks to a Director. Pools and storages are chosen from the
 * names the Director reported (".pools", ".storages"; decision of the
 * maintainer, 19.11.2026: pick lists, no free text); messages resources and
 * times are typed. What cannot be written as a Run line is said in the form
 * and cannot be accepted.
 */

/**
 * @brief Edits what a Run line overrides: the level and the other settings
 *
 * Every field is optional. An empty text field, "Level of the job" and
 * "not set" mean that the Run line does not name the setting and the job's
 * own applies.
 *
 * The pool fields and the storage field are pick lists of the names set
 * with setResourceNames(), the names the Director reported. A name a Run
 * line already has and the Director did not report is kept as an entry of
 * its own, marked as such, so that opening a line and accepting it changes
 * nothing.
 */
class BRunOverridesEditor : public QWidget
{
    Q_OBJECT

public:
    explicit BRunOverridesEditor(QWidget *parent = nullptr);

    void setOverrides(const BRunOverrides &overrides);
    BRunOverrides overrides() const;

    QComboBox *levelCombo() const { return m_level; }
    QComboBox *poolCombo() const { return m_pool; }
    QComboBox *storageCombo() const { return m_storage; }

    /// The names of the pick lists, for every editor made from now on (the
    /// names the Director reported with .pools and .storages)
    static void setResourceNames(const QStringList &pools, const QStringList &storages);
    static QStringList poolNames();
    static QStringList storageNames();

    /// Chooses @p name in a pick list (adding it marked if it is not offered); empty: not set
    static void choose(QComboBox *combo, const QString &name);

signals:
    void changed();

private:
    QComboBox *m_level;
    QComboBox *m_pool;
    QComboBox *m_fullPool;
    QComboBox *m_incrementalPool;
    QComboBox *m_differentialPool;
    QComboBox *m_nextPool;
    QComboBox *m_storage;
    QLineEdit *m_messages;
    QLineEdit *m_maxRunSchedTime;
    QCheckBox *m_havePriority;
    QSpinBox *m_priority;
    QComboBox *m_spoolData;
    QComboBox *m_accurate;
};

/**
 * @brief "New brush": the level, the minute and the overrides of a brush
 */
class BScheduleBrushDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BScheduleBrushDialog(QWidget *parent = nullptr);

    void setBrush(const BGridBrush &brush);
    BGridBrush brush() const;

    BRunOverridesEditor *overridesEditor() const { return m_overrides; }
    QSpinBox *minuteBox() const { return m_minute; }
    QDialogButtonBox *buttons() const { return m_buttons; }

    /// Can the brush be written as a Run line? Otherwise why not
    bool isValid(QString *error = nullptr) const;

private:
    void updatePreview();

    BRunOverridesEditor *m_overrides;
    QSpinBox *m_minute;
    QLabel *m_preview;
    QLabel *m_error;
    QDialogButtonBox *m_buttons;
};

/**
 * @brief "Add scope": the weeks of the month and the months of a new scope
 */
class BScheduleScopeDialog : public QDialog
{
    Q_OBJECT

public:
    /// @param existing the scopes there are, to say that one exists already
    explicit BScheduleScopeDialog(const QList<BGridScopeInfo> &existing, QWidget *parent = nullptr);

    void setScope(const BGridScope &scope);
    BGridScope scope() const;

    QList<QCheckBox *> weekBoxes() const { return m_weeks; }  ///< "1st" ... "5th"
    QCheckBox *lastBox() const { return m_last; }
    QList<QCheckBox *> monthBoxes() const { return m_months; }
    QDialogButtonBox *buttons() const { return m_buttons; }
    QString noteText() const;

private:
    void updateState();

    QList<BGridScopeInfo> m_existing;
    QList<QCheckBox *> m_weeks;
    QCheckBox *m_last;
    QList<QCheckBox *> m_months;
    QLabel *m_note;
    QDialogButtonBox *m_buttons;
};

#endif // BSCHEDULEGRIDDIALOGS_H
