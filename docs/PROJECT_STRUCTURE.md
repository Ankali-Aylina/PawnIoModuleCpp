# 📁 项目目录结构说明

## 优化后的目录布局

```
PawnIoModuleCpp/
├── cmake/                          # CMake 配置文件
│   └── PawnIoModuleConfig.cmake.in
├── include/                        # 公共头文件
│   ├── PawnIoModule.h             # PawnIO 模块接口
│   └── IntelMsrReader.h           # MSR 读取器接口
├── src/                            # 源代码实现
│   ├── PawnIoModule.cpp           # PawnIO 模块实现
│   └── IntelMsrReader.cpp         # MSR 读取器实现
├── examples/                       # 示例程序
│   └── main.cpp                   # CPU 温度读取示例
├── tests/                          # 单元测试
│   └── test_pawnio.cpp            # PawnIO 模块测试
├── docs/                           # 文档（可选）
├── build/                          # 构建输出目录（自动生成）
├── IntelMSR.bin                    # Intel MSR 驱动模块
├── CMakeLists.txt                  # CMake 构建配置
├── README.md                       # 项目主文档
├── README_en.md                    # 英文文档
├── THIRD-PARTY-NOTICES.md          # 第三方声明
├── LICENSE-MIT.txt                 # MIT 许可证
├── LICENSE-MPL-2.0.txt             # MPL 2.0 许可证
├── LICENSE-GPL-2.0.txt             # GPL 2.0 许可证
└── .gitignore                      # Git 忽略规则
```

## 🎯 主要改进

### 1. **标准化目录结构**
- ✅ `include/` - 存放公共头文件，便于库的使用者查找
- ✅ `src/` - 存放私有实现文件，分离接口与实现
- ✅ `examples/` - 独立示例程序，与原 Example 目录区分
- ✅ `tests/` - 单元测试集中管理
- ✅ `cmake/` - CMake 配置文件独立存放

### 2. **模块化设计**
- ✅ **静态库目标** - `PawnIoModule` 库可被其他项目复用
- ✅ **清晰的依赖关系** - 示例程序和测试都链接到库
- ✅ **导出配置** - 支持其他项目通过 `find_package()` 使用

### 3. **现代化 CMake 实践**
- ✅ **生成器表达式** - 使用 `$<BUILD_INTERFACE:...>` 和 `$<INSTALL_INTERFACE:...>`
- ✅ **命名空间导出** - `PawnIo::PawnIoModule` 避免命名冲突
- ✅ **版本兼容** - 提供 `PawnIoModuleConfigVersion.cmake`
- ✅ **配置选项** - 灵活的编译选项控制

### 4. **完整的安装支持**
- ✅ 安装库文件到 `lib/`
- ✅ 安装头文件到 `include/`
- ✅ 安装可执行文件到 `bin/`
- ✅ 安装 CMake 配置到 `cmake/`
- ✅ 安装许可证和文档

## 📦 使用方式

### 作为库使用者

```cmake
# 在其他项目的 CMakeLists.txt 中
find_package(PawnIoModule REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE PawnIo::PawnIoModule)
```

### 构建项目

```bash
# 标准构建
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .

# 包含测试
cmake .. -DBUILD_TESTS=ON
cmake --build .
ctest

# 仅构建示例
cmake .. -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build .
```

### 安装和使用

```bash
# 安装到系统目录
cmake --install . --prefix /usr/local

# 或安装到自定义目录
cmake --install . --prefix ./install

# 生成安装包
cpack
```

## 🔍 文件说明

### 核心文件

| 文件 | 说明 |
|------|------|
| `include/PawnIoModule.h` | PawnIO 驱动接口封装类 |
| `include/IntelMsrReader.h` | Intel MSR 寄存器读取器 |
| `src/PawnIoModule.cpp` | PawnIO 模块实现 |
| `src/IntelMsrReader.cpp` | MSR 读取器实现 |

### 示例和测试

| 文件 | 说明 |
|------|------|
| `examples/main.cpp` | CPU 温度监控完整示例 |
| `tests/test_pawnio.cpp` | 单元测试套件 |

### 配置文件

| 文件 | 说明 |
|------|------|
| `CMakeLists.txt` | 主构建配置 |
| `cmake/PawnIoModuleConfig.cmake.in` | CMake 包配置模板 |

## 🎨 架构优势

1. **清晰的职责分离** - 接口（include）与实现（src）分离
2. **可复用的库** - 核心功能封装为静态库
3. **易于测试** - 独立的测试目录和 CTest 集成
4. **专业的发布** - 完整的安装和导出配置
5. **灵活的配置** - 多个 option 控制构建内容

## 📝 迁移指南

如果您之前使用的是旧版本，需要进行以下调整：

### 代码引用变更

**旧方式：**
```cpp
#include "PawnIoModule.h"  // 从根目录包含
```

**新方式：**
```cpp
#include <PawnIoModule.h>  // 从 include 目录包含
// 或
#include "PawnIoModule.h"  // 如果配置了 include 路径
```

### 构建变更

**旧 CMakeLists.txt：**
```cmake
add_executable(cpu_temp 
    PawnIoModule.cpp
    Example/main.cpp
)
```

**新 CMakeLists.txt：**
```cmake
add_library(PawnIoModule STATIC src/*.cpp)
add_executable(cpu_temp examples/main.cpp)
target_link_libraries(cpu_temp PawnIoModule)
```

## ⚠️ 注意事项

1. **向后兼容性** - 旧的构建方式不再支持
2. **路径变更** - 所有源文件已移动到新的目录
3. **CMake 版本** - 需要 CMake 3.10+ 以支持所有特性
4. **命名空间** - 使用时需添加 `PawnIo::` 前缀

---

**更新日期**: 2026-03-19  
**版本**: 1.0.0
