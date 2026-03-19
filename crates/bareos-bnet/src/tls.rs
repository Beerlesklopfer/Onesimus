//! TLS support for BNET connections.
//!
//! Uses rustls for TLS, loading PEM certificates from Bareos config paths.
//! Implementation will be filled in Phase R1b.

/// TLS configuration loaded from Bareos resource directives.
#[derive(Debug, Clone)]
pub struct TlsConfig {
    pub ca_certfile: Option<String>,
    pub certfile: Option<String>,
    pub keyfile: Option<String>,
    pub verify_peer: bool,
}

impl Default for TlsConfig {
    fn default() -> Self {
        Self {
            ca_certfile: None,
            certfile: None,
            keyfile: None,
            verify_peer: true,
        }
    }
}
