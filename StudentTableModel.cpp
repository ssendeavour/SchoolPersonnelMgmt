#include "StudentTableModel.h"

#if QT_VERSION >= 0x050200
#include <string>
#endif

StudentTableModel::StudentTableModel(QVector<CONST::HDG> indexMap,  QObject *parent /*= 0*/) :
    QAbstractTableModel(parent), indexMap_(indexMap) {

}

StudentTableModel::~StudentTableModel(){

}

int StudentTableModel::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->list_.size();
}

int StudentTableModel::columnCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->indexMap_.size();
}

QVariant StudentTableModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const{
    //* NOTE: if used across days, Today string will be inaccurate.
    static QString birthTooltip = "Valid date range: 1880-01-01 to Today(" + QDate::currentDate().toString("yyyy-MM-dd") + ")";
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
            return static_cast<quint8>(t.getSex());
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
            return birthTooltip;
        case CONST::HDG::IDNUMBER:
            return "18 digits or 17 digits and letter X";
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
        default:
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
    // NOTE: [row, row+count-1], not [row, row+count)
    beginInsertRows(parent, row, row + count - 1);
    for(int i = row; i < row + count; ++i){
        this->list_.insert(i, Student());
    }
    endInsertRows();
    return true;
}

// remove from high index to low index
bool StudentTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    // NOTE: [row, row+count-1], not [row, row+count)
    beginRemoveRows(parent, row, row + count -1);
    for(int i = row + count - 1; i >= row; i--){
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
        return CONST::getHeadingString(this->indexMap_.at(section));
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

QVector<CONST::HDG> StudentTableModel::getHeaderIndexs() const{
    return this->indexMap_;
}

void StudentTableModel::sort(int column, Qt::SortOrder order){
    // std::function is about two times slower than lambda expression
//    std::function<bool(Student, Student)> func;
    beginResetModel();
    switch(this->indexMap_[column]){
    case CONST::HDG::ID:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getId() < b.getId(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getId() >= b.getId(); });
        }
        break;
    case CONST::HDG::NAME:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [this](Student a, Student b) -> bool {
#if QT_VERSION >= 0x050200
                return this->zh_CN_collate_.compare(a.getName(), b.getName()) < 0;
#else
                std::string sa = a.getName().toStdString();
                std::string sb = b.getName().toStdString();
                const char *pa = sa.data();
                const char *pb = sb.data();
                return this->zh_CN_collate_.compare(pa, pa + sa.size(), pb, pb + sb.size()) < 0;
#endif
            });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [this](Student a, Student b) -> bool {
#if QT_VERSION >= 0x050200
                return this->zh_CN_collate_.compare(a.getName(), b.getName()) >= 0;
#else
                std::string sa = a.getName().toStdString();
                std::string sb = b.getName().toStdString();
                const char *pa = sa.data();
                const char *pb = sb.data();
                return this->zh_CN_collate_.compare(pa, pa + sa.size(), pb, pb + sb.size()) >= 0;
#endif
            });
        }
        break;
    case CONST::HDG::SEX:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getSex() < b.getSex(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getSex() >= b.getSex(); });
        }
        break;
    case CONST::HDG::BIRTHDAY:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getBirthday() < b.getBirthday(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getBirthday() >= b.getBirthday(); });
        }
        break;
    case CONST::HDG::IDNUMBER:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getIdNumber() < b.getIdNumber(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getIdNumber() >= b.getIdNumber(); });
        }
        break;
    case CONST::HDG::CLASSNO:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getClassNo() < b.getClassNo(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Student a, Student b) -> bool {return a.getClassNo() >= b.getClassNo(); });
        }
        break;
    default:
        break;
    }
    endResetModel();
}

void StudentTableModel::filter(){
    if(! this->filterEnabled_)
        return;
    this->isFiltering_ = true;

    QList<Student> newList;
    switch (this->filterColumn_) {
    case CONST::HDG::ID: {
        if (this->useRegexp_) {
            for (Student &stu : this->listOrig_){
                if(stu.getId().contains(this->filterRegexp_)){
                    newList.append(stu);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Student &stu : this->listOrig_){
                if(stu.getId().contains(this->filterString_, sensitivity)) {
                    newList.append(stu);
                }
            }
        }
    }
        break;
    case CONST::HDG::NAME: {
        if (this->useRegexp_) {
            for (Student &stu : this->listOrig_){
                if(stu.getName().contains(this->filterRegexp_)){
                    newList.append(stu);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Student &stu : this->listOrig_){
                if(stu.getName().contains(this->filterString_, sensitivity)) {
                    newList.append(stu);
                }
            }
        }
    }
        break;
    case CONST::HDG::SEX: {
        for (Student &stu : this->listOrig_){
            if(stu.getSex() == this->filterSex_){
                newList.append(stu);
            }
        }
    }
        break;
    case CONST::HDG::BIRTHDAY: {
        for (Student &stu : this->listOrig_){
            if(this->minDate_ <= stu.getBirthday() && stu.getBirthday() <= this->maxDate_){
                newList.append(stu);
            }
        }
    }
        break;
    case CONST::HDG::CLASSNO: {
        if (this->useRegexp_) {
            for (Student &stu : this->listOrig_){
                if(stu.getClassNo().contains(this->filterRegexp_)){
                    newList.append(stu);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Student &stu : this->listOrig_){
                if(stu.getClassNo().contains(this->filterString_, sensitivity)) {
                    newList.append(stu);
                }
            }
        }
    }
        break;
    case CONST::HDG::IDNUMBER: {
        if (this->useRegexp_) {
            for (Student &stu : this->listOrig_){
                if(stu.getIdNumber().contains(this->filterRegexp_)){
                    newList.append(stu);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Student &stu : this->listOrig_){
                if(stu.getIdNumber().contains(this->filterString_, sensitivity)) {
                    newList.append(stu);
                }
            }
        }
    }
        break;
    default:
        return;
    }

    beginResetModel();
    this->list_ = newList;
    endResetModel();
    this->isFiltering_ = false;
}

void StudentTableModel::setRegexp(QString pattern){
    this->filterRegexp_.setPattern(pattern);
    if(this->caseSensitive_){
        this->filterRegexp_.setPatternOptions(QRegularExpression::DontCaptureOption);
    } else {
        this->filterRegexp_.setPatternOptions(QRegularExpression::DontCaptureOption|QRegularExpression::CaseInsensitiveOption);
    }
}
