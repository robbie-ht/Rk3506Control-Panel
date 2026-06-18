#include "filemanager.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>
#include <QDebug>

FileManager::FileManager(QObject* parent)
    : QObject(parent)
    , m_currentPath(QDir::homePath())
{
}

FileManager::~FileManager()
{
}

QString FileManager::currentPath() const
{
    return m_currentPath;
}

void FileManager::setCurrentPath(const QString& path)
{
    QDir dir(path);
    if (dir.exists() && dir.isReadable()) {
        m_currentPath = dir.absolutePath();
        emit directoryChanged(m_currentPath);
    } else {
        emit errorOccurred(QString("无法访问目录: %1").arg(path));
    }
}

bool FileManager::goUp()
{
    QDir dir(m_currentPath);
    if (dir.cdUp()) {
        m_currentPath = dir.absolutePath();
        emit directoryChanged(m_currentPath);
        return true;
    }
    return false;
}

void FileManager::goHome()
{
    setCurrentPath(QDir::homePath());
}

void FileManager::goRoot()
{
    setCurrentPath("/");
}

void FileManager::refresh()
{
    emit directoryChanged(m_currentPath);
}

bool FileManager::copy(const QString& source, const QString& destDir)
{
    QFileInfo srcInfo(source);
    QDir dest(destDir);

    if (!srcInfo.exists()) {
        emit errorOccurred(QString("源文件不存在: %1").arg(source));
        return false;
    }

    if (!dest.exists()) {
        emit errorOccurred(QString("目标目录不存在: %1").arg(destDir));
        return false;
    }

    QString destPath = dest.absoluteFilePath(srcInfo.fileName());

    // 检查目标是否已存在
    if (QFileInfo::exists(destPath)) {
        emit errorOccurred(QString("目标已存在: %1").arg(destPath));
        return false;
    }

    bool success = false;

    if (srcInfo.isDir()) {
        // 复制目录
        QDir srcDir(source);
        QDir newDestDir(destPath);

        if (!newDestDir.mkpath(".")) {
            emit errorOccurred("无法创建目标目录");
            return false;
        }

        // 递归复制
        QDirIterator it(source, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            QString relativePath = srcDir.relativeFilePath(it.filePath());
            QString newDestPath = newDestDir.absoluteFilePath(relativePath);

            if (it.fileInfo().isDir()) {
                newDestDir.mkpath(relativePath);
            } else {
                QFile::copy(it.filePath(), newDestPath);
            }
        }
        success = true;
    } else {
        // 复制文件
        success = QFile::copy(source, destPath);
    }

    if (success) {
        emit operationCompleted(true, QString("已复制: %1").arg(srcInfo.fileName()));
    } else {
        emit errorOccurred(QString("复制失败: %1").arg(srcInfo.fileName()));
    }

    return success;
}

bool FileManager::remove(const QString& path)
{
    QFileInfo info(path);

    if (!info.exists()) {
        emit errorOccurred(QString("文件不存在: %1").arg(path));
        return false;
    }

    bool success = false;

    if (info.isDir()) {
        QDir dir(path);
        success = dir.removeRecursively();
    } else {
        success = QFile::remove(path);
    }

    if (success) {
        emit operationCompleted(true, QString("已删除: %1").arg(info.fileName()));
        refresh();
    } else {
        emit errorOccurred(QString("删除失败: %1").arg(info.fileName()));
    }

    return success;
}

bool FileManager::mkdir(const QString& path)
{
    QDir dir(m_currentPath);
    bool success = dir.mkdir(path);

    if (success) {
        emit operationCompleted(true, QString("已创建目录: %1").arg(path));
        refresh();
    } else {
        emit errorOccurred(QString("创建目录失败: %1").arg(path));
    }

    return success;
}

bool FileManager::rename(const QString& oldPath, const QString& newName)
{
    QFileInfo info(oldPath);
    QDir dir = info.absoluteDir();
    QString newPath = dir.absoluteFilePath(newName);

    if (QFileInfo::exists(newPath)) {
        emit errorOccurred(QString("名称已存在: %1").arg(newName));
        return false;
    }

    bool success = QFile::rename(oldPath, newPath);

    if (success) {
        emit operationCompleted(true, QString("已重命名: %1 → %2").arg(info.fileName(), newName));
        refresh();
    } else {
        emit errorOccurred("重命名失败");
    }

    return success;
}

bool FileManager::canGoUp() const
{
    QDir dir(m_currentPath);
    return dir.cdUp();
}

QFileInfo FileManager::fileInfo(const QString& path) const
{
    return QFileInfo(path);
}

QStringList FileManager::entryList(const QString& path) const
{
    QDir dir(path);
    return dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);
}

QString FileManager::formatSize(qint64 bytes)
{
    if (bytes < 1024)
        return QString("%1 B").arg(bytes);
    else if (bytes < 1024 * 1024)
        return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    else if (bytes < 1024 * 1024 * 1024)
        return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    else
        return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 1);
}

QString FileManager::fileType(const QFileInfo& info)
{
    if (info.isDir())
        return "文件夹";
    else if (info.isFile()) {
        QString suffix = info.suffix().toLower();
        if (suffix == "txt" || suffix == "log" || suffix == "md")
            return "文本文件";
        else if (suffix == "jpg" || suffix == "jpeg" || suffix == "png" || suffix == "gif" || suffix == "bmp")
            return "图片文件";
        else if (suffix == "mp3" || suffix == "wav" || suffix == "ogg" || suffix == "flac")
            return "音频文件";
        else if (suffix == "mp4" || suffix == "avi" || suffix == "mkv" || suffix == "mov")
            return "视频文件";
        else if (suffix == "cpp" || suffix == "h" || suffix == "c" || suffix == "py" || suffix == "java")
            return "源代码";
        else if (suffix == "sh" || suffix == "bash")
            return "脚本文件";
        else if (suffix == "so" || suffix == "a" || suffix == "o")
            return "库文件";
        else
            return "文件";
    }
    return "未知";
}
