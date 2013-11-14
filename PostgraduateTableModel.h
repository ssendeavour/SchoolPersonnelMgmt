#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include <QStringList>

#include "teachingassistant.h"

template <class P>
class StudentTableModel : public QAbstractTableModel
{
//    Q_OBJECT
public:
    explicit StudentTableModel(QVector<CONST::HDG> indexMap, QStringList headerString, QObject *parent = 0);

    // must override
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // optional override
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setDataList(QList<P> list);
    QList<P> getDataList();
    void setHeader(const QVector<CONST::HDG> hdgMap, const QStringList headerString);
    QVector<CONST::HDG> getHeaderIndexs() const;

//signals:

//public slots:

private:
    QList<P> list_ = QList<P>();
    QStringList headerString_ = QStringList();

    // column index map to CONST::HDG enum types
    QVector<CONST::HDG> indexMap_ = QVector<CONST::HDG>(static_cast<int>(CONST::HDG::COUNT));
};
