#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序属性
    app.setApplicationName("Lyra Shell");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Lyra");

    // 创建并初始化主窗口
    MainWindow window;
    window.initialize();

    // 全屏显示（适合嵌入式设备）
    window.showFullScreen();

    return app.exec();
}
