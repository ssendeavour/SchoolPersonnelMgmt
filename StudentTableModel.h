#pragma once

/* QT Q_OBJECTS doesn't support templated classes, so convert back to StudentTabloeModel */

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include <QStringList>

#include "teachingassistant.h"


class StudentTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StudentTableModel(QVector<CONST::HDG> indexMap, QObject *parent = 0);
    ~StudentTableModel();

    // SortProxyModel using this model as source model should override these same functions
    // must override
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // optional override
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order);

    void setDataList(QList<Student> list);
    QList<Student> getDataList();
    QVector<CONST::HDG> getHeaderIndexs() const;

signals:

public slots:

private:
    QList<Student> list_ = QList<Student>();

    // column index map to CONST::HDG enum types
    QVector<CONST::HDG> indexMap_ = QVector<CONST::HDG>(static_cast<int>(CONST::HDG::COUNT));
};
