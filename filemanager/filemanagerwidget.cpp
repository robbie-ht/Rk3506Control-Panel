#include "filemanagerwidget.h"
#include "filemanager.h"
#include "filelistview.h"
#include "toolbar.h"
#include "statusbar.h"
#include "dialogs.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QMenu>
#include <QAction>

FileManagerWidget::FileManagerWidget(QWidget* parent)
    : QWidget(parent)
    , m_clipboardIsCut(false)
    , m_clipboardSourcePanel(-1)
    , m_activePanel(0)
{
}

FileManagerWidget::~FileManagerWidget()
{
}

void FileManagerWidget::initialize()
{
    setupUI();
    setupConnections();

    // 左栏默认打开主目录，右栏打开根目录
    m_leftFileManager->goHome();
    m_rightFileManager->goRoot();
}

void FileManagerWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 工具栏
    m_toolBar = new ToolBar(this);
    mainLayout->addWidget(m_toolBar);

    // 双栏区域
    QWidget* panelsWidget = new QWidget(this);
    QHBoxLayout* panelsLayout = new QHBoxLayout(panelsWidget);
    panelsLayout->setContentsMargins(0, 0, 0, 0);
    panelsLayout->setSpacing(1);

    // 左栏
    QWidget* leftPanel = new QWidget(panelsWidget);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);

    m_leftPathLabel = new QLabel(leftPanel);
    m_leftPathLabel->setObjectName("panelPathLabel");
    m_leftPathLabel->setFixedHeight(24);
    m_leftPathLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    leftLayout->addWidget(m_leftPathLabel);

    m_leftView = new FileListView(leftPanel);
    leftLayout->addWidget(m_leftView, 1);

    // 分隔线
    QWidget* separator = new QWidget(panelsWidget);
    separator->setFixedWidth(1);
    separator->setStyleSheet("background-color: #3a3a5c;");

    // 右栏
    QWidget* rightPanel = new QWidget(panelsWidget);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    m_rightPathLabel = new QLabel(rightPanel);
    m_rightPathLabel->setObjectName("panelPathLabel");
    m_rightPathLabel->setFixedHeight(24);
    m_rightPathLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    rightLayout->addWidget(m_rightPathLabel);

    m_rightView = new FileListView(rightPanel);
    rightLayout->addWidget(m_rightView, 1);

    panelsLayout->addWidget(leftPanel, 1);
    panelsLayout->addWidget(separator);
    panelsLayout->addWidget(rightPanel, 1);

    mainLayout->addWidget(panelsWidget, 1);

    // 状态栏
    m_statusBar = new StatusBar(this);
    mainLayout->addWidget(m_statusBar);

    // 两个独立的文件管理器
    m_leftFileManager = new FileManager(this);
    m_rightFileManager = new FileManager(this);
}

void FileManagerWidget::setupConnections()
{
    // 左栏信号
    connect(m_leftView, &FileListView::fileDoubleClicked, this, &FileManagerWidget::onLeftDoubleClicked);
    connect(m_leftView, &FileListView::contextMenuRequested, this, &FileManagerWidget::onLeftContextMenu);
    connect(m_leftView, &FileListView::selectionChanged, [this](const QStringList&) {
        m_activePanel = 0;
        m_toolBar->setUpEnabled(m_leftFileManager->canGoUp());
    });
    connect(m_leftFileManager, &FileManager::directoryChanged, [this](const QString& path) {
        m_leftView->setRootPath(path);
        m_leftPathLabel->setText(path);
        if (m_activePanel == 0)
            m_toolBar->setUpEnabled(m_leftFileManager->canGoUp());
        updateStatusBar();
    });
    connect(m_leftFileManager, &FileManager::operationCompleted, this, &FileManagerWidget::onOperationCompleted);
    connect(m_leftFileManager, &FileManager::errorOccurred, this, &FileManagerWidget::onErrorOccurred);

    // 右栏信号
    connect(m_rightView, &FileListView::fileDoubleClicked, this, &FileManagerWidget::onRightDoubleClicked);
    connect(m_rightView, &FileListView::contextMenuRequested, this, &FileManagerWidget::onRightContextMenu);
    connect(m_rightView, &FileListView::selectionChanged, [this](const QStringList&) {
        m_activePanel = 1;
        m_toolBar->setUpEnabled(m_rightFileManager->canGoUp());
    });
    connect(m_rightFileManager, &FileManager::directoryChanged, [this](const QString& path) {
        m_rightView->setRootPath(path);
        m_rightPathLabel->setText(path);
        if (m_activePanel == 1)
            m_toolBar->setUpEnabled(m_rightFileManager->canGoUp());
        updateStatusBar();
    });
    connect(m_rightFileManager, &FileManager::operationCompleted, this, &FileManagerWidget::onOperationCompleted);
    connect(m_rightFileManager, &FileManager::errorOccurred, this, &FileManagerWidget::onErrorOccurred);

    // 工具栏导航 — 控制当前激活的面板
    connect(m_toolBar, &ToolBar::goUp, [this]() {
        FileManager* mgr = (m_activePanel == 0) ? m_leftFileManager : m_rightFileManager;
        mgr->goUp();
    });
    connect(m_toolBar, &ToolBar::goHome, [this]() {
        FileManager* mgr = (m_activePanel == 0) ? m_leftFileManager : m_rightFileManager;
        mgr->goHome();
    });
    connect(m_toolBar, &ToolBar::goRoot, [this]() {
        FileManager* mgr = (m_activePanel == 0) ? m_leftFileManager : m_rightFileManager;
        mgr->goRoot();
    });
    connect(m_toolBar, &ToolBar::refresh, [this]() {
        FileManager* mgr = (m_activePanel == 0) ? m_leftFileManager : m_rightFileManager;
        mgr->refresh();
    });
}

void FileManagerWidget::navigateTo(int panel, const QString& path)
{
    if (panel == 0) {
        m_leftFileManager->setCurrentPath(path);
    } else {
        m_rightFileManager->setCurrentPath(path);
    }
}

void FileManagerWidget::onLeftDoubleClicked(const QString& path)
{
    m_activePanel = 0;
    QFileInfo info(path);
    if (info.isDir()) {
        navigateTo(0, path);
    }
}

void FileManagerWidget::onRightDoubleClicked(const QString& path)
{
    m_activePanel = 1;
    QFileInfo info(path);
    if (info.isDir()) {
        navigateTo(1, path);
    }
}

void FileManagerWidget::onLeftContextMenu(const QString& path, const QPoint& pos)
{
    m_activePanel = 0;
    showContextMenu(0, path, pos);
}

void FileManagerWidget::onRightContextMenu(const QString& path, const QPoint& pos)
{
    m_activePanel = 1;
    showContextMenu(1, path, pos);
}

void FileManagerWidget::showContextMenu(int panel, const QString& path, const QPoint& globalPos)
{
    QFileInfo info(path);
    bool isFile = info.exists() && info.isFile();
    bool isDir = info.exists() && info.isDir();
    bool isOnItem = isFile || isDir;

    // 判断点击的是当前目录的空白区域还是某个文件
    QString currentDir;
    if (panel == 0) {
        currentDir = m_leftFileManager->currentPath();
    } else {
        currentDir = m_rightFileManager->currentPath();
    }

    // 如果点到了目录上，判断是当前面板的根目录还是子目录
    bool isCurrentDir = (isDir && QDir(path) == QDir(currentDir));

    QMenu menu(this);

    if (isOnItem && !isCurrentDir) {
        // 点在文件/子目录上
        QAction* actCopy = menu.addAction("复制");
        QAction* actCut = menu.addAction("剪切");
        QAction* actRename = menu.addAction("重命名");
        menu.addSeparator();
        QAction* actDelete = menu.addAction("删除");

        connect(actCopy, &QAction::triggered, [this, panel, path]() {
            m_clipboardFiles.clear();
            m_clipboardFiles << path;
            m_clipboardIsCut = false;
            m_clipboardSourcePanel = panel;
            m_statusBar->setMessage(QString("已复制: %1").arg(QFileInfo(path).fileName()));
        });

        connect(actCut, &QAction::triggered, [this, panel, path]() {
            m_clipboardFiles.clear();
            m_clipboardFiles << path;
            m_clipboardIsCut = true;
            m_clipboardSourcePanel = panel;
            m_statusBar->setMessage(QString("已剪切: %1").arg(QFileInfo(path).fileName()));
        });

        connect(actRename, &QAction::triggered, [this, panel, path]() {
            doRename(panel, path);
        });

        connect(actDelete, &QAction::triggered, [this, panel, path]() {
            m_clipboardFiles.clear();
            m_clipboardFiles << path;
            doDelete(panel);
        });
    } else {
        // 点在空白区域（当前目录）
        if (!m_clipboardFiles.isEmpty()) {
            QAction* actPaste = menu.addAction("粘贴");
            connect(actPaste, &QAction::triggered, [this, panel]() {
                doPaste(panel);
            });
        }

        QAction* actNewFolder = menu.addAction("新建文件夹");
        connect(actNewFolder, &QAction::triggered, [this, panel]() {
            doNewFolder(panel);
        });
    }

    if (!menu.isEmpty()) {
        menu.exec(globalPos);
    }
}

void FileManagerWidget::doCopy(int panel)
{
    FileListView* view = (panel == 0) ? m_leftView : m_rightView;
    m_clipboardFiles = view->selectedFiles();
    m_clipboardIsCut = false;
    m_clipboardSourcePanel = panel;
    m_statusBar->setMessage(QString("已复制 %1 个项目").arg(m_clipboardFiles.size()));
}

void FileManagerWidget::doCut(int panel)
{
    FileListView* view = (panel == 0) ? m_leftView : m_rightView;
    m_clipboardFiles = view->selectedFiles();
    m_clipboardIsCut = true;
    m_clipboardSourcePanel = panel;
    m_statusBar->setMessage(QString("已剪切 %1 个项目").arg(m_clipboardFiles.size()));
}

void FileManagerWidget::doPaste(int panel)
{
    if (m_clipboardFiles.isEmpty()) return;

    FileManager* destManager = (panel == 0) ? m_leftFileManager : m_rightFileManager;
    FileManager* srcManager = (m_clipboardSourcePanel == 0) ? m_leftFileManager : m_rightFileManager;
    QString destDir = destManager->currentPath();

    for (const QString& file : m_clipboardFiles) {
        bool success = destManager->copy(file, destDir);
        if (m_clipboardIsCut && success) {
            srcManager->remove(file);
        }
    }

    // 刷新源面板
    if (m_clipboardIsCut) {
        srcManager->refresh();
    }

    m_clipboardFiles.clear();
    m_clipboardIsCut = false;
    m_clipboardSourcePanel = -1;
    m_statusBar->setMessage("粘贴完成");
}

void FileManagerWidget::doDelete(int panel)
{
    if (m_clipboardFiles.isEmpty()) return;

    ConfirmDeleteDialog dialog(m_clipboardFiles, this);
    if (dialog.exec() == QDialog::Accepted) {
        FileManager* manager = (panel == 0) ? m_leftFileManager : m_rightFileManager;
        for (const QString& file : m_clipboardFiles) {
            manager->remove(file);
        }
        m_clipboardFiles.clear();
    }
}

void FileManagerWidget::doRename(int panel, const QString& filePath)
{
    QFileInfo info(filePath);
    RenameDialog dialog(info.fileName(), this);
    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.newName();
        if (!newName.isEmpty()) {
            FileManager* manager = (panel == 0) ? m_leftFileManager : m_rightFileManager;
            manager->rename(filePath, newName);
        }
    }
}

void FileManagerWidget::doNewFolder(int panel)
{
    NewFolderDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.folderName();
        if (!name.isEmpty()) {
            FileManager* manager = (panel == 0) ? m_leftFileManager : m_rightFileManager;
            manager->mkdir(name);
        }
    }
}

void FileManagerWidget::onOperationCompleted(bool success, const QString& message)
{
    Q_UNUSED(success)
    m_statusBar->setMessage(message);
}

void FileManagerWidget::onErrorOccurred(const QString& message)
{
    MessageDialog dialog(MessageDialog::Error, "错误", message, this);
    dialog.exec();
}

void FileManagerWidget::updateStatusBar()
{
    // 显示左栏信息
    QString leftPath = m_leftFileManager->currentPath();
    QDir leftDir(leftPath);
    QFileInfoList leftEntries = leftDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    int leftFiles = 0, leftDirs = 0;
    for (const QFileInfo& i : leftEntries) {
        if (i.isDir()) leftDirs++;
        else leftFiles++;
    }

    QString rightPath = m_rightFileManager->currentPath();
    QDir rightDir(rightPath);
    QFileInfoList rightEntries = rightDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    int rightFiles = 0, rightDirs = 0;
    for (const QFileInfo& i : rightEntries) {
        if (i.isDir()) rightDirs++;
        else rightFiles++;
    }

    QString msg = QString("左: %1个文件夹 %2个文件 | 右: %3个文件夹 %4个文件")
                      .arg(leftDirs).arg(leftFiles).arg(rightDirs).arg(rightFiles);
    m_statusBar->setMessage(msg);
}

void FileManagerWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
