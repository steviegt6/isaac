use std::panic::PanicInfo;

use retour_utils::hook_module;
use windows::{
    core::{HSTRING, PCWSTR},
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

pub unsafe fn panic(info: &PanicInfo) {
    MessageBoxW(
        HWND(0),
        PCWSTR(HSTRING::from(info.to_string()).as_ptr().cast_mut()),
        w!("Panic! - error originating from rustic"),
        MESSAGEBOX_STYLE(0),
    );
}

#[no_mangle]
pub unsafe extern "system" fn DllMain(_module: usize, _reason: u32, _reserved: usize) -> BOOL {
    // DLL_PROCESS_ATTACH
    if _reason == 1 {
        std::panic::set_hook(Box::new(|info| panic(info)));

        panic!("test");

        user32::init_detours().expect("Failed to initialize detours.");
        MessageBoxW(
            HWND(0),
            w!("Hello from isaac-inject.dll!"),
            w!("isaac-inject.dll"),
            MESSAGEBOX_STYLE(0),
        );

        //let pid = GetCurrentProcessId();
        //let phandle =
        //    OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, pid).expect("Failed to open process.");
        //if phandle.is_invalid() {
        //    panic!("phandle is invalid.");
        //}
        //
        //\/*if*/
        //ResumeThread(phandle) /*== -1 {
        //    panic!("Failed to resume thread.");
        //}*/;
        //
        //CloseHandle(phandle);
    }

    TRUE
}
