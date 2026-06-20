#ifndef FILELISTVIEW_H
#define FILELISTVIEW_H

#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QItemSelectionModel>
#include <QTimer>
#include <QPoint>

/**
 * @brief 文件列表视图
 * 支持长按弹出菜单（触屏兼容）
 */
class FileListView : public QWidget
{
    Q_OBJECT

public:
    enum ViewMode {
        ListView,
        IconView
    };

    explicit FileListView(QWidget* parent = nullptr);
    ~FileListView();

    void setRootPath(const QString& path);
    QStringList selectedFiles() const;
    QString selectedFile() const;
    void setViewMode(ViewMode mode);
    ViewMode viewMode() const;
    QFileSystemModel* model() const;
    QString currentPath() const;

signals:
    void fileDoubleClicked(const QString& path);
    void selectionChanged(const QStringList& files);
    void contextMenuRequested(const QString& path, const QPoint& pos);

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onDoubleClicked(const QModelIndex& index);
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void onLongPress();

private:
    void setupUI();
    void setupConnections();

    QFileSystemModel* m_model;
    QTreeView* m_treeView;
    QListView* m_listView;
    ViewMode m_viewMode;
    QString m_currentPath;

    // 长按检测
    QTimer* m_longPressTimer;
    QPoint m_pressPos;
    QWidget* m_pressWidget;
    bool m_longPressTriggered;
};

#endif // FILELISTVIEW_H
