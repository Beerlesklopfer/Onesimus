#ifndef STORAGEWIDGET_H
#define STORAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "bdirector.h"

namespace Ui {
class StorageWidget;
}

class StorageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StorageWidget(BDirector *director, QWidget *parent = nullptr);
    ~StorageWidget();

signals:
    void sendCommand(BDirector::Command, const QString &args);

private slots:
    void onVolumesReceived(const QList<BDirector::VolumeInfo> &volumes);
    void onRefreshClicked();
    void onVolumeSelectionChanged();

private:
    void setupUI();
    void updateVolumeTable(const QList<BDirector::VolumeInfo> &volumes);
    QString formatBytes(qint64 bytes);

    Ui::StorageWidget *ui;
    BDirector *m_director;
    QTableWidget *m_volumeTable;
    QPushButton *m_refreshButton;
};

#endif // STORAGEWIDGET_H
