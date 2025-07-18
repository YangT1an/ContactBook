/****************************************************************************
**
** 联系人管理器
** 功能：负责联系人数据的持久化（JSON 存取）、通话记录更新与最近联系人查询
**
****************************************************************************/

#pragma once
#include "contact.h"
#include <QVector>
#include <QJsonArray>

/**
 * @class ContactManager
 * @brief 统一管理通讯录数据：加载、保存、更新通话记录、获取最近联系人
 */
class ContactManager
{
public:
    QVector<Contact> contacts;  /**< 内存中的全部联系人数据 */

    /**
     * @brief 从 JSON 文件加载联系人
     * @param path 文件路径
     */
    void load(const QString &path);

    /**
     * @brief 将当前联系人保存为 JSON 文件
     * @param path 目标文件路径
     */
    void save(const QString &path) const;

    /**
     * @brief 把当前联系人列表序列化为 QJsonArray
     * @return 对应的 JSON 数组
     */
    QJsonArray toJson() const;

    /**
     * @brief 从 JSON 数组反序列化联系人
     * @param arr JSON 数组
     */
    void fromJson(const QJsonArray &arr);

    /**
     * @brief 更新指定联系人的通话记录（callCount + 1，并设置 lastCallTime）
     * @param name 联系人姓名
     */
    void updateCallHistory(const QString &name);

    /**
     * @brief 获取最近拨打过的联系人（最多 5 条，按最后通话时间降序）
     * @return 联系人列表
     */
    QVector<Contact> getRecentContacts() const;
};
