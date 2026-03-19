# PawnIoModuleCpp - Intel CPU Temperature Reader Module

[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-orange.svg)](https://www.mozilla.org/en-US/MPL/2.0/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE-MIT.txt)
[![License: GPL-2.0](https://img.shields.io/badge/License-GPL--2.0-blue.svg)](LICENSE-GPL-2.0.txt)

---

<div align="center">

**🌍 Choose Language / 选择语言:**

[**🇨🇳 简体中文**](README.md) | [**🌐 English**](README_en.md)

</div>

---

## 📖 Introduction

**PawnIoModuleCpp** is a C++20-based system-level temperature reading module that provides hardware-level CPU temperature monitoring by accessing Intel MSR (Model Specific Registers). This project uses the PawnIO v2 driver and IntelMSR module to deliver precise CPU temperature data for developers and hardware monitoring tools.

### 💡 Project Origin

The core binary component **IntelMSR.bin** of this project is extracted from the open-source hardware monitoring tool **[LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor)**. Referencing LibreHardwareMonitor's mature MSR register access solution and temperature calculation algorithm, we have implemented the complete interaction logic with the IntelMSR.bin module using C++20.

**Key Improvements:**
- 🎯 **Modular Encapsulation** - Separates temperature reading logic from the monolithic monitoring system
- 🚀 **Simplified Dependencies** - Removes unnecessary hardware support, focusing solely on CPU temperature monitoring
- 📦 **Easy Integration** - Provides clean C++ API interfaces for convenient integration into other projects
- 🔧 **Cross-Platform Build** - CMake-based build system supporting multiple development environments
- 💻 **Native C++ Implementation** - Better performance compared to the C# version, more suitable for system integration

### Core Features

- 🔍 **Real-time Temperature Monitoring** - Reads CPU core temperature and package temperature
- 🚀 **High Performance** - Direct MSR register access with low latency
- 🛡️ **Modular Design** - Clean code structure, easy to integrate
- 📦 **Dual-Mode Build** - Supports both Debug and Release configurations
- 💻 **Windows Platform** - Optimized specifically for Windows systems

## 🏗️ System Architecture

```
┌─────────────────┐
│   main.cpp      │  ← Example program (temperature monitoring)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ IntelMsrReader  │  ← MSR reader encapsulation
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ PawnIoModule    │  ← PawnIO driver interface
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ IntelMSR.bin    │  ← Intel MSR driver module
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ PawnIO.sys      │  ← Kernel driver
└────────┬────────┘
```

## 📋 Dependencies

### Required Dependencies

- **Operating System**: Windows 10/11 (64-bit)
- **Compiler**: C++20 compatible compiler (MSVC 2019+, GCC 10+, Clang 12+)
- **Build System**: CMake 3.10 or higher
- **Driver**: [PawnIO v2 Driver](https://github.com/namazso/PawnIO)
- **Driver Module**: `IntelMSR.bin` (extracted from LibreHardwareMonitor project)

### Optional Tools

- **Ninja** - Fast build tool
- **CPack** - Package generator

## 🔧 Installation & Configuration

### 1. Install PawnIO Driver

Before running this project, you must install the PawnIO driver:

```powershell
# Download and install PawnIO driver
# Visit https://github.com/namazso/PawnIO for installation package
```

**Note**: Administrator privileges are required to install the driver.

### 2. Prepare IntelMSR.bin

Obtain `IntelMSR.bin` from the [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) project.
Ensure `IntelMSR.bin` is located in one of the following paths:
- The executable file directory
- Windows system directory (`C:\Windows\System32`)
- PawnIO driver installation directory

### 3. Clone the Project

```bash
git clone <repository-url>
cd PawnIoModuleCpp
```

## 🏃 Quick Start

### Building the Project

```bash
# Navigate to project root directory
cd PawnIoModuleCpp

# Create build directory
mkdir build && cd build

# Configure CMake (Release mode)
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release
```

### Running the Example Program

**Important**: The program requires administrator privileges to access hardware registers!

```bash
# Run as Administrator
# In PowerShell or CMD (Right-click → Run as Administrator)
cd build\Release
.\cpu_temp.exe
```

### Expected Output

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

## 💻 Usage

### Basic API

#### Initialize MSR Reader

```cpp
#include "IntelMsrReader.h"

IntelMsrReader reader;
std::wstring modulePath = L"IntelMSR.bin";

if (!reader.Initialize(modulePath)) {
    std::cerr << "Initialization failed!" << std::endl;
    return 1;
}
```

#### Read MSR Register

```cpp
uint64_t value;
// Method 1: Read 64-bit value
if (reader.ReadMsr(0x19C, value)) {
    std::cout << "MSR value: 0x" << std::hex << value << std::endl;
}

// Method 2: Read EAX and EDX separately
uint32_t eax, edx;
if (reader.ReadMsr(0x19C, eax, edx)) {
    std::cout << "EAX: 0x" << std::hex << eax 
              << ", EDX: 0x" << edx << std::endl;
}
```

#### Temperature Reading Example

```cpp
// Read TjMax (maximum junction temperature)
float tjMax;
ReadTjMax(reader, tjMax);

// Read core temperature
float coreTemp = ReadCoreTemperature(reader, tjMax);

// Read package temperature (entire CPU)
float packageTemp = ReadPackageTemperature(reader, tjMax);

std::cout << "CPU Temperature: " << packageTemp << "°C" << std::endl;
```

### Complete Example

Refer to [`Example/main.cpp`](Example/main.cpp) for the complete temperature monitoring program.

## 📁 Project Structure

```
PawnIoModuleCpp/
├── PawnIoModule.h          # PawnIO module interface definition
├── PawnIoModule.cpp        # PawnIO module implementation
├── Example/
│   ├── IntelMsrReader.h    # MSR reader header file
│   ├── IntelMsrReader.cpp  # MSR reader implementation
│   └── main.cpp            # Example program (temperature monitoring)
├── CMakeLists.txt          # CMake build configuration
└── README.md               # This file
```

## 🔬 Technical Details

### MSR Register Addresses

Key MSR registers used by the project:

| Register Name | Address | Description |
|---------------|---------|-------------|
| `IA32_THERM_STATUS_MSR` | 0x019C | Core thermal status register |
| `IA32_PACKAGE_THERM_STATUS` | 0x01B1 | Package thermal status register |
| `IA32_TEMPERATURE_TARGET` | 0x01A2 | Temperature target register (contains TjMax) |

### Temperature Calculation Principle

According to Intel SDM (Software Developer's Manual):

```
Actual Temperature = TjMax - DeltaT

Where:
- TjMax: CPU maximum junction temperature (read from bits 23:16 of IA32_TEMPERATURE_TARGET)
- DeltaT: Current temperature offset (read from bits 22:16 of IA32_THERM_STATUS_MSR)
```

### Safety Considerations

⚠️ **Warning**: 
- Accessing MSR registers requires **administrator privileges**
- Incorrect MSR operations may cause system instability
- This project is intended for educational and monitoring purposes only
- Thoroughly test before using in production environments

## 🛠️ Troubleshooting

### Common Issues

#### 1. "Initialization failed" Error

**Cause**: PawnIO driver not installed or IntelMSR.bin not found

**Solution**:
```powershell
# Check driver status
sc query PawnIO

# If not installed, reinstall PawnIO driver
# Ensure IntelMSR.bin exists in the executable directory
```

#### 2. "NOT running as Administrator" Error

**Cause**: Program not running with administrator privileges

**Solution**:
- Right-click executable → "Run as administrator"
- Or run in an elevated command prompt

#### 3. Temperature Reading Failure (Returns -1.0f)

**Causes**: 
- CPU does not support the required MSR registers
- Timing issues (CPU in low-power state)

**Solutions**:
- Check CPU compatibility list
- Try reading multiple times
- Add appropriate delays

### Debugging Tips

Enable verbose output:
```cpp
// Detailed debug output is already available in main.cpp
// Look for information prefixed with [DEBUG]
```

## 🤝 Contributing

Issues and Pull Requests are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project uses a dual-license model:
- **Original C++ code**: MIT License
- **IntelMSR.bin binary**: MPL 2.0 (from LibreHardwareMonitor)

See the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **LibreHardwareMonitor** - The core binary component IntelMSR.bin of this project is extracted from LibreHardwareMonitor. The core algorithms for temperature reading (MSR register address definitions, thermal calculation methods) reference LibreHardwareMonitor's open-source implementation.
  - Project URL: https://github.com/LibreHardwareMonitor/LibreHardwareMonitor
  - License: **MPL 2.0** (Mozilla Public License 2.0)
  - ⚠️ **Important Notice**: The IntelMSR.bin file is directly sourced from LibreHardwareMonitor and its use is subject to LibreHardwareMonitor's license terms (MPL 2.0).
  
- **PawnIO Project**: https://github.com/namazso/PawnIO - Provides low-level driver support for accessing MSR registers
  - License: Please refer to the PawnIO project's specific license terms

- **Intel® 64 and IA-32 Architectures Software Developer's Manual** - Intel's official documentation providing authoritative definitions and usage of MSR registers

### 🔗 Relationship with LibreHardwareMonitor

| Feature | LibreHardwareMonitor | PawnIoModuleCpp |
|---------|---------------------|-----------------|
| **Purpose** | Full-featured hardware monitoring tool (C#/.NET) | Lightweight CPU temperature module (C++20) |
| **Language** | C# (.NET) | C++20 |
| **Build System** | Visual Studio / MSBuild | CMake |
| **Supported Hardware** | CPU, GPU, Motherboard, Memory, etc. | Intel CPU only |
| **Size** | ~10MB+ | <100KB |
| **Integration Difficulty** | Requires entire framework | Simple C++ library calls |
| **Temperature Algorithm** | Reference implementation | Independently implemented based on same principles |
| **Core Module** | IntelMSR.bin (original) | IntelMSR.bin (extracted from LHM) |
| **Interaction Logic** | C# implementation | ✅ **C++ independently refactored** |

**Technical References:**
- ✅ **IntelMSR.bin Module** - Directly extracted from LibreHardwareMonitor project (binary file unmodified)
- ✅ MSR register address definitions (`IA32_THERM_STATUS_MSR`, `IA32_PACKAGE_THERM_STATUS`, `IA32_TEMPERATURE_TARGET`)
- ✅ TjMax extraction method (bits 23:16)
- ✅ DeltaT calculation logic (bits 22:16)
- ✅ Temperature formula: `Temperature = TjMax - DeltaT`
- ✅ PawnIO driver call flow

**Project Implementation:**
- [`PawnIoModule`](PawnIoModule.h/cpp) - PawnIO driver interaction interface
- [`IntelMsrReader`](Example/IntelMsrReader.h/cpp) - MSR reader encapsulation class
- Temperature monitoring example program ([`main.cpp`](Example/main.cpp))
- CMake build configuration and package generation

## 🔄 Changelog

### v1.0.0 (2026-03-19)
- ✨ Initial release
- ✅ Intel CPU temperature reading support
- ✅ Core and package temperature monitoring
- ✅ Real-time temperature statistics (min, max, average)
- ✅ Comprehensive error handling and diagnostics

---

## ⚖️ License Compliance Information

### 📜 Licenses Used in This Project

**Important Notice**: This project contains multiple components, each using different licenses:

#### 1. IntelMSR.bin - MPL 2.0 License

**Source**: LibreHardwareMonitor Project  
**License**: **MPL 2.0 (Mozilla Public License)**  
**Modified**: ❌ No

MPL 2.0 is a **weak Copyleft** license with the following main requirements:

✅ **Permitted Uses:**
- Commercial use
- Modification
- Distribution
- Patent use
- Private modifications (limited to MPL-covered files)

⚠️ **Required Obligations:**
1. **Source Code Disclosure**: If you modify the source code related to IntelMSR.bin (the MPL-licensed part), you must make those modifications available under the MPL 2.0 license
2. **License Notice**: You must clearly state in documentation that MPL-licensed code is used
3. **License Text**: You must include the full text of the MPL 2.0 license
4. **File-Level Copyleft**: MPL requirements apply only to the MPL-licensed files themselves and do not infect other files

**This project has NOT modified the IntelMSR.bin file from LibreHardwareMonitor.**

#### 2. PawnIO.sys - GPL-2.0 License (with Special Exception)

**Source**: PawnIO Project (https://github.com/namazso/PawnIO)  
**Author**: namazso <admin@namazso.eu>  
**License**: **GPL-2.0 (GNU General Public License version 2)** + Special Exception  
**Modified**: ❌ No

GPL-2.0 is a **strong Copyleft** license with the following characteristics:

✅ **Permitted Uses:**
- Commercial use
- Modification and distribution
- Patent use

⚠️ **Required Obligations:**
1. **Strong Viral Nature** - Any derivative works based on GPL code must be licensed as a whole under GPL-2.0
2. **Source Code Provision** - Must provide complete source code or written offer to provide it
3. **License Text** - Must include the full text of GPL-2.0 license
4. **Clear Notice** - Must clearly state modifications and dates
5. **NO WARRANTY** - Provided "AS IS" without warranty of any kind

**Special Exception Clause**:
The copyright holders of PawnIO grant you additional permission to combine PawnIO with:
- Free software programs or libraries released under GNU LGPL
- Independent modules that communicate with PawnIO solely through the device IO control interface

This means:
- ✅ IntelMSR.bin (MPL 2.0) can be used with PawnIO under this exception
- ✅ This project's original code (MIT License) can be used with PawnIO under this exception
- ⚠️ But if communicating directly with PawnIO over the Pawn interface, must be compatible with GPL-2.0

**Runtime Dependency**:
PawnIO driver is a runtime dependency that users need to download and install separately; it is not included in this project.

#### 3. This Project's Original Code - MIT License

**Scope**: 
- [`PawnIoModule.cpp`](PawnIoModule.cpp) - PawnIO driver interaction interface implementation
- [`PawnIoModule.h`](PawnIoModule.h) - PawnIO driver interaction interface definition
- [`Example/IntelMsrReader.cpp`](Example/IntelMsrReader.cpp) - MSR reader implementation
- [`Example/IntelMsrReader.h`](Example/IntelMsrReader.h) - MSR reader definition
- [`Example/main.cpp`](Example/main.cpp) - Example program

**MIT License** is a permissive open source license:
- ✅ Permits commercial use, modification, distribution
- ✅ Can be combined with other licenses
- ⚠️ Requires retention of copyright and license notice
- ❌ NO WARRANTY

### 📋 Multi-License Structure

The compiled executable (e.g., `cpu_temp.exe`) contains:
1. Project author's original code (MIT License)
2. IntelMSR.bin module (MPL 2.0)
3. Depends on PawnIO driver (GPL-2.0 + special exception)

Therefore, **distribution of the executable must comply with the requirements of MIT License, MPL 2.0, and GPL-2.0 simultaneously**.

For detailed compliance information, please see the [`THIRD-PARTY-NOTICES.md`](THIRD-PARTY-NOTICES.md) document.

---

⚠️ **Disclaimer**: This software is provided for educational and research purposes only. Using this software to access MSR registers may pose system risks, and you assume all responsibility for such use. It is recommended to thoroughly test before using in production environments.

### 📝 Recommended Compliance Measures

#### ✅ **Immediate Actions Required:**

1. **Include LICENSE Files** - Include all license files in the project:
   ```
   LICENSE-MPL-2.0.txt    (LibreHardwareMonitor's MPL 2.0 license)
   LICENSE-MIT.txt        (Your own code's MIT license)
   LICENSE-GPL-2.0.txt    (PawnIO's GPL-2.0 license)
   ```

2. **Create THIRD-PARTY-NOTICES File** - Clearly declare all third-party components:
   ```md
   # Third-Party Notices
   
   ## IntelMSR.bin
   - Source: LibreHardwareMonitor (https://github.com/LibreHardwareMonitor/LibreHardwareMonitor)
   - License: MPL 2.0
   - Modification Status: Unmodified (using original binary file only)
   
   ## PawnIO Driver
   - Source: https://github.com/namazso/PawnIO
   - Author: namazso <admin@namazso.eu>
   - License: GPL-2.0 with special exception
   - Modification Status: Unmodified
   ```
   
   **Important Note**: All C++ code in this project (including interaction logic with IntelMSR.bin) is independently and originally implemented by the author and does not directly originate from the LibreHardwareMonitor project.

3. **Update README License Badges** - Reflect true multi-license status with badges for MPL 2.0, MIT, and GPL-2.0

### 🚨 Risk Warning

❌ **Current Project Risks:**
1. Multiple licenses must be complied with simultaneously
2. GPL-2.0 has strong copyleft requirements (though mitigated by special exception)
3. Runtime dependency on external driver (PawnIO)

⚠️ **Non-compliance may result in:**
- Copyright infringement risks
- Accountability from original project authors
- Impact on commercial use legitimacy

### 📋 Next Steps Checklist

- [x] **Download MPL 2.0 license text** - https://www.mozilla.org/en-US/MPL/2.0/
- [x] **Download GPL-2.0 license text** - https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
- [x] **Create THIRD-PARTY-NOTICES.md** - Declare all third-party components
- [x] **Confirm IntelMSR.bin is unmodified** - ✅ Using original binary file only
- [x] **Confirm PawnIO is unmodified** - ✅ Using original driver
- [x] **Update README license badges** - Reflect true license status
- [ ] **Replace placeholder information** - Add your name and year to LICENSE-MIT.txt
- [ ] **Consider legal consultation** - If used for commercial purposes, consult an intellectual property attorney

---