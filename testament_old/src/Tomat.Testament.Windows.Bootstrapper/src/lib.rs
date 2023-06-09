use windows::{
    w,
    Win32::{
        Foundation::{BOOL, HWND, TRUE},
        UI::WindowsAndMessaging::{MessageBoxW, MESSAGEBOX_STYLE},
    },
};

#[no_mangle]
pub unsafe extern "system" fn DllMain(_module: usize, _reason: u32, _reserved: usize) -> BOOL {
    MessageBoxW(
        HWND(0),
        w!("Hello, world!"),
        w!("Hello, world!"),
        MESSAGEBOX_STYLE(0),
    );

    TRUE
}
