#ifndef FILEMANAGERWIDGET_H
#define FILEMANAGERWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QLabel>

class FileManager;
class FileListView;
class ToolBar;
class StatusBar;

/**
 * @brief 双栏文件管理器组件
 * 类似 MT 文件管理器，左右两栏独立导航
 */
class FileManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileManagerWidget(QWidget* parent = nullptr);
    ~FileManagerWidget();

    void initialize();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onLeftDoubleClicked(const QString& path);
    void onRightDoubleClicked(const QString& path);
    void onLeftContextMenu(const QString& path, const QPoint& pos);
    void onRightContextMenu(const QString& path, const QPoint& pos);
    void onOperationCompleted(bool success, const QString& message);
    void onErrorOccurred(const QString& message);

private:
    void setupUI();
    void setupConnections();
    void navigateTo(int panel, const QString& path);
    void updateStatusBar();
    void showContextMenu(int panel, const QString& path, const QPoint& globalPos);
    void doCopy(int panel);
    void doCut(int panel);
    void doPaste(int panel);
    void doDelete(int panel);
    void doRename(int panel, const QString& filePath);
    void doNewFolder(int panel);

    FileManager* m_leftFileManager;
    FileManager* m_rightFileManager;
    FileListView* m_leftView;
    FileListView* m_rightView;
    ToolBar* m_toolBar;
    StatusBar* m_statusBar;

    QLabel* m_leftPathLabel;
    QLabel* m_rightPathLabel;

    QStringList m_clipboardFiles;
    bool m_clipboardIsCut;
    int m_clipboardSourcePanel; // 来源面板：0=左，1=右
    int m_activePanel;          // 当前激活面板：0=左，1=右
};

#endif // FILEMANAGERWIDGET_H
