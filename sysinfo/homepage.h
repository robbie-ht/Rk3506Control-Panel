#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class SysInfoCard;
class SysInfoManager;

/**
 * @brief 首页组件
 * 显示系统信息卡片
 */
class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget* parent = nullptr);
    ~HomePage();

    /**
     * @brief 初始化
     */
    void initialize();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    /**
     * @brief 更新时间显示
     */
    void updateTime();

    /**
     * @brief 更新系统信息显示
     */
    void updateSysInfo();

private:
    void setupUI();

    QLabel* m_timeLabel;
    QLabel* m_dateLabel;
    QLabel* m_welcomeLabel;

    SysInfoCard* m_cpuTempCard;
    SysInfoCard* m_cpuUsageCard;
    SysInfoCard* m_memoryCard;
    SysInfoCard* m_timeCard;

    SysInfoManager* m_sysInfoManager;
    QTimer* m_timeTimer;
};

#endif // HOMEPAGE_H
