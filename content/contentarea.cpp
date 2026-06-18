#include "contentarea.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QStyleOption>

ContentArea::ContentArea(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setObjectName("contentContainer");
    layout->addWidget(m_stackedWidget);
}

ContentArea::~ContentArea()
{
}

int ContentArea::addPage(QWidget* page)
{
    return m_stackedWidget->addWidget(page);
}

void ContentArea::setCurrentPage(int index)
{
    m_stackedWidget->setCurrentIndex(index);
}

int ContentArea::currentPage() const
{
    return m_stackedWidget->currentIndex();
}

void ContentArea::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
