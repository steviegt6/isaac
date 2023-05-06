#[cfg_attr(target_os = "windows", path = "os/windows.rs")]
#[cfg_attr(target_os = "linux", path = "os/linux.rs")]
#[cfg_attr(target_os = "macos", path = "os/macos.rs")]
#[cfg_attr(r#true, path = "os/unknown.rs")]
mod os;

fn main() {
    os::main();
}
