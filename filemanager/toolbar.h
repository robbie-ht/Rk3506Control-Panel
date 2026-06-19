#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

/**
 * @brief 工具栏组件
 * 精简版：只保留导航按钮和路径显示
 */
class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget* parent = nullptr);
    ~ToolBar();

    void setCurrentPath(const QString& path);
    void setUpEnabled(bool enabled);

signals:
    void goUp();
    void goHome();
    void goRoot();
    void refresh();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void setupUI();

    QPushButton* m_btnUp;
    QPushButton* m_btnHome;
    QPushButton* m_btnRoot;
    QPushButton* m_btnRefresh;
    QLabel* m_pathLabel;
};

#endif // TOOLBAR_H
