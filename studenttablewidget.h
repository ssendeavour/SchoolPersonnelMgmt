#pragma once

#include <QTableWidget>

#include "student.h"


class StudentTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit StudentTableWidget(QWidget *parent = 0);
    ~StudentTableWidget();

signals:

public slots:

private:
    StudentTableWidget *table_;
};

