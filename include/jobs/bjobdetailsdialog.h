#ifndef BJOBDETAILSDIALOG_H
#define BJOBDETAILSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>

/**
 * @brief Dialog to display detailed information about a backup job
 */
class BJobDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BJobDetailsDialog(const QJsonObject &job, QWidget *parent = nullptr);

private:
    void setupUi(const QJsonObject &job);
    QString formatBytes(qint64 bytes) const;
    QString formatStatus(const QString &status) const;
    QString getStatusColor(const QString &status) const;
};

#endif // BJOBDETAILSDIALOG_H
