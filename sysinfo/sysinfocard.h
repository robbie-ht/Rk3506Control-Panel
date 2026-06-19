#ifndef SYSINFOCARD_H
#define SYSINFOCARD_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

/**
 * @brief 系统信息卡片
 * 显示单个系统信息项
 */
class SysInfoCard : public QWidget
{
    Q_OBJECT

public:
    explicit SysInfoCard(const QString& icon, const QString& title, QWidget* parent = nullptr);
    ~SysInfoCard();

    /**
     * @brief 设置值
     */
    void setValue(const QString& value);

    /**
     * @brief 设置子标题
     */
    void setSubtitle(const QString& subtitle);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void setupUI();

    QString m_icon;
    QString m_title;
    QLabel* m_iconLabel;
    QLabel* m_titleLabel;
    QLabel* m_valueLabel;
    QLabel* m_subtitleLabel;
};

#endif // SYSINFOCARD_H
