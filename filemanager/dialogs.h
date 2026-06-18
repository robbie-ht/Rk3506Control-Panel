#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

/**
 * @brief 新建文件夹对话框
 */
class NewFolderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFolderDialog(QWidget* parent = nullptr);
    ~NewFolderDialog();

    /**
     * @brief 获取文件夹名称
     */
    QString folderName() const;

private:
    void setupUI();

    QLineEdit* m_nameEdit;
    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
};

/**
 * @brief 重命名对话框
 */
class RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(const QString& currentName, QWidget* parent = nullptr);
    ~RenameDialog();

    /**
     * @brief 获取新名称
     */
    QString newName() const;

private:
    void setupUI();

    QString m_currentName;
    QLineEdit* m_nameEdit;
    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
};

/**
 * @brief 确认删除对话框
 */
class ConfirmDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDeleteDialog(const QStringList& files, QWidget* parent = nullptr);
    ~ConfirmDeleteDialog();

private:
    void setupUI();

    QStringList m_files;
    QLabel* m_messageLabel;
    QPushButton* m_btnDelete;
    QPushButton* m_btnCancel;
};

/**
 * @brief 错误/信息对话框
 */
class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    enum Type {
        Info,
        Warning,
        Error
    };

    explicit MessageDialog(Type type, const QString& title, const QString& message, QWidget* parent = nullptr);
    ~MessageDialog();

private:
    void setupUI();

    Type m_type;
    QString m_title;
    QString m_message;
    QLabel* m_iconLabel;
    QLabel* m_messageLabel;
    QPushButton* m_btnOk;
};

#endif // DIALOGS_H
