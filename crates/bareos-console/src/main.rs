//! baReos bconsole — Interactive console client for the Bareos Director.
//!
//! Connects via BNET (TCP + TLS + CRAM-MD5) or gRPC.

use std::io::{self, BufRead, Write};
use std::net::TcpStream;
use std::time::Duration;

use clap::Parser;
use tracing::{info, error, debug};
use tracing_subscriber::EnvFilter;
use rustyline::completion::{Completer, Pair};
use rustyline::error::ReadlineError;
use rustyline::highlight::Highlighter;
use rustyline::hint::Hinter;
use rustyline::validate::Validator;
use rustyline::{Editor, Helper, Context};

use bareos_bnet::{BnetStream, BnetSignal, BnetMessage, BnetError};
use bareos_bnet::hello::make_console_hello;
use bareos_bnet::cram_md5::CramMd5Auth;

/// baReos bconsole — Bareos Console Client
#[derive(Parser, Debug)]
#[command(
    name = "brconsole",
    version,
    about = "The baReos Console.\n\nCopyright (C) 2026 Onesomus.io — Rust Version"
)]
struct Cli {
    /// Use <path> as configuration file.
    #[arg(short, long, default_value = "/etc/bareos/brconsole.conf")]
    config: Option<String>,

    /// Director address (overrides config)
    #[arg(short = 'a', long)]
    address: Option<String>,

    /// Director port (overrides config)
    #[arg(short, long)]
    port: Option<u16>,

    /// Console name (overrides config)
    #[arg(short, long)]
    name: Option<String>,

    /// Password (overrides config)
    #[arg(short = 'w', long)]
    password: Option<String>,

    /// Use gRPC protocol
    #[arg(long)]
    grpc: bool,

    /// Execute single command and exit
    #[arg(short = 'e', long)]
    command: Option<String>,
}

/// Parsed config from brconsole.conf
struct ConsoleConfig {
    address: String,
    port: u16,
    name: String,
    password: String,
}

/// Effective CLI — resolved from config file + CLI overrides
struct EffCli {
    address: String,
    port: u16,
    name: String,
    password: String,
    grpc: bool,
    command: Option<String>,
}

fn load_console_config(path: &str) -> ConsoleConfig {
    let mut cfg = ConsoleConfig {
        address: "localhost".to_string(),
        port: 9101,
        name: "*UserAgent*".to_string(),
        password: String::new(),
    };

    let content = match std::fs::read_to_string(path) {
        Ok(c) => c,
        Err(_) => return cfg,
    };

    for line in content.lines() {
        let line = line.trim();
        if line.starts_with('#') || line.is_empty() { continue; }
        if let Some(pos) = line.find('=') {
            let key = line[..pos].trim().to_lowercase();
            let val = line[pos+1..].trim().trim_matches('"').to_string();
            match key.as_str() {
                "address" => cfg.address = val,
                "port" | "dirport" => cfg.port = val.parse().unwrap_or(9101),
                "name" => cfg.name = val,
                "password" => cfg.password = val,
                _ => {}
            }
        }
    }

    cfg
}

fn main() {
    let cli = Cli::parse();

    // Load config file, then override with CLI args
    let file_cfg = load_console_config(
        cli.config.as_deref().unwrap_or("/etc/bareos/brconsole.conf")
    );

    // CLI args override config file
    let address = cli.address.unwrap_or(file_cfg.address);
    let port = cli.port.unwrap_or(file_cfg.port);
    let name = cli.name.unwrap_or(file_cfg.name);
    let password = cli.password.unwrap_or(file_cfg.password);

    tracing_subscriber::fmt()
        .with_env_filter(
            EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| EnvFilter::new("warn")),
        )
        .init();

    // Build effective CLI with resolved values
    let eff = EffCli {
        address: address.clone(),
        port,
        name: name.clone(),
        password: password.clone(),
        grpc: cli.grpc,
        command: cli.command.clone(),
    };

    let addr = format!("{}:{}", eff.address, eff.port);

    if eff.grpc {
        run_grpc_mode(&eff, &addr);
    } else {
        run_bnet_mode(&eff, &addr);
    }
}

fn run_bnet_mode(cli: &EffCli, addr: &str) {
    // Connect
    eprintln!("Connecting to Director {addr}");
    let stream = match TcpStream::connect(addr) {
        Ok(s) => s,
        Err(e) => {
            eprintln!("Connection failed: {e}");
            std::process::exit(1);
        }
    };
    stream.set_read_timeout(Some(Duration::from_secs(30))).ok();

    // Try TLS-PSK if password is set
    if !cli.password.is_empty() {
        let psk_identity = format!("R_CONSOLE\x1e{}", cli.name);
        let psk = bareos_bnet::tls_psk::password_to_psk(&cli.password);
        match bareos_bnet::tls_psk::psk_connect(stream, &psk_identity, &psk) {
            Ok(psk_stream) => {
                eprintln!(" Encryption: TLS-PSK");
                let mut bnet = BnetStream::new(psk_stream);
                run_bnet_session(cli, bnet);
                return;
            }
            Err(e) => {
                debug!("TLS-PSK failed ({e}), reconnecting plaintext");
                let stream2 = TcpStream::connect(addr).unwrap();
                stream2.set_read_timeout(Some(Duration::from_secs(30))).ok();
                let mut bnet = BnetStream::new(stream2);
                run_bnet_session(cli, bnet);
                return;
            }
        }
    }
    let mut bnet = BnetStream::new(stream);
    run_bnet_session(cli, bnet);
}

fn run_bnet_session<S: std::io::Read + std::io::Write>(cli: &EffCli, mut bnet: BnetStream<S>) {
    // Send hello
    let version = env!("CARGO_PKG_VERSION");
    let hello = if cli.name == "*UserAgent*" {
        make_console_hello("*UserAgent*", "", cli.grpc)
    } else {
        make_console_hello(&cli.name, version, cli.grpc)
    };
    if let Err(e) = bnet.send_str(&hello) {
        eprintln!("Failed to send hello: {e}");
        std::process::exit(1);
    }

    // CRAM-MD5 authentication (if password provided)
    if !cli.password.is_empty() {
        let auth = CramMd5Auth::new(&cli.password, &cli.name, 0);
        match auth.two_way_authenticate(&mut bnet, false) {
            Ok(result) if result.success => {
                debug!("authentication successful");
            }
            Ok(_) => {
                eprintln!("Authentication failed");
                std::process::exit(1);
            }
            Err(e) => {
                eprintln!("Authentication error: {e}");
                std::process::exit(1);
            }
        }
    }

    // Receive banner
    match bnet.recv_str() {
        Ok(banner) => {
            eprintln!("{}", banner.trim());
        }
        Err(e) => {
            eprintln!("Failed to receive banner: {e}");
            std::process::exit(1);
        }
    }

    // Single command mode
    if let Some(cmd) = &cli.command {
        execute_single_command(&mut bnet, cmd);
        return;
    }

    // Interactive mode
    interactive_loop(&mut bnet, &cli.name, "bareos-dir");
}

fn execute_single_command<S: std::io::Read + std::io::Write>(bnet: &mut BnetStream<S>, cmd: &str) {
    // Wait for prompt
    drain_signals(bnet);

    // Send command
    if bnet.send_str(cmd).is_err() {
        return;
    }

    // Read response until EOD
    loop {
        match bnet.recv() {
            Ok(BnetMessage::Data(data)) => {
                let text = String::from_utf8_lossy(&data);
                print!("{text}");
            }
            Ok(BnetMessage::Signal(BnetSignal::Eod)) => break,
            Ok(BnetMessage::Signal(BnetSignal::MainPrompt)) => break,
            Ok(BnetMessage::Signal(BnetSignal::Terminate)) => break,
            Ok(BnetMessage::Signal(_)) => continue,
            Err(_) => break,
        }
    }

    // Send quit
    let _ = bnet.send_str("quit");
}

/// rustyline Helper with tab-completion (from Director .help) and syntax coloring.
struct BareosHelper {
    /// Commands loaded from Director via .help at session start
    commands: Vec<String>,
}

impl BareosHelper {
    fn new() -> Self {
        Self { commands: Vec::new() }
    }

    /// Load available commands from Director via .help response
    fn load_commands_from<S: std::io::Read + std::io::Write>(&mut self, bnet: &mut BnetStream<S>) {
        // Send .help to get command list
        if bnet.send_str(".help").is_err() { return; }

        let mut output = String::new();
        loop {
            match bnet.recv() {
                Ok(BnetMessage::Data(data)) => {
                    output.push_str(&String::from_utf8_lossy(&data));
                }
                Ok(BnetMessage::Signal(BnetSignal::Eod)) => break,
                Ok(BnetMessage::Signal(BnetSignal::MainPrompt)) => break,
                _ => break,
            }
        }

        // Parse: each line is "command\thelp\tusage"
        self.commands = output.lines()
            .filter_map(|line| {
                let cmd = line.split('\t').next()?.trim();
                if cmd.is_empty() { None } else { Some(cmd.to_string()) }
            })
            .collect();

        if self.commands.is_empty() {
            // Fallback: basic commands
            self.commands = vec![
                "version", "time", "quit", "exit", "help", ".help",
                "status", "run", "cancel", "list", "reload",
            ].into_iter().map(String::from).collect();
        }
    }
}

impl Completer for BareosHelper {
    type Candidate = Pair;

    fn complete(&self, line: &str, pos: usize, _ctx: &Context<'_>) -> rustyline::Result<(usize, Vec<Pair>)> {
        let line = &line[..pos];
        let parts: Vec<&str> = line.split_whitespace().collect();

        if parts.is_empty() || (parts.len() == 1 && !line.ends_with(' ')) {
            // Complete command name from Director's command list
            let prefix = parts.first().copied().unwrap_or("");
            let matches: Vec<Pair> = self.commands.iter()
                .filter(|c| c.starts_with(prefix))
                .map(|c| Pair { display: c.to_string(), replacement: c.to_string() })
                .collect();
            Ok((0, matches))
        } else {
            // No argument completion for now — could query Director
            Ok((pos, Vec::new()))
        }
    }
}

impl Highlighter for BareosHelper {
    fn highlight<'l>(&self, line: &'l str, _pos: usize) -> std::borrow::Cow<'l, str> {
        let parts: Vec<&str> = line.splitn(2, ' ').collect();
        let cmd = parts[0];

        if self.commands.iter().any(|c| c == cmd) {
            // Green for valid command, rest normal
            if parts.len() > 1 {
                std::borrow::Cow::Owned(format!("\x1b[32m{}\x1b[0m {}", cmd, parts[1]))
            } else {
                std::borrow::Cow::Owned(format!("\x1b[32m{}\x1b[0m", cmd))
            }
        } else if cmd.starts_with('.') {
            // Yellow for dot-commands
            std::borrow::Cow::Owned(format!("\x1b[33m{}\x1b[0m", line))
        } else if !cmd.is_empty() {
            // Red for unknown
            std::borrow::Cow::Owned(format!("\x1b[31m{}\x1b[0m", line))
        } else {
            std::borrow::Cow::Borrowed(line)
        }
    }

    fn highlight_prompt<'b, 's: 'b, 'p: 'b>(&'s self, prompt: &'p str, _default: bool) -> std::borrow::Cow<'b, str> {
        std::borrow::Cow::Owned(format!("\x1b[1;36m{}\x1b[0m", prompt))  // Cyan bold prompt
    }

    fn highlight_char(&self, _line: &str, _pos: usize, _kind: rustyline::highlight::CmdKind) -> bool {
        true
    }
}

impl Hinter for BareosHelper {
    type Hint = String;
}

impl Validator for BareosHelper {}

impl Helper for BareosHelper {}

fn interactive_loop<S: std::io::Read + std::io::Write>(
    bnet: &mut BnetStream<S>,
    console_name: &str,
    director_name: &str,
) {
    // Load command list from Director for tab-completion
    let mut helper = BareosHelper::new();
    drain_signals(bnet); // consume initial MainPrompt
    helper.load_commands_from(bnet);
    eprintln!("\x1b[2m({} commands loaded for tab-completion)\x1b[0m", helper.commands.len());

    let mut rl = Editor::new().unwrap();
    rl.set_helper(Some(helper));

    // Load history
    let history_file = dirs_home().join(".brconsole_history");
    let _ = rl.load_history(&history_file);

    // Prompt: console@director[version]>
    let version = env!("CARGO_PKG_VERSION");
    let prompt = format!("{}@{}[{}]> ", console_name, director_name, version);

    loop {
        // Drain any pending signals/prompts
        drain_signals(bnet);

        // Read with tab-completion and coloring
        match rl.readline(&prompt) {
            Ok(line) => {
                let cmd = line.trim();
                if cmd.is_empty() { continue; }

                rl.add_history_entry(cmd).ok();

                // Send command
                if bnet.send_str(cmd).is_err() {
                    eprintln!("\x1b[31mConnection lost\x1b[0m");
                    break;
                }

                // Check for quit
                if matches!(cmd, "quit" | "exit" | ".quit") {
            // Read goodbye
            if let Ok(msg) = bnet.recv_str() {
                println!("{}", msg.trim());
            }
            break;
        }

        // Read response with colors
        loop {
            match bnet.recv() {
                Ok(BnetMessage::Data(data)) => {
                    let text = String::from_utf8_lossy(&data);
                    // Color output: errors red, warnings yellow, OK green
                    for line in text.lines() {
                        if line.contains("Error") || line.contains("FEHLER") || line.contains("failed") {
                            println!("\x1b[31m{line}\x1b[0m");
                        } else if line.contains("Warning") || line.contains("WARNUNG") {
                            println!("\x1b[33m{line}\x1b[0m");
                        } else if line.contains("OK") || line.contains("success") {
                            println!("\x1b[32m{line}\x1b[0m");
                        } else {
                            println!("{line}");
                        }
                    }
                }
                Ok(BnetMessage::Signal(BnetSignal::Eod)) => break,
                Ok(BnetMessage::Signal(BnetSignal::MainPrompt)) => break,
                Ok(BnetMessage::Signal(BnetSignal::Terminate)) => {
                    eprintln!("\x1b[31mDirector terminated connection\x1b[0m");
                    let _ = rl.save_history(&history_file);
                    return;
                }
                Ok(BnetMessage::Signal(BnetSignal::YesNo)) => {
                    match rl.readline("\x1b[33m(yes/no): \x1b[0m") {
                        Ok(answer) => { let _ = bnet.send_str(answer.trim()); }
                        Err(_) => break,
                    }
                }
                Ok(BnetMessage::Signal(BnetSignal::SubPrompt)) => {
                    match rl.readline("\x1b[36m> \x1b[0m") {
                        Ok(input) => { let _ = bnet.send_str(input.trim()); }
                        Err(_) => break,
                    }
                }
                Ok(BnetMessage::Signal(BnetSignal::TextInput)) => {
                    match rl.readline("") {
                        Ok(input) => { let _ = bnet.send_str(input.trim()); }
                        Err(_) => break,
                    }
                }
                Ok(BnetMessage::Signal(sig)) => {
                    debug!("signal: {sig:?}");
                }
                Err(BnetError::HardEof) | Err(BnetError::Terminated) => {
                    eprintln!("Connection closed");
                    return;
                }
                Err(e) => {
                    eprintln!("\x1b[31mError: {e}\x1b[0m");
                    let _ = rl.save_history(&history_file);
                    return;
                }
            }
        }
            }
            Err(ReadlineError::Interrupted) => {
                // Ctrl-C — just show new prompt
                continue;
            }
            Err(ReadlineError::Eof) => {
                // Ctrl-D — quit
                let _ = bnet.send_str("quit");
                break;
            }
            Err(e) => {
                eprintln!("\x1b[31mReadline error: {e}\x1b[0m");
                break;
            }
        }
    }

    let _ = rl.save_history(&history_file);
}

/// Drain pending signals (prompts, etc.) without blocking.
fn dirs_home() -> std::path::PathBuf {
    std::env::var("HOME")
        .map(std::path::PathBuf::from)
        .unwrap_or_else(|_| std::path::PathBuf::from("/tmp"))
}

fn drain_signals<S: std::io::Read + std::io::Write>(bnet: &mut BnetStream<S>) {
    // Read until we get MainPrompt or error
    loop {
        match bnet.recv() {
            Ok(BnetMessage::Signal(BnetSignal::MainPrompt)) => break,
            Ok(BnetMessage::Signal(_)) => continue,
            Ok(BnetMessage::Data(_)) => continue,
            Err(_) => break,
        }
    }
}

fn run_grpc_mode(cli: &EffCli, addr: &str) {
    eprintln!("gRPC mode connecting to {addr}");

    let rt = tokio::runtime::Runtime::new().unwrap();
    rt.block_on(async {
        use bareos_proto_rs::bareos::director::{
            director_client::DirectorClient,
            CommandRequest, CommandArgument, ListCommandsRequest,
        };

        let url = format!("http://{addr}");
        let mut client = match DirectorClient::connect(url.clone()).await {
            Ok(c) => {
                eprintln!("Connected to gRPC Director at {addr}");
                c
            }
            Err(e) => {
                eprintln!("gRPC connect failed: {e}");
                return;
            }
        };

        // Single command mode
        if let Some(cmd) = &cli.command {
            let parts: Vec<&str> = cmd.splitn(2, ' ').collect();
            let command_name = parts[0].to_string();
            let arguments = if parts.len() > 1 {
                parts[1].split_whitespace().map(|arg| {
                    if let Some(eq) = arg.find('=') {
                        CommandArgument {
                            key: arg[..eq].to_string(),
                            value: Some(bareos_proto_rs::bareos::director::command_argument::Value::StringValue(
                                arg[eq+1..].to_string()
                            )),
                        }
                    } else {
                        CommandArgument { key: arg.to_string(), value: None }
                    }
                }).collect()
            } else {
                Vec::new()
            };

            let request = tonic::Request::new(CommandRequest {
                command_name,
                arguments,
                session_id: String::new(),
                response_format: 0,
            });

            match client.execute_command(request).await {
                Ok(response) => {
                    let resp = response.into_inner();
                    if resp.success {
                        print!("{}", resp.text_output);
                    } else {
                        eprintln!("Error: {}", resp.error_message);
                    }
                }
                Err(e) => eprintln!("gRPC error: {e}"),
            }
            return;
        }

        // Interactive mode
        let stdin = std::io::stdin();
        let mut stdout = std::io::stdout();
        loop {
            print!("* ");
            use std::io::Write;
            stdout.flush().ok();

            let mut line = String::new();
            use std::io::BufRead;
            match stdin.lock().read_line(&mut line) {
                Ok(0) => break,
                Ok(_) => {}
                Err(_) => break,
            }

            let cmd = line.trim();
            if cmd.is_empty() { continue; }
            if matches!(cmd, "quit" | "exit") { break; }

            let parts: Vec<&str> = cmd.splitn(2, ' ').collect();
            let request = tonic::Request::new(CommandRequest {
                command_name: parts[0].to_string(),
                arguments: Vec::new(),
                session_id: String::new(),
                response_format: 0,
            });

            match client.execute_command(request).await {
                Ok(response) => {
                    let resp = response.into_inner();
                    if resp.success {
                        print!("{}", resp.text_output);
                    } else {
                        eprintln!("{}", resp.error_message);
                    }
                }
                Err(e) => eprintln!("gRPC error: {e}"),
            }
        }
    });
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn cli_defaults() {
        let cli = Cli::parse_from(["brconsole"]);
        assert!(cli.address.is_none());
        assert!(cli.port.is_none());
        assert!(cli.name.is_none());
        assert!(!cli.grpc);
        assert!(cli.command.is_none());
    }

    #[test]
    fn cli_with_args() {
        let cli = Cli::parse_from([
            "brconsole", "-a", "10.0.0.1", "-p", "9102",
            "-n", "admin", "-e", "status dir",
        ]);
        assert_eq!(cli.address, Some("10.0.0.1".to_string()));
        assert_eq!(cli.port, Some(9102));
        assert_eq!(cli.name, Some("admin".to_string()));
        assert_eq!(cli.command, Some("status dir".to_string()));
    }
}
