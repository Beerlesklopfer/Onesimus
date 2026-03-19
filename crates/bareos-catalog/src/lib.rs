//! PostgreSQL catalog interface for the Bareos Director.
//!
//! Provides async database access for job records, media/volume management,
//! file catalog, and raw SQL queries.
//!
//! Implementation will be filled in Phase R5.

/// Unique job identifier.
pub type JobId = i32;
