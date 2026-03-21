//! Schema-Migration: vergleicht JSON-Schema mit Live-DB und migriert.

use std::path::{Path, PathBuf};

use tracing::{info, warn, debug};

use crate::schema::CatalogSchema;
use crate::error::OrmError;

/// Schema-Migrator: liest JSON-Versionen und migriert die DB.
pub struct Migrator {
    schema_dir: PathBuf,
}

impl Migrator {
    pub fn new(schema_dir: &Path) -> Self {
        Self {
            schema_dir: schema_dir.to_path_buf(),
        }
    }

    /// Lade alle Schema-Versionen aus dem Verzeichnis, sortiert nach Version.
    pub fn load_versions(&self) -> Result<Vec<CatalogSchema>, OrmError> {
        let mut schemas = Vec::new();

        if !self.schema_dir.exists() {
            return Ok(schemas);
        }

        for entry in std::fs::read_dir(&self.schema_dir)
            .map_err(|e| OrmError::Other(format!("read dir: {e}")))?
        {
            let entry = entry.map_err(|e| OrmError::Other(format!("dir entry: {e}")))?;
            let path = entry.path();
            if path.extension().map(|e| e == "json").unwrap_or(false) {
                match CatalogSchema::load(&path) {
                    Ok(schema) => schemas.push(schema),
                    Err(e) => warn!("skip {}: {e}", path.display()),
                }
            }
        }

        schemas.sort_by_key(|s| s.version);
        Ok(schemas)
    }

    /// Lade die neueste Schema-Version.
    pub fn latest(&self) -> Result<Option<CatalogSchema>, OrmError> {
        let versions = self.load_versions()?;
        Ok(versions.into_iter().last())
    }

    /// Generiere die Migration-SQL von Version `from` nach `to`.
    pub fn diff(from: &CatalogSchema, to: &CatalogSchema) -> Vec<String> {
        let mut statements = Vec::new();

        // Neue Tabellen
        for table in &to.tables {
            if from.find_table(&table.name).is_none() {
                statements.push(table.to_create_sql());
                for idx in &table.indices {
                    statements.push(idx.to_sql(&table.name));
                }
            }
        }

        // Neue Spalten in bestehenden Tabellen
        for to_table in &to.tables {
            if let Some(from_table) = from.find_table(&to_table.name) {
                let from_cols: Vec<&str> = from_table.columns.iter()
                    .map(|c| c.name.as_str()).collect();

                for col in &to_table.columns {
                    if !from_cols.contains(&col.name.as_str()) {
                        statements.push(
                            crate::schema::TableDef::alter_add_column(&to_table.name, col)
                        );
                    }
                }

                // Neue Indizes
                let from_idx_names: Vec<&str> = from_table.indices.iter()
                    .map(|i| i.name.as_str()).collect();
                for idx in &to_table.indices {
                    if !from_idx_names.contains(&idx.name.as_str()) {
                        statements.push(idx.to_sql(&to_table.name));
                    }
                }
            }
        }

        // Gelöschte Tabellen (optional, vorsichtig)
        for from_table in &from.tables {
            if to.find_table(&from_table.name).is_none() {
                statements.push(format!("-- DROP TABLE {} (removed in v{});\n",
                    from_table.name, to.version));
            }
        }

        statements
    }

    /// Generiere vollständiges DDL für die neueste Version.
    pub fn full_ddl(&self) -> Result<String, OrmError> {
        match self.latest()? {
            Some(schema) => Ok(schema.to_ddl()),
            None => Err(OrmError::Other("no schema versions found".to_string())),
        }
    }

    /// Führe Migration auf einer PostgreSQL-Datenbank durch.
    pub async fn migrate(&self, pool: &sqlx::PgPool) -> Result<u32, OrmError> {
        // Aktuelle DB-Version ermitteln
        let db_version = self.get_db_version(pool).await?;

        let versions = self.load_versions()?;
        let mut applied = 0;

        for schema in &versions {
            if schema.version <= db_version {
                continue;
            }

            info!("migrating to schema v{}: {}", schema.version, schema.description);

            if db_version == 0 {
                // Erstinstallation: vollständiges DDL
                let ddl = schema.to_ddl();
                sqlx::query(&ddl).execute(pool).await?;
            } else {
                // Inkrementelle Migration
                let prev = versions.iter()
                    .filter(|s| s.version < schema.version)
                    .last();

                if let Some(prev_schema) = prev {
                    let stmts = Self::diff(prev_schema, schema);
                    for stmt in &stmts {
                        debug!("  {}", stmt.trim());
                        sqlx::query(stmt).execute(pool).await?;
                    }
                }
            }

            // Version in DB speichern
            self.set_db_version(pool, schema.version).await?;
            applied += 1;
        }

        Ok(applied)
    }

    async fn get_db_version(&self, pool: &sqlx::PgPool) -> Result<u32, OrmError> {
        // Prüfe ob die Version-Tabelle existiert
        let exists: bool = sqlx::query_scalar(
            "SELECT EXISTS(SELECT 1 FROM information_schema.tables WHERE table_name = 'bareos_schema_version')"
        ).fetch_one(pool).await?;

        if !exists {
            // Erstelle Version-Tabelle
            sqlx::query(
                "CREATE TABLE IF NOT EXISTS bareos_schema_version (version integer NOT NULL, applied_at timestamp DEFAULT now())"
            ).execute(pool).await?;
            return Ok(0);
        }

        let version: Option<i32> = sqlx::query_scalar(
            "SELECT MAX(version) FROM bareos_schema_version"
        ).fetch_one(pool).await?;

        Ok(version.unwrap_or(0) as u32)
    }

    async fn set_db_version(&self, pool: &sqlx::PgPool, version: u32) -> Result<(), OrmError> {
        sqlx::query("INSERT INTO bareos_schema_version (version) VALUES ($1)")
            .bind(version as i32)
            .execute(pool)
            .await?;
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn diff_detects_new_table() {
        let v1: CatalogSchema = serde_json::from_str(r#"{
            "version": 1, "description": "v1", "created": "2026", "author": "test",
            "tables": [
                {"name": "client", "primary_key": "id", "columns": [
                    {"name": "id", "type": "serial", "not_null": true}
                ]}
            ]
        }"#).unwrap();

        let v2: CatalogSchema = serde_json::from_str(r#"{
            "version": 2, "description": "v2", "created": "2026", "author": "test",
            "tables": [
                {"name": "client", "primary_key": "id", "columns": [
                    {"name": "id", "type": "serial", "not_null": true}
                ]},
                {"name": "job", "primary_key": "id", "columns": [
                    {"name": "id", "type": "serial", "not_null": true},
                    {"name": "name", "type": "text", "not_null": true}
                ]}
            ]
        }"#).unwrap();

        let stmts = Migrator::diff(&v1, &v2);
        assert!(!stmts.is_empty());
        let sql = stmts.join("\n");
        assert!(sql.contains("CREATE TABLE IF NOT EXISTS job"));
    }

    #[test]
    fn diff_detects_new_column() {
        let v1: CatalogSchema = serde_json::from_str(r#"{
            "version": 1, "description": "v1", "created": "2026", "author": "test",
            "tables": [
                {"name": "job", "primary_key": "id", "columns": [
                    {"name": "id", "type": "serial", "not_null": true}
                ]}
            ]
        }"#).unwrap();

        let v2: CatalogSchema = serde_json::from_str(r#"{
            "version": 2, "description": "v2", "created": "2026", "author": "test",
            "tables": [
                {"name": "job", "primary_key": "id", "columns": [
                    {"name": "id", "type": "serial", "not_null": true},
                    {"name": "metadata", "type": "jsonb"}
                ]}
            ]
        }"#).unwrap();

        let stmts = Migrator::diff(&v1, &v2);
        let sql = stmts.join("\n");
        assert!(sql.contains("ALTER TABLE job ADD COLUMN metadata jsonb"));
    }
}
