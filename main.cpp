#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include "lyramainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序属性
    app.setApplicationName("Lyra Shell");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Lyra");

    // 加载中文字体
    int fontIndex = QFontDatabase::addApplicationFont(":/fonts/wqy-zenhei.ttc");
    if (fontIndex < 0) {
        // 如果资源文件加载失败，尝试从系统路径加载
        fontIndex = QFontDatabase::addApplicationFont("/usr/lib/fonts/wqy-zenhei/wqy-zenhei.ttc");
    }
    if (fontIndex >= 0) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontIndex);
        if (!fontFamilies.isEmpty()) {
            QFont font(fontFamilies.at(0));
            font.setPixelSize(14);
            app.setFont(font);
        }
    }

    // 创建并初始化主窗口
    LyraMainWindow window;
    window.initialize();

    // 全屏显示（适合嵌入式设备）
    window.showFullScreen();

    return app.exec();
}
