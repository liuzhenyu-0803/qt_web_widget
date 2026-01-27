# ==============================================================================
# 示例程序源文件配置
# ==============================================================================

set(EXAMPLE_SOURCES
    main.cpp
    main_window.cpp
)

set(EXAMPLE_HEADERS
    main_window.h
)

# ==============================================================================
# 创建可执行文件
# ==============================================================================

add_executable(web_widget_example
    ${EXAMPLE_SOURCES}
    ${EXAMPLE_HEADERS}
)

# 设置目标属性
set_target_properties(web_widget_example PROPERTIES
    WIN32_EXECUTABLE TRUE
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# ==============================================================================
# 依赖项链接
# ==============================================================================

# 查找 Qt5 组件
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# 链接核心库与依赖项
target_link_libraries(web_widget_example
    PRIVATE
        Qt5::Widgets
        WebWidget::web_widget
)
