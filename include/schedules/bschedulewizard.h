/**
 * @file bschedulewizard.h
 * @brief Wizard for creating new Schedule resources
 *
 * Three pages: the name (with description and Enabled), the grid of
 * weekdays and hours (BScheduleGridView, the editor of the schedule tab,
 * on an empty schedule) and the review of the Run lines and the text that
 * will be written. The schedule is created through CreateResource
 * (BSchedulePersistence::createSchedule(), checked and reviewed) where the
 * session offers it, else with "configure add schedule".
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#ifndef BSCHEDULEWIZARD_H
#define BSCHEDULEWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QStringList>

#include "director/bdirector.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QProgressBar;
class QPushButton;
class QTimer;
class BScheduleGridView;
class BSchedulePersistence;

// ============================================================================
// Wizard pages
// ============================================================================

/**
 * @brief Page 1: name, description, enabled
 */
class BScheduleNamePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BScheduleNamePage(QWidget *parent = nullptr);

    bool isComplete() const override;

    QString scheduleName() const;
    QString scheduleDescription() const;
    bool enabled() const;

    QLineEdit *nameEdit() const { return m_nameEdit; }
    QLineEdit *descriptionEdit() const { return m_descriptionEdit; }
    QCheckBox *enabledCheck() const { return m_enabledCheck; }
    /// Why the name cannot be taken; empty if it can
    QString nameProblem() const;

private:
    void updateProblem();

    QLineEdit *m_nameEdit = nullptr;
    QLineEdit *m_descriptionEdit = nullptr;
    QCheckBox *m_enabledCheck = nullptr;
    QLabel *m_problemLabel = nullptr;
};

/**
 * @brief Page 2: the grid editor on an empty schedule
 */
class BScheduleGridPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BScheduleGridPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool isComplete() const override;

    BScheduleGridView *gridView() const { return m_view; }

private:
    void updateHint();

    BScheduleGridView *m_view = nullptr;
    QLabel *m_hint = nullptr;
    bool m_started = false;
};

/**
 * @brief Page 3: the Run lines and the text that will be written; creates
 *        the schedule
 */
class BScheduleReviewPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BScheduleReviewPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool validatePage() override;

    QString runLinesText() const;
    QString configText() const;
    QString commandText() const;
    QString statusText() const;

private slots:
    void onConfigureResult(bool success, const QString &message);
    void onDirectorDisconnected();
    void onConfigureTimeout();

private:
    void executeConfigureCommand();
    void executeCreateResource();
    void disconnectDirectorSignals();
    void endWaiting();
    void setStatus(const QString &text, const QString &style = QString());

    QPlainTextEdit *m_runsEdit = nullptr;
    QPlainTextEdit *m_configEdit = nullptr;
    QLabel *m_commandLabel = nullptr;
    QPlainTextEdit *m_commandEdit = nullptr;
    QPushButton *m_copyButton = nullptr;
    QLabel *m_statusLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QTimer *m_timeoutTimer = nullptr;
    bool m_executed = false;
    bool m_waiting = false;     ///< A command or a create was sent, no result yet
    QList<QMetaObject::Connection> m_connections;
};

// ============================================================================
// Main wizard class
// ============================================================================

/**
 * @brief Wizard for creating new Schedule resources
 *
 * Pages:
 *   1. Name: name, description, Enabled
 *   2. Grid: the Run lines, painted on the grid of weekdays and hours
 *   3. Review: the Run lines and the resource text; Finish creates it
 */
class BScheduleWizard : public QWizard
{
    Q_OBJECT

public:
    enum PageId {
        Page_Name,
        Page_Grid,
        Page_Review
    };

    explicit BScheduleWizard(BDirector *director, QWidget *parent = nullptr);
    ~BScheduleWizard() override;

    BDirector *director() const { return m_director; }

    /**
     * @brief Create the schedule with CreateResource instead of "configure add"
     *
     * Set by the main window when the session of the Onesimus Director lists
     * CreateResource for schedules (BCapabilities::usesConfigMethod()). The
     * text of the review page is sent as it is shown; the Director checks it
     * first and the review asks before anything is written.
     */
    void setPersistence(BSchedulePersistence *persistence) { m_persistence = persistence; }
    BSchedulePersistence *persistence() const { return m_persistence; }

    /// Names of the schedules the Director has: a new one may not take them
    void setExistingScheduleNames(const QStringList &names) { m_existingNames = names; }
    QStringList existingScheduleNames() const { return m_existingNames; }

    BScheduleNamePage *namePage() const;
    BScheduleGridPage *gridPage() const;
    BScheduleReviewPage *reviewPage() const;
    BScheduleGridView *gridView() const;

    QString scheduleName() const;
    QString scheduleDescription() const;
    bool scheduleEnabled() const;

    /**
     * @brief Why a name cannot be the name of a new schedule
     * @return empty if it can
     */
    static QString nameProblem(const QString &name, const QStringList &existing);

    /**
     * @brief The Run lines of the grid as they would be written
     * @return false with @p error set if there are none or one cannot be
     *         written; then nothing is written
     */
    bool runTexts(QStringList *texts, QString *error = nullptr) const;

    /// The resource text for CreateResource (BScheduleResource::createText())
    QString generateConfigText() const;
    /// The arguments of "configure" for a Director without CreateResource
    QString generateConfigureCommand() const;

private:
    void init();

    BDirector *m_director = nullptr;
    BSchedulePersistence *m_persistence = nullptr;
    QStringList m_existingNames;
};

#endif // BSCHEDULEWIZARD_H
