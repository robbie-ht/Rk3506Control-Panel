#include "sidebaritem.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QStyle>
#include <cmath>

SidebarItem::SidebarItem(const QString& name, const QString& iconType, QWidget* parent)
    : QWidget(parent)
    , m_name(name)
    , m_iconType(iconType)
    , m_selected(false)
    , m_hovered(false)
{
    setFixedSize(68, 64);
    setCursor(Qt::PointingHandCursor);
    setToolTip(name);
    setObjectName("sidebarItem");
}

SidebarItem::~SidebarItem()
{
}

void SidebarItem::setSelected(bool selected)
{
    if (m_selected != selected) {
        m_selected = selected;
        setProperty("selected", selected);
        style()->unpolish(this);
        style()->polish(this);
        update();
    }
}

bool SidebarItem::isSelected() const
{
    return m_selected;
}

void SidebarItem::drawIcon(QPainter& painter, const QRect& rect, const QString& iconType)
{
    painter.save();

    QPen pen(m_selected ? QColor(255, 255, 255) : QColor(160, 160, 160), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    int x = rect.x();
    int y = rect.y();
    int w = rect.width();
    int h = rect.height();
    int cx = rect.center().x();
    int cy = rect.center().y();

    if (iconType == "home") {
        // 房子图标
        QPainterPath path;
        path.moveTo(cx, y + 4);
        path.lineTo(x + w - 4, cy - 2);
        path.lineTo(x + w - 4, y + h - 4);
        path.lineTo(x + 4, y + h - 4);
        path.lineTo(x + 4, cy - 2);
        path.closeSubpath();
        painter.drawPath(path);

        // 门
        painter.drawRect(cx - 4, cy + 4, 8, 10);

    } else if (iconType == "settings") {
        // 齿轮图标
        int r1 = w / 2 - 4;
        int r2 = w / 2 - 10;

        // 外圈
        painter.drawEllipse(cx, cy, r1, r1);

        // 内圈
        painter.drawEllipse(cx, cy, r2, r2);

        // 齿轮齿
        for (int i = 0; i < 8; i++) {
            double angle = i * 45.0 * M_PI / 180.0;
            int x1 = cx + (r1 - 2) * cos(angle);
            int y1 = cy + (r1 - 2) * sin(angle);
            int x2 = cx + (r1 + 3) * cos(angle);
            int y2 = cy + (r1 + 3) * sin(angle);
            painter.drawLine(x1, y1, x2, y2);
        }

    } else if (iconType == "files") {
        // 文件夹图标
        QPainterPath path;
        path.moveTo(x + 4, y + 8);
        path.lineTo(x + 10, y + 4);
        path.lineTo(x + w - 4, y + 4);
        path.lineTo(x + w - 4, y + h - 4);
        path.lineTo(x + 4, y + h - 4);
        path.closeSubpath();
        painter.drawPath(path);

        // 文件夹盖子
        painter.drawLine(x + 4, y + 8, x + 10, y + 4);
        painter.drawLine(x + 10, y + 4, x + w - 4, y + 4);

    } else if (iconType == "hello") {
        // Hello 图标（手 waving）
        painter.drawEllipse(cx - 8, cy - 10, 16, 16);  // 头
        painter.drawLine(cx, cy + 6, cx, cy + 14);      // 身体
        painter.drawLine(cx - 8, cy, cx - 14, cy - 6);  // 左手
        painter.drawLine(cx + 8, cy, cx + 14, cy - 8);  // 右手（举起）
        painter.drawLine(cx, cy + 14, cx - 6, cy + 22); // 左脚
        painter.drawLine(cx, cy + 14, cx + 6, cy + 22); // 右脚

    } else {
        // 默认图标（圆角矩形）
        painter.drawRoundedRect(rect.adjusted(4, 4, -4, -4), 6, 6);
    }

    painter.restore();
}

void SidebarItem::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF itemRect = rect().adjusted(6, 4, -6, -4);

    // 绘制选中/悬停背景
    if (m_selected) {
        QLinearGradient gradient(itemRect.topLeft(), itemRect.bottomLeft());
        gradient.setColorAt(0, QColor(255, 255, 255, 18));
        gradient.setColorAt(1, QColor(255, 255, 255, 8));
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(itemRect, 12, 12);

        // 左侧指示条
        painter.setBrush(QColor(255, 255, 255, 200));
        painter.drawRoundedRect(QRectF(itemRect.left(), itemRect.top() + 16, 3, 28), 1.5, 1.5);
    } else if (m_hovered) {
        QLinearGradient gradient(itemRect.topLeft(), itemRect.bottomLeft());
        gradient.setColorAt(0, QColor(255, 255, 255, 10));
        gradient.setColorAt(1, QColor(255, 255, 255, 4));
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(itemRect, 12, 12);
    }

    // 绘制图标
    QRect iconRect(width() / 2 - 12, 10, 24, 24);
    drawIcon(painter, iconRect, m_iconType);

    // 绘制文字
    if (m_selected) {
        painter.setPen(QColor(255, 255, 255));
    } else {
        painter.setPen(QColor(160, 160, 160));
    }
    QFont font = painter.font();
    font.setPixelSize(11);
    font.setWeight(QFont::Normal);
    painter.setFont(font);

    QRect textRect(0, 42, width(), 18);
    painter.drawText(textRect, Qt::AlignCenter, m_name);
}

void SidebarItem::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void SidebarItem::enterEvent(QEvent* event)
{
    Q_UNUSED(event)
    m_hovered = true;
    update();
}

void SidebarItem::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    m_hovered = false;
    update();
}
