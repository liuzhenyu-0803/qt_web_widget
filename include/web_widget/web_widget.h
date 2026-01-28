#ifndef WEB_WIDGET_H
#define WEB_WIDGET_H

#include "web_widget_export.h"
#include <QWidget>

namespace WebWidgetLib
{

    class WEB_WIDGET_API WebWidget : public QWidget
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
        struct WebWidgetPrivate *d; // PIMPL 模式中的 d-pointer
    };

} // namespace WebWidgetLib

#endif // WEB_WIDGET_H
