#include "dateeditdelegate.h"

#include <QApplication>
#include <QDateEdit>
#include <QComboBox>

#include "studenttablemodel.h"

DateEditDelegate::DateEditDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *DateEditDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    switch(static_cast<StudentTableModel::heading>(index.column())){
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
    default:
        return QStyledItemDelegate::createEditor(parent,option, index);
    }
}

void DateEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
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
void DateEditDelegate::setModelData(
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

void DateEditDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
