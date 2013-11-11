#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>

#include "student.h"

class StudentTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StudentTableModel(QObject *parent = 0);

    enum class heading : quint8 {
        ID = 0,
        NAME = 1,
        SEX = 2,
        BIRTHDAY = 3,
        CLASSNO = 4,
        IDNUMBER = 5
    };

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

    void setStudentList(QList<Student> list);
    QList<Student> getStudentList();

signals:

public slots:

protected:
    QStringList headerString_ = {tr("ID"), tr("Name"), tr("Sex"), tr("Birthday"), tr("Class No."), tr("ID number") };

private:
    QList<Student> list_;
};
