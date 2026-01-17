#ifndef STORAGEWIDGET_H
#define STORAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "baculadirector.h"

namespace Ui {
class StorageWidget;
}

class StorageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StorageWidget(BaculaDirector *director, QWidget *parent = nullptr);
    ~StorageWidget();

private slots:
    void onVolumesReceived(const QList<BaculaDirector::VolumeInfo> &volumes);
    void onRefreshClicked();
    void onVolumeSelectionChanged();

private:
    void setupUI();
    void updateVolumeTable(const QList<BaculaDirector::VolumeInfo> &volumes);
    QString formatBytes(qint64 bytes);

    Ui::StorageWidget *ui;
    BaculaDirector *m_director;
    QTableWidget *m_volumeTable;
    QPushButton *m_refreshButton;
};

#endif // STORAGEWIDGET_H
