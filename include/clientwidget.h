#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "director.h"

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(Director *director, QWidget *parent = nullptr);
    ~ClientWidget();

private slots:
    void onClientsReceived(const QList<Director::ClientInfo> &clients);
    void onStatusClicked();
    void onRefreshClicked();
    void onClientSelectionChanged();

private:
    void setupUI();
    void updateClientTable(const QList<Director::ClientInfo> &clients);

    Ui::ClientWidget *ui;
    Director *m_director;
    QTableWidget *m_clientTable;
    QPushButton *m_statusButton;
    QPushButton *m_refreshButton;
};

#endif // CLIENTWIDGET_H
