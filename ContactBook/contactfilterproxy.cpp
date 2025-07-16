/****************************************************************************
**
** 联系人“姓名/电话”模糊搜索代理模型实现
**
****************************************************************************/

#include "contactfilterproxy.h"
#include <QSortFilterProxyModel>

ContactFilterProxy::ContactFilterProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    // 默认不区分大小写
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

bool ContactFilterProxy::filterAcceptsRow(int source_row,
                                          const QModelIndex &source_parent) const
{
    /* 当前过滤关键字为空时全部放行 */
    const QString pattern = filterRegularExpression().pattern();
    if (pattern.isEmpty())
        return true;

    /* 获取“姓名”和“电话”两列数据 */
    const QModelIndex nameIdx  = sourceModel()->index(source_row, 0, source_parent);
    const QModelIndex phoneIdx = sourceModel()->index(source_row, 1, source_parent);

    const QString name  = nameIdx.data().toString();
    const QString phone = phoneIdx.data().toString();

    /* 任意一列包含关键字即保留 */
    return name.contains(pattern, Qt::CaseInsensitive) ||
           phone.contains(pattern, Qt::CaseInsensitive);
}
