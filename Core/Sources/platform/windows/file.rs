// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::{
    ffi::{c_char, c_void},
    mem::size_of,
    ptr::{null, null_mut},
};

use crate::{interop::StringWideEncodeable, io::{AccessOptions, Error}, os_impl::HandleExtensions};

extern "C" {
    fn CreateFileW(
        path: *const u16,
        access_mode: u32,
        share_mode: u32,
        security_attributes: *const c_void,
        creation_mode: u32,
        flags: u32,
        template_file: *mut c_void,
    ) -> *mut c_void;

    fn DeleteFileW(path: *const u16) -> i32;
    fn GetFileSizeEx(file: *mut c_void, size: *mut u64) -> i32;
    fn GetLastError() -> u32;

    fn _open_osfhandle(handle: *mut c_void, flags: i32) -> i32;
    fn _get_osfhandle(handle: i32) -> *mut c_void;
    fn _fdopen(handle: i32, mode: *const c_char) -> *mut c_void;
    fn _fileno(stream: *mut c_void) -> i32;

    fn fread(buffer: *mut c_void, size: usize, count: usize, stream: *mut c_void) -> usize;
    fn fwrite(buffer: *const c_void, size: usize, count: usize, stream: *mut c_void) -> usize;
    fn _fseeki64(stream: *mut c_void, offset: i64, origin: i32) -> i32;
    fn feof(stream: *mut c_void) -> i32;
    fn fclose(stream: *mut c_void) -> i32;
    fn ferror(stream: *mut c_void) -> i32;
}

pub struct File {
    handle: *mut c_void,
}

impl AccessOptions {
    fn as_access_type(&self) -> u32 {
        assert!(self.read | self.write);

        (if self.read  { 0x80000000 /* GENERIC_READ  */ } else { 0 }) |
        (if self.write { 0x40000000 /* GENERIC_WRITE */ } else { 0 })
    }

    fn as_share_mode(&self) -> u32 {
        if !self.write && self.share { 0x01 /* FILE_SHARE_READ */ } else { 0 }
    }

    fn get_descriptor(&self, handle: *mut c_void) -> *mut c_void {
        let (flag, mode) = match (self.read, self.write) {
            (true,  false) => (0x8000, "rb\0"),  // O_BINARY | O_RDONLY
            (false, true)  => (0x8001, "wb\0"),  // O_BINARY | O_WRONLY
            (true,  true)  => (0x8002, "rb+\0"), // O_BINARY | O_RDWR

            _ => { unreachable!() }
        };

        let descriptor = unsafe { _fdopen(_open_osfhandle(handle, flag), mode.as_ptr() as *const c_char) };
        assert_ne!(descriptor, null_mut());

        descriptor
    }
}

impl File {
    /// Opens the file at the given path with the given options.
    pub fn open_ex(path: &str, options: AccessOptions) -> Result<File, Error> {
        // OPEN_EXISTING (0x03), FILE_ATTRIBUTES_NORMAL (0x80)
        let handle = unsafe { CreateFileW(path.to_wide().as_ptr(), options.as_access_type(), options.as_share_mode(), null(), 0x03, 0x80, null_mut()) };
        if handle.is_invalid_handle() {
            let result = unsafe { GetLastError() };
            return match result {
                // ERROR_ACCESS_DENIED
                5 => Err(Error::AccessDenied),

                // ERROR_PATH_NOT_FOUND
                // ERROR_FILE_NOT_FOUND
                // ERROR_NOT_FOUND
                2 | 3 | 1168 => Err(Error::NotFound),

                _ => {
                    panic!("CreateFileW failed");
                }
            };
        }

        Ok(File { handle: options.get_descriptor(handle) })
    }

    /// Opens the file at the given path.
    pub fn open(path: &str) -> Result<File, Error> {
        Self::open_ex(path, AccessOptions::new())
    }

    /// Creates a new file at the given path.
    pub fn create_ex(path: &str, options: AccessOptions) -> Result<File, Error> {
        // CREATE_NEW (0x01), FILE_ATTRIBUTES_NORMAL (0x80)
        let handle = unsafe { CreateFileW(path.to_wide().as_ptr(), options.as_access_type(), options.as_share_mode(), null(), 0x01, 0x80, null_mut()) };
        if handle.is_invalid_handle() {
            let result = unsafe { GetLastError() };
            return match result {
                // ERROR_ACCESS_DENIED
                5 => Err(Error::AccessDenied),

                // ERROR_FILE_NOT_FOUND
                // ERROR_PATH_NOT_FOUND
                // ERROR_NOT_FOUND
                2 | 3 | 1168 => Err(Error::NotFound),

                // ERROR_FILE_EXISTS
                // ERROR_ALREADY_EXISTS
                80 | 183 => Err(Error::AlreadyExists),

                _ => {
                    panic!("CreateFileW failed");
                }
            };
        }

        Ok(File { handle: options.get_descriptor(handle) })
    }

    /// Creates a new file at the given path.
    pub fn create(path: &str) -> Result<File, Error> {
        Self::create_ex(path, AccessOptions::new().write(true))
    }

    /// Reads the equivalent byte size of the block into it.
    pub fn read<T>(&mut self, block: &mut [T]) -> Result<(), Error> {
        let count = unsafe { fread(block.as_mut_ptr() as *mut c_void, size_of::<T>(), block.len(), self.handle) };
        if count == block.len() {
            return Ok(());
        }

        if unsafe { feof(self.handle) } != 0 {
            return Err(Error::ReachedEnd);
        }

        match unsafe { ferror(self.handle) } {
            // EPERM
            1 => Err(Error::InvalidOperation),

            // ENOMEM
            12 => Err(Error::NotEnoughMemory),

            _ => {
                panic!("fwrite failed");
            }
        }
    }

    /// Writes all elements in the block to the file.
    pub fn write<T>(&mut self, block: &[T]) -> Result<(), Error> {
        let count = unsafe { fwrite(block.as_ptr() as *const c_void, size_of::<T>(), block.len(), self.handle) };
        if count == block.len() {
            return Ok(());
        }

        if unsafe { feof(self.handle) } != 0 {
            return Err(Error::ReachedEnd);
        }

        match unsafe { ferror(self.handle) } {
            // EPERM
            1 => Err(Error::InvalidOperation),

            // ENOMEM
            12 => Err(Error::NotEnoughMemory),

            _ => {
                panic!("fwrite failed");
            }
        }
    }

    /// Seeks to the given byte offset.
    pub fn seek(&mut self, offset: usize) -> Result<(), Error> {
        if offset > i64::MAX as usize {
            return Err(Error::InvalidSize);
        }

        // SEEK_SET (0)
        let result = unsafe { _fseeki64(self.handle, offset as i64, 0) };
        if result == 0 {
            return Ok(());
        }

        if unsafe { feof(self.handle) } != 0 {
            return Err(Error::ReachedEnd);
        }

        match unsafe { ferror(self.handle) } {
            // ENOMEM
            12 => Err(Error::NotEnoughMemory),

            _ => {
                panic!("_fseeki64 failed");
            }
        }
    }

    /// Retrieves the size of the file.
    pub fn len(&self) -> usize {
        let no = unsafe { _fileno(self.handle) };
        assert!(no > 0);

        let file = unsafe { _get_osfhandle(no) };
        assert!(file.is_invalid_handle());

        let mut size: u64 = 0;
        let result = unsafe { GetFileSizeEx(file, &mut size) };
        assert_eq!(result, 1);

        size as usize
    }

    /// Deletes the file at the given path.
    pub fn delete(path: &str) -> Result<(), Error> {
        let success = unsafe { DeleteFileW(path.to_wide().as_ptr()) } == 1;
        if success {
            return Ok(());
        }

        match unsafe { GetLastError() } {
            // ERROR_ACCESS_DENIED
            5 => Err(Error::AccessDenied),

            // ERROR_FILE_NOT_FOUND
            // ERROR_PATH_NOT_FOUND
            // ERROR_NOT_FOUND
            2 | 3 | 1168 => Err(Error::NotFound),

            _ => {
                panic!("DeleteFileW failed");
            }
        }
    }
}

impl Drop for File {
    fn drop(&mut self) {
        unsafe { fclose(self.handle) };
    }
}
