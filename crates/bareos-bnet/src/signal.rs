//! BNET signal constants.
//!
//! Negative message lengths in the BNET wire protocol encode out-of-band
//! signals.  These map 1:1 to the C++ constants in `lib/bnet.h`.

/// Out-of-band signals transmitted as negative length values.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(i32)]
pub enum BnetSignal {
    /// End of data stream
    Eod = -1,
    /// End of data, poll for more
    EodPoll = -2,
    /// Request status
    StatusMessage = -3,
    /// Terminate connection
    Terminate = -4,
    /// Poll for data
    Poll = -5,
    /// Heartbeat
    Heartbeat = -6,
    /// Heartbeat response
    HeartbeatResponse = -7,
    /// Reserved: backoff
    Backoff = -8,
    /// Reserved: backoff clear
    BackoffClear = -9,
    /// Set buffer size
    SetBufSize = -10,
    /// Set debug level
    SetDebug = -11,
    /// Sub-prompt begin
    SubPrompt = -12,
    /// Text input prompt
    TextInput = -13,
}

impl BnetSignal {
    /// Try to convert an i32 to a BnetSignal.
    pub fn from_i32(val: i32) -> Option<Self> {
        match val {
            -1 => Some(Self::Eod),
            -2 => Some(Self::EodPoll),
            -3 => Some(Self::StatusMessage),
            -4 => Some(Self::Terminate),
            -5 => Some(Self::Poll),
            -6 => Some(Self::Heartbeat),
            -7 => Some(Self::HeartbeatResponse),
            -8 => Some(Self::Backoff),
            -9 => Some(Self::BackoffClear),
            -10 => Some(Self::SetBufSize),
            -11 => Some(Self::SetDebug),
            -12 => Some(Self::SubPrompt),
            -13 => Some(Self::TextInput),
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn signal_roundtrip() {
        assert_eq!(BnetSignal::from_i32(-1), Some(BnetSignal::Eod));
        assert_eq!(BnetSignal::from_i32(-4), Some(BnetSignal::Terminate));
        assert_eq!(BnetSignal::from_i32(0), None);
        assert_eq!(BnetSignal::from_i32(-99), None);
    }

    #[test]
    fn signal_repr_matches_c() {
        assert_eq!(BnetSignal::Eod as i32, -1);
        assert_eq!(BnetSignal::Terminate as i32, -4);
        assert_eq!(BnetSignal::Heartbeat as i32, -6);
    }
}
