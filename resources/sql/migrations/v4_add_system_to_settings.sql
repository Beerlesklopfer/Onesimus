-- Migration from v3 to v4
-- Add backup_system field to settings_history
-- Created: 2026-01-31

-- Update schema version
INSERT INTO schema_version (version, description)
VALUES (4, 'Add backup system indicator to settings_history for Bareos/Bacula compatibility');

-- ============================================================================
-- Add backup_system column to settings_history
-- ============================================================================

-- Add column with default 'both' (compatible with both systems)
ALTER TABLE settings_history ADD COLUMN backup_system TEXT DEFAULT 'both'
    CHECK(backup_system IN ('bareos', 'bacula', 'both'));

-- Update existing records to 'both' for backward compatibility
UPDATE settings_history SET backup_system = 'both' WHERE backup_system IS NULL;

-- ============================================================================
-- Update views to include backup_system
-- ============================================================================

-- Drop old views
DROP VIEW IF EXISTS current_settings;
DROP VIEW IF EXISTS director_settings;
DROP VIEW IF EXISTS client_settings;
DROP VIEW IF EXISTS storage_settings;
DROP VIEW IF EXISTS console_settings;

-- Recreate current_settings view with backup_system
CREATE VIEW current_settings AS
SELECT
    resource_type,
    resource_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version,
    backup_system
FROM settings_history
WHERE is_current = 1;

-- Recreate director_settings view
CREATE VIEW director_settings AS
SELECT
    resource_id AS director_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version,
    backup_system
FROM current_settings
WHERE resource_type = 'director';

-- Recreate client_settings view
CREATE VIEW client_settings AS
SELECT
    resource_id AS client_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version,
    backup_system
FROM current_settings
WHERE resource_type = 'client';

-- Recreate storage_settings view
CREATE VIEW storage_settings AS
SELECT
    resource_id AS storage_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version,
    backup_system
FROM current_settings
WHERE resource_type = 'storage';

-- Recreate console_settings view
CREATE VIEW console_settings AS
SELECT
    resource_id AS console_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version,
    backup_system
FROM current_settings
WHERE resource_type = 'console';

-- ============================================================================
-- Update indexes
-- ============================================================================

-- Add index for filtering by backup system
CREATE INDEX IF NOT EXISTS idx_settings_system ON settings_history(backup_system);

-- Combined index for system-specific queries
CREATE INDEX IF NOT EXISTS idx_settings_system_current
    ON settings_history(resource_type, resource_id, backup_system, is_current);
