/****************************************************************************
**
** 主窗口
** 功能：通讯录主界面，支持增删改查、按姓名/电话模糊搜索、按分组精确过滤、
**       最近联系人停靠窗、CSV 导入导出、右键菜单拨打电话
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QComboBox>

#include "contactmanager.h"
#include "contact.h"
#include "groupfilterproxy.h"
#include "recentcontactdock.h"

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void addContact();      /**< 添加联系人     */
    void editContact();     /**< 编辑联系人     */
    void deleteContact();   /**< 删除联系人     */
    void refreshTable();    /**< 刷新主表格     */
    void exportToCSV();     /**< 导出 CSV       */
    void importFromCSV();   /**< 导入 CSV       */
    void callContact();     /**< 拨打电话       */

private:
    void setupUi();         /**< 构建界面       */

    /* 数据成员 */
    ContactManager           mgr;         /**< 联系人管理器            */
    QTableView              *view;        /**< 主表格视图              */
    QStandardItemModel      *model;       /**< 表格数据模型            */
    QSortFilterProxyModel   *proxy;       /**< 姓名/电话搜索代理       */
    GroupFilterProxy        *groupProxy;  /**< 分组精准过滤代理        */
    QComboBox               *groupCombo;  /**< 分组下拉框              */
    RecentContactDock       *recentDock;  /**< 最近联系人停靠窗        */

    /* 右键菜单相关 */
    int     contextRow   = -1;            /**< 右键行号（已废弃）      */
    QString contextName;                  /**< 右键联系人姓名          */
    QString contextPhone;                 /**< 右键联系人电话          */

signals:
    void recentContactTriggered(const Contact &c); /**< 暂留接口，未使用 */
};

#endif // MAINWINDOW_H
