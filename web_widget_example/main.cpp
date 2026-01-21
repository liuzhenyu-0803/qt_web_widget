#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    
    MainWindow window;
    window.setWindowTitle("WebWidget DLL 示例");
    window.resize(1024, 768);
    window.show();
    
    return app.exec();
}
