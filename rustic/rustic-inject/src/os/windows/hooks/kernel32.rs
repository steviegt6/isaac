use retour_utils::hook_module;

#[hook_module("kernel32.dll")]
pub(crate) mod detours {
    use std::io::BufWriter;
    use std::io::Write;
    use windows::{
        core::{HSTRING, PCSTR, PCWSTR},
        s, w,
        Win32::{
            Foundation::HWND,
            UI::WindowsAndMessaging::{MessageBoxA, MessageBoxW, MESSAGEBOX_STYLE},
        },
    };

    unsafe fn report_library_w(lp_lib_file_name: PCWSTR, module_addr: usize) -> usize {
        report_library(lp_lib_file_name.to_string().unwrap());

        module_addr
    }

    unsafe fn report_library_a(lp_lib_file_name: PCSTR, module_addr: usize) -> usize {
        report_library(lp_lib_file_name.to_string().unwrap());

        module_addr
    }

    unsafe fn report_library(lp_lib_file_name: String) {
        println!("Loaded library: {}", lp_lib_file_name);

        /*MessageBoxW(
            HWND(0),
            PCWSTR(HSTRING::from(lp_lib_file_name).as_ptr().cast_mut()),
            w!("Loaded library!"),
            MESSAGEBOX_STYLE(0),
        );*/

        // write to file "loaded_libraries.txt" using write!

        let mut file = std::fs::File::create(format!("{}.txt", { &lp_lib_file_name }))
            .expect("Failed to create file.");

        write!(file, "{}\n", lp_lib_file_name.to_string()).expect("Failed to write to file.");
    }

    #[hook(unsafe extern "system" LoadLibraryA, symbol = "LoadLibraryA")]
    pub fn load_library_a(lp_lib_file_name: PCSTR) -> usize {
        unsafe { report_library_a(lp_lib_file_name, LoadLibraryA.call(lp_lib_file_name)) }
    }

    #[hook(unsafe extern "system" LoadLibraryW, symbol = "LoadLibraryW")]
    pub fn load_library_w(lp_lib_file_name: PCWSTR) -> usize {
        unsafe { report_library_w(lp_lib_file_name, LoadLibraryW.call(lp_lib_file_name)) }
    }

    #[hook(unsafe extern "system" LoadLibraryExA, symbol = "LoadLibraryExA")]
    pub fn load_library_ex_a(lp_lib_file_name: PCSTR, h_file: usize, dw_flags: usize) -> usize {
        unsafe {
            report_library_a(
                lp_lib_file_name,
                LoadLibraryExA.call(lp_lib_file_name, h_file, dw_flags),
            )
        }
    }

    #[hook(unsafe extern "system" LoadLibraryExW, symbol = "LoadLibraryExW")]
    pub fn load_library_ex_w(lp_lib_file_name: PCWSTR, h_file: usize, dw_flags: usize) -> usize {
        unsafe {
            report_library_w(
                lp_lib_file_name,
                LoadLibraryExW.call(lp_lib_file_name, h_file, dw_flags),
            )
        }
    }
}
