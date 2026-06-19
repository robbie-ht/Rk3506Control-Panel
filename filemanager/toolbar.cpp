#include "toolbar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>

ToolBar::ToolBar(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

ToolBar::~ToolBar()
{
}

void ToolBar::setupUI()
{
    setObjectName("toolbar");
    setFixedHeight(44);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(8, 4, 8, 4);
    mainLayout->setSpacing(4);

    // 导航按钮
    m_btnUp = new QPushButton("↑", this);
    m_btnUp->setObjectName("navButton");
    m_btnUp->setFixedSize(32, 32);
    m_btnUp->setToolTip("上级目录");
    m_btnUp->setEnabled(false);

    m_btnHome = new QPushButton("🏠", this);
    m_btnHome->setObjectName("navButton");
    m_btnHome->setFixedSize(32, 32);
    m_btnHome->setToolTip("主目录");

    m_btnRoot = new QPushButton("/", this);
    m_btnRoot->setObjectName("navButton");
    m_btnRoot->setFixedSize(32, 32);
    m_btnRoot->setToolTip("根目录");

    m_btnRefresh = new QPushButton("🔄", this);
    m_btnRefresh->setObjectName("navButton");
    m_btnRefresh->setFixedSize(32, 32);
    m_btnRefresh->setToolTip("刷新");

    mainLayout->addWidget(m_btnUp);
    mainLayout->addWidget(m_btnHome);
    mainLayout->addWidget(m_btnRoot);
    mainLayout->addWidget(m_btnRefresh);

    // 分隔符
    QWidget* sep = new QWidget(this);
    sep->setFixedWidth(1);
    sep->setStyleSheet("background-color: #444444;");
    mainLayout->addWidget(sep);

    // 路径显示（只读）
    m_pathLabel = new QLabel(this);
    m_pathLabel->setObjectName("pathLabel");
    m_pathLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_pathLabel, 1);

    // 连接信号
    connect(m_btnUp, &QPushButton::clicked, this, &ToolBar::goUp);
    connect(m_btnHome, &QPushButton::clicked, this, &ToolBar::goHome);
    connect(m_btnRoot, &QPushButton::clicked, this, &ToolBar::goRoot);
    connect(m_btnRefresh, &QPushButton::clicked, this, &ToolBar::refresh);
}

void ToolBar::setCurrentPath(const QString& path)
{
    m_pathLabel->setText(path);
}

void ToolBar::setUpEnabled(bool enabled)
{
    m_btnUp->setEnabled(enabled);
}

void ToolBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
