#ifndef BCONNECTIONPROFILE_H
#define BCONNECTIONPROFILE_H

#include <QString>
#include <QStringList>
#include <QUuid>
#include <QJsonObject>
#include <QJsonArray>

/**
 * @brief Connection profile for Bareos/Bacula Directors
 *
 * Encapsulates all connection parameters for a single Director connection.
 * Profiles can be serialized to/from JSON for storage in QSettings.
 *
 * Based on Bareos Console Configuration:
 * https://docs.bareos.org/Configuration/Console.html
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
    QString description;    ///< Optional description of the connection

    // Connection settings
    QString host;           ///< Hostname or IP address
    int port = 9101;        ///< Port number (default: 9101)
    QString directorName;   ///< Director resource name
    QString consoleName;    ///< Console resource name
    QString password;       ///< Password (stored encrypted if possible)
    int heartbeatInterval = 0; ///< Keepalive interval in seconds (0 = disabled)

    // Authentication method
    bool legacyAuth = false; ///< Use Legacy Auth (CRAM-MD5 without TLS)
    bool tlsEnabled = true;  ///< Enable TLS encryption (TLS Enable)
    bool tlsUsePSK = true;   ///< Use PSK (true) or Certificate (false)

    // TLS Certificate settings (for certificate-based auth)
    QString tlsCaCertFile;   ///< Path to CA certificate file
    QString tlsCaCertDir;    ///< Path to CA certificate directory
    QString tlsCertFile;     ///< Path to client certificate
    QString tlsKeyFile;      ///< Path to private key
    QString tlsPfxFile;      ///< Path to PFX file (Windows)
    bool tlsVerifyPeer = true;  ///< Verify server certificate (TLS Verify Peer)
    bool tlsRequire = true;     ///< Require TLS connection (TLS Require)
    bool tlsAuthenticate = false; ///< TLS for auth only, not encryption (TLS Authenticate)

    // Advanced TLS settings
    QString tlsCipherList;      ///< TLSv1.2 cipher list (colon-separated)
    QString tlsCipherSuites;    ///< TLSv1.3 cipher suites (colon-separated)
    QString tlsDhFile;          ///< Path to Diffie-Hellman parameters file
    QString tlsProtocol;        ///< OpenSSL protocol configuration
    QStringList tlsAllowedCn;   ///< Allowed certificate Common Names
    QString tlsCrlFile;         ///< Certificate Revocation List file

    // Console ACLs (Access Control Lists)
    // These define what resources this console can access on the Director
    QStringList aclJob;         ///< Job ACL - allowed job names (regex)
    QStringList aclClient;      ///< Client ACL - allowed client names
    QStringList aclStorage;     ///< Storage ACL - allowed storage names
    QStringList aclSchedule;    ///< Schedule ACL - allowed schedule names
    QStringList aclPool;        ///< Pool ACL - allowed pool names
    QStringList aclFileSet;     ///< FileSet ACL - allowed fileset names
    QStringList aclCatalog;     ///< Catalog ACL - allowed catalog names
    QStringList aclCommand;     ///< Command ACL - allowed commands
    QStringList aclWhere;       ///< Where ACL - allowed restore locations
    QStringList aclPluginOptions; ///< Plugin Options ACL

    // Profile (references a Director Profile resource)
    QString profile;            ///< Director Profile name (contains ACLs)

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
        obj["description"] = description;
        obj["host"] = host;
        obj["port"] = port;
        obj["directorName"] = directorName;
        obj["consoleName"] = consoleName;
        obj["password"] = password;  // TODO: Encrypt password
        obj["heartbeatInterval"] = heartbeatInterval;
        obj["legacyAuth"] = legacyAuth;
        obj["tlsEnabled"] = tlsEnabled;
        obj["tlsUsePSK"] = tlsUsePSK;
        obj["tlsCaCertFile"] = tlsCaCertFile;
        obj["tlsCaCertDir"] = tlsCaCertDir;
        obj["tlsCertFile"] = tlsCertFile;
        obj["tlsKeyFile"] = tlsKeyFile;
        obj["tlsPfxFile"] = tlsPfxFile;
        obj["tlsVerifyPeer"] = tlsVerifyPeer;
        obj["tlsRequire"] = tlsRequire;
        obj["tlsAuthenticate"] = tlsAuthenticate;
        obj["tlsCipherList"] = tlsCipherList;
        obj["tlsCipherSuites"] = tlsCipherSuites;
        obj["tlsDhFile"] = tlsDhFile;
        obj["tlsProtocol"] = tlsProtocol;
        obj["tlsAllowedCn"] = QJsonArray::fromStringList(tlsAllowedCn);
        obj["tlsCrlFile"] = tlsCrlFile;

        // ACLs
        obj["aclJob"] = QJsonArray::fromStringList(aclJob);
        obj["aclClient"] = QJsonArray::fromStringList(aclClient);
        obj["aclStorage"] = QJsonArray::fromStringList(aclStorage);
        obj["aclSchedule"] = QJsonArray::fromStringList(aclSchedule);
        obj["aclPool"] = QJsonArray::fromStringList(aclPool);
        obj["aclFileSet"] = QJsonArray::fromStringList(aclFileSet);
        obj["aclCatalog"] = QJsonArray::fromStringList(aclCatalog);
        obj["aclCommand"] = QJsonArray::fromStringList(aclCommand);
        obj["aclWhere"] = QJsonArray::fromStringList(aclWhere);
        obj["aclPluginOptions"] = QJsonArray::fromStringList(aclPluginOptions);
        obj["profile"] = profile;

        return obj;
    }

    /**
     * @brief Helper to convert QJsonArray to QStringList
     */
    static QStringList jsonArrayToStringList(const QJsonArray &arr)
    {
        QStringList list;
        for (const QJsonValue &v : arr) {
            list.append(v.toString());
        }
        return list;
    }

    /**
     * @brief Deserialize profile from JSON
     */
    static BConnectionProfile fromJson(const QJsonObject &obj)
    {
        BConnectionProfile profile;
        profile.id = obj["id"].toString();
        profile.name = obj["name"].toString();
        profile.description = obj["description"].toString();
        profile.host = obj["host"].toString();
        profile.port = obj["port"].toInt(9101);
        profile.directorName = obj["directorName"].toString();
        profile.consoleName = obj["consoleName"].toString();
        profile.password = obj["password"].toString();  // TODO: Decrypt password
        profile.heartbeatInterval = obj["heartbeatInterval"].toInt(0);
        profile.legacyAuth = obj["legacyAuth"].toBool(false);
        profile.tlsEnabled = obj["tlsEnabled"].toBool(true);
        profile.tlsUsePSK = obj["tlsUsePSK"].toBool(true);
        profile.tlsCaCertFile = obj["tlsCaCertFile"].toString();
        profile.tlsCaCertDir = obj["tlsCaCertDir"].toString();
        profile.tlsCertFile = obj["tlsCertFile"].toString();
        profile.tlsKeyFile = obj["tlsKeyFile"].toString();
        profile.tlsPfxFile = obj["tlsPfxFile"].toString();
        profile.tlsVerifyPeer = obj["tlsVerifyPeer"].toBool(true);
        profile.tlsRequire = obj["tlsRequire"].toBool(true);
        profile.tlsAuthenticate = obj["tlsAuthenticate"].toBool(false);
        profile.tlsCipherList = obj["tlsCipherList"].toString();
        profile.tlsCipherSuites = obj["tlsCipherSuites"].toString();
        profile.tlsDhFile = obj["tlsDhFile"].toString();
        profile.tlsProtocol = obj["tlsProtocol"].toString();
        profile.tlsAllowedCn = jsonArrayToStringList(obj["tlsAllowedCn"].toArray());
        profile.tlsCrlFile = obj["tlsCrlFile"].toString();

        // ACLs
        profile.aclJob = jsonArrayToStringList(obj["aclJob"].toArray());
        profile.aclClient = jsonArrayToStringList(obj["aclClient"].toArray());
        profile.aclStorage = jsonArrayToStringList(obj["aclStorage"].toArray());
        profile.aclSchedule = jsonArrayToStringList(obj["aclSchedule"].toArray());
        profile.aclPool = jsonArrayToStringList(obj["aclPool"].toArray());
        profile.aclFileSet = jsonArrayToStringList(obj["aclFileSet"].toArray());
        profile.aclCatalog = jsonArrayToStringList(obj["aclCatalog"].toArray());
        profile.aclCommand = jsonArrayToStringList(obj["aclCommand"].toArray());
        profile.aclWhere = jsonArrayToStringList(obj["aclWhere"].toArray());
        profile.aclPluginOptions = jsonArrayToStringList(obj["aclPluginOptions"].toArray());
        profile.profile = obj["profile"].toString();

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

    // ========================================================================
    // Bareos Configuration Export Methods
    // ========================================================================

    /**
     * @brief Generate Console resource configuration
     * @return Bareos Console resource config string
     *
     * Output path: etc/bareos/bareos-dir.d/console/<consoleName>.conf
     */
    QString toConsoleConfig() const
    {
        QString config;
        config += QString("Console {\n");
        config += QString("  Name = \"%1\"\n").arg(consoleName);
        config += QString("  Password = \"%1\"\n").arg(password);

        if (!description.isEmpty()) {
            config += QString("  Description = \"%1\"\n").arg(description);
        }

        // TLS settings
        // Note: In Bareos 18.2+, PSK is the default when TLS is enabled.
        // "TLS PSK Enable" directive is deprecated/removed.
        if (tlsEnabled) {
            config += QString("  TLS Enable = yes\n");

            if (!tlsUsePSK) {
                // Certificate mode - need to specify certificates
                if (!tlsCaCertFile.isEmpty())
                    config += QString("  TLS CA Certificate File = \"%1\"\n").arg(tlsCaCertFile);
                if (!tlsCaCertDir.isEmpty())
                    config += QString("  TLS CA Certificate Dir = \"%1\"\n").arg(tlsCaCertDir);
                if (!tlsCertFile.isEmpty())
                    config += QString("  TLS Certificate = \"%1\"\n").arg(tlsCertFile);
                if (!tlsKeyFile.isEmpty())
                    config += QString("  TLS Key = \"%1\"\n").arg(tlsKeyFile);
                config += QString("  TLS Verify Peer = %1\n").arg(tlsVerifyPeer ? "yes" : "no");
            }
            // PSK mode: TLS Enable = yes is sufficient, PSK is default

            if (tlsAuthenticate)
                config += QString("  TLS Authenticate = yes\n");
            if (!tlsCipherList.isEmpty())
                config += QString("  TLS Cipher List = \"%1\"\n").arg(tlsCipherList);
            if (!tlsCipherSuites.isEmpty())
                config += QString("  TLS Cipher Suites = \"%1\"\n").arg(tlsCipherSuites);
            if (!tlsDhFile.isEmpty())
                config += QString("  TLS DH File = \"%1\"\n").arg(tlsDhFile);
            if (!tlsProtocol.isEmpty())
                config += QString("  TLS Protocol = \"%1\"\n").arg(tlsProtocol);
            for (const QString &cn : tlsAllowedCn)
                config += QString("  TLS Allowed CN = \"%1\"\n").arg(cn);
            if (!tlsCrlFile.isEmpty())
                config += QString("  TLS Certificate Revocation List = \"%1\"\n").arg(tlsCrlFile);
        } else {
            config += QString("  TLS Enable = no\n");
        }

        // Profile reference (server-side ACLs)
        if (!profile.isEmpty()) {
            config += QString("  Profile = \"%1\"\n").arg(profile);
        }

        // Direct ACLs (only if no Profile is referenced)
        if (profile.isEmpty()) {
            auto writeAcl = [&config](const QString &name, const QStringList &values) {
                for (const QString &v : values)
                    config += QString("  %1 = \"%2\"\n").arg(name, v);
            };

            writeAcl("Job ACL", aclJob);
            writeAcl("Client ACL", aclClient);
            writeAcl("Storage ACL", aclStorage);
            writeAcl("Schedule ACL", aclSchedule);
            writeAcl("Pool ACL", aclPool);
            writeAcl("FileSet ACL", aclFileSet);
            writeAcl("Catalog ACL", aclCatalog);
            writeAcl("Command ACL", aclCommand);
            writeAcl("Where ACL", aclWhere);
            writeAcl("PluginOptions ACL", aclPluginOptions);
        }

        config += QString("}\n");
        return config;
    }

    /**
     * @brief Generate Profile resource configuration
     * @return Bareos Profile resource config string (empty if no ACLs defined)
     *
     * Output path: etc/bareos/bareos-dir.d/profile/<profileName>.conf
     */
    QString toProfileConfig() const
    {
        // Check if any ACLs are defined
        bool hasAcls = !aclJob.isEmpty() || !aclClient.isEmpty() || !aclStorage.isEmpty() ||
                       !aclSchedule.isEmpty() || !aclPool.isEmpty() || !aclFileSet.isEmpty() ||
                       !aclCatalog.isEmpty() || !aclCommand.isEmpty() || !aclWhere.isEmpty() ||
                       !aclPluginOptions.isEmpty();

        if (!hasAcls) return QString();

        QString profileName = this->profile.isEmpty() ? consoleName + "-profile" : this->profile;

        QString config;
        config += QString("Profile {\n");
        config += QString("  Name = \"%1\"\n").arg(profileName);

        if (!description.isEmpty()) {
            config += QString("  Description = \"%1\"\n").arg(description);
        }

        auto writeAcl = [&config](const QString &name, const QStringList &values) {
            for (const QString &v : values)
                config += QString("  %1 = \"%2\"\n").arg(name, v);
        };

        writeAcl("Job ACL", aclJob);
        writeAcl("Client ACL", aclClient);
        writeAcl("Storage ACL", aclStorage);
        writeAcl("Schedule ACL", aclSchedule);
        writeAcl("Pool ACL", aclPool);
        writeAcl("FileSet ACL", aclFileSet);
        writeAcl("Catalog ACL", aclCatalog);
        writeAcl("Command ACL", aclCommand);
        writeAcl("Where ACL", aclWhere);
        writeAcl("PluginOptions ACL", aclPluginOptions);

        config += QString("}\n");
        return config;
    }

    /**
     * @brief Generate Director resource configuration template
     * @return Bareos Director resource config string
     *
     * Output path: etc/bareos/bareos-dir.d/director/bareos-dir.conf
     * Note: This is a template - actual Director config may need additional customization
     */
    QString toDirectorConfig() const
    {
        QString config;
        config += QString("Director {\n");
        config += QString("  Name = \"%1\"\n").arg(directorName);
        config += QString("  QueryFile = \"/usr/lib/bareos/scripts/query.sql\"\n");
        config += QString("  Maximum Concurrent Jobs = 10\n");
        config += QString("  Password = \"CHANGE_ME\"\n");
        config += QString("  Messages = \"Daemon\"\n");
        config += QString("  Auditing = yes\n");

        // TLS settings for Director
        // Note: In Bareos 18.2+, PSK is the default when TLS is enabled
        if (tlsEnabled) {
            config += QString("  TLS Enable = yes\n");

            if (!tlsUsePSK) {
                // Certificate mode
                config += QString("  # TLS Certificate paths - adjust as needed\n");
                config += QString("  # TLS CA Certificate File = \"/etc/bareos/tls/ca.pem\"\n");
                config += QString("  # TLS Certificate = \"/etc/bareos/tls/bareos-dir.pem\"\n");
                config += QString("  # TLS Key = \"/etc/bareos/tls/bareos-dir-key.pem\"\n");
            }
            // PSK mode: TLS Enable = yes is sufficient
        }

        if (heartbeatInterval > 0) {
            config += QString("  Heartbeat Interval = %1\n").arg(heartbeatInterval);
        }

        config += QString("}\n");
        return config;
    }

    /**
     * @brief Generate bconsole.conf for client-side configuration
     * @return Bareos bconsole configuration string
     *
     * This is the configuration for the Onesimus client to connect to the Director.
     */
    QString toBconsoleConfig() const
    {
        QString config;
        config += QString("Director {\n");
        config += QString("  Name = \"%1\"\n").arg(directorName);
        config += QString("  DIRport = %1\n").arg(port);
        config += QString("  Address = \"%1\"\n").arg(host);
        config += QString("  Password = \"%1\"\n").arg(password);

        if (tlsEnabled) {
            config += QString("  TLS Enable = yes\n");
            config += QString("  TLS Require = %1\n").arg(tlsRequire ? "yes" : "no");

            if (tlsUsePSK) {
                config += QString("  TLS PSK Enable = yes\n");
            } else {
                if (!tlsCaCertFile.isEmpty())
                    config += QString("  TLS CA Certificate File = \"%1\"\n").arg(tlsCaCertFile);
                if (!tlsCertFile.isEmpty())
                    config += QString("  TLS Certificate = \"%1\"\n").arg(tlsCertFile);
                if (!tlsKeyFile.isEmpty())
                    config += QString("  TLS Key = \"%1\"\n").arg(tlsKeyFile);
            }
        }

        if (heartbeatInterval > 0) {
            config += QString("  Heartbeat Interval = %1\n").arg(heartbeatInterval);
        }

        config += QString("}\n\n");

        config += QString("Console {\n");
        config += QString("  Name = \"%1\"\n").arg(consoleName);
        config += QString("  Password = \"%1\"\n").arg(password);
        config += QString("}\n");

        return config;
    }

    /**
     * @brief Check if this profile has any ACLs defined
     * @return true if any ACL list is non-empty
     */
    bool hasAcls() const
    {
        return !aclJob.isEmpty() || !aclClient.isEmpty() || !aclStorage.isEmpty() ||
               !aclSchedule.isEmpty() || !aclPool.isEmpty() || !aclFileSet.isEmpty() ||
               !aclCatalog.isEmpty() || !aclCommand.isEmpty() || !aclWhere.isEmpty() ||
               !aclPluginOptions.isEmpty();
    }
};

#endif // BCONNECTIONPROFILE_H
