use std::{
    ffi::{c_char, CStr},
    path::{Path, PathBuf},
    ptr::addr_of_mut,
};

use netcorehost::{nethost, pdcstr, pdcstring::PdCString};

#[repr(C)]
#[derive(Debug)]
pub struct InitArgs {}

#[no_mangle]
pub unsafe extern "C" fn host_start_clr(module_path: *const c_char) {
    println!("Starting CLR... (Rust)");

    let module_path: &CStr = CStr::from_ptr(module_path);
    let module_path: &str = module_path.to_str().unwrap();
    let module_path: PathBuf = module_path.into();
    let module_path = module_path.parent().unwrap();
    println!(
        "Module path directory: {}",
        module_path.to_str().unwrap().to_string()
    );

    let runtime_config_path = module_path.join("Tomat.Testament.API.runtimeconfig.json");
    println!(
        "Runtime config path: {}",
        runtime_config_path.to_str().unwrap().to_string()
    );

    let hostfxr = nethost::load_hostfxr().expect("Error loading hostfxr");

    let context = hostfxr
        .initialize_for_runtime_config(pdcstr(runtime_config_path))
        .unwrap();

    let api = context
        .get_delegate_loader_for_assembly(pdcstr(module_path.join("Tomat.Testament.API.dll")))
        .unwrap();

    let init = api
        .get_function_with_unmanaged_callers_only::<fn(*mut InitArgs)>(
            pdcstr!("Tomat.Testament.API.Native, Tomat.Testament.API"),
            pdcstr!("Initialize"),
        )
        .unwrap();

    println!("Calling Initialize... (Rust)");
    let mut args = InitArgs {};
    init(addr_of_mut!(args));
    println!("Initialize called! (Rust)");
}

pub fn pdcstr<P: AsRef<Path>>(path: P) -> PdCString {
    PdCString::from_os_str(path.as_ref().as_os_str()).unwrap()
}
