/**
 * @file bprivatefile.h
 * @brief Files that only their owner may read (log file, files with passwords)
 */
#ifndef BPRIVATEFILE_H
#define BPRIVATEFILE_H

#include <QFile>

/**
 * @brief Open a file for writing with owner-only permissions (0600)
 *
 * A new file is created with these permissions; a file that already exists
 * is tightened before anything is written to it. On Windows the permission
 * bits have no effect beyond the read-only attribute; the file is protected
 * by the ACL of its directory there.
 *
 * @return false if the file could not be opened
 */
inline bool bOpenOwnerOnly(QFile &file, QIODevice::OpenMode mode)
{
    const QFileDevice::Permissions ownerOnly = QFileDevice::ReadOwner | QFileDevice::WriteOwner;
    if (!file.open(mode, ownerOnly)) {
        return false;
    }
    file.setPermissions(ownerOnly);
    return true;
}

#endif // BPRIVATEFILE_H
