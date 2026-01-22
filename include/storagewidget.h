#ifndef STORAGEWIDGET_H
#define STORAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "director.h"

namespace Ui {
class StorageWidget;
}

class StorageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StorageWidget(Director *director, QWidget *parent = nullptr);
    ~StorageWidget();

private slots:
    void onVolumesReceived(const QList<Director::VolumeInfo> &volumes);
    void onRefreshClicked();
    void onVolumeSelectionChanged();

private:
    void setupUI();
    void updateVolumeTable(const QList<Director::VolumeInfo> &volumes);
    QString formatBytes(qint64 bytes);

    Ui::StorageWidget *ui;
    Director *m_director;
    QTableWidget *m_volumeTable;
    QPushButton *m_refreshButton;
};

#endif // STORAGEWIDGET_H
