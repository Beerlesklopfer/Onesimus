#ifndef BJOBLOGDIALOG_H
#define BJOBLOGDIALOG_H

#include <QDialog>
#include <QListView>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QJsonObject>
#include "jobs/bjobmodels.h"
#include "bdirector.h"

/**
 * @brief Kleiner Dialog zur Anzeige des Job-Logs
 * @version 1.0
 * @since 2026-01-29
 *
 * Zeigt das Log eines Jobs in einem kompakten Dialog an.
 */
class BJobLogDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor
     * @param job Job-Daten als JSON-Objekt (muss mindestens "jobid" und "name" enthalten)
     * @param director Director-Verbindung zum Laden des Logs
     * @param parent Eltern-Widget
     */
    explicit BJobLogDialog(const QJsonObject &job, BDirector *director, QWidget *parent = nullptr);

private slots:
    /**
     * @brief Empfängt Job-Log-Daten vom Director
     * @param command Ausgeführter Befehl
     * @param response Antwort vom Director
     */
    void onJobLogReceived(const QString &command, const QString &response);

    /**
     * @brief Kopiert ausgewählte Log-Zeilen in die Zwischenablage
     */
    void onCopySelected();

    /**
     * @brief Wählt alle Log-Zeilen aus
     */
    void onSelectAll();

private:
    void setupUI();
    void loadJobLog();

    QJsonObject m_job;
    BDirector *m_director;
    quint64 m_jobId;

    QListView *m_logListView;
    BJobLogModel *m_logModel;
    QPushButton *m_copyButton;
    QPushButton *m_selectAllButton;
};

#endif // BJOBLOGDIALOG_H
