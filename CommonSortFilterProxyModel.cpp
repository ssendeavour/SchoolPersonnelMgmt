#include "CommonSortFilterProxyModel.h"

#include <QDate>
#include <QString>

CommonSortFilterProxyModel::CommonSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool CommonSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    if (leftData.type() == QVariant::Date) {
        return leftData.toDate() < rightData.toDate();
    } else {
        return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
    }
}
