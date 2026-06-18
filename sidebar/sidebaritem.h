#ifndef SIDEBARITEM_H
#define SIDEBARITEM_H

#include <QWidget>
#include <QIcon>
#include <QString>

/**
 * @brief 侧边栏项目组件
 */
class SidebarItem : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ isSelected WRITE setSelected)

public:
    explicit SidebarItem(const QString& name, const QString& iconType, QWidget* parent = nullptr);
    ~SidebarItem();

    /**
     * @brief 设置选中状态
     */
    void setSelected(bool selected);

    /**
     * @brief 是否选中
     */
    bool isSelected() const;

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    /**
     * @brief 绘制图标
     */
    void drawIcon(QPainter& painter, const QRect& rect, const QString& iconType);

    QString m_name;
    QString m_iconType;  // 图标类型名称，如 "home", "settings", "files"
    bool m_selected;
    bool m_hovered;
};

#endif // SIDEBARITEM_H
