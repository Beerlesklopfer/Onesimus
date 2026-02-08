#ifndef BCOLUMNCONFIGURATION_H
#define BCOLUMNCONFIGURATION_H

#include <QObject>
#include <QSettings>
#include <QHeaderView>
#include <QMap>
#include <QString>

/**
 * @brief Manages column configurations (width, order, visibility, presets)
 * @version 1.0
 * @since 2026-01-26
 * 
 * Features:
 * - Save/Load column widths
 * - Save/Load column order
 * - Save/Load column visibility
 * - Manage multiple presets (views)
 */
class BColumnConfiguration : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Column configuration structure
     */
    struct ColumnConfig {
        int visualIndex;        ///< Visual index (position)
        int width;              ///< Column width in pixels
        bool visible;           ///< Visibility state
    };

    /**
     * @brief Preset (complete view configuration)
     */
    struct Preset {
        QString name;                           ///< Preset name
        QString description;                    ///< Preset description
        QMap<int, ColumnConfig> columns;        ///< Column configurations
    };

    explicit BColumnConfiguration(QObject *parent = nullptr);
    
    /**
     * @brief Saves current header configuration
     * @param header Header view to save
     * @param settingsKey Settings key (e.g., "MainWindow/Columns")
     * @since 1.0
     */
    void saveHeaderState(QHeaderView *header, const QString &settingsKey = "Columns");
    
    /**
     * @brief Restores header configuration
     * @param header Header view to restore
     * @param settingsKey Settings key
     * @since 1.0
     */
    void restoreHeaderState(QHeaderView *header, const QString &settingsKey = "Columns");
    
    /**
     * @brief Saves current configuration as a preset
     * @param header Header view
     * @param presetName Preset name
     * @param description Preset description
     * @since 1.0
     */
    void savePreset(QHeaderView *header, const QString &presetName, const QString &description = "");
    
    /**
     * @brief Loads a preset
     * @param header Header view
     * @param presetName Preset name
     * @return True if preset was found and loaded
     * @since 1.0
     */
    bool loadPreset(QHeaderView *header, const QString &presetName);
    
    /**
     * @brief Deletes a preset
     * @param presetName Preset name
     * @since 1.0
     */
    void deletePreset(const QString &presetName);
    
    /**
     * @brief Gets list of available presets
     * @return List of preset names
     * @since 1.0
     */
    QStringList availablePresets() const;
    
    /**
     * @brief Gets preset information
     * @param presetName Preset name
     * @return Preset structure
     * @since 1.0
     */
    Preset getPreset(const QString &presetName) const;
    
    /**
     * @brief Resets to default configuration
     * @param header Header view
     * @since 1.0
     */
    void resetToDefault(QHeaderView *header);

signals:
    /**
     * @brief Emitted when configuration changes
     * @since 1.0
     */
    void configurationChanged();
    
    /**
     * @brief Emitted when a preset is loaded
     * @param presetName Preset name
     * @since 1.0
     */
    void presetLoaded(const QString &presetName);

private:
    /**
     * @brief Reads configuration from header
     * @param header Header view
     * @return Column configuration map
     * @since 1.0
     */
    QMap<int, ColumnConfig> readConfiguration(QHeaderView *header) const;
    
    /**
     * @brief Applies configuration to header
     * @param header Header view
     * @param config Configuration to apply
     * @since 1.0
     */
    void applyConfiguration(QHeaderView *header, const QMap<int, ColumnConfig> &config);

private:
    QSettings m_settings;       ///< Settings storage
};

#endif // BCOLUMNCONFIGURATION_H
