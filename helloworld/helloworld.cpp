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
}

QString HelloWorld::appName() const
{
    return "Hello World";
}

QString HelloWorld::iconPath() const
{
    return "hello";
}

QWidget* HelloWorld::contentWidget()
{
    if (!m_widget) {
        m_widget = new QWidget();
        m_widget->setStyleSheet("background: transparent;");

        QVBoxLayout* layout = new QVBoxLayout(m_widget);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(20);

        QLabel* icon = new QLabel("👋");
        icon->setStyleSheet("font-size: 64px; background: transparent;");
        icon->setAlignment(Qt::AlignCenter);
        layout->addWidget(icon);

        QLabel* title = new QLabel("Hello World!");
        title->setStyleSheet("font-size: 32px; color: #e8e8e8; background: transparent; font-weight: bold;");
        title->setAlignment(Qt::AlignCenter);
        layout->addWidget(title);

        QLabel* subtitle = new QLabel("这是一个示例插件");
        subtitle->setStyleSheet("font-size: 16px; color: #888888; background: transparent;");
        subtitle->setAlignment(Qt::AlignCenter);
        layout->addWidget(subtitle);

        QPushButton* btn = new QPushButton("点击我");
        btn->setFixedSize(160, 44);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #3a3a7a, stop:1 #2a2a6a);
                border: 1px solid #4a4a8a;
                border-radius: 8px;
                color: #e0e0f0;
                font-size: 14px;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #4a4a8a, stop:1 #3a3a7a);
            }
            QPushButton:pressed {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2a2a6a, stop:1 #1a1a5a);
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
}
