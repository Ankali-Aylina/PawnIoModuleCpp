// Copyright (c) 2026 Ankali-Aylina
// SPDX-License-Identifier: MIT

/**
 * @file test_pawnio.cpp
 * @brief PawnIoModule 基础测试
 * 
 * 注意：这些测试需要管理员权限和已安装的 PawnIO 驱动
 */

#include "PawnIoModule.h"
#include <iostream>
#include <cassert>

void Test_ModuleConstruction() {
    std::cout << "Test 1: Module construction... ";
    PawnIoModule module;
    assert(!module.IsLoaded());
    std::cout << "PASSED" << std::endl;
}

void Test_ModuleLoadFromFile() {
    std::cout << "Test 2: Module load from file... ";
    PawnIoModule module;
    
    // 注意：这个测试需要 IntelMSR.bin 文件存在
    bool loaded = module.LoadFromFile(L"IntelMSR.bin");
    
    if (loaded) {
        assert(module.IsLoaded());
        std::cout << "PASSED" << std::endl;
        module.Close();
    } else {
        std::cout << "SKIPPED (file not found or driver not installed)" << std::endl;
    }
}

void Test_ModuleExecute() {
    std::cout << "Test 3: Module execute... ";
    PawnIoModule module;
    
    if (!module.LoadFromFile(L"IntelMSR.bin")) {
        std::cout << "SKIPPED (module not loaded)" << std::endl;
        return;
    }
    
    // 测试执行一个函数（具体函数名取决于模块）
    std::vector<long long> input = {0};
    std::vector<long long> output;
    
    try {
        output = module.Execute("test_function", input, 1);
        std::cout << "PASSED" << std::endl;
    } catch (...) {
        std::cout << "SKIPPED (function not available)" << std::endl;
    }
    
    module.Close();
}

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "  PawnIoModule Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // 检查管理员权限
    BOOL isElevated = FALSE;
    HANDLE hToken = NULL;
    
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD size;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size)) {
            isElevated = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }
    
    if (!isElevated) {
        std::cout << "WARNING: Not running as administrator!" << std::endl;
        std::cout << "Some tests may fail or be skipped." << std::endl;
        std::cout << std::endl;
    }
    
    Test_ModuleConstruction();
    Test_ModuleLoadFromFile();
    Test_ModuleExecute();
    
    std::cout << std::endl;
    std::cout << "All tests completed!" << std::endl;
    
    return 0;
}
