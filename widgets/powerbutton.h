#ifndef POWERBUTTON_H
#define POWERBUTTON_H

#include <QPushButton>

/**
 * @brief 电源按钮组件
 * 悬浮在右下角的圆形电源按钮
 */
class PowerButton : public QPushButton
{
    Q_OBJECT

public:
    explicit PowerButton(QWidget* parent = nullptr);
    ~PowerButton();

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    /**
     * @brief 绘制电源图标
     */
    void drawPowerIcon(QPainter& painter, const QRectF& rect);

    bool m_hovered;
};

#endif // POWERBUTTON_H
