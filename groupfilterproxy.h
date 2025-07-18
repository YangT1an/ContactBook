/****************************************************************************
**
** 分组精确过滤代理模型
** 功能：根据“分组”列（第 9 列，索引 8）对联系人进行精确匹配过滤
**
****************************************************************************/

#pragma once
#include <QSortFilterProxyModel>

/**
 * @class GroupFilterProxy
 * @brief 继承自 QSortFilterProxyModel，实现按分组精确过滤
 */
class GroupFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit GroupFilterProxy(QObject *parent = nullptr);

    /**
     * @brief 设置要过滤的分组名称
     * @param g 分组字符串（前后空格会被自动去除）
     */
    void setGroup(const QString &g);

protected:
    /**
     * @brief 重写过滤规则：分组列完全匹配则保留
     * @param source_row    源模型行号
     * @param source_parent 源模型父索引
     * @return true  保留该行；false 过滤掉
     */
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;

private:
    QString group; /**< 当前过滤分组（空字符串表示不过滤） */
};
