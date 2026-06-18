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
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

FileManagerWidget::FileManagerWidget(QWidget* parent)
    : QWidget(parent)
    , m_clipboardIsCut(false)
{
}

FileManagerWidget::~FileManagerWidget()
{
}

void FileManagerWidget::initialize()
{
    setupUI();
    setupConnections();
    loadStyleSheet();
    m_fileManager->goHome();
}

void FileManagerWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_toolBar = new ToolBar(this);
    mainLayout->addWidget(m_toolBar);

    m_fileListView = new FileListView(this);
    mainLayout->addWidget(m_fileListView, 1);

    m_statusBar = new StatusBar(this);
    mainLayout->addWidget(m_statusBar);

    m_fileManager = new FileManager(this);
}

void FileManagerWidget::setupConnections()
{
    connect(m_fileManager, &FileManager::directoryChanged, this, &FileManagerWidget::onDirectoryChanged);
    connect(m_fileManager, &FileManager::operationCompleted, this, &FileManagerWidget::onOperationCompleted);
    connect(m_fileManager, &FileManager::errorOccurred, this, &FileManagerWidget::onErrorOccurred);

    connect(m_fileListView, &FileListView::fileDoubleClicked, this, &FileManagerWidget::onFileDoubleClicked);
    connect(m_fileListView, &FileListView::selectionChanged, this, &FileManagerWidget::onSelectionChanged);

    connect(m_toolBar, &ToolBar::goUp, [this]() { m_fileManager->goUp(); });
    connect(m_toolBar, &ToolBar::goHome, [this]() { m_fileManager->goHome(); });
    connect(m_toolBar, &ToolBar::goRoot, [this]() { m_fileManager->goRoot(); });
    connect(m_toolBar, &ToolBar::refresh, [this]() { m_fileManager->refresh(); });
    connect(m_toolBar, &ToolBar::pathChanged, [this](const QString& path) { m_fileManager->setCurrentPath(path); });
    connect(m_toolBar, &ToolBar::copy, this, &FileManagerWidget::onCopy);
    connect(m_toolBar, &ToolBar::cut, this, &FileManagerWidget::onCut);
    connect(m_toolBar, &ToolBar::paste, this, &FileManagerWidget::onPaste);
    connect(m_toolBar, &ToolBar::remove, this, &FileManagerWidget::onDelete);
    connect(m_toolBar, &ToolBar::newFolder, this, &FileManagerWidget::onNewFolder);
    connect(m_toolBar, &ToolBar::viewModeChanged, [this](int mode) {
        m_fileListView->setViewMode(static_cast<FileListView::ViewMode>(mode));
    });
}

void FileManagerWidget::loadStyleSheet()
{
    // 样式由父窗口统一管理
}

void FileManagerWidget::onDirectoryChanged(const QString& path)
{
    m_fileListView->setRootPath(path);
    m_toolBar->setCurrentPath(path);
    m_toolBar->setUpEnabled(m_fileManager->canGoUp());
    updateStatusBar();
}

void FileManagerWidget::onFileDoubleClicked(const QString& path)
{
    QFileInfo info(path);
    if (info.isDir()) {
        m_fileManager->setCurrentPath(path);
    }
}

void FileManagerWidget::onSelectionChanged(const QStringList& files)
{
    m_selectedFiles = files;
    m_toolBar->setSelectedCount(files.size());
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

void FileManagerWidget::onCopy()
{
    m_clipboardFiles = m_selectedFiles;
    m_clipboardIsCut = false;
    m_statusBar->setMessage(QString("已复制 %1 个项目").arg(m_clipboardFiles.size()));
}

void FileManagerWidget::onCut()
{
    m_clipboardFiles = m_selectedFiles;
    m_clipboardIsCut = true;
    m_statusBar->setMessage(QString("已剪切 %1 个项目").arg(m_clipboardFiles.size()));
}

void FileManagerWidget::onPaste()
{
    if (m_clipboardFiles.isEmpty()) return;

    QString destDir = m_fileManager->currentPath();
    for (const QString& file : m_clipboardFiles) {
        bool success = m_fileManager->copy(file, destDir);
        if (m_clipboardIsCut && success) {
            m_fileManager->remove(file);
        }
    }

    m_clipboardFiles.clear();
    m_clipboardIsCut = false;
    m_statusBar->setMessage("粘贴完成");
}

void FileManagerWidget::onDelete()
{
    if (m_selectedFiles.isEmpty()) return;

    ConfirmDeleteDialog dialog(m_selectedFiles, this);
    if (dialog.exec() == QDialog::Accepted) {
        for (const QString& file : m_selectedFiles) {
            m_fileManager->remove(file);
        }
        m_selectedFiles.clear();
        m_toolBar->setSelectedCount(0);
    }
}

void FileManagerWidget::onNewFolder()
{
    NewFolderDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.folderName();
        if (!name.isEmpty()) {
            m_fileManager->mkdir(name);
        }
    }
}

void FileManagerWidget::onRename()
{
    if (m_selectedFiles.size() != 1) return;

    QFileInfo info(m_selectedFiles.first());
    RenameDialog dialog(info.fileName(), this);
    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.newName();
        if (!newName.isEmpty()) {
            m_fileManager->rename(m_selectedFiles.first(), newName);
        }
    }
}

void FileManagerWidget::updateStatusBar()
{
    QString path = m_fileManager->currentPath();
    m_statusBar->setCurrentPath(path);

    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    int files = 0, dirs = 0;
    for (const QFileInfo& info : entries) {
        if (info.isDir()) dirs++;
        else files++;
    }
    m_statusBar->setFileCount(files, dirs);
}

void FileManagerWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
