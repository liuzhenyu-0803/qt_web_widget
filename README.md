# Qt WebWidget Project

这是一个将 Microsoft Edge WebView2 深度集成到 Qt 框架中的示例项目。它通过封装 DLL 的方式，为 Qt 应用程序提供了高性能、现代化的 Chromium 渲染能力。

## 项目结构

项目分为两个主要部分：

*   **[`web_widget_dll/`](web_widget_dll/)**: 核心组件。
    *   基于 WebView2 SDK 封装的 Qt 控件库。
    *   提供环境检测、导航控制、透明背景、缓存禁用等功能。
    *   采用 PIMPL 模式，隔离了复杂的 COM 接口和 Windows 原生 API。
*   **[`web_widget_example/`](web_widget_example/)**: 示例程序。
    *   演示了如何链接并使用 `web_widget_dll`。
    *   提供了一个完整的浏览器外壳界面（地址栏、导航按钮等）。

## 核心特性

*   **现代内核**: 使用 Chromium 渲染引擎，完美支持 HTML5/CSS3/JavaScript。
*   **Qt 深度集成**: 将 WebView2 封装为标准 `QWidget`，可直接放入布局。
*   **环境自检**: 内置 `isWebView2RuntimeInstalled()` 静态方法，方便程序在启动时进行依赖检查。
*   **高性能**: 相比 Qt 自带的 WebEngine，本项目通过直接使用系统运行时，减小了应用分发体积。

## 快速开始

### 1. 环境要求
*   **操作系统**: Windows 10 或 Windows 11。
*   **运行环境**: [Microsoft Edge WebView2 Runtime](https://developer.microsoft.com/en-us/microsoft-edge/webview2/)。
*   **开发环境**: Qt 5.15+ (MSVC 2015/2017/2019)。

### 2. 构建项目
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## 文档指引

*   如果你想了解控件的详细接口和集成方法，请阅读 [**WebWidget DLL 详细文档**](web_widget_dll/README.md)。
*   如果你想查看如何编写调用代码，请参考 [`web_widget_example/MainWindow.cpp`](web_widget_example/MainWindow.cpp)。

## 许可证
本项目仅供学习与参考使用。
