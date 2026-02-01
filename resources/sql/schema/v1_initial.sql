-- Onesimus Database Schema v1
-- Initial schema for Bareos/Bacula management
-- Created: 2026-01-31

-- Schema version tracking
CREATE TABLE IF NOT EXISTS schema_version (
    version INTEGER PRIMARY KEY,
    applied_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    description TEXT NOT NULL
);

-- Insert initial version
INSERT INTO schema_version (version, description)
VALUES (1, 'Initial schema: Directors, Clients, Storages with daemon configurations');

-- ============================================================================
-- Director Configuration
-- ============================================================================

-- Director table (main Director resource)
CREATE TABLE IF NOT EXISTS directors (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    address TEXT NOT NULL,
    port INTEGER DEFAULT 9101,
    password_hash TEXT NOT NULL,  -- Hashed password for connection
    description TEXT,

    -- Director resource settings
    dir_port INTEGER,              -- DIRport directive
    query_file TEXT,               -- QueryFile directive
    working_directory TEXT,        -- WorkingDirectory directive
    pid_directory TEXT,            -- PidDirectory directive
    scripts_directory TEXT,        -- ScriptsDirectory directive
    plugin_directory TEXT,         -- PluginDirectory directive
    sub_sys_directory TEXT,        -- SubSysDirectory directive
    maximum_concurrent_jobs INTEGER DEFAULT 20,

    -- TLS/SSL settings
    tls_enable BOOLEAN DEFAULT 0,
    tls_require BOOLEAN DEFAULT 0,
    tls_verify_peer BOOLEAN DEFAULT 0,
    tls_ca_certificate_file TEXT,
    tls_certificate TEXT,
    tls_key TEXT,
    tls_allowed_cn TEXT,           -- Comma-separated list

    -- Metadata
    backup_system TEXT DEFAULT 'bareos',  -- 'bareos' or 'bacula'
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_connected_at TIMESTAMP,
    is_active BOOLEAN DEFAULT 1
);

-- Director configuration (key-value pairs for additional settings)
CREATE TABLE IF NOT EXISTS director_config (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    director_id INTEGER NOT NULL,
    config_key TEXT NOT NULL,
    config_value TEXT,
    FOREIGN KEY (director_id) REFERENCES directors(id) ON DELETE CASCADE,
    UNIQUE(director_id, config_key)
);

-- ============================================================================
-- Client Configuration
-- ============================================================================

-- Clients table (Client resources)
CREATE TABLE IF NOT EXISTS clients (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    director_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    address TEXT NOT NULL,
    fd_port INTEGER DEFAULT 9102,
    password_hash TEXT NOT NULL,  -- Client password
    description TEXT,

    -- Client resource settings
    catalog TEXT,
    file_retention TEXT,           -- e.g., "60 days"
    job_retention TEXT,            -- e.g., "180 days"
    auto_prune BOOLEAN DEFAULT 1,
    maximum_concurrent_jobs INTEGER DEFAULT 1,

    -- File Daemon settings
    working_directory TEXT,
    pid_directory TEXT,
    plugin_directory TEXT,

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

-- Client configuration (key-value pairs for additional settings)
CREATE TABLE IF NOT EXISTS client_config (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    client_id INTEGER NOT NULL,
    config_key TEXT NOT NULL,
    config_value TEXT,
    FOREIGN KEY (client_id) REFERENCES clients(id) ON DELETE CASCADE,
    UNIQUE(client_id, config_key)
);

-- ============================================================================
-- Storage Configuration
-- ============================================================================

-- Storages table (Storage resources)
CREATE TABLE IF NOT EXISTS storages (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    director_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    address TEXT NOT NULL,
    sd_port INTEGER DEFAULT 9103,
    password_hash TEXT NOT NULL,  -- Storage password
    description TEXT,

    -- Storage resource settings
    device TEXT,                   -- Device name
    media_type TEXT,               -- Media Type
    autochanger BOOLEAN DEFAULT 0,
    maximum_concurrent_jobs INTEGER DEFAULT 1,
    allow_compression BOOLEAN DEFAULT 1,
    heartbeat_interval INTEGER,

    -- Storage Daemon settings
    working_directory TEXT,
    pid_directory TEXT,
    plugin_directory TEXT,
    scripts_directory TEXT,

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

-- Storage configuration (key-value pairs for additional settings)
CREATE TABLE IF NOT EXISTS storage_config (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    storage_id INTEGER NOT NULL,
    config_key TEXT NOT NULL,
    config_value TEXT,
    FOREIGN KEY (storage_id) REFERENCES storages(id) ON DELETE CASCADE,
    UNIQUE(storage_id, config_key)
);

-- ============================================================================
-- Indexes for performance
-- ============================================================================

CREATE INDEX IF NOT EXISTS idx_directors_name ON directors(name);
CREATE INDEX IF NOT EXISTS idx_directors_active ON directors(is_active);
CREATE INDEX IF NOT EXISTS idx_clients_director ON clients(director_id);
CREATE INDEX IF NOT EXISTS idx_clients_name ON clients(name);
CREATE INDEX IF NOT EXISTS idx_storages_director ON storages(director_id);
CREATE INDEX IF NOT EXISTS idx_storages_name ON storages(name);
CREATE INDEX IF NOT EXISTS idx_director_config ON director_config(director_id, config_key);
CREATE INDEX IF NOT EXISTS idx_client_config ON client_config(client_id, config_key);
CREATE INDEX IF NOT EXISTS idx_storage_config ON storage_config(storage_id, config_key);
