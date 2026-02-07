#ifndef BCONSOLEMODEL_H
#define BCONSOLEMODEL_H

#include "db/bresourcemodel.h"

/**
 * @brief Model for Consoles (bconsole) table
 *
 * @since 0.1.0.5
 */
class BConsoleModel : public BResourceModel
{
    Q_OBJECT

public:
    enum Column {
        Id = 0,
        DirectorId = 1,
        Name = 2,
        PasswordHash = 3,
        Description = 4,
        Catalog = 5,
        CommandAcl = 6,
        JobAcl = 7,
        ScheduleAcl = 8,
        ClientAcl = 9,
        StorageAcl = 10,
        PoolAcl = 11,
        FilesetAcl = 12,
        WhereAcl = 13,
        TlsEnable = 14,
        TlsRequire = 15,
        TlsCaCertificateFile = 16,
        TlsCertificate = 17,
        TlsKey = 18,
        CreatedAt = 19,
        UpdatedAt = 20,
        IsActive = 21
    };

    explicit BConsoleModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    bool initialize();

    int consoleId(int row) const;
    int findConsoleRow(int consoleId) const;
    QString consoleName(int row) const;

    int createConsole(int directorId, const QString &name, const QString &passwordHash);
    bool deleteConsole(int consoleId);
    void setFilterByDirector(int directorId);
    void setFilterActiveOnly(bool activeOnly = true);
};

#endif // BCONSOLEMODEL_H
