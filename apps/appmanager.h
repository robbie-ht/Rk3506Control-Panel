#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QPluginLoader>

class AppInterface;

/**
 * @brief App管理器
 * 负责管理和加载所有App插件（单例模式）
 */
class AppManager : public QObject
{
    Q_OBJECT

public:
    static AppManager* instance();

    /**
     * @brief 注册App
     */
    void registerApp(AppInterface* app);

    /**
     * @brief 扫描并加载插件目录
     * @param path 插件目录路径
     */
    void loadPlugins(const QString& path);

    /**
     * @brief 获取所有已注册的App
     */
    QList<AppInterface*> apps() const;

    /**
     * @brief 根据名称获取App
     */
    AppInterface* app(const QString& name) const;

    /**
     * @brief 获取App数量
     */
    int count() const;

private:
    explicit AppManager(QObject* parent = nullptr);
    ~AppManager();

    static AppManager* s_instance;
    QList<AppInterface*> m_apps;
    QMap<QString, AppInterface*> m_appMap;
    QList<QPluginLoader*> m_loaders;
};

#endif // APPMANAGER_H
