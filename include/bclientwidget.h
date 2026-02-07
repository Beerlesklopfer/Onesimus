#ifndef BCLIENTWIDGET_H
#define BCLIENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "director/bdirector.h"

namespace Ui {
class BClientWidget;
}

class BClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BClientWidget(BDirector *director, QWidget *parent = nullptr);
    ~BClientWidget();

signals:
    void sendCommand(BDirector::Command, const QString &args);

private slots:
    void onClientsReceived(const QList<BDirector::ClientInfo> &clients);
    void onStatusClicked();
    void onRefreshClicked();
    void onClientSelectionChanged();

private:
    void setupUI();
    void updateClientTable(const QList<BDirector::ClientInfo> &clients);

    Ui::BClientWidget *ui;
    BDirector *m_director;
    QTableWidget *m_clientTable;
    QPushButton *m_statusButton;
    QPushButton *m_refreshButton;
};

#endif // BCLIENTWIDGET_H
