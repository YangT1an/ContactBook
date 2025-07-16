/****************************************************************************
**
** 联系人数据结构
** 功能：描述通讯录中一条联系人记录的所有字段及辅助方法
**
****************************************************************************/

#pragma once
#include <QString>
#include <QDate>
#include <QDateTime>

/**
 * @struct Contact
 * @brief  通讯录联系人信息结构体
 *
 * 包含姓名、电话、公司、职位、邮箱、地址、生日、备注、分组
 * 以及通话统计相关字段：callCount（累计拨打次数）、lastCallTime（最后通话时间）
 */
struct Contact {
    QString     name;          /**< 姓名               */
    QString     phone;         /**< 电话号码           */
    QString     company;       /**< 公司/组织          */
    QString     job;           /**< 职位/职务          */
    QString     email;         /**< 电子邮箱           */
    QString     address;       /**< 地址               */
    QDate       birthday;      /**< 生日               */
    QString     note;          /**< 备注               */
    QString     group;         /**< 所属分组           */
    int         callCount = 0; /**< 累计拨打次数       */
    QDateTime   lastCallTime;  /**< 最后通话时间       */

    /**
     * @brief 判断联系人是否为空
     * @return true  姓名和电话均为空
     * @return false 至少一项不为空
     */
    bool isEmpty() const;
};
