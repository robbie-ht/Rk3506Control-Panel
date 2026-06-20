#include "filelistview.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>

FileListView::FileListView(QWidget* parent)
    : QWidget(parent)
    , m_viewMode(ListView)
    , m_pressWidget(nullptr)
    , m_longPressTriggered(false)
{
    setupUI();
    setupConnections();

    // 长按定时器
    m_longPressTimer = new QTimer(this);
    m_longPressTimer->setSingleShot(true);
    m_longPressTimer->setInterval(600); // 600ms 触发长按
    connect(m_longPressTimer, &QTimer::timeout, this, &FileListView::onLongPress);
}

FileListView::~FileListView()
{
}

void FileListView::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_model = new QFileSystemModel(this);
    m_model->setReadOnly(false);
    m_model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    // 树形视图
    m_treeView = new QTreeView(this);
    m_treeView->setModel(m_model);
    m_treeView->setRootIndex(m_model->index(QDir::homePath()));
    m_treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_treeView->setSortingEnabled(true);
    m_treeView->sortByColumn(0, Qt::AscendingOrder);
    m_treeView->setAnimated(true);
    m_treeView->setHeaderHidden(false);
    m_treeView->installEventFilter(this);

    // 设置列宽 — 优先显示文件名
    m_treeView->header()->setStretchLastSection(false);
    m_treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_treeView->setColumnHidden(2, true); // 隐藏"类型"列
    m_treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // 列表视图
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
    m_listView->installEventFilter(this);

    layout->addWidget(m_treeView);
    layout->addWidget(m_listView);

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
}

bool FileListView::eventFilter(QObject* obj, QEvent* event)
{
    // 长按检测：press 启动定时器，release 取消
    if (obj == m_treeView || obj == m_listView) {
        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::TouchBegin) {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            m_pressPos = me->pos();
            m_pressWidget = qobject_cast<QWidget*>(obj);
            m_longPressTriggered = false;
            m_longPressTimer->start();
        } else if (event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::TouchEnd) {
            m_longPressTimer->stop();
        } else if (event->type() == QEvent::MouseMove || event->type() == QEvent::TouchUpdate) {
            // 移动超过一定距离取消长按
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            if ((me->pos() - m_pressPos).manhattanLength() > 20) {
                m_longPressTimer->stop();
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void FileListView::onLongPress()
{
    m_longPressTriggered = true;

    if (!m_pressWidget) return;

    QModelIndex index;
    QString path;

    if (m_pressWidget == m_treeView) {
        index = m_treeView->indexAt(m_pressPos);
        if (index.isValid()) {
            path = m_model->filePath(index);
        }
    } else {
        index = m_listView->indexAt(m_pressPos);
        if (index.isValid()) {
            path = m_model->filePath(index);
        }
    }

    if (path.isEmpty()) {
        path = m_currentPath;
    }

    QPoint globalPos = m_pressWidget->mapToGlobal(m_pressPos);
    emit contextMenuRequested(path, globalPos);
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
    QItemSelectionModel* selModel = (m_viewMode == ListView)
        ? m_treeView->selectionModel()
        : m_listView->selectionModel();

    if (selModel) {
        QModelIndexList indexes = selModel->selectedIndexes();
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
    // 长按触发后不响应双击
    if (m_longPressTriggered) {
        m_longPressTriggered = false;
        return;
    }

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

void FileListView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
