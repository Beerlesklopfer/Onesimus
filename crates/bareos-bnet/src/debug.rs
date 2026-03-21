//! Debug-Level System — 1:1 kompatibel mit C++ Bareos d_msg/Dmsg.
//!
//! C++ Original (baconfig.h):
//!   #define Dmsg0(lvl, ...)
//!     if ((lvl) <= debug_level) d_msg(__FILE__, __LINE__, lvl, __VA_ARGS__)
//!
//! d_msg Ausgabe-Format:
//!   [timestamp] daemon_name (level): file:line-jobid message
//!
//! Level-Konvention (aus C++ Quellen):
//!   1-49    Basis (Startup, Shutdown, Config)
//!   50-99   Ausfuehrlich (Commands, Session-Lifecycle)
//!   100     Protokoll (TLS, Auth, DoTlsHandshake)
//!   110     Daemon-Messages (>stored:, <stored:)
//!   116     Wire (>filed:, <filed: — gesendete/empfangene Strings)
//!   150     Detail (einzelne Schritte in Auth, Config-Parse)
//!   200     Trace (Memory, Locks, jeden Funktionsaufruf)

use std::sync::atomic::{AtomicBool, AtomicU32, Ordering};
use std::time::SystemTime;

/// Globaler Debug-Level (gesetzt via -d CLI-Flag)
static DEBUG_LEVEL: AtomicU32 = AtomicU32::new(0);

/// Debug-Timestamps aktiviert (--dt Flag)
static DEBUG_TIMESTAMPS: AtomicBool = AtomicBool::new(false);

/// Daemon-Name fuer Debug-Ausgabe
static DAEMON_NAME: std::sync::OnceLock<String> = std::sync::OnceLock::new();

/// Setze den globalen Debug-Level.
pub fn set_debug_level(level: u32) {
    DEBUG_LEVEL.store(level, Ordering::Relaxed);
}

/// Lese den aktuellen Debug-Level.
pub fn debug_level() -> u32 {
    DEBUG_LEVEL.load(Ordering::Relaxed)
}

/// Aktiviere/deaktiviere Timestamps in Debug-Ausgabe.
pub fn set_debug_timestamps(enabled: bool) {
    DEBUG_TIMESTAMPS.store(enabled, Ordering::Relaxed);
}

/// Setze den Daemon-Namen (z.B. "bareos-dir", "bareos-fd").
pub fn set_daemon_name(name: &str) {
    let _ = DAEMON_NAME.set(name.to_string());
}

/// Prüfe ob ein bestimmter Level aktiv ist.
#[inline]
pub fn is_debug(level: u32) -> bool {
    level <= DEBUG_LEVEL.load(Ordering::Relaxed)
}

/// Debug-Nachricht ausgeben — Äquivalent zu C++ d_msg().
///
/// Format: [timestamp] daemon (level): file:line message
pub fn d_msg(file: &str, line: u32, level: u32, msg: &str) {
    if !is_debug(level) {
        return;
    }

    let daemon = DAEMON_NAME.get().map(|s| s.as_str()).unwrap_or("bareos");

    if DEBUG_TIMESTAMPS.load(Ordering::Relaxed) {
        let now = SystemTime::now()
            .duration_since(SystemTime::UNIX_EPOCH)
            .unwrap_or_default();
        let secs = now.as_secs();
        let usecs = now.subsec_micros();
        eprint!("{}.{:06} ", secs, usecs);
    }

    // Basename aus file extrahieren
    let basename = file.rsplit('/').next().unwrap_or(file);

    eprintln!("{daemon} ({level}): {basename}:{line} {msg}");
}

/// Bareos-kompatibles Dmsg Macro.
///
/// Verwendung: `dmsg!(100, "TLS handshake OK for {}", client_name);`
///
/// Gibt aus: `bareos-dir (100): accept.rs:42 TLS handshake OK for bareos-fd`
#[macro_export]
macro_rules! dmsg {
    ($level:expr, $($arg:tt)*) => {
        if $crate::debug::is_debug($level) {
            $crate::debug::d_msg(file!(), line!(), $level, &format!($($arg)*));
        }
    };
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn debug_level_default() {
        set_debug_level(0);
        assert_eq!(debug_level(), 0);
        assert!(!is_debug(1));
        assert!(is_debug(0)); // Level 0 = immer wenn debug_level >= 0
    }

    #[test]
    fn debug_level_set() {
        set_debug_level(100);
        assert!(is_debug(1));
        assert!(is_debug(50));
        assert!(is_debug(100));
        assert!(!is_debug(101));
        set_debug_level(0);
    }

    #[test]
    fn d_msg_output() {
        set_debug_level(100);
        set_daemon_name("test-dir");
        // Sollte nicht paniken
        d_msg("src/test.rs", 42, 100, "test message");
        d_msg("src/test.rs", 42, 200, "should not print"); // level too high
        set_debug_level(0);
    }

    #[test]
    fn timestamps() {
        set_debug_level(1);
        set_debug_timestamps(true);
        d_msg("test.rs", 1, 1, "with timestamp");
        set_debug_timestamps(false);
        d_msg("test.rs", 1, 1, "without timestamp");
        set_debug_level(0);
    }
}
