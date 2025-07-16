/****************************************************************************
**
** 联系人管理器实现
**
****************************************************************************/

#include "contactmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <algorithm>
#include <QJsonObject>

/**
 * @brief 从 JSON 文件加载联系人
 */
void ContactManager::load(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    const QByteArray raw = file.readAll();
    fromJson(QJsonDocument::fromJson(raw).array());
}

/**
 * @brief 将联系人数据保存到 JSON 文件
 */
void ContactManager::save(const QString &path) const
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
        file.write(QJsonDocument(toJson()).toJson());
}

/**
 * @brief 把联系人列表序列化为 JSON 数组
 */
QJsonArray ContactManager::toJson() const
{
    QJsonArray arr;
    for (const Contact &c : contacts) {
        QJsonObject obj;
        obj["name"]        = c.name;
        obj["phone"]       = c.phone;
        obj["company"]     = c.company;
        obj["job"]         = c.job;
        obj["email"]       = c.email;
        obj["address"]     = c.address;
        obj["birthday"]    = c.birthday.toString(Qt::ISODate);
        obj["note"]        = c.note;
        obj["group"]       = c.group;
        obj["callCount"]   = c.callCount;
        obj["lastCallTime"]= c.lastCallTime.toString(Qt::ISODateWithMs);
        arr.append(obj);
    }
    return arr;
}

/**
 * @brief 从 JSON 数组反序列化联系人
 */
void ContactManager::fromJson(const QJsonArray &arr)
{
    contacts.clear();
    for (const QJsonValue &v : arr) {
        const QJsonObject obj = v.toObject();
        Contact c;
        c.name        = obj["name"].toString();
        c.phone       = obj["phone"].toString();
        c.company     = obj["company"].toString();
        c.job         = obj["job"].toString();
        c.email       = obj["email"].toString();
        c.address     = obj["address"].toString();
        c.birthday    = QDate::fromString(obj["birthday"].toString(), Qt::ISODate);
        c.note        = obj["note"].toString();
        c.group       = obj["group"].toString();
        c.callCount   = obj["callCount"].toInt();
        c.lastCallTime= QDateTime::fromString(obj["lastCallTime"].toString(),
                                               Qt::ISODateWithMs);
        contacts.append(c);
    }
}

/**
 * @brief 更新指定联系人的通话记录
 * 1. 找到匹配姓名的联系人
 * 2. callCount++ 并设置当前时间为最后通话时间
 * 3. 按 lastCallTime 降序重新排序联系人列表
 */
void ContactManager::updateCallHistory(const QString &name)
{
    for (Contact &c : contacts) {
        if (c.name == name) {
            c.callCount++;
            c.lastCallTime = QDateTime::currentDateTime();
            break;
        }
    }

    // 按最后通话时间降序排序
    std::sort(contacts.begin(), contacts.end(),
              [](const Contact &a, const Contact &b) {
                  return a.lastCallTime > b.lastCallTime;
              });
}

/**
 * @brief 获取最近拨打过的联系人（最多 5 条）
 * 规则：
 * 1. callCount > 0
 * 2. 按 lastCallTime 降序
 * 3. 只返回前 5 条
 */
QVector<Contact> ContactManager::getRecentContacts() const
{
    QVector<Contact> called;
    for (const Contact &c : contacts)
        if (c.callCount > 0)
            called.append(c);

    std::sort(called.begin(), called.end(),
              [](const Contact &a, const Contact &b) {
                  return a.lastCallTime > b.lastCallTime;
              });

    if (called.size() > 5)
        called.resize(5);

    return called;
}
