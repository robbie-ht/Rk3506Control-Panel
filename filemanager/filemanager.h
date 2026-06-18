#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileSystemModel>
#include <QDir>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

/**
 * @brief 文件管理器核心类
 * 负责文件操作逻辑
 */
class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject* parent = nullptr);
    ~FileManager();

    /**
     * @brief 获取当前目录
     */
    QString currentPath() const;

    /**
     * @brief 设置当前目录
     */
    void setCurrentPath(const QString& path);

    /**
     * @brief 导航到上级目录
     */
    bool goUp();

    /**
     * @brief 导航到主目录
     */
    void goHome();

    /**
     * @brief 导航到根目录
     */
    void goRoot();

    /**
     * @brief 刷新当前目录
     */
    void refresh();

    /**
     * @brief 复制文件/目录
     * @param source 源路径
     * @param destDir 目标目录
     * @return 是否成功
     */
    bool copy(const QString& source, const QString& destDir);

    /**
     * @brief 删除文件/目录
     * @param path 要删除的路径
     * @return 是否成功
     */
    bool remove(const QString& path);

    /**
     * @brief 创建目录
     * @param path 目录路径
     * @return 是否成功
     */
    bool mkdir(const QString& path);

    /**
     * @brief 重命名
     * @param oldPath 旧路径
     * @param newName 新名称
     * @return 是否成功
     */
    bool rename(const QString& oldPath, const QString& newName);

    /**
     * @brief 检查是否可以返回上级目录
     */
    bool canGoUp() const;

    /**
     * @brief 获取文件信息
     */
    QFileInfo fileInfo(const QString& path) const;

    /**
     * @brief 获取目录内容列表
     */
    QStringList entryList(const QString& path) const;

    /**
     * @brief 获取文件大小（格式化）
     */
    static QString formatSize(qint64 bytes);

    /**
     * @brief 获取文件类型描述
     */
    static QString fileType(const QFileInfo& info);

signals:
    /**
     * @brief 目录改变信号
     */
    void directoryChanged(const QString& path);

    /**
     * @brief 操作完成信号
     */
    void operationCompleted(bool success, const QString& message);

    /**
     * @brief 错误信号
     */
    void errorOccurred(const QString& message);

private:
    QString m_currentPath;
};

#endif // FILEMANAGER_H
