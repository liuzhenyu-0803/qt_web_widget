#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

namespace WebWidgetLib
{
    class WebWidget;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNavigate();
    void onGoBack();
    void onGoForward();
    void onRefresh();

private:
    void setupUI();
    
    QVBoxLayout *mainLayout;
    QHBoxLayout *toolbarLayout;
    QLineEdit *urlEdit;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *refreshButton;
    QPushButton *goButton;
    WebWidgetLib::WebWidget *webWidget;
};

#endif // MAIN_WINDOW_H
