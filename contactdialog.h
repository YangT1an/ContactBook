/****************************************************************************
**
** 联系人编辑/添加对话框
** 功能：以表单方式录入或修改联系人全部信息
**
****************************************************************************/

#pragma once
#include <QDialog>
#include "contact.h"

/**
 * @class ContactDialog
 * @brief 用于“添加”或“编辑”联系人的模态对话框
 */
class ContactDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口
     * @param c      待编辑的联系人；传入空对象时为“添加”模式
     */
    explicit ContactDialog(QWidget *parent = nullptr,
                           const Contact &c = {});

    /**
     * @brief 获取当前表单内填写的联系人数据
     * @return Contact 结构体实例
     */
    Contact contact() const;

private:
    /** 前向声明的私有 UI 结构体，隐藏实现细节 */
    struct Ui;
    Ui *ui;
};
