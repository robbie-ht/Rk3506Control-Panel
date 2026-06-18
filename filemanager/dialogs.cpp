#include "dialogs.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileInfo>

// NewFolderDialog
NewFolderDialog::NewFolderDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
}

NewFolderDialog::~NewFolderDialog()
{
}

void NewFolderDialog::setupUI()
{
    setWindowTitle("新建文件夹");
    setFixedSize(300, 150);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(16);

    QLabel* label = new QLabel("请输入文件夹名称:", this);
    layout->addWidget(label);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("新建文件夹");
    m_nameEdit->setText("新建文件夹");
    m_nameEdit->selectAll();
    layout->addWidget(m_nameEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_btnCancel = new QPushButton("取消", this);
    m_btnOk = new QPushButton("确定", this);
    m_btnOk->setDefault(true);

    btnLayout->addWidget(m_btnCancel);
    btnLayout->addWidget(m_btnOk);
    layout->addLayout(btnLayout);

    connect(m_btnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_nameEdit, &QLineEdit::returnPressed, this, &QDialog::accept);
}

QString NewFolderDialog::folderName() const
{
    return m_nameEdit->text().trimmed();
}

// RenameDialog
RenameDialog::RenameDialog(const QString& currentName, QWidget* parent)
    : QDialog(parent)
    , m_currentName(currentName)
{
    setupUI();
}

RenameDialog::~RenameDialog()
{
}

void RenameDialog::setupUI()
{
    setWindowTitle("重命名");
    setFixedSize(300, 150);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(16);

    QLabel* label = new QLabel("请输入新名称:", this);
    layout->addWidget(label);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setText(m_currentName);
    m_nameEdit->selectAll();
    layout->addWidget(m_nameEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_btnCancel = new QPushButton("取消", this);
    m_btnOk = new QPushButton("确定", this);
    m_btnOk->setDefault(true);

    btnLayout->addWidget(m_btnCancel);
    btnLayout->addWidget(m_btnOk);
    layout->addLayout(btnLayout);

    connect(m_btnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_nameEdit, &QLineEdit::returnPressed, this, &QDialog::accept);
}

QString RenameDialog::newName() const
{
    return m_nameEdit->text().trimmed();
}

// ConfirmDeleteDialog
ConfirmDeleteDialog::ConfirmDeleteDialog(const QStringList& files, QWidget* parent)
    : QDialog(parent)
    , m_files(files)
{
    setupUI();
}

ConfirmDeleteDialog::~ConfirmDeleteDialog()
{
}

void ConfirmDeleteDialog::setupUI()
{
    setWindowTitle("确认删除");
    setFixedSize(400, 200);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(16);

    QString message;
    if (m_files.size() == 1) {
        QFileInfo info(m_files.first());
        message = QString("确定要删除 \"%1\" 吗？").arg(info.fileName());
    } else {
        message = QString("确定要删除这 %1 个项目吗？").arg(m_files.size());
    }

    m_messageLabel = new QLabel(message, this);
    m_messageLabel->setWordWrap(true);
    layout->addWidget(m_messageLabel);

    QLabel* warningLabel = new QLabel("此操作不可撤销。", this);
    warningLabel->setStyleSheet("color: #ff6b6b;");
    layout->addWidget(warningLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_btnCancel = new QPushButton("取消", this);
    m_btnDelete = new QPushButton("删除", this);
    m_btnDelete->setObjectName("deleteButton");

    btnLayout->addWidget(m_btnCancel);
    btnLayout->addWidget(m_btnDelete);
    layout->addLayout(btnLayout);

    connect(m_btnDelete, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

// MessageDialog
MessageDialog::MessageDialog(Type type, const QString& title, const QString& message, QWidget* parent)
    : QDialog(parent)
    , m_type(type)
    , m_title(title)
    , m_message(message)
{
    setupUI();
}

MessageDialog::~MessageDialog()
{
}

void MessageDialog::setupUI()
{
    setWindowTitle(m_title);
    setFixedSize(350, 150);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(16);

    QHBoxLayout* contentLayout = new QHBoxLayout();

    QString iconText;
    switch (m_type) {
    case Info:
        iconText = "ℹ️";
        break;
    case Warning:
        iconText = "⚠️";
        break;
    case Error:
        iconText = "❌";
        break;
    }

    m_iconLabel = new QLabel(iconText, this);
    m_iconLabel->setFixedSize(48, 48);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setStyleSheet("font-size: 32px;");
    contentLayout->addWidget(m_iconLabel);

    m_messageLabel = new QLabel(m_message, this);
    m_messageLabel->setWordWrap(true);
    contentLayout->addWidget(m_messageLabel, 1);

    layout->addLayout(contentLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_btnOk = new QPushButton("确定", this);
    m_btnOk->setDefault(true);

    btnLayout->addWidget(m_btnOk);
    layout->addLayout(btnLayout);

    connect(m_btnOk, &QPushButton::clicked, this, &QDialog::accept);
}
