# Qt WebWidget

基于 Microsoft Edge WebView2 的高性能 Qt 浏览器控件库。

## 简介
`WebWidget` 是一个将 WebView2 深度集成到 Qt 框架中的控件库。它允许开发者在 Qt 应用程序中嵌入现代化的 Chromium 渲染引擎，支持 HTML5、CSS3、JavaScript 以及透明背景、自定义请求头等高级功能。

本项目采用 PIMPL 模式封装，有效隔离了底层 Windows SDK 依赖，确保 Qt 项目的简洁性。

## 核心特性
*   **现代内核**: 基于 Microsoft Edge WebView2 (Chromium)，提供卓越的渲染性能和最新的 Web 标准支持。
*   **Qt 深度集成**: 封装为标准的 `QWidget`，可直接通过代码或 Qt Designer (需自行包装) 放入布局，支持信号槽机制。
*   **透明背景**: 支持设置透明背景色，允许网页内容与 Qt 本地界面完美融合。
*   **缓存控制**: 通过内部请求拦截器实现，默认禁用 HTTP 缓存，确保网页内容实时更新（尤其适用于本地调试）。
*   **环境自检**: 提供静态方法 `isWebView2RuntimeInstalled()`，方便在程序启动时引导用户安装必要运行时。
*   **自动清理**: 程序退出时自动删除临时产生的 `UserDataFolder`（包含缓存、Cookies 等），保护用户隐私。
*   **单窗口导航**: 拦截新窗口弹出请求，强制在当前控件内加载，适合嵌入式或单页应用场景。

## 环境要求
*   **操作系统**: Windows 10 或 Windows 11 (x64)
*   **开发环境**: Qt 5.15+ (仅限 **MSVC** 编译器)
*   **运行时**: [Microsoft Edge WebView2 Runtime](https://developer.microsoft.com/en-us/microsoft-edge/webview2/)

## 快速上手

### 1. 引用库
在你的 Qt 项目中包含头文件：
```cpp
#include "web_widget/web_widget.h"
```

### 2. 基本使用
```cpp
using WebWidgetLib::WebWidget;

// 1. 检查运行时环境
if (!WebWidget::isWebView2RuntimeInstalled()) {
    QMessageBox::critical(nullptr, "错误", "请先安装 Microsoft Edge WebView2 Runtime");
    return -1;
}

// 2. 创建并使用控件
WebWidget *view = new WebWidget(this);
view->navigate("https://www.bing.com");

// 3. 添加到布局
layout->addWidget(view);
```

### 3. 主要 API
| 方法 | 说明 |
| :--- | :--- |
| `navigate(const QString &url)` | 导航至指定 URL (支持 http/https/file 等) |
| `goBack()` / `goForward()` | 页面后退 / 前进 |
| `refresh()` | 刷新当前页面 |
| `canGoBack()` / `canGoForward()` | 返回是否可以执行后退或前进操作 |
| `isWebView2RuntimeInstalled()` | (静态方法) 检查系统是否已安装 WebView2 运行时 |

## 构建说明

### 修改 Qt 路径
在根目录 `CMakeLists.txt` 中，找到以下行并修改为你的 Qt MSVC 环境路径：
```cmake
set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.15.17/5.15.17/msvc2015_64")
```

### 推荐构建方式 (Windows 脚本)
进入 `scripts` 目录，提供了便捷的批处理脚本进行一键构建和安装：
*   执行 `install_debug.bat`: 构建并安装 Debug 版本。
*   执行 `install_release.bat`: 构建并安装 Release 版本。

构建成功后，所有产物（DLL、LIB、头文件）将存放在项目根目录下的 `install` 文件夹中。

### 手动构建 (命令行)
```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
cmake --build . --config Release --target install
```

## 项目结构
*   **`include/`**: 公共头文件，定义导出接口。
*   **`src/`**: 核心源码，包含 Qt 控件封装与 WebView2 回调逻辑。
*   **`examples/`**: 示例程序，展示了完整的工具栏及浏览器基本操作。
*   **`third_party/`**: 内置 WebView2 SDK (1.0.x)，包含编译所需的 Lib 和 DLL。
*   **`scripts/`**: 自动化构建与分发脚本。

## 许可证
本项目仅供学习与参考使用。
