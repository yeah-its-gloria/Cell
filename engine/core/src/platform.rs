pub enum PlatformId {
    Windows,
    Linux,
}

#[cfg(windows)]
pub const PLATFORM_ID: PlatformId = PlatformId::Windows;

#[cfg(unix)]
pub const PLATFORM_ID: PlatformId = PlatformId::Linux;

pub struct Platform {
    pub a: u32,
}
