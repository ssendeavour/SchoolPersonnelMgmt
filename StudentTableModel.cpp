#include "StudentTableModel.h"

#include <QSize>

StudentTableModel::StudentTableModel(QVector<CONST::HDG> indexMap, QStringList headerString, QObject *parent /*= 0*/) :
    QAbstractTableModel(parent)
{
    setHeader(indexMap, headerString);
}

int StudentTableModel::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->list_.size();
}

int StudentTableModel::columnCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->headerString_.size();
}

QVariant StudentTableModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const{
    //* NOTE: if used across days, Today string will be inaccurate.
    static QString birtyTooltip = "Valid date range: 1880-01-01 to Today(" + QDate::currentDate().toString("yyyy-MM-dd") + ")";
    if(!index.isValid()){
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole){
        return static_cast<int>(Qt::AlignLeft|Qt::AlignVCenter);
    } else if ((role == Qt::DisplayRole|| role == Qt::EditRole) && !this->list_.isEmpty()){
        const Student &t = list_.at(index.row());
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
        default:
            return QVariant();
        }
    } else if (role == Qt::ToolTipRole) {
        switch(this->indexMap_.at(index.column())){
        case CONST::HDG::BIRTHDAY:
            return birtyTooltip;
        case CONST::HDG::IDNUMBER:
            return "18 digits or 17 digits with letter X";
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool StudentTableModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/){
    if(index.isValid() && role == Qt::EditRole){
        bool result = false;
        Student &t = this->list_[index.row()];
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

bool StudentTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    // note: [row, row+count-1], not [row, row+count)
    beginInsertRows(parent, row, row + count - 1);
    for(int i = row; i < row + count; ++i){
        this->list_.insert(i, Student());
    }
    endInsertRows();
    return true;
}

bool StudentTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    // note [row, row+count-1], not [row, row+count)
    beginRemoveRows(parent, row, row + count -1);
    for(int i = row + count -1; i >= row; i--){
        this->list_.removeAt(i);
    }
    endRemoveRows();
    return true;
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

void StudentTableModel::setDataList(QList<Student> list){
    beginResetModel();
    this->list_ = list;
    endResetModel();
}

QList<Student> StudentTableModel::getDataList(){
    return this->list_;
}

void StudentTableModel::setHeader(const QVector<CONST::HDG> hdgMap, const QStringList headerString){
    if(this->indexMap_.size() == hdgMap.size()){
        this->indexMap_ = hdgMap;
        this->headerString_ = headerString;
    } else {
        qDebug() << "Warning: hdgMap size mismatch, size should be static_cast<int>(CONST::HDG::COUNT), header not set";
    }
}

QVector<CONST::HDG> StudentTableModel::getHeaderIndexs() const{
    return this->indexMap_;
}

void StudentTableModel::sort(int column, Qt::SortOrder order){
    qDebug() << column << ", order: " << order;
}
