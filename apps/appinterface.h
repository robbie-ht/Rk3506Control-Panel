#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QWidget>
#include <QString>
#include <QIcon>

/**
 * @brief 应用插件接口
 * 所有App都需要实现此接口
 */
class AppInterface
{
public:
    virtual ~AppInterface() = default;

    /**
     * @brief 获取App名称
     */
    virtual QString appName() const = 0;

    /**
     * @brief 获取App图标路径（支持SVG）
     */
    virtual QString iconPath() const = 0;

    /**
     * @brief 获取App内容Widget
     */
    virtual QWidget* contentWidget() = 0;

    /**
     * @brief App初始化
     */
    virtual void initialize() = 0;
};

#define AppInterface_iid "com.lyra.shell.AppInterface"
Q_DECLARE_INTERFACE(AppInterface, AppInterface_iid)

#endif // APPINTERFACE_H
