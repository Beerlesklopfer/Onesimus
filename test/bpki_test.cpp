// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bpki_test.cpp
 * @brief Certificate generator and PFX converter without widgets: owner-only
 *        key files, encrypted CA key, client certificate extensions, random
 *        serials, PFX written with a password and read back, or an error
 */

#include "bpki.h"

#include <QFile>
#include <QTemporaryDir>
#include <QtTest>

#include <openssl/bio.h>
#include <openssl/x509.h>

using namespace BPki;

class BPkiTest : public QObject
{
    Q_OBJECT

private:
    // 2048 bits: the generator uses 4096, the tests only need valid keys
    static KeyPtr key()
    {
        QString error;
        KeyPtr k = generateRsaKey(2048, &error);
        if (!k) qWarning() << error;
        return k;
    }

    static bool ownerOnly(const QString &path)
    {
#ifdef Q_OS_WIN
        Q_UNUSED(path)
        return true;   // permission bits do not apply; the directory ACL does
#else
        const QFileDevice::Permissions p = QFile::permissions(path);
        return (p & (QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup
                     | QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther)) == 0
            && (p & QFileDevice::ReadOwner);
#endif
    }

    static QByteArray readAll(const QString &path)
    {
        QFile f(path);
        return f.open(QIODevice::ReadOnly) ? f.readAll() : QByteArray();
    }

    static QByteArray der(X509 *cert)
    {
        unsigned char *buf = nullptr;
        const int len = i2d_X509(cert, &buf);
        QByteArray out(reinterpret_cast<const char *>(buf), len);
        OPENSSL_free(buf);
        return out;
    }

    struct Pair { KeyPtr caKey; CertPtr ca; KeyPtr clientKey; CertPtr client; };

    static Pair makePair(const QString &cn = QStringLiteral("onesimus-client"))
    {
        Pair p;
        QString error;
        p.caKey = key();
        CertificateRequest caReq;
        caReq.commonName = QStringLiteral("Onesimus-CA");
        caReq.organization = QStringLiteral("Onesimus");
        caReq.isCa = true;
        p.ca = makeCertificate(p.caKey.get(), caReq, nullptr, nullptr, &error);
        p.clientKey = key();
        CertificateRequest req;
        req.commonName = cn;
        req.organization = QStringLiteral("Onesimus");
        p.client = makeCertificate(p.clientKey.get(), req, p.ca.get(), p.caKey.get(), &error);
        return p;
    }

private slots:
    void clientCertificateExtensions()
    {
        Pair p = makePair();
        QVERIFY(p.ca);
        QVERIFY(p.client);
        const QString ext = extensionTexts(p.client.get()).join(QLatin1Char('\n'));
        QVERIFY2(ext.contains(QStringLiteral("basicConstraints (critical): CA:FALSE")), qPrintable(ext));
        QVERIFY2(ext.contains(QStringLiteral("keyUsage (critical): Digital Signature, Key Encipherment")), qPrintable(ext));
        QVERIFY2(ext.contains(QStringLiteral("extendedKeyUsage: TLS Web Client Authentication")), qPrintable(ext));
        QVERIFY2(ext.contains(QStringLiteral("subjectAltName: DNS:onesimus-client")), qPrintable(ext));
        QVERIFY2(ext.contains(QStringLiteral("authorityKeyIdentifier")), qPrintable(ext));
        QCOMPARE(commonName(p.client.get()), QStringLiteral("onesimus-client"));

        // The client certificate verifies against the CA
        X509_STORE *store = X509_STORE_new();
        X509_STORE_add_cert(store, p.ca.get());
        X509_STORE_CTX *ctx = X509_STORE_CTX_new();
        X509_STORE_CTX_init(ctx, store, p.client.get(), nullptr);
        const int ok = X509_verify_cert(ctx);
        X509_STORE_CTX_free(ctx);
        X509_STORE_free(store);
        QCOMPARE(ok, 1);
    }

    void caCertificateExtensions()
    {
        Pair p = makePair();
        const QString ext = extensionTexts(p.ca.get()).join(QLatin1Char('\n'));
        QVERIFY2(ext.contains(QStringLiteral("basicConstraints (critical): CA:TRUE")), qPrintable(ext));
        QVERIFY2(ext.contains(QStringLiteral("keyUsage (critical): Certificate Sign, CRL Sign")), qPrintable(ext));
        QVERIFY(!ext.contains(QStringLiteral("subjectAltName")));
    }

    void noSanForNonHostName()
    {
        Pair p = makePair(QStringLiteral("My Laptop"));
        QVERIFY(p.client);
        QVERIFY(!extensionTexts(p.client.get()).join(QLatin1Char('\n')).contains(QStringLiteral("subjectAltName")));
        QVERIFY(isDnsName(QStringLiteral("backup.example.org")));
        QVERIFY(!isDnsName(QStringLiteral("-bad")));
        QVERIFY(!isDnsName(QStringLiteral("a b")));
    }

    void randomSerials()
    {
        Pair a = makePair();
        Pair b = makePair();
        const QStringList serials = { serialHex(a.ca.get()), serialHex(a.client.get()),
                                      serialHex(b.ca.get()), serialHex(b.client.get()) };
        for (const QString &s : serials) {
            QVERIFY2(s.size() >= 32, qPrintable(s));   // ~159 bits, not a time stamp
        }
        QCOMPARE(QSet<QString>(serials.begin(), serials.end()).size(), 4);
    }

    void privateKeyOwnerOnly()
    {
        QTemporaryDir dir;
        KeyPtr k = key();
        const QString path = dir.filePath(QStringLiteral("client.key"));
        QString error;
        QVERIFY2(writePrivateKey(k.get(), path, QByteArray(), &error), qPrintable(error));
        QVERIFY(ownerOnly(path));
        KeyPtr back = loadPrivateKey(readAll(path), &error);
        QVERIFY2(back, qPrintable(error));
        QCOMPARE(EVP_PKEY_eq(k.get(), back.get()), 1);
    }

    void existingKeyFileTightened()
    {
        QTemporaryDir dir;
        const QString path = dir.filePath(QStringLiteral("ca.key"));
        {
            QFile f(path);
            QVERIFY(f.open(QIODevice::WriteOnly));
            f.write("old content that is longer than nothing\n");
        }
        QFile::setPermissions(path, QFileDevice::ReadOwner | QFileDevice::WriteOwner
                                        | QFileDevice::ReadGroup | QFileDevice::ReadOther);
        KeyPtr k = key();
        QString error;
        QVERIFY2(writePrivateKey(k.get(), path, QByteArray(), &error), qPrintable(error));
        QVERIFY(ownerOnly(path));
        QVERIFY(!readAll(path).contains("old content"));
    }

    void encryptedCaKey()
    {
        QTemporaryDir dir;
        KeyPtr k = key();
        const QString path = dir.filePath(QStringLiteral("ca.key"));
        QString error;
        QVERIFY2(writePrivateKey(k.get(), path, QByteArrayLiteral("s3cret passphrase"), &error), qPrintable(error));
        QVERIFY(ownerOnly(path));
        const QByteArray pem = readAll(path);
        QVERIFY(pem.startsWith("-----BEGIN ENCRYPTED PRIVATE KEY-----"));
        // Without the passphrase it does not load, and nothing prompts
        QVERIFY(!loadPrivateKey(pem, &error));
        QVERIFY2(error.contains(QStringLiteral("encrypted")), qPrintable(error));
    }

    void writeKeyIntoMissingDirectoryFails()
    {
        QTemporaryDir dir;
        KeyPtr k = key();
        QString error;
        QVERIFY(!writePrivateKey(k.get(), dir.filePath(QStringLiteral("no/such/dir/key.pem")), QByteArray(), &error));
        QVERIFY(!error.isEmpty());
    }

    void pkcs12WrittenAndReadBack()
    {
        QTemporaryDir dir;
        Pair p = makePair();
        const QString path = dir.filePath(QStringLiteral("client.pfx"));
        QString error;
        QVERIFY2(writePkcs12(p.clientKey.get(), p.client.get(), p.ca.get(), path,
                             QByteArrayLiteral("pfx-pass"), &error), qPrintable(error));
        QVERIFY(ownerOnly(path));
        KeyPtr k;
        CertPtr c;
        int caCount = -1;
        QVERIFY2(readPkcs12(path, QByteArrayLiteral("pfx-pass"), &k, &c, &caCount, &error), qPrintable(error));
        QVERIFY(keyMatchesCertificate(k.get(), c.get()));
        QCOMPARE(caCount, 1);
        QCOMPARE(commonName(c.get()), QStringLiteral("onesimus-client"));
        QVERIFY(!readPkcs12(path, QByteArrayLiteral("wrong"), nullptr, nullptr, nullptr, &error));
    }

    void pkcs12WithoutPasswordRefused()
    {
        QTemporaryDir dir;
        Pair p = makePair();
        const QString path = dir.filePath(QStringLiteral("client.pfx"));
        QString error;
        QVERIFY(!writePkcs12(p.clientKey.get(), p.client.get(), nullptr, path, QByteArray(), &error));
        QVERIFY(!error.isEmpty());
        QVERIFY(!QFile::exists(path));
    }

    void convertPemAndDer()
    {
        QTemporaryDir dir;
        Pair p = makePair();
        QString error;
        const QString keyPath = dir.filePath(QStringLiteral("client.key"));
        const QString certPath = dir.filePath(QStringLiteral("client.pem"));
        const QString caPath = dir.filePath(QStringLiteral("ca.pem"));
        QVERIFY(writePrivateKey(p.clientKey.get(), keyPath, QByteArray(), &error));
        QVERIFY(writeCertificate(p.client.get(), certPath, &error));
        QVERIFY(writeCertificate(p.ca.get(), caPath, &error));

        const QString pfx = dir.filePath(QStringLiteral("out.pfx"));
        QVERIFY2(convertToPkcs12(readAll(certPath), readAll(keyPath), readAll(caPath), pfx,
                                 QByteArrayLiteral("pw"), &error), qPrintable(error));
        int caCount = -1;
        QVERIFY(readPkcs12(pfx, QByteArrayLiteral("pw"), nullptr, nullptr, &caCount, &error));
        QCOMPARE(caCount, 1);

        // A DER certificate without CA works as well
        const QString pfx2 = dir.filePath(QStringLiteral("der.pfx"));
        QVERIFY2(convertToPkcs12(der(p.client.get()), readAll(keyPath), QByteArray(), pfx2,
                                 QByteArrayLiteral("pw"), &error), qPrintable(error));
        QVERIFY(readPkcs12(pfx2, QByteArrayLiteral("pw"), nullptr, nullptr, &caCount, &error));
        QCOMPARE(caCount, 0);
    }

    void convertReportsErrors()
    {
        QTemporaryDir dir;
        Pair p = makePair();
        Pair other = makePair();
        QString error;
        const QString keyPath = dir.filePath(QStringLiteral("other.key"));
        const QString certPath = dir.filePath(QStringLiteral("client.pem"));
        QVERIFY(writePrivateKey(other.clientKey.get(), keyPath, QByteArray(), &error));
        QVERIFY(writeCertificate(p.client.get(), certPath, &error));
        const QString pfx = dir.filePath(QStringLiteral("out.pfx"));

        // Key of another certificate
        error.clear();
        QVERIFY(!convertToPkcs12(readAll(certPath), readAll(keyPath), QByteArray(), pfx, "pw", &error));
        QVERIFY2(error.contains(QStringLiteral("does not belong")), qPrintable(error));
        QVERIFY(!QFile::exists(pfx));

        // Garbage instead of a certificate
        error.clear();
        QVERIFY(!convertToPkcs12("not a certificate", readAll(keyPath), QByteArray(), pfx, "pw", &error));
        QVERIFY2(error.startsWith(QStringLiteral("Certificate:")), qPrintable(error));
        QVERIFY(!QFile::exists(pfx));

        // Unwritable target
        error.clear();
        QVERIFY(writePrivateKey(p.clientKey.get(), keyPath, QByteArray(), &error));
        QVERIFY(!convertToPkcs12(readAll(certPath), readAll(keyPath), QByteArray(),
                                 dir.filePath(QStringLiteral("missing/out.pfx")), "pw", &error));
        QVERIFY(!error.isEmpty());
    }
};

QTEST_GUILESS_MAIN(BPkiTest)
#include "bpki_test.moc"
