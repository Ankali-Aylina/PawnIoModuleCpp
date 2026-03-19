cmake_minimum_required(VERSION 3.10)
project(PawnIoCpuTemp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 配置选项
option(BUILD_AUTO_INSTALL "Build auto install version" ON)
option(BUILD_DRIVER_MANAGER "Build driver manager tool" ON)

# 源文件和头文件
set(PAWNIO_SOURCES
    PawnIoModule.cpp
)

set(PAWNIO_HEADERS
    PawnIoModule.h
)

set(MSR_READER_SOURCES
    IntelMsrReader.cpp
)

set(MSR_READER_HEADERS
    IntelMsrReader.h
)

# ========================================
# 基础版本 - cpu_temp
# ========================================
set(BASE_SOURCES
    ${PAWNIO_SOURCES}
    Example/IntelMsrReader.cpp
    Example/main.cpp
)

set(BASE_HEADERS
    ${PAWNIO_HEADERS}
    Example/IntelMsrReader.h
)

add_executable(cpu_temp ${BASE_SOURCES} ${BASE_HEADERS})
target_include_directories(cpu_temp PRIVATE ${CMAKE_SOURCE_DIR})


# ========================================
# 文件复制和安装规则
# ========================================

# 复制 IntelMSR.bin 到所有可执行文件的输出目录
foreach(target cpu_temp cpu_temp_auto PawnIoDriverManager)
    if(TARGET ${target})
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/IntelMSR.bin"
                "$<TARGET_FILE_DIR:${target}>/IntelMSR.bin"
            COMMENT "Copying IntelMSR.bin to output directory for ${target}"
        )
    endif()
endforeach()

# 设置输出目录
set_target_properties(cpu_temp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)

if(TARGET cpu_temp_auto)
    set_target_properties(cpu_temp_auto PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    )
endif()

if(TARGET PawnIoDriverManager)
    set_target_properties(PawnIoDriverManager PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    )
endif()

# ========================================
# 安装目标
# ========================================
install(TARGETS cpu_temp
    RUNTIME DESTINATION bin
)


# 安装 IntelMSR.bin
install(FILES IntelMSR.bin DESTINATION bin)

# 创建安装包（需要 CPack）
set(CPACK_PACKAGE_NAME "PawnIoCpuTemp")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "Your Name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PawnIo CPU Temperature Reader")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/THIRD-PARTY-NOTICES.md")
set(CPACK_GENERATOR "ZIP")

# 包含第三方声明和许可证文件
install(FILES THIRD-PARTY-NOTICES.md DESTINATION .)
install(FILES LICENSE-MPL-2.0.txt DESTINATION .)
install(FILES LICENSE-MIT.txt DESTINATION .)

include(CPack)
