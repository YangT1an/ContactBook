/****************************************************************************
**
** 最近联系人停靠窗
** 功能：以停靠窗口形式展示最近拨打过的联系人（最多 5 条）
**
****************************************************************************/

#pragma once
#include <QDockWidget>
#include <QListWidget>
#include "contactmanager.h"

class ContactManager;

/**
 * @class RecentContactDock
 * @brief 右侧停靠窗口，显示最近联系过的联系人列表
 */
class RecentContactDock : public QDockWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param mgr    联系人管理器，用于获取最近联系人数据
     * @param parent 父窗口
     */
    explicit RecentContactDock(ContactManager *mgr, QWidget *parent = nullptr);

public slots:
    /**
     * @brief 重新从 ContactManager 拉取数据并刷新列表
     */
    void refresh();

    /**
     * @brief 切换停靠窗的显示/隐藏状态
     */
    void toggleVisible();

private:
    QListWidget    *list;   /**< 列表控件，用于展示联系人 */
    ContactManager *m_mgr;  /**< 联系人管理器指针 */
};
