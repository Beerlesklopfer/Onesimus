//! baReos bconsole — Interactive console client for the Bareos Director.
//!
//! Connects via BNET (TCP + TLS + CRAM-MD5) or gRPC.

use std::io::{self, BufRead, Write};
use std::net::TcpStream;
use std::time::Duration;

use clap::Parser;
use tracing::{info, error, debug};
use tracing_subscriber::EnvFilter;

use bareos_bnet::{BnetStream, BnetSignal, BnetMessage, BnetError};
use bareos_bnet::hello::make_console_hello;
use bareos_bnet::cram_md5::CramMd5Auth;

/// baReos bconsole — Bareos Console Client
#[derive(Parser, Debug)]
#[command(name = "bconsole", version, about)]
struct Cli {
    /// Director address
    #[arg(short = 'a', long, default_value = "localhost")]
    address: String,

    /// Director port
    #[arg(short, long, default_value_t = 9101)]
    port: u16,

    /// Console name
    #[arg(short, long, default_value = "*UserAgent*")]
    name: String,

    /// Password
    #[arg(short = 'w', long, default_value = "")]
    password: String,

    /// Use gRPC protocol
    #[arg(long)]
    grpc: bool,

    /// Execute single command and exit
    #[arg(short = 'e', long)]
    command: Option<String>,
}

fn main() {
    let cli = Cli::parse();

    tracing_subscriber::fmt()
        .with_env_filter(
            EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| EnvFilter::new("warn")),
        )
        .init();

    let addr = format!("{}:{}", cli.address, cli.port);

    if cli.grpc {
        run_grpc_mode(&cli, &addr);
    } else {
        run_bnet_mode(&cli, &addr);
    }
}

fn run_bnet_mode(cli: &Cli, addr: &str) {
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

    let mut bnet = BnetStream::new(stream);

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
    interactive_loop(&mut bnet);
}

fn execute_single_command(bnet: &mut BnetStream<TcpStream>, cmd: &str) {
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

fn interactive_loop(bnet: &mut BnetStream<TcpStream>) {
    let stdin = io::stdin();
    let mut stdout = io::stdout();

    loop {
        // Drain any pending signals/prompts
        drain_signals(bnet);

        // Show prompt
        print!("* ");
        stdout.flush().ok();

        // Read user input
        let mut line = String::new();
        match stdin.lock().read_line(&mut line) {
            Ok(0) => break, // EOF
            Ok(_) => {}
            Err(_) => break,
        }

        let cmd = line.trim();
        if cmd.is_empty() {
            continue;
        }

        // Send command
        if bnet.send_str(cmd).is_err() {
            eprintln!("Connection lost");
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

        // Read response
        loop {
            match bnet.recv() {
                Ok(BnetMessage::Data(data)) => {
                    let text = String::from_utf8_lossy(&data);
                    print!("{text}");
                }
                Ok(BnetMessage::Signal(BnetSignal::Eod)) => break,
                Ok(BnetMessage::Signal(BnetSignal::MainPrompt)) => break,
                Ok(BnetMessage::Signal(BnetSignal::Terminate)) => {
                    eprintln!("Director terminated connection");
                    return;
                }
                Ok(BnetMessage::Signal(BnetSignal::YesNo)) => {
                    // Interactive yes/no prompt
                    print!("(yes/no): ");
                    stdout.flush().ok();
                    let mut answer = String::new();
                    stdin.lock().read_line(&mut answer).ok();
                    let _ = bnet.send_str(answer.trim());
                }
                Ok(BnetMessage::Signal(BnetSignal::SubPrompt)) => {
                    print!("> ");
                    stdout.flush().ok();
                    let mut input = String::new();
                    stdin.lock().read_line(&mut input).ok();
                    let _ = bnet.send_str(input.trim());
                }
                Ok(BnetMessage::Signal(BnetSignal::TextInput)) => {
                    let mut input = String::new();
                    stdin.lock().read_line(&mut input).ok();
                    let _ = bnet.send_str(input.trim());
                }
                Ok(BnetMessage::Signal(sig)) => {
                    debug!("signal: {sig:?}");
                }
                Err(BnetError::HardEof) | Err(BnetError::Terminated) => {
                    eprintln!("Connection closed");
                    return;
                }
                Err(e) => {
                    eprintln!("Error: {e}");
                    return;
                }
            }
        }
    }
}

/// Drain pending signals (prompts, etc.) without blocking.
fn drain_signals(bnet: &mut BnetStream<TcpStream>) {
    // Set short timeout to drain
    bnet.inner().set_read_timeout(Some(Duration::from_millis(500))).ok();

    loop {
        match bnet.recv() {
            Ok(BnetMessage::Signal(BnetSignal::MainPrompt)) => break,
            Ok(BnetMessage::Signal(_)) => continue,
            Ok(BnetMessage::Data(_)) => continue,
            Err(_) => break,
        }
    }

    // Restore normal timeout
    bnet.inner().set_read_timeout(Some(Duration::from_secs(30))).ok();
}

fn run_grpc_mode(cli: &Cli, addr: &str) {
    eprintln!("gRPC mode connecting to {addr}");

    let rt = tokio::runtime::Runtime::new().unwrap();
    rt.block_on(async {
        // TODO: tonic gRPC client
        eprintln!("gRPC client not yet fully implemented");
        eprintln!("Use --no-grpc for BNET mode");
    });
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn cli_defaults() {
        let cli = Cli::parse_from(["bconsole"]);
        assert_eq!(cli.address, "localhost");
        assert_eq!(cli.port, 9101);
        assert_eq!(cli.name, "*UserAgent*");
        assert!(!cli.grpc);
        assert!(cli.command.is_none());
    }

    #[test]
    fn cli_with_args() {
        let cli = Cli::parse_from([
            "bconsole", "-a", "10.0.0.1", "-p", "9102",
            "-n", "admin", "-e", "status dir",
        ]);
        assert_eq!(cli.address, "10.0.0.1");
        assert_eq!(cli.port, 9102);
        assert_eq!(cli.name, "admin");
        assert_eq!(cli.command, Some("status dir".to_string()));
    }
}
