#pragma once

#include <QTableView>

#include "studenttablemodel.h"

class StudentTableWidget : public QTableView
{
    Q_OBJECT
public:
    explicit StudentTableWidget(QWidget *parent = 0);
    ~StudentTableWidget();

signals:

public slots:
    bool saveDataToFile();
    bool loadDataFromFile();

private:

};

