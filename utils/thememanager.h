#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>

/**
 * @brief 主题管理器（单例）
 * 负责加载、保存、切换主题
 */
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    static ThemeManager* instance();

    // 应用主题（dark / light / 自定义文件路径）
    void applyTheme(const QString& themeName);

    // 获取当前主题名
    QString currentTheme() const;

    // 获取可用主题列表
    QStringList availableThemes() const;

    // 从外部 QSS 文件加载主题
    void loadFromFile(const QString& filePath);

signals:
    void themeChanged(const QString& themeName);

private:
    explicit ThemeManager(QObject* parent = nullptr);

    void saveConfig();
    void loadConfig();
    QString configPath() const;
    QString themesDir() const;

    // 内置主题样式表
    QString darkTheme() const;
    QString lightTheme() const;

    static ThemeManager* s_instance;
    QString m_currentTheme;
};

#endif // THEMEMANAGER_H
