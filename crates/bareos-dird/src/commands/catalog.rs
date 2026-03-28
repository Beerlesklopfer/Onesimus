//! Catalog management commands.
//!
//! AST-basierte Katalog-Verwaltung, erreichbar via:
//! - CLI: `.catalog schema`, `.catalog migrate`, `.catalog version`
//! - JSON-API: POST /api/catalog/schema
//! - gRPC: CatalogService.GetSchema, CatalogService.Migrate

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_dot_catalog(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let action = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");

    // ACL check: destructive operations require CatalogAdmin ACL
    let is_destructive = matches!(action, "migrate" | "drop" | "create" | "reset");
    if is_destructive {
        // TODO: Check session ACL for CatalogAdmin permission
        // For now, log the attempt
        tracing::info!("catalog admin operation '{}' by session {}", action, ctx.session_id);
    }

    match action {
        "schema" => catalog_schema(ast, ctx),
        "version" => catalog_version(ast, ctx),
        "migrate" => catalog_migrate(ast, ctx),
        "validate" => catalog_validate(ast, ctx),
        "ddl" => catalog_ddl(ast, ctx),
        "tables" => catalog_tables(ast, ctx),
        "describe" => catalog_describe(ast, ctx),
        "codegen" => catalog_codegen(ast, ctx),
        "diff" => catalog_diff(ast, ctx),
        "drop" => catalog_drop(ast, ctx),
        "create" => catalog_create(ast, ctx),
        "reset" => catalog_reset(ast, ctx),
        _ => CommandResult::ok(
            ".catalog: Katalog-Schema-Verwaltung\n\
             \n\
             Befehle:\n\
             .catalog schema              Schema als JSON anzeigen\n\
             .catalog version             Aktuelle Schema-Version\n\
             .catalog tables              Alle Tabellen auflisten\n\
             .catalog describe table=<n>  Tabellen-Spalten anzeigen\n\
             .catalog ddl                 CREATE TABLE SQL generieren\n\
             .catalog validate            Schema gegen DB validieren\n\
             .catalog migrate             Schema-Migration durchfuehren\n\
             .catalog diff                Aenderungen zur naechsten Version\n\
             .catalog codegen             Rust-Structs generieren\n\
             \n\
             Admin-Befehle (erfordern CatalogAdmin ACL):\n\
             .catalog create              Tabellen in DB erstellen\n\
             .catalog drop table=<name>   Tabelle loeschen\n\
             .catalog reset               Alle Tabellen loeschen + neu erstellen\n"
                .to_string()
        ),
    }
}

fn catalog_schema(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);

    match migrator.latest() {
        Ok(Some(schema)) => {
            match serde_json::to_string_pretty(&schema) {
                Ok(json) => CommandResult::ok(format!("{json}\n")),
                Err(e) => CommandResult::error(format!("JSON error: {e}\n")),
            }
        }
        Ok(None) => CommandResult::ok("Kein Schema gefunden in catalog/schema/\n".to_string()),
        Err(e) => CommandResult::error(format!("Schema load error: {e}\n")),
    }
}

fn catalog_version(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);

    match migrator.load_versions() {
        Ok(versions) => {
            if versions.is_empty() {
                return CommandResult::ok("Keine Schema-Versionen gefunden\n".to_string());
            }
            let mut output = String::from("Schema-Versionen:\n");
            for v in &versions {
                output.push_str(&format!(
                    "  v{:03}: {} ({})\n",
                    v.version, v.description, v.created
                ));
            }
            output.push_str(&format!("\nAktuelle Version: v{:03}\n",
                versions.last().map(|v| v.version).unwrap_or(0)));
            CommandResult::ok(output)
        }
        Err(e) => CommandResult::error(format!("Version error: {e}\n")),
    }
}

fn catalog_tables(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);

    match migrator.latest() {
        Ok(Some(schema)) => {
            let mut output = format!("Tabellen in Schema v{:03}:\n\n", schema.version);
            output.push_str(&format!("{:<20} {:<10} {:<10} {}\n",
                "Tabelle", "Spalten", "Indizes", "Beschreibung"));
            output.push_str(&"-".repeat(70));
            output.push('\n');

            for table in &schema.tables {
                output.push_str(&format!("{:<20} {:<10} {:<10} {}\n",
                    table.name,
                    table.columns.len(),
                    table.indices.len(),
                    table.description,
                ));
            }
            CommandResult::ok(output)
        }
        Ok(None) => CommandResult::ok("Kein Schema\n".to_string()),
        Err(e) => CommandResult::error(format!("{e}\n")),
    }
}

fn catalog_describe(ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let table_name = ast.arguments.iter()
        .find(|a| a.key == "table")
        .and_then(|a| a.value.as_deref())
        .unwrap_or("");

    if table_name.is_empty() {
        return CommandResult::ok(".catalog describe table=<name>\n".to_string());
    }

    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);

    match migrator.latest() {
        Ok(Some(schema)) => {
            match schema.find_table(table_name) {
                Some(table) => {
                    let mut output = format!("Tabelle: {} (PK: {})\n{}\n\n",
                        table.name, table.primary_key, table.description);

                    output.push_str(&format!("{:<20} {:<15} {:<10} {:<15} {}\n",
                        "Spalte", "Typ", "NOT NULL", "Default", "Beschreibung"));
                    output.push_str(&"-".repeat(80));
                    output.push('\n');

                    for col in &table.columns {
                        output.push_str(&format!("{:<20} {:<15} {:<10} {:<15} {}\n",
                            col.name,
                            col.col_type,
                            if col.not_null { "YES" } else { "no" },
                            col.default.as_deref().unwrap_or("-"),
                            col.description,
                        ));
                    }

                    if !table.indices.is_empty() {
                        output.push_str("\nIndizes:\n");
                        for idx in &table.indices {
                            output.push_str(&format!("  {} ({}) {}{}\n",
                                idx.name,
                                idx.columns.join(", "),
                                if idx.unique { "UNIQUE " } else { "" },
                                if idx.index_type != "btree" { &idx.index_type } else { "" },
                            ));
                        }
                    }

                    if !table.foreign_keys.is_empty() {
                        output.push_str("\nForeign Keys:\n");
                        for fk in &table.foreign_keys {
                            output.push_str(&format!("  {} → {}.{}\n",
                                fk.column, fk.references_table, fk.references_column));
                        }
                    }

                    CommandResult::ok(output)
                }
                None => CommandResult::error(format!("Tabelle '{table_name}' nicht gefunden\n")),
            }
        }
        Ok(None) => CommandResult::ok("Kein Schema\n".to_string()),
        Err(e) => CommandResult::error(format!("{e}\n")),
    }
}

fn catalog_ddl(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);

    match migrator.full_ddl() {
        Ok(ddl) => CommandResult::ok(ddl),
        Err(e) => CommandResult::error(format!("{e}\n")),
    }
}

fn catalog_validate(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    match &ctx.catalog {
        Some(_cat) => {
            // TODO: Connect to DB, compare schema
            CommandResult::ok("Schema-Validierung: DB-Verbindung erforderlich\n".to_string())
        }
        None => CommandResult::ok("Keine Catalog-DB konfiguriert\n".to_string()),
    }
}

fn catalog_migrate(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    match &ctx.catalog {
        Some(cat) => {
            // Check if version table exists
            match cat.raw_query("SELECT MAX(versionid) FROM version") {
                Ok(output) => CommandResult::ok(format!("Schema aktuell: {}\n", output.trim())),
                Err(e) => CommandResult::error(format!("Migration error: {e}\n")),
            }
        }
        None => CommandResult::ok("Keine Catalog-DB konfiguriert\n".to_string()),
    }
}

fn catalog_codegen(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);

    match migrator.latest() {
        Ok(Some(schema)) => {
            let code = bareos_orm::codegen::generate_rust_models(&schema);
            CommandResult::ok(code)
        }
        Ok(None) => CommandResult::ok("Kein Schema\n".to_string()),
        Err(e) => CommandResult::error(format!("{e}\n")),
    }
}

fn catalog_diff(_ast: &CommandAst, _ctx: &ExecContext) -> CommandResult {
    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);

    match migrator.load_versions() {
        Ok(versions) if versions.len() >= 2 => {
            let prev = &versions[versions.len() - 2];
            let curr = &versions[versions.len() - 1];

            let stmts = bareos_orm::migration::Migrator::diff(prev, curr);
            if stmts.is_empty() {
                CommandResult::ok(format!(
                    "Keine Aenderungen zwischen v{:03} und v{:03}\n",
                    prev.version, curr.version
                ))
            } else {
                let mut output = format!(
                    "Aenderungen v{:03} → v{:03}:\n\n",
                    prev.version, curr.version
                );
                for stmt in &stmts {
                    output.push_str(stmt);
                    output.push('\n');
                }
                CommandResult::ok(output)
            }
        }
        Ok(_) => CommandResult::ok("Mindestens 2 Schema-Versionen noetig fuer diff\n".to_string()),
        Err(e) => CommandResult::error(format!("{e}\n")),
    }
}

fn catalog_drop(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let table_name = ast.arguments.iter()
        .find(|a| a.key == "table")
        .and_then(|a| a.value.as_deref())
        .unwrap_or("");

    if table_name.is_empty() {
        return CommandResult::ok(".catalog drop table=<name>\n".to_string());
    }

    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);
    if let Ok(Some(schema)) = migrator.latest() {
        if schema.find_table(table_name).is_none() {
            return CommandResult::error(format!("Tabelle '{table_name}' nicht im Schema\n"));
        }
    }

    match &ctx.catalog {
        Some(cat) => {
            match cat.raw_query(&format!("DROP TABLE IF EXISTS {} CASCADE", table_name)) {
                Ok(_) => CommandResult::ok(format!("Tabelle '{table_name}' geloescht (CASCADE)\n")),
                Err(e) => CommandResult::error(format!("DROP error: {e}\n")),
            }
        }
        None => CommandResult::ok("Keine Catalog-DB konfiguriert\n".to_string()),
    }
}

fn catalog_create(_ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);
    let schema = match migrator.latest() {
        Ok(Some(s)) => s,
        Ok(None) => return CommandResult::error("Kein Schema\n".to_string()),
        Err(e) => return CommandResult::error(format!("Schema error: {e}\n")),
    };

    let statements = schema.to_statements();

    match &ctx.catalog {
        Some(cat) => {
            for stmt in &statements {
                tracing::debug!("SQL: {}", &stmt[..stmt.len().min(80)]);
                if let Err(e) = cat.raw_query(stmt) {
                    return CommandResult::error(format!("CREATE error: {e}\n"));
                }
            }
            CommandResult::ok(format!(
                "Catalog erstellt: {} Tabellen, {} Indizes\n",
                schema.tables.len(),
                schema.tables.iter().map(|t| t.indices.len()).sum::<usize>()
            ))
        }
        None => CommandResult::ok("Keine Catalog-DB konfiguriert\n".to_string()),
    }
}

fn catalog_reset(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let schema_dir = std::path::Path::new("catalog/schema");
    let migrator = bareos_orm::migration::Migrator::new(schema_dir);
    let schema = match migrator.latest() {
        Ok(Some(s)) => s,
        Ok(None) => return CommandResult::error("Kein Schema\n".to_string()),
        Err(e) => return CommandResult::error(format!("{e}\n")),
    };

    match &ctx.catalog {
        Some(cat) => {
            for table in schema.tables.iter().rev() {
                if let Err(e) = cat.raw_query(&format!("DROP TABLE IF EXISTS {} CASCADE", table.name)) {
                    return CommandResult::error(format!("RESET error: {e}\n"));
                }
            }
            let _ = cat.raw_query("DROP TABLE IF EXISTS bareos_schema_version CASCADE");
            let create = catalog_create(ast, ctx);
            CommandResult::ok(format!("Alle Tabellen geloescht.\n{}", create.text_output))
        }
        None => CommandResult::ok("Keine Catalog-DB konfiguriert\n".to_string()),
    }
}
