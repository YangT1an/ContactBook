/****************************************************************************
**
** 分组精确过滤代理模型实现
**
****************************************************************************/

#include "groupfilterproxy.h"

GroupFilterProxy::GroupFilterProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    // 默认不区分大小写
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

/**
 * @brief 设置过滤分组
 */
void GroupFilterProxy::setGroup(const QString &g)
{
    group = g.trimmed();
}

/**
 * @brief 过滤规则：分组列（索引 8）完全匹配
 */
bool GroupFilterProxy::filterAcceptsRow(int source_row,
                                        const QModelIndex &source_parent) const
{
    /* 空分组表示“全部”，直接放行 */
    if (group.isEmpty())
        return true;

    /* 取第 9 列（索引 8）的分组字段并去除两端空格 */
    const QString cell = sourceModel()
                             ->index(source_row, 8, source_parent)
                             .data(Qt::DisplayRole)
                             .toString()
                             .trimmed();

    /* 精确匹配（忽略大小写） */
    return cell.compare(group, Qt::CaseInsensitive) == 0;
}
