-- Migration from v1 to v2
-- Adds Console configuration support
-- Created: 2026-01-31

-- Update schema version first
INSERT INTO schema_version (version, description)
VALUES (2, 'Add Console configuration support');

-- ============================================================================
-- Console Configuration (BCons)
-- ============================================================================

-- Consoles table (Console resources for bconsole)
CREATE TABLE IF NOT EXISTS consoles (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    director_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    password_hash TEXT NOT NULL,  -- Console password
    description TEXT,

    -- Console resource settings
    catalog TEXT,
    command_acl TEXT,              -- Comma-separated list
    job_acl TEXT,                  -- Comma-separated list
    schedule_acl TEXT,             -- Comma-separated list
    client_acl TEXT,               -- Comma-separated list
    storage_acl TEXT,              -- Comma-separated list
    pool_acl TEXT,                 -- Comma-separated list
    fileset_acl TEXT,              -- Comma-separated list
    where_acl TEXT,                -- Comma-separated list

    -- TLS/SSL settings
    tls_enable BOOLEAN DEFAULT 0,
    tls_require BOOLEAN DEFAULT 0,
    tls_ca_certificate_file TEXT,
    tls_certificate TEXT,
    tls_key TEXT,

    -- Metadata
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_active BOOLEAN DEFAULT 1,

    FOREIGN KEY (director_id) REFERENCES directors(id) ON DELETE CASCADE,
    UNIQUE(director_id, name)
);

-- Console configuration (key-value pairs for additional settings)
CREATE TABLE IF NOT EXISTS console_config (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    console_id INTEGER NOT NULL,
    config_key TEXT NOT NULL,
    config_value TEXT,
    FOREIGN KEY (console_id) REFERENCES consoles(id) ON DELETE CASCADE,
    UNIQUE(console_id, config_key)
);

-- Indexes
CREATE INDEX IF NOT EXISTS idx_consoles_director ON consoles(director_id);
CREATE INDEX IF NOT EXISTS idx_consoles_name ON consoles(name);
CREATE INDEX IF NOT EXISTS idx_console_config ON console_config(console_id, config_key);
