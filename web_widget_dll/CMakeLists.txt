cmake_minimum_required(VERSION 3.16)

project(web_widget_dll VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 如果不是作为子项目构建，则需要设置这些变量
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.15.17/5.15.17/msvc2015_64")
endif()

# 定义导出宏
add_compile_definitions(WEB_WIDGET_DLL_EXPORTS)

# 查找 Qt 依赖
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# WebView2 SDK 路径
set(WEBVIEW2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/webview2/include")
set(WEBVIEW2_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/webview2/lib")

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 使用通配符方式自动导入头文件和源文件
file(GLOB INC_FILES CONFIGURE_DEPENDS "${INC_DIR}/*.h")
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

# 生成动态库
add_library(web_widget_dll SHARED
    ${INC_FILES}
    ${SRC_FILES}
)

# 设置包含目录
target_include_directories(web_widget_dll 
    PUBLIC 
        ${INC_DIR}
        ${WEBVIEW2_INCLUDE_DIR}
)

# 链接 Qt 和 WebView2 库
target_link_libraries(web_widget_dll 
    PRIVATE 
        Qt5::Widgets
        "${WEBVIEW2_LIB_DIR}/WebView2Loader.dll.lib"
)

# 设置输出文件名
set_target_properties(web_widget_dll PROPERTIES
    OUTPUT_NAME "web_widget"
)

# 仅在独立构建时设置输出目录到项目下的 bin 目录
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set_target_properties(web_widget_dll PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        PDB_OUTPUT_DIRECTORY     "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    )
endif()

# Release 模式下生成 PDB
if(MSVC)
    target_compile_options(web_widget_dll PRIVATE $<$<CONFIG:Release>:/Zi>)
    target_link_options(web_widget_dll PRIVATE $<$<CONFIG:Release>:/DEBUG /OPT:REF /OPT:ICF>)
endif()

# 拷贝第三方库到生成目录
file(GLOB THIRD_PARTY_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/webview2/lib/*.dll")

if(THIRD_PARTY_LIBS)
    add_custom_command(TARGET web_widget_dll POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${THIRD_PARTY_LIBS}
            $<TARGET_FILE_DIR:web_widget_dll>
        COMMENT "Copying WebView2 libraries to output directory..."
    )
endif()
