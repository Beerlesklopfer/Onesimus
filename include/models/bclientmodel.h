#ifndef BCLIENTMODEL_H
#define BCLIENTMODEL_H

#include "models/bresourcemodel.h"

/**
 * @brief Model for Clients (File Daemon) table
 *
 * @since 0.1.0.5
 */
class BClientModel : public BResourceModel
{
    Q_OBJECT

public:
    enum Column {
        Id = 0,
        DirectorId = 1,
        Name = 2,
        Address = 3,
        FdPort = 4,
        PasswordHash = 5,
        Description = 6,
        Catalog = 7,
        FileRetention = 8,
        JobRetention = 9,
        AutoPrune = 10,
        MaximumConcurrentJobs = 11,
        WorkingDirectory = 12,
        PidDirectory = 13,
        PluginDirectory = 14,
        TlsEnable = 15,
        TlsRequire = 16,
        TlsCaCertificateFile = 17,
        TlsCertificate = 18,
        TlsKey = 19,
        CreatedAt = 20,
        UpdatedAt = 21,
        IsActive = 22
    };

    explicit BClientModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    bool initialize();

    int clientId(int row) const;
    int findClientRow(int clientId) const;
    QString clientName(int row) const;

    int createClient(int directorId, const QString &name, const QString &address,
                    int fdPort, const QString &passwordHash);
    bool deleteClient(int clientId);
    void setFilterByDirector(int directorId);
    void setFilterActiveOnly(bool activeOnly = true);
};

#endif // BCLIENTMODEL_H
