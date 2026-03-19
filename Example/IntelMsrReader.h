// Copyright (c) 2026 Ankali-Aylina
// SPDX-License-Identifier: MIT

#pragma once

#include "PawnIoModule.h"
#include <cstdint>

/**
 * @brief Intel MSR 读取器
 *
 * 使用 PawnIo IntelMSR 模块读取 Intel CPU 的 MSR 寄存器
 */
class IntelMsrReader
{
public:
    IntelMsrReader();
    ~IntelMsrReader();

    /**
     * @brief 初始化读取器
     * @param modulePath IntelMSR.bin 文件路径
     * @return 成功返回 true，失败返回 false
     */
    bool Initialize(const std::wstring &modulePath);

    /**
     * @brief 读取 MSR 寄存器（64 位）
     * @param msrIndex MSR 寄存器索引
     * @param value 输出值
     * @return 成功返回 true，失败返回 false
     */
    bool ReadMsr(uint32_t msrIndex, uint64_t &value);

    /**
     * @brief 读取 MSR 寄存器（拆分为 EAX 和 EDX）
     * @param msrIndex MSR 寄存器索引
     * @param eax 输出 EAX 值
     * @param edx 输出 EDX 值
     * @return 成功返回 true，失败返回 false
     */
    bool ReadMsr(uint32_t msrIndex, uint32_t &eax, uint32_t &edx);

    /**
     * @brief 关闭读取器
     */
    void Close();

    /**
     * @brief 检查是否已初始化
     * @return 已初始化返回 true，否则返回 false
     */
    bool IsInitialized() const;

private:
    PawnIoModule _pawnIo;
    bool _initialized;
};