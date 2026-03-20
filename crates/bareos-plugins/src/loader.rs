//! Plugin loader — discovers and loads Python plugins via PyO3.

use std::path::{Path, PathBuf};
use std::collections::HashMap;

use pyo3::prelude::*;
use pyo3::types::{PyDict, PyModule};
use tracing::{info, error, debug, warn};

use crate::bareosdir;
use crate::events::{DirEvent, PluginReturnCode};
use crate::variables::JobContext;

/// A loaded Python plugin instance.
pub struct PythonPlugin {
    /// Module name (e.g., "bareos-dir-class-plugin")
    pub module_name: String,
    /// Plugin instance number
    pub instance: u32,
    /// The Python plugin object
    plugin_object: PyObject,
    /// Registered events
    registered_events: Vec<DirEvent>,
}

/// Manages all loaded plugins.
pub struct PluginManager {
    plugins: Vec<PythonPlugin>,
    plugin_dir: PathBuf,
}

impl PluginManager {
    pub fn new(plugin_dir: &Path) -> Self {
        Self {
            plugins: Vec::new(),
            plugin_dir: plugin_dir.to_path_buf(),
        }
    }

    /// Load a plugin from a module definition string.
    /// Format: "python:module_name=<name>:module_path=<path>:instance=<N>:key=value"
    pub fn load_plugin(&mut self, plugindef: &str) -> Result<(), String> {
        let params = parse_plugin_definition(plugindef);

        let module_name = params.get("module_name")
            .ok_or("missing module_name in plugin definition")?
            .clone();

        let module_path = params.get("module_path")
            .map(|s| PathBuf::from(s))
            .unwrap_or_else(|| self.plugin_dir.clone());

        let instance: u32 = params.get("instance")
            .and_then(|s| s.parse().ok())
            .unwrap_or(0);

        info!("loading plugin '{module_name}' instance={instance} from {}", module_path.display());

        Python::with_gil(|py| {
            // Add module path and plugin dir to sys.path
            let sys = py.import("sys").map_err(|e| format!("import sys: {e}"))?;
            let path = sys.getattr("path").map_err(|e| format!("sys.path: {e}"))?;

            let module_path_str = module_path.to_string_lossy().to_string();
            path.call_method1("insert", (0, &module_path_str))
                .map_err(|e| format!("sys.path.insert: {e}"))?;

            let plugin_dir_str = self.plugin_dir.to_string_lossy().to_string();
            path.call_method1("insert", (0, &plugin_dir_str))
                .map_err(|e| format!("sys.path.insert: {e}"))?;

            // Register bareosdir module
            let bareosdir_module = bareosdir::create_bareosdir_module(py)
                .map_err(|e| format!("create bareosdir: {e}"))?;
            let sys_modules: Bound<'_, PyDict> = sys.getattr("modules")
                .map_err(|e| format!("sys.modules: {e}"))?
                .downcast_into()
                .map_err(|e| format!("downcast: {e}"))?;
            sys_modules.set_item("bareosdir", &bareosdir_module)
                .map_err(|e| format!("register bareosdir: {e}"))?;

            // Import the plugin module
            let module = py.import(&*module_name)
                .map_err(|e| format!("import {module_name}: {e}"))?;

            // Call load_bareos_plugin(plugindef)
            let load_fn = module.getattr("load_bareos_plugin")
                .map_err(|e| format!("{module_name}.load_bareos_plugin: {e}"))?;

            let rc = load_fn.call1((plugindef,))
                .map_err(|e| format!("load_bareos_plugin({plugindef}): {e}"))?;

            let rc_val: i32 = rc.extract().unwrap_or(0);
            if PluginReturnCode::from_i32(rc_val) != PluginReturnCode::Ok {
                return Err(format!("plugin returned error code {rc_val}"));
            }

            // Get the plugin object from BareosDirWrapper
            let wrapper = py.import("BareosDirWrapper")
                .map_err(|e| format!("import BareosDirWrapper: {e}"))?;
            let plugin_obj = wrapper.getattr("bareos_dir_plugin_object")
                .map_err(|e| format!("get plugin object: {e}"))?;

            self.plugins.push(PythonPlugin {
                module_name,
                instance,
                plugin_object: plugin_obj.into_pyobject(py)
                    .map_err(|e| format!("into_pyobject: {e}"))?
                    .unbind(),
                registered_events: vec![DirEvent::NewPluginOptions],
            });

            info!("plugin loaded successfully");
            Ok(())
        })
    }

    /// Dispatch an event to all plugins that registered for it.
    pub fn dispatch_event(&self, event: DirEvent, job_ctx: &JobContext) -> PluginReturnCode {
        let mut result = PluginReturnCode::Ok;

        for plugin in &self.plugins {
            if !plugin.registered_events.contains(&event) {
                continue;
            }

            let rc = Python::with_gil(|py| {
                match plugin.plugin_object.call_method1(
                    py,
                    "handle_plugin_event",
                    (event as i32,),
                ) {
                    Ok(rc) => {
                        let val: i32 = rc.extract(py).unwrap_or(0);
                        PluginReturnCode::from_i32(val)
                    }
                    Err(e) => {
                        error!("plugin '{}' event {} error: {e}",
                               plugin.module_name, event.name());
                        PluginReturnCode::Error
                    }
                }
            });

            if rc != PluginReturnCode::Ok {
                result = rc;
            }
        }

        result
    }

    /// Number of loaded plugins.
    pub fn plugin_count(&self) -> usize {
        self.plugins.len()
    }
}

/// Parse plugin definition string into key-value pairs.
/// Format: "python:module_name=val:module_path=val:instance=0:key=value"
fn parse_plugin_definition(plugindef: &str) -> HashMap<String, String> {
    let mut params = HashMap::new();

    for part in plugindef.split(':') {
        if let Some(eq_pos) = part.find('=') {
            let key = part[..eq_pos].trim().to_string();
            let value = part[eq_pos + 1..].trim().to_string();
            params.insert(key, value);
        }
    }

    params
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_plugindef() {
        let params = parse_plugin_definition(
            "python:module_name=myplugin:module_path=/usr/lib/bareos/plugins:instance=0:custom=val"
        );
        assert_eq!(params.get("module_name").unwrap(), "myplugin");
        assert_eq!(params.get("module_path").unwrap(), "/usr/lib/bareos/plugins");
        assert_eq!(params.get("instance").unwrap(), "0");
        assert_eq!(params.get("custom").unwrap(), "val");
    }

    #[test]
    fn parse_plugindef_minimal() {
        let params = parse_plugin_definition("python:module_name=test");
        assert_eq!(params.get("module_name").unwrap(), "test");
        assert_eq!(params.len(), 2); // "python" has no =, so only module_name
    }

    #[test]
    fn plugin_manager_empty() {
        let mgr = PluginManager::new(Path::new("/tmp"));
        assert_eq!(mgr.plugin_count(), 0);
    }

    #[test]
    fn dispatch_event_no_plugins() {
        let mgr = PluginManager::new(Path::new("/tmp"));
        let ctx = JobContext::default();
        let rc = mgr.dispatch_event(DirEvent::JobStart, &ctx);
        assert_eq!(rc, PluginReturnCode::Ok);
    }
}
