//! Typed message bus for inter-component communication.
//!
//! Uses crossbeam broadcast channels. Each console session subscribes
//! to receive events (FD/SD status, console text, job events).

use crossbeam_channel::{Sender, Receiver, bounded};

/// All message types on the bus.
/// Maps 1:1 to the C++ BusMessage variant in message_bus.h.
#[derive(Debug, Clone)]
pub enum BusMessage {
    ConsoleText { text: String },
    FdConnected { client_name: String, protocol_version: i32, connect_time: i64 },
    FdDisconnected { client_name: String },
    FdStatus { client_name: String, alive: bool, check_time: i64 },
    SdConnected { job_id: u32, storage_name: String, connect_time: i64 },
    SdDisconnected { job_id: u32, storage_name: String },
    SdStatus { job_id: u32, storage_name: String, alive: bool, check_time: i64 },
    JobStatus { job_id: u32, job_name: String, status: String, timestamp: i64 },
}

/// Broadcast message bus. Subscribers get their own Receiver.
pub struct MessageBus {
    sender: Sender<BusMessage>,
    /// We keep one receiver to clone for new subscribers.
    /// crossbeam bounded channels are MPMC, so all receivers see all messages.
    _receiver: Receiver<BusMessage>,
    _capacity: usize,
}

impl MessageBus {
    pub fn new(capacity: usize) -> Self {
        let (sender, receiver) = bounded(capacity);
        Self {
            sender,
            _receiver: receiver,
            _capacity: capacity,
        }
    }

    /// Post a message to the bus (non-blocking, drops on full).
    pub fn post(&self, msg: BusMessage) {
        let _ = self.sender.try_send(msg);
    }

    /// Convenience: post a console text message.
    pub fn post_text(&self, text: String) {
        self.post(BusMessage::ConsoleText { text });
    }

    /// Create a new subscriber that receives future messages.
    /// crossbeam bounded channels are MPMC — cloning the receiver works.
    pub fn subscribe(&self) -> Receiver<BusMessage> {
        self._receiver.clone()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn post_and_receive() {
        let bus = MessageBus::new(16);
        let rx = bus.subscribe();

        bus.post(BusMessage::ConsoleText { text: "hello".into() });

        let msg = rx.try_recv().unwrap();
        match msg {
            BusMessage::ConsoleText { text } => assert_eq!(text, "hello"),
            _ => panic!("wrong message type"),
        }
    }

    #[test]
    fn multiple_message_types() {
        let bus = MessageBus::new(16);
        let rx = bus.subscribe();

        bus.post(BusMessage::FdConnected {
            client_name: "client1-fd".into(),
            protocol_version: 54,
            connect_time: 1234567890,
        });
        bus.post(BusMessage::SdConnected {
            job_id: 42,
            storage_name: "File1".into(),
            connect_time: 1234567891,
        });
        bus.post(BusMessage::ConsoleText { text: "done".into() });

        let mut count = 0;
        while rx.try_recv().is_ok() {
            count += 1;
        }
        assert_eq!(count, 3);
    }

    #[test]
    fn post_text_convenience() {
        let bus = MessageBus::new(16);
        let rx = bus.subscribe();

        bus.post_text("test message".into());

        match rx.try_recv().unwrap() {
            BusMessage::ConsoleText { text } => assert_eq!(text, "test message"),
            _ => panic!("wrong type"),
        }
    }
}
