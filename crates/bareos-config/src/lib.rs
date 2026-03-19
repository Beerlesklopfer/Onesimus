//! Bareos configuration file parser.
//!
//! Parses the Bareos Director configuration syntax into typed resource structs.
//! Supports all 14 resource types: Director, Client, Job, JobDefs, Storage,
//! Catalog, Schedule, FileSet, Pool, Messages, Counter, Profile, Console, User.
//!
//! Implementation will be filled in Phase R2.

pub mod resources;

pub use resources::*;
