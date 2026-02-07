-- Migration from v4 to v5
-- Add fileset_templates table for storing FileSet templates
-- Created: 2026-02-07

-- Update schema version
INSERT INTO schema_version (version, description)
VALUES (5, 'Add fileset_templates table for built-in and user-defined FileSet templates');

-- ============================================================================
-- Create fileset_templates table
-- ============================================================================

CREATE TABLE fileset_templates (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    description TEXT,
    platform TEXT DEFAULT 'all' CHECK(platform IN ('linux', 'windows', 'macos', 'all')),
    category TEXT DEFAULT 'custom' CHECK(category IN ('system', 'database', 'web', 'mail', 'container', 'custom')),
    is_builtin INTEGER DEFAULT 0 CHECK(is_builtin IN (0, 1)),
    content TEXT NOT NULL,  -- Full JSON template content
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- Create indexes for efficient querying
-- ============================================================================

-- Index for filtering by platform
CREATE INDEX idx_fileset_templates_platform ON fileset_templates(platform);

-- Index for filtering by category
CREATE INDEX idx_fileset_templates_category ON fileset_templates(category);

-- Index for filtering built-in vs user templates
CREATE INDEX idx_fileset_templates_builtin ON fileset_templates(is_builtin);

-- Combined index for common queries
CREATE INDEX idx_fileset_templates_platform_category
    ON fileset_templates(platform, category, is_builtin);

-- ============================================================================
-- Create trigger to update updated_at on modification
-- ============================================================================

CREATE TRIGGER fileset_templates_update_timestamp
AFTER UPDATE ON fileset_templates
FOR EACH ROW
BEGIN
    UPDATE fileset_templates SET updated_at = CURRENT_TIMESTAMP WHERE id = OLD.id;
END;
