#pragma once

#include <QDialog>
#include <QVector>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include "const.h"

class StudentFilterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StudentFilterDialog(QVector<CONST::HDG> headings, QWidget *parent = 0);

signals:

public slots:

private:
    QLineEdit *filterLineEdit_;
    QComboBox *filterType_;
    QComboBox *filterColumn_;
    QVector<CONST::HDG> headingList_;
};
