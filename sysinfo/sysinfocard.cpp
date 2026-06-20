#include "sysinfocard.h"
#include "utils/thememanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

SysInfoCard::SysInfoCard(const QString& icon, const QString& title, QWidget* parent)
    : QWidget(parent)
    , m_icon(icon)
    , m_title(title)
{
    setObjectName("sysInfoCard");
    setupUI();
}

SysInfoCard::~SysInfoCard()
{
}

void SysInfoCard::setupUI()
{
    setFixedSize(220, 130);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(18, 14, 18, 14);
    mainLayout->setSpacing(6);

    // 顶部：图标和标题
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(10);

    m_iconLabel = new QLabel(m_icon, this);
    m_iconLabel->setObjectName("cardIcon");
    m_iconLabel->setFixedSize(36, 36);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    topLayout->addWidget(m_iconLabel);

    m_titleLabel = new QLabel(m_title, this);
    m_titleLabel->setObjectName("cardTitle");
    topLayout->addWidget(m_titleLabel);
    topLayout->addStretch();

    mainLayout->addLayout(topLayout);

    mainLayout->addStretch();

    // 值
    m_valueLabel = new QLabel("--", this);
    m_valueLabel->setObjectName("cardValue");
    mainLayout->addWidget(m_valueLabel);

    // 子标题
    m_subtitleLabel = new QLabel(this);
    m_subtitleLabel->setObjectName("cardSubtitle");
    mainLayout->addWidget(m_subtitleLabel);
}

void SysInfoCard::setValue(const QString& value)
{
    m_valueLabel->setText(value);
}

void SysInfoCard::setSubtitle(const QString& subtitle)
{
    m_subtitleLabel->setText(subtitle);
}

void SysInfoCard::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    bool isLight = ThemeManager::instance()->currentTheme().startsWith("light");

    QRectF cardRect = rect().adjusted(1, 1, -1, -1);

    // 绘制阴影
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(isLight ? QColor(180, 150, 100, 30) : QColor(0, 0, 0, 40));
    painter.drawRoundedRect(cardRect.adjusted(3, 3, 3, 3), 14, 14);
    painter.restore();

    // 绘制背景渐变
    QLinearGradient gradient(cardRect.topLeft(), cardRect.bottomRight());
    if (isLight) {
        gradient.setColorAt(0, QColor(255, 252, 245));
        gradient.setColorAt(1, QColor(248, 236, 216));
    } else {
        gradient.setColorAt(0, QColor(30, 30, 50));
        gradient.setColorAt(1, QColor(20, 20, 40));
    }

    painter.setPen(QPen(isLight ? QColor(212, 184, 150) : QColor(50, 50, 80), 1));
    painter.setBrush(gradient);
    painter.drawRoundedRect(cardRect, 14, 14);

    // 绘制顶部高光线
    QLinearGradient highlightGradient(cardRect.topLeft(), QPointF(cardRect.topRight().x(), cardRect.topLeft().y() + 2));
    if (isLight) {
        highlightGradient.setColorAt(0, QColor(255, 200, 100, 0));
        highlightGradient.setColorAt(0.5, QColor(255, 200, 100, 40));
        highlightGradient.setColorAt(1, QColor(255, 200, 100, 0));
    } else {
        highlightGradient.setColorAt(0, QColor(80, 80, 120, 0));
        highlightGradient.setColorAt(0.5, QColor(80, 80, 120, 60));
        highlightGradient.setColorAt(1, QColor(80, 80, 120, 0));
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(highlightGradient);
    painter.drawRoundedRect(QRectF(cardRect.topLeft(), QSizeF(cardRect.width(), 2)), 1, 1);
}
