#include "studenttablewidget.h"

#include <QDebug>
#include <QHeaderView>
#include <QDateEdit>
#include <QModelIndex>

#include "dateeditdelegate.h"

StudentTableWidget::StudentTableWidget(QWidget *parent) :
    QTableView(parent)
{
    StudentTableModel *studentModel = new StudentTableModel(this);
    this->setModel(studentModel);
    this->resizeColumnsToContents();
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);

    this->setItemDelegate(new DateEditDelegate(this));
}

StudentTableWidget::~StudentTableWidget()
{
}

bool StudentTableWidget::saveDataToFile(){
    return true;
}

bool StudentTableWidget::loadDataFromFile(){
    return true;
}
