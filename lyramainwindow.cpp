#include "lyramainwindow.h"
#include "sidebar/sidebar.h"
#include "content/contentarea.h"
#include "widgets/powerbutton.h"
#include "apps/appmanager.h"
#include "apps/appinterface.h"
#include "filemanager/filemanagerapp.h"
#include "sysinfo/homepage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QApplication>
#include <QLabel>
#include <QDebug>

LyraMainWindow::LyraMainWindow(QWidget* parent)
    : QWidget(parent)
    , m_sidebar(nullptr)
    , m_contentArea(nullptr)
    , m_powerButton(nullptr)
{
}

LyraMainWindow::~LyraMainWindow()
{
}

void LyraMainWindow::initialize()
{
    setupUI();
    loadStyleSheet();
    loadBuiltInApps();
    updatePowerButtonPosition();
}

void LyraMainWindow::setupUI()
{
    setWindowTitle("Lyra Shell");
    setMinimumSize(1024, 600);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_sidebar = new Sidebar(this);
    mainLayout->addWidget(m_sidebar);

    m_contentArea = new ContentArea(this);
    mainLayout->addWidget(m_contentArea, 1);

    m_powerButton = new PowerButton(this);

    connect(m_sidebar, &Sidebar::itemClicked, this, &LyraMainWindow::onSidebarItemClicked);
}

void LyraMainWindow::loadStyleSheet()
{
    QString styleSheet = R"(
        /* 全局样式 */
        QWidget {
            background-color: #0d0d1a;
            color: #e0e0f0;
            font-family: "PingFang SC", "Microsoft YaHei", "Noto Sans CJK SC", sans-serif;
            font-size: 13px;
        }

        /* 侧边栏 */
        #sidebar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #0f0f1e, stop:1 #141428);
            border-right: 1px solid #1a1a3a;
            min-width: 80px;
            max-width: 80px;
        }

        /* 内容区域 */
        #contentArea {
            background-color: #0d0d1a;
        }

        /* 首页 */
        #homePage {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0d0d1a, stop:1 #0a0a14);
        }

        /* 电源按钮 */
        #powerButton {
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.4,
                stop:0 #2a2a4a, stop:0.8 #1e1e3a, stop:1 #161630);
            border: 1.5px solid #3a3a5a;
            border-radius: 24px;
        }

        #powerButton:hover {
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.4,
                stop:0 #353560, stop:0.8 #282850, stop:1 #1e1e40);
            border: 1.5px solid #4a4a7a;
        }

        #powerButton:pressed {
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.4,
                stop:0 #1e1e3a, stop:0.8 #161630, stop:1 #0e0e20);
            border: 1.5px solid #2a2a4a;
        }

        /* 滚动条 */
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical {
            background: rgba(100, 100, 150, 0.3);
            min-height: 40px;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical:hover {
            background: rgba(100, 100, 150, 0.5);
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }

        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: transparent;
        }

        /* 文件管理器样式 */
        #toolbar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #141428, stop:1 #0f0f1e);
            border-bottom: 1px solid #1a1a3a;
        }

        #navButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e1e3a, stop:1 #161630);
            border: 1px solid #2a2a4a;
            border-radius: 6px;
            padding: 6px;
            font-size: 16px;
        }

        #navButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #282850, stop:1 #1e1e40);
            border: 1px solid #3a3a5a;
        }

        #navButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #161630, stop:1 #0f0f20);
        }

        #navButton:disabled {
            background: #0f0f1e;
            color: #444466;
            border: 1px solid #1a1a3a;
        }

        #pathLabel {
            color: #8888bb;
            padding: 4px 12px;
        }

        /* 文件列表 */
        QTreeView, QListView {
            background: #0d0d1a;
            alternate-background-color: #10102a;
            border: none;
            outline: none;
        }

        QTreeView::item, QListView::item {
            padding: 8px;
            border-bottom: 1px solid #1a1a3a;
        }

        QTreeView::item:selected, QListView::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3a3a7a, stop:1 #2a2a6a);
            color: #ffffff;
        }

        QTreeView::item:hover, QListView::item:hover {
            background: #1a1a3a;
        }

        QHeaderView::section {
            background: #0f0f1e;
            color: #8888aa;
            padding: 10px;
            border: none;
            border-right: 1px solid #1a1a3a;
            border-bottom: 1px solid #1a1a3a;
        }

        /* 面板路径标签 */
        #panelPathLabel {
            background: #10102a;
            color: #8888bb;
            padding: 2px 8px;
            font-size: 12px;
            border-bottom: 1px solid #1a1a3a;
        }

        /* 右键菜单 */
        QMenu {
            background: #141428;
            border: 1px solid #2a2a4a;
            border-radius: 6px;
            padding: 4px;
        }

        QMenu::item {
            padding: 8px 24px;
            border-radius: 4px;
            color: #e0e0f0;
        }

        QMenu::item:selected {
            background: #3a3a7a;
            color: #ffffff;
        }

        QMenu::separator {
            height: 1px;
            background: #2a2a4a;
            margin: 4px 8px;
        }

        /* 状态栏 */
        #statusbar {
            background: #0f0f1e;
            border-top: 1px solid #1a1a3a;
        }

        #statusPath {
            color: #666688;
        }

        #statusCount {
            color: #555577;
        }

        #statusMessage {
            color: #6a6aaa;
        }

        /* 对话框 */
        QDialog {
            background: #141428;
        }

        QLabel {
            color: #e0e0f0;
        }

        QLineEdit {
            background: #0f0f1e;
            border: 1px solid #2a2a4a;
            border-radius: 8px;
            padding: 10px;
            color: #e0e0f0;
        }

        QLineEdit:focus {
            border: 1px solid #4a4a8a;
        }

        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e1e3a, stop:1 #161630);
            border: 1px solid #2a2a4a;
            border-radius: 8px;
            padding: 10px 20px;
            color: #e0e0f0;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #282850, stop:1 #1e1e40);
            border: 1px solid #3a3a5a;
        }

        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #161630, stop:1 #0f0f20);
        }

        QPushButton:disabled {
            background: #0f0f1e;
            color: #444466;
            border: 1px solid #1a1a3a;
        }

        #deleteButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #8b2252, stop:1 #6b1242);
            border: 1px solid #ab3262;
        }

        #deleteButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #9b3262, stop:1 #7b2252);
        }
    )";

    qApp->setStyleSheet(styleSheet);
}

void LyraMainWindow::loadBuiltInApps()
{
    // 添加内置App
    m_sidebar->addItem("主页", "home");
    m_sidebar->addItem("设置", "settings");

    // 创建主页内容（带系统信息卡片）
    HomePage* homePage = new HomePage();
    homePage->initialize();
    m_contentArea->addPage(homePage);

    // 创建设置页面
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

    // 注册文件管理器App
    FileManagerApp* fileManagerApp = new FileManagerApp(this);
    AppManager::instance()->registerApp(fileManagerApp);

    // 将注册的App添加到侧边栏和内容区
    QList<AppInterface*> plugins = AppManager::instance()->apps();
    for (AppInterface* app : plugins) {
        m_sidebar->addItem(app->appName(), app->iconPath());
        m_contentArea->addPage(app->contentWidget());
    }

    // 默认选中第一项
    m_sidebar->setCurrentItem(0);
    m_contentArea->setCurrentPage(0);
}

void LyraMainWindow::onSidebarItemClicked(int index)
{
    m_contentArea->setCurrentPage(index);
}

void LyraMainWindow::updatePowerButtonPosition()
{
    if (m_powerButton) {
        int x = width() - m_powerButton->width() - 24;
        int y = height() - m_powerButton->height() - 24;
        m_powerButton->move(x, y);
    }
}

void LyraMainWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(10, 10, 10));
}

void LyraMainWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updatePowerButtonPosition();
}
