#include "studenttablewidget.h"

#include <QDebug>

StudentTableWidget::StudentTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    static const QStringList headers{"ID", "Name", "ClassNo", "IDNo", "Sex", "Birthday"};
    setColumnCount(headers.length());
    setHorizontalHeaderLabels(headers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setWordWrap(true);

    setRowCount(10);
    for(int i=0; i<rowCount(); i++){
        for(int j=0; j<columnCount(); j++){
            QTableWidgetItem *item = new QTableWidgetItem(tr("(%1,%2)").arg(i).arg(j));
            setItem(i,j,item);
        }
    }

    resizeColumnsToContents();
}

StudentTableWidget::~StudentTableWidget()
{
}
