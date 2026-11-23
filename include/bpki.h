// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bpki.h
 * @brief Keys, certificates and PKCS#12 files for the certificate generator
 *        and the PFX converter, without widgets
 *
 * Every file that holds a private key (PEM key, PFX) is created with owner-only
 * permissions before anything is written to it; OpenSSL only ever writes into
 * a memory BIO. The functions report failures through @p error and never
 * claim success without a file on disk.
 */
#ifndef BPKI_H
#define BPKI_H

#include <QByteArray>
#include <QString>
#include <QStringList>

#include <memory>

typedef struct evp_pkey_st EVP_PKEY;
typedef struct x509_st X509;

namespace BPki {

struct KeyDeleter  { void operator()(EVP_PKEY *key) const; };
struct CertDeleter { void operator()(X509 *cert) const; };
using KeyPtr  = std::unique_ptr<EVP_PKEY, KeyDeleter>;
using CertPtr = std::unique_ptr<X509, CertDeleter>;

/// What goes into a certificate
struct CertificateRequest {
    QString commonName;
    QString organization;
    int validDays = 3650;
    /// true: CA certificate (CA:TRUE, keyCertSign); false: TLS client
    /// certificate (CA:FALSE, digitalSignature/keyEncipherment, clientAuth,
    /// the common name as DNS subject alternative name when it is a host name)
    bool isCa = false;
};

/// A new RSA key, or null with @p error set
KeyPtr generateRsaKey(int bits, QString *error);

/**
 * @brief A certificate for @p key, signed by @p issuerKey (or self-signed when
 *        @p issuerCert / @p issuerKey are null), with a random 159-bit serial
 */
CertPtr makeCertificate(EVP_PKEY *key, const CertificateRequest &request,
                        X509 *issuerCert, EVP_PKEY *issuerKey, QString *error);

/// The serial number of @p cert as upper-case hex (tests, log)
QString serialHex(X509 *cert);

/// true when @p name may stand as a DNS subject alternative name
bool isDnsName(const QString &name);

/**
 * @brief Write @p key as PEM to @p path with owner-only permissions
 *
 * With a non-empty @p passphrase the key is stored as encrypted PKCS#8
 * (AES-256-CBC); without one it is stored unencrypted, which the TLS client
 * needs for a key it loads without asking.
 */
bool writePrivateKey(EVP_PKEY *key, const QString &path, const QByteArray &passphrase,
                     QString *error);

/// Write @p cert as PEM to @p path (a certificate is public)
bool writeCertificate(X509 *cert, const QString &path, QString *error);

/**
 * @brief Write @p key and @p cert (and @p caCert, if given, as chain) as a
 *        password-protected PKCS#12 file with owner-only permissions
 *
 * An empty @p password is refused: a PFX without a password leaves the
 * private key readable for anyone who gets the file.
 */
bool writePkcs12(EVP_PKEY *key, X509 *cert, X509 *caCert, const QString &path,
                 const QByteArray &password, QString *error);

/**
 * @brief The PFX converter's work: certificate, key and optional CA
 *        certificate (PEM or DER data) into a password-protected PKCS#12 file
 *
 * Checks that the key belongs to the certificate and reads the written file
 * back with @p password before it reports success.
 */
bool convertToPkcs12(const QByteArray &certData, const QByteArray &keyData,
                     const QByteArray &caData, const QString &path,
                     const QByteArray &password, QString *error);

/// A certificate from PEM or DER data
CertPtr loadCertificate(const QByteArray &data, QString *error);

/// An unencrypted private key from PEM or DER data
KeyPtr loadPrivateKey(const QByteArray &data, QString *error);

/// true when @p key belongs to the public key in @p cert
bool keyMatchesCertificate(EVP_PKEY *key, X509 *cert);

/**
 * @brief Read a PKCS#12 file back (tests, and to check a written file)
 * @param caCount set to the number of chain certificates, if not null
 */
bool readPkcs12(const QString &path, const QByteArray &password, KeyPtr *key,
                CertPtr *cert, int *caCount, QString *error);

/// The common name of @p cert
QString commonName(X509 *cert);

/// The extensions of @p cert as OpenSSL prints them, one "name: value" per entry
QStringList extensionTexts(X509 *cert);

} // namespace BPki

#endif // BPKI_H
