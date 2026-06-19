#include "homepage.h"
#include "sysinfocard.h"
#include "sysinfomanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPainter>
#include <QDateTime>
#include <QStyleOption>
#include <QFile>
#include <QTextStream>
#include <QGraphicsDropShadowEffect>

HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{
}

HomePage::~HomePage()
{
}

void HomePage::initialize()
{
    setupUI();

    // 系统信息管理器
    m_sysInfoManager = new SysInfoManager(this);
    connect(m_sysInfoManager, &SysInfoManager::infoUpdated, this, &HomePage::updateSysInfo);
    m_sysInfoManager->startMonitoring();

    // 时间定时器
    m_timeTimer = new QTimer(this);
    connect(m_timeTimer, &QTimer::timeout, this, &HomePage::updateTime);
    m_timeTimer->start(1000);
    updateTime();
}

void HomePage::setupUI()
{
    setObjectName("homePage");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 20);
    mainLayout->setSpacing(20);

    // 顶部区域：左侧时间，右侧欢迎信息
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(40);

    // 左侧：时间显示
    QWidget* timeWidget = new QWidget(this);
    timeWidget->setObjectName("timeWidget");
    timeWidget->setStyleSheet(R"(
        #timeWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1a1a2e, stop:1 #16213e);
            border-radius: 16px;
            border: 1px solid #2a2a4a;
        }
    )");

    QVBoxLayout* timeLayout = new QVBoxLayout(timeWidget);
    timeLayout->setContentsMargins(30, 20, 30, 20);
    timeLayout->setSpacing(4);

    m_timeLabel = new QLabel(timeWidget);
    m_timeLabel->setStyleSheet(R"(
        font-size: 56px;
        color: #ffffff;
        font-weight: bold;
        font-family: "Noto Sans Mono", monospace;
        background: transparent;
        letter-spacing: 2px;
    )");
    timeLayout->addWidget(m_timeLabel);

    m_dateLabel = new QLabel(timeWidget);
    m_dateLabel->setStyleSheet(R"(
        font-size: 16px;
        color: #8888aa;
        background: transparent;
    )");
    timeLayout->addWidget(m_dateLabel);

    timeWidget->setFixedWidth(320);
    topLayout->addWidget(timeWidget);

    // 右侧：欢迎信息和快捷操作
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(12);

    m_welcomeLabel = new QLabel("欢迎使用 Lyra Shell", this);
    m_welcomeLabel->setStyleSheet(R"(
        font-size: 28px;
        color: #ffffff;
        font-weight: bold;
        background: transparent;
    )");
    rightLayout->addWidget(m_welcomeLabel);

    QLabel* deviceInfo = new QLabel("幸狐 RK3506 Lyra Pi W · 1024×600", this);
    deviceInfo->setStyleSheet("font-size: 14px; color: #666688; background: transparent;");
    rightLayout->addWidget(deviceInfo);

    rightLayout->addStretch();
    topLayout->addLayout(rightLayout, 1);

    mainLayout->addLayout(topLayout);

    // 系统信息卡片区域
    QLabel* sectionTitle = new QLabel("系统状态", this);
    sectionTitle->setStyleSheet(R"(
        font-size: 16px;
        color: #8888aa;
        background: transparent;
        margin-top: 5px;
    )");
    mainLayout->addWidget(sectionTitle);

    QGridLayout* cardsLayout = new QGridLayout();
    cardsLayout->setSpacing(15);

    // CPU温度卡片
    m_cpuTempCard = new SysInfoCard("🌡️", "CPU 温度", this);
    cardsLayout->addWidget(m_cpuTempCard, 0, 0);

    // CPU使用率卡片
    m_cpuUsageCard = new SysInfoCard("⚡", "CPU 使用率", this);
    cardsLayout->addWidget(m_cpuUsageCard, 0, 1);

    // 内存使用卡片
    m_memoryCard = new SysInfoCard("💾", "内存使用", this);
    cardsLayout->addWidget(m_memoryCard, 0, 2);

    // 运行时间卡片
    m_timeCard = new SysInfoCard("⏱️", "系统运行", this);
    cardsLayout->addWidget(m_timeCard, 0, 3);

    mainLayout->addLayout(cardsLayout);

    // 底部状态栏
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(0, 10, 0, 0);

    QLabel* versionLabel = new QLabel("v1.0.0", this);
    versionLabel->setStyleSheet("font-size: 12px; color: #444466; background: transparent;");
    bottomLayout->addWidget(versionLabel);

    bottomLayout->addStretch();

    QLabel* copyrightLabel = new QLabel("© 2024 Lyra Shell", this);
    copyrightLabel->setStyleSheet("font-size: 12px; color: #444466; background: transparent;");
    bottomLayout->addWidget(copyrightLabel);

    mainLayout->addLayout(bottomLayout);
}

void HomePage::updateTime()
{
    QDateTime now = QDateTime::currentDateTime();
    m_timeLabel->setText(now.toString("HH:mm:ss"));
    m_dateLabel->setText(now.toString("yyyy年MM月dd日 dddd"));
}

void HomePage::updateSysInfo()
{
    // 更新CPU温度
    m_cpuTempCard->setValue(m_sysInfoManager->cpuTemperature());

    // 更新CPU使用率
    m_cpuUsageCard->setValue(m_sysInfoManager->cpuUsage());
    m_cpuUsageCard->setSubtitle(QString("使用率 %1%").arg(m_sysInfoManager->cpuPercent()));

    // 更新内存使用
    m_memoryCard->setValue(m_sysInfoManager->memoryUsage());
    m_memoryCard->setSubtitle(QString("使用率 %1%").arg(m_sysInfoManager->memoryPercent()));

    // 更新运行时间
    QFile uptimeFile("/proc/uptime");
    if (uptimeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&uptimeFile);
        QString line = in.readLine();
        double uptime = line.split(" ")[0].toDouble();

        int days = (int)(uptime / 86400);
        int hours = ((int)uptime % 86400) / 3600;
        int minutes = ((int)uptime % 3600) / 60;

        QString uptimeStr;
        if (days > 0) {
            uptimeStr = QString("%1天 %2时").arg(days).arg(hours);
        } else if (hours > 0) {
            uptimeStr = QString("%1时 %2分").arg(hours).arg(minutes);
        } else {
            uptimeStr = QString("%1分钟").arg(minutes);
        }

        m_timeCard->setValue(uptimeStr);
    }
}

void HomePage::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
