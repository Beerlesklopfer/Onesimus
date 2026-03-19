//! Central connection manager for Console, FD, and SD connections.

use std::sync::Arc;
use std::sync::atomic::{AtomicU64, Ordering};

use dashmap::DashMap;

use crate::message_bus::MessageBus;

static NEXT_SESSION_ID: AtomicU64 = AtomicU64::new(1);

pub fn next_session_id() -> u64 {
    NEXT_SESSION_ID.fetch_add(1, Ordering::Relaxed)
}

/// Information about a console session.
#[derive(Debug)]
pub struct SessionInfo {
    pub id: u64,
    pub client_name: String,
    pub authenticated: bool,
}

/// Central manager for all connections.
pub struct SessionManager {
    sessions: DashMap<u64, SessionInfo>,
    bus: Arc<MessageBus>,
}

impl SessionManager {
    pub fn new(bus: Arc<MessageBus>) -> Self {
        Self {
            sessions: DashMap::new(),
            bus,
        }
    }

    pub fn create_session(&self, client_name: String) -> u64 {
        let id = next_session_id();
        self.sessions.insert(id, SessionInfo {
            id,
            client_name,
            authenticated: false,
        });
        id
    }

    pub fn remove_session(&self, id: u64) {
        self.sessions.remove(&id);
    }

    pub fn active_session_count(&self) -> usize {
        self.sessions.len()
    }

    pub fn bus(&self) -> &MessageBus {
        &self.bus
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn create_and_remove_session() {
        let bus = Arc::new(MessageBus::new(16));
        let mgr = SessionManager::new(bus);

        let id1 = mgr.create_session("console1".into());
        let id2 = mgr.create_session("console2".into());
        assert_eq!(mgr.active_session_count(), 2);

        mgr.remove_session(id1);
        assert_eq!(mgr.active_session_count(), 1);

        mgr.remove_session(id2);
        assert_eq!(mgr.active_session_count(), 0);
    }

    #[test]
    fn session_ids_are_unique() {
        let bus = Arc::new(MessageBus::new(16));
        let mgr = SessionManager::new(bus);

        let id1 = mgr.create_session("a".into());
        let id2 = mgr.create_session("b".into());
        let id3 = mgr.create_session("c".into());
        assert_ne!(id1, id2);
        assert_ne!(id2, id3);
    }
}
