//! Python plugin system for the baReos Director via PyO3.
//!
//! Embeds a Python interpreter and exposes the `bareosdir` module
//! with the same API as the C++ Bareos Director plugin interface.
//! Existing Bareos Python plugins (BareosDirPluginBaseclass) work unchanged.

pub mod events;
pub mod variables;
pub mod loader;
pub mod bareosdir;

pub use events::DirEvent;
pub use loader::PluginManager;
