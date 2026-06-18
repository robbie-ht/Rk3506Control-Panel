#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>

class SidebarItem;

/**
 * @brief 侧边栏组件
 * 模块化设计，方便添加新的App入口
 */
class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget* parent = nullptr);
    ~Sidebar();

    /**
     * @brief 添加侧边栏项目
     * @param name 显示名称
     * @param iconType 图标类型（home/settings/files）
     */
    void addItem(const QString& name, const QString& iconType);

    /**
     * @brief 添加分隔符
     */
    void addSeparator();

    /**
     * @brief 设置当前选中项
     */
    void setCurrentItem(int index);

    /**
     * @brief 获取当前选中索引
     */
    int currentIndex() const;

signals:
    /**
     * @brief 项目被点击信号
     */
    void itemClicked(int index);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVBoxLayout* m_layout;
    QWidget* m_container;
    QList<SidebarItem*> m_items;
    int m_currentIndex;
};

#endif // SIDEBAR_H
