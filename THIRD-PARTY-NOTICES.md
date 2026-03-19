# Third-Party Notices and Licenses

本项目包含了来自第三方开源项目的组件，以下是详细的声明和许可信息。

---

## 📦 第三方组件清单

### 1. IntelMSR.bin

**组件名称**: IntelMSR.bin  
**来源项目**: LibreHardwareMonitor  
**来源地址**: https://github.com/LibreHardwareMonitor/LibreHardwareMonitor  
**许可证**: MPL 2.0  
**是否修改**: ❌ 否

**描述**:
IntelMSR.bin 是从 LibreHardwareMonitor 项目获取的二进制驱动模块，用于读取 Intel CPU 的温度信息，**未作任何修改**。

**使用方式**:

- 本项目依赖此二进制文件
- 以二进制形式分发
- 交互逻辑由本项目 C++ 代码自主实现

**许可证要求**:

- ✅ 已包含 MPL 2.0 许可证
- ✅ 已在文档中声明来源

---

### 2. PawnIO Driver

**组件名称**: PawnIO.sys  
**来源项目**: PawnIO  
**来源地址**: https://github.com/namazso/PawnIO  
**许可证**: 见原项目  
**是否修改**: ❌ 否

**描述**:
PawnIO 是一个 Windows 内核驱动，用于访问硬件寄存器。本项目调用它来运行 IntelMSR.bin。

**使用方式**:

- 需单独安装
- 不包含在本项目中
- 用户需自行下载

**许可证要求**:

- 遵循原项目许可

---

## 📄 本项目代码的许可证

### 原创代码部分

以下文件为项目作者编写，采用 **MIT License** 许可：

- [`PawnIoModule.cpp`](PawnIoModule.cpp) - PawnIO 驱动交互接口实现
- [`PawnIoModule.h`](PawnIoModule.h) - PawnIO 驱动交互接口定义
- [`Example/IntelMsrReader.cpp`](Example/IntelMsrReader.cpp) - MSR 读取器实现
- [`Example/IntelMsrReader.h`](Example/IntelMsrReader.h) - MSR 读取器定义
- [`Example/main.cpp`](Example/main.cpp) - 示例程序

**MIT License 摘要**:

- 可商用、修改、分发
- 需保留版权和许可声明
- 无担保

完整的 MIT 许可证文本请见下方的"MIT License"章节。

---

### 编译后的可执行文件

编译生成的可执行文件（如 `cpu_temp.exe`）包含：

1. 项目作者的原创代码（MIT License）
2. IntelMSR.bin 模块（MPL 2.0）

因此，**可执行文件的分发需同时遵守 MIT License 和 MPL 2.0 的要求**。

---

## ⚖️ 许可证兼容性说明

### MPL 2.0 的特点

MPL 2.0 是一个**弱 Copyleft**许可证，具有以下特点：

✅ **允许**:

- 与专有软件结合
- 商业使用
- 专利使用
- 修改代码

⚠️ **要求**:

- 修改 MPL 许可的代码必须开源
- 必须包含许可证文本
- 必须声明来源

📋 **文件级 Copyleft**:
MPL 的要求仅限于 MPL 许可的文件本身，不会"传染"给其他文件。这意味着：

- IntelMSR.bin（MPL 2.0）的修改必须开源
- 您的原创代码（如 PawnIoModule.cpp）可以保持闭源或使用其他许可证

### 双重许可证结构

本项目采用事实上的双重许可证结构：

```
┌─────────────────────────────────────┐
│   可执行文件 (cpu_temp.exe)         │
├─────────────────────────────────────┤
│  ┌─────────────────────────────┐    │
│  │ 原创代码部分                 │    │
│  │ (PawnIoModule.cpp 等)        │    │
│  │ 许可证：MIT                  │    │
│  └─────────────────────────────┘    │
│                                     │
│  ┌─────────────────────────────┐    │
│  │ IntelMSR.bin                 │    │
│  │ 许可证：MPL 2.0              │    │
│  └─────────────────────────────┘    │
└─────────────────────────────────────┘
```

---

## 📝 合规性检查清单

作为本项目的维护者或使用者，请确保：

- [x] ✅ 包含 MPL 2.0 许可证文本（LICENSE-MPL-2.0.txt）
- [x] ✅ 创建 THIRD-PARTY-NOTICES.md 声明第三方组件
- [x] ✅ IntelMSR.bin 未修改
- [x] ✅ 在 README 中声明 LibreHardwareMonitor 的贡献

---

## 🔍 如何获取源代码

根据 MPL 2.0 许可证第 3.2 条的要求，如果您需要本项目的源代码（特别是 IntelMSR.bin 相关的实现），请通过以下方式联系作者：

- GitHub: https://github.com/namazso/PawnIO
- Email: [请联系项目作者]

对于 LibreHardwareMonitor 项目的源代码，请访问：
https://github.com/LibreHardwareMonitor/LibreHardwareMonitor

---

## 📚 参考资料

- **MPL 2.0 许可证全文**: https://www.mozilla.org/en-US/MPL/2.0/
- **LibreHardwareMonitor 项目**: https://github.com/LibreHardwareMonitor/LibreHardwareMonitor
- **PawnIO 项目**: https://github.com/namazso/PawnIO
- **MPL 2.0 FAQ**: https://www.mozilla.org/en-US/MPL/2.0/FAQ.html

---

## ⚠️ 免责声明

本文档仅供参考，不构成法律建议。如果您计划将本项目用于商业用途，建议咨询专业的知识产权律师以确保合规。

---

## 📄 MIT License (适用于项目原创代码)

```
Copyright (c) 2026 Ankali-Aylina

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
