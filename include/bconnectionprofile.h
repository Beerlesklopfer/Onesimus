#ifndef BCONNECTIONPROFILE_H
#define BCONNECTIONPROFILE_H

#include <QString>
#include <QUuid>
#include <QJsonObject>

/**
 * @brief Connection profile for Bareos/Bacula Directors
 *
 * Encapsulates all connection parameters for a single Director connection.
 * Profiles can be serialized to/from JSON for storage in QSettings.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */
struct BConnectionProfile
{
    // Unique identifier
    QString id;             ///< UUID for the profile

    // Display name
    QString name;           ///< User-friendly name (e.g., "Production Bareos", "Test Server")

    // Connection settings
    QString host;           ///< Hostname or IP address
    int port = 9101;        ///< Port number (default: 9101)
    QString directorName;   ///< Director resource name
    QString consoleName;    ///< Console resource name
    QString password;       ///< Password (stored encrypted if possible)

    // Authentication method
    bool legacyAuth = false; ///< Use Legacy Auth (CRAM-MD5 without TLS)
    bool tlsEnabled = true;  ///< Enable TLS encryption
    bool tlsUsePSK = true;   ///< Use PSK (true) or Certificate (false)

    // TLS Certificate settings (for certificate-based auth)
    QString tlsCaCertFile;   ///< Path to CA certificate
    QString tlsCertFile;     ///< Path to client certificate
    QString tlsKeyFile;      ///< Path to private key
    QString tlsPfxFile;      ///< Path to PFX file (Windows)
    bool tlsVerifyPeer = true; ///< Verify server certificate

    /**
     * @brief Create a new profile with a generated UUID
     */
    static BConnectionProfile create(const QString &profileName = QString())
    {
        BConnectionProfile profile;
        profile.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        profile.name = profileName.isEmpty() ? QObject::tr("New Connection") : profileName;
        profile.host = "localhost";
        profile.port = 9101;
        profile.directorName = "bareos-dir";
        profile.consoleName = "onesimus";
        return profile;
    }

    /**
     * @brief Serialize profile to JSON
     */
    QJsonObject toJson() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["host"] = host;
        obj["port"] = port;
        obj["directorName"] = directorName;
        obj["consoleName"] = consoleName;
        obj["password"] = password;  // TODO: Encrypt password
        obj["legacyAuth"] = legacyAuth;
        obj["tlsEnabled"] = tlsEnabled;
        obj["tlsUsePSK"] = tlsUsePSK;
        obj["tlsCaCertFile"] = tlsCaCertFile;
        obj["tlsCertFile"] = tlsCertFile;
        obj["tlsKeyFile"] = tlsKeyFile;
        obj["tlsPfxFile"] = tlsPfxFile;
        obj["tlsVerifyPeer"] = tlsVerifyPeer;
        return obj;
    }

    /**
     * @brief Deserialize profile from JSON
     */
    static BConnectionProfile fromJson(const QJsonObject &obj)
    {
        BConnectionProfile profile;
        profile.id = obj["id"].toString();
        profile.name = obj["name"].toString();
        profile.host = obj["host"].toString();
        profile.port = obj["port"].toInt(9101);
        profile.directorName = obj["directorName"].toString();
        profile.consoleName = obj["consoleName"].toString();
        profile.password = obj["password"].toString();  // TODO: Decrypt password
        profile.legacyAuth = obj["legacyAuth"].toBool(false);
        profile.tlsEnabled = obj["tlsEnabled"].toBool(true);
        profile.tlsUsePSK = obj["tlsUsePSK"].toBool(true);
        profile.tlsCaCertFile = obj["tlsCaCertFile"].toString();
        profile.tlsCertFile = obj["tlsCertFile"].toString();
        profile.tlsKeyFile = obj["tlsKeyFile"].toString();
        profile.tlsPfxFile = obj["tlsPfxFile"].toString();
        profile.tlsVerifyPeer = obj["tlsVerifyPeer"].toBool(true);
        return profile;
    }

    /**
     * @brief Get display string for ComboBox
     */
    QString displayName() const
    {
        if (name.isEmpty()) {
            return QString("%1:%2").arg(host).arg(port);
        }
        return QString("%1 (%2:%3)").arg(name, host).arg(port);
    }

    /**
     * @brief Check if profile is valid (has required fields)
     */
    bool isValid() const
    {
        return !id.isEmpty() && !host.isEmpty() && !directorName.isEmpty();
    }
};

#endif // BCONNECTIONPROFILE_H
