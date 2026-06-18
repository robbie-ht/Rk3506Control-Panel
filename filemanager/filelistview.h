#ifndef FILELISTVIEW_H
#define FILELISTVIEW_H

#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QItemSelectionModel>

/**
 * @brief 文件列表视图
 * 支持列表和图标两种视图模式
 */
class FileListView : public QWidget
{
    Q_OBJECT

public:
    enum ViewMode {
        ListView,    // 列表视图
        IconView     // 图标视图
    };

    explicit FileListView(QWidget* parent = nullptr);
    ~FileListView();

    /**
     * @brief 设置根路径
     */
    void setRootPath(const QString& path);

    /**
     * @brief 获取当前选中的文件路径列表
     */
    QStringList selectedFiles() const;

    /**
     * @brief 获取当前选中的单个文件路径
     */
    QString selectedFile() const;

    /**
     * @brief 设置视图模式
     */
    void setViewMode(ViewMode mode);

    /**
     * @brief 获取视图模式
     */
    ViewMode viewMode() const;

    /**
     * @brief 获取文件系统模型
     */
    QFileSystemModel* model() const;

signals:
    /**
     * @brief 文件被双击信号
     */
    void fileDoubleClicked(const QString& path);

    /**
     * @brief 选择改变信号
     */
    void selectionChanged(const QStringList& files);

protected:
    /**
     * @brief 绘制事件
     */
    void paintEvent(QPaintEvent* event) override;

private slots:
    /**
     * @brief 双击处理
     */
    void onDoubleClicked(const QModelIndex& index);

    /**
     * @brief 选择改变处理
     */
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    void setupUI();
    void setupConnections();

    QFileSystemModel* m_model;
    QTreeView* m_treeView;
    QListView* m_listView;
    ViewMode m_viewMode;
    QString m_currentPath;
};

#endif // FILELISTVIEW_H
