#include "appmanager.h"
#include "appinterface.h"
#include <QDir>
#include <QPluginLoader>
#include <QDebug>

AppManager* AppManager::s_instance = nullptr;

AppManager* AppManager::instance()
{
    if (!s_instance) {
        s_instance = new AppManager();
    }
    return s_instance;
}

AppManager::AppManager(QObject* parent)
    : QObject(parent)
{
}

AppManager::~AppManager()
{
    qDeleteAll(m_loaders);
    qDeleteAll(m_apps);
}

void AppManager::registerApp(AppInterface* app)
{
    if (app && !m_appMap.contains(app->appName())) {
        m_apps.append(app);
        m_appMap.insert(app->appName(), app);
        qDebug() << "Registered app:" << app->appName();
    }
}

void AppManager::loadPlugins(const QString& path)
{
    QDir pluginsDir(path);
    if (!pluginsDir.exists()) {
        qDebug() << "Plugins directory not found:" << path;
        return;
    }

    qDebug() << "Loading plugins from:" << path;

    // 扫描所有 .so 文件
    QStringList filters;
    filters << "*.so";
    QFileInfoList fileList = pluginsDir.entryInfoList(filters, QDir::Files);

    for (const QFileInfo& fileInfo : fileList) {
        QString filePath = fileInfo.absoluteFilePath();
        qDebug() << "Trying to load:" << filePath;

        QPluginLoader* loader = new QPluginLoader(filePath, this);
        QObject* plugin = loader->instance();

        if (plugin) {
            AppInterface* app = qobject_cast<AppInterface*>(plugin);
            if (app) {
                registerApp(app);
                m_loaders.append(loader);
                qDebug() << "Loaded plugin:" << app->appName();
            } else {
                qDebug() << "Not a valid AppInterface:" << filePath;
                delete loader;
            }
        } else {
            qDebug() << "Failed to load:" << loader->errorString();
            delete loader;
        }
    }
}

QList<AppInterface*> AppManager::apps() const
{
    return m_apps;
}

AppInterface* AppManager::app(const QString& name) const
{
    return m_appMap.value(name, nullptr);
}

int AppManager::count() const
{
    return m_apps.size();
}
