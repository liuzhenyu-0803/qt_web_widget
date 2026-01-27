cmake_minimum_required(VERSION 3.16)

project(WebWidget
    VERSION 0.1.0
    DESCRIPTION "Web Widget library using WebView2 and Qt"
    LANGUAGES CXX
)

# ==============================================================================
# 全局编译选项设置
# ==============================================================================

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt 库路径 (根据实际环境配置)
set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.15.17/5.15.17/msvc2015_64")

# MSVC 平台在 Release 模式下生成 PDB 调试信息
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    add_compile_options(/MP)
endif()

# ==============================================================================
# 构建输出目录设置 (统一输出到顶层 build 目录下)
# ==============================================================================

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()
if(NOT CMAKE_PDB_OUTPUT_DIRECTORY)
    set(CMAKE_PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

# ==============================================================================
# 子项目包含
# ==============================================================================

# 构建示例程序的选项
option(BUILD_EXAMPLES "Build example applications" ON)

# 包含核心库源码
add_subdirectory(src)

# 包含示例程序
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
