// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bpki.cpp
 * @brief Keys, certificates and PKCS#12 files (see bpki.h)
 *
 * OpenSSL writes into memory BIOs only; the bytes reach the disk through
 * QFile, which creates key files with owner-only permissions (and avoids the
 * FILE* applink problem on Windows).
 */

#include "bpki.h"
#include "bprivatefile.h"

#include <QCoreApplication>
#include <QFile>
#include <QRegularExpression>

#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

namespace BPki {

void KeyDeleter::operator()(EVP_PKEY *key) const { EVP_PKEY_free(key); }
void CertDeleter::operator()(X509 *cert) const { X509_free(cert); }

namespace {

struct BioDeleter { void operator()(BIO *bio) const { BIO_free(bio); } };
using BioPtr = std::unique_ptr<BIO, BioDeleter>;

// Translation context "BPki" for the messages below (lupdate reads Text::tr()
// inside a class with Q_DECLARE_TR_FUNCTIONS)
class Text
{
    Q_DECLARE_TR_FUNCTIONS(BPki)
};

// The oldest error OpenSSL queued, then the queue is emptied so that the
// next failure does not report a stale reason
QString opensslError()
{
    const unsigned long err = ERR_get_error();
    ERR_clear_error();
    if (err == 0) {
        return QString();
    }
    char buf[256];
    ERR_error_string_n(err, buf, sizeof(buf));
    return QString::fromLatin1(buf);
}

void setError(QString *error, const QString &text)
{
    if (error) {
        const QString reason = opensslError();
        *error = reason.isEmpty() ? text : QStringLiteral("%1 (%2)").arg(text, reason);
    }
}

QByteArray bioBytes(BIO *bio)
{
    char *data = nullptr;
    const long len = BIO_get_mem_data(bio, &data);
    return len > 0 ? QByteArray(data, int(len)) : QByteArray();
}

bool writeFile(const QString &path, const QByteArray &bytes, bool ownerOnly, QString *error)
{
    QFile file(path);
    const QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Truncate;
    const bool opened = ownerOnly ? bOpenOwnerOnly(file, mode) : file.open(mode);
    if (!opened) {
        if (error) {
            *error = Text::tr("Cannot open %1 for writing: %2").arg(path, file.errorString());
        }
        return false;
    }
    if (file.write(bytes) != bytes.size() || !file.flush()) {
        if (error) {
            *error = Text::tr("Cannot write %1: %2").arg(path, file.errorString());
        }
        file.close();
        file.remove();
        return false;
    }
    file.close();
    return true;
}

bool addExtension(X509 *cert, X509V3_CTX *ctx, int nid, const char *value, QString *error)
{
    X509_EXTENSION *ext = X509V3_EXT_conf_nid(nullptr, ctx, nid, const_cast<char *>(value));
    if (!ext) {
        setError(error, Text::tr("Cannot create certificate extension %1").arg(QString::fromLatin1(OBJ_nid2sn(nid))));
        return false;
    }
    const int ok = X509_add_ext(cert, ext, -1);
    X509_EXTENSION_free(ext);
    if (ok != 1) {
        setError(error, Text::tr("Cannot add certificate extension %1").arg(QString::fromLatin1(OBJ_nid2sn(nid))));
        return false;
    }
    return true;
}

// A random positive serial of 159 bits (RFC 5280: at most 20 octets, and
// CA/Browser Forum: at least 64 bits of randomness)
bool setRandomSerial(X509 *cert)
{
    BIGNUM *bn = BN_new();
    if (!bn) {
        return false;
    }
    bool ok = BN_rand(bn, 159, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY) == 1;
    if (ok && BN_is_zero(bn)) {
        ok = BN_set_word(bn, 1) == 1;
    }
    ASN1_INTEGER *serial = ok ? BN_to_ASN1_INTEGER(bn, nullptr) : nullptr;
    BN_free(bn);
    if (!serial) {
        return false;
    }
    ok = X509_set_serialNumber(cert, serial) == 1;
    ASN1_INTEGER_free(serial);
    return ok;
}

} // namespace

bool isDnsName(const QString &name)
{
    static const QRegularExpression re(
        QStringLiteral("^(?=.{1,253}$)([A-Za-z0-9]([A-Za-z0-9-]{0,61}[A-Za-z0-9])?)(\\.[A-Za-z0-9]([A-Za-z0-9-]{0,61}[A-Za-z0-9])?)*$"));
    return re.match(name).hasMatch();
}

KeyPtr generateRsaKey(int bits, QString *error)
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx) {
        setError(error, Text::tr("Cannot create the key context"));
        return KeyPtr();
    }
    EVP_PKEY *key = nullptr;
    if (EVP_PKEY_keygen_init(ctx) <= 0
        || EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0
        || EVP_PKEY_keygen(ctx, &key) <= 0) {
        setError(error, Text::tr("Cannot generate the RSA key"));
        EVP_PKEY_CTX_free(ctx);
        return KeyPtr();
    }
    EVP_PKEY_CTX_free(ctx);
    return KeyPtr(key);
}

CertPtr makeCertificate(EVP_PKEY *key, const CertificateRequest &request,
                        X509 *issuerCert, EVP_PKEY *issuerKey, QString *error)
{
    if (!key || request.commonName.isEmpty() || request.validDays <= 0
        || (issuerCert == nullptr) != (issuerKey == nullptr)) {
        if (error) {
            *error = Text::tr("Invalid certificate request");
        }
        return CertPtr();
    }

    CertPtr cert(X509_new());
    if (!cert || X509_set_version(cert.get(), 2) != 1 || !setRandomSerial(cert.get())) {
        setError(error, Text::tr("Cannot create the certificate"));
        return CertPtr();
    }

    X509_gmtime_adj(X509_getm_notBefore(cert.get()), 0);
    X509_time_adj_ex(X509_getm_notAfter(cert.get()), request.validDays, 0, nullptr);

    if (X509_set_pubkey(cert.get(), key) != 1) {
        setError(error, Text::tr("Cannot set the public key"));
        return CertPtr();
    }

    X509_NAME *name = X509_get_subject_name(cert.get());
    const QByteArray org = request.organization.toUtf8();
    const QByteArray cn = request.commonName.toUtf8();
    if ((!org.isEmpty()
         && X509_NAME_add_entry_by_txt(name, "O", MBSTRING_UTF8,
                                       reinterpret_cast<const unsigned char *>(org.constData()), -1, -1, 0) != 1)
        || X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_UTF8,
                                      reinterpret_cast<const unsigned char *>(cn.constData()), -1, -1, 0) != 1) {
        setError(error, Text::tr("Cannot set the subject name"));
        return CertPtr();
    }

    X509 *issuer = issuerCert ? issuerCert : cert.get();
    if (X509_set_issuer_name(cert.get(), X509_get_subject_name(issuer)) != 1) {
        setError(error, Text::tr("Cannot set the issuer name"));
        return CertPtr();
    }

    X509V3_CTX ctx;
    X509V3_set_ctx_nodb(&ctx);
    X509V3_set_ctx(&ctx, issuer, cert.get(), nullptr, nullptr, 0);

    bool ok = addExtension(cert.get(), &ctx, NID_subject_key_identifier, "hash", error);
    if (ok && issuerCert) {
        ok = addExtension(cert.get(), &ctx, NID_authority_key_identifier, "keyid:always", error);
    }
    if (ok && request.isCa) {
        ok = addExtension(cert.get(), &ctx, NID_basic_constraints, "critical,CA:TRUE", error)
          && addExtension(cert.get(), &ctx, NID_key_usage, "critical,keyCertSign,cRLSign", error);
    } else if (ok) {
        ok = addExtension(cert.get(), &ctx, NID_basic_constraints, "critical,CA:FALSE", error)
          && addExtension(cert.get(), &ctx, NID_key_usage, "critical,digitalSignature,keyEncipherment", error)
          && addExtension(cert.get(), &ctx, NID_ext_key_usage, "clientAuth", error);
        if (ok && isDnsName(request.commonName)) {
            const QByteArray san = "DNS:" + cn;
            ok = addExtension(cert.get(), &ctx, NID_subject_alt_name, san.constData(), error);
        }
    }
    if (!ok) {
        return CertPtr();
    }

    if (X509_sign(cert.get(), issuerKey ? issuerKey : key, EVP_sha256()) <= 0) {
        setError(error, Text::tr("Cannot sign the certificate"));
        return CertPtr();
    }
    return cert;
}

QString serialHex(X509 *cert)
{
    const ASN1_INTEGER *serial = cert ? X509_get0_serialNumber(cert) : nullptr;
    BIGNUM *bn = serial ? ASN1_INTEGER_to_BN(serial, nullptr) : nullptr;
    if (!bn) {
        return QString();
    }
    char *hex = BN_bn2hex(bn);
    const QString result = hex ? QString::fromLatin1(hex) : QString();
    OPENSSL_free(hex);
    BN_free(bn);
    return result;
}

bool writePrivateKey(EVP_PKEY *key, const QString &path, const QByteArray &passphrase,
                     QString *error)
{
    BioPtr bio(BIO_new(BIO_s_mem()));
    if (!bio || !key) {
        setError(error, Text::tr("Cannot prepare the private key"));
        return false;
    }
    int ok = 0;
    if (passphrase.isEmpty()) {
        ok = PEM_write_bio_PrivateKey(bio.get(), key, nullptr, nullptr, 0, nullptr, nullptr);
    } else {
        ok = PEM_write_bio_PKCS8PrivateKey(bio.get(), key, EVP_aes_256_cbc(),
                                           passphrase.constData(), int(passphrase.size()),
                                           nullptr, nullptr);
    }
    if (ok != 1) {
        setError(error, Text::tr("Cannot encode the private key"));
        return false;
    }
    return writeFile(path, bioBytes(bio.get()), true, error);
}

bool writeCertificate(X509 *cert, const QString &path, QString *error)
{
    BioPtr bio(BIO_new(BIO_s_mem()));
    if (!bio || !cert || PEM_write_bio_X509(bio.get(), cert) != 1) {
        setError(error, Text::tr("Cannot encode the certificate"));
        return false;
    }
    return writeFile(path, bioBytes(bio.get()), false, error);
}

bool writePkcs12(EVP_PKEY *key, X509 *cert, X509 *caCert, const QString &path,
                 const QByteArray &password, QString *error)
{
    if (password.isEmpty()) {
        if (error) {
            *error = Text::tr("A PFX file needs a password");
        }
        return false;
    }
    if (!key || !cert) {
        if (error) {
            *error = Text::tr("A PFX file needs a certificate and its private key");
        }
        return false;
    }
    if (!keyMatchesCertificate(key, cert)) {
        if (error) {
            *error = Text::tr("The private key does not belong to the certificate");
        }
        return false;
    }

    STACK_OF(X509) *chain = nullptr;
    if (caCert) {
        chain = sk_X509_new_null();
        if (!chain || sk_X509_push(chain, caCert) == 0) {
            sk_X509_free(chain);
            setError(error, Text::tr("Cannot prepare the certificate chain"));
            return false;
        }
    }
    // nid 0 / iter 0 / mac_iter 0: OpenSSL's defaults (AES-256-CBC with
    // PBKDF2 and an SHA-256 MAC since OpenSSL 3.0)
    PKCS12 *p12 = PKCS12_create(password.constData(), "Onesimus Client Certificate",
                                key, cert, chain, 0, 0, 0, 0, 0);
    sk_X509_free(chain);   // the stack only; the certificate stays the caller's
    if (!p12) {
        setError(error, Text::tr("Cannot create the PKCS#12 structure"));
        return false;
    }

    BioPtr bio(BIO_new(BIO_s_mem()));
    const int ok = bio ? i2d_PKCS12_bio(bio.get(), p12) : 0;
    PKCS12_free(p12);
    if (ok != 1) {
        setError(error, Text::tr("Cannot encode the PFX file"));
        return false;
    }
    return writeFile(path, bioBytes(bio.get()), true, error);
}

CertPtr loadCertificate(const QByteArray &data, QString *error)
{
    BioPtr bio(BIO_new_mem_buf(data.constData(), int(data.size())));
    CertPtr cert(bio ? PEM_read_bio_X509(bio.get(), nullptr, nullptr, nullptr) : nullptr);
    if (!cert) {
        ERR_clear_error();
        bio.reset(BIO_new_mem_buf(data.constData(), int(data.size())));
        cert.reset(bio ? d2i_X509_bio(bio.get(), nullptr) : nullptr);
    }
    if (!cert) {
        setError(error, Text::tr("Not a PEM or DER certificate"));
    }
    return cert;
}

namespace {
// An encrypted PEM key must fail instead of OpenSSL prompting on the terminal
int noPassphrase(char *, int, int, void *)
{
    return 0;
}
} // namespace

KeyPtr loadPrivateKey(const QByteArray &data, QString *error)
{
    BioPtr bio(BIO_new_mem_buf(data.constData(), int(data.size())));
    KeyPtr key(bio ? PEM_read_bio_PrivateKey(bio.get(), nullptr, noPassphrase, nullptr) : nullptr);
    if (!key) {
        ERR_clear_error();
        bio.reset(BIO_new_mem_buf(data.constData(), int(data.size())));
        key.reset(bio ? d2i_PrivateKey_bio(bio.get(), nullptr) : nullptr);
    }
    if (!key) {
        if (data.contains("ENCRYPTED")) {
            ERR_clear_error();
            if (error) {
                *error = Text::tr("The private key is encrypted; decrypt it first (openssl pkey -in key.pem -out plain.pem)");
            }
        } else {
            setError(error, Text::tr("Not a PEM or DER private key"));
        }
    }
    return key;
}

bool keyMatchesCertificate(EVP_PKEY *key, X509 *cert)
{
    if (!key || !cert) {
        return false;
    }
    const bool ok = X509_check_private_key(cert, key) == 1;
    ERR_clear_error();
    return ok;
}

bool readPkcs12(const QString &path, const QByteArray &password, KeyPtr *key,
                CertPtr *cert, int *caCount, QString *error)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        if (error) {
            *error = Text::tr("Cannot open %1: %2").arg(path, file.errorString());
        }
        return false;
    }
    const QByteArray data = file.readAll();
    BioPtr bio(BIO_new_mem_buf(data.constData(), int(data.size())));
    PKCS12 *p12 = bio ? d2i_PKCS12_bio(bio.get(), nullptr) : nullptr;
    if (!p12) {
        setError(error, Text::tr("%1 is not a PKCS#12 file").arg(path));
        return false;
    }
    EVP_PKEY *k = nullptr;
    X509 *c = nullptr;
    STACK_OF(X509) *ca = nullptr;
    const int ok = PKCS12_parse(p12, password.constData(), &k, &c, &ca);
    PKCS12_free(p12);
    if (ok != 1) {
        setError(error, Text::tr("Cannot open %1 with this password").arg(path));
        return false;
    }
    if (caCount) {
        *caCount = ca ? sk_X509_num(ca) : 0;
    }
    sk_X509_pop_free(ca, X509_free);
    if (key) key->reset(k); else EVP_PKEY_free(k);
    if (cert) cert->reset(c); else X509_free(c);
    return true;
}

bool convertToPkcs12(const QByteArray &certData, const QByteArray &keyData,
                     const QByteArray &caData, const QString &path,
                     const QByteArray &password, QString *error)
{
    QString why;
    CertPtr cert = loadCertificate(certData, &why);
    if (!cert) {
        if (error) *error = Text::tr("Certificate: %1").arg(why);
        return false;
    }
    KeyPtr key = loadPrivateKey(keyData, &why);
    if (!key) {
        if (error) *error = Text::tr("Private key: %1").arg(why);
        return false;
    }
    CertPtr ca;
    if (!caData.trimmed().isEmpty()) {
        ca = loadCertificate(caData, &why);
        if (!ca) {
            if (error) *error = Text::tr("CA certificate: %1").arg(why);
            return false;
        }
    }
    if (!writePkcs12(key.get(), cert.get(), ca.get(), path, password, error)) {
        return false;
    }
    KeyPtr readKey;
    CertPtr readCert;
    if (!readPkcs12(path, password, &readKey, &readCert, nullptr, error)
        || !keyMatchesCertificate(readKey.get(), readCert.get())) {
        if (error && error->isEmpty()) {
            *error = Text::tr("The written PFX file cannot be read back");
        }
        QFile::remove(path);
        return false;
    }
    return true;
}

QString commonName(X509 *cert)
{
    if (!cert) {
        return QString();
    }
    char buf[256] = {};
    const int len = X509_NAME_get_text_by_NID(X509_get_subject_name(cert), NID_commonName,
                                              buf, sizeof(buf));
    return len > 0 ? QString::fromUtf8(buf, len) : QString();
}

QStringList extensionTexts(X509 *cert)
{
    QStringList result;
    if (!cert) {
        return result;
    }
    for (int i = 0; i < X509_get_ext_count(cert); ++i) {
        X509_EXTENSION *ext = X509_get_ext(cert, i);
        const int nid = OBJ_obj2nid(X509_EXTENSION_get_object(ext));
        BioPtr bio(BIO_new(BIO_s_mem()));
        if (!bio) {
            continue;
        }
        if (X509V3_EXT_print(bio.get(), ext, 0, 0) != 1) {
            ERR_clear_error();
        }
        QString value = QString::fromUtf8(bioBytes(bio.get())).trimmed();
        value.replace(QStringLiteral("\n"), QStringLiteral(" "));
        result << QStringLiteral("%1%2: %3")
                      .arg(QString::fromLatin1(OBJ_nid2sn(nid)),
                           X509_EXTENSION_get_critical(ext) ? QStringLiteral(" (critical)") : QString(),
                           value);
    }
    return result;
}

} // namespace BPki
