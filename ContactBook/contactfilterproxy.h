/****************************************************************************
**
** 联系人“姓名/电话”模糊搜索代理模型
** 功能：在源模型中按姓名或电话（不区分大小写）进行模糊过滤
**
****************************************************************************/

#pragma once
#include <QSortFilterProxyModel>
#include <QRegularExpression>

/**
 * @class ContactFilterProxy
 * @brief 继承自 QSortFilterProxyModel，实现按姓名或电话模糊过滤
 */
class ContactFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit ContactFilterProxy(QObject *parent = nullptr);

protected:
    /**
     * @brief 重写过滤规则：只要“姓名”或“电话”列包含关键字即接受该行
     * @param source_row     源模型中的行号
     * @param source_parent  源模型中的父索引
     * @return true  保留该行；false 过滤掉该行
     */
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;
};
