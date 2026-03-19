# 🚀 快速开始指南

本文档将帮助您在 5 分钟内完成项目的构建和运行。

## 📋 前置要求

- **操作系统**: Windows 10/11 (64 位)
- **编译器**: MSVC 2019+, GCC 10+, 或 Clang 12+
- **CMake**: 3.10 或更高版本
- **驱动程序**: PawnIO v2 Driver（已安装）
- **权限**: 管理员权限（访问 MSR 寄存器必需）

## 🔧 构建步骤

### 1. 克隆项目

```bash
git clone <repository-url>
cd PawnIoModuleCpp
```

### 2. 创建构建目录

```bash
mkdir build
cd build
```

### 3. 配置 CMake

**Windows (Visual Studio):**
```bash
cmake .. -G "Visual Studio 17 2022" -A x64
```

**Windows (Ninja - 推荐):**
```bash
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
```

**Linux (GCC):**
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### 4. 编译项目

```bash
# Release 模式
cmake --build . --config Release

# Debug 模式（可选）
cmake --build . --config Debug
```

### 5. 运行示例程序

```bash
# 进入输出目录
cd bin/Release

# 以管理员身份运行
# 在 Windows 上右键 → 以管理员身份运行，然后执行：
cpu_temp.exe

# 或在 PowerShell/CMD 中（管理员）：
.\cpu_temp.exe
```

## 🎯 预期输出

如果一切正常，您将看到类似以下的输出：

```
========================================
  Intel CPU Temperature Reader
  Using PawnIO v2 Driver
========================================

Loading module: IntelMSR.bin
[OK] Module loaded successfully!

Reading CPU information...
[OK] CPU detected - TjMax: 100.0 C

Starting Temperature Monitoring (Press Ctrl+C to stop)
========================================
[ 1] Temperature:  45.0 C ( 113.0 F)
[ 2] Temperature:  46.0 C ( 114.8 F)
[ 3] Temperature:  47.0 C ( 116.6 F)
...
```

## 🧪 运行测试（可选）

如果您在配置时启用了测试：

```bash
# 配置时启用测试
cmake .. -DBUILD_TESTS=ON

# 编译后运行测试
ctest

# 或手动运行测试程序
./bin/Debug/test_pawnio.exe
```

## 📦 作为库使用

要在您自己的项目中使用 PawnIoModule：

### 方法 1: 通过 CMake 包

```cmake
# 安装库后
find_package(PawnIoModule REQUIRED)
target_link_libraries(your_target PRIVATE PawnIo::PawnIoModule)
```

### 方法 2: 直接包含源文件

```cpp
#include "PawnIoModule.h"
#include "IntelMsrReader.h"

// 使用库的 API
```

## ⚠️ 常见问题

### Q1: "Initialization failed" 错误

**解决方案:**
1. 确认已安装 PawnIO 驱动：`sc query PawnIO`
2. 确认 IntelMSR.bin 文件存在
3. 以管理员身份运行程序

### Q2: 找不到 CMake

**解决方案:**
- Windows: 从 [cmake.org](https://cmake.org/download/) 下载并安装
- Linux: `sudo apt-get install cmake` 或 `sudo yum install cmake`

### Q3: 编译失败 - C++20 不支持

**解决方案:**
- 升级到更新的编译器（MSVC 2019+, GCC 10+, Clang 12+）
- 或在 CMakeLists.txt 中降低 CMAKE_CXX_STANDARD（不推荐）

### Q4: 温度读取失败

**解决方案:**
- 确认 CPU 支持 MSR 寄存器访问
- 检查是否以管理员权限运行
- 验证 PawnIO 驱动是否正确安装

## 🛠️ 开发建议

### IDE 配置

**VS Code:**
1. 安装 C/C++ 扩展
2. 打开项目文件夹
3. CMake Tools 会自动检测配置

**Visual Studio:**
1. 文件 → 打开 → 文件夹
2. 选择项目根目录
3. CMake 工具会自动加载

**CLion:**
1. 文件 → 打开
2. 选择 CMakeLists.txt
3. CLion 会自动配置项目

### 调试技巧

```bash
# 生成 Debug 版本
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 启用详细输出
cmake --build . --verbose

# 查看构建数据库
cat build/compile_commands.json
```

## 📚 下一步

- 阅读 [`docs/PROJECT_STRUCTURE.md`](docs/PROJECT_STRUCTURE.md) 了解项目结构
- 查看 [`examples/main.cpp`](examples/main.cpp) 学习 API 使用
- 参考 [README.md](README.md) 获取完整文档

## 🆘 获取帮助

如果遇到问题：
1. 检查 [README.md](README.md) 中的依赖项说明
2. 查看 [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md) 了解许可证信息
3. 在项目 Issues 中提问

---

**祝您使用愉快！** 🎉
