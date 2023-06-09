use retour_utils::hook_module;

#[hook_module("lua5.3.3r.dll")]
pub(crate) mod detours {
    use rlua_lua53_sys::{lua_Alloc, lua_State};
    use std::os::raw::c_void;
    use windows::{
        w,
        Win32::{
            Foundation::HWND,
            UI::WindowsAndMessaging::{MessageBoxW, MESSAGEBOX_STYLE},
        },
    };

    #[hook(unsafe extern "C" Lua_newstate, symbol = "Lua_newstate")]
    pub fn new_state(f: *mut lua_Alloc, ud: *mut c_void) -> *mut lua_State {
        unsafe {
            MessageBoxW(HWND(0), w!("omg"), w!("huge"), MESSAGEBOX_STYLE(0));
            let state = Lua_newstate.call(f, ud);

            panic!("AHHHHHHHHHHH");

            state
        }
    }
}
