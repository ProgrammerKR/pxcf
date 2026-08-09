use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use std::ptr;

#[repr(C)]
pub struct PxcfError {
    pub code: i32,
    pub line: i32,
    pub column: i32,
    pub offset: usize,
    pub message: [c_char; 256],
    pub file_path: [c_char; 256],
}

pub enum PxcfValue {}
pub enum PxcfDocument {}

extern "C" {
    pub fn pxcf_parse_string(source: *const c_char, length: usize, error: *mut PxcfError) -> *mut PxcfDocument;
    pub fn pxcf_document_free(document: *mut PxcfDocument);
    // Bind more as needed
}

pub struct Document {
    ptr: *mut PxcfDocument,
}

impl Document {
    pub fn parse(source: &str) -> Result<Self, String> {
        let c_str = CString::new(source).map_err(|_| "String contains null byte")?;
        let mut err = PxcfError {
            code: 0,
            line: 0,
            column: 0,
            offset: 0,
            message: [0; 256],
            file_path: [0; 256],
        };
        
        let ptr = unsafe { pxcf_parse_string(c_str.as_ptr(), source.len(), &mut err) };
        if ptr.is_null() {
            let msg = unsafe { CStr::from_ptr(err.message.as_ptr()) };
            return Err(format!("Parse error {}: {}", err.code, msg.to_string_lossy()));
        }
        
        Ok(Self { ptr })
    }
}

impl Drop for Document {
    fn drop(&mut self) {
        if !self.ptr.is_null() {
            unsafe { pxcf_document_free(self.ptr) };
        }
    }
}
