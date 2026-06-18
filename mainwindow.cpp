#include "mainwindow.h"
#include "sidebar/sidebar.h"
#include "content/contentarea.h"
#include "widgets/powerbutton.h"
#include "apps/appmanager.h"
#include "apps/appinterface.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QApplication>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
    , m_sidebar(nullptr)
    , m_contentArea(nullptr)
    , m_powerButton(nullptr)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::initialize()
{
    setupUI();
    loadStyleSheet();
    loadBuiltInApps();
    updatePowerButtonPosition();
}

void MainWindow::setupUI()
{
    setWindowTitle("Lyra Shell");
    setMinimumSize(1024, 600);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 侧边栏
    m_sidebar = new Sidebar(this);
    mainLayout->addWidget(m_sidebar);

    // 内容区域
    m_contentArea = new ContentArea(this);
    mainLayout->addWidget(m_contentArea, 1);

    // 电源按钮（浮动）
    m_powerButton = new PowerButton(this);

    connect(m_sidebar, &Sidebar::itemClicked, this, &MainWindow::onSidebarItemClicked);
}

void MainWindow::loadStyleSheet()
{
    QString styleSheet = R"(
        /* 全局样式 */
        QWidget {
            background-color: #0a0a0a;
            color: #e8e8e8;
            font-family: "PingFang SC", "Microsoft YaHei", "Noto Sans CJK SC", sans-serif;
            font-size: 13px;
        }

        /* 侧边栏 */
        #sidebar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #141414, stop:1 #1a1a1a);
            border-right: 1px solid #222222;
        }

        /* 内容区域 */
        #contentArea {
            background-color: #0f0f0f;
        }

        /* 电源按钮 */
        #powerButton {
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.4,
                stop:0 #2a2a2a, stop:0.8 #1e1e1e, stop:1 #161616);
            border: 1.5px solid #333333;
            border-radius: 26px;
        }

        #powerButton:hover {
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.4,
                stop:0 #353535, stop:0.8 #282828, stop:1 #1e1e1e);
            border: 1.5px solid #444444;
        }

        #powerButton:pressed {
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.4,
                stop:0 #1e1e1e, stop:0.8 #161616, stop:1 #0e0e0e);
            border: 1.5px solid #2a2a2a;
        }

        /* 滚动条 */
        QScrollBar:vertical {
            background: transparent;
            width: 6px;
            margin: 0;
            border-radius: 3px;
        }

        QScrollBar::handle:vertical {
            background: rgba(255, 255, 255, 0.15);
            min-height: 30px;
            border-radius: 3px;
        }

        QScrollBar::handle:vertical:hover {
            background: rgba(255, 255, 255, 0.25);
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0;
        }

        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: transparent;
        }
    )";

    qApp->setStyleSheet(styleSheet);
}

void MainWindow::loadBuiltInApps()
{
    // 添加侧边栏项目

    // 创建示例页面
    QWidget* homePage = new QWidget();
    homePage->setStyleSheet("background: transparent;");
    QVBoxLayout* homeLayout = new QVBoxLayout(homePage);
    homeLayout->setAlignment(Qt::AlignCenter);

    QLabel* homeIcon = new QLabel("🏠");
    homeIcon->setStyleSheet("font-size: 48px; background: transparent;");
    homeIcon->setAlignment(Qt::AlignCenter);
    homeLayout->addWidget(homeIcon);

    QLabel* homeTitle = new QLabel("欢迎使用 Lyra Shell");
    homeTitle->setStyleSheet("font-size: 24px; color: #e8e8e8; background: transparent; margin-top: 16px;");
    homeTitle->setAlignment(Qt::AlignCenter);
    homeLayout->addWidget(homeTitle);

    QLabel* homeSubtitle = new QLabel("幸狐 RK3606 Lyra Pi W");
    homeSubtitle->setStyleSheet("font-size: 14px; color: #888888; background: transparent;");
    homeSubtitle->setAlignment(Qt::AlignCenter);
    homeLayout->addWidget(homeSubtitle);

    m_contentArea->addPage(homePage);

    // 设置页面
    QWidget* settingsPage = new QWidget();
    settingsPage->setStyleSheet("background: transparent;");
    QVBoxLayout* settingsLayout = new QVBoxLayout(settingsPage);
    settingsLayout->setAlignment(Qt::AlignCenter);

    QLabel* settingsIcon = new QLabel("⚙️");
    settingsIcon->setStyleSheet("font-size: 48px; background: transparent;");
    settingsIcon->setAlignment(Qt::AlignCenter);
    settingsLayout->addWidget(settingsIcon);

    QLabel* settingsTitle = new QLabel("系统设置");
    settingsTitle->setStyleSheet("font-size: 24px; color: #e8e8e8; background: transparent; margin-top: 16px;");
    settingsTitle->setAlignment(Qt::AlignCenter);
    settingsLayout->addWidget(settingsTitle);

    m_contentArea->addPage(settingsPage);

    // 文件管理页面
    QWidget* filesPage = new QWidget();
    filesPage->setStyleSheet("background: transparent;");
    QVBoxLayout* filesLayout = new QVBoxLayout(filesPage);
    filesLayout->setAlignment(Qt::AlignCenter);

    QLabel* filesIcon = new QLabel("📁");
    filesIcon->setStyleSheet("font-size: 48px; background: transparent;");
    filesIcon->setAlignment(Qt::AlignCenter);
    filesLayout->addWidget(filesIcon);

    QLabel* filesTitle = new QLabel("文件管理");
    filesTitle->setStyleSheet("font-size: 24px; color: #e8e8e8; background: transparent; margin-top: 16px;");
    filesTitle->setAlignment(Qt::AlignCenter);
    filesLayout->addWidget(filesTitle);

    m_contentArea->addPage(filesPage);

    // 添加侧边栏项目
    m_sidebar->addItem("主页", "home");
    m_sidebar->addItem("设置", "settings");
    m_sidebar->addItem("文件", "files");

    // 加载插件
    QString pluginsPath = QCoreApplication::applicationDirPath() + "/plugins";
    qDebug() << "Loading plugins from:" << pluginsPath;
    AppManager::instance()->loadPlugins(pluginsPath);
    qDebug() << "Loaded" << AppManager::instance()->count() << "plugins";

    // 将插件添加到侧边栏
    QList<AppInterface*> plugins = AppManager::instance()->apps();
    for (AppInterface* app : plugins) {
        m_sidebar->addItem(app->appName(), app->iconPath());
        m_contentArea->addPage(app->contentWidget());
    }

    // 默认选中
    m_sidebar->setCurrentItem(0);
    m_contentArea->setCurrentPage(0);
}

void MainWindow::onSidebarItemClicked(int index)
{
    m_contentArea->setCurrentPage(index);
}

void MainWindow::updatePowerButtonPosition()
{
    if (m_powerButton) {
        int x = width() - m_powerButton->width() - 24;
        int y = height() - m_powerButton->height() - 24;
        m_powerButton->move(x, y);
    }
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(10, 10, 10));
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updatePowerButtonPosition();
}
