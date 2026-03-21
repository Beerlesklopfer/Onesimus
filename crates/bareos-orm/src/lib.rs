//! JSON-basiertes Schema-ORM für den Bareos Catalog.
//!
//! Die JSON-Dateien in `/var/lib/bareos/catalog/` sind die Single Source of Truth
//! für das Datenbank-Schema. Jede JSON-Datei enthält eine Versionsnummer.
//!
//! # Architektur
//!
//! ```text
//! /var/lib/bareos/catalog/
//!   schema/
//!     v001_initial.json          ← Basis-Schema
//!     v002_add_encryption.json   ← Migration
//!     v003_add_dedup.json        ← Migration
//!   current_version.json         ← Aktuelle Schema-Version
//! ```
//!
//! Jede Schema-Datei beschreibt Tabellen, Spalten, Typen, Indizes, Constraints.
//! Das ORM:
//! 1. Liest die JSON-Schema-Dateien
//! 2. Generiert DDL-SQL daraus
//! 3. Führt Migrationen durch (Version N → N+1)
//! 4. Validiert das Live-Schema gegen die JSON-Definition
//! 5. Generiert Rust-Structs (optional, zur Compile-Zeit)

pub mod schema;
pub mod migration;
pub mod codegen;
pub mod error;

pub use schema::{CatalogSchema, TableDef, ColumnDef, IndexDef};
pub use migration::Migrator;
pub use error::OrmError;
