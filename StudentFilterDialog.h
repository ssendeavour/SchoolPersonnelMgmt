#pragma once

#include <QDialog>
#include <QVector>
#include <QDate>

#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QStackedLayout>
#include <QListWidget>
#include <QDateEdit>


#include "const.h"
#include "person.h"

class StudentFilterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StudentFilterDialog(QVector<CONST::HDG> headings, QWidget *parent = 0);

signals:
    void filterColumnChanged(const CONST::HDG &column);
    void filterTextChanged(const QString &newText);
    void fromBirthdayChanged(const QDate &newDate);
    void toBirthdayChanged(const QDate &newDate);
    void sexTypeChanged(const Person::Sex newSex);
    void caseSensitivityChanged(bool sensitive);
    void useRegExp(bool yes);

public slots:

protected slots:
    void handleFilterColumnChange(int idx);

private:
    QLineEdit *filterLineEdit_;
    QComboBox *filterColumn_;
    QVector<CONST::HDG> headingList_;
    QCheckBox *caseSensitivity_;
    QCheckBox *useRegexp_;
    QStackedLayout *stackedLayout_;
    QListWidget *sexSelector_;
    QDateEdit *fromDate_;
    QDateEdit *toDate_;
};
