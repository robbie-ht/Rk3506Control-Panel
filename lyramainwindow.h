#ifndef LYRAMAINWINDOW_H
#define LYRAMAINWINDOW_H

#include <QWidget>

class Sidebar;
class ContentArea;
class PowerButton;

/**
 * @brief LyraShell 主窗口
 */
class LyraMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LyraMainWindow(QWidget* parent = nullptr);
    ~LyraMainWindow();

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

#endif // LYRAMAINWINDOW_H
