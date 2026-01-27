#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "bdirector.h"

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(BDirector *director, QWidget *parent = nullptr);
    ~ClientWidget();

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

    Ui::ClientWidget *ui;
    BDirector *m_director;
    QTableWidget *m_clientTable;
    QPushButton *m_statusButton;
    QPushButton *m_refreshButton;
};

#endif // CLIENTWIDGET_H
