//! gRPC authentication interceptor.
//!
//! Validates the x-bareos-session-token header on every RPC call.
//! Token is HMAC-SHA256 derived from password + server challenge.

use tonic::{Request, Status};

/// Validate the session token from gRPC metadata.
pub fn validate_token(req: &Request<()>, expected_token: &str) -> Result<(), Status> {
    let token = req
        .metadata()
        .get("x-bareos-session-token")
        .ok_or_else(|| Status::unauthenticated("missing x-bareos-session-token header"))?
        .to_str()
        .map_err(|_| Status::unauthenticated("invalid token encoding"))?;

    if token == expected_token {
        Ok(())
    } else {
        Err(Status::unauthenticated("invalid session token"))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tonic::metadata::MetadataValue;

    #[test]
    fn valid_token() {
        let mut req = Request::new(());
        req.metadata_mut().insert(
            "x-bareos-session-token",
            MetadataValue::from_static("abc123"),
        );
        assert!(validate_token(&req, "abc123").is_ok());
    }

    #[test]
    fn missing_token() {
        let req = Request::new(());
        assert!(validate_token(&req, "abc123").is_err());
    }

    #[test]
    fn wrong_token() {
        let mut req = Request::new(());
        req.metadata_mut().insert(
            "x-bareos-session-token",
            MetadataValue::from_static("wrong"),
        );
        assert!(validate_token(&req, "abc123").is_err());
    }
}
