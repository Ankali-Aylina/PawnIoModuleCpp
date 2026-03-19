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
**作者**: namazso <admin@namazso.eu>  
**许可证**: GPL-2.0（含特殊例外条款）  
**是否修改**: ❌ 否

**描述**:
PawnIO 是一个 Windows 内核驱动，用于访问硬件寄存器。本项目调用它来运行 IntelMSR.bin。

**使用方式**:

- 需单独安装
- 不包含在本项目中
- 用户需自行下载

**许可证摘要**:

PawnIO 使用 **GNU General Public License version 2 (GPL-2.0)**，并包含一项特殊例外：

**主要条款**:
- ✅ 允许重新分发和修改
- ✅ 根据自由软件基金会发布的 GPL 第 2 版或更高版本
- ⚠️ **Copyleft 强传染性** - 基于 GPL 代码的衍生作品必须使用相同许可证
- ⚠️ 必须提供源代码
- ❌ 无担保责任

**特殊例外**:
作为额外许可，PawnIO 的版权持有者允许您将 PawnIO 与以下组件组合：
1. 根据 GNU LGPL 发布的免费软件程序或库
2. 仅通过设备 IO 控制接口与 PawnIO 通信的独立模块

但请注意：
- 此例外**不包括**通过 Pawn 接口与 PawnIO 通信的程序
- 所有加载到 PawnIO 中的模块必须与此许可证兼容
- 推荐使用 **GNU Lesser General Public License version 2.1**

**完整许可证文本**:
完整的 GPL-2.0 许可证文本请参见项目根目录中的 [`LICENSE-GPL-2.0.txt`](LICENSE-GPL-2.0.txt) 文件。

**替代许可**:
如需其他许可选项，请联系版权持有者：admin@namazso.eu

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
3. 依赖 PawnIO 驱动（GPL-2.0 + 特殊例外）

因此，**可执行文件的分发需同时遵守 MIT License、MPL 2.0 和 GPL-2.0 的要求**。

---

## ⚖️ 许可证兼容性说明

### GPL-2.0 的特点

GPL-2.0 是一个**强 Copyleft**许可证，具有以下特点：

✅ **允许**:

- 商业使用
- 修改和分发
- 专利使用

⚠️ **要求**:

- **强传染性** - 任何基于 GPL 代码的衍生作品必须整体采用 GPL-2.0 许可
- 必须提供完整的源代码
- 必须包含 GPL-2.0 许可证文本
- 明确声明修改和日期
- 无担保责任

📋 **与 MPL 2.0 的区别**:
- MPL 2.0 是**文件级** Copyleft，仅要求 MPL 许可的文件本身保持开源
- GPL-2.0 是**整体性** Copyleft，要求整个衍生作品都采用 GPL 许可

### PawnIO 的特殊例外

PawnIO 的 GPL-2.0 许可证包含一项重要例外，允许：

1. 与 LGPL 许可的库组合
2. 与通过设备 IO 控制接口通信的独立模块组合

这意味着：
- ✅ IntelMSR.bin（MPL 2.0）可以通过此例外与 PawnIO 一起使用
- ✅ 项目的原创代码（MIT License）可以通过此例外与 PawnIO 一起使用
- ⚠️ 但如果直接与 PawnIO 通过 Pawn 接口通信，则必须兼容 GPL-2.0

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

### 多重许可证结构

本项目采用事实上的多重许可证结构：

```
┌─────────────────────────────────────────────┐
│   可执行文件 (cpu_temp.exe)                 │
├─────────────────────────────────────────────┤
│  ┌─────────────────────────────────────┐    │
│  │ 原创代码部分                         │    │
│  │ (PawnIoModule.cpp 等)                │    │
│  │ 许可证：MIT                          │    │
│  └─────────────────────────────────────┘    │
│                                             │
│  ┌─────────────────────────────────────┐    │
│  │ IntelMSR.bin                         │    │
│  │ 许可证：MPL 2.0                      │    │
│  └─────────────────────────────────────┘    │
│                                             │
│  ┌─────────────────────────────────────┐    │
│  │ PawnIO.sys (运行时依赖，需单独安装)   │    │
│  │ 许可证：GPL-2.0 + 特殊例外           │    │
│  └─────────────────────────────────────┘    │
└─────────────────────────────────────────────┘
```

---

## 📝 合规性检查清单

作为本项目的维护者或使用者，请确保：

- [x] ✅ 包含 MPL 2.0 许可证文本（LICENSE-MPL-2.0.txt）
- [x] ✅ 包含 GPL-2.0 许可证文本（LICENSE-GPL-2.0.txt）
- [x] ✅ 创建 THIRD-PARTY-NOTICES.md 声明第三方组件
- [x] ✅ IntelMSR.bin 未修改
- [x] ✅ 在 README 中声明 LibreHardwareMonitor 的贡献

---

## 🔍 如何获取源代码

根据 MPL 2.0 许可证第 3.2 条和 GPL-2.0 许可证的要求，如果您需要本项目的源代码：

**本项目源代码**:
- GitHub: [请提供项目地址]
- 联系方式：[请提供联系方式]

**LibreHardwareMonitor 项目** (IntelMSR.bin 来源):
- GitHub: https://github.com/LibreHardwareMonitor/LibreHardwareMonitor

**PawnIO 项目** (驱动依赖):
- GitHub: https://github.com/namazso/PawnIO
- 联系方式：admin@namazso.eu

---

## 📚 参考资料

- **MPL 2.0 许可证全文**: https://www.mozilla.org/en-US/MPL/2.0/
- **GPL-2.0 许可证全文**: https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
- **LibreHardwareMonitor 项目**: https://github.com/LibreHardwareMonitor/LibreHardwareMonitor
- **PawnIO 项目**: https://github.com/namazso/PawnIO
- **MPL 2.0 FAQ**: https://www.mozilla.org/en-US/MPL/2.0/FAQ.html
- **GPL-2.0 FAQ**: https://www.gnu.org/licenses/gpl-faq.html

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
