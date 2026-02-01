#ifndef BPASSWORDUTIL_H
#define BPASSWORDUTIL_H

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

/**
 * @brief Utility class for Bareos/Bacula password hashing
 *
 * Provides password hashing functions compatible with Bareos/Bacula authentication.
 * Uses MD5 hashing as required by the Bareos CRAM-MD5 protocol.
 *
 * @note MD5 is cryptographically weak but required by Bareos protocol
 * @note Always use TLS encryption when connecting to Directors
 *
 * @since 0.1.0
 */
class BPasswordUtil
{
public:
    /**
     * @brief Hash password for database storage (MD5 hex format)
     *
     * Computes MD5 hash of password and returns as 32-character hex string.
     * This format is compatible with Bareos authentication and can be stored directly.
     *
     * @param password Plain text password
     * @return QString 32-character hexadecimal MD5 hash
     *
     * @par Example:
     * @code
     * QString hash = BPasswordUtil::hashPasswordForStorage("MyPassword123");
     * // Result: "482c811da5d5b4bc6d497ffa98491e38"
     * @endcode
     *
     * @see getPasswordHashBinary()
     * @since 0.1.0
     */
    static QString hashPasswordForStorage(const QString &password)
    {
        QByteArray passwordBytes = password.toLatin1();
        QByteArray md5Hash = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Md5);
        return QString::fromLatin1(md5Hash.toHex());
    }

    /**
     * @brief Get binary MD5 hash from stored hex hash
     *
     * Converts the 32-character hex MD5 hash back to 16-byte binary format.
     * Used for CRAM-MD5 authentication with HMAC calculation.
     *
     * @param storedHash 32-character hex hash from database
     * @return QByteArray 16-byte binary MD5 hash
     *
     * @par Example:
     * @code
     * QString storedHash = "482c811da5d5b4bc6d497ffa98491e38";
     * QByteArray binary = BPasswordUtil::getPasswordHashBinary(storedHash);
     * // Result: 16-byte binary data
     * @endcode
     *
     * @see hashPasswordForStorage()
     * @since 0.1.0
     */
    static QByteArray getPasswordHashBinary(const QString &storedHash)
    {
        return QByteArray::fromHex(storedHash.toLatin1());
    }

    /**
     * @brief Get hex MD5 hash for TLS-PSK authentication
     *
     * Returns the stored hash as-is (already in hex format).
     * Used for TLS-PSK mode where PSK key is MD5(password) as hex string.
     *
     * @param storedHash 32-character hex hash from database
     * @return QString Same hash (32-character hex string)
     *
     * @par Example:
     * @code
     * QString storedHash = "482c811da5d5b4bc6d497ffa98491e38";
     * QString pskKey = BPasswordUtil::getPasswordHashHex(storedHash);
     * // pskKey == storedHash (for TLS-PSK)
     * @endcode
     *
     * @since 0.1.0
     */
    static QString getPasswordHashHex(const QString &storedHash)
    {
        return storedHash;
    }

    /**
     * @brief Validate password hash format
     *
     * Checks if the provided hash is a valid 32-character hexadecimal MD5 hash.
     *
     * @param hash Hash string to validate
     * @return true if valid MD5 hex hash, false otherwise
     *
     * @par Example:
     * @code
     * bool valid = BPasswordUtil::isValidPasswordHash("482c811da5d5b4bc6d497ffa98491e38");
     * // Result: true
     *
     * bool invalid = BPasswordUtil::isValidPasswordHash("invalid");
     * // Result: false
     * @endcode
     *
     * @since 0.1.0
     */
    static bool isValidPasswordHash(const QString &hash)
    {
        if (hash.length() != 32) {
            return false;
        }

        // Check if all characters are hexadecimal
        for (const QChar &c : hash) {
            if (!c.isDigit() && (c.toLower() < 'a' || c.toLower() > 'f')) {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Hash password directly from plain text (for immediate use)
     *
     * Computes MD5 hash and returns as 16-byte binary.
     * Used when you need the binary hash directly without storing.
     *
     * @param password Plain text password
     * @return QByteArray 16-byte binary MD5 hash
     *
     * @par Example:
     * @code
     * QByteArray hash = BPasswordUtil::hashPassword("MyPassword123");
     * // Use hash directly for CRAM-MD5 HMAC
     * @endcode
     *
     * @since 0.1.0
     */
    static QByteArray hashPassword(const QString &password)
    {
        return QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5);
    }
};

#endif // BPASSWORDUTIL_H
