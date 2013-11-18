#include "StudentFilterDialog.h"

StudentFilterDialog::StudentFilterDialog(QVector<CONST::HDG> headings, QWidget *parent) :
    QDialog(parent), headingList_(headings)
{
    setWindowTitle(tr("Student Table Filter"));
    this->filterLineEdit_ = new QLineEdit(this);
    this->filterType_ = new QComboBox(this);
    this->filterColumn_ = new QComboBox(this);
    QVBoxLayout *filterLayout = new QVBoxLayout();
    filterLayout->addWidget(filterColumn_);
    filterLayout->addWidget(filterType_);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(filterLineEdit_);
    layout->addLayout(filterLayout);
    setLayout(layout);
}
