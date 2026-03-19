// Copyright (c) 2026 Ankali-Aylina
// SPDX-License-Identifier: MIT

#include "PawnIoModule.h"
#include <codecvt>
#include <stdexcept>
#include <sstream>
#include <cstdint>
#include <inttypes.h>

// 全局常量定义（文件作用域）
namespace
{
    constexpr size_t MAX_MODULE_SIZE = 10 * 1024 * 1024; // 10MB 限制
    constexpr size_t MAX_INPUT_PARAMS = 1024;            // 最大输入参数数量
}

PawnIoModule::PawnIoModule()
    : _handle(INVALID_HANDLE_VALUE), _lastError(0)
{
}

PawnIoModule::~PawnIoModule()
{
}

bool PawnIoModule::OpenDevice()
{
    if (_handle != INVALID_HANDLE_VALUE)
    {
        return true; // 已打开
    }

    try
    {
        // 打开 PawnIo 设备
        _handle = CreateFileW(
            PAWNIO_DEVICE,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (_handle == INVALID_HANDLE_VALUE)
        {
            _lastError = GetLastError();
            std::ostringstream oss;
            oss << "Failed to open PawnIo device, error code: " << _lastError;
            throw std::runtime_error(oss.str());
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] OpenDevice exception: " << e.what() << std::endl;
        _handle = INVALID_HANDLE_VALUE;
        return false;
    }
}

std::vector<char> PawnIoModule::ReadBinaryFile(const std::wstring &filePath)
{
    std::vector<char> buffer;

    try
    {
        // 验证路径不为空
        if (filePath.empty())
        {
            _lastError = ERROR_INVALID_PARAMETER;
            throw std::invalid_argument("File path is empty");
        }

        // 使用 Windows API 读取文件（支持宽字符路径）
        HANDLE hFile = CreateFileW(
            filePath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            _lastError = GetLastError();
            std::ostringstream oss;
            oss << "Failed to open file, error code: " << _lastError;
            throw std::runtime_error(oss.str());
        }

        // 获取并验证文件大小
        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(hFile, &fileSize))
        {
            _lastError = GetLastError();
            CloseHandle(hFile);
            throw std::runtime_error("Failed to get file size");
        }

        // 限制最大文件大小（防止内存耗尽攻击）
        constexpr uint64_t MAX_FILE_SIZE = static_cast<uint64_t>(MAX_MODULE_SIZE);
        if (fileSize.QuadPart <= 0 || static_cast<uint64_t>(fileSize.QuadPart) > MAX_FILE_SIZE)
        {
            _lastError = (static_cast<uint64_t>(fileSize.QuadPart) > MAX_FILE_SIZE)
                             ? ERROR_FILE_TOO_LARGE
                             : ERROR_FILE_CORRUPT;
            CloseHandle(hFile);

            std::ostringstream oss;
            oss << "Invalid file size: " << fileSize.QuadPart
                << " bytes (max: " << MAX_MODULE_SIZE << " bytes)";
            throw std::length_error(oss.str());
        }

        // 转换为 size_t 并检查溢出
        if (fileSize.QuadPart < 0 || static_cast<uint64_t>(fileSize.QuadPart) > static_cast<uint64_t>(SIZE_MAX))
        {
            _lastError = ERROR_ARITHMETIC_OVERFLOW;
            CloseHandle(hFile);
            throw std::overflow_error("File size too large for size_t");
        }

        // 调整缓冲区大小（带异常处理）
        try
        {
            buffer.resize(static_cast<size_t>(fileSize.QuadPart));
        }
        catch (const std::bad_alloc &)
        {
            _lastError = ERROR_NOT_ENOUGH_MEMORY;
            CloseHandle(hFile);
            throw std::bad_alloc();
        }

        // 读取文件内容
        DWORD bytesRead = 0;
        if (!ReadFile(hFile, buffer.data(), static_cast<DWORD>(fileSize.QuadPart), &bytesRead, NULL))
        {
            _lastError = GetLastError();
            buffer.clear();
            CloseHandle(hFile);

            std::ostringstream oss;
            oss << "Failed to read file, error code: " << _lastError;
            throw std::runtime_error(oss.str());
        }

        // 验证读取的字节数
        if (bytesRead != fileSize.QuadPart)
        {
            buffer.clear();
            _lastError = ERROR_HANDLE_EOF;
            CloseHandle(hFile);
            throw std::runtime_error("Incomplete file read");
        }

        CloseHandle(hFile);
        return buffer;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] ReadBinaryFile exception: " << e.what() << std::endl;
        return buffer; // 返回空缓冲区
    }
}

bool PawnIoModule::LoadFromFile(const std::wstring &binFilePath)
{
    try
    {
        // 打开设备
        if (!OpenDevice())
        {
            throw std::runtime_error("Failed to open PawnIo device");
        }

        // 读取二进制文件
        std::vector<char> binData = ReadBinaryFile(binFilePath);
        if (binData.empty())
        {
            throw std::runtime_error("Failed to read module file: file is empty or read failed");
        }

        // 从内存加载
        if (!LoadFromMemory(binData.data(), binData.size()))
        {
            throw std::runtime_error("Failed to load module from memory");
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] LoadFromFile exception: " << e.what() << std::endl;
        return false;
    }
}

bool PawnIoModule::LoadFromMemory(const void *binData, size_t dataSize)
{
    try
    {
        // 验证句柄有效性
        if (_handle == INVALID_HANDLE_VALUE)
        {
            _lastError = ERROR_INVALID_HANDLE;
            throw std::runtime_error("Invalid device handle");
        }

        // 验证数据指针和大小
        if (binData == nullptr)
        {
            _lastError = ERROR_INVALID_PARAMETER;
            throw std::invalid_argument("Binary data pointer is null");
        }

        if (dataSize == 0)
        {
            _lastError = ERROR_INVALID_PARAMETER;
            throw std::invalid_argument("Data size cannot be zero");
        }

        // 验证数据大小合理性
        if (dataSize > MAX_MODULE_SIZE)
        {
            _lastError = ERROR_FILE_TOO_LARGE;
            std::ostringstream oss;
            oss << "Data size (" << dataSize << " bytes) exceeds maximum allowed size ("
                << MAX_MODULE_SIZE << " bytes)";
            throw std::length_error(oss.str());
        }

        // 加载模块到驱动
        DWORD bytesReturned;
        BOOL result = DeviceIoControl(
            _handle,
            IOCTL_PIO_LOAD_BINARY_CODE,
            (LPVOID)binData,
            static_cast<DWORD>(dataSize),
            NULL,
            0,
            &bytesReturned,
            NULL);

        if (!result)
        {
            _lastError = GetLastError();
            std::ostringstream oss;
            oss << "Failed to load module, error code: " << _lastError;
            throw std::runtime_error(oss.str());
        }

        std::cout << "[INFO] Module loaded successfully, size: " << dataSize << " bytes" << std::endl;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] LoadFromMemory exception: " << e.what() << std::endl;
        return false;
    }
}

bool PawnIoModule::Execute(const std::string &functionName,
                           const std::vector<long long> &input,
                           std::vector<long long> &output)
{
    try
    {
        // 验证句柄有效性
        if (_handle == INVALID_HANDLE_VALUE)
        {
            _lastError = ERROR_INVALID_HANDLE;
            throw std::runtime_error("Invalid device handle");
        }

        // 验证输入参数
        if (functionName.empty())
        {
            _lastError = ERROR_INVALID_PARAMETER;
            throw std::invalid_argument("Function name is empty");
        }

        if (output.empty())
        {
            _lastError = ERROR_INSUFFICIENT_BUFFER;
            throw std::invalid_argument("Output buffer is empty");
        }

        // 限制最大参数数量（防止 DoS 攻击）
        if (input.size() > MAX_INPUT_PARAMS)
        {
            _lastError = ERROR_INVALID_PARAMETER;
            std::ostringstream oss;
            oss << "Too many input parameters: " << input.size()
                << " (max: " << MAX_INPUT_PARAMS << ")";
            throw std::length_error(oss.str());
        }

        // 防止整数溢出
        if (input.size() > (SIZE_MAX - FN_NAME_LENGTH) / sizeof(long long))
        {
            _lastError = ERROR_ARITHMETIC_OVERFLOW;
            throw std::overflow_error("Input buffer size calculation would overflow");
        }

        // 构建输入缓冲区
        size_t inputBufferSize = FN_NAME_LENGTH + (input.size() * sizeof(long long));
        std::vector<char> inputBuffer(inputBufferSize, 0);

        //  安全复制函数名（确保 null 结尾，固定长度填充）
        std::string safeName = functionName.substr(0, FN_NAME_LENGTH - 1);
        safeName.resize(FN_NAME_LENGTH, '\0'); // 填充到固定长度
        memcpy(inputBuffer.data(), safeName.c_str(), FN_NAME_LENGTH);

        // 复制输入参数
        if (!input.empty())
        {
            memcpy(
                inputBuffer.data() + FN_NAME_LENGTH,
                input.data(),
                input.size() * sizeof(long long));
        }

        // 准备输出缓冲区
        size_t outputSize = output.size() * sizeof(long long);
        std::vector<char> outputBuffer(outputSize, 0);

        // 执行命令
        DWORD bytesReturned;
        BOOL result = DeviceIoControl(
            _handle,
            IOCTL_PIO_EXECUTE_FN_CODE,
            inputBuffer.data(),
            static_cast<DWORD>(inputBufferSize),
            outputBuffer.data(),
            static_cast<DWORD>(outputSize),
            &bytesReturned,
            NULL);

        if (!result)
        {
            _lastError = GetLastError();
            std::ostringstream oss;
            oss << "Failed to execute command, error code: " << _lastError;
            throw std::runtime_error(oss.str());
        }

        //  验证返回数据长度
        if (bytesReturned > 0)
        {
            if (bytesReturned > outputBuffer.size())
            {
                _lastError = ERROR_INVALID_DATA;
                std::ostringstream oss;
                oss << "Driver returned more data than expected: " << bytesReturned
                    << " bytes (buffer size: " << outputBuffer.size() << " bytes)";
                throw std::length_error(oss.str());
            }

            size_t outputCount = bytesReturned / sizeof(long long);
            outputCount = std::min(outputCount, output.size());

            if (outputCount > 0)
            {
                memcpy(output.data(), outputBuffer.data(), outputCount * sizeof(long long));
            }
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] Execute exception: " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "[ERROR] Execute unknown exception" << std::endl;
        _lastError = ERROR_EXCEPTION_IN_SERVICE;
        return false;
    }
}

std::vector<long long> PawnIoModule::Execute(const std::string &functionName,
                                             const std::vector<long long> &input,
                                             size_t outLength)
{
    try
    {
        //  验证输出长度
        if (outLength == 0)
        {
            throw std::invalid_argument("Output length cannot be zero");
        }

        if (outLength > MAX_INPUT_PARAMS)
        {
            throw std::length_error("Output length exceeds maximum allowed");
        }

        std::vector<long long> output(outLength, 0);

        if (!Execute(functionName, input, output))
        {
            // 发生错误时返回全零数组（已在内部处理）
            std::fill(output.begin(), output.end(), 0);
        }

        return output;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] Execute (with return) exception: " << e.what() << std::endl;
        // 返回全零数组
        return std::vector<long long>(outLength, 0);
    }
    catch (...)
    {
        std::cerr << "[ERROR] Execute (with return) unknown exception" << std::endl;
        return std::vector<long long>(outLength, 0);
    }
}

bool PawnIoModule::IsLoaded() const
{
    return _handle != INVALID_HANDLE_VALUE;
}

void PawnIoModule::Close()
{
    try
    {
        if (_handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(_handle);
            _handle = INVALID_HANDLE_VALUE;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] Close exception: " << e.what() << std::endl;
        _handle = INVALID_HANDLE_VALUE; // 强制设置为无效
    }
    catch (...)
    {
        std::cerr << "[ERROR] Close unknown exception" << std::endl;
        _handle = INVALID_HANDLE_VALUE; // 强制设置为无效
    }
}

std::string PawnIoModule::GetLastErrorString() const
{
    if (_lastError == 0)
    {
        return "No error";
    }

    char *messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        _lastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer,
        0,
        NULL);

    std::string message;
    if (messageBuffer != nullptr && size > 0)
    {
        message.assign(messageBuffer, size);
        LocalFree(messageBuffer);

        // 移除末尾的换行符
        while (!message.empty() && (message.back() == '\n' || message.back() == '\r'))
        {
            message.pop_back();
        }
    }
    else
    {
        std::ostringstream oss;
        oss << "Unknown error: " << _lastError;
        message = oss.str();
    }

    return message;
}
