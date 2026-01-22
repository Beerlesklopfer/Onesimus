#ifndef JOBWIDGET_H
#define JOBWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "director.h"

namespace Ui {
class JobWidget;
}

class JobWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JobWidget(Director *director, QWidget *parent = nullptr);
    ~JobWidget();

private slots:
    void onJobsReceived(const QList<Director::JobInfo> &jobs);
    void onRunJobClicked();
    void onCancelJobClicked();
    void onShowDetailsClicked();
    void onRefreshClicked();
    void onJobSelectionChanged();

private:
    void setupUI();
    void updateJobTable(const QList<Director::JobInfo> &jobs);
    QString formatBytes(qint64 bytes);
    QString formatJobStatus(const QString &status);

    Ui::JobWidget *ui;
    Director *m_director;
    
    QTableWidget *m_jobTable;
    QPushButton *m_runJobButton;
    QPushButton *m_cancelJobButton;
    QPushButton *m_detailsButton;
    QPushButton *m_refreshButton;
};

#endif // JOBWIDGET_H
