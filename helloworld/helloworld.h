#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include <QObject>
#include <QWidget>
#include "../apps/appinterface.h"

/**
 * @brief Hello World 示例插件
 */
class HelloWorld : public QObject, public AppInterface
{
    Q_OBJECT
    Q_INTERFACES(AppInterface)
    Q_PLUGIN_METADATA(IID "com.lyra.shell.AppInterface")

public:
    explicit HelloWorld(QObject* parent = nullptr);
    ~HelloWorld();

    QString appName() const override;
    QString iconPath() const override;
    QWidget* contentWidget() override;
    void initialize() override;

private:
    QWidget* m_widget;
};

#endif // HELLOWORLD_H
