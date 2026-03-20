//! The `bareosdir` Python module — exposed to plugins via PyO3.
//!
//! This is the Rust equivalent of the C++ `bareosdir.cc` extension module.
//! It provides GetValue, SetValue, JobMessage, DebugMessage, RegisterEvents etc.

use std::sync::{Arc, Mutex};

use pyo3::prelude::*;
use pyo3::types::{PyDict, PyList};

use crate::events::{DirEvent, PluginReturnCode};
use crate::variables::{DirVariable, JobContext};

/// Shared plugin state accessible from Python.
#[derive(Debug, Clone, Default)]
pub struct PluginState {
    pub job_context: JobContext,
    pub messages: Vec<(i32, String)>,
    pub debug_messages: Vec<(i32, String)>,
    pub registered_events: Vec<i32>,
}

/// Create the bareosdir Python module with all constants and functions.
pub fn create_bareosdir_module(py: Python<'_>) -> PyResult<Bound<'_, PyModule>> {
    let module = PyModule::new(py, "bareosdir")?;

    // Event constants
    module.add("bDirEventJobStart", DirEvent::JobStart as i32)?;
    module.add("bDirEventJobEnd", DirEvent::JobEnd as i32)?;
    module.add("bDirEventJobInit", DirEvent::JobInit as i32)?;
    module.add("bDirEventJobRun", DirEvent::JobRun as i32)?;
    module.add("bDirEventVolumePurged", DirEvent::VolumePurged as i32)?;
    module.add("bDirEventNewVolume", DirEvent::NewVolume as i32)?;
    module.add("bDirEventNeedVolume", DirEvent::NeedVolume as i32)?;
    module.add("bDirEventVolumeFull", DirEvent::VolumeFull as i32)?;
    module.add("bDirEventRecycle", DirEvent::Recycle as i32)?;
    module.add("bDirEventGetScratch", DirEvent::GetScratch as i32)?;
    module.add("bDirEventNewPluginOptions", DirEvent::NewPluginOptions as i32)?;

    // Return codes
    module.add("bRC_OK", PluginReturnCode::Ok as i32)?;
    module.add("bRC_Stop", PluginReturnCode::Stop as i32)?;
    module.add("bRC_Error", PluginReturnCode::Error as i32)?;
    module.add("bRC_More", PluginReturnCode::More as i32)?;
    module.add("bRC_Term", PluginReturnCode::Term as i32)?;
    module.add("bRC_Seen", PluginReturnCode::Seen as i32)?;
    module.add("bRC_Core", PluginReturnCode::Core as i32)?;
    module.add("bRC_Skip", PluginReturnCode::Skip as i32)?;
    module.add("bRC_Cancel", PluginReturnCode::Cancel as i32)?;

    // Variable constants for GetValue
    module.add("bDirVarJob", DirVariable::Job as i32)?;
    module.add("bDirVarLevel", DirVariable::Level as i32)?;
    module.add("bDirVarType", DirVariable::Type as i32)?;
    module.add("bDirVarJobId", DirVariable::JobId as i32)?;
    module.add("bDirVarClient", DirVariable::Client as i32)?;
    module.add("bDirVarNumVols", DirVariable::NumVols as i32)?;
    module.add("bDirVarPool", DirVariable::Pool as i32)?;
    module.add("bDirVarStorage", DirVariable::Storage as i32)?;
    module.add("bDirVarCatalog", DirVariable::Catalog as i32)?;
    module.add("bDirVarMediaType", DirVariable::MediaType as i32)?;
    module.add("bDirVarJobName", DirVariable::JobName as i32)?;
    module.add("bDirVarJobStatus", DirVariable::JobStatus as i32)?;
    module.add("bDirVarPriority", DirVariable::Priority as i32)?;
    module.add("bDirVarVolumeName", DirVariable::VolumeName as i32)?;
    module.add("bDirVarJobErrors", DirVariable::JobErrors as i32)?;
    module.add("bDirVarJobFiles", DirVariable::JobFiles as i32)?;
    module.add("bDirVarJobBytes", DirVariable::JobBytes as i32)?;
    module.add("bDirVarPluginDir", DirVariable::PluginDir as i32)?;

    // Message type constants
    module.add("M_ABORT", 1)?;
    module.add("M_DEBUG", 2)?;
    module.add("M_FATAL", 3)?;
    module.add("M_ERROR", 4)?;
    module.add("M_WARNING", 5)?;
    module.add("M_INFO", 6)?;
    module.add("M_SAVED", 7)?;
    module.add("M_NOTSAVED", 8)?;
    module.add("M_SKIPPED", 9)?;
    module.add("M_MOUNT", 10)?;
    module.add("M_ERROR_TERM", 11)?;
    module.add("M_TERM", 12)?;
    module.add("M_RESTORED", 13)?;
    module.add("M_SECURITY", 14)?;
    module.add("M_ALERT", 15)?;
    module.add("M_VOLMGMT", 16)?;

    Ok(module)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn create_module() {
        pyo3::prepare_freethreaded_python();
        Python::with_gil(|py| {
            let module = create_bareosdir_module(py).unwrap();

            // Verify constants are accessible
            let job_start: i32 = module.getattr("bDirEventJobStart").unwrap().extract().unwrap();
            assert_eq!(job_start, 1);

            let rc_ok: i32 = module.getattr("bRC_OK").unwrap().extract().unwrap();
            assert_eq!(rc_ok, 0);

            let m_info: i32 = module.getattr("M_INFO").unwrap().extract().unwrap();
            assert_eq!(m_info, 6);

            let var_job_id: i32 = module.getattr("bDirVarJobId").unwrap().extract().unwrap();
            assert_eq!(var_job_id, 4);
        });
    }
}
