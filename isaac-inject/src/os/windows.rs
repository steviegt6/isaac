use retour_utils::hook_module;
use windows::{
    w,
    Win32::{
        Foundation::{BOOL, HWND, TRUE},
        UI::WindowsAndMessaging::{MessageBoxW, MESSAGEBOX_STYLE},
    },
};

#[hook_module("user32.dll")]
mod user32 {
    use windows::{core::PCWSTR, w, Win32::Foundation::HWND};

    #[hook(unsafe extern "system" MessageBoxWHook, symbol = "MessageBoxW")]
    fn message_box_w_detour(hwnd: HWND, text: PCWSTR, _caption: PCWSTR, u_type: u32) -> i32 {
        let caption = w!("Nope, detoured!");
        unsafe { MessageBoxWHook.call(hwnd, text, caption, u_type) }
    }
}

#[no_mangle]
pub unsafe extern "system" fn DllMain(_module: usize, _reason: u32, _reserved: usize) -> BOOL {
    // DLL_PROCESS_ATTACH
    if _reason == 1 {
        user32::init_detours().expect("Failed to initialize detours.");
        MessageBoxW(
            HWND(0),
            w!("Hello from isaac-inject.dll!"),
            w!("isaac-inject.dll"),
            MESSAGEBOX_STYLE(0),
        );
    }

    TRUE
}
