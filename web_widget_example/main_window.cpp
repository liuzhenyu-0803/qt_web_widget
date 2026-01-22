#include "main_window.h"
#include "web_widget.h"
#include <QMessageBox>

using WebWidgetLib::WebWidget;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , mainLayout(nullptr)
    , toolbarLayout(nullptr)
    , urlEdit(nullptr)
    , backButton(nullptr)
    , forwardButton(nullptr)
    , refreshButton(nullptr)
    , goButton(nullptr)
    , webWidget(nullptr)
{
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // 鍒涘缓涓诲竷灞€
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 鍒涘缓宸ュ叿鏍忓竷灞€
    toolbarLayout = new QHBoxLayout();
    toolbarLayout->setContentsMargins(5, 5, 5, 5);
    toolbarLayout->setSpacing(5);

    // 鍒涘缓鎸夐挳
    backButton = new QPushButton("<", this);
    backButton->setFixedWidth(40);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onGoBack);

    forwardButton = new QPushButton(">", this);
    forwardButton->setFixedWidth(40);
    connect(forwardButton, &QPushButton::clicked, this, &MainWindow::onGoForward);

    refreshButton = new QPushButton("R", this);
    refreshButton->setFixedWidth(40);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefresh);

    // 鍒涘缓URL杈撳叆妗?
    urlEdit = new QLineEdit(this);
    urlEdit->setText("https://www.bing.com");
    connect(urlEdit, &QLineEdit::returnPressed, this, &MainWindow::onNavigate);

    goButton = new QPushButton("鍓嶅線", this);
    goButton->setFixedWidth(60);
    connect(goButton, &QPushButton::clicked, this, &MainWindow::onNavigate);

    // 娣诲姞鎺т欢鍒板伐鍏锋爮
    toolbarLayout->addWidget(backButton);
    toolbarLayout->addWidget(forwardButton);
    toolbarLayout->addWidget(refreshButton);
    toolbarLayout->addWidget(urlEdit);
    toolbarLayout->addWidget(goButton);

    // 鍒涘缓WebWidget锛堜粠DLL瀵煎叆锛?
    webWidget = new WebWidget(this);
    webWidget->navigate(urlEdit->text().trimmed());

    // 娣诲姞鍒颁富甯冨眬
    mainLayout->addLayout(toolbarLayout);
    mainLayout->addWidget(webWidget, 1);

    setLayout(mainLayout);
}

void MainWindow::onNavigate()
{
    QString url = urlEdit->text().trimmed();
    if (url.isEmpty())
    {
        QMessageBox::warning(this, "璀﹀憡", "璇疯緭鍏RL鍦板潃");
        return;
    }

    // 濡傛灉URL涓嶅寘鍚崗璁紝鑷姩娣诲姞https://
    if (!url.startsWith("http://") && !url.startsWith("https://"))
    {
        url = "https://" + url;
        urlEdit->setText(url);
    }

    webWidget->navigate(url);
}

void MainWindow::onGoBack()
{
    if (webWidget->canGoBack())
    {
        webWidget->goBack();
    }
}

void MainWindow::onGoForward()
{
    if (webWidget->canGoForward())
    {
        webWidget->goForward();
    }
}

void MainWindow::onRefresh()
{
    webWidget->refresh();
}
