/****************************************************************************
**
** 最近联系人停靠窗实现
**
****************************************************************************/

#include "recentcontactdock.h"
#include <QVBoxLayout>
#include <QLabel>

/**
 * @brief 构造函数
 */
RecentContactDock::RecentContactDock(ContactManager *mgr, QWidget *parent)
    : QDockWidget("最近联系", parent)
    , list(new QListWidget(this))
    , m_mgr(mgr)
{
    /* 停靠窗基本属性 */
    setObjectName("RecentContactDock");
    setAllowedAreas(Qt::RightDockWidgetArea);
    setFeatures(DockWidgetClosable | DockWidgetMovable);

    /* 内部布局：标签 + 列表 */
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addWidget(new QLabel("最近联系过的联系人"));
    layout->addWidget(list);
    setWidget(central);

    /* 最小宽度限制，避免过度压缩 */
    setMinimumWidth(180);

    /* 首次加载数据 */
    refresh();
}

/**
 * @brief 从 ContactManager 获取最近联系人并刷新列表
 */
void RecentContactDock::refresh()
{
    list->clear();

    const QVector<Contact> recent = m_mgr->getRecentContacts();
    for (const Contact &c : recent) {
        list->addItem(QString("%1 (%2)  %3")
                          .arg(c.name,
                               c.phone,
                               c.lastCallTime.toString("MM-dd hh:mm")));
    }
}

/**
 * @brief 显示/隐藏停靠窗
 */
void RecentContactDock::toggleVisible()
{
    setVisible(!isVisible());
}
