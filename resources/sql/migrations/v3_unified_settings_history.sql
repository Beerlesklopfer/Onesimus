-- Migration from v2 to v3
-- Replace separate config tables with unified settings_history table
-- Created: 2026-01-31

-- Update schema version
INSERT INTO schema_version (version, description)
VALUES (3, 'Unified settings table with full history tracking and user attribution');

-- ============================================================================
-- Drop old config tables (data will be migrated if needed)
-- ============================================================================

DROP TABLE IF EXISTS director_config;
DROP TABLE IF EXISTS client_config;
DROP TABLE IF EXISTS storage_config;
DROP TABLE IF EXISTS console_config;

-- ============================================================================
-- Unified Settings History Table
-- ============================================================================

-- Settings history with audit trail
CREATE TABLE settings_history (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    -- Resource reference (polymorphic)
    resource_type TEXT NOT NULL CHECK(resource_type IN ('director', 'client', 'storage', 'console')),
    resource_id INTEGER NOT NULL,

    -- Setting key-value
    setting_key TEXT NOT NULL,
    setting_value TEXT,

    -- Audit fields
    changed_by TEXT NOT NULL,          -- Username or system identifier
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    -- Version control
    is_current BOOLEAN DEFAULT 1,      -- 1 = current value, 0 = historical
    version INTEGER DEFAULT 1,         -- Increments with each change

    -- Optional metadata
    change_reason TEXT,                -- Why this change was made
    previous_value TEXT                -- Previous value (for easy rollback)
);

-- ============================================================================
-- Indexes for performance
-- ============================================================================

-- Query current settings for a resource
CREATE INDEX idx_settings_resource ON settings_history(resource_type, resource_id, is_current);

-- Query by setting key
CREATE INDEX idx_settings_key ON settings_history(setting_key);

-- Query history by user
CREATE INDEX idx_settings_user ON settings_history(changed_by);

-- Query history by time
CREATE INDEX idx_settings_time ON settings_history(changed_at DESC);

-- Compound index for current settings lookup
CREATE INDEX idx_settings_current ON settings_history(resource_type, resource_id, setting_key, is_current);

-- ============================================================================
-- Views for convenience
-- ============================================================================

-- View: Current settings only (filters is_current = 1)
CREATE VIEW current_settings AS
SELECT
    resource_type,
    resource_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version
FROM settings_history
WHERE is_current = 1;

-- View: Director settings (current only)
CREATE VIEW director_settings AS
SELECT
    resource_id AS director_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version
FROM current_settings
WHERE resource_type = 'director';

-- View: Client settings (current only)
CREATE VIEW client_settings AS
SELECT
    resource_id AS client_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version
FROM current_settings
WHERE resource_type = 'client';

-- View: Storage settings (current only)
CREATE VIEW storage_settings AS
SELECT
    resource_id AS storage_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version
FROM current_settings
WHERE resource_type = 'storage';

-- View: Console settings (current only)
CREATE VIEW console_settings AS
SELECT
    resource_id AS console_id,
    setting_key,
    setting_value,
    changed_by,
    changed_at,
    version
FROM current_settings
WHERE resource_type = 'console';

-- ============================================================================
-- Triggers for automatic history management
-- ============================================================================

-- Trigger: Mark previous value as historical when updating
CREATE TRIGGER settings_update_history
BEFORE INSERT ON settings_history
FOR EACH ROW
WHEN NEW.is_current = 1
BEGIN
    -- Mark existing current value as historical
    UPDATE settings_history
    SET is_current = 0
    WHERE resource_type = NEW.resource_type
      AND resource_id = NEW.resource_id
      AND setting_key = NEW.setting_key
      AND is_current = 1;

    -- Set version number (max + 1)
    UPDATE settings_history
    SET version = (
        SELECT COALESCE(MAX(version), 0) + 1
        FROM settings_history
        WHERE resource_type = NEW.resource_type
          AND resource_id = NEW.resource_id
          AND setting_key = NEW.setting_key
    )
    WHERE id = NEW.id;
END;
