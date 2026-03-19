//! Bareos Hello protocol — connection type identification.
//!
//! The first message on a BNET connection identifies the peer:
//! - "Hello Client <name> FdProtocolVersion=<N> calling" — File Daemon
//! - "Hello Client <name> calling" — File Daemon (legacy)
//! - "Hello <name> calling version <ver>" — Named Console
//! - "Hello <name> calling version <ver> proto=grpc" — Named Console (gRPC)
//! - "Hello *UserAgent* calling" — Default Console
//! - "Hello Director <name> calling" — Director (outbound to FD/SD)

use crate::cram_md5::bash_spaces;

/// Parsed hello message from a connecting peer.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Hello {
    /// File Daemon connecting to Director
    FileDaemon {
        client_name: String,
        fd_protocol_version: i32,
    },
    /// Default console (no name, uses Director password)
    DefaultConsole,
    /// Named console
    NamedConsole {
        name: String,
        version: String,
        wants_grpc: bool,
    },
}

/// Parse an incoming hello message.
pub fn parse_hello(msg: &str) -> Option<Hello> {
    let msg = msg.trim();

    // File Daemon with protocol version
    // "Hello Client <name> FdProtocolVersion=<N> calling"
    if msg.starts_with("Hello Client ") {
        return parse_fd_hello(msg);
    }

    // Console: "Hello <name> calling [version <ver>] [proto=grpc]"
    if msg.starts_with("Hello ") {
        return parse_console_hello(msg);
    }

    None
}

fn parse_fd_hello(msg: &str) -> Option<Hello> {
    // Strip "Hello Client "
    let rest = msg.strip_prefix("Hello Client ")?;

    // Try: "<name> FdProtocolVersion=<N> calling"
    if let Some(fd_pos) = rest.find(" FdProtocolVersion=") {
        let name = &rest[..fd_pos];
        let after_fd = &rest[fd_pos + " FdProtocolVersion=".len()..];
        if let Some(space) = after_fd.find(' ') {
            let version_str = &after_fd[..space];
            let fd_protocol_version = version_str.parse().ok()?;
            return Some(Hello::FileDaemon {
                client_name: unbash_name(name),
                fd_protocol_version,
            });
        }
    }

    // Fallback: "<name> calling"
    if let Some(pos) = rest.find(" calling") {
        let name = &rest[..pos];
        return Some(Hello::FileDaemon {
            client_name: unbash_name(name),
            fd_protocol_version: 0,
        });
    }

    None
}

fn parse_console_hello(msg: &str) -> Option<Hello> {
    let rest = msg.strip_prefix("Hello ")?;

    // Default console: "*UserAgent* calling"
    if rest.starts_with("*UserAgent*") {
        return Some(Hello::DefaultConsole);
    }

    // Named console: "<name> calling version <ver> [proto=grpc]"
    if let Some(calling_pos) = rest.find(" calling") {
        let name = &rest[..calling_pos];

        // Check for "version <ver>"
        let after_calling = &rest[calling_pos + " calling".len()..];
        let (version, wants_grpc) = if let Some(ver_rest) = after_calling.strip_prefix(" version ") {
            let wants_grpc = ver_rest.contains("proto=grpc");
            // Version string: everything up to "proto=" or end
            let version = if let Some(proto_pos) = ver_rest.find(" proto=") {
                ver_rest[..proto_pos].trim().to_string()
            } else {
                ver_rest.trim().to_string()
            };
            (version, wants_grpc)
        } else {
            (String::new(), false)
        };

        return Some(Hello::NamedConsole {
            name: unbash_name(name),
            version,
            wants_grpc,
        });
    }

    None
}

/// Build a Director hello message for outbound connections to FD/SD.
pub fn make_director_hello(director_name: &str) -> String {
    format!("Hello Director {} calling\n", bash_spaces(director_name))
}

/// Build a Console hello message.
pub fn make_console_hello(name: &str, version: &str, want_grpc: bool) -> String {
    if name == "*UserAgent*" || name.is_empty() {
        return "Hello *UserAgent* calling\n".to_string();
    }
    let bashed = bash_spaces(name);
    if want_grpc {
        format!("Hello {bashed} calling version {version} proto=grpc\n")
    } else {
        format!("Hello {bashed} calling version {version}\n")
    }
}

/// Unbash a name: replace \x01 with space (Bareos control char convention).
/// Note: underscores in names are NOT replaced — they may be literal.
/// Only \x01 (ASCII SOH) is the encoded space character.
fn unbash_name(name: &str) -> String {
    name.replace('\x01', " ").trim().to_string()
}

/// Well-known response messages.
pub const FD_OK_HELLO: &str = "2000 OK Hello";
pub const SD_OK_HELLO: &str = "3000 OK Hello";

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_fd_hello_with_version() {
        let msg = "Hello Client bareos-fd FdProtocolVersion=54 calling";
        assert_eq!(
            parse_hello(msg),
            Some(Hello::FileDaemon {
                client_name: "bareos-fd".into(),
                fd_protocol_version: 54,
            })
        );
    }

    #[test]
    fn parse_fd_hello_without_version() {
        let msg = "Hello Client old-client-fd calling";
        assert_eq!(
            parse_hello(msg),
            Some(Hello::FileDaemon {
                client_name: "old-client-fd".into(),
                fd_protocol_version: 0,
            })
        );
    }

    #[test]
    fn parse_default_console() {
        let msg = "Hello *UserAgent* calling";
        assert_eq!(parse_hello(msg), Some(Hello::DefaultConsole));
    }

    #[test]
    fn parse_named_console() {
        let msg = "Hello admin-console calling version 25.0.0";
        assert_eq!(
            parse_hello(msg),
            Some(Hello::NamedConsole {
                name: "admin-console".into(),
                version: "25.0.0".into(),
                wants_grpc: false,
            })
        );
    }

    #[test]
    fn parse_console_with_grpc() {
        let msg = "Hello admin calling version 25.0.0 proto=grpc";
        assert_eq!(
            parse_hello(msg),
            Some(Hello::NamedConsole {
                name: "admin".into(),
                version: "25.0.0".into(),
                wants_grpc: true,
            })
        );
    }

    #[test]
    fn make_director_hello_bashes_spaces() {
        assert_eq!(
            make_director_hello("my director"),
            "Hello Director my_director calling\n"
        );
    }

    #[test]
    fn make_console_hello_default() {
        assert_eq!(
            make_console_hello("*UserAgent*", "", false),
            "Hello *UserAgent* calling\n"
        );
    }

    #[test]
    fn make_console_hello_grpc() {
        assert_eq!(
            make_console_hello("admin", "25.0.0", true),
            "Hello admin calling version 25.0.0 proto=grpc\n"
        );
    }

    #[test]
    fn invalid_hello() {
        assert_eq!(parse_hello("not a hello"), None);
        assert_eq!(parse_hello(""), None);
    }
}
