#include "StudentFilterDialog.h"

#include <QRegExp>

#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QGroupBox>
#include <QStackedLayout>

StudentFilterDialog::StudentFilterDialog(QVector<CONST::HDG> headings, QWidget *parent) :
    QDialog(parent), headingList_(headings)
{
    setWindowTitle(tr("Student Table Filter"));

    QVBoxLayout *layout = new QVBoxLayout();
    this->stackedLayout_ = new QStackedLayout;

    // header
    // Column selector
    this->filterColumn_ = new QComboBox;
    for(CONST::HDG column : this->headingList_){
        this->filterColumn_->addItem(CONST::getHeadingString(column), static_cast<int>(column));
    }
    connect(this->filterColumn_, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged),
            this, &StudentFilterDialog::handleFilterColumnChange);
    this->filterColumn_->setCurrentIndex(0);

    QFormLayout *headerLayout = new QFormLayout;
    headerLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    headerLayout->addRow(tr("Filter by &column:"), this->filterColumn_);
    layout->addLayout(headerLayout);

    // horizonal line
    QFrame *hline = new QFrame;
    hline->setFrameShape(QFrame::HLine);
    hline->setLineWidth(2);
    layout->addWidget(hline);

    // Frame 0: string filter
    QVBoxLayout *stringLayout = new QVBoxLayout;
    this->filterLineEdit_ = new QLineEdit(this);
    connect(this->filterLineEdit_, &QLineEdit::textChanged, this, &StudentFilterDialog::filterTextChanged);
    this->filterLineEdit_->setPlaceholderText(tr("what are you looking for ?"));
    stringLayout->addWidget(this->filterLineEdit_);

    // Frame 0 - options layout
    QGroupBox *filterTextOptionBox = new QGroupBox(tr("Option"));
    QVBoxLayout *filterTextOptionLayout = new QVBoxLayout;

    this->caseSensitivity_ = new QCheckBox(tr("Case &sensitive"));
    connect(this->caseSensitivity_, &QCheckBox::toggled, this, &StudentFilterDialog::caseSensitivityChanged);
    this->caseSensitivity_->setChecked(false);

    this->useRegexp_ = new QCheckBox(tr("&Regexp(PCRE)"));
    connect(this->useRegexp_, &QCheckBox::toggled, this, &StudentFilterDialog::useRegExp);
    this->useRegexp_->setChecked(false);

    filterTextOptionLayout->addWidget(this->caseSensitivity_);
    filterTextOptionLayout->addWidget(this->useRegexp_);
    filterTextOptionBox->setLayout(filterTextOptionLayout);
    stringLayout->addWidget(filterTextOptionBox);

    QWidget *widget = new QWidget;
    widget->setLayout(stringLayout);
    this->stackedLayout_->addWidget(widget);

    // Frame 1: Birthday filter
    QFormLayout *dateRangeLayout = new QFormLayout;
    this->fromDate_ = new QDateEdit();
    this->fromDate_->setDisplayFormat("yyyy-MM-dd");
    this->fromDate_->setCalendarPopup(true);
    connect(this->fromDate_, &QDateEdit::dateChanged, this, &StudentFilterDialog::fromBirthdayChanged);
    dateRangeLayout->addRow(tr("&From: "), this->fromDate_);

    this->toDate_ = new QDateEdit();
    this->toDate_->setDisplayFormat("yyyy-MM-dd");
    this->toDate_->setCalendarPopup(true);
    connect(this->toDate_, &QDateEdit::dateChanged, this, &StudentFilterDialog::toBirthdayChanged);
    dateRangeLayout->addRow(tr("&To: "), this->toDate_);

    // emit initial dateChanged signal
    this->toDate_->setDate(QDate::currentDate());
    this->fromDate_->setDate(this->toDate_->date().addYears(-15));

    // set max date range: 1880.01.01 to Today
    this->fromDate_->setMinimumDate(QDate(1880,1,1));
    this->fromDate_->setMaximumDate(this->toDate_->date());
    this->toDate_->setMinimumDate(this->fromDate_->date());
    this->toDate_->setMaximumDate(QDate::currentDate());

    // confine user always select meaningful date range, i.e. fromdate always <= todate
    connect(this->fromDate_, &QDateEdit::dateChanged, [this](const QDate &date){
        if(date != this->toDate_->minimumDate()){
            this->toDate_->setMinimumDate(date);
        }
    });
    connect(this->toDate_, &QDateEdit::dateChanged, [this](const QDate &date){
        if(date != this->fromDate_->maximumDate()){
            this->fromDate_->setMaximumDate(date);
        }
    });

    widget = new QWidget;
    widget->setLayout(dateRangeLayout);
    this->stackedLayout_->addWidget(widget);

    // Frame 2: Sex filter
    this->sexSelector_ = new QListWidget;
    this->sexSelector_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    QListWidgetItem *item = new QListWidgetItem(Person::getSexString(Person::Sex::Male),
                                                this->sexSelector_, static_cast<u_int8_t>(Person::Sex::Male));
    this->sexSelector_->addItem(item);
    item = new QListWidgetItem(Person::getSexString(Person::Sex::Female),
                               this->sexSelector_, static_cast<u_int8_t>(Person::Sex::Female));
    this->sexSelector_->addItem(item);
    item = new QListWidgetItem(Person::getSexString(Person::Sex::Other),
                               this->sexSelector_, static_cast<u_int8_t>(Person::Sex::Other));
    this->sexSelector_->addItem(item);
    item = new QListWidgetItem(Person::getSexString(Person::Sex::Unspecified),
                               this->sexSelector_, static_cast<u_int8_t>(Person::Sex::Unspecified));
    this->sexSelector_->addItem(item);
    connect(this->sexSelector_, &QListWidget::itemActivated, [this](QListWidgetItem *item){
        emit sexTypeChanged(static_cast<Person::Sex>(item->type()));
    });
    this->sexSelector_->setSelectionMode(QAbstractItemView::SingleSelection);
    this->sexSelector_->setCurrentRow(0);
    QVBoxLayout *sexLayout = new QVBoxLayout;
    sexLayout->addWidget(this->sexSelector_);
    sexLayout->addWidget(new QLabel(tr("<b>Double click to sort</b>")));
    widget = new QWidget;
    widget->setLayout(sexLayout);
    this->stackedLayout_->addWidget(widget);

    layout->addLayout(this->stackedLayout_);

    // button
    QPushButton *okButton = new QPushButton(tr("&Close"));
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &StudentFilterDialog::done);
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    setLayout(layout);
}


void StudentFilterDialog::handleFilterColumnChange(int idx){
    CONST::HDG index = static_cast<CONST::HDG>(this->filterColumn_->itemData(idx).toInt());
    emit filterColumnChanged(index);

    switch (index) {
    case CONST::HDG::ID:
    case CONST::HDG::IDNUMBER:
    case CONST::HDG::CLASSNO:
    case CONST::HDG::DEPT:
    case CONST::HDG::MAJOR:
    case CONST::HDG::NAME:
    case CONST::HDG::POSITION:
    case CONST::HDG::TUTORID:
        this->stackedLayout_->setCurrentIndex(0);
        break;
    case CONST::HDG::BIRTHDAY:
        this->stackedLayout_->setCurrentIndex(1);
        break;
    case CONST::HDG::SEX:
        this->stackedLayout_->setCurrentIndex(2);
        break;
    case CONST::HDG::COUNT:
        break;
    }
}
