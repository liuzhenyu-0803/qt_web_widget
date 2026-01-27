# ==============================================================================
# 库源文件和头文件配置
# ==============================================================================

set(WEB_WIDGET_SOURCES
    web_widget.cpp
)

set(WEB_WIDGET_HEADERS
    ${PROJECT_SOURCE_DIR}/include/web_widget/web_widget.h
    ${PROJECT_SOURCE_DIR}/include/web_widget/web_widget_dll_export.h
)

# ==============================================================================
# 创建目标
# ==============================================================================

# 创建共享库 (DLL)
add_library(web_widget SHARED
    ${WEB_WIDGET_SOURCES}
    ${WEB_WIDGET_HEADERS}
)

# 为库添加别名，遵循现代 CMake 命名约定 (Namespace::Target)
add_library(WebWidget::web_widget ALIAS web_widget)

# 设置目标属性
set_target_properties(web_widget PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "web_widget"
    WINDOWS_EXPORT_ALL_SYMBOLS OFF  # 使用导出宏手动控制符号导出
)

# Qt 自动化工具设置
set_target_properties(web_widget PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# 编译定义
target_compile_definitions(web_widget PRIVATE WEB_WIDGET_DLL_EXPORTS)

# ==============================================================================
# 包含路径设置
# ==============================================================================

# 头文件包含路径 (使用生成器表达式支持构建和安装)
target_include_directories(web_widget
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# WebView2 SDK 路径设置
set(WEBVIEW2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/third_party/webview2/include")
set(WEBVIEW2_LIB_DIR "${PROJECT_SOURCE_DIR}/third_party/webview2/lib")

# 添加 WebView2 包含目录 (仅在构建时使用)
target_include_directories(web_widget PUBLIC $<BUILD_INTERFACE:${WEBVIEW2_INCLUDE_DIR}>)

# ==============================================================================
# 依赖库查找与链接
# ==============================================================================

# 查找 Qt5 依赖组件
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# 链接库 (PUBLIC 确保依赖传递)
target_link_libraries(web_widget
    PUBLIC
        Qt5::Widgets
    PRIVATE
        "${WEBVIEW2_LIB_DIR}/WebView2Loader.dll.lib"
)

# ==============================================================================
# 后置构建任务
# ==============================================================================

# 拷贝第三方库 (WebView2Loader.dll) 到输出目录
set(THIRD_PARTY_DIR "${WEBVIEW2_LIB_DIR}")
file(GLOB THIRD_PARTY_DLLS "${THIRD_PARTY_DIR}/*.dll")

if(THIRD_PARTY_DLLS)
    foreach(dll_file ${THIRD_PARTY_DLLS})
        add_custom_command(TARGET web_widget POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll_file}"
                $<TARGET_FILE_DIR:web_widget>
            COMMENT "Copying ${dll_file} to output directory"
            VERBATIM
        )
    endforeach()
endif()

# ==============================================================================
# 安装规则 (Installation Rules)
# ==============================================================================

include(GNUInstallDirs)

# 安装目标 (DLL/LIB)
install(TARGETS web_widget
    EXPORT WebWidgetTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 安装 PDB 调试符号 (仅限 MSVC)
if(MSVC)
    install(FILES $<TARGET_PDB_FILE:web_widget>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        OPTIONAL
    )
endif()

# 安装公共头文件
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/web_widget
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 安装依赖的第三方 DLL
if(THIRD_PARTY_DLLS)
    install(FILES ${THIRD_PARTY_DLLS}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# 导出 CMake 配置文件
install(EXPORT WebWidgetTargets
    FILE WebWidgetTargets.cmake
    NAMESPACE WebWidget::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WebWidget
)
