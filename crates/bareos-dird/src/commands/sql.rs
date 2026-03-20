//! SQL commands: .sql, sqlquery
//!
//! Execute raw SQL against the PostgreSQL catalog.

use crate::command_ast::CommandAst;
use crate::command_executor::ExecContext;
use crate::command_queue::CommandResult;

pub fn cmd_dot_sql(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    let query = ast.arguments.iter()
        .find(|a| a.key == "query")
        .and_then(|a| a.value.as_deref());

    let query = match query {
        Some(q) => q,
        None => {
            let sub = ast.arguments.first().map(|a| a.key.as_str()).unwrap_or("");
            match sub {
                "tables" => return sql_tables(ctx),
                "schema" => return sql_schema(ctx),
                _ => return CommandResult::ok(
                    ".sql: specify one of:\n  .sql query=\"SELECT ...\"\n  .sql tables\n  .sql schema\n"
                        .to_string()
                ),
            }
        }
    };

    // Execute SQL query
    match &ctx.catalog_url {
        Some(url) => execute_sql(query, url),
        None => CommandResult::ok(format!("No catalog configured. Query: {query}\n")),
    }
}

pub fn cmd_sql_query(ast: &CommandAst, ctx: &ExecContext) -> CommandResult {
    // "sqlquery" expects the query as all remaining arguments joined
    let query: String = ast.arguments.iter()
        .map(|a| {
            if let Some(v) = &a.value {
                format!("{}={}", a.key, v)
            } else {
                a.key.clone()
            }
        })
        .collect::<Vec<_>>()
        .join(" ");

    if query.is_empty() {
        return CommandResult::ok("sqlquery: enter SQL query\n".to_string());
    }

    match &ctx.catalog_url {
        Some(url) => execute_sql(&query, url),
        None => CommandResult::ok(format!("No catalog configured. Query: {query}\n")),
    }
}

fn execute_sql(query: &str, url: &str) -> CommandResult {
    // Use a blocking runtime for the async sqlx query
    let rt = match tokio::runtime::Runtime::new() {
        Ok(rt) => rt,
        Err(e) => return CommandResult::error(format!("Runtime error: {e}\n")),
    };

    let result = rt.block_on(async {
        let pool = sqlx::postgres::PgPoolOptions::new()
            .max_connections(1)
            .connect(url)
            .await?;

        let rows = sqlx::query(query)
            .fetch_all(&pool)
            .await?;

        let mut output = String::new();

        if rows.is_empty() {
            output.push_str("No results\n");
        } else {
            use sqlx::{Row, Column};
            // Header
            let columns = rows[0].columns();
            let headers: Vec<String> = columns.iter().map(|c| c.name().to_string()).collect();
            output.push_str(&headers.iter().map(|h| format!("{:<20}", h)).collect::<String>());
            output.push('\n');
            output.push_str(&"-".repeat(headers.len() * 20));
            output.push('\n');

            // Rows
            for row in &rows {
                for col in columns {
                    let val: String = row.try_get::<String, _>(col.ordinal())
                        .or_else(|_| row.try_get::<i64, _>(col.ordinal()).map(|v| v.to_string()))
                        .or_else(|_| row.try_get::<i32, _>(col.ordinal()).map(|v| v.to_string()))
                        .or_else(|_| row.try_get::<bool, _>(col.ordinal()).map(|v| v.to_string()))
                        .unwrap_or_else(|_| "NULL".to_string());
                    output.push_str(&format!("{:<20}", val));
                }
                output.push('\n');
            }
        }

        Ok::<String, sqlx::Error>(output)
    });

    match result {
        Ok(output) => CommandResult::ok(output),
        Err(e) => CommandResult::error(format!("SQL error: {e}\n")),
    }
}

fn sql_tables(ctx: &ExecContext) -> CommandResult {
    match &ctx.catalog_url {
        Some(url) => execute_sql(
            "SELECT tablename FROM pg_catalog.pg_tables WHERE schemaname = 'public' ORDER BY tablename",
            url,
        ),
        None => CommandResult::ok("No catalog configured\n".to_string()),
    }
}

fn sql_schema(ctx: &ExecContext) -> CommandResult {
    match &ctx.catalog_url {
        Some(url) => execute_sql(
            "SELECT table_name, column_name, data_type FROM information_schema.columns \
             WHERE table_schema = 'public' ORDER BY table_name, ordinal_position",
            url,
        ),
        None => CommandResult::ok("No catalog configured\n".to_string()),
    }
}
