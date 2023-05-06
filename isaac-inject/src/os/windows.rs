use windows::{
    w,
    Win32::{
        Foundation::{BOOL, HWND, TRUE},
        UI::WindowsAndMessaging::{MessageBoxW, MESSAGEBOX_STYLE},
    },
};

#[no_mangle]
pub unsafe extern "system" fn DllMain(_module: usize, _reason: u32, _reserved: usize) -> BOOL {
    // DLL_PROCESS_ATTACH
    if _reason == 1 {
        MessageBoxW(
            HWND(0),
            w!("Hello from isaac-inject.dll!"),
            w!("isaac-inject.dll"),
            MESSAGEBOX_STYLE(0),
        );
    }

    TRUE
}
