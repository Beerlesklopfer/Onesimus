//! ORM error types.

#[derive(Debug, thiserror::Error)]
pub enum OrmError {
    #[error("database error: {0}")]
    Database(#[from] sqlx::Error),

    #[error("serialization error: {0}")]
    Serialization(#[from] serde_json::Error),

    #[error("not found: {entity} with id {id}")]
    NotFound { entity: String, id: i64 },

    #[error("constraint violation: {0}")]
    Constraint(String),

    #[error("{0}")]
    Other(String),
}
