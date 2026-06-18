#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class Sidebar;
class ContentArea;
class PowerButton;

/**
 * @brief 主窗口
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    /**
     * @brief 初始化窗口
     */
    void initialize();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onSidebarItemClicked(int index);

private:
    void setupUI();
    void loadStyleSheet();
    void loadBuiltInApps();
    void updatePowerButtonPosition();

    Sidebar* m_sidebar;
    ContentArea* m_contentArea;
    PowerButton* m_powerButton;
};

#endif // MAINWINDOW_H
