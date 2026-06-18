#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

/**
 * @brief 工具栏组件
 * 包含导航按钮和路径栏
 */
class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget* parent = nullptr);
    ~ToolBar();

    /**
     * @brief 设置当前路径
     */
    void setCurrentPath(const QString& path);

    /**
     * @brief 设置是否可以返回上级目录
     */
    void setUpEnabled(bool enabled);

    /**
     * @brief 设置选中文件数量
     */
    void setSelectedCount(int count);

signals:
    /**
     * @brief 返回上级目录信号
     */
    void goUp();

    /**
     * @brief 导航到主目录信号
     */
    void goHome();

    /**
     * @brief 导航到根目录信号
     */
    void goRoot();

    /**
     * @brief 刷新信号
     */
    void refresh();

    /**
     * @brief 路径改变信号
     */
    void pathChanged(const QString& path);

    /**
     * @brief 复制信号
     */
    void copy();

    /**
     * @brief 剪切信号
     */
    void cut();

    /**
     * @brief 粘贴信号
     */
    void paste();

    /**
     * @brief 删除信号
     */
    void remove();

    /**
     * @brief 新建文件夹信号
     */
    void newFolder();

    /**
     * @brief 视图模式改变信号
     */
    void viewModeChanged(int mode);

protected:
    /**
     * @brief 绘制事件
     */
    void paintEvent(QPaintEvent* event) override;

private slots:
    /**
     * @brief 路径输入确认
     */
    void onPathReturnPressed();

private:
    void setupUI();
    void setupConnections();

    QPushButton* m_btnUp;
    QPushButton* m_btnHome;
    QPushButton* m_btnRoot;
    QPushButton* m_btnRefresh;
    QLineEdit* m_pathEdit;
    QPushButton* m_btnCopy;
    QPushButton* m_btnCut;
    QPushButton* m_btnPaste;
    QPushButton* m_btnDelete;
    QPushButton* m_btnNewFolder;
    QPushButton* m_btnListView;
    QPushButton* m_btnIconView;
    QLabel* m_statusLabel;
};

#endif // TOOLBAR_H
