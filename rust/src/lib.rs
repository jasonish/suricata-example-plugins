use suricata::SCLogNotice;

#[repr(C)]
#[allow(non_snake_case)]
pub struct SCPlugin {
    name: *const libc::c_char,
    license: *const libc::c_char,
    author: *const libc::c_char,
    Init: extern "C" fn(),
}

extern "C" fn rust_plugin_init() {
    SCLogNotice!("Initializing Rust plugin");
}

#[no_mangle]
extern "C" fn SCPluginRegister() -> *const SCPlugin {
    suricata::plugin::init();
    let plugin = SCPlugin {
        name: b"rust\0".as_ptr() as *const libc::c_char,
        license: b"author\0".as_ptr() as *const libc::c_char,
        author: b"author\0".as_ptr() as *const libc::c_char,
        Init: rust_plugin_init,
    };
    return Box::into_raw(Box::new(plugin));
}