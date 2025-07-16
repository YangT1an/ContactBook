/****************************************************************************
**
** 联系人编辑/添加对话框实现
**
****************************************************************************/

#include "contactdialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QDialogButtonBox>

/**
 * @struct ContactDialog::Ui
 * @brief  封装所有控件指针，便于统一管理
 */
struct ContactDialog::Ui
{
    QLineEdit *name;      /**< 姓名      */
    QLineEdit *phone;     /**< 电话      */
    QLineEdit *company;   /**< 公司      */
    QLineEdit *job;       /**< 职位      */
    QLineEdit *email;     /**< 邮箱      */
    QLineEdit *address;   /**< 地址      */
    QLineEdit *group;     /**< 分组      */
    QDateEdit *birthday;  /**< 生日      */
    QTextEdit *note;      /**< 备注      */
};

ContactDialog::ContactDialog(QWidget *parent, const Contact &c)
    : QDialog(parent)
    , ui(new Ui)
{
    /* 窗口标题根据是否为空对象自动切换 */
    setWindowTitle(c.isEmpty() ? QStringLiteral("添加联系人")
                               : QStringLiteral("编辑联系人"));

    /* 构建表单布局 */
    QFormLayout *lay = new QFormLayout(this);

    /* 初始化所有控件并填入已有数据 */
    ui->name    = new QLineEdit(c.name.trimmed());
    ui->phone   = new QLineEdit(c.phone.trimmed());
    ui->company = new QLineEdit(c.company.trimmed());
    ui->job     = new QLineEdit(c.job.trimmed());
    ui->email   = new QLineEdit(c.email.trimmed());
    ui->address = new QLineEdit(c.address.trimmed());
    ui->group   = new QLineEdit(c.group.trimmed());
    ui->birthday= new QDateEdit(c.birthday.isValid() ? c.birthday
                                                      : QDate::currentDate());
    ui->note    = new QTextEdit(c.note);

    /* 添加表单行 */
    lay->addRow(QStringLiteral("姓名"), ui->name);
    lay->addRow(QStringLiteral("电话"), ui->phone);
    lay->addRow(QStringLiteral("公司"), ui->company);
    lay->addRow(QStringLiteral("职位"), ui->job);
    lay->addRow(QStringLiteral("邮箱"), ui->email);
    lay->addRow(QStringLiteral("地址"), ui->address);
    lay->addRow(QStringLiteral("生日"), ui->birthday);
    lay->addRow(QStringLiteral("备注"), ui->note);
    lay->addRow(QStringLiteral("分组"), ui->group);

    /* 确定/取消按钮 */
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                     QDialogButtonBox::Cancel,
                                                 this);
    connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);
    lay->addWidget(box);
}

/**
 * @brief 将界面数据打包为 Contact 结构体
 */
Contact ContactDialog::contact() const
{
    Contact c;
    c.name    = ui->name->text().trimmed();
    c.phone   = ui->phone->text().trimmed();
    c.company = ui->company->text().trimmed();
    c.job     = ui->job->text().trimmed();
    c.email   = ui->email->text().trimmed();
    c.address = ui->address->text().trimmed();
    c.birthday= ui->birthday->date();
    c.note    = ui->note->toPlainText().trimmed();
    c.group   = ui->group->text().trimmed();
    return c;
}
