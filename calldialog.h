/****************************************************************************
**
** 通话对话框模块
** 功能：弹出一个模态窗口，显示正在与指定联系人通话，
**       并提供“结束通话”按钮。
**
** 主要类：
**   CallDialog
**
****************************************************************************/

#pragma once
#include <QDialog>
#include <QPushButton>
#include <QLabel>

class CallDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param name  联系人姓名
     * @param phone 联系人电话
     * @param parent 父窗口指针
     */
    explicit CallDialog(const QString &name,
                        const QString &phone,
                        QWidget *parent = nullptr);

private slots:
    /** 结束通话并关闭窗口 */
    void endCall();

private:
    QLabel      *titleLabel; /**< 显示通话提示信息 */
    QPushButton *endButton;  /**< “结束通话”按钮 */
};
