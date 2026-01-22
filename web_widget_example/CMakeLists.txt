cmake_minimum_required(VERSION 3.16)

project(web_widget_example VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 如果不是作为子项目构建，则需要设置这些变量
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.15.17/5.15.17/msvc2015_64")
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets)

# 项目源文件
set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

# 创建可执行文件
add_executable(web_widget_example
    ${PROJECT_SOURCES}
)

# 设置包含目录
target_include_directories(web_widget_example PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../web_widget_dll/include"
    # 现在不再需要包含 webview2 的路径，因为 WebWidget.h 已经使用了 PIMPL 模式隔离了依赖
)

# 链接 Qt 和 web_widget_dll (纯手动路径模式)
# 这里直接使用磁盘上的相对路径进行链接，不依赖 CMake 的 Target 自动关联机制

# 链接与部署逻辑 (纯手动路径模式)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # ==========================================
    # 场景 A：独立构建模式
    # ==========================================
    # 库文件位于同级库目录下的 bin 文件夹中
    set(WEB_WIDGET_DLL_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../web_widget_dll/bin")
    
    # 手动链接具体的 .lib 文件路径
    target_link_libraries(web_widget_example PRIVATE
        Qt5::Widgets
        debug     "${WEB_WIDGET_DLL_BASE_DIR}/Debug/web_widget.lib"
        optimized "${WEB_WIDGET_DLL_BASE_DIR}/Release/web_widget.lib"
    )

    # 独立构建时，必须手动拷贝 DLL 到本工程输出目录，否则程序无法运行
    add_custom_command(TARGET web_widget_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${WEB_WIDGET_DLL_BASE_DIR}/$<CONFIG>/web_widget.dll"
            "$<TARGET_FILE_DIR:web_widget_example>/web_widget.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${WEB_WIDGET_DLL_BASE_DIR}/$<CONFIG>/WebView2Loader.dll"
            "$<TARGET_FILE_DIR:web_widget_example>/WebView2Loader.dll"
        COMMENT "Independent Build: Copying DLLs to output directory..."
        VERBATIM
    )
else()
    # ==========================================
    # 场景 B：非独立构建模式 (顶层集成)
    # ==========================================
    # 库文件位于顶层输出目录的 bin 中
    set(WEB_WIDGET_DLL_BASE_DIR "${CMAKE_SOURCE_DIR}/bin")
    
    # 虽然在同一个工程中，但仍采用纯手动路径方式链接 .lib
    target_link_libraries(web_widget_example PRIVATE
        Qt5::Widgets
        debug     "${WEB_WIDGET_DLL_BASE_DIR}/Debug/web_widget.lib"
        optimized "${WEB_WIDGET_DLL_BASE_DIR}/Release/web_widget.lib"
    )

    # 添加对 web_widget_dll 的构建依赖，确保 DLL 先于 example 构建完成
    add_dependencies(web_widget_example web_widget_dll)
    
    # 注意：此时 exe 和 dll 已经由顶层配置生成到了同一个 bin 目录下
    # 因此无需执行 POST_BUILD 拷贝动作。
endif()

# 设置可执行文件属性
set_target_properties(web_widget_example PROPERTIES
    WIN32_EXECUTABLE TRUE
)
