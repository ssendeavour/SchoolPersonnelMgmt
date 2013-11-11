#include "studenttablewidget.h"

#include <QDebug>
#include <QHeaderView>
#include <QDateEdit>
#include <QModelIndex>

#include "StudentTableDelegate.h"

StudentTableWidget::StudentTableWidget(QWidget *parent) :
    QTableView(parent)
{
    studentModel = new StudentTableModel(this);
    this->setModel(studentModel);
    this->resizeColumnsToContents();
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);

    this->setItemDelegate(new StudentTableDelegate(this));
}

StudentTableWidget::~StudentTableWidget()
{
}

bool StudentTableWidget::saveDataToFile(QFile &file){
    auto list = studentModel->getStudentList();
    return Student::writeToFile(file, list);
}

bool StudentTableWidget::openFromFile(QFile &file){
    bool succeed;
    auto list = Student::readFromFile(file, succeed);
    if(!succeed){
        return false;
    }
    studentModel->setStudentList(list);
    resizeColumnsToContents();
    return true;
}
