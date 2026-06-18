#include "statusbar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>

StatusBar::StatusBar(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

StatusBar::~StatusBar()
{
}

void StatusBar::setupUI()
{
    setObjectName("statusbar");
    setFixedHeight(28);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 12, 0);
    layout->setSpacing(20);

    m_pathLabel = new QLabel(this);
    m_pathLabel->setObjectName("statusPath");

    m_countLabel = new QLabel(this);
    m_countLabel->setObjectName("statusCount");

    m_messageLabel = new QLabel(this);
    m_messageLabel->setObjectName("statusMessage");
    m_messageLabel->setAlignment(Qt::AlignRight);

    layout->addWidget(m_pathLabel);
    layout->addWidget(m_countLabel);
    layout->addStretch();
    layout->addWidget(m_messageLabel);
}

void StatusBar::setFileCount(int files, int dirs)
{
    QString text;
    if (dirs > 0 && files > 0) {
        text = QString("%1 个文件夹, %2 个文件").arg(dirs).arg(files);
    } else if (dirs > 0) {
        text = QString("%1 个文件夹").arg(dirs);
    } else if (files > 0) {
        text = QString("%1 个文件").arg(files);
    }
    m_countLabel->setText(text);
}

void StatusBar::setCurrentPath(const QString& path)
{
    m_pathLabel->setText(path);
}

void StatusBar::setMessage(const QString& message)
{
    m_messageLabel->setText(message);
}

void StatusBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
