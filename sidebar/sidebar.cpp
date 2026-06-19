#include "sidebar.h"
#include "sidebaritem.h"
#include <QPainter>
#include <QStyleOption>
#include <QFrame>

Sidebar::Sidebar(QWidget* parent)
    : QWidget(parent)
    , m_currentIndex(-1)
{
    setObjectName("sidebar");
    setFixedWidth(80);

    // 创建滚动区域
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("sidebarScroll");
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);

    // 创建容器
    m_container = new QWidget();
    m_container->setObjectName("sidebarContainer");
    m_layout = new QVBoxLayout(m_container);
    m_layout->setContentsMargins(0, 16, 0, 16);
    m_layout->setSpacing(2);
    m_layout->addStretch();

    scrollArea->setWidget(m_container);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(scrollArea);
}

Sidebar::~Sidebar()
{
}

void Sidebar::addItem(const QString& name, const QString& iconType)
{
    SidebarItem* item = new SidebarItem(name, iconType, m_container);
    int index = m_items.size();

    connect(item, &SidebarItem::clicked, this, [this, index]() {
        setCurrentItem(index);
        emit itemClicked(index);
    });

    m_items.append(item);
    m_layout->insertWidget(m_layout->count() - 1, item);
}

void Sidebar::addSeparator()
{
    QFrame* line = new QFrame(m_container);
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("sidebarSeparator");
    line->setFixedHeight(1);
    line->setStyleSheet("background-color: #222222; margin: 8px 16px;");
    m_layout->insertWidget(m_layout->count() - 1, line);
}

void Sidebar::setCurrentItem(int index)
{
    if (index >= 0 && index < m_items.size()) {
        if (m_currentIndex >= 0 && m_currentIndex < m_items.size()) {
            m_items[m_currentIndex]->setSelected(false);
        }
        m_currentIndex = index;
        m_items[m_currentIndex]->setSelected(true);
    }
}

int Sidebar::currentIndex() const
{
    return m_currentIndex;
}

void Sidebar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
