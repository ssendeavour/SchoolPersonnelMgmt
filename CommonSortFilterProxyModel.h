#pragma once

#include <QSortFilterProxyModel>

class CommonSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommonSortFilterProxyModel(QObject *parent = 0);

public:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

signals:

public slots:

};
