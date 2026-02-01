#include "bdirectormodel.h"
#include "bpasswordutil.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDateTime>

BDirectorModel::BDirectorModel(QObject *parent, QSqlDatabase db)
    : BResourceModel(parent, db, "directors", "director")
{
}

bool BDirectorModel::initialize()
{
    // Select all records
    if (!select()) {
        return false;
    }

    // Set user-friendly header labels
    setHeaderData(Id, Qt::Horizontal, tr("ID"));
    setHeaderData(Name, Qt::Horizontal, tr("Name"));
    setHeaderData(Address, Qt::Horizontal, tr("Address"));
    setHeaderData(Port, Qt::Horizontal, tr("Port"));
    setHeaderData(PasswordHash, Qt::Horizontal, tr("Password"));
    setHeaderData(Description, Qt::Horizontal, tr("Description"));
    setHeaderData(DirPort, Qt::Horizontal, tr("DIR Port"));
    setHeaderData(QueryFile, Qt::Horizontal, tr("Query File"));
    setHeaderData(WorkingDirectory, Qt::Horizontal, tr("Working Dir"));
    setHeaderData(PidDirectory, Qt::Horizontal, tr("PID Dir"));
    setHeaderData(ScriptsDirectory, Qt::Horizontal, tr("Scripts Dir"));
    setHeaderData(PluginDirectory, Qt::Horizontal, tr("Plugin Dir"));
    setHeaderData(SubSysDirectory, Qt::Horizontal, tr("SubSys Dir"));
    setHeaderData(MaximumConcurrentJobs, Qt::Horizontal, tr("Max Jobs"));
    setHeaderData(TlsEnable, Qt::Horizontal, tr("TLS Enable"));
    setHeaderData(TlsRequire, Qt::Horizontal, tr("TLS Require"));
    setHeaderData(TlsVerifyPeer, Qt::Horizontal, tr("TLS Verify"));
    setHeaderData(TlsCaCertificateFile, Qt::Horizontal, tr("TLS CA Cert"));
    setHeaderData(TlsCertificate, Qt::Horizontal, tr("TLS Cert"));
    setHeaderData(TlsKey, Qt::Horizontal, tr("TLS Key"));
    setHeaderData(TlsAllowedCn, Qt::Horizontal, tr("TLS Allowed CN"));
    setHeaderData(BackupSystem, Qt::Horizontal, tr("System"));
    setHeaderData(CreatedAt, Qt::Horizontal, tr("Created"));
    setHeaderData(UpdatedAt, Qt::Horizontal, tr("Updated"));
    setHeaderData(LastConnectedAt, Qt::Horizontal, tr("Last Connected"));
    setHeaderData(IsActive, Qt::Horizontal, tr("Active"));

    return true;
}

int BDirectorModel::directorId(int row) const
{
    QModelIndex idx = index(row, Id);
    return data(idx).toInt();
}

int BDirectorModel::findDirectorRow(int directorId) const
{
    for (int row = 0; row < rowCount(); ++row) {
        if (this->directorId(row) == directorId) {
            return row;
        }
    }
    return -1;
}

QString BDirectorModel::directorName(int row) const
{
    QModelIndex idx = index(row, Name);
    return data(idx).toString();
}

int BDirectorModel::createDirector(const QString &name,
                                    const QString &address,
                                    int port,
                                    const QString &passwordHash,
                                    const QString &backupSystem)
{
    // Validate password hash
    if (!BPasswordUtil::isValidPasswordHash(passwordHash)) {
        qWarning() << "Invalid password hash format";
        return -1;
    }

    // Insert new row
    int row = rowCount();
    if (!insertRow(row)) {
        return -1;
    }

    // Set required fields
    setData(index(row, Name), name);
    setData(index(row, Address), address);
    setData(index(row, Port), port);
    setData(index(row, PasswordHash), passwordHash);
    setData(index(row, BackupSystem), backupSystem);
    setData(index(row, IsActive), 1);
    setData(index(row, MaximumConcurrentJobs), 20);  // Default

    // Set timestamps
    QString now = QDateTime::currentDateTime().toString(Qt::ISODate);
    setData(index(row, CreatedAt), now);
    setData(index(row, UpdatedAt), now);

    // Submit to database
    if (!submitAll()) {
        qWarning() << "Failed to create director:" << lastError().text();
        revertAll();
        return -1;
    }

    // Get the ID of the newly inserted record
    QSqlQuery query(database());
    query.prepare("SELECT id FROM directors WHERE name = :name ORDER BY id DESC LIMIT 1");
    query.bindValue(":name", name);

    if (query.exec() && query.next()) {
        int newId = query.value(0).toInt();
        emit resourceModified(newId, m_currentUser);
        return newId;
    }

    return -1;
}

bool BDirectorModel::deleteDirector(int directorId)
{
    int row = findDirectorRow(directorId);
    if (row < 0) {
        return false;
    }

    // Soft delete: set is_active = 0
    setData(index(row, IsActive), 0);
    setData(index(row, UpdatedAt), QDateTime::currentDateTime().toString(Qt::ISODate));

    bool success = submitAll();
    if (success) {
        // Also delete all custom settings
        deleteAllSettings(directorId);
        emit resourceModified(directorId, m_currentUser);
    }

    return success;
}

void BDirectorModel::setFilterActiveOnly(bool activeOnly)
{
    if (activeOnly) {
        setFilter("is_active = 1");
    } else {
        setFilter("");
    }
    select();
}
