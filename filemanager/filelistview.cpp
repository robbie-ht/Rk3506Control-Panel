#include "filelistview.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QStyleOption>

FileListView::FileListView(QWidget* parent)
    : QWidget(parent)
    , m_viewMode(ListView)
{
    setupUI();
    setupConnections();
}

FileListView::~FileListView()
{
}

void FileListView::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 创建文件系统模型
    m_model = new QFileSystemModel(this);
    m_model->setReadOnly(false);
    m_model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    // 创建树形视图（默认）
    m_treeView = new QTreeView(this);
    m_treeView->setModel(m_model);
    m_treeView->setRootIndex(m_model->index(QDir::homePath()));
    m_treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_treeView->setSortingEnabled(true);
    m_treeView->sortByColumn(0, Qt::AscendingOrder);
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeView->setAnimated(true);
    m_treeView->setHeaderHidden(false);

    // 设置列宽
    m_treeView->header()->setStretchLastSection(false);
    m_treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // 创建列表视图
    m_listView = new QListView(this);
    m_listView->setModel(m_model);
    m_listView->setRootIndex(m_model->index(QDir::homePath()));
    m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_listView->setViewMode(QListView::IconMode);
    m_listView->setGridSize(QSize(80, 80));
    m_listView->setIconSize(QSize(48, 48));
    m_listView->setMovement(QListView::Snap);
    m_listView->setResizeMode(QListView::Adjust);
    m_listView->setWrapping(true);
    m_listView->setContextMenuPolicy(Qt::CustomContextMenu);

    layout->addWidget(m_treeView);
    layout->addWidget(m_listView);

    // 默认显示树形视图
    m_treeView->show();
    m_listView->hide();
}

void FileListView::setupConnections()
{
    connect(m_treeView, &QTreeView::doubleClicked, this, &FileListView::onDoubleClicked);
    connect(m_listView, &QListView::doubleClicked, this, &FileListView::onDoubleClicked);

    connect(m_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &FileListView::onSelectionChanged);
    connect(m_listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &FileListView::onSelectionChanged);

    // 长按/右键菜单
    connect(m_treeView, &QTreeView::customContextMenuRequested, this, &FileListView::onContextMenu);
    connect(m_listView, &QListView::customContextMenuRequested, this, &FileListView::onContextMenu);
}

void FileListView::setRootPath(const QString& path)
{
    m_currentPath = path;
    QModelIndex index = m_model->setRootPath(path);
    m_treeView->setRootIndex(index);
    m_listView->setRootIndex(index);
}

QStringList FileListView::selectedFiles() const
{
    QStringList files;
    QItemSelectionModel* selectionModel = nullptr;

    if (m_viewMode == ListView) {
        selectionModel = m_treeView->selectionModel();
    } else {
        selectionModel = m_listView->selectionModel();
    }

    if (selectionModel) {
        QModelIndexList indexes = selectionModel->selectedIndexes();
        for (const QModelIndex& index : indexes) {
            if (index.column() == 0) {
                files << m_model->filePath(index);
            }
        }
    }

    return files;
}

QString FileListView::selectedFile() const
{
    QStringList files = selectedFiles();
    return files.isEmpty() ? QString() : files.first();
}

void FileListView::setViewMode(ViewMode mode)
{
    m_viewMode = mode;

    if (mode == ListView) {
        m_treeView->show();
        m_listView->hide();
    } else {
        m_treeView->hide();
        m_listView->show();
    }
}

FileListView::ViewMode FileListView::viewMode() const
{
    return m_viewMode;
}

QFileSystemModel* FileListView::model() const
{
    return m_model;
}

void FileListView::onDoubleClicked(const QModelIndex& index)
{
    QString path = m_model->filePath(index);
    QFileInfo info(path);

    if (info.isDir()) {
        setRootPath(path);
        emit fileDoubleClicked(path);
    } else {
        emit fileDoubleClicked(path);
    }
}

void FileListView::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)
    emit selectionChanged(selectedFiles());
}

QString FileListView::currentPath() const
{
    return m_currentPath;
}

void FileListView::onContextMenu(const QPoint& pos)
{
    QWidget* sender = qobject_cast<QWidget*>(QObject::sender());
    if (!sender) return;

    QModelIndex index;
    QString path;

    if (sender == m_treeView) {
        index = m_treeView->indexAt(pos);
        if (index.isValid()) {
            path = m_model->filePath(index);
        }
    } else {
        index = m_listView->indexAt(pos);
        if (index.isValid()) {
            path = m_model->filePath(index);
        }
    }

    // 如果没点到文件上，传当前目录路径
    if (path.isEmpty()) {
        path = m_currentPath;
    }

    QPoint globalPos = sender->mapToGlobal(pos);
    emit contextMenuRequested(path, globalPos);
}

void FileListView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
