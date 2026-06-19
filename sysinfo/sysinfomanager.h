#ifndef SYSINFOMANAGER_H
#define SYSINFOMANAGER_H

#include <QObject>
#include <QTimer>

/**
 * @brief 系统信息管理器
 * 获取CPU温度、内存使用等系统信息
 */
class SysInfoManager : public QObject
{
    Q_OBJECT

public:
    explicit SysInfoManager(QObject* parent = nullptr);
    ~SysInfoManager();

    /**
     * @brief 开始监控
     */
    void startMonitoring();

    /**
     * @brief 停止监控
     */
    void stopMonitoring();

    /**
     * @brief 获取CPU温度
     */
    QString cpuTemperature() const;

    /**
     * @brief 获取CPU使用率
     */
    QString cpuUsage() const;

    /**
     * @brief 获取内存使用情况
     */
    QString memoryUsage() const;

    /**
     * @brief 获取内存使用率
     */
    int memoryPercent() const;

    /**
     * @brief 获取CPU使用率百分比
     */
    int cpuPercent() const;

signals:
    /**
     * @brief 系统信息更新信号
     */
    void infoUpdated();

private slots:
    /**
     * @brief 更新系统信息
     */
    void updateInfo();

private:
    /**
     * @brief 读取CPU温度
     */
    void readCpuTemperature();

    /**
     * @brief 读取CPU使用率
     */
    void readCpuUsage();

    /**
     * @brief 读取内存使用情况
     */
    void readMemoryUsage();

    QTimer* m_timer;

    QString m_cpuTemp;
    QString m_cpuUsage;
    QString m_memoryUsage;
    int m_cpuPercent;
    int m_memoryPercent;

    // CPU使用率计算
    qint64 m_lastCpuTotal;
    qint64 m_lastCpuIdle;
};

#endif // SYSINFOMANAGER_H
