use std::mem::transmute;

use windows::core::{HSTRING, PCWSTR, PWSTR};
use windows::Win32::Foundation::NO_ERROR;
use windows::Win32::System::Diagnostics::Debug::WriteProcessMemory;
use windows::Win32::System::LibraryLoader::GetModuleHandleW;
use windows::Win32::System::Memory::{MEM_COMMIT, MEM_RESERVE, PAGE_EXECUTE_READWRITE};
use windows::Win32::System::Threading::{
    CreateProcessW, CreateRemoteThread, WaitForSingleObject, INFINITE, PROCESS_CREATION_FLAGS,
    PROCESS_INFORMATION, STARTUPINFOW,
};
use windows::Win32::System::{LibraryLoader::GetProcAddress, Memory::VirtualAllocEx};
use windows::{s, w};

/// The entry point for the Windows version of the Rustic Bootstrapper.
pub fn main() {
    let args = std::env::args().collect::<Vec<String>>();
    let isaac_path = find_isaac_path(&args).expect("Failed to find 'isaac-ng.exe'!");
    let inject_dll = find_inject_dll(&args).expect("Failed to find 'rustic-inject.dll'!");

    unsafe {
        start(&isaac_path, inject_dll);
    }
}

/// Attempts to resolve the path to the TBoI:R executable, named
/// `isaac-ng.exe`.
///
/// Checks the `RUSTIC_ISAAC_PATH` environment variable first, then the first
/// argument after the executable name, then searches the current directory.
fn find_isaac_path(args: &Vec<String>) -> Result<String, ()> {
    if let Ok(path) = std::env::var("RUSTIC_ISAAC_PATH") {
        println!("RUSTIC_ISAAC_PATH environment variable found: {}", path);
        return Ok(path);
    }

    if args.len() >= 2 {
        let path = std::path::Path::new(&args[1]);
        println!("Using first argument as path: {:?}", path);
        // check if path is file or dir
        if path.is_file() {
            println!("Treating path as file!");
            return Ok(path.to_str().unwrap().to_string());
        }

        println!("Treating path as directory, searching for 'isaac-ng.exe'!");
        let mut dir = path.to_path_buf();
        dir.push("isaac-ng.exe");
        if dir.exists() {
            return Ok(dir.to_str().unwrap().to_string());
        }

        println!("'isaac-ng.exe' not found in specified directory!");
    }

    println!("Searching for 'isaac-ng.exe' in current directory...");
    let mut current_dir = std::env::current_dir().unwrap();
    current_dir.push("isaac-ng.exe");
    if current_dir.exists() {
        return Ok(current_dir.to_str().unwrap().to_string());
    }

    Err(())
}

/// Attempts to resolve the path to the Rustic payload, named
/// `rustic-inject.dll`.
///
/// Checks the `RUSTIC_INJECT_DLL` environment variable first, then searches
/// the current directory.
fn find_inject_dll(_args: &Vec<String>) -> Result<String, ()> {
    if let Ok(path) = std::env::var("RUSTIC_INJECT_DLL") {
        println!("RUSTIC_INJECT_DLL environment variable found: {}", path);
        return Ok(path);
    }

    println!("Searching for 'rustic_inject.dll' in current directory...");
    let mut current_dir = std::env::current_dir().unwrap();
    current_dir.push("rustic_inject.dll");
    if current_dir.exists() {
        return Ok(current_dir.to_str().unwrap().to_string());
    }

    Err(())
}

/// Starts the TBoI:R process and injects the Rustic payload.
unsafe fn start(executable: &String, inject_dll: String) {
    println!("Using path: {}", executable);
    println!("Using inject dll: {}", inject_dll);

    let mut startup_info = STARTUPINFOW::default();
    let mut process_info = PROCESS_INFORMATION::default();
    let dll_size = (inject_dll.len() + 1) * 2;

    println!("Creating process...");

    CreateProcessW(
        PCWSTR(HSTRING::from(executable).as_ptr()),
        PWSTR(HSTRING::from("").as_ptr().cast_mut()),
        None,
        None,
        false,
        PROCESS_CREATION_FLAGS(0),
        None,
        None,
        &mut startup_info,
        &mut process_info,
    );

    let process_handle = process_info.hProcess;

    println!("Getting module handle...");

    let kernel32 = GetModuleHandleW(w!("kernel32.dll"));
    let kernel32 = kernel32.unwrap();

    println!("Getting proc address...");

    let load_library_w_addr = GetProcAddress(kernel32, s!("LoadLibraryW"));
    let load_library_w_addr = load_library_w_addr.unwrap();

    println!("Allocating memory...");

    let mem_addr = VirtualAllocEx(
        process_handle,
        None,
        dll_size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE,
    );

    if mem_addr.is_null() {
        println!("Failed to allocate memory!");
        return;
    }

    println!("Writing memory...");

    let mem = WriteProcessMemory(
        process_handle,
        mem_addr,
        PCWSTR(HSTRING::from(inject_dll).as_ptr().cast_mut()).as_ptr() as _,
        dll_size,
        None,
    );

    if !mem.as_bool() {
        println!("Failed to write memory!");
        return;
    }

    println!("Creating remote thread...");

    let handle = CreateRemoteThread(
        process_handle,
        None,
        0,
        Some(transmute(load_library_w_addr as usize)),
        Some(mem_addr),
        0,
        None,
    )
    .expect("Failed to create remote thread!");

    if WaitForSingleObject(handle, INFINITE) != NO_ERROR {
        println!("Failed to wait for single object!");
        return;
    }

    println!("Done!");
}
