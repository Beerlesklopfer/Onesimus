//! Console session — BNET command loop with FSM.
//!
//! Each console session runs in its own named thread (dir:session:N).
//! The session FSM handles authentication, command loop, and optional
//! gRPC upgrade.

use std::io::{Read, Write};
use std::sync::Arc;

use tracing::{info, debug, warn, error};

use bareos_bnet::{BnetStream, BnetSignal, BnetMessage, BnetError, CramMd5Auth};
use bareos_bnet::tls::TlsOrPlain;

use crate::command_ast::{CommandAst, Argument, CommandSource, ResponseFormat};
use crate::command_executor::{self, ExecContext};
use crate::command_queue::CommandQueue;
use crate::director_config::SharedConfig;
use crate::fsm::SessionState;
use crate::session_manager::SessionManager;

/// Handle a console session (BNET mode).
pub fn handle_console_session<S: Read + Write>(
    mut bnet: BnetStream<S>,
    session_id: u64,
    console_name: &str,
    password: &str,
    session_mgr: &Arc<SessionManager>,
    cmd_queue: &CommandQueue,
    config: &Option<SharedConfig>,
) {
    // Register session
    let mgr_session_id = session_mgr.create_session(console_name);

    // CRAM-MD5 two-way authentication (Director is server = initiated_by_remote=true)
    if !password.is_empty() {
        let auth = CramMd5Auth::new(password, "bareos-dir", 0);
        match auth.two_way_authenticate(&mut bnet, true) {
            Ok(result) if result.success => {
                debug!("session {session_id}: CRAM-MD5 auth OK");
                session_mgr.authenticate_session(mgr_session_id);
            }
            Ok(_) => {
                warn!("session {session_id}: CRAM-MD5 auth failed for '{console_name}'");
                session_mgr.remove_session(mgr_session_id);
                return;
            }
            Err(e) => {
                warn!("session {session_id}: CRAM-MD5 error: {e}");
                session_mgr.remove_session(mgr_session_id);
                return;
            }
        }
    } else {
        // No password configured — allow unauthenticated
        session_mgr.authenticate_session(mgr_session_id);
    }

    // Post-18.2 protocol: send MessageID-based responses
    // Format: "<id><RS><payload>" where RS = 0x1e (Record Separator)
    let version = env!("CARGO_PKG_VERSION");
    let rs = "\x1e";

    // 1. kMessageIdOk (1000) with version info
    let ok_msg = format!("1000{rs}OK: bareos-dir Version: {version}");
    if bnet.send_str(&ok_msg).is_err() {
        session_mgr.remove_session(mgr_session_id);
        return;
    }

    // 2. kMessageIdInfoMessage (1002) with connection info
    let info_msg = format!("1002{rs}You are connected using the default console");
    if bnet.send_str(&info_msg).is_err() {
        session_mgr.remove_session(mgr_session_id);
        return;
    }

    let mut state = SessionState::CommandLoop;
    let mut api_mode = command_executor::ApiMode::Human;

    // Subscribe to MessageBus for job messages, status updates, etc.
    let bus_rx = session_mgr.bus().subscribe();

    // Command loop
    loop {
        // Send main prompt
        if bnet.send_signal(BnetSignal::MainPrompt).is_err() {
            break;
        }

        // Receive command
        let msg = match bnet.recv() {
            Ok(BnetMessage::Data(data)) => {
                match String::from_utf8(data) {
                    Ok(s) => s,
                    Err(_) => continue,
                }
            }
            Ok(BnetMessage::Signal(BnetSignal::Terminate)) => {
                debug!("session {session_id}: client sent TERMINATE");
                break;
            }
            Ok(BnetMessage::Signal(_)) => continue,
            Err(BnetError::HardEof) | Err(BnetError::Terminated) => {
                debug!("session {session_id}: connection closed");
                break;
            }
            Err(e) => {
                // Connection reset / broken pipe = normales Session-Ende (kein Fehler)
                debug!("session {session_id}: recv: {e}");
                break;
            }
        };

        let cmd_str = msg.trim();
        if cmd_str.is_empty() {
            continue;
        }

        debug!("session {session_id}: command '{cmd_str}'");

        // Parse command into AST
        let ast = parse_cli_command(cmd_str);

        // Check for quit
        if matches!(ast.command_name.as_str(), "quit" | "exit" | ".quit") {
            let _ = bnet.send_str("bye\n");
            let _ = bnet.send_signal(BnetSignal::Terminate);
            break;
        }

        // Execute command
        // Build catalog URL from config if available
        let catalog_url = config.as_ref().and_then(|c| c.read().ok()).and_then(|cfg| {
            cfg.catalogs.first().map(|cat| {
                // URL-encode Passwort (Sonderzeichen wie @, :, / etc.)
                let pass = if cat.db_password.is_empty() {
                    String::new()
                } else {
                    let encoded: String = cat.db_password.bytes().map(|b| {
                        if b.is_ascii_alphanumeric() || b == b'-' || b == b'_' || b == b'.' || b == b'~' {
                            format!("{}", b as char)
                        } else {
                            format!("%{:02X}", b)
                        }
                    }).collect();
                    format!(":{}", encoded)
                };

                // Leere/localhost Adresse → Unix-Socket (peer auth, wie C++ libpq)
                // Explizite Adresse → TCP
                if cat.db_address.is_empty()
                    || cat.db_address == "localhost"
                    || cat.db_address == "127.0.0.1"
                {
                    // Unix-Socket: sqlx braucht einen host in der URL + socket path
                    format!("postgres://{}{}@localhost/{db}?host=/var/run/postgresql",
                        cat.db_user, pass, db = cat.db_name)
                } else {
                    format!("postgres://{}{}@{}:{}/{}",
                        cat.db_user, pass, cat.db_address, cat.db_port, cat.db_name)
                }
            })
        });

        let ctx = ExecContext {
            session_id,
            config: config.clone(),
            session_mgr: Some(session_mgr.clone()),
            catalog_url,
            api_mode,
            bus_rx: Some(bus_rx.clone()),
            job_manager: None, // TODO: pass from main
        };
        let result = command_executor::dispatch(&ast, &ctx);

        // .api command changes session api_mode
        if ast.command_name == ".api" {
            if let Some(mode_str) = ast.arguments.first().map(|a| a.value.as_deref().unwrap_or(a.key.as_str())) {
                api_mode = match mode_str {
                    "0" | "off" => command_executor::ApiMode::Human,
                    "1" | "on" => command_executor::ApiMode::Api,
                    "2" | "json" => command_executor::ApiMode::Json,
                    _ => api_mode,
                };
            }
        }

        // Send result
        if !result.text_output.is_empty() {
            if bnet.send_str(&result.text_output).is_err() {
                break;
            }
        }
        if !result.error_message.is_empty() {
            if bnet.send_str(&result.error_message).is_err() {
                break;
            }
        }

        // Send EOD after command output
        if bnet.send_signal(BnetSignal::Eod).is_err() {
            break;
        }

        // Drain and send any pending bus messages
        // TODO(R8-full): Drain session bus_rx and send as console text
    }

    // Cleanup
    session_mgr.remove_session(mgr_session_id);
    info!("session {session_id} ({console_name}) ended");
}

/// Parse a CLI command string into a CommandAST.
/// Simple parser: first word is command name, rest are key=value arguments.
fn parse_cli_command(input: &str) -> CommandAst {
    let mut parts = input.splitn(2, char::is_whitespace);
    let command_name = parts.next().unwrap_or("").to_string();
    let args_str = parts.next().unwrap_or("");

    let mut arguments = Vec::new();

    // Parse arguments: key=value or bare words
    for token in args_str.split_whitespace() {
        if let Some(eq_pos) = token.find('=') {
            let key = token[..eq_pos].to_string();
            let value = token[eq_pos + 1..].trim_matches('"').to_string();
            arguments.push(Argument {
                key,
                value: Some(value),
            });
        } else {
            arguments.push(Argument {
                key: token.to_string(),
                value: None,
            });
        }
    }

    CommandAst {
        command_name,
        arguments,
        session_id: String::new(),
        source: CommandSource::Cli,
        response_format: ResponseFormat::Default,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_simple_command() {
        let ast = parse_cli_command("status dir");
        assert_eq!(ast.command_name, "status");
        assert_eq!(ast.arguments.len(), 1);
        assert_eq!(ast.arguments[0].key, "dir");
        assert!(ast.arguments[0].value.is_none());
    }

    #[test]
    fn parse_command_with_args() {
        let ast = parse_cli_command("run job=BackupJob level=Full");
        assert_eq!(ast.command_name, "run");
        assert_eq!(ast.arguments.len(), 2);
        assert_eq!(ast.arguments[0].key, "job");
        assert_eq!(ast.arguments[0].value.as_deref(), Some("BackupJob"));
        assert_eq!(ast.arguments[1].key, "level");
        assert_eq!(ast.arguments[1].value.as_deref(), Some("Full"));
    }

    #[test]
    fn parse_bare_command() {
        let ast = parse_cli_command("version");
        assert_eq!(ast.command_name, "version");
        assert!(ast.arguments.is_empty());
    }

    #[test]
    fn parse_dot_command() {
        let ast = parse_cli_command(".clients");
        assert_eq!(ast.command_name, ".clients");
        assert!(ast.arguments.is_empty());
    }

    #[test]
    fn parse_quoted_value_no_spaces() {
        let ast = parse_cli_command("run job=\"BackupJob\"");
        assert_eq!(ast.arguments[0].key, "job");
        assert_eq!(ast.arguments[0].value.as_deref(), Some("BackupJob"));
    }

    #[test]
    fn parse_empty_input() {
        let ast = parse_cli_command("");
        assert_eq!(ast.command_name, "");
        assert!(ast.arguments.is_empty());
    }
}
