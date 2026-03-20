//! Director plugin events — maps 1:1 to C++ bDirEventType.

/// Director plugin events.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
#[repr(i32)]
pub enum DirEvent {
    JobStart = 1,
    JobEnd = 2,
    JobInit = 3,
    JobRun = 4,
    VolumePurged = 5,
    NewVolume = 6,
    NeedVolume = 7,
    VolumeFull = 8,
    Recycle = 9,
    GetScratch = 10,
    NewPluginOptions = 11,
}

impl DirEvent {
    pub fn from_i32(val: i32) -> Option<Self> {
        match val {
            1 => Some(Self::JobStart),
            2 => Some(Self::JobEnd),
            3 => Some(Self::JobInit),
            4 => Some(Self::JobRun),
            5 => Some(Self::VolumePurged),
            6 => Some(Self::NewVolume),
            7 => Some(Self::NeedVolume),
            8 => Some(Self::VolumeFull),
            9 => Some(Self::Recycle),
            10 => Some(Self::GetScratch),
            11 => Some(Self::NewPluginOptions),
            _ => None,
        }
    }

    pub fn name(&self) -> &'static str {
        match self {
            Self::JobStart => "bDirEventJobStart",
            Self::JobEnd => "bDirEventJobEnd",
            Self::JobInit => "bDirEventJobInit",
            Self::JobRun => "bDirEventJobRun",
            Self::VolumePurged => "bDirEventVolumePurged",
            Self::NewVolume => "bDirEventNewVolume",
            Self::NeedVolume => "bDirEventNeedVolume",
            Self::VolumeFull => "bDirEventVolumeFull",
            Self::Recycle => "bDirEventRecycle",
            Self::GetScratch => "bDirEventGetScratch",
            Self::NewPluginOptions => "bDirEventNewPluginOptions",
        }
    }
}

/// Plugin return codes — maps to C++ bRC.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(i32)]
pub enum PluginReturnCode {
    Ok = 0,
    Stop = 1,
    Error = 2,
    More = 3,
    Term = 4,
    Seen = 5,
    Core = 6,
    Skip = 7,
    Cancel = 8,
}

impl PluginReturnCode {
    pub fn from_i32(val: i32) -> Self {
        match val {
            0 => Self::Ok,
            1 => Self::Stop,
            2 => Self::Error,
            3 => Self::More,
            4 => Self::Term,
            5 => Self::Seen,
            6 => Self::Core,
            7 => Self::Skip,
            8 => Self::Cancel,
            _ => Self::Error,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn event_roundtrip() {
        for val in 1..=11 {
            let event = DirEvent::from_i32(val).unwrap();
            assert_eq!(event as i32, val);
        }
    }

    #[test]
    fn event_names() {
        assert_eq!(DirEvent::JobStart.name(), "bDirEventJobStart");
        assert_eq!(DirEvent::JobEnd.name(), "bDirEventJobEnd");
        assert_eq!(DirEvent::NewPluginOptions.name(), "bDirEventNewPluginOptions");
    }

    #[test]
    fn return_code_roundtrip() {
        assert_eq!(PluginReturnCode::from_i32(0), PluginReturnCode::Ok);
        assert_eq!(PluginReturnCode::from_i32(2), PluginReturnCode::Error);
        assert_eq!(PluginReturnCode::from_i32(99), PluginReturnCode::Error);
    }
}
