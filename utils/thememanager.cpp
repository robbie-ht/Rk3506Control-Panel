#include "thememanager.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QCoreApplication>
#include <QApplication>
#include <QDebug>

ThemeManager* ThemeManager::s_instance = nullptr;

ThemeManager* ThemeManager::instance()
{
    if (!s_instance) {
        s_instance = new ThemeManager();
    }
    return s_instance;
}

ThemeManager::ThemeManager(QObject* parent)
    : QObject(parent)
    , m_currentTheme("dark")
{
    loadConfig();
}

QString ThemeManager::configPath() const
{
    return QDir::homePath() + "/.lyrashell/theme.conf";
}

void ThemeManager::loadConfig()
{
    QFile file(configPath());
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString theme = in.readLine().trimmed();
        if (!theme.isEmpty()) {
            m_currentTheme = theme;
        }
        file.close();
    }
}

void ThemeManager::saveConfig()
{
    QDir dir = QFileInfo(configPath()).absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(configPath());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << m_currentTheme << "\n";
        file.close();
    }
}

QStringList ThemeManager::availableThemes() const
{
    return QStringList() << "dark" << "light";
}

QString ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::applyTheme(const QString& themeName)
{
    QString qss;

    // 优先从 Qt 资源加载（编译时嵌入）
    QString resourcePath = ":/themes/" + themeName + ".qss";
    QFile resFile(resourcePath);
    if (resFile.exists() && resFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qss = resFile.readAll();
        resFile.close();
        qDebug() << "Loaded theme from resource:" << resourcePath;
    } else {
        // 尝试从外部文件加载
        QString externalPath = QCoreApplication::applicationDirPath() + "/themes/" + themeName + ".qss";
        QFile extFile(externalPath);
        if (extFile.exists() && extFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qss = extFile.readAll();
            extFile.close();
            qDebug() << "Loaded theme from file:" << externalPath;
        } else {
            qDebug() << "Theme not found:" << themeName << ", falling back to dark";
            // 回退到深色主题
            if (themeName != "dark") {
                applyTheme("dark");
                return;
            }
            return;
        }
    }

    qApp->setStyleSheet(qss);
    m_currentTheme = themeName;
    saveConfig();

    emit themeChanged(themeName);
}

void ThemeManager::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString qss = file.readAll();
        file.close();
        qApp->setStyleSheet(qss);
        m_currentTheme = "custom:" + QFileInfo(filePath).baseName();
        saveConfig();
        emit themeChanged(m_currentTheme);
    }
}
