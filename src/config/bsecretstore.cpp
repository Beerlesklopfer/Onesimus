// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

// The system headers first: GLib names struct members "signals", which Qt
// defines as a macro
#if defined(ONESIMUS_NO_KEYCHAIN)
#define ONESIMUS_SECRET_BACKEND 0
#elif defined(ONESIMUS_HAVE_LIBSECRET)
#define ONESIMUS_SECRET_BACKEND 1
#elif defined(__APPLE__)
#define ONESIMUS_SECRET_BACKEND 2
#elif defined(_WIN32)
#define ONESIMUS_SECRET_BACKEND 3
#else
#define ONESIMUS_SECRET_BACKEND 0
#endif

#if ONESIMUS_SECRET_BACKEND == 1
#include <libsecret/secret.h>
#elif ONESIMUS_SECRET_BACKEND == 2
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#elif ONESIMUS_SECRET_BACKEND == 3
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <wincred.h>
#endif

#include "config/bsecretstore.h"

// No texts to translate here: the names are product names, the messages
// come from the system (already in its language) or are error numbers

namespace {

#if ONESIMUS_SECRET_BACKEND == 1

// ---------------------------------------------------------------------------
// Linux: Secret Service through libsecret
// ---------------------------------------------------------------------------

const SecretSchema *onesimusSchema()
{
    static const SecretSchema schema = {
        "io.onesimus.Onesimus", SECRET_SCHEMA_NONE,
        {
            {"service", SECRET_SCHEMA_ATTRIBUTE_STRING},
            {"account", SECRET_SCHEMA_ATTRIBUTE_STRING},
            {nullptr, SECRET_SCHEMA_ATTRIBUTE_STRING},
        },
        0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };
    return &schema;
}

BSecretBackend::Result fromError(GError *error, QString *message)
{
    if (message)
        *message = QString::fromUtf8(error->message);
    // No Secret Service on the bus, or the user refused to unlock it
    const bool unavailable = error->domain == G_DBUS_ERROR
                             || error->domain == G_IO_ERROR
                             || (error->domain == SECRET_ERROR);
    g_error_free(error);
    return unavailable ? BSecretBackend::Result::Unavailable : BSecretBackend::Result::Failed;
}

class LibsecretBackend : public BSecretBackend
{
public:
    QString name() const override { return QStringLiteral("Secret Service"); }

    Result write(const QString &account, const QString &secret, QString *message) override
    {
        GError *error = nullptr;
        const QByteArray label = QStringLiteral("Onesimus: %1").arg(account).toUtf8();
        const QByteArray service = BSecretStore::serviceName().toUtf8();
        const QByteArray acc = account.toUtf8();
        const QByteArray value = secret.toUtf8();
        secret_password_store_sync(onesimusSchema(), SECRET_COLLECTION_DEFAULT,
                                   label.constData(), value.constData(), nullptr, &error,
                                   "service", service.constData(),
                                   "account", acc.constData(), nullptr);
        if (error)
            return fromError(error, message);
        return Result::Ok;
    }

    Result read(const QString &account, QString *secret, QString *message) override
    {
        GError *error = nullptr;
        const QByteArray service = BSecretStore::serviceName().toUtf8();
        const QByteArray acc = account.toUtf8();
        gchar *value = secret_password_lookup_sync(onesimusSchema(), nullptr, &error,
                                                   "service", service.constData(),
                                                   "account", acc.constData(), nullptr);
        if (error)
            return fromError(error, message);
        if (!value)
            return Result::NotFound;
        *secret = QString::fromUtf8(value);
        secret_password_free(value);
        return Result::Ok;
    }

    Result remove(const QString &account, QString *message) override
    {
        GError *error = nullptr;
        const QByteArray service = BSecretStore::serviceName().toUtf8();
        const QByteArray acc = account.toUtf8();
        const gboolean removed = secret_password_clear_sync(onesimusSchema(), nullptr, &error,
                                                            "service", service.constData(),
                                                            "account", acc.constData(), nullptr);
        if (error)
            return fromError(error, message);
        return removed ? Result::Ok : Result::NotFound;
    }
};

#elif ONESIMUS_SECRET_BACKEND == 2

// ---------------------------------------------------------------------------
// macOS: generic passwords in the login keychain
// ---------------------------------------------------------------------------

CFStringRef toCF(const QString &text)
{
    const QByteArray utf8 = text.toUtf8();
    return CFStringCreateWithBytes(kCFAllocatorDefault,
                                   reinterpret_cast<const UInt8 *>(utf8.constData()),
                                   CFIndex(utf8.size()), kCFStringEncodingUTF8, false);
}

QString statusText(OSStatus status)
{
    CFStringRef text = SecCopyErrorMessageString(status, nullptr);
    if (!text)
        return QStringLiteral("OSStatus %1").arg(int(status));
    char buffer[512];
    QString result = CFStringGetCString(text, buffer, sizeof buffer, kCFStringEncodingUTF8)
                         ? QString::fromUtf8(buffer)
                         : QStringLiteral("OSStatus %1").arg(int(status));
    CFRelease(text);
    return result;
}

BSecretBackend::Result fromStatus(OSStatus status, QString *message)
{
    if (status == errSecSuccess)
        return BSecretBackend::Result::Ok;
    if (status == errSecItemNotFound)
        return BSecretBackend::Result::NotFound;
    if (message)
        *message = statusText(status);
    if (status == errSecNotAvailable || status == errSecInteractionNotAllowed
        || status == errSecAuthFailed || status == errSecUserCanceled
        || status == errSecNoSuchKeychain)
        return BSecretBackend::Result::Unavailable;
    return BSecretBackend::Result::Failed;
}

/// Query of one entry; the caller adds what it needs and releases it
CFMutableDictionaryRef baseQuery(const QString &account)
{
    CFMutableDictionaryRef query = CFDictionaryCreateMutable(
        kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFStringRef service = toCF(BSecretStore::serviceName());
    CFStringRef acc = toCF(account);
    CFDictionarySetValue(query, kSecClass, kSecClassGenericPassword);
    CFDictionarySetValue(query, kSecAttrService, service);
    CFDictionarySetValue(query, kSecAttrAccount, acc);
    CFRelease(service);
    CFRelease(acc);
    return query;
}

class KeychainBackend : public BSecretBackend
{
public:
    QString name() const override { return QStringLiteral("Keychain"); }

    Result write(const QString &account, const QString &secret, QString *message) override
    {
        const QByteArray utf8 = secret.toUtf8();
        CFDataRef data = CFDataCreate(kCFAllocatorDefault,
                                      reinterpret_cast<const UInt8 *>(utf8.constData()),
                                      CFIndex(utf8.size()));
        CFMutableDictionaryRef query = baseQuery(account);
        CFMutableDictionaryRef update = CFDictionaryCreateMutable(
            kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        CFDictionarySetValue(update, kSecValueData, data);
        OSStatus status = SecItemUpdate(query, update);
        if (status == errSecItemNotFound) {
            CFDictionarySetValue(query, kSecValueData, data);
            status = SecItemAdd(query, nullptr);
        }
        CFRelease(update);
        CFRelease(query);
        CFRelease(data);
        return fromStatus(status, message);
    }

    Result read(const QString &account, QString *secret, QString *message) override
    {
        CFMutableDictionaryRef query = baseQuery(account);
        CFDictionarySetValue(query, kSecReturnData, kCFBooleanTrue);
        CFDictionarySetValue(query, kSecMatchLimit, kSecMatchLimitOne);
        CFTypeRef result = nullptr;
        const OSStatus status = SecItemCopyMatching(query, &result);
        CFRelease(query);
        const Result r = fromStatus(status, message);
        if (r == Result::Ok && result) {
            CFDataRef data = static_cast<CFDataRef>(result);
            *secret = QString::fromUtf8(reinterpret_cast<const char *>(CFDataGetBytePtr(data)),
                                        int(CFDataGetLength(data)));
        }
        if (result)
            CFRelease(result);
        return r;
    }

    Result remove(const QString &account, QString *message) override
    {
        CFMutableDictionaryRef query = baseQuery(account);
        const OSStatus status = SecItemDelete(query);
        CFRelease(query);
        return fromStatus(status, message);
    }
};

#elif ONESIMUS_SECRET_BACKEND == 3

// ---------------------------------------------------------------------------
// Windows: generic credentials of the Credential Manager
// ---------------------------------------------------------------------------

std::wstring targetName(const QString &account)
{
    return (BSecretStore::serviceName() + QLatin1Char('/') + account).toStdWString();
}

BSecretBackend::Result fromLastError(QString *message)
{
    const DWORD code = GetLastError();
    if (code == ERROR_NOT_FOUND)
        return BSecretBackend::Result::NotFound;
    if (message)
        *message = QStringLiteral("Windows error %1").arg(qulonglong(code));
    if (code == ERROR_NO_SUCH_LOGON_SESSION)
        return BSecretBackend::Result::Unavailable;
    return BSecretBackend::Result::Failed;
}

class CredentialBackend : public BSecretBackend
{
public:
    QString name() const override { return QStringLiteral("Credential Manager"); }

    Result write(const QString &account, const QString &secret, QString *message) override
    {
        std::wstring target = targetName(account);
        std::wstring user = account.toStdWString();
        QByteArray blob = secret.toUtf8();
        CREDENTIALW cred = {};
        cred.Type = CRED_TYPE_GENERIC;
        cred.TargetName = target.data();
        cred.UserName = user.data();
        cred.CredentialBlobSize = DWORD(blob.size());
        cred.CredentialBlob = reinterpret_cast<LPBYTE>(blob.data());
        cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
        if (!CredWriteW(&cred, 0))
            return fromLastError(message);
        return Result::Ok;
    }

    Result read(const QString &account, QString *secret, QString *message) override
    {
        const std::wstring target = targetName(account);
        PCREDENTIALW cred = nullptr;
        if (!CredReadW(target.c_str(), CRED_TYPE_GENERIC, 0, &cred))
            return fromLastError(message);
        *secret = QString::fromUtf8(reinterpret_cast<const char *>(cred->CredentialBlob),
                                    int(cred->CredentialBlobSize));
        CredFree(cred);
        return Result::Ok;
    }

    Result remove(const QString &account, QString *message) override
    {
        const std::wstring target = targetName(account);
        if (!CredDeleteW(target.c_str(), CRED_TYPE_GENERIC, 0))
            return fromLastError(message);
        return Result::Ok;
    }
};

#endif

} // namespace

std::unique_ptr<BSecretBackend> BSecretStore::createPlatformBackend()
{
#if ONESIMUS_SECRET_BACKEND == 1
    return std::make_unique<LibsecretBackend>();
#elif ONESIMUS_SECRET_BACKEND == 2
    return std::make_unique<KeychainBackend>();
#elif ONESIMUS_SECRET_BACKEND == 3
    return std::make_unique<CredentialBackend>();
#else
    return nullptr;
#endif
}
