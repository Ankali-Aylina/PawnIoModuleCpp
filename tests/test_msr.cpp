// Copyright (c) 2026 Ankali-Aylina
// SPDX-License-Identifier: MIT

/**
 * @file test_msr.cpp
 * @brief 测试 MSR 读取的简化版本
 */

#include "PawnIoModule.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    std::cout << "=== MSR Read Test ===" << std::endl;
    
    // 1. 加载模块
    PawnIoModule module;
    if (!module.LoadFromFile(L"IntelMSR.bin")) {
        std::cerr << "Failed to load module!" << std::endl;
        return 1;
    }
    
    std::cout << "[OK] Module loaded" << std::endl;
    
    // 2. 尝试直接调用 ioctl_read_msr
    uint32_t msrIndex = 0x1A2;  // IA32_TEMPERATURE_TARGET
    
    std::cout << "Testing MSR read for index: 0x" << std::hex << msrIndex << std::dec << std::endl;
    
    // 方法 1: 单个参数
    std::cout << "\n--- Method 1: Single parameter ---" << std::endl;
    std::vector<long long> input1 = { static_cast<long long>(msrIndex) };
    std::vector<long long> output1(2, 0);
    
    if (module.Execute("ioctl_read_msr", input1, output1)) {
        std::cout << "[OK] Success with single parameter!" << std::endl;
        std::cout << "output[0] = " << output1[0] << std::endl;
        std::cout << "output[1] = " << output1[1] << std::endl;
    } else {
        std::cout << "[FAIL] Failed with single parameter" << std::endl;
        std::cout << "Error: " << module.GetLastErrorString() << std::endl;
    }
    
    // 方法 2: 两个参数
    std::cout << "\n--- Method 2: Two parameters ---" << std::endl;
    std::vector<long long> input2 = { static_cast<long long>(msrIndex), 0 };
    std::vector<long long> output2(2, 0);
    
    if (module.Execute("ioctl_read_msr", input2, output2)) {
        std::cout << "[OK] Success with two parameters!" << std::endl;
        std::cout << "output[0] = " << output2[0] << std::endl;
        std::cout << "output[1] = " << output2[1] << std::endl;
    } else {
        std::cout << "[FAIL] Failed with two parameters" << std::endl;
        std::cout << "Error: " << module.GetLastErrorString() << std::endl;
    }
    
    // 方法 3: 使用不同的函数名
    std::cout << "\n--- Method 3: Different function name ---" << std::endl;
    std::vector<long long> input3 = { static_cast<long long>(msrIndex) };
    std::vector<long long> output3(2, 0);
    
    if (module.Execute("read_msr", input3, output3)) {
        std::cout << "[OK] Success with 'read_msr' function!" << std::endl;
        std::cout << "output[0] = " << output3[0] << std::endl;
        std::cout << "output[1] = " << output3[1] << std::endl;
    } else {
        std::cout << "[FAIL] Failed with 'read_msr' function" << std::endl;
        std::cout << "Error: " << module.GetLastErrorString() << std::endl;
    }
    
    module.Close();
    
    std::cout << "\nTest completed." << std::endl;
    return 0;
}
