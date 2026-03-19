cmake_minimum_required(VERSION 3.10)
project(PawnIoCpuTemp VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 配置选项
option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_TESTS "Build test programs" OFF)
option(BUILD_AUTO_INSTALL "Build auto install version" OFF)
option(BUILD_DRIVER_MANAGER "Build driver manager tool" OFF)

# ========================================
# 库目标 - PawnIoModule 静态库
# ========================================
set(PAWNIO_SOURCES
    src/PawnIoModule.cpp
    src/IntelMsrReader.cpp
)

set(PAWNIO_HEADERS
    include/PawnIoModule.h
    include/IntelMsrReader.h
)

add_library(PawnIoModule STATIC ${PAWNIO_SOURCES} ${PAWNIO_HEADERS})
target_include_directories(PawnIoModule PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Windows 特定配置
if(WIN32)
    target_link_libraries(PawnIoModule PRIVATE 
        advapi32.lib
    )
endif()

# ========================================
# 示例程序
# ========================================
if(BUILD_EXAMPLES)
    add_executable(cpu_temp examples/main.cpp)
    target_link_libraries(cpu_temp PRIVATE PawnIoModule)
    
    # 复制 IntelMSR.bin 到输出目录
    add_custom_command(TARGET cpu_temp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/IntelMSR.bin"
            "$<TARGET_FILE_DIR:cpu_temp>/IntelMSR.bin"
        COMMENT "Copying IntelMSR.bin to output directory"
    )
    
    # 设置输出目录
    set_target_properties(cpu_temp PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    )
endif()

# ========================================
# 测试程序
# ========================================
if(BUILD_TESTS)
    enable_testing()
    
    add_executable(test_pawnio tests/test_pawnio.cpp)
    target_link_libraries(test_pawnio PRIVATE PawnIoModule)
    
    # 复制 IntelMSR.bin 到测试输出目录
    add_custom_command(TARGET test_pawnio POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/IntelMSR.bin"
            "$<TARGET_FILE_DIR:test_pawnio>/IntelMSR.bin"
        COMMENT "Copying IntelMSR.bin to test output directory"
    )
    
    set_target_properties(test_pawnio PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    )
    
    # 添加测试
    add_test(NAME PawnIoModuleTest COMMAND test_pawnio)
endif()

# ========================================
# 安装规则
# ========================================

# 安装库文件
install(TARGETS PawnIoModule
    EXPORT PawnIoModuleTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY include/ DESTINATION include)

# 安装可执行文件
if(BUILD_EXAMPLES)
    install(TARGETS cpu_temp
        RUNTIME DESTINATION bin
    )
endif()

# 安装 IntelMSR.bin
install(FILES IntelMSR.bin DESTINATION bin)

# 安装许可证和文档
install(FILES THIRD-PARTY-NOTICES.md DESTINATION .)
install(FILES LICENSE-MPL-2.0.txt DESTINATION .)
install(FILES LICENSE-MIT.txt DESTINATION .)
install(FILES README.md DESTINATION .)

# ========================================
# CPack 打包配置
# ========================================
set(CPACK_PACKAGE_NAME "PawnIoCpuTemp")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "Ankali-Aylina")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PawnIo CPU Temperature Reader - Lightweight C++20 MSR-based temperature monitoring")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/THIRD-PARTY-NOTICES.md")
set(CPACK_GENERATOR "ZIP")

# 导出目标配置
include(CMakePackageConfigHelpers)

# 生成版本配置文件
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/PawnIoModuleConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# 生成配置文件
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/PawnIoModuleConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/PawnIoModuleConfig.cmake"
    INSTALL_DESTINATION cmake
)

# 安装配置文件
install(FILES 
    "${CMAKE_CURRENT_BINARY_DIR}/PawnIoModuleConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/PawnIoModuleConfigVersion.cmake"
    DESTINATION cmake
)

# 导出目标
install(EXPORT PawnIoModuleTargets
    FILE PawnIoModuleTargets.cmake
    NAMESPACE PawnIo::
    DESTINATION cmake
)

include(CPack)
