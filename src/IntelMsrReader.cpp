// Copyright (c) 2026 Ankali-Aylina
// SPDX-License-Identifier: MIT

#include "IntelMsrReader.h"
#include <iostream>

IntelMsrReader::IntelMsrReader()
    : _initialized(false)
{
}

IntelMsrReader::~IntelMsrReader()
{
    Close();
}

bool IntelMsrReader::Initialize(const std::wstring &modulePath)
{
    if (_pawnIo.LoadFromFile(modulePath))
    {
        _initialized = true;
        std::cout << "IntelMsrReader initialized successfully" << std::endl;
        return true;
    }

    std::cerr << "IntelMsrReader initialization failed" << std::endl;
    return false;
}

bool IntelMsrReader::ReadMsr(uint32_t msrIndex, uint64_t &value)
{
    if (!_initialized)
    {
        return false;
    }

    // 准备输入参数 - 只传递 MSR 索引
    std::vector<long long> input = {static_cast<long long>(msrIndex)};

    // 准备输出参数 - 只需要一个元素
    std::vector<long long> output(1);

    // 执行 ioctl_read_msr
    if (!_pawnIo.Execute("ioctl_read_msr", input, output))
    {
        std::cerr << "Failed to read MSR 0x" << std::hex << msrIndex << std::dec << std::endl;
        return false;
    }

    value = static_cast<uint64_t>(output[0]);
    return true;
}

bool IntelMsrReader::ReadMsr(uint32_t msrIndex, uint32_t &eax, uint32_t &edx)
{
    uint64_t value;
    if (!ReadMsr(msrIndex, value))
    {
        return false;
    }

    // 拆分为 EAX 和 EDX
    eax = static_cast<uint32_t>(value & 0xFFFFFFFF);
    edx = static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF);
    return true;
}

void IntelMsrReader::Close()
{
    _pawnIo.Close();
    _initialized = false;
}

bool IntelMsrReader::IsInitialized() const
{
    return _initialized;
}
