#ifndef WEB_WIDGET_H
#define WEB_WIDGET_H

#include "web_widget_dll_export.h"
#include <QWidget>

namespace WebWidgetLib
{

    // 前置声明，避免在头文件中包含 WebView2.h 和 wrl.h
    struct WebWidgetPrivate;

    class WEB_WIDGET_DLL_API WebWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit WebWidget(QWidget *parent = nullptr);
        ~WebWidget();

        /**
         * @brief 检查系统是否已安装 Microsoft Edge WebView2 Runtime
         * @return 如果已安装返回 true，否则返回 false
         */
        static bool isWebView2RuntimeInstalled();

        void navigate(const QString &url);
        void goBack();
        void goForward();
        void refresh();
        bool canGoBack() const;
        bool canGoForward() const;

    protected:
        void resizeEvent(QResizeEvent *event) override;
        void showEvent(QShowEvent *event) override;

    private:
        void initializeWebView();
        void updateWebViewBounds();
        void cleanupUserDataFolder(const QString &folderPath);

        WebWidgetPrivate *d; // PIMPL 模式中的 d-pointer
    };

} // namespace WebWidgetLib

#endif // WEB_WIDGET_H
