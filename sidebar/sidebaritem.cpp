#include "sidebaritem.h"
#include "utils/thememanager.h"
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
    setFixedSize(60, 58);
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

    bool isLight = ThemeManager::instance()->currentTheme().startsWith("light");
    QColor iconSelected = isLight ? QColor(224, 136, 32) : QColor(200, 200, 255);
    QColor iconNormal = isLight ? QColor(140, 100, 60) : QColor(120, 120, 160);

    QPen pen(m_selected ? iconSelected : iconNormal, 1.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
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
        path.moveTo(cx, y + 3);
        path.lineTo(x + w - 3, cy - 2);
        path.lineTo(x + w - 3, y + h - 3);
        path.lineTo(x + 3, y + h - 3);
        path.lineTo(x + 3, cy - 2);
        path.closeSubpath();
        painter.drawPath(path);

        // 门
        painter.drawRect(cx - 3, cy + 4, 6, 8);

    } else if (iconType == "settings") {
        // 齿轮图标
        int r1 = w / 2 - 3;
        int r2 = w / 2 - 8;

        painter.drawEllipse(cx, cy, r1, r1);
        painter.drawEllipse(cx, cy, r2, r2);

        for (int i = 0; i < 8; i++) {
            double angle = i * 45.0 * M_PI / 180.0;
            int x1 = cx + (r1 - 2) * cos(angle);
            int y1 = cy + (r1 - 2) * sin(angle);
            int x2 = cx + (r1 + 2) * cos(angle);
            int y2 = cy + (r1 + 2) * sin(angle);
            painter.drawLine(x1, y1, x2, y2);
        }

    } else if (iconType == "files") {
        // 文件夹图标
        QPainterPath path;
        path.moveTo(x + 3, y + 7);
        path.lineTo(x + 9, y + 3);
        path.lineTo(x + w - 3, y + 3);
        path.lineTo(x + w - 3, y + h - 3);
        path.lineTo(x + 3, y + h - 3);
        path.closeSubpath();
        painter.drawPath(path);

        painter.drawLine(x + 3, y + 7, x + 9, y + 3);
        painter.drawLine(x + 9, y + 3, x + w - 3, y + 3);

    } else {
        // 默认图标
        painter.drawRoundedRect(rect.adjusted(3, 3, -3, -3), 5, 5);
    }

    painter.restore();
}

void SidebarItem::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF itemRect = rect().adjusted(4, 3, -4, -3);

    bool isLight = ThemeManager::instance()->currentTheme().startsWith("light");

    // 绘制选中/悬停背景
    if (m_selected) {
        QLinearGradient gradient(itemRect.topLeft(), itemRect.bottomLeft());
        if (isLight) {
            gradient.setColorAt(0, QColor(240, 160, 64, 100));
            gradient.setColorAt(1, QColor(224, 136, 32, 50));
        } else {
            gradient.setColorAt(0, QColor(74, 74, 138, 80));
            gradient.setColorAt(1, QColor(58, 58, 122, 40));
        }
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(itemRect, 10, 10);

        // 左侧指示条
        painter.setBrush(isLight ? QColor(224, 136, 32) : QColor(140, 140, 220));
        painter.drawRoundedRect(QRectF(itemRect.left(), itemRect.top() + 14, 3, 26), 1.5, 1.5);
    } else if (m_hovered) {
        QLinearGradient gradient(itemRect.topLeft(), itemRect.bottomLeft());
        if (isLight) {
            gradient.setColorAt(0, QColor(224, 180, 100, 60));
            gradient.setColorAt(1, QColor(200, 160, 80, 30));
        } else {
            gradient.setColorAt(0, QColor(40, 40, 80, 60));
            gradient.setColorAt(1, QColor(30, 30, 60, 30));
        }
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(itemRect, 10, 10);
    }

    // 绘制图标
    QRect iconRect(width() / 2 - 10, 8, 20, 20);
    drawIcon(painter, iconRect, m_iconType);

    // 绘制文字
    if (m_selected) {
        painter.setPen(isLight ? QColor(224, 136, 32) : QColor(200, 200, 255));
    } else {
        painter.setPen(isLight ? QColor(140, 100, 60) : QColor(120, 120, 160));
    }
    QFont font = painter.font();
    font.setPixelSize(10);
    font.setWeight(QFont::Normal);
    painter.setFont(font);

    QRect textRect(0, 34, width(), 16);
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
