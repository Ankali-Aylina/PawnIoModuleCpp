// Copyright (c) 2026 Ankali-Aylina
// SPDX-License-Identifier: MIT

#include "IntelMsrReader.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <windows.h>
#include <csignal>
#include <string>

// 辅助函数：将 wstring 转换为 string
std::string ModulePathToString(const std::wstring& wstr) {
    std::string result;
    for (wchar_t wc : wstr) {
        result.push_back(static_cast<char>(wc));
    }
    return result;
}

// MSR 寄存器地址
constexpr uint32_t IA32_THERM_STATUS_MSR = 0x019C;
constexpr uint32_t IA32_PACKAGE_THERM_STATUS = 0x01B1;  // 封装级温度传感器
constexpr uint32_t IA32_TEMPERATURE_TARGET = 0x01A2;

// 用于优雅关闭的全局标志
volatile bool g_running = true;

void signalHandler(int signum)
{
    std::cout << "\nInterrupt received, exiting..." << std::endl;
    g_running = false;
}

/**
 * @brief 读取 TjMax (最大结温)
 */
bool ReadTjMax(IntelMsrReader& reader, float& tjMax)
{
    tjMax = 100.0f; // 默认值
    
    uint64_t value;
    if (!reader.ReadMsr(IA32_TEMPERATURE_TARGET, value)) {
        std::cerr << "Failed to read IA32_TEMPERATURE_TARGET" << std::endl;
        return false;
    }
    
    // 调试：显示原始 MSR 值
    std::cout << "[DEBUG] IA32_TEMPERATURE_TARGET (0x1A2) = 0x" << std::hex << value << std::dec << std::endl;
    
    // TjMax 在 bits 23:16 中
    tjMax = static_cast<float>((value >> 16) & 0xFF);
    std::cout << "[DEBUG] TjMax extracted: " << tjMax << " C" << std::endl;
    return true;
}

/**
 * @brief 读取核心温度并返回值
 */
float ReadCoreTemperature(IntelMsrReader& reader, float tjMax)
{
    uint64_t value;
    
    if (!reader.ReadMsr(IA32_THERM_STATUS_MSR, value)) {
        std::cerr << "[DEBUG] Failed to read MSR 0x19C" << std::endl;
        return -1.0f;
    }
    
    // 调试输出：显示原始值
    std::cout << "[DEBUG] MSR 0x19C = 0x" << std::hex << value << std::dec << std::endl;
    
    // 检查读取是否有效（bit 31）
    if ((value & 0x80000000ULL) == 0)
    {
        std::cerr << "[DEBUG] Reading invalid (bit 31 = 0)" << std::endl;
        return -1.0f;
    }

    // 提取 DeltaT（bits 22:16）- 与核心温度格式相同
    uint64_t deltaTMask = 0x007F0000ULL; // Bits 22:16（7 位）
    float deltaT = static_cast<float>((value & deltaTMask) >> 16);

    std::cout << "[DEBUG] DeltaT = " << deltaT << ", TjMax = " << tjMax << std::endl;
    
    // 计算实际温度：TjMax - DeltaT
    float temperature = tjMax - deltaT;
    std::cout << "[DEBUG] Temperature = " << temperature << std::endl;

    return temperature;
}

/**
 * @brief Read CPU package temperature (entire CPU chip)
 * @param reader Intel MSR reader instance
 * @param tjMax Maximum junction temperature
 * @return Package temperature in Celsius, or -1.0f on failure
 */
float ReadPackageTemperature(IntelMsrReader& reader, float tjMax) {
    uint64_t value;
    
    if (!reader.ReadMsr(IA32_PACKAGE_THERM_STATUS, value)) {
        std::cerr << "[DEBUG] Failed to read MSR 0x1B1 (Package)" << std::endl;
        return -1.0f;
    }
    
    // 调试输出：显示封装 MSR 原始值
    std::cout << "[DEBUG] MSR 0x1B1 (Package) = 0x" << std::hex << value << std::dec << std::endl;
    
    // 检查读取是否有效（bit 31）
    if ((value & 0x80000000ULL) == 0)
    {
        std::cerr << "[DEBUG] Package reading invalid (bit 31 = 0)" << std::endl;
        return -1.0f;
    }

    // Extract DeltaT (bits 22:16) - same format as core temperature
    uint64_t deltaTMask = 0x007F0000ULL; // Bits 22:16 (7 bits)
    float deltaT = static_cast<float>((value & deltaTMask) >> 16);

    std::cout << "[DEBUG] Package DeltaT = " << deltaT << ", TjMax = " << tjMax << std::endl;

    // Calculate actual temperature: TjMax - DeltaT
    float temperature = tjMax - deltaT;
    std::cout << "[DEBUG] Package Temperature = " << temperature << std::endl;

    return temperature;
}

/**
 * @brief 读取并显示温度统计信息
 */
void DisplayTemperature(IntelMsrReader &reader, float tjMax, int iteration)
{
    // 读取核心和封装温度
    float coreTemp = ReadCoreTemperature(reader, tjMax);
    float packageTemp = ReadPackageTemperature(reader, tjMax);

    if (coreTemp > 0 || packageTemp > 0)
    {
        // 优先使用封装温度，如果不可用则使用核心温度
        float displayTemp = (packageTemp > 0) ? packageTemp : coreTemp;

        // 计算华氏温度
        float tempF = (displayTemp * 9.0f / 5.0f) + 32.0f;

        // 根据较高温度确定状态图标
        float maxTemp = std::max(coreTemp, packageTemp);
        const char *statusIcon = "[OK]";
        const char *statusLevel = "NORMAL";

        if (maxTemp >= 90.0f)
        {
            statusIcon = "[!!]";
            statusLevel = "CRITICAL";
        }
        else if (maxTemp >= 80.0f)
        {
            statusIcon = "[!]";
            statusLevel = "HIGH";
        }

        std::cout << "[" << std::setw(2) << iteration << "] "
                  << statusIcon << " Package: "
                  << std::fixed << std::setprecision(1)
                  << std::setw(5) << displayTemp << " C"
                  << " (" << std::setw(5) << tempF << " F)"
                  << " - " << statusLevel;

        // 显示当前读数，包含两个温度
        if (coreTemp > 0 && packageTemp > 0)
        {
            std::cout << " | Core: " << std::setw(5) << coreTemp << " C";
        }

        std::cout << std::endl;
    }
    else
    {
        std::cout << "[" << std::setw(2) << iteration++ << "] "
                  << "[X] Failed to read temperature" << std::endl;
    }
}

/**
 * @brief 打印程序头部和系统信息
 */
void PrintHeader()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Intel CPU Temperature Reader" << std::endl;
    std::cout << "  Using PawnIO v2 Driver" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
}

/**
 * @brief 从注册表获取 PawnIO 驱动版本
 */
std::wstring GetPawnIoVersion()
{
    HKEY hKey;
    wchar_t version[64] = L"Unknown";

    // 优先尝试 64 位注册表视图
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                      L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PawnIO",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS ||
        RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                      L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PawnIO",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD size = sizeof(version);
        if (RegQueryValueExW(hKey, L"DisplayVersion", NULL, NULL, (LPBYTE)version, &size) != ERROR_SUCCESS)
        {
            wcscpy_s(version, L"Not found");
        }
        RegCloseKey(hKey);
    }
    else
    {
        return L"Not installed";
    }

    return std::wstring(version);
}

/**
 * @brief 检查是否以管理员权限运行
 */
bool IsRunningAsAdmin()
{
    BOOL isElevated = FALSE;
    HANDLE hToken = NULL;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        TOKEN_ELEVATION elevation;
        DWORD size;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size))
        {
            isElevated = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }

    return isElevated;
}

/**
 * @brief 打印系统状态信息
 */
void PrintSystemStatus()
{
    // 检查管理员权限
    std::cout << "System Status:" << std::endl;
    if (IsRunningAsAdmin())
    {
        std::cout << "  [OK] Running as Administrator" << std::endl;
    }
    else
    {
        std::cout << "  [X] NOT running as Administrator!" << std::endl;
        std::cout << "      Please restart with elevated privileges." << std::endl;
    }

    // 检查 PawnIO 版本
    std::wstring version = GetPawnIoVersion();
    // 将宽字符串转换为窄字符串以便输出
    std::string versionStr(version.begin(), version.end());
    std::cout << "  [i] PawnIO Driver Version: " << versionStr << std::endl;

    // 检查 IntelMSR.bin
    if (GetFileAttributesW(L"IntelMSR.bin") != INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "  [OK] IntelMSR.bin found" << std::endl;
    }
    else
    {
        std::cout << "  [X] IntelMSR.bin NOT found!" << std::endl;
    }

    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    // 设置 Ctrl+C 信号处理器
    signal(SIGINT, signalHandler);

    PrintHeader();

    // 打印系统状态
    PrintSystemStatus();

    // 检查是否以管理员身份运行
    if (!IsRunningAsAdmin())
    {
        std::wcerr << L"ERROR: This program requires administrator privileges!" << std::endl;
        std::wcerr << L"Please right-click and select 'Run as Administrator'." << std::endl;
        std::wcerr << std::endl;
        std::wcerr << L"Press any key to exit..." << std::endl;
        _getwch();
        return 1;
    }

    // 初始化 MSR 读取器
    IntelMsrReader reader;
    std::wstring modulePath = L"IntelMSR.bin";

    std::cout << "Loading module: " << ModulePathToString(modulePath) << std::endl;

    if (!reader.Initialize(modulePath))
    {
        std::cerr << "\n[ERROR] Initialization failed!" << std::endl;
        std::cerr << "\nTroubleshooting steps:" << std::endl;
        std::cerr << "  1. Verify PawnIO driver is installed: sc query PawnIO" << std::endl;
        std::cerr << "  2. Reinstall driver if needed using PawnIO_setup.exe" << std::endl;
        std::cerr << "  3. Ensure IntelMSR.bin exists in current directory" << std::endl;
        std::cerr << "  4. Run this program as Administrator" << std::endl;
        std::cerr << "\nPress any key to exit..." << std::endl;
        _getwch();
        return 1;
    }

    std::cout << "[OK] Module loaded successfully!" << std::endl;
    std::cout << std::endl;

    // 读取 TjMax
    float tjMax;
    std::cout << "Reading CPU information..." << std::endl;

    if (!ReadTjMax(reader, tjMax))
    {
        std::cerr << "[ERROR] Failed to read TjMax (CPU thermal threshold)" << std::endl;
        std::cerr << "Your CPU may not support the required MSR registers." << std::endl;
        std::cerr << "\nPress any key to exit..." << std::endl;
        _getwch();
        return 1;
    }

    std::cout << "[OK] CPU detected - TjMax: " << std::fixed << std::setprecision(1)
              << tjMax << " C" << std::endl;
    std::cout << std::endl;

    // 开始温度监控
    std::cout << "========================================" << std::endl;
    std::cout << "  Starting Temperature Monitoring" << std::endl;
    std::cout << "  (Press Ctrl+C to stop)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    int iteration = 1;
    float minTemp = 999.0f, maxTemp = 0.0f, totalTemp = 0.0f;
    int validReadings = 0;

    while (g_running)
    {
        // 读取两个温度用于统计
        float displayTemp = ReadPackageTemperature(reader, tjMax);
        float coreTemp = ReadCoreTemperature(reader, tjMax);

        // 优先使用封装温度（如果可用）
        float useForStats = (displayTemp > 0) ? displayTemp : coreTemp;

        if (useForStats > 0)
        {
            // 更新统计信息
            if (useForStats < minTemp)
                minTemp = useForStats;
            if (useForStats > maxTemp)
                maxTemp = useForStats;
            totalTemp += useForStats;
            validReadings++;

            // 显示当前读数，同时显示两个温度
            DisplayTemperature(reader, tjMax, iteration++);
        }
        else
        {
            std::cout << "[" << std::setw(2) << iteration++ << "] "
                      << "[X] Read error" << std::endl;
        }

        // 等待 1 秒后进行下一次读取
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 打印汇总统计信息
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Monitoring Summary" << std::endl;
    std::cout << "========================================" << std::endl;

    if (validReadings > 0)
    {
        float avgTemp = totalTemp / validReadings;
        std::cout << "Total readings: " << validReadings << std::endl;
        std::cout << "Minimum temp:   " << std::fixed << std::setprecision(1)
                  << minTemp << " C" << std::endl;
        std::cout << "Maximum temp:   " << std::fixed << std::setprecision(1)
                  << maxTemp << " C" << std::endl;
        std::cout << "Average temp:   " << std::fixed << std::setprecision(1)
                  << avgTemp << " C" << std::endl;
    }
    else
    {
        std::cout << "No successful temperature readings." << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Done! Press any key to exit..." << std::endl;
    _getwch();

    return 0;
}