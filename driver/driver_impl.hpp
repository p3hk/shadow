#pragma once
#define NOMINMAX
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <string>
#include <cmath>
#include <type_traits>
#include <stdexcept>
#include <mutex>
#include <unordered_map>
#include <vector>

// NT_SUCCESS macro for NT API calls
#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

extern uintptr_t virtualaddy;

namespace mem {
    extern HANDLE process_handle;
    extern INT32 process_id;
    extern std::recursive_mutex write_mutex;  // Using recursive_mutex to allow recursive locking

    bool find_driver();
    void read_physical(PVOID address, PVOID buffer, DWORD size);
    void write_physical(PVOID address, PVOID buffer, DWORD size);
    uintptr_t find_image();
    uintptr_t get_guarded_region();
    INT32 find_process(LPCTSTR process_name);
    std::string readString(uint64_t address, size_t max_length = 128);

    // Memory cache system
    struct MemoryCache {
    private:
        std::unordered_map<uintptr_t, std::vector<uint8_t>> read_cache;
        std::recursive_mutex cache_mutex; // Use recursive mutex to allow nested locks
        const size_t MAX_CACHE_SIZE = 2000;
        const size_t CACHE_ENTRY_LIMIT = 1024; // Max size of a single cache entry

        void check_cache_size() {
            std::lock_guard<std::recursive_mutex> lock(cache_mutex);
            if (read_cache.size() > MAX_CACHE_SIZE) {
                read_cache.clear();
            }
        }

    public:
        template<typename T>
        bool get(uintptr_t address, T& out_value) {
            // Only reject null addresses
            if (!address) {
                return false;
            }
            
            try {
                std::lock_guard<std::recursive_mutex> lock(cache_mutex);
                auto it = read_cache.find(address);
                if (it != read_cache.end() && it->second.size() >= sizeof(T)) {
                    memcpy(&out_value, it->second.data(), sizeof(T));
                    return true;
                }
                return false;
            }
            catch (...) {
                return false;
            }
        }

        template<typename T>
        void store(uintptr_t address, const T& value) {
            // Only reject null addresses
            if (!address) {
                return;
            }
            
            try {
                // Don't cache values that are too large
                if (sizeof(T) > CACHE_ENTRY_LIMIT) {
                    return;
                }
                
                std::vector<uint8_t> data(sizeof(T));
                memcpy(data.data(), &value, sizeof(T));

                std::lock_guard<std::recursive_mutex> lock(cache_mutex);
                read_cache[address] = data;
                check_cache_size();
            }
            catch (...) {
                // Silently handle exceptions
            }
        }

        void clear() {
            std::lock_guard<std::recursive_mutex> lock(cache_mutex);
            read_cache.clear();
        }
    };

    extern MemoryCache memory_cache;
}

// Forward declarations for template functions
template <typename T>
T read(uint64_t address);

template <typename T>
bool read_safe(uint64_t address, T& out_value);

template <typename T>
T write(uint64_t address, const T& buffer);

// Read array of elements from memory
template <typename T>
std::vector<T> read_array(uint64_t address, size_t size) {
    // Basic validation - reject null address and excessive sizes
    if (!address || size == 0 || size > 10000 || 
        !mem::process_handle || mem::process_handle == INVALID_HANDLE_VALUE) {
        return std::vector<T>(); // Return empty vector for invalid parameters
    }
    
    std::vector<T> buffer(size);
    try {
        mem::read_physical(reinterpret_cast<PVOID>(address), buffer.data(), size * sizeof(T));
    }
    catch (...) {
        buffer.clear();
    }
    return buffer;
}

// Improved implementation of readString function
inline std::string mem::readString(uint64_t address, size_t max_length) {
    if (!address || address < 0x1000 || address > 0x7FFFFFFFFFF || 
        !mem::process_handle || mem::process_handle == INVALID_HANDLE_VALUE) {
        return ""; // Invalid address or process handle
    }

    std::string result;
    result.reserve(max_length);

    try {
        // For Roblox Job Names, we need to check multiple string formats
        
        // First approach: Try to read as direct char* string with null terminator
        for (size_t i = 0; i < max_length; i++) {
            char c = read<char>(address + i);
            if (c == 0) break;
            if (c < 0x20 || c > 0x7E) { // Check if it's a reasonable ASCII character
                if (i == 0) break; // If first char is invalid, it's not a direct string
                // Allow some non-printable chars in the middle
                if (c != '\t' && c != '\r' && c != '\n') {
                    c = '.'; // Replace with dot for readability
                }
            }
            result.push_back(c);
        }

        // If direct reading didn't work, try the C++ string approach
        if (result.empty()) {
            // Check if it's a C++ std::string object
            const auto stringLength = read<int>(address + 0x18);
            
            // Validate string length to avoid crashes
            if (stringLength >= 0 && stringLength < 1000) {
                // Handle small string optimization (SSO)
                std::uint64_t stringAddress;
                if (stringLength >= 16) {
                    // For strings >= 16 chars, data is stored at pointer in first 8 bytes
                    stringAddress = read<std::uint64_t>(address);
                    if (!stringAddress || stringAddress < 0x1000 || stringAddress > 0x7FFFFFFFFFF) return "";
                }
                else {
                    // For strings < 16 chars, data is stored inline within the string object
                    stringAddress = address;
                }

                // Read characters up to the reported length
                const size_t bytesToRead = (std::min)(static_cast<size_t>(stringLength), max_length);
                for (size_t i = 0; i < bytesToRead; i++) {
                    char c = read<char>(stringAddress + i);
                    if (c == 0) break;
                    result.push_back(c);
                }
            }
        }
    }
    catch (...) {
        // Handle exceptions silently
    }

    return result;
}

// Helper function for safe write operation
template <typename T>
static bool write_safe_internal(uint64_t address, const T& buffer) {
    SIZE_T bytesWritten = 0;
    return WriteProcessMemory(
        mem::process_handle,
        reinterpret_cast<LPVOID>(address),
        &buffer,
        sizeof(T),
        &bytesWritten
    ) && (bytesWritten == sizeof(T));
}

// Template implementations
template <typename T>
T read(uint64_t address) {
    // Enhanced validation for null and invalid addresses
    if (!address || address < 0x1000 || address > 0x7FFFFFFFFFF ||
        !mem::process_handle || mem::process_handle == INVALID_HANDLE_VALUE) {
        return T{}; // Return default-initialized value for invalid addresses
    }
    
    T buffer{};
    try {
        mem::read_physical((PVOID)address, &buffer, sizeof(T));
    }
    catch (...) {
        // Return default-initialized value on exception
        buffer = T{};
    }
    
    return buffer;
}

template <typename T>
bool read_safe(uint64_t address, T& out_value) {
    // Enhanced validation for address and handle
    if (!address || address < 0x1000 || address > 0x7FFFFFFFFFF ||
        !mem::process_handle || mem::process_handle == INVALID_HANDLE_VALUE) {
        out_value = T{};
        return false;
    }

    try {
        // Check cache first
        if (mem::memory_cache.get(address, out_value)) {
            return true;
        }

        T buffer{};
        mem::read_physical((PVOID)address, &buffer, sizeof(T));
        out_value = buffer;

        // Store in cache
        mem::memory_cache.store(address, out_value);

        // Basic validation for common types
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
            // Check if value is NaN or infinity
            if (std::isnan(out_value) || !std::isfinite(out_value)) {
                out_value = T{};
                return false;
            }
        }

        return true;
    }
    catch (...) {
        out_value = T{};
        return false;
    }
}

template <typename T>
T write(uint64_t address, const T& buffer) {
    // Enhanced validation for address and handle
    if (!address || address < 0x1000 || address > 0x7FFFFFFFFFF ||
        !mem::process_handle || mem::process_handle == INVALID_HANDLE_VALUE) {
        return buffer; // Return the buffer value without writing
    }

    try {
        // Lock mutex for thread safety
        std::lock_guard<std::recursive_mutex> lock(mem::write_mutex);

        mem::write_physical(reinterpret_cast<PVOID>(address), const_cast<PVOID>(static_cast<const void*>(&buffer)), sizeof(T));

        // Update cache only if write was attempted
        mem::memory_cache.store(address, buffer);
    }
    catch (...) {
        // Silently handle any exceptions during the write operation
    }

    return buffer;
}