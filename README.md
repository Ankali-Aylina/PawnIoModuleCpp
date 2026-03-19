# PawnIoModuleCpp - Intel CPU 温度读取模块

[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-orange.svg)](https://www.mozilla.org/en-US/MPL/2.0/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE-MIT.txt)
[![License: GPL-2.0](https://img.shields.io/badge/License-GPL--2.0-blue.svg)](LICENSE-GPL-2.0.txt)

---

<div align="center">

**🌍 选择语言 / Choose Language:**

[**🇨🇳 简体中文**](README.md) | [**🌐 English**](README_en.md)

</div>

---

## 📖 项目简介

**PawnIoModuleCpp** 是一个基于 C++20 的系统级温度读取模块，通过访问 Intel MSR（Model Specific Registers）实现硬件层面的 CPU 温度监控。该项目使用 PawnIO v2 驱动程序和 IntelMSR 模块，为开发者和硬件监控工具提供精确的 CPU 温度数据。

### 💡 项目来源

本项目的核心二进制组件 **IntelMSR.bin** 提取自开源硬件监控工具 **[LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor)** 项目。参考 LibreHardwareMonitor 中成熟的 MSR 寄存器访问方案和温度计算算法，使用 C++20 实现了与 IntelMSR.bin 模块的完整交互逻辑。

**主要改进：**
- 🎯 **模块化封装** - 将温度读取逻辑从庞大的监控系统中独立出来
- 🚀 **简化依赖** - 移除不必要的硬件支持，专注于 CPU 温度监控
- 📦 **易于集成** - 提供清晰的 C++ API 接口，方便其他项目调用
- 🔧 **跨平台构建** - 基于 CMake，支持多种开发环境
- 💻 **原生 C++ 实现** - 相比 C# 版本性能更优，更适合系统集成

### 核心特性

- 🔍 **实时温度监控** - 读取 CPU 核心温度和封装温度
- 🚀 **高性能** - 直接访问 MSR 寄存器，低延迟
- 🛡️ **模块化设计** - 清晰的代码结构，易于集成
- 📦 **双模式构建** - 支持 Debug 和 Release 配置
- 💻 **Windows 平台** - 专为 Windows 系统优化

## 🏗️ 系统架构

```
┌─────────────────┐
│   main.cpp      │  ← 示例程序（温度监控）
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ IntelMsrReader  │  ← MSR 读取器封装
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ PawnIoModule    │  ← PawnIO 驱动接口
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ IntelMSR.bin    │  ← Intel MSR 驱动模块
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ PawnIO.sys      │  ← 内核驱动
└────────┬────────┘
```

## 📋 依赖项

### 必需依赖

- **操作系统**: Windows 10/11（64 位）
- **编译器**: 支持 C++20 的编译器（MSVC 2019+, GCC 10+, Clang 12+）
- **构建系统**: CMake 3.10 或更高版本
- **驱动程序**: [PawnIO v2 Driver](https://github.com/namazso/PawnIO)
- **驱动模块**: ``（该驱动模块是从）

### 可选工具

- **Ninja** - 快速构建工具
- **CPack** - 安装包生成工具

## 🔧 安装与配置

### 1. 安装 PawnIO 驱动

在运行此项目之前，必须先安装 PawnIO 驱动程序：

```powershell
# 下载并安装 PawnIO 驱动
# 访问 https://github.com/namazso/PawnIO 获取安装包
```

**注意**: 安装驱动需要管理员权限。

### 2. 准备 IntelMSR.bin

`IntelMSR.bin` 由 [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) 项目中获取。
确保 `IntelMSR.bin` 文件位于以下路径之一：
- 可执行文件所在目录
- Windows 系统目录（`C:\Windows\System32`）
- PawnIO 驱动安装目录

### 3. 克隆项目

```bash
git clone <repository-url>
cd PawnIoModuleCpp
```

## 🏃 快速开始

### 构建项目

```bash
# 进入项目根目录
cd PawnIoModuleCpp

# 创建构建目录
mkdir build && cd build

# 配置 CMake（Release 模式）
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release

# 编译项目
cmake --build . --config Release
```

### 运行示例程序

**重要**: 程序需要管理员权限才能访问硬件寄存器！

```bash
# 以管理员身份运行
# 在 PowerShell 或 CMD 中（右键 → 以管理员身份运行）
cd build\Release
.\cpu_temp.exe
```

### 预期输出

```
========================================
  Intel CPU Temperature Reader
  Using PawnIO v2 Driver
========================================

System Status:
  [OK] Running as Administrator
  [i] PawnIO Driver Version: 2.0.0
  [OK] IntelMSR.bin found

Loading module: IntelMSR.bin
[OK] Module loaded successfully!

Reading CPU information...
[OK] CPU detected - TjMax: 100.0 C

========================================
  Starting Temperature Monitoring
  (Press Ctrl+C to stop)
========================================

[ 1] [OK] Package:  45.0 C ( 113.0 F) - NORMAL
[ 2] [OK] Package:  46.0 C ( 114.8 F) - NORMAL
[ 3] [OK] Package:  47.0 C ( 116.6 F) - NORMAL
...
```

## 💻 使用方法

### 基本 API

#### 初始化 MSR 读取器

```cpp
#include "IntelMsrReader.h"

IntelMsrReader reader;
std::wstring modulePath = L"IntelMSR.bin";

if (!reader.Initialize(modulePath)) {
    std::cerr << "Initialization failed!" << std::endl;
    return 1;
}
```

#### 读取 MSR 寄存器

```cpp
uint64_t value;
// 方法 1: 读取 64 位值
if (reader.ReadMsr(0x19C, value)) {
    std::cout << "MSR value: 0x" << std::hex << value << std::endl;
}

// 方法 2: 分别读取 EAX 和 EDX
uint32_t eax, edx;
if (reader.ReadMsr(0x19C, eax, edx)) {
    std::cout << "EAX: 0x" << std::hex << eax 
              << ", EDX: 0x" << edx << std::endl;
}
```

#### 温度读取示例

```
// 读取 TjMax（最大结温）
float tjMax;
ReadTjMax(reader, tjMax);

// 读取核心温度
float coreTemp = ReadCoreTemperature(reader, tjMax);

// 读取封装温度（整个 CPU）
float packageTemp = ReadPackageTemperature(reader, tjMax);

std::cout << "CPU Temperature: " << packageTemp << "°C" << std::endl;
```

### 完整示例

参考 [`Example/main.cpp`](Example/main.cpp) 查看完整的温度监控程序。

## 📁 项目结构

```
PawnIoModuleCpp/
├── PawnIoModule.h          # PawnIO 模块接口定义
├── PawnIoModule.cpp        # PawnIO 模块实现
├── Example/
│   ├── IntelMsrReader.h    # MSR 读取器头文件
│   ├── IntelMsrReader.cpp  # MSR 读取器实现
│   └── main.cpp            # 示例程序（温度监控）
├── CMakeLists.txt          # CMake 构建配置
└── README.md               # 本文件
```

## 🔬 技术细节

### MSR 寄存器地址

项目使用的关键 MSR 寄存器：

| 寄存器名称 | 地址 | 描述 |
|-----------|------|------|
| `IA32_THERM_STATUS_MSR` | 0x019C | 核心热状态寄存器 |
| `IA32_PACKAGE_THERM_STATUS` | 0x01B1 | 封装热状态寄存器 |
| `IA32_TEMPERATURE_TARGET` | 0x01A2 | 目标温度寄存器（包含 TjMax） |

### 温度计算原理

根据 Intel SDM（Software Developer's Manual）：

```
实际温度 = TjMax - DeltaT

其中:
- TjMax: CPU 最大结温（从 IA32_TEMPERATURE_TARGET 的 bits 23:16 读取）
- DeltaT: 当前温度偏移量（从 IA32_THERM_STATUS_MSR 的 bits 22:16 读取）
```

### 安全注意事项

⚠️ **警告**: 
- 访问 MSR 寄存器需要**管理员权限**
- 错误的 MSR 操作可能导致系统不稳定
- 本项目仅用于学习和监控目的
- 生产环境使用请充分测试

## 🛠️ 故障排除

### 常见问题

#### 1. "Initialization failed" 错误

**原因**: PawnIO 驱动未安装或 IntelMSR.bin 找不到

**解决方案**:
```powershell
# 检查驱动状态
sc query PawnIO

# 如果未安装，重新安装 PawnIO 驱动
# 确保 IntelMSR.bin 在可执行文件目录
```

#### 2. "NOT running as Administrator" 错误

**原因**: 程序未以管理员身份运行

**解决方案**:
- 右键点击可执行文件 → "以管理员身份运行"
- 或在提升的命令行中运行

#### 3. 温度读取失败（返回 -1.0f）

**原因**: 
- CPU 不支持相应的 MSR 寄存器
- 读取时机问题（CPU 处于低功耗状态）

**解决方案**:
- 检查 CPU 兼容性列表
- 多次尝试读取
- 添加适当的延迟

### 调试技巧

启用详细输出：
```cpp
// 在 main.cpp 中已有详细的 debug 输出
// 查看 [DEBUG] 前缀的信息
```

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🙏 致谢

- **LibreHardwareMonitor** - 本项目的核心二进制组件 IntelMSR.bin 提取自 LibreHardwareMonitor 项目，温度读取的核心算法（MSR 寄存器地址定义、热状态计算方法）参考了 LibreHardwareMonitor 的开源实现。
  - 项目地址：https://github.com/LibreHardwareMonitor/LibreHardwareMonitor
  - 许可证：**MPL 2.0** (Mozilla Public License 2.0)
  - ⚠️ **重要声明**: IntelMSR.bin 文件直接来源于 LibreHardwareMonitor 项目，其使用需遵循 LibreHardwareMonitor 的许可条款（MPL 2.0）。
  
- **PawnIO 项目**: https://github.com/namazso/PawnIO - 提供了访问 MSR 寄存器的底层驱动支持
  - 许可证：请查看 PawnIO 项目的具体许可条款

- **Intel® 64 and IA-32 Architectures Software Developer's Manual** - Intel 官方文档提供了 MSR 寄存器的权威定义和使用说明

### 🔗 与 LibreHardwareMonitor 的关系

| 特性 | LibreHardwareMonitor | PawnIoModuleCpp |
|------|---------------------|-----------------|
| **定位** | 全功能硬件监控工具（C#/.NET） | 专注 CPU 温度的轻量级模块（C++20） |
| **语言** | C# (.NET) | C++20 |
| **构建系统** | Visual Studio / MSBuild | CMake |
| **支持硬件** | CPU、GPU、主板、内存等 | 仅 Intel CPU |
| **体积** | ~10MB+ | <100KB |
| **集成难度** | 需引用整个框架 | 简单的 C++ 库调用 |
| **温度算法** | 参考实现 | 基于相同原理实现 |
| **核心模块** | IntelMSR.bin（原始） | IntelMSR.bin（提取自 LHM） |
| **交互逻辑** | C# 实现 |  C++ 重构 |

**技术参考：**
- ✅ **IntelMSR.bin 模块** - 直接从 LibreHardwareMonitor 项目中提取（二进制文件未修改）
- ✅ MSR 寄存器地址定义（`IA32_THERM_STATUS_MSR`, `IA32_PACKAGE_THERM_STATUS`, `IA32_TEMPERATURE_TARGET`）
- ✅ TjMax 提取方法（bits 23:16）
- ✅ DeltaT 计算逻辑（bits 22:16）
- ✅ 温度公式：`Temperature = TjMax - DeltaT`
- ✅ PawnIO 驱动调用流程

**本项目实现：**
- [`PawnIoModule`](PawnIoModule.h/cpp) - PawnIO 驱动交互接口
- [`IntelMsrReader`](Example/IntelMsrReader.h/cpp) - MSR 读取器封装类
- 温度监控示例程序（[`main.cpp`](Example/main.cpp)）
- CMake 构建配置和安装包生成

## 🔄 更新日志

### v1.0.0 (2026-03-19)
- ✨ 初始版本发布
- ✅ 支持 Intel CPU 温度读取
- ✅ 核心温度和封装温度监控
- ✅ 实时温度统计（最小值、最大值、平均值）
- ✅ 完善的错误处理和故障诊断

---

## ⚖️ 许可证合规性说明

### 📜 本项目使用的许可证

**重要提示**: 本项目包含多个第三方组件，每个组件使用不同的许可证：

#### 1. IntelMSR.bin - MPL 2.0 许可证

**来源**: LibreHardwareMonitor 项目  
**许可证**: **MPL 2.0 (Mozilla Public License)**  
**是否修改**: ❌ 否

MPL 2.0 是一个**弱 Copyleft**许可证，主要要求包括：

✅ **允许的使用方式：**
- 商业使用
- 修改代码
- 分发
- 专利使用
- 私有化修改（但仅限于 MPL 覆盖的文件）

⚠️ **必须遵守的义务：**
1. **源代码公开义务**：如果您修改了 IntelMSR.bin 相关的源代码（即 MPL 许可的部分），您必须将这些修改以 MPL 2.0 许可证公开
2. **许可证声明**：必须在文档中明确声明使用了 MPL 许可的代码
3. **许可证文本**：必须包含 MPL 2.0 许可证全文
4. **文件级 Copyleft**：MPL 的要求仅限于 MPL 许可的文件本身，不传染其他文件

**本项目未修改 LibreHardwareMonitor 项目中的 IntelMSR.bin 文件**

#### 2. PawnIO.sys - GPL-2.0 许可证（含特殊例外）

**来源**: PawnIO 项目 (https://github.com/namazso/PawnIO)  
**作者**: namazso <admin@namazso.eu>  
**许可证**: **GPL-2.0 (GNU General Public License version 2)** + 特殊例外条款  
**是否修改**: ❌ 否

GPL-2.0 是一个**强 Copyleft**许可证，具有以下特点：

✅ **允许的使用方式：**
- 商业使用
- 修改和分发
- 专利使用

⚠️ **必须遵守的义务：**
1. **强传染性** - 任何基于 GPL 代码的衍生作品必须整体采用 GPL-2.0 许可
2. **源代码提供** - 必须提供完整的源代码或书面提供承诺
3. **许可证文本** - 必须包含 GPL-2.0 许可证全文
4. **明确声明** - 明确声明修改和日期
5. **无担保责任** - 按"原样"提供，无任何担保

**特殊例外条款**：
PawnIO 的版权持有者授予额外许可，允许您将 PawnIO 与以下组件组合：
- 根据 GNU LGPL 发布的免费软件程序或库
- 仅通过设备 IO 控制接口与 PawnIO 通信的独立模块

这意味着：
- ✅ IntelMSR.bin（MPL 2.0）可以通过此例外与 PawnIO 一起使用
- ✅ 本项目的原创代码（MIT License）可以通过此例外与 PawnIO 一起使用
- ⚠️ 但如果直接与 PawnIO 通过 Pawn 接口通信，则必须兼容 GPL-2.0

**运行时依赖**：
PawnIO 驱动是运行时依赖，需用户自行下载安装，不包含在本项目中。

#### 3. 本项目原创代码 - MIT License

**范围**: 
- [`PawnIoModule.cpp`](PawnIoModule.cpp) - PawnIO 驱动交互接口实现
- [`PawnIoModule.h`](PawnIoModule.h) - PawnIO 驱动交互接口定义
- [`Example/IntelMsrReader.cpp`](Example/IntelMsrReader.cpp) - MSR 读取器实现
- [`Example/IntelMsrReader.h`](Example/IntelMsrReader.h) - MSR 读取器定义
- [`Example/main.cpp`](Example/main.cpp) - 示例程序

**MIT License** 是一个宽松的开源许可证：
- ✅ 可商用、修改、分发
- ✅ 可与其他许可证组合
- ⚠️ 需保留版权和许可声明
- ❌ 无担保责任

### 📋 多重许可证结构

编译后的可执行文件（如 `cpu_temp.exe`）包含：
1. 项目作者的原创代码（MIT License）
2. IntelMSR.bin 模块（MPL 2.0）
3. 依赖 PawnIO 驱动（GPL-2.0 + 特殊例外）

因此，**可执行文件的分发需同时遵守 MIT License、MPL 2.0 和 GPL-2.0 的要求**。

详细合规信息请查看 [`THIRD-PARTY-NOTICES.md`](THIRD-PARTY-NOTICES.md) 文档。

---

**⚠️ 免责声明**: 本软件仅供学习和研究使用。使用本软件访问 MSR 寄存器可能存在系统风险，请自行承担使用后果。建议在生产环境使用前充分测试。
