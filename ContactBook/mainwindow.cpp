/****************************************************************************
**
** 主窗口实现
**
****************************************************************************/

#include "mainwindow.h"
#include "contactdialog.h"
#include "contactfilterproxy.h"
#include "calldialog.h"
#include <QToolBar>
#include <QLineEdit>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QRegularExpression>
#include <QFileDialog>
#include <QSet>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , view(nullptr)
    , model(nullptr)
    , proxy(nullptr)
    , groupProxy(nullptr)
    , groupCombo(nullptr)
    , recentDock(nullptr)
    , contextRow(-1)
{
    setWindowTitle(tr("通讯录"));
    resize(900, 600);

    setupUi();              // 构建界面
    mgr.load("contacts.json");
    refreshTable();
}

/*——————————————————————————————————————————————————————————————————
 * 界面构建
 *——————————————————————————————————————————————————————————————————*/
void MainWindow::setupUi()
{


    /* 1. 数据链：源模型 → 姓名/电话代理 → 分组代理 → 视图 */
    view  = new QTableView(this);
    model = new QStandardItemModel(0, 9, this);

    proxy       = new ContactFilterProxy(this);
    proxy->setSourceModel(model);

    groupProxy  = new GroupFilterProxy(this);
    groupProxy->setSourceModel(proxy);
    view->setModel(groupProxy);

    /* 2. 表头 */
    model->setHorizontalHeaderLabels({
        tr("姓名"),  tr("电话"),  tr("公司"),  tr("职位"),
        tr("邮箱"),  tr("地址"),  tr("生日"),  tr("备注"),  tr("分组")
    });

    /* 3. 表格行为 */
    view->setSortingEnabled(true);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->verticalHeader()->setVisible(false);

    /* 4. 工具栏 */
    QToolBar *tb = addToolBar("Tools");
    tb->setMovable(false);

    /* 4-1 搜索输入框 */
    QLineEdit *search = new QLineEdit;
    search->setClearButtonEnabled(true);
    search->setPlaceholderText(tr("姓名或电话"));
    tb->addWidget(new QLabel(tr("搜索:")));
    tb->addWidget(search);

    /* 4-2 分组下拉框 */
    groupCombo = new QComboBox;
    groupCombo->addItem(tr("全部"), QVariant());
    tb->addSeparator();
    tb->addWidget(new QLabel(tr("分组:")));
    tb->addWidget(groupCombo);

    /* 5. 信号连接 */
    connect(search, &QLineEdit::textChanged,
            this, [this](const QString &text){
                proxy->setFilterKeyColumn(-1);
                proxy->setFilterRegularExpression(
                    QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
            });

    connect(groupCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]{
                groupProxy->setGroup(groupCombo->currentData().toString().trimmed());
                groupProxy->invalidate();
            });

    /* 6. 布局 */
    view->horizontalHeader()->setStretchLastSection(true);
    setCentralWidget(view);

    /* 7. 工具栏按钮 */
    tb->addSeparator();
    tb->addAction(tr("添加"),  this, &MainWindow::addContact);
    tb->addAction(tr("导出CSV"), this, &MainWindow::exportToCSV);
    tb->addAction(tr("导入CSV"), this, &MainWindow::importFromCSV);
    QAction *actRecent = tb->addAction(tr("最近"));
    connect(actRecent, &QAction::triggered,
            [this]{ recentDock->setVisible(!recentDock->isVisible()); });

    /* 8. 最近联系人停靠窗 */
    recentDock = new RecentContactDock(&mgr, this);
    addDockWidget(Qt::RightDockWidgetArea, recentDock);
    recentDock->hide();

    /* 9. 右键菜单 */
    connect(view, &QTableView::customContextMenuRequested,
            this, [this](const QPoint &pos){
                const QModelIndex proxyIdx = view->indexAt(pos);
                if (!proxyIdx.isValid()) return;

                const QModelIndex srcIdx = groupProxy->mapToSource(proxyIdx);
                if (!srcIdx.isValid()) return;

                contextName  = srcIdx.sibling(srcIdx.row(), 0).data().toString();
                contextPhone = srcIdx.sibling(srcIdx.row(), 1).data().toString();

                QMenu menu(this);
                QAction *edit = menu.addAction(tr("编辑"));
                QAction *del  = menu.addAction(tr("删除"));
                QAction *call = menu.addAction(tr("拨打"));

                connect(edit, &QAction::triggered, this, &MainWindow::editContact);
                connect(del,  &QAction::triggered, this, &MainWindow::deleteContact);
                connect(call, &QAction::triggered, this, &MainWindow::callContact);

                menu.exec(view->viewport()->mapToGlobal(pos));
            });
}

/*——————————————————————————————————————————————————————————————————
 * 表格刷新
 *——————————————————————————————————————————————————————————————————*/
void MainWindow::refreshTable()
{
    model->removeRows(0, model->rowCount());
    QSet<QString> groups;

    for (const Contact &c : std::as_const(mgr.contacts)) {
        groups.insert(c.group.trimmed());

        QList<QStandardItem *> row;
        row << new QStandardItem(c.name)
            << new QStandardItem(c.phone)
            << new QStandardItem(c.company)
            << new QStandardItem(c.job)
            << new QStandardItem(c.email)
            << new QStandardItem(c.address)
            << new QStandardItem(c.birthday.toString(Qt::ISODate))
            << new QStandardItem(c.note)
            << new QStandardItem(c.group.trimmed());
        model->appendRow(row);
    }

    /* 同步分组下拉框 */
    groupCombo->clear();
    groupCombo->addItem(tr("全部"), QVariant());
    for (const QString &g : groups)
        groupCombo->addItem(g, g);
    groupCombo->setCurrentIndex(0);
}

/*——————————————————————————————————————————————————————————————————
 * 增删改查 / 导入导出 / 拨打
 *——————————————————————————————————————————————————————————————————*/
void MainWindow::addContact()
{
    ContactDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Contact c = dlg.contact();
        if (!c.isEmpty()) {
            mgr.contacts.append(c);
            refreshTable();
            mgr.save("contacts.json");
        }
    }
}

void MainWindow::editContact()
{
    auto it = std::find_if(mgr.contacts.begin(), mgr.contacts.end(),
                           [&](const Contact &c){
                               return c.name == contextName && c.phone == contextPhone;
                           });
    if (it == mgr.contacts.end()) {
        QMessageBox::information(this, tr("提示"), tr("请单击要编辑的行！"));
        return;
    }

    ContactDialog dlg(this, *it);
    if (dlg.exec() == QDialog::Accepted) {
        *it = dlg.contact();
        refreshTable();
        mgr.save("contacts.json");
    }
}

void MainWindow::deleteContact()
{
    auto it = std::find_if(mgr.contacts.begin(), mgr.contacts.end(),
                           [&](const Contact &c){
                               return c.name == contextName && c.phone == contextPhone;
                           });
    if (it == mgr.contacts.end()) {
        QMessageBox::information(this, tr("提示"), tr("请单击要删除的行！"));
        return;
    }

    if (QMessageBox::question(this, tr("删除确认"),
                              tr("确定删除【%1】？").arg(it->name))
        == QMessageBox::Yes) {
        mgr.contacts.erase(it);
        refreshTable();
        mgr.save("contacts.json");
    }
}

void MainWindow::exportToCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出CSV"),
                                                    QDir::homePath(),
                                                    tr("CSV Files (*.csv)"));
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("导出失败"), tr("无法打开文件以导出CSV"));
        return;
    }

    QTextStream out(&file);
    out << "姓名,电话,公司,职位,邮箱,地址,生日,备注,分组\n";
    for (const Contact &c : std::as_const(mgr.contacts)) {
        out << c.name << ',' << c.phone << ',' << c.company << ','
            << c.job << ',' << c.email << ',' << c.address << ','
            << c.birthday.toString(Qt::ISODate) << ','
            << c.note << ',' << c.group << '\n';
    }
    file.close();
    QMessageBox::information(this, tr("导出成功"), tr("通讯录已导出为CSV文件"));
}

void MainWindow::importFromCSV()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("导入CSV"),
                                                    QDir::homePath(),
                                                    tr("CSV Files (*.csv)"));
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("导入失败"), tr("无法打开CSV文件以导入"));
        return;
    }

    QTextStream in(&file);
    in.readLine(); // 跳过表头
    mgr.contacts.clear();

    while (!in.atEnd()) {
        const QString line = in.readLine();
        const QStringList fields = line.split(',');
        if (fields.size() < 9) continue;

        Contact c;
        c.name    = fields[0];
        c.phone   = fields[1];
        c.company = fields[2];
        c.job     = fields[3];
        c.email   = fields[4];
        c.address = fields[5];
        c.birthday= QDate::fromString(fields[6], Qt::ISODate);
        c.note    = fields[7];
        c.group   = fields[8];
        mgr.contacts.append(c);
    }
    file.close();
    refreshTable();
    mgr.save("contacts.json");
    QMessageBox::information(this, tr("导入成功"), tr("通讯录已从CSV文件导入"));
}

void MainWindow::callContact()
{
    auto it = std::find_if(mgr.contacts.begin(), mgr.contacts.end(),
                           [&](const Contact &c){
                               return c.name == contextName && c.phone == contextPhone;
                           });
    if (it == mgr.contacts.end()) {
        QMessageBox::information(this, tr("提示"), tr("请单击要拨打的行！"));
        return;
    }

    CallDialog dlg(it->name, it->phone, this);
    dlg.exec();

    mgr.updateCallHistory(it->name);
    if (recentDock)
        recentDock->refresh();
}
