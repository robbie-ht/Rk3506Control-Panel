#ifndef FILEMANAGERWIDGET_H
#define FILEMANAGERWIDGET_H

#include <QWidget>

class FileManager;
class FileListView;
class ToolBar;
class StatusBar;

/**
 * @brief 文件管理器组件
 * 可嵌入到其他窗口中使用
 */
class FileManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileManagerWidget(QWidget* parent = nullptr);
    ~FileManagerWidget();

    /**
     * @brief 初始化
     */
    void initialize();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onDirectoryChanged(const QString& path);
    void onFileDoubleClicked(const QString& path);
    void onSelectionChanged(const QStringList& files);
    void onOperationCompleted(bool success, const QString& message);
    void onErrorOccurred(const QString& message);
    void onCopy();
    void onCut();
    void onPaste();
    void onDelete();
    void onNewFolder();
    void onRename();

private:
    void setupUI();
    void setupConnections();
    void loadStyleSheet();
    void updateStatusBar();
    void updateToolBar();

    FileManager* m_fileManager;
    FileListView* m_fileListView;
    ToolBar* m_toolBar;
    StatusBar* m_statusBar;

    QStringList m_clipboardFiles;
    bool m_clipboardIsCut;
    QStringList m_selectedFiles;
};

#endif // FILEMANAGERWIDGET_H
