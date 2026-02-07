#ifndef BVIEWPRESETS_H
#define BVIEWPRESETS_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include <QSettings>

/**
 * @brief Manages table view presets (column widths, order, visibility)
 * @version 1.0
 * @since 2026-01-26
 * 
 * Allows saving and loading different view configurations:
 * - Column widths
 * - Column order
 * - Column visibility
 * - Sort column and order
 */
class BViewPresets : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Preset configuration structure
     */
    struct Preset {
        QString name;                   ///< Preset name
        QList<int> columnWidths;        ///< Width of each column
        QList<int> columnOrder;         ///< Visual order of columns
        QList<bool> columnVisibility;   ///< Visibility of each column
        int sortColumn;                 ///< Sort column index
        Qt::SortOrder sortOrder;        ///< Sort order (Ascending/Descending)
        
        Preset() : sortColumn(-1), sortOrder(Qt::AscendingOrder) {}
    };

    /**
     * @brief Constructs a view presets manager
     * @param parent Parent object
     * @since 1.0
     */
    explicit BViewPresets(QObject *parent = nullptr);
    
    /**
     * @brief Gets the singleton instance
     * @return Pointer to BViewPresets instance
     * @since 1.0
     */
    static BViewPresets* instance();
    
    /**
     * @brief Saves a preset
     * @param name Preset name
     * @param preset Preset configuration
     * @since 1.0
     */
    void savePreset(const QString &name, const Preset &preset);
    
    /**
     * @brief Loads a preset
     * @param name Preset name
     * @return Preset configuration (empty if not found)
     * @since 1.0
     */
    Preset loadPreset(const QString &name) const;
    
    /**
     * @brief Deletes a preset
     * @param name Preset name
     * @since 1.0
     */
    void deletePreset(const QString &name);
    
    /**
     * @brief Gets list of all preset names
     * @return List of preset names
     * @since 1.0
     */
    QStringList presetNames() const;
    
    /**
     * @brief Checks if a preset exists
     * @param name Preset name
     * @return True if preset exists
     * @since 1.0
     */
    bool hasPreset(const QString &name) const;
    
    /**
     * @brief Renames a preset
     * @param oldName Old name
     * @param newName New name
     * @return True if successful
     * @since 1.0
     */
    bool renamePreset(const QString &oldName, const QString &newName);
    
    /**
     * @brief Saves the auto-save preset (last used configuration)
     * @param preset Preset configuration
     * @since 1.0
     */
    void saveAutoPreset(const Preset &preset);
    
    /**
     * @brief Loads the auto-save preset
     * @return Auto-saved preset (empty if not found)
     * @since 1.0
     */
    Preset loadAutoPreset() const;

signals:
    /**
     * @brief Emitted when a preset is saved
     * @param name Preset name
     * @since 1.0
     */
    void presetSaved(const QString &name);
    
    /**
     * @brief Emitted when a preset is deleted
     * @param name Preset name
     * @since 1.0
     */
    void presetDeleted(const QString &name);
    
    /**
     * @brief Emitted when a preset is renamed
     * @param oldName Old name
     * @param newName New name
     * @since 1.0
     */
    void presetRenamed(const QString &oldName, const QString &newName);

private:
    void saveToSettings();
    void loadFromSettings();
    
    static BViewPresets *s_instance;    ///< Singleton instance
    QMap<QString, Preset> m_presets;    ///< All saved presets
    QSettings *m_settings;              ///< Persistent storage
};

#endif // BVIEWPRESETS_H
