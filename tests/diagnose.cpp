忽悠// Copyright (c) 2026 Ankali-Aylina
// SPDX-License-Identifier: MIT

/**
 * @file diagnose.cpp
 * @brief 诊断工具 - 检查 PawnIO 驱动和 IntelMSR.bin
 */

#include "PawnIoModule.h"
#include <iostream>
#include <windows.h>

void CheckDriverStatus() {
    std::cout << "=== Checking PawnIO Driver Status ===" << std::endl;
    
    // 1. 检查设备是否存在
    HANDLE hDevice = CreateFileW(
        L"\\\\?\\GLOBALROOT\\Device\\PawnIO",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hDevice == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        std::cout << "[FAIL] Cannot open PawnIO device" << std::endl;
        std::cout << "       Error code: " << error << std::endl;
        
        if (error == 2) {
            std::cout << "       Message: The system cannot find the file specified." << std::endl;
            std::cout << "       Solution: Please install PawnIO driver first!" << std::endl;
        } else if (error == 5) {
            std::cout << "       Message: Access is denied." << std::endl;
            std::cout << "       Solution: Run as Administrator!" << std::endl;
        }
    } else {
        std::cout << "[OK] PawnIO device is accessible" << std::endl;
        CloseHandle(hDevice);
    }
    
    std::cout << std::endl;
}

void CheckBinFile() {
    std::cout << "=== Checking IntelMSR.bin File ===" << std::endl;
    
    // 检查当前目录
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(L"IntelMSR.bin", &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        std::cout << "[OK] IntelMSR.bin found in current directory" << std::endl;
        std::cout << "     File size: " << findData.nFileSizeLow << " bytes" << std::endl;
        FindClose(hFind);
    } else {
        std::cout << "[WARN] IntelMSR.bin NOT found in current directory" << std::endl;
        std::cout << "       Current path: " << std::endl;
        
        wchar_t buffer[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, buffer);
        std::wcout << L"       " << buffer << std::endl;
    }
    
    // 检查程序所在目录
    wchar_t modulePath[MAX_PATH];
    GetModuleFileNameW(NULL, modulePath, MAX_PATH);
    std::wcout << L"     Program path: " << modulePath << std::endl;
    
    std::cout << std::endl;
}

void TestModuleLoad() {
    std::cout << "=== Testing Module Loading ===" << std::endl;
    
    PawnIoModule module;
    
    // 尝试从不同路径加载
    std::vector<std::wstring> paths = {
        L"IntelMSR.bin",                          // 当前目录
        L".\\IntelMSR.bin",                       // 显式当前目录
    };
    
    // 添加绝对路径
    wchar_t modulePath[MAX_PATH];
    GetModuleFileNameW(NULL, modulePath, MAX_PATH);
    std::wstring programDir(modulePath);
    size_t lastSlash = programDir.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        programDir = programDir.substr(0, lastSlash);
        paths.push_back(programDir + L"\\IntelMSR.bin");
    }
    
    for (const auto& path : paths) {
        std::wcout << L"Trying: " << path << std::endl;
        
        if (module.LoadFromFile(path)) {
            std::cout << "[OK] Module loaded successfully!" << std::endl;
            
            // 尝试执行一个简单的调用
            std::vector<long long> input = {0};
            std::vector<long long> output(2, 0);
            
            std::cout << "Testing function call..." << std::endl;
            if (module.Execute("test", input, output)) {
                std::cout << "[OK] Function execution succeeded" << std::endl;
            } else {
                std::cout << "[INFO] Function execution failed (expected)" << std::endl;
                std::cout << "       Error: " << module.GetLastErrorString() << std::endl;
            }
            
            module.Close();
            return;
        } else {
            std::cout << "[FAIL] Failed to load from this path" << std::endl;
            std::cout << "       Error: " << module.GetLastErrorString() << std::endl;
        }
    }
    
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "  PawnIO Diagnostic Tool" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    CheckDriverStatus();
    CheckBinFile();
    TestModuleLoad();
    
    std::cout << "Diagnostic completed." << std::endl;
    std::cout << "Press any key to exit..." << std::endl;
    _getwch();
    
    return 0;
}
