#include "filemanagerapp.h"
#include "filemanagerwidget.h"
#include <QVBoxLayout>

FileManagerApp::FileManagerApp(QObject* parent)
    : QObject(parent)
    , m_widget(nullptr)
{
}

FileManagerApp::~FileManagerApp()
{
}

QString FileManagerApp::appName() const
{
    return "文件管理";
}

QString FileManagerApp::iconPath() const
{
    return "files";
}

QWidget* FileManagerApp::contentWidget()
{
    if (!m_widget) {
        m_widget = new QWidget();
        m_widget->setStyleSheet("background: transparent;");

        QVBoxLayout* layout = new QVBoxLayout(m_widget);
        layout->setContentsMargins(0, 0, 0, 0);

        // 创建文件管理器组件
        FileManagerWidget* fileManager = new FileManagerWidget(m_widget);
        fileManager->initialize();
        layout->addWidget(fileManager);
    }

    return m_widget;
}

void FileManagerApp::initialize()
{
    // 初始化逻辑
}
