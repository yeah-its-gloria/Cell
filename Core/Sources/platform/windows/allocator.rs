// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::{
    alloc::{GlobalAlloc, Layout},
    ffi::c_void,
    ptr::null_mut,
};

extern "C" {
    fn GetProcessHeap() -> *mut c_void;
    fn HeapAlloc(heap: *mut c_void, flags: u32, bytes: usize) -> *mut c_void;
    fn HeapReAlloc(heap: *mut c_void, flags: u32, memory: *mut c_void, bytes: usize) -> *mut c_void;
    fn HeapFree(heap: *mut c_void, flags: u32, memory: *mut c_void) -> i32;
}

pub struct CellAllocator;

unsafe impl GlobalAlloc for CellAllocator {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        let block = HeapAlloc(GetProcessHeap(), 0, layout.size());
        assert_ne!(block, null_mut());

        block as *mut u8
    }

    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        let result = HeapFree(GetProcessHeap(), 0, ptr as *mut c_void);
        assert_eq!(result, 1);
    }

    unsafe fn alloc_zeroed(&self, layout: Layout) -> *mut u8 {
        // HEAP_ZERO_MEMORY (0x08)
        let block = HeapAlloc(GetProcessHeap(), 0x08, layout.size());
        assert_ne!(block, null_mut());

        block as *mut u8
    }

    unsafe fn realloc(&self, ptr: *mut u8, _layout: Layout, new_size: usize) -> *mut u8 {
        let block = HeapReAlloc(GetProcessHeap(), 0, ptr as *mut c_void, new_size);
        assert_ne!(block, null_mut());

        block as *mut u8
    }
}
