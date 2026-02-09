#ifndef BRESOURCEMODELS_H
#define BRESOURCEMODELS_H

#include "models/bbasemodels.h"
#include <QMap>
#include <QJsonObject>

// ============================================================================
// BFilesetModel - Model for Bareos filesets
// ============================================================================

/**
 * @brief List model for displaying Bareos filesets with full config caching
 *
 * Stores fileset names from .filesets dot-command AND full resource configs
 * from "show filesets" for use in the Edit FileSet wizard.
 */
class BFilesetModel : public BListModel
{
    Q_OBJECT

public:
    explicit BFilesetModel(QObject *parent = nullptr);

    void parseFilesets(const QString &jsonResponse);
    void parseShowFilesets(const QString &response);

    QStringList filesetNames() const;
    QJsonObject filesetConfig(const QString &name) const;
    bool hasFilesetConfigs() const { return !m_filesetConfigs.isEmpty(); }

protected:
    QString getDisplayText(const QJsonObject &item) const override;

private:
    QMap<QString, QJsonObject> m_filesetConfigs;
};


// ============================================================================
// BStorageModel - Model for Bareos storages
// ============================================================================

/**
 * @brief List model for displaying Bareos storage daemons
 * @version 1.0
 * @since 2026-01-29
 *
 * Displays storage information from .storages dot-command response.
 * Inherits from BListModel for basic JSON handling.
 */
class BStorageModel : public BListModel
{
    Q_OBJECT

public:
    explicit BStorageModel(QObject *parent = nullptr);

    /**
     * @brief Parses .storages dot-command response
     * @param jsonResponse JSON response from Director
     */
    void parseStorages(const QString &jsonResponse);

    /**
     * @brief Returns storage names as string list
     * @return QStringList of storage names
     */
    QStringList storageNames() const;

protected:
    QString getDisplayText(const QJsonObject &item) const override;
};


// ============================================================================
// BPoolModel - Model for Bareos pools
// ============================================================================

/**
 * @brief List model for displaying Bareos pools
 * @version 1.0
 * @since 2026-01-29
 *
 * Displays pool information from .pools dot-command response.
 * Inherits from BListModel for basic JSON handling.
 */
class BPoolModel : public BListModel
{
    Q_OBJECT

public:
    explicit BPoolModel(QObject *parent = nullptr);

    /**
     * @brief Parses .pools dot-command response
     * @param jsonResponse JSON response from Director
     */
    void parsePools(const QString &jsonResponse);

    /**
     * @brief Returns pool names as string list
     * @return QStringList of pool names
     */
    QStringList poolNames() const;

    /**
     * @brief Returns detailed pool information for a specific pool
     * @param poolName Name of the pool
     * @return QJsonObject containing pool details
     */
    QJsonObject poolInfo(const QString &poolName) const;

protected:
    QString getDisplayText(const QJsonObject &item) const override;
};


// ============================================================================
// BLevelModel - Model for Bareos backup levels
// ============================================================================

/**
 * @brief List model for displaying Bareos backup levels
 * @version 1.0
 * @since 2026-01-29
 *
 * Displays level information from .levels dot-command response.
 * Levels: Full (F), Incremental (I), Differential (D), VirtualFull (V)
 */
class BLevelModel : public BListModel
{
    Q_OBJECT

public:
    explicit BLevelModel(QObject *parent = nullptr);

    /**
     * @brief Parses .levels dot-command response
     * @param jsonResponse JSON response from Director
     */
    void parseLevels(const QString &jsonResponse);

    /**
     * @brief Returns level codes as string list (F, I, D, V)
     * @return QStringList of level codes
     */
    QStringList levelCodes() const;

    /**
     * @brief Returns level descriptions as string list
     * @return QStringList of level descriptions
     */
    QStringList levelDescriptions() const;

protected:
    QString getDisplayText(const QJsonObject &item) const override;
};

#endif // BRESOURCEMODELS_H
