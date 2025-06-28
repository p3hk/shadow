#include "driver_impl.hpp"
#include <Psapi.h>
#include <mutex>

HANDLE mem::process_handle = NULL;
INT32 mem::process_id;
mem::MemoryCache mem::memory_cache;
std::recursive_mutex mem::write_mutex;  // Use recursive mutex instead

uintptr_t virtualaddy;
// NT API function prototypes
typedef NTSTATUS(WINAPI* pNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesRead);
typedef NTSTATUS(WINAPI* pNtWriteVirtualMemory)(HANDLE Processhandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);

// NT API function pointers
pNtReadVirtualMemory NtReadVirtualMemory = nullptr;
pNtWriteVirtualMemory NtWriteVirtualMemory = nullptr;

namespace mem {
    bool find_driver() {
        // Initialize NT functions if available for better performance
        HMODULE ntdllModule = GetModuleHandleA("ntdll.dll");
        if (ntdllModule) {
            NtReadVirtualMemory = (pNtReadVirtualMemory)GetProcAddress(ntdllModule, "NtReadVirtualMemory");
            NtWriteVirtualMemory = (pNtWriteVirtualMemory)GetProcAddress(ntdllModule, "NtWriteVirtualMemory");
        }

        // Always return true since we're using user-mode now
        return true;
    }

    void read_physical(PVOID address, PVOID buffer, DWORD size) {
        // Enhanced validation - prevent null pointers, invalid handle, and zero size
        if (!process_handle || process_handle == INVALID_HANDLE_VALUE || 
            !buffer || size == 0 || !address) {
            memset(buffer, 0, size); // Initialize buffer with zeros if validation fails
            return;
        }

        try {
            SIZE_T bytesRead = 0;

            // First try NT API for better performance
            if (NtReadVirtualMemory) {
                ULONG bytes = 0;
                NTSTATUS status = NtReadVirtualMemory(
                    process_handle,
                    address,
                    buffer,
                    size,
                    &bytes
                );

                // If successful, return
                if (NT_SUCCESS(status)) {
                    return;
                }
            }

            // Fall back to standard API
            if (!ReadProcessMemory(
                process_handle,
                address,
                buffer,
                size,
                &bytesRead
            )) {
                // On failure, zero out the buffer to prevent using garbage data
                memset(buffer, 0, size);
            }
        }
        catch (...) {
            // Handle exceptions by clearing the buffer
            memset(buffer, 0, size);
        }
    }

    // Helper function to perform the actual write operation
    static bool write_physical_internal(PVOID address, PVOID buffer, DWORD size) {
        // First try NT API for better performance
        if (NtWriteVirtualMemory) {
            ULONG bytes = 0;
            NTSTATUS status = NtWriteVirtualMemory(
                process_handle,
                address,
                buffer,
                size,
                &bytes
            );

            if (NT_SUCCESS(status)) {
                return true;
            }
        }

        // Fall back to standard API
        SIZE_T bytesWritten = 0;
        return WriteProcessMemory(
            process_handle,
            address,
            buffer,
            size,
            &bytesWritten
        ) && (bytesWritten == size);
    }

    void write_physical(PVOID address, PVOID buffer, DWORD size) {
        // Enhanced validation
        if (!process_handle || process_handle == INVALID_HANDLE_VALUE) {
            return;
        }

        if (!buffer || size == 0 || !address) {
            return;
        }

        // Validate address range - check for null and kernel addresses
        uintptr_t addr = reinterpret_cast<uintptr_t>(address);
        if (addr == 0 || addr < 0x10000 || addr > 0x7FFFFFFFFFF) {
            return;
        }

        // Lock mutex
        std::lock_guard<std::recursive_mutex> lock(write_mutex);

        // Perform write with basic exception handling
        try {
            write_physical_internal(address, buffer, size);
        }
        catch (...) {
            // Silently handle any exceptions
        }
    }

    uintptr_t find_image() {
        if (!process_handle) {
            return 0;
        }

        HMODULE modules[1024];
        DWORD needed;

        if (!EnumProcessModules(process_handle, modules, sizeof(modules), &needed)) {
            return 0;
        }

        // Return the base module
        return reinterpret_cast<uintptr_t>(modules[0]);
    }

    uintptr_t get_guarded_region() {
        // User-mode implementation doesn't support guarded regions
        // Returning 0 as a fallback
        return 0;
    }

    INT32 find_process(LPCTSTR process_name) {
        PROCESSENTRY32 pt;
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hsnap, &pt)) {
            do {
                if (!lstrcmpi(pt.szExeFile, process_name)) {
                    INT32 pid = pt.th32ProcessID;
                    process_id = pid;

                    // Close previous handle if open
                    if (process_handle && process_handle != INVALID_HANDLE_VALUE) {
                        CloseHandle(process_handle);
                    }

                    // Open new handle with all access
                    process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

                    CloseHandle(hsnap);
                    return pid;
                }
            } while (Process32Next(hsnap, &pt));
        }

        CloseHandle(hsnap);
        return 0;
    }

    std::string read_string(uint64_t address, size_t max_length) {
        if (!address || !process_handle || max_length == 0) {
            return "";
        }

        std::string result;
        result.reserve(max_length);

        char buffer[128];
        size_t read_size = (max_length < sizeof(buffer)) ? max_length : (sizeof(buffer) - 1);

        if (read_size == 0) {
            return "";
        }

        SIZE_T bytesRead = 0;
        bool success = ReadProcessMemory(
            process_handle,
            reinterpret_cast<LPCVOID>(address),
            buffer,
            read_size,
            &bytesRead
        );

        if (!success || bytesRead == 0) {
            return "";
        }

        for (size_t i = 0; i < bytesRead; ++i) {
            if (buffer[i] == '\0') {
                break;
            }
            if (buffer[i] < 32 || buffer[i] > 126) {
                // Skip non-printable characters
                continue;
            }
            result.push_back(buffer[i]);
        }

        return result;
    }
}