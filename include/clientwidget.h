#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "baculadirector.h"

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(BaculaDirector *director, QWidget *parent = nullptr);
    ~ClientWidget();

private slots:
    void onClientsReceived(const QList<BaculaDirector::ClientInfo> &clients);
    void onStatusClicked();
    void onRefreshClicked();
    void onClientSelectionChanged();

private:
    void setupUI();
    void updateClientTable(const QList<BaculaDirector::ClientInfo> &clients);

    Ui::ClientWidget *ui;
    BaculaDirector *m_director;
    QTableWidget *m_clientTable;
    QPushButton *m_statusButton;
    QPushButton *m_refreshButton;
};

#endif // CLIENTWIDGET_H
