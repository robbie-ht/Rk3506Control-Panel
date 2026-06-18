#include "powerbutton.h"
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QGraphicsDropShadowEffect>

PowerButton::PowerButton(QWidget* parent)
    : QPushButton(parent)
    , m_hovered(false)
{
    setObjectName("powerButton");
    setFixedSize(52, 52);
    setCursor(Qt::PointingHandCursor);
    setToolTip("退出系统");

    // 添加阴影效果
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 4);
    setGraphicsEffect(shadow);

    connect(this, &QPushButton::clicked, qApp, &QApplication::quit);
}

PowerButton::~PowerButton()
{
}

void PowerButton::drawPowerIcon(QPainter& painter, const QRectF& rect)
{
    painter.save();

    QPointF center = rect.center();
    qreal radius = rect.width() / 2.0;

    // 绘制圆环（断开的圆）
    QPen pen(QColor(200, 200, 200), 2.2, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    QPainterPath arcPath;
    QRectF arcRect(center.x() - radius * 0.55, center.y() - radius * 0.55,
                   radius * 1.1, radius * 1.1);
    arcPath.arcMoveTo(arcRect, 50);
    arcPath.arcTo(arcRect, 50, 260);
    painter.drawPath(arcPath);

    // 绘制竖线
    painter.drawLine(QPointF(center.x(), center.y() - radius * 0.65),
                     QPointF(center.x(), center.y() - radius * 0.1));

    painter.restore();
}

void PowerButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF btnRect = rect().adjusted(3, 3, -3, -3);
    QPointF center = btnRect.center();

    // 绘制外圈光晕
    if (m_hovered) {
        QRadialGradient glowGradient(center, btnRect.width() / 2 + 5);
        glowGradient.setColorAt(0.7, QColor(255, 255, 255, 15));
        glowGradient.setColorAt(1.0, QColor(255, 255, 255, 0));
        painter.setBrush(glowGradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(btnRect.adjusted(-5, -5, 5, 5));
    }

    // 绘制按钮背景
    QRadialGradient bgGradient(center, btnRect.width() / 2);
    if (m_hovered) {
        bgGradient.setColorAt(0, QColor(42, 42, 42));
        bgGradient.setColorAt(0.7, QColor(32, 32, 32));
        bgGradient.setColorAt(1, QColor(26, 26, 26));
    } else {
        bgGradient.setColorAt(0, QColor(32, 32, 32));
        bgGradient.setColorAt(0.7, QColor(24, 24, 24));
        bgGradient.setColorAt(1, QColor(18, 18, 18));
    }
    painter.setBrush(bgGradient);
    painter.setPen(QPen(QColor(45, 45, 45), 1.5));
    painter.drawEllipse(btnRect);

    // 绘制电源图标
    drawPowerIcon(painter, btnRect);
}

void PowerButton::enterEvent(QEvent* event)
{
    Q_UNUSED(event)
    m_hovered = true;
    update();
}

void PowerButton::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    m_hovered = false;
    update();
}
