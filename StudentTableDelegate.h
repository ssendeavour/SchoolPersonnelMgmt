#pragma once

#include <QStyledItemDelegate>
#include <QVector>

#include "const.h"

class StudentTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StudentTableDelegate(QVector<CONST::HDG> indexMap, QObject *parent = 0);

signals:

public:

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QVector<CONST::HDG> indexMap_;
};
