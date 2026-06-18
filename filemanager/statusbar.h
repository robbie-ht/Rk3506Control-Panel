#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>
#include <QLabel>

/**
 * @brief 状态栏组件
 * 显示当前目录信息和操作提示
 */
class StatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget* parent = nullptr);
    ~StatusBar();

    /**
     * @brief 设置文件数量信息
     */
    void setFileCount(int files, int dirs);

    /**
     * @brief 设置当前路径
     */
    void setCurrentPath(const QString& path);

    /**
     * @brief 设置状态消息
     */
    void setMessage(const QString& message);

protected:
    /**
     * @brief 绘制事件
     */
    void paintEvent(QPaintEvent* event) override;

private:
    void setupUI();

    QLabel* m_pathLabel;
    QLabel* m_countLabel;
    QLabel* m_messageLabel;
};

#endif // STATUSBAR_H
