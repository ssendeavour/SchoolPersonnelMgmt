#pragma once

#include <QTableWidget>

#include "teacher.h"

class TeacherTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TeacherTableWidget(QWidget *parent = 0);
    ~TeacherTableWidget();

signals:

public slots:

private:
    TeacherTableWidget *table_;
};

