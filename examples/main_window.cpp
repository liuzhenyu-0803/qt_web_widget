#include "main_window.h"
#include "web_widget/web_widget.h"
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
    // 创建主布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建工具栏布局
    toolbarLayout = new QHBoxLayout();
    toolbarLayout->setContentsMargins(5, 5, 5, 5);
    toolbarLayout->setSpacing(5);

    // 创建按钮
    backButton = new QPushButton("<", this);
    backButton->setFixedWidth(40);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onGoBack);

    forwardButton = new QPushButton(">", this);
    forwardButton->setFixedWidth(40);
    connect(forwardButton, &QPushButton::clicked, this, &MainWindow::onGoForward);

    refreshButton = new QPushButton("R", this);
    refreshButton->setFixedWidth(40);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefresh);

    // 创建URL输入框
    urlEdit = new QLineEdit(this);
    urlEdit->setText("https://www.bing.com");
    connect(urlEdit, &QLineEdit::returnPressed, this, &MainWindow::onNavigate);

    goButton = new QPushButton("go", this);
    goButton->setFixedWidth(60);
    connect(goButton, &QPushButton::clicked, this, &MainWindow::onNavigate);

    // 添加控件到工具栏
    toolbarLayout->addWidget(backButton);
    toolbarLayout->addWidget(forwardButton);
    toolbarLayout->addWidget(refreshButton);
    toolbarLayout->addWidget(urlEdit);
    toolbarLayout->addWidget(goButton);

    // 创建WebWidget（从DLL导入）
    webWidget = new WebWidget(this);
    webWidget->navigate(urlEdit->text().trimmed());

    // 添加到主布局
    mainLayout->addLayout(toolbarLayout);
    mainLayout->addWidget(webWidget, 1);

    setLayout(mainLayout);
}

void MainWindow::onNavigate()
{
    QString url = urlEdit->text().trimmed();
    if (url.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请输入URL地址");
        return;
    }

    // 如果URL不包含协议，自动添加https://
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
