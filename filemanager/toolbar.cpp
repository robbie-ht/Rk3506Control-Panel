#include "toolbar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QStyle>

ToolBar::ToolBar(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

ToolBar::~ToolBar()
{
}

void ToolBar::setupUI()
{
    setObjectName("toolbar");
    setFixedHeight(48);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(8, 4, 8, 4);
    mainLayout->setSpacing(4);

    // 导航按钮
    m_btnUp = new QPushButton("↑", this);
    m_btnUp->setObjectName("navButton");
    m_btnUp->setFixedSize(36, 36);
    m_btnUp->setToolTip("上级目录");
    m_btnUp->setEnabled(false);

    m_btnHome = new QPushButton("🏠", this);
    m_btnHome->setObjectName("navButton");
    m_btnHome->setFixedSize(36, 36);
    m_btnHome->setToolTip("主目录");

    m_btnRoot = new QPushButton("/", this);
    m_btnRoot->setObjectName("navButton");
    m_btnRoot->setFixedSize(36, 36);
    m_btnRoot->setToolTip("根目录");

    m_btnRefresh = new QPushButton("🔄", this);
    m_btnRefresh->setObjectName("navButton");
    m_btnRefresh->setFixedSize(36, 36);
    m_btnRefresh->setToolTip("刷新");

    mainLayout->addWidget(m_btnUp);
    mainLayout->addWidget(m_btnHome);
    mainLayout->addWidget(m_btnRoot);
    mainLayout->addWidget(m_btnRefresh);

    // 分隔符
    QWidget* separator1 = new QWidget(this);
    separator1->setFixedWidth(1);
    separator1->setStyleSheet("background-color: #444444;");
    mainLayout->addWidget(separator1);

    // 路径栏
    m_pathEdit = new QLineEdit(this);
    m_pathEdit->setObjectName("pathEdit");
    m_pathEdit->setPlaceholderText("输入路径...");
    mainLayout->addWidget(m_pathEdit, 1);

    // 分隔符
    QWidget* separator2 = new QWidget(this);
    separator2->setFixedWidth(1);
    separator2->setStyleSheet("background-color: #444444;");
    mainLayout->addWidget(separator2);

    // 操作按钮
    m_btnCopy = new QPushButton("📋", this);
    m_btnCopy->setObjectName("actionButton");
    m_btnCopy->setFixedSize(36, 36);
    m_btnCopy->setToolTip("复制");
    m_btnCopy->setEnabled(false);

    m_btnCut = new QPushButton("✂️", this);
    m_btnCut->setObjectName("actionButton");
    m_btnCut->setFixedSize(36, 36);
    m_btnCut->setToolTip("剪切");
    m_btnCut->setEnabled(false);

    m_btnPaste = new QPushButton("📄", this);
    m_btnPaste->setObjectName("actionButton");
    m_btnPaste->setFixedSize(36, 36);
    m_btnPaste->setToolTip("粘贴");
    m_btnPaste->setEnabled(false);

    m_btnDelete = new QPushButton("🗑️", this);
    m_btnDelete->setObjectName("actionButton");
    m_btnDelete->setFixedSize(36, 36);
    m_btnDelete->setToolTip("删除");
    m_btnDelete->setEnabled(false);

    m_btnNewFolder = new QPushButton("📁", this);
    m_btnNewFolder->setObjectName("actionButton");
    m_btnNewFolder->setFixedSize(36, 36);
    m_btnNewFolder->setToolTip("新建文件夹");

    mainLayout->addWidget(m_btnCopy);
    mainLayout->addWidget(m_btnCut);
    mainLayout->addWidget(m_btnPaste);
    mainLayout->addWidget(m_btnDelete);
    mainLayout->addWidget(m_btnNewFolder);

    // 分隔符
    QWidget* separator3 = new QWidget(this);
    separator3->setFixedWidth(1);
    separator3->setStyleSheet("background-color: #444444;");
    mainLayout->addWidget(separator3);

    // 视图切换按钮
    m_btnListView = new QPushButton("☰", this);
    m_btnListView->setObjectName("viewButton");
    m_btnListView->setFixedSize(36, 36);
    m_btnListView->setToolTip("列表视图");
    m_btnListView->setCheckable(true);
    m_btnListView->setChecked(true);

    m_btnIconView = new QPushButton("⊞", this);
    m_btnIconView->setObjectName("viewButton");
    m_btnIconView->setFixedSize(36, 36);
    m_btnIconView->setToolTip("图标视图");
    m_btnIconView->setCheckable(true);

    mainLayout->addWidget(m_btnListView);
    mainLayout->addWidget(m_btnIconView);

    // 状态标签
    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName("statusLabel");
    m_statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_statusLabel->setMinimumWidth(100);
    mainLayout->addWidget(m_statusLabel);
}

void ToolBar::setupConnections()
{
    connect(m_btnUp, &QPushButton::clicked, this, &ToolBar::goUp);
    connect(m_btnHome, &QPushButton::clicked, this, &ToolBar::goHome);
    connect(m_btnRoot, &QPushButton::clicked, this, &ToolBar::goRoot);
    connect(m_btnRefresh, &QPushButton::clicked, this, &ToolBar::refresh);
    connect(m_pathEdit, &QLineEdit::returnPressed, this, &ToolBar::onPathReturnPressed);

    connect(m_btnCopy, &QPushButton::clicked, this, &ToolBar::copy);
    connect(m_btnCut, &QPushButton::clicked, this, &ToolBar::cut);
    connect(m_btnPaste, &QPushButton::clicked, this, &ToolBar::paste);
    connect(m_btnDelete, &QPushButton::clicked, this, &ToolBar::remove);
    connect(m_btnNewFolder, &QPushButton::clicked, this, &ToolBar::newFolder);

    connect(m_btnListView, &QPushButton::clicked, [this]() {
        m_btnIconView->setChecked(false);
        emit viewModeChanged(0);
    });

    connect(m_btnIconView, &QPushButton::clicked, [this]() {
        m_btnListView->setChecked(false);
        emit viewModeChanged(1);
    });
}

void ToolBar::setCurrentPath(const QString& path)
{
    m_pathEdit->setText(path);
}

void ToolBar::setUpEnabled(bool enabled)
{
    m_btnUp->setEnabled(enabled);
}

void ToolBar::setSelectedCount(int count)
{
    bool hasSelection = count > 0;
    m_btnCopy->setEnabled(hasSelection);
    m_btnCut->setEnabled(hasSelection);
    m_btnDelete->setEnabled(hasSelection);

    if (count > 0) {
        m_statusLabel->setText(QString("已选择 %1 项").arg(count));
    } else {
        m_statusLabel->clear();
    }
}

void ToolBar::onPathReturnPressed()
{
    QString path = m_pathEdit->text();
    if (!path.isEmpty()) {
        emit pathChanged(path);
    }
}

void ToolBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
