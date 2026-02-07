#include "jobs/bviewpresets.h"
#include <QCoreApplication>

BViewPresets* BViewPresets::s_instance = nullptr;

BViewPresets::BViewPresets(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings(QCoreApplication::organizationName(), 
                               QCoreApplication::applicationName(), this))
{
    loadFromSettings();
}

BViewPresets* BViewPresets::instance()
{
    if (!s_instance) {
        s_instance = new BViewPresets(qApp);
    }
    return s_instance;
}

void BViewPresets::savePreset(const QString &name, const Preset &preset)
{
    m_presets[name] = preset;
    saveToSettings();
    emit presetSaved(name);
}

BViewPresets::Preset BViewPresets::loadPreset(const QString &name) const
{
    return m_presets.value(name, Preset());
}

void BViewPresets::deletePreset(const QString &name)
{
    if (m_presets.remove(name) > 0) {
        saveToSettings();
        emit presetDeleted(name);
    }
}

QStringList BViewPresets::presetNames() const
{
    return m_presets.keys();
}

bool BViewPresets::hasPreset(const QString &name) const
{
    return m_presets.contains(name);
}

bool BViewPresets::renamePreset(const QString &oldName, const QString &newName)
{
    if (!m_presets.contains(oldName) || m_presets.contains(newName)) {
        return false;
    }
    
    Preset preset = m_presets.take(oldName);
    preset.name = newName;
    m_presets[newName] = preset;
    
    saveToSettings();
    emit presetRenamed(oldName, newName);
    return true;
}

void BViewPresets::saveAutoPreset(const Preset &preset)
{
    m_settings->beginGroup("AutoSave");
    
    m_settings->setValue("columnWidths", QVariant::fromValue(preset.columnWidths));
    m_settings->setValue("columnOrder", QVariant::fromValue(preset.columnOrder));
    
    QList<int> visibility;
    for (bool v : preset.columnVisibility) {
        visibility.append(v ? 1 : 0);
    }
    m_settings->setValue("columnVisibility", QVariant::fromValue(visibility));
    
    m_settings->setValue("sortColumn", preset.sortColumn);
    m_settings->setValue("sortOrder", static_cast<int>(preset.sortOrder));
    
    m_settings->endGroup();
    m_settings->sync();
}

BViewPresets::Preset BViewPresets::loadAutoPreset() const
{
    Preset preset;
    
    m_settings->beginGroup("AutoSave");
    
    preset.columnWidths = m_settings->value("columnWidths").value<QList<int>>();
    preset.columnOrder = m_settings->value("columnOrder").value<QList<int>>();
    
    QList<int> visibility = m_settings->value("columnVisibility").value<QList<int>>();
    for (int v : visibility) {
        preset.columnVisibility.append(v != 0);
    }
    
    preset.sortColumn = m_settings->value("sortColumn", -1).toInt();
    preset.sortOrder = static_cast<Qt::SortOrder>(
        m_settings->value("sortOrder", Qt::AscendingOrder).toInt());
    
    const_cast<QSettings*>(m_settings)->endGroup();
    
    return preset;
}

void BViewPresets::saveToSettings()
{
    m_settings->beginGroup("Presets");
    m_settings->remove("");  // Clear all presets
    
    for (auto it = m_presets.constBegin(); it != m_presets.constEnd(); ++it) {
        const QString &name = it.key();
        const Preset &preset = it.value();
        
        m_settings->beginGroup(name);
        m_settings->setValue("columnWidths", QVariant::fromValue(preset.columnWidths));
        m_settings->setValue("columnOrder", QVariant::fromValue(preset.columnOrder));
        
        QList<int> visibility;
        for (bool v : preset.columnVisibility) {
            visibility.append(v ? 1 : 0);
        }
        m_settings->setValue("columnVisibility", QVariant::fromValue(visibility));
        
        m_settings->setValue("sortColumn", preset.sortColumn);
        m_settings->setValue("sortOrder", static_cast<int>(preset.sortOrder));
        m_settings->endGroup();
    }
    
    m_settings->endGroup();
    m_settings->sync();
}

void BViewPresets::loadFromSettings()
{
    m_presets.clear();
    
    m_settings->beginGroup("Presets");
    QStringList presetGroups = m_settings->childGroups();
    
    for (const QString &name : presetGroups) {
        m_settings->beginGroup(name);
        
        Preset preset;
        preset.name = name;
        preset.columnWidths = m_settings->value("columnWidths").value<QList<int>>();
        preset.columnOrder = m_settings->value("columnOrder").value<QList<int>>();
        
        QList<int> visibility = m_settings->value("columnVisibility").value<QList<int>>();
        for (int v : visibility) {
            preset.columnVisibility.append(v != 0);
        }
        
        preset.sortColumn = m_settings->value("sortColumn", -1).toInt();
        preset.sortOrder = static_cast<Qt::SortOrder>(
            m_settings->value("sortOrder", Qt::AscendingOrder).toInt());
        
        m_presets[name] = preset;
        
        m_settings->endGroup();
    }
    
    m_settings->endGroup();
}
