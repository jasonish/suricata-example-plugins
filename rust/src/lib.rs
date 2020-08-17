use suricata::{SCLogNotice};

#[repr(C)]
#[allow(non_snake_case)]
pub struct SCPlugin {
    name: *const libc::c_char,
    license: *const libc::c_char,
    author: *const libc::c_char,
    Init: extern "C" fn(),
}

extern "C" fn rust_plugin_init() {
    println!("rust_plugin_init");
    SCLogNotice!("Initializing Rust plugin");
}

#[export_name = "PluginSpec"]
static mut SPEC: Option<SCPlugin> = None;

#[no_mangle]
unsafe extern "C" fn __init() {
    let spec = SCPlugin {
        name: b"rust\0".as_ptr() as *const libc::c_char,
        license: b"author\0".as_ptr() as *const libc::c_char,
        author: b"author\0".as_ptr() as *const libc::c_char,
        Init: rust_plugin_init,
    };
    SPEC = Some(spec);
}

#[link_section = ".init_array"]
#[allow(dead_code)]
static INITIALIZE: unsafe extern "C" fn() = __init;
