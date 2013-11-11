#include "StudentTableDelegate.h"

#include <QApplication>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QRegExp>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "studenttablemodel.h"

StudentTableDelegate::StudentTableDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *StudentTableDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    switch(static_cast<StudentTableModel::heading>(index.column())){
    case StudentTableModel::heading::ID : {
        QLineEdit * idEditor = new QLineEdit(parent);
        idEditor->setMaxLength(Person::PERSON_ID_LEN);
        return idEditor;
    }
    case StudentTableModel::heading::NAME : {
        QLineEdit * nameEditor = new QLineEdit(parent);
        nameEditor->setMaxLength(Person::PERSON_NAME_LEN);
        return nameEditor;
    }
    case StudentTableModel::heading::SEX : {
        QComboBox *sexEditor = new QComboBox(parent);
        sexEditor->addItem(Student::getSexString(Student::Sex::Male),
                           static_cast<u_int8_t>(Student::Sex::Male));
        sexEditor->addItem(Student::getSexString(Student::Sex::Female),
                           static_cast<u_int8_t>(Student::Sex::Female));
        sexEditor->addItem(Student::getSexString(Student::Sex::Unspecified),
                           static_cast<u_int8_t>(Student::Sex::Unspecified));
        sexEditor->addItem(Student::getSexString(Student::Sex::Other),
                           static_cast<u_int8_t>(Student::Sex::Other));
        return sexEditor;
    }
    case StudentTableModel::heading::BIRTHDAY : {
        QDateEdit *birthdayEditor = new QDateEdit(parent);
        birthdayEditor->setDateRange(QDate(1880, 1, 1), QDate::currentDate());
        birthdayEditor->setDisplayFormat("yyyy-MM-dd");
        birthdayEditor->setCalendarPopup(true);
        birthdayEditor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        return birthdayEditor;
    }
    case StudentTableModel::heading::CLASSNO : {
        QLineEdit * idEditor = new QLineEdit(parent);
        idEditor->setMaxLength(Student::STUDENT_CLASSNO_LEN);
        return idEditor;
    }
    case StudentTableModel::heading::IDNUMBER : {
        QLineEdit * idEditor = new QLineEdit(parent);
        idEditor->setMaxLength(Person::PERSON_ID_NUM_LEN);
//        idEditor->setValidator(Person::obtainIDNumberValidator());
        idEditor->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{17}[0-9xX]"), parent));
//        idEditor->setInputMask("999999-99999999-999>H;_");
        return idEditor;
    }
    default:
        return QStyledItemDelegate::createEditor(parent,option, index);
    }
}

void StudentTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    switch(static_cast<StudentTableModel::heading>(index.column())){
    case StudentTableModel::heading::SEX : {
        QComboBox *sexEditor = qobject_cast<QComboBox*>(editor);
        int idx = sexEditor->findData(index.model()->data(index, Qt::EditRole).toUInt());
        qDebug() << "found index :" << idx;
        sexEditor->setCurrentIndex(idx);
        break;
    }

    case StudentTableModel::heading::BIRTHDAY: {
        QDateEdit *birthdayEditor = qobject_cast<QDateEdit*>(editor);
        if(birthdayEditor){
            birthdayEditor->setDate(index.model()->data(index, Qt::EditRole).toDate());
        } else {
            birthdayEditor->setDate(QDate());
        }
        break;
    }

    default:
        return QStyledItemDelegate::setEditorData(editor, index);
    }
}
void StudentTableDelegate::setModelData(
        QWidget *editor,
        QAbstractItemModel *model,
        const QModelIndex &index) const
{
    switch(static_cast<StudentTableModel::heading>(index.column())){
    case StudentTableModel::heading::SEX : {
        QComboBox *sexEditor = qobject_cast<QComboBox*>(editor);
        model->setData(index, sexEditor->itemData(sexEditor->currentIndex(), Qt::UserRole));
        break;
    }
    case StudentTableModel::heading::BIRTHDAY : {
        QDateEdit *birthdayEditor = qobject_cast<QDateEdit*>(editor);
        model->setData(index, birthdayEditor->date());
        break;
    }
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void StudentTableDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
