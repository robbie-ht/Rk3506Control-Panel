#ifndef CONTENTAREA_H
#define CONTENTAREA_H

#include <QWidget>
#include <QStackedWidget>

/**
 * @brief 内容区域组件
 * 用于显示各个App的内容
 */
class ContentArea : public QWidget
{
    Q_OBJECT

public:
    explicit ContentArea(QWidget* parent = nullptr);
    ~ContentArea();

    /**
     * @brief 添加内容页面
     * @return 页面索引
     */
    int addPage(QWidget* page);

    /**
     * @brief 设置当前显示的页面
     */
    void setCurrentPage(int index);

    /**
     * @brief 获取当前页面索引
     */
    int currentPage() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QStackedWidget* m_stackedWidget;
};

#endif // CONTENTAREA_H
