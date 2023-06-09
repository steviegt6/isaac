#[cfg_attr(target_os = "windows", path = "windows/main.rs")]
#[cfg_attr(target_os = "linux", path = "linux/main.rs")]
#[cfg_attr(target_os = "macos", path = "macos/main.rs")]
#[cfg_attr(r#true, path = "unknown/main.rs")]
pub(crate) mod platform;
