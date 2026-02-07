#ifndef BSTORAGEMODEL_H
#define BSTORAGEMODEL_H

#include "db/bresourcemodel.h"

/**
 * @brief Model for Storages (Storage Daemon) table
 *
 * @since 0.1.0.5
 */
class BStorageModel : public BResourceModel
{
    Q_OBJECT

public:
    enum Column {
        Id = 0,
        DirectorId = 1,
        Name = 2,
        Address = 3,
        SdPort = 4,
        PasswordHash = 5,
        Description = 6,
        Device = 7,
        MediaType = 8,
        Autochanger = 9,
        MaximumConcurrentJobs = 10,
        AllowCompression = 11,
        HeartbeatInterval = 12,
        WorkingDirectory = 13,
        PidDirectory = 14,
        PluginDirectory = 15,
        ScriptsDirectory = 16,
        TlsEnable = 17,
        TlsRequire = 18,
        TlsCaCertificateFile = 19,
        TlsCertificate = 20,
        TlsKey = 21,
        CreatedAt = 22,
        UpdatedAt = 23,
        IsActive = 24
    };

    explicit BStorageModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    bool initialize();

    int storageId(int row) const;
    int findStorageRow(int storageId) const;
    QString storageName(int row) const;

    int createStorage(int directorId, const QString &name, const QString &address,
                     int sdPort, const QString &passwordHash);
    bool deleteStorage(int storageId);
    void setFilterByDirector(int directorId);
    void setFilterActiveOnly(bool activeOnly = true);
};

#endif // BSTORAGEMODEL_H
