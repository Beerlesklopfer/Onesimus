// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BSECRETSTORE_H
#define BSECRETSTORE_H

#include <QHash>
#include <QString>
#include <memory>

/**
 * @brief A place for secrets outside the settings file: the keychain of the system
 *
 * Linux: the Secret Service (GNOME Keyring, KWallet, KeePassXC) through
 * libsecret; macOS: the login keychain; Windows: the Credential Manager.
 * Every call is synchronous; BSettings keeps what it read in memory, so the
 * keychain is asked once per secret and session.
 *
 * Entries are stored under the service "Onesimus" and an account name
 * chosen by the caller (BSettings: "<profile id>/passwordHash").
 */
class BSecretBackend
{
public:
    enum class Result {
        Ok,
        NotFound,       ///< read/remove: no such entry
        Unavailable,    ///< No keychain (no Secret Service running, locked and refused, ...)
        Failed          ///< The keychain answered with an error
    };

    virtual ~BSecretBackend() = default;

    /// Name shown to the user ("Secret Service", "Keychain", "Credential Manager")
    virtual QString name() const = 0;

    virtual Result write(const QString &account, const QString &secret, QString *error) = 0;
    virtual Result read(const QString &account, QString *secret, QString *error) = 0;
    virtual Result remove(const QString &account, QString *error) = 0;
};

namespace BSecretStore {

/// The keychain of this platform; nullptr when the build has none
std::unique_ptr<BSecretBackend> createPlatformBackend();

/// Service name of the entries
inline QString serviceName() { return QStringLiteral("Onesimus"); }

} // namespace BSecretStore

/**
 * @brief Keychain in memory, for tests
 *
 * setAvailable(false) makes every call answer Unavailable, as a system
 * without a keychain does.
 */
class BMemorySecretBackend : public BSecretBackend
{
public:
    QString name() const override { return QStringLiteral("Memory"); }

    Result write(const QString &account, const QString &secret, QString *) override
    {
        if (!m_available)
            return Result::Unavailable;
        m_entries.insert(account, secret);
        ++m_writes;
        return Result::Ok;
    }

    Result read(const QString &account, QString *secret, QString *) override
    {
        if (!m_available)
            return Result::Unavailable;
        ++m_reads;
        const auto it = m_entries.constFind(account);
        if (it == m_entries.constEnd())
            return Result::NotFound;
        *secret = it.value();
        return Result::Ok;
    }

    Result remove(const QString &account, QString *) override
    {
        if (!m_available)
            return Result::Unavailable;
        return m_entries.remove(account) > 0 ? Result::Ok : Result::NotFound;
    }

    void setAvailable(bool available) { m_available = available; }
    QHash<QString, QString> entries() const { return m_entries; }
    int writes() const { return m_writes; }
    int reads() const { return m_reads; }

private:
    QHash<QString, QString> m_entries;
    bool m_available = true;
    int m_writes = 0;
    int m_reads = 0;
};

#endif // BSECRETSTORE_H
