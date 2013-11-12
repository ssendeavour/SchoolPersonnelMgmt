#include "studenttablemodel.h"
#include <QSize>

StudentTableModel::StudentTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    Student stu("Class1", "Bear", "001", "23342535234131321X", Student::Sex::Female, QDate(1995, 12, 10));
    this->list_.append(stu);

    if(!stu.setName("yang")){
        qDebug() << stu.errorString();
    }
    if(!stu.setBirthday(QDate(2008,12, 31))){
        qDebug() << stu.errorString();
    }
    this->list_.append(stu);

    if(!stu.setName("Hu JIn Tao")){
        qDebug() << stu.errorString();
    }
    this->list_.append(stu);

    if(!stu.setBirthday(QDate(2999, 12, 22))){
        qDebug() << stu.errorString();
    }
    this->list_.append(stu);

    Student stu2("Class2", "Bear simple", "002", "23342535234131321f", Student::Sex::Male, QDate(1998, 12, 32));
    this->list_.append(stu2);

    if(!stu2.setBirthday(QDate::currentDate())){
        qDebug() << stu2.errorString();
    }
    this->list_.append(stu2);

    stu = Student();
    this->list_.append(stu);
    list_ += list_;
    list_ += list_;
    /*
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    list_ += list_;
    */
}

int StudentTableModel::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->list_.size();
}

int StudentTableModel::columnCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return headerString_.size();
}

QVariant StudentTableModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const{
    if(!index.isValid()){
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole){
        return static_cast<int>(Qt::AlignLeft|Qt::AlignVCenter);
    } else if ((role == Qt::DisplayRole|| role == Qt::EditRole) && !this->list_.isEmpty()){
        const Student &stu = list_.at(index.row());
        switch(static_cast<StudentTableModel::heading>(index.column())){
        case heading::ID:
            return stu.getId();
        case heading::NAME:
            return stu.getName();
        case heading::SEX:
            if(role == Qt::DisplayRole){
                return Student::getSexString(stu.getSex());
            }
            return static_cast<u_int8_t>(stu.getSex());
        case heading::BIRTHDAY:
            if(role == Qt::DisplayRole){
                return stu.getBirthday().toString("yyyy-MM-dd");
            }
            return stu.getBirthday();
        case heading::CLASSNO:
            return stu.getClassNo();
        case heading::IDNUMBER:
            return stu.getIdNumber();
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool StudentTableModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/){
    if(index.isValid() && role == Qt::EditRole){
        bool result = false;
        Student &stu = this->list_[index.row()];
        switch(static_cast<StudentTableModel::heading>(index.column())){
        case heading::ID:
            result = stu.setId(value.toString());
            break;
        case heading::NAME:
            result = stu.setName(value.toString());
            break;
        case heading::SEX:
            stu.setSex(static_cast<Student::Sex>(value.toUInt()));
            result = true;
            break;
        case heading::BIRTHDAY:
            result = stu.setBirthday(value.toDate());
            break;
        case heading::CLASSNO:
            result = stu.setClassNo(value.toString());
            break;
        case heading::IDNUMBER:
            result = stu.setIdNumber(value.toString());
            break;
        }
        if(result){
            emit dataChanged(index, index);
        }
        return result;
    }
    return QAbstractTableModel::setData(index, value, role);
}

Qt::ItemFlags StudentTableModel::flags(const QModelIndex & index) const{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}
bool StudentTableModel::insertRows(int row, int count, const QModelIndex &parent){

}

bool StudentTableModel::removeRows(int row, int count, const QModelIndex &parent){

}

bool StudentTableModel::insertColumns(int column, int count, const QModelIndex &parent){

}

bool StudentTableModel::removeColumns(int column, int count, const QModelIndex &parent){

}

QVariant StudentTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(orientation == Qt::Vertical){
        return section;
    } else {
        return this->headerString_.at(section);
    }
}

void StudentTableModel::setStudentList(QList<Student> list){
    beginResetModel();
    this->list_ = list;
    endResetModel();
}

QList<Student> StudentTableModel::getStudentList(){
    return this->list_;
}
