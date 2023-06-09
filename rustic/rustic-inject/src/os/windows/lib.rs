mod hooks;

use retour_utils::hook_module;
use std::panic::PanicInfo;
use windows::{
    core::{HSTRING, PCWSTR},
    w,
    Win32::{
        Foundation::{CloseHandle, BOOL, HWND, TRUE},
        System::{
            Console::AllocConsole,
            LibraryLoader::LoadLibraryW,
            SystemServices::DLL_PROCESS_ATTACH,
            Threading::{CreateThread, THREAD_CREATION_FLAGS},
        },
        UI::WindowsAndMessaging::{MessageBoxW, MESSAGEBOX_STYLE},
    },
};

pub unsafe fn panic(info: &PanicInfo) {
    MessageBoxW(
        HWND(0),
        PCWSTR(HSTRING::from(info.to_string()).as_ptr().cast_mut()),
        w!("Panic! - error originating from rustic"),
        MESSAGEBOX_STYLE(0),
    );
}

unsafe extern "system" fn main(_lp_thread_parameter: *mut ::core::ffi::c_void) -> u32 {
    // AllocConsole();

    hooks::kernel32::detours::init_detours().expect("Failed to initialize detours.");

    // sleep for 5 seconds
    std::thread::sleep(std::time::Duration::from_secs(1));
    hooks::lua::detours::init_detours().expect("Failed to initialize detours.");

    0
}

#[no_mangle]
pub unsafe extern "system" fn DllMain(module: usize, _reason: u32, _reserved: usize) -> BOOL {
    if _reason == DLL_PROCESS_ATTACH {
        std::panic::set_hook(Box::new(|info| panic(info)));

        CloseHandle(
            CreateThread(
                None,
                0,
                Some(main),
                Some(std::mem::transmute(module)),
                THREAD_CREATION_FLAGS(0),
                None,
            )
            .expect("Failed to create thread!"),
        );

        //LoadLibraryW(w!("lua5.3.3r.dll")).expect("Failed to load lua5.3.3r.dll.");
        //lua::init_detours().expect("Failed to initialize detours.");

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
