/****************************************************************************
**
** 通话对话框实现
**
****************************************************************************/

#include "callDialog.h"
#include <QVBoxLayout>
#include <QFont>

CallDialog::CallDialog(const QString &name,
                       const QString &phone,
                       QWidget *parent)
    : QDialog(parent)
    , titleLabel(new QLabel(this))
    , endButton(new QPushButton(this))
{
    /* 窗口基本属性 */
    setWindowTitle(QStringLiteral("通话中"));
    setModal(true);                     // 模态对话框

    /* 通话提示标签 */
    titleLabel->setText(QStringLiteral("正在与 %1 (%2) 通话").arg(name, phone));
    titleLabel->setFont(QFont("Arial", 14, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);

    /* 结束通话按钮 */
    endButton->setText(QStringLiteral("结束通话"));

    /* 布局：垂直居中显示标签与按钮 */
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(endButton);
    layout->setAlignment(Qt::AlignCenter);

    /* 信号连接：点击按钮结束通话 */
    connect(endButton, &QPushButton::clicked,
            this,        &CallDialog::endCall);
}

/**
 * @brief 结束通话槽函数
 * 关闭对话框并返回主界面
 */
void CallDialog::endCall()
{
    close();
}
