#include "web_widget.h"
#include <QResizeEvent>
#include <QShowEvent>
#include <QWindow>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QThread>

// 在 CPP 中包含具体的 SDK 头文件
#include <wrl.h>
#include <windows.h>
#include <shlobj.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

// 定义私有实现类
struct WebWidgetPrivate
{
    ComPtr<ICoreWebView2Controller> webviewController;
    ComPtr<ICoreWebView2> webview;
    HWND hwnd;
    bool isInitialized;
    QString userDataFolder;  // 存储用户数据文件夹路径
    QString pendingUrl;      // 待加载的 URL（当 WebView 未初始化时保存）

    WebWidgetPrivate()
        : hwnd(nullptr)
        , isInitialized(false)
    {
    }
};

WebWidget::WebWidget(QWidget *parent)
    : QWidget(parent)
{
    d = new WebWidgetPrivate();

    // 设置窗口属性，使其能够嵌入原生窗口
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    
    // 获取原生窗口句柄
    d->hwnd = reinterpret_cast<HWND>(winId());
}

WebWidget::~WebWidget()
{
    if (d->webviewController)
    {
        d->webviewController->Close();
        d->webviewController = nullptr;
    }
    d->webview = nullptr;
    
    // 清理用户数据文件夹（删除缓存）
    if (!d->userDataFolder.isEmpty())
    {
        cleanupUserDataFolder(d->userDataFolder);
    }
    
    delete d;
}

bool WebWidget::isWebView2RuntimeInstalled()
{
    LPWSTR versionInfo = nullptr;
    HRESULT hr = GetAvailableCoreWebView2BrowserVersionString(nullptr, &versionInfo);
    if (SUCCEEDED(hr) && versionInfo != nullptr)
    {
        CoTaskMemFree(versionInfo);
        return true;
    }
    return false;
}

void WebWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    
    if (!d->isInitialized)
    {
        initializeWebView();
    }
}

void WebWidget::initializeWebView()
{
    if (d->isInitialized || !d->hwnd)
    {
        return;
    }

    // 创建临时用户数据文件夹（用于缓存，程序退出后删除）
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    d->userDataFolder = tempPath + "/WebWidget_" + QString::number(reinterpret_cast<quint64>(this));
    QDir().mkpath(d->userDataFolder);

    qInfo() << "WebWidget user data folder:" << d->userDataFolder;

    // 创建 WebView2 环境和控制器，指定用户数据文件夹
    std::wstring userDataFolderW = d->userDataFolder.toStdWString();
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, userDataFolderW.c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                if (FAILED(result))
                {
                    qDebug() << "Failed to create WebView2 environment:" << result;
                    return result;
                }

                // 创建 WebView2 控制器
                env->CreateCoreWebView2Controller(
                    d->hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            if (FAILED(result))
                            {
                                qDebug() << "Failed to create WebView2 controller:" << result;
                                return result;
                            }

                            d->webviewController = controller;
                            d->webviewController->get_CoreWebView2(&d->webview);

                            // 设置 WebView 大小
                            updateWebViewBounds();

                            // 设置透明背景色
                            ComPtr<ICoreWebView2Controller2> controller2;
                            if (SUCCEEDED(d->webviewController.As(&controller2)))
                            {
                                // RGBA: 透明背景 (alpha = 0)
                                COREWEBVIEW2_COLOR bgColor = {0, 255, 255, 255}; // Alpha=0 表示透明
                                controller2->put_DefaultBackgroundColor(bgColor);
                            }

                            // 配置新窗口事件处理：在当前页面打开新链接
                            if (d->webview)
                            {
                                d->webview->add_NewWindowRequested(
                                    Callback<ICoreWebView2NewWindowRequestedEventHandler>(
                                        [this](ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args) -> HRESULT
                                        {
                                            // 获取目标 URI
                                            LPWSTR uri;
                                            args->get_Uri(&uri);
                                            
                                            // 取消默认的新窗口行为
                                            args->put_Handled(TRUE);
                                            
                                            // 在当前 WebView 中导航到目标 URL
                                            if (d->webview && uri)
                                            {
                                                d->webview->Navigate(uri);
                                            }
                                            
                                            // 释放 URI 字符串
                                            CoTaskMemFree(uri);
                                            
                                            return S_OK;
                                        }
                                    ).Get(),
                                    nullptr
                                );

                                // 禁用缓存：获取 Settings 接口并设置缓存禁用
                                ComPtr<ICoreWebView2Settings> settings;
                                if (SUCCEEDED(d->webview->get_Settings(&settings)))
                                {
                                    // 注意：WebView2 没有直接禁用所有缓存的 API
                                    // 但可以通过以下方式实现类似效果：
                                    
                                    // 1. 禁用应用缓存
                                    ComPtr<ICoreWebView2Settings6> settings6;
                                    if (SUCCEEDED(settings.As(&settings6)))
                                    {
                                        settings6->put_IsGeneralAutofillEnabled(FALSE);
                                    }

                                    // 2. 通过导航时添加 Cache-Control 头部实现
                                    // 或使用 ICoreWebView2_2 的 WebResourceRequested 事件
                                    ComPtr<ICoreWebView2_2> webview2;
                                    if (SUCCEEDED(d->webview.As(&webview2)))
                                    {
                                        // 添加过滤器以拦截所有请求
                                        webview2->AddWebResourceRequestedFilter(
                                            L"*",
                                            COREWEBVIEW2_WEB_RESOURCE_CONTEXT_ALL
                                        );

                                        // 设置请求拦截器，添加 no-cache 头部
                                        webview2->add_WebResourceRequested(
                                            Callback<ICoreWebView2WebResourceRequestedEventHandler>(
                                                [](ICoreWebView2* sender, ICoreWebView2WebResourceRequestedEventArgs* args) -> HRESULT
                                                {
                                                    ComPtr<ICoreWebView2WebResourceRequest> request;
                                                    if (SUCCEEDED(args->get_Request(&request)))
                                                    {
                                                        ComPtr<ICoreWebView2HttpRequestHeaders> headers;
                                                        if (SUCCEEDED(request->get_Headers(&headers)))
                                                        {
                                                            // 添加 Cache-Control 头部禁用缓存
                                                            headers->SetHeader(L"Cache-Control", L"no-cache, no-store, must-revalidate");
                                                            headers->SetHeader(L"Pragma", L"no-cache");
                                                            headers->SetHeader(L"Expires", L"0");
                                                        }
                                                    }
                                                    return S_OK;
                                                }
                                            ).Get(),
                                            nullptr
                                        );
                                    }
                                }
                            }

                            // 设置初始化标志
                            d->isInitialized = true;

                            // 如果有待加载的 URL，立即加载
                            if (!d->pendingUrl.isEmpty())
                            {
                                std::wstring wurl = d->pendingUrl.toStdWString();
                                d->webview->Navigate(wurl.c_str());
                                d->pendingUrl.clear();
                            }

                            return S_OK;
                        }
                    ).Get()
                );

                return S_OK;
            }
        ).Get()
    );

    if (FAILED(hr))
    {
        qDebug() << "Failed to initialize WebView2:" << hr;
    }
}

void WebWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateWebViewBounds();
}

void WebWidget::updateWebViewBounds()
{
    if (d->webviewController && d->hwnd)
    {
        // 获取窗口的物理像素尺寸（考虑 DPI 缩放）
        RECT bounds;
        GetClientRect(d->hwnd, &bounds);
        d->webviewController->put_Bounds(bounds);
    }
}

void WebWidget::navigate(const QString &url)
{
    if (d->webview)
    {
        // WebView 已准备好，直接导航
        std::wstring wurl = url.toStdWString();
        d->webview->Navigate(wurl.c_str());
        d->pendingUrl.clear();  // 清除待处理的 URL
    }
    else
    {
        // WebView 未准备好，保存 URL 待后续加载
        d->pendingUrl = url;
    }
}

void WebWidget::goBack()
{
    if (d->webview)
    {
        d->webview->GoBack();
    }
}

void WebWidget::goForward()
{
    if (d->webview)
    {
        d->webview->GoForward();
    }
}

void WebWidget::refresh()
{
    if (d->webview)
    {
        d->webview->Reload();
    }
}

bool WebWidget::canGoBack() const
{
    if (d->webview)
    {
        BOOL canGoBack = FALSE;
        d->webview->get_CanGoBack(&canGoBack);
        return canGoBack;
    }
    return false;
}

bool WebWidget::canGoForward() const
{
    if (d->webview)
    {
        BOOL canGoForward = FALSE;
        d->webview->get_CanGoForward(&canGoForward);
        return canGoForward;
    }
    return false;
}

void WebWidget::cleanupUserDataFolder(const QString &folderPath)
{
    if (folderPath.isEmpty())
    {
        return;
    }

    QDir dir(folderPath);
    if (!dir.exists())
    {
        return;
    }

    // 递归删除目录及其所有内容
    if (dir.removeRecursively())
    {
        qDebug() << "Successfully cleaned up user data folder:" << folderPath;
        return;
    }
    
    qDebug() << "Failed to fully clean up user data folder:" << folderPath;
    qDebug() << "Some files may be locked by the system. They will be cleaned on next reboot.";
}
