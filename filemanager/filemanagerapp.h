#ifndef FILEMANAGERAPP_H
#define FILEMANAGERAPP_H

#include <QObject>
#include <QWidget>
#include "apps/appinterface.h"

/**
 * @brief 文件管理器App
 * 实现AppInterface接口，可作为插件加载
 */
class FileManagerApp : public QObject, public AppInterface
{
    Q_OBJECT
    Q_INTERFACES(AppInterface)

public:
    explicit FileManagerApp(QObject* parent = nullptr);
    ~FileManagerApp();

    QString appName() const override;
    QString iconPath() const override;
    QWidget* contentWidget() override;
    void initialize() override;

private:
    QWidget* m_widget;
};

#endif // FILEMANAGERAPP_H
