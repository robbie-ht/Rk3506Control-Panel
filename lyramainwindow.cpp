#include "lyramainwindow.h"
#include "sidebar/sidebar.h"
#include "content/contentarea.h"
#include "widgets/powerbutton.h"
#include "apps/appmanager.h"
#include "apps/appinterface.h"
#include "filemanager/filemanagerapp.h"
#include "sysinfo/homepage.h"
#include "utils/thememanager.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QApplication>
#include <QCoreApplication>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QStyle>

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
    // 使用主题管理器加载主题
    ThemeManager::instance()->applyTheme(ThemeManager::instance()->currentTheme());
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
    m_contentArea->addPage(createSettingsPage());

    // 加载外部插件
    QString pluginsPath = QCoreApplication::applicationDirPath() + "/plugins";
    AppManager::instance()->loadPlugins(pluginsPath);

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

QWidget* LyraMainWindow::createSettingsPage()
{
    QWidget* page = new QWidget();
    page->setStyleSheet("background: transparent;");

    QVBoxLayout* mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(24);

    // 标题
    QLabel* title = new QLabel("系统设置");
    title->setObjectName("settingsTitle");
    title->setStyleSheet("font-size: 22px; font-weight: bold; background: transparent;");
    mainLayout->addWidget(title);

    // 主题选择区域
    QLabel* themeLabel = new QLabel("主题风格");
    themeLabel->setObjectName("settingsSubtitle");
    themeLabel->setStyleSheet("font-size: 15px; color: #8888aa; background: transparent; margin-top: 8px;");
    mainLayout->addWidget(themeLabel);

    QHBoxLayout* themeLayout = new QHBoxLayout();
    themeLayout->setSpacing(16);

    // 深色主题按钮
    QPushButton* btnDark = new QPushButton("深色");
    btnDark->setObjectName("themeButton");
    btnDark->setFixedSize(120, 44);
    btnDark->setCursor(Qt::PointingHandCursor);

    // 浅色主题按钮
    QPushButton* btnLight = new QPushButton("浅色");
    btnLight->setObjectName("themeButton");
    btnLight->setFixedSize(120, 44);
    btnLight->setCursor(Qt::PointingHandCursor);

    // 更新按钮选中状态
    auto updateButtons = [btnDark, btnLight]() {
        QString current = ThemeManager::instance()->currentTheme();
        if (current == "dark") {
            btnDark->setProperty("selected", true);
            btnLight->setProperty("selected", false);
        } else {
            btnDark->setProperty("selected", false);
            btnLight->setProperty("selected", true);
        }
        btnDark->style()->unpolish(btnDark);
        btnDark->style()->polish(btnDark);
        btnLight->style()->unpolish(btnLight);
        btnLight->style()->polish(btnLight);
    };

    connect(btnDark, &QPushButton::clicked, [this, updateButtons]() {
        ThemeManager::instance()->applyTheme("dark");
        updateButtons();
    });

    connect(btnLight, &QPushButton::clicked, [this, updateButtons]() {
        ThemeManager::instance()->applyTheme("light");
        updateButtons();
    });

    themeLayout->addWidget(btnDark);
    themeLayout->addWidget(btnLight);
    themeLayout->addStretch();

    mainLayout->addLayout(themeLayout);

    // 初始状态
    updateButtons();

    // 连接主题变化信号刷新按钮样式
    connect(ThemeManager::instance(), &ThemeManager::themeChanged, [updateButtons](const QString&) {
        updateButtons();
    });

    mainLayout->addStretch();

    return page;
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
