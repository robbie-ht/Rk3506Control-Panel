#include "sysinfomanager.h"
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include <QDir>

SysInfoManager::SysInfoManager(QObject* parent)
    : QObject(parent)
    , m_cpuPercent(0)
    , m_memoryPercent(0)
    , m_lastCpuTotal(0)
    , m_lastCpuIdle(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SysInfoManager::updateInfo);
}

SysInfoManager::~SysInfoManager()
{
    stopMonitoring();
}

void SysInfoManager::startMonitoring()
{
    updateInfo();
    m_timer->start(1000); // 每秒更新一次
}

void SysInfoManager::stopMonitoring()
{
    m_timer->stop();
}

QString SysInfoManager::cpuTemperature() const
{
    return m_cpuTemp;
}

QString SysInfoManager::cpuUsage() const
{
    return m_cpuUsage;
}

QString SysInfoManager::memoryUsage() const
{
    return m_memoryUsage;
}

int SysInfoManager::memoryPercent() const
{
    return m_memoryPercent;
}

int SysInfoManager::cpuPercent() const
{
    return m_cpuPercent;
}

void SysInfoManager::updateInfo()
{
    readCpuTemperature();
    readCpuUsage();
    readMemoryUsage();
    emit infoUpdated();
}

void SysInfoManager::readCpuTemperature()
{
    // 尝试读取CPU温度
    QStringList tempPaths = {
        "/sys/class/thermal/thermal_zone0/temp",
        "/sys/class/thermal/thermal_zone1/temp",
        "/sys/class/hwmon/hwmon0/temp1_input"
    };

    for (const QString& path : tempPaths) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString tempStr = in.readLine().trimmed();
            bool ok;
            double temp = tempStr.toDouble(&ok);
            if (ok) {
                // 温度通常是毫度，需要除以1000
                if (temp > 1000) {
                    temp /= 1000.0;
                }
                m_cpuTemp = QString("%1°C").arg(temp, 0, 'f', 1);
                return;
            }
        }
    }

    m_cpuTemp = "N/A";
}

void SysInfoManager::readCpuUsage()
{
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_cpuUsage = "N/A";
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    QStringList parts = line.split(QRegExp("\\s+"));

    if (parts.size() < 5) {
        m_cpuUsage = "N/A";
        return;
    }

    qint64 user = parts[1].toLongLong();
    qint64 nice = parts[2].toLongLong();
    qint64 system = parts[3].toLongLong();
    qint64 idle = parts[4].toLongLong();
    qint64 iowait = parts.size() > 5 ? parts[5].toLongLong() : 0;
    qint64 irq = parts.size() > 6 ? parts[6].toLongLong() : 0;
    qint64 softirq = parts.size() > 7 ? parts[7].toLongLong() : 0;

    qint64 total = user + nice + system + idle + iowait + irq + softirq;
    qint64 totalDiff = total - m_lastCpuTotal;
    qint64 idleDiff = idle - m_lastCpuIdle;

    if (totalDiff > 0) {
        m_cpuPercent = (int)((totalDiff - idleDiff) * 100 / totalDiff);
        m_cpuUsage = QString("%1%").arg(m_cpuPercent);
    }

    m_lastCpuTotal = total;
    m_lastCpuIdle = idle;
}

void SysInfoManager::readMemoryUsage()
{
    QFile file("/proc/meminfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_memoryUsage = "N/A";
        return;
    }

    qint64 memTotal = 0;
    qint64 memFree = 0;
    qint64 memAvailable = 0;
    qint64 buffers = 0;
    qint64 cached = 0;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("MemTotal:")) {
            memTotal = line.split(QRegExp("\\s+"))[1].toLongLong();
        } else if (line.startsWith("MemFree:")) {
            memFree = line.split(QRegExp("\\s+"))[1].toLongLong();
        } else if (line.startsWith("MemAvailable:")) {
            memAvailable = line.split(QRegExp("\\s+"))[1].toLongLong();
        } else if (line.startsWith("Buffers:")) {
            buffers = line.split(QRegExp("\\s+"))[1].toLongLong();
        } else if (line.startsWith("Cached:")) {
            cached = line.split(QRegExp("\\s+"))[1].toLongLong();
        }
    }

    if (memTotal > 0) {
        qint64 used = memTotal - memAvailable;
        m_memoryPercent = (int)(used * 100 / memTotal);

        double usedMB = used / 1024.0;
        double totalMB = memTotal / 1024.0;

        if (totalMB >= 1024) {
            m_memoryUsage = QString("%1 GB / %2 GB")
                .arg(usedMB / 1024.0, 0, 'f', 1)
                .arg(totalMB / 1024.0, 0, 'f', 1);
        } else {
            m_memoryUsage = QString("%1 MB / %2 MB")
                .arg(usedMB, 0, 'f', 0)
                .arg(totalMB, 0, 'f', 0);
        }
    } else {
        m_memoryUsage = "N/A";
    }
}
