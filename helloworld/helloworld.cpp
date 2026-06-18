#include "helloworld.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

HelloWorld::HelloWorld(QObject* parent)
    : QObject(parent)
    , m_widget(nullptr)
{
}

HelloWorld::~HelloWorld()
{
    if (m_widget) {
        delete m_widget;
    }
}

QString HelloWorld::appName() const
{
    return "Hello World";
}

QString HelloWorld::iconPath() const
{
    return "hello";  // 图标类型名称
}

QWidget* HelloWorld::contentWidget()
{
    if (!m_widget) {
        m_widget = new QWidget();
        m_widget->setStyleSheet("background: transparent;");

        QVBoxLayout* layout = new QVBoxLayout(m_widget);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(20);

        // 图标
        QLabel* icon = new QLabel("👋");
        icon->setStyleSheet("font-size: 64px; background: transparent;");
        icon->setAlignment(Qt::AlignCenter);
        layout->addWidget(icon);

        // 标题
        QLabel* title = new QLabel("Hello World!");
        title->setStyleSheet("font-size: 32px; color: #e8e8e8; background: transparent; font-weight: bold;");
        title->setAlignment(Qt::AlignCenter);
        layout->addWidget(title);

        // 副标题
        QLabel* subtitle = new QLabel("这是一个示例插件");
        subtitle->setStyleSheet("font-size: 16px; color: #888888; background: transparent;");
        subtitle->setAlignment(Qt::AlignCenter);
        layout->addWidget(subtitle);

        // 按钮
        QPushButton* btn = new QPushButton("点击我");
        btn->setFixedSize(160, 44);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #3a3a3a, stop:1 #2a2a2a);
                border: 1px solid #444444;
                border-radius: 8px;
                color: #e8e8e8;
                font-size: 14px;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #444444, stop:1 #333333);
                border: 1px solid #555555;
            }
            QPushButton:pressed {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2a2a2a, stop:1 #1e1e1e);
            }
        )");

        connect(btn, &QPushButton::clicked, []() {
            QMessageBox::information(nullptr, "Hello", "你好！这是 Lyra Shell 插件系统！");
        });

        layout->addWidget(btn, 0, Qt::AlignCenter);
    }

    return m_widget;
}

void HelloWorld::initialize()
{
    // 初始化逻辑
}
