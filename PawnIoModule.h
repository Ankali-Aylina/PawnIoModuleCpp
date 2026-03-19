// Copyright (c) 2026 Ankali-Aylina
// SPDX-License-Identifier: MIT

#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// PawnIo 设备路径
#define PAWNIO_DEVICE L"\\\\?\\GLOBALROOT\\Device\\PawnIO"

// Windows DDK 常量
#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED 0
#endif

#ifndef FILE_ANY_ACCESS
#define FILE_ANY_ACCESS 0
#endif

#ifndef FILE_READ_ACCESS
#define FILE_READ_ACCESS 1
#endif

#ifndef FILE_WRITE_ACCESS
#define FILE_WRITE_ACCESS 2
#endif

// CTL_CODE 宏定义
#define CTL_CODE(DeviceType, Function, Method, Access) \
    (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

// ========================================
// PawnIO IOCTL 控制码（基于 LibreHardwareMonitor）
// ========================================
// 注意：设备类型值是 41394 = 0xA1B2
#define PAWNIO_DEVICE_TYPE 41394u // 0xA1B2
#define FUNCTION_LOAD_BINARY 0x821
#define FUNCTION_EXECUTE_FN 0x841

// IOCTL 控制码
#define IOCTL_PIO_LOAD_BINARY_CODE ((PAWNIO_DEVICE_TYPE << 16) | (FUNCTION_LOAD_BINARY << 2))
#define IOCTL_PIO_EXECUTE_FN_CODE ((PAWNIO_DEVICE_TYPE << 16) | (FUNCTION_EXECUTE_FN << 2))

// 函数名长度
#define FN_NAME_LENGTH 32

/**
 * @brief PawnIo 模块封装类
 *
 * 用于加载和管理 PawnIo 二进制模块，提供执行模块函数的接口
 */
class PawnIoModule
{
public:
    // 构造函数和析构函数
    PawnIoModule();
    ~PawnIoModule();

    // 禁止拷贝
    PawnIoModule(const PawnIoModule &) = delete;
    PawnIoModule &operator=(const PawnIoModule &) = delete;

    /**
     * @brief 从文件加载模块
     * @param binFilePath .bin 文件路径
     * @return 成功返回 true，失败返回 false
     */
    bool LoadFromFile(const std::wstring &binFilePath);

    /**
     * @brief 从内存加载模块
     * @param binData 二进制数据
     * @param dataSize 数据大小
     * @return 成功返回 true，失败返回 false
     */
    bool LoadFromMemory(const void *binData, size_t dataSize);

    /**
     * @brief 执行模块函数
     * @param functionName 函数名
     * @param input 输入参数数组
     * @param output 输出参数数组（会被填充）
     * @return 成功返回 true，失败返回 false
     */
    bool Execute(const std::string &functionName,
                 const std::vector<long long> &input,
                 std::vector<long long> &output);

    /**
     * @brief 执行模块函数（带返回值）
     * @param functionName 函数名
     * @param input 输入参数数组
     * @param outLength 期望的输出长度
     * @return 输出参数数组
     */
    std::vector<long long> Execute(const std::string &functionName,
                                   const std::vector<long long> &input,
                                   size_t outLength);

    /**
     * @brief 检查模块是否已加载
     * @return 已加载返回 true，否则返回 false
     */
    bool IsLoaded() const;

    /**
     * @brief 关闭模块
     */
    void Close();

    /**
     * @brief 获取最后的错误信息
     * @return 错误描述字符串
     */
    std::string GetLastErrorString() const;

private:
    HANDLE _handle;
    DWORD _lastError;

    /**
     * @brief 打开 PawnIo 设备
     * @return 成功返回 true，失败返回 false
     */
    bool OpenDevice();

    /**
     * @brief 读取二进制文件
     * @param filePath 文件路径
     * @return 文件内容
     */
    std::vector<char> ReadBinaryFile(const std::wstring &filePath);
};