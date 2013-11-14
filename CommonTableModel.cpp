#include "CommonTableModel.h"

#include <QSize>

template <class P>
CommonTableModel<P>::CommonTableModel(QVector<CONST::HDG> indexMap, QStringList headerString, QObject *parent /*= 0*/) :
    QAbstractTableModel(parent)
{
    setHeader(indexMap, headerString);

    // test data
    this->list_ = QList<Student>();
    Student t("Class1", "Bear", "001", "23342535234131321X", Student::Sex::Female, QDate(1995, 12, 10));
    this->list_.append(t);

    if(!t.setName("yang")){
        qDebug() << t.errorString();
    }
    if(!t.setBirthday(QDate(2008,12, 31))){
        qDebug() << t.errorString();
    }
    this->list_.append(t);

    if(!t.setName("Hu JIn Tao")){
        qDebug() << t.errorString();
    }
    this->list_.append(t);

    if(!t.setBirthday(QDate(2999, 12, 22))){
        qDebug() << t.errorString();
    }
    this->list_.append(t);

    Student stu2("Class2", "Bear simple", "002", "23342535234131321f", Student::Sex::Male, QDate(1998, 12, 32));
    this->list_.append(stu2);

    if(!stu2.setBirthday(QDate::currentDate())){
        qDebug() << stu2.errorString();
    }
    this->list_.append(stu2);

    Student stu = Student();
    this->list_.append(stu);
    list_ += list_;
    list_ += list_;
}

template <class P>
int CommonTableModel<P>::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->list_.size();
}

template <class P>
int CommonTableModel<P>::columnCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return headerString_.size();
}

template <class P>
QVariant CommonTableModel<P>::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const{
    if(!index.isValid()){
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole){
        return static_cast<int>(Qt::AlignLeft|Qt::AlignVCenter);
    } else if ((role == Qt::DisplayRole|| role == Qt::EditRole) && !this->list_.isEmpty()){
        const P &t = list_.at(index.row());
        switch(this->indexMap_.at(index.column())){
        case CONST::HDG::ID:
            return t.getId();
        case CONST::HDG::NAME:
            return t.getName();
        case CONST::HDG::SEX:
            if(role == Qt::DisplayRole){
                return Person::getSexString(t.getSex());
            }
            return static_cast<u_int8_t>(t.getSex());
        case CONST::HDG::BIRTHDAY:
            if(role == Qt::DisplayRole){
                return t.getBirthday().toString("yyyy-MM-dd");
            }
            return t.getBirthday();
        case CONST::HDG::CLASSNO:
            return t.getClassNo();
        case CONST::HDG::IDNUMBER:
            return t.getIdNumber();
        case CONST::HDG::MAJOR:
            return t.getMajor();
        case CONST::HDG::TUTORID:
            return t.getTutorId();
        case CONST::HDG::DEPT:
            return t.getDepartment();
        case CONST::HDG::POSITION:
            return t.getPosition();
        default:
            return QVariant();
        }
    }
    return QVariant();
}

template <class P>
bool CommonTableModel<P>::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/){
    if(index.isValid() && role == Qt::EditRole){
        bool result = false;
//        T &t = this->list_.at(index.row());
        TeachingAssistant t;
        switch(this->indexMap_.at(index.column())){
        case CONST::HDG::ID:
            result = t.setId(value.toString());
            break;
        case CONST::HDG::NAME:
            result = t.setName(value.toString());
            break;
        case CONST::HDG::SEX:
            t.setSex(static_cast<Person::Sex>(value.toUInt()));
            result = true;
            break;
        case CONST::HDG::BIRTHDAY:
            result = t.setBirthday(value.toDate());
            break;
        case CONST::HDG::CLASSNO:
            result = t.setClassNo(value.toString());
            break;
        case CONST::HDG::IDNUMBER:
            result = t.setIdNumber(value.toString());
            break;
        case CONST::HDG::MAJOR:
            result = t.setMajor(value.toString());
            break;
        case CONST::HDG::TUTORID:
            result = t.setTutorId(value.toString());
            break;
        case CONST::HDG::DEPT:
            result = t.setDepartment(value.toString());
            break;
        case CONST::HDG::POSITION:
            result = t.setPosition(value.toString());
            break;
        default:
            ;
        }
        if(result){
            emit dataChanged(index, index);
        }
        return result;
    }
    return QAbstractTableModel::setData(index, value, role);
}

template <class P>
Qt::ItemFlags CommonTableModel<P>::flags(const QModelIndex & index) const{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

template <class P>
bool CommonTableModel<P>::insertRows(int row, int count, const QModelIndex &parent){
    return true;
}

template <class P>
bool CommonTableModel<P>::removeRows(int row, int count, const QModelIndex &parent){
    return true;
}

template <class P>
bool CommonTableModel<P>::insertColumns(int column, int count, const QModelIndex &parent){
    return true;
}

template <class P>
bool CommonTableModel<P>::removeColumns(int column, int count, const QModelIndex &parent){
    return true;
}

template <class P>
QVariant CommonTableModel<P>::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(orientation == Qt::Vertical){
        return section;
    } else {
        return this->headerString_.at(section);
    }
}

template <class P>
void CommonTableModel<P>::setDataList(QList<P> list){
    beginResetModel();
    this->list_ = list;
    endResetModel();
}

template <class P>
QList<P> CommonTableModel<P>::getDataList(){
    return this->list_;
}

template <class P>
void CommonTableModel<P>::setHeader(const QVector<CONST::HDG> hdgMap, const QStringList headerString){
    if(this->indexMap_.size() == hdgMap.size()){
        this->indexMap_ = hdgMap;
        this->headerString_ = headerString;
    } else {
        qDebug() << "Warning: hdgMap size mismatch, size should be static_cast<int>(CONST::HDG::COUNT), header not set";
    }
}

template <class P>
QVector<CONST::HDG> CommonTableModel<P>::getHeaderIndexs() const{
    return this->indexMap_;
}
