#[cfg_attr(target_os = "windows", path = "windows/lib.rs")]
#[cfg_attr(target_os = "linux", path = "linux/lib.rs")]
#[cfg_attr(target_os = "macos", path = "macos/lib.rs")]
#[cfg_attr(r#true, path = "unknown/lib.rs")]
pub(crate) mod platform;
