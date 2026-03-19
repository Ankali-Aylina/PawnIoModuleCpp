# 🏗️ 项目架构图

## 系统架构层次

```
┌─────────────────────────────────────────────────────────┐
│                    应用层 (Application)                  │
│  ┌─────────────────┐  ┌──────────────────┐              │
│  │ examples/       │  │ tests/           │              │
│  │ main.cpp        │  │ test_pawnio.cpp  │              │
│  │ - 温度监控示例   │  │ - 单元测试       │              │
│  └────────┬────────┘  └─────────┬────────┘              │
└───────────┼─────────────────────┼───────────────────────┘
            │                     │
            ▼                     ▼
┌─────────────────────────────────────────────────────────┐
│                    库层 (Library)                        │
│  ┌─────────────────────────────────────────────────┐    │
│  │  PawnIoModule (静态库)                           │    │
│  │  ┌─────────────────┐  ┌──────────────────────┐  │    │
│  │  │ include/        │  │ src/                 │  │    │
│  │  │ - PawnIoModule.h│  │ - PawnIoModule.cpp   │  │    │
│  │  │ - IntelMsrReader│  │ - IntelMsrReader.cpp │  │    │
│  │  └─────────────────┘  └──────────────────────┘  │    │
│  └─────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────┐
│                  驱动层 (Driver)                         │
│  ┌─────────────────┐  ┌──────────────────┐              │
│  │ IntelMSR.bin    │  │ PawnIO.sys       │              │
│  │ MSR 驱动模块     │  │ 内核驱动         │              │
│  └─────────────────┘  └──────────────────┘              │
└─────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────┐
│                  硬件层 (Hardware)                       │
│  ┌─────────────────────────────────────────────────┐    │
│  │  Intel CPU MSR Registers (Model Specific Regs)  │    │
│  │  - IA32_THERM_STATUS_MSR (0x19C)                │    │
│  │  - IA32_PACKAGE_THERM_STATUS (0x1B1)            │    │
│  │  - IA32_TEMPERATURE_TARGET (0x1A2)              │    │
│  └─────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────┘
```

## 模块依赖关系

```
┌─────────────────┐
│   main.cpp      │
│ (示例程序)      │
└────────┬────────┘
         │ depends on
         ▼
┌─────────────────┐
│ IntelMsrReader  │
│ (MSR 读取器)     │
└────────┬────────┘
         │ uses
         ▼
┌─────────────────┐
│ PawnIoModule    │
│ (驱动封装)      │
└────────┬────────┘
         │ loads
         ▼
┌─────────────────┐
│ IntelMSR.bin    │
│ (驱动模块)      │
└─────────────────┘
```

## CMake 目标关系

```
┌──────────────────────────────────────────────────────┐
│                   顶级目标                            │
│                                                      │
│  ┌─────────────────┐    ┌─────────────────┐         │
│  │ PawnIoModule    │    │ cpu_temp        │         │
│  │ (STATIC LIB)    │◄───│ (EXECUTABLE)    │         │
│  │                 │    │                 │         │
│  │ - 导出头文件     │    │ - 链接库        │         │
│  │ - 可安装        │    │ - 复制 bin 文件   │         │
│  └────────┬────────┘    └─────────────────┘         │
│           │                                         │
│           │           ┌─────────────────┐           │
│           └──────────►│ test_pawnio     │           │
│                       │ (TEST)          │           │
│                       │                 │           │
│                       │ - 单元测试      │           │
│                       │ - CTest 集成     │           │
│                       └─────────────────┘           │
└──────────────────────────────────────────────────────┘
```

## 构建流程

```
CMakeLists.txt
    │
    ├─► cmake/PawnIoModuleConfig.cmake.in
    │
    ├─► configure_package_config_file()
    │       │
    │       ▼
    │   PawnIoModuleConfig.cmake (生成)
    │   PawnIoModuleConfigVersion.cmake (生成)
    │
    ├─► add_library(PawnIoModule STATIC ...)
    │       │
    │       ├─► 编译 .cpp → .obj
    │       │
    │       └─► 链接 → .lib (静态库)
    │
    ├─► add_executable(cpu_temp ...)
    │       │
    │       ├─► 编译 main.cpp → main.obj
    │       │
    │       ├─► 链接 cpu_temp + PawnIoModule.lib
    │       │
    │       └─► POST_BUILD: 复制 IntelMSR.bin
    │
    └─► install(...)
            │
            ├─► bin/cpu_temp.exe
            ├─► lib/PawnIoModule.lib
            ├─► include/*.h
            ├─► bin/IntelMSR.bin
            └─► cmake/*.cmake
```

## 目录结构树

```
PawnIoModuleCpp/
│
├── 📁 cmake/                          [CMake 配置]
│   └── PawnIoModuleConfig.cmake.in
│
├── 📁 include/                        [公共头文件]
│   ├── PawnIoModule.h                [PawnIO 接口]
│   └── IntelMsrReader.h              [MSR 读取器]
│
├── 📁 src/                            [源代码实现]
│   ├── PawnIoModule.cpp              [PawnIO 实现]
│   └── IntelMsrReader.cpp            [MSR 读取器实现]
│
├── 📁 examples/                       [示例程序]
│   └── main.cpp                      [CPU 温度监控]
│
├── 📁 tests/                          [单元测试]
│   └── test_pawnio.cpp               [基础测试]
│
├── 📁 docs/                           [文档]
│   ├── PROJECT_STRUCTURE.md          [结构说明]
│   ├── QUICKSTART.md                 [快速开始]
│   ├── OPTIMIZATION_SUMMARY.md       [优化总结]
│   └── ARCHITECTURE.md               [本文档]
│
├── 📁 build/                          [构建输出 - 自动生成]
│   ├── bin/                          [可执行文件]
│   ├── lib/                          [库文件]
│   └── cmake/                        [CMake 配置]
│
├── CMakeLists.txt                     [主构建配置]
├── IntelMSR.bin                       [驱动模块]
├── README.md                          [中文文档]
├── README_en.md                       [英文文档]
├── THIRD-PARTY-NOTICES.md             [第三方声明]
├── LICENSE-MIT.txt                    [MIT 许可证]
├── LICENSE-MPL-2.0.txt                [MPL 2.0 许可证]
├── LICENSE-GPL-2.0.txt                [GPL 2.0 许可证]
└── .gitignore                         [Git 忽略规则]
```

## 数据流图

### 温度读取流程

```
用户程序
    │
    │ 1. Initialize(modulePath)
    ▼
IntelMsrReader
    │
    │ 2. LoadFromFile(binData)
    ▼
PawnIoModule
    │
    │ 3. DeviceIoControl(IOCTL_PIO_LOAD_BINARY)
    ▼
PawnIO Driver (内核态)
    │
    │ 4. 加载 IntelMSR.bin
    ▼
IntelMSR Module
    │
    │ 5. Execute("read_msr", {msr_index})
    ▼
CPU MSR Register (0x19C, 0x1B1)
    │
    │ 6. 返回 64-bit 值
    ▼
数据处理
    │
    │ 7. 提取 DeltaT (bits 22:16)
    │ 8. 计算 Temperature = TjMax - DeltaT
    ▼
返回温度值 (°C)
```

## 设计模式应用

### 1. **外观模式 (Facade Pattern)**
```
IntelMsrReader 作为外观类，简化了底层复杂的 MSR 访问逻辑
```

### 2. **RAII (Resource Acquisition Is Initialization)**
```cpp
PawnIoModule module;  // 构造函数获取资源
// ... 使用资源
// 析构函数自动释放资源
```

### 3. **Pimpl 惯用语**
```cpp
// 隐藏实现细节，减少编译依赖
class PawnIoModule {
    HANDLE _handle;      // 设备句柄
    DWORD _lastError;    // 最后错误代码
};
```

## 关键组件说明

### PawnIoModule 类
- **职责**: 封装 PawnIO 驱动通信
- **主要方法**:
  - `LoadFromFile()` - 从文件加载二进制模块
  - `Execute()` - 执行模块函数
  - `Close()` - 关闭设备句柄

### IntelMsrReader 类
- **职责**: 封装 MSR 寄存器访问逻辑
- **主要方法**:
  - `Initialize()` - 初始化读取器
  - `ReadMsr()` - 读取 MSR 寄存器
  - `Close()` - 关闭读取器

### 温度计算算法
```cpp
// 1. 读取 TjMax (最大结温)
value = ReadMsr(0x1A2);
tjMax = (value >> 16) & 0xFF;

// 2. 读取温度状态
value = ReadMsr(0x19C);
deltaT = (value >> 16) & 0x7F;

// 3. 计算实际温度
temperature = tjMax - deltaT;
```

---

**更新日期**: 2026-03-19  
**版本**: 1.0.0
