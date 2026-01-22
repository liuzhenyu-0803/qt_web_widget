# WebWidget DLL

基于 **Microsoft Edge WebView2** 封装的 Qt Web 控件库。

## 技术栈
*   **渲染引擎**: Chromium (via Microsoft Edge WebView2)
*   **框架**: Qt 5.15.x
*   **接口模式**: PIMPL (d-pointer) 模式，隐藏底层 COM 细节
*   **构建工具**: CMake

## 操作系统要求
*   **最低支持**: Windows 10 (或安装了 WebView2 运行时的 Windows 7/8.1)
*   **推荐系统**: Windows 11 (已内置运行时)
*   **编译器**: 必须使用 **MSVC** (不支持 MinGW)

## 核心依赖
运行环境必须安装 **Microsoft Edge WebView2 Runtime**。
*   [官方下载地址](https://developer.microsoft.com/en-us/microsoft-edge/webview2/)

## 主要功能
*   [x] 嵌入式网页展示
*   [x] 基础导航控制（前进、后退、重载）
*   [x] 透明背景支持
*   [x] 自动处理新窗口请求（在当前窗口打开）
*   [x] 强制禁用缓存（通过请求头拦截实现）
*   [x] **运行时自动检测**: 提供静态方法判断系统是否支持 WebView2

## 接口说明 (WebWidget 类)

### 静态方法
*   `static bool isWebView2RuntimeInstalled()`: 推荐在程序启动时调用，检测系统是否安装了必要的运行时。

### 常用方法
*   `void navigate(const QString &url)`: 加载指定 URL。
*   `void goBack()` / `void goForward()`: 历史记录导航。
*   `void refresh()`: 刷新当前页面。
*   `bool canGoBack()` / `bool canGoForward()`: 判断是否可进行导航。

## 使用示例

```cpp
#include "WebWidget.h"
#include <QMessageBox>

// 1. 推荐先检测环境
if (!WebWidget::isWebView2RuntimeInstalled()) {
    QMessageBox::warning(nullptr, "环境缺失", "请先安装 WebView2 Runtime");
    return;
}

// 2. 创建并使用
WebWidget* webView = new WebWidget(parent);
webView->navigate("https://www.example.com");
webView->show();
```

## 构建说明
1.  确保已安装 Qt (推荐 MSVC 版本)。
2.  WebView2 SDK 已内置在 `third_party/webview2` 目录下。
3.  使用 CMake 进行配置和构建。
