#pragma once

#include <QDialog>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class StudentFilterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StudentFilterDialog(QWidget *parent = 0);

signals:

public slots:

private:
    QLineEdit *filterLineEdit;
    QComboBox *filterType;

};
