#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

class WebWidget;

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
    WebWidget *webWidget;
};

#endif // MAINWINDOW_H
