#include "config/bcolumnconfiguration.h"
#include "blogging.h"
#include <QApplication>

BColumnConfiguration::BColumnConfiguration(QObject *parent)
    : QObject(parent)
    , m_settings(QCoreApplication::organizationName(), 
                 QCoreApplication::applicationName())
{
}

void BColumnConfiguration::saveHeaderState(QHeaderView *header, const QString &settingsKey)
{
    if (!header) return;
    
    QMap<int, ColumnConfig> config = readConfiguration(header);
    
    m_settings.beginGroup(settingsKey);
    m_settings.remove("");  // Clear existing settings
    
    for (auto it = config.constBegin(); it != config.constEnd(); ++it) {
        int logicalIndex = it.key();
        const ColumnConfig &col = it.value();
        
        m_settings.beginGroup(QString("Column_%1").arg(logicalIndex));
        m_settings.setValue("visualIndex", col.visualIndex);
        m_settings.setValue("width", col.width);
        m_settings.setValue("visible", col.visible);
        m_settings.endGroup();
    }
    
    m_settings.endGroup();
    m_settings.sync();
    
    emit configurationChanged();
}

void BColumnConfiguration::restoreHeaderState(QHeaderView *header, const QString &settingsKey)
{
    if (!header) return;
    
    m_settings.beginGroup(settingsKey);
    
    if (m_settings.childGroups().isEmpty()) {
        m_settings.endGroup();
        return;  // No saved configuration
    }
    
    QMap<int, ColumnConfig> config;
    
    for (const QString &group : m_settings.childGroups()) {
        m_settings.beginGroup(group);
        
        // Extract logical index from group name "Column_X"
        int logicalIndex = group.mid(7).toInt();
        
        ColumnConfig col;
        col.visualIndex = m_settings.value("visualIndex", logicalIndex).toInt();
        col.width = m_settings.value("width", 100).toInt();
        col.visible = m_settings.value("visible", true).toBool();
        
        config[logicalIndex] = col;
        
        m_settings.endGroup();
    }
    
    m_settings.endGroup();
    
    applyConfiguration(header, config);
}

void BColumnConfiguration::savePreset(QHeaderView *header, const QString &presetName, const QString &description)
{
    if (!header || presetName.isEmpty()) return;
    
    QMap<int, ColumnConfig> config = readConfiguration(header);
    
    m_settings.beginGroup("Presets");
    m_settings.beginGroup(presetName);
    
    m_settings.setValue("description", description);
    
    for (auto it = config.constBegin(); it != config.constEnd(); ++it) {
        int logicalIndex = it.key();
        const ColumnConfig &col = it.value();
        
        m_settings.beginGroup(QString("Column_%1").arg(logicalIndex));
        m_settings.setValue("visualIndex", col.visualIndex);
        m_settings.setValue("width", col.width);
        m_settings.setValue("visible", col.visible);
        m_settings.endGroup();
    }
    
    m_settings.endGroup();
    m_settings.endGroup();
    m_settings.sync();
    
    BLOG_DEBUG() << "Saved preset:" << presetName;
}

bool BColumnConfiguration::loadPreset(QHeaderView *header, const QString &presetName)
{
    if (!header || presetName.isEmpty()) return false;
    
    m_settings.beginGroup("Presets");
    
    if (!m_settings.childGroups().contains(presetName)) {
        m_settings.endGroup();
        BLOG_DEBUG() << "Preset not found:" << presetName;
        return false;
    }
    
    m_settings.beginGroup(presetName);
    
    QMap<int, ColumnConfig> config;
    
    for (const QString &group : m_settings.childGroups()) {
        m_settings.beginGroup(group);
        
        int logicalIndex = group.mid(7).toInt();
        
        ColumnConfig col;
        col.visualIndex = m_settings.value("visualIndex", logicalIndex).toInt();
        col.width = m_settings.value("width", 100).toInt();
        col.visible = m_settings.value("visible", true).toBool();
        
        config[logicalIndex] = col;
        
        m_settings.endGroup();
    }
    
    m_settings.endGroup();
    m_settings.endGroup();
    
    applyConfiguration(header, config);
    emit presetLoaded(presetName);
    
    BLOG_DEBUG() << "Loaded preset:" << presetName;
    return true;
}

void BColumnConfiguration::deletePreset(const QString &presetName)
{
    if (presetName.isEmpty()) return;
    
    m_settings.beginGroup("Presets");
    m_settings.remove(presetName);
    m_settings.endGroup();
    m_settings.sync();
    
    BLOG_DEBUG() << "Deleted preset:" << presetName;
}

QStringList BColumnConfiguration::availablePresets() const
{
    // Create temporary settings object to avoid const issues
    QSettings settings(m_settings.organizationName(), m_settings.applicationName());
    
    settings.beginGroup("Presets");
    QStringList presets = settings.childGroups();
    settings.endGroup();
    
    return presets;
}

BColumnConfiguration::Preset BColumnConfiguration::getPreset(const QString &presetName) const
{
    Preset preset;
    preset.name = presetName;
    
    // Can't use m_settings directly in const method, need to work around
    QSettings settings(m_settings.organizationName(), m_settings.applicationName());
    
    settings.beginGroup("Presets");
    
    if (!settings.childGroups().contains(presetName)) {
        settings.endGroup();
        return preset;
    }
    
    settings.beginGroup(presetName);
    
    preset.description = settings.value("description").toString();
    
    for (const QString &group : settings.childGroups()) {
        settings.beginGroup(group);
        
        int logicalIndex = group.mid(7).toInt();
        
        ColumnConfig col;
        col.visualIndex = settings.value("visualIndex", logicalIndex).toInt();
        col.width = settings.value("width", 100).toInt();
        col.visible = settings.value("visible", true).toBool();
        
        preset.columns[logicalIndex] = col;
        
        settings.endGroup();
    }
    
    settings.endGroup();
    settings.endGroup();
    
    return preset;
}

void BColumnConfiguration::resetToDefault(QHeaderView *header)
{
    if (!header) return;
    
    // Reset to default widths and order
    for (int i = 0; i < header->count(); ++i) {
        header->setSectionHidden(i, false);
        header->moveSection(header->visualIndex(i), i);
    }
    
    // Set reasonable default widths (can be customized)
    if (header->count() >= 11) {  // BJobsModel specific
        header->resizeSection(0, 40);   // Checkbox
        header->resizeSection(1, 60);   // Job ID
        header->resizeSection(2, 150);  // Name
        header->resizeSection(3, 100);  // Client
        header->resizeSection(4, 150);  // Start Time
        header->resizeSection(5, 90);   // Duration
        header->resizeSection(6, 50);   // Type
        header->resizeSection(7, 50);   // Level
        header->resizeSection(8, 80);   // Files
        header->resizeSection(9, 100);  // Bytes
        header->resizeSection(10, 120); // Status
    }
    
    BLOG_DEBUG() << "Reset to default configuration";
}

QMap<int, BColumnConfiguration::ColumnConfig> BColumnConfiguration::readConfiguration(QHeaderView *header) const
{
    QMap<int, ColumnConfig> config;
    
    for (int logicalIndex = 0; logicalIndex < header->count(); ++logicalIndex) {
        ColumnConfig col;
        col.visualIndex = header->visualIndex(logicalIndex);
        col.width = header->sectionSize(logicalIndex);
        col.visible = !header->isSectionHidden(logicalIndex);
        
        config[logicalIndex] = col;
    }
    
    return config;
}

void BColumnConfiguration::applyConfiguration(QHeaderView *header, const QMap<int, ColumnConfig> &config)
{
    // First pass: Set widths and visibility
    for (auto it = config.constBegin(); it != config.constEnd(); ++it) {
        int logicalIndex = it.key();
        const ColumnConfig &col = it.value();
        
        if (logicalIndex >= 0 && logicalIndex < header->count()) {
            header->resizeSection(logicalIndex, col.width);
            header->setSectionHidden(logicalIndex, !col.visible);
        }
    }
    
    // Second pass: Reorder columns
    // We need to do this carefully to avoid conflicts
    QList<QPair<int, int>> moves;  // (logical, targetVisual)
    
    for (auto it = config.constBegin(); it != config.constEnd(); ++it) {
        int logicalIndex = it.key();
        int targetVisual = it.value().visualIndex;
        
        if (logicalIndex >= 0 && logicalIndex < header->count()) {
            moves.append(qMakePair(logicalIndex, targetVisual));
        }
    }
    
    // Sort by target visual index to apply moves in order
    std::sort(moves.begin(), moves.end(), 
              [](const QPair<int, int> &a, const QPair<int, int> &b) {
                  return a.second < b.second;
              });
    
    // Apply moves
    for (const auto &move : moves) {
        int logicalIndex = move.first;
        int targetVisual = move.second;
        int currentVisual = header->visualIndex(logicalIndex);
        
        if (currentVisual != targetVisual) {
            header->moveSection(currentVisual, targetVisual);
        }
    }
    
    BLOG_DEBUG() << "Applied column configuration";
}
