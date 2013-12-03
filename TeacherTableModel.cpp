#include "TeacherTableModel.h"

#if QT_VERSION >= 0x050200
#include <string>
#endif

TeacherTableModel::TeacherTableModel(QVector<CONST::HDG> indexMap,  QObject *parent /*= 0*/) :
    QAbstractTableModel(parent), indexMap_(indexMap) {
}

TeacherTableModel::~TeacherTableModel(){

}

int TeacherTableModel::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->list_.size();
}

int TeacherTableModel::columnCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->indexMap_.size();
}

QVariant TeacherTableModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const{
    //* NOTE: if used across days, Today string will be inaccurate.
    static QString birthTooltip = "Valid date range: 1880-01-01 to Today(" + QDate::currentDate().toString("yyyy-MM-dd") + ")";
    if(!index.isValid()){
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole){
        return static_cast<int>(Qt::AlignLeft|Qt::AlignVCenter);
    } else if ((role == Qt::DisplayRole|| role == Qt::EditRole) && !this->list_.isEmpty()){
        const Teacher &t = list_.at(index.row());
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
        case CONST::HDG::IDNUMBER:
            return t.getIdNumber();
        case CONST::HDG::POSITION:
            return t.getPosition();
        case CONST::HDG::DEPT:
            return t.getDepartment();
        default:
            return QVariant();
        }
    } else if (role == Qt::ToolTipRole) {
        switch(this->indexMap_.at(index.column())){
        case CONST::HDG::BIRTHDAY:
            return birthTooltip;
        case CONST::HDG::IDNUMBER:
            return "18 digits or 17 digits with letter X";
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool TeacherTableModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/){
    if(index.isValid() && role == Qt::EditRole){
        bool result = false;
        Teacher &t = this->list_[index.row()];
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
        case CONST::HDG::IDNUMBER:
            result = t.setIdNumber(value.toString());
            break;
        case CONST::HDG::POSITION:
            result = t.setPosition(value.toString());
            break;
        case CONST::HDG::DEPT:
            result = t.setDepartment(value.toString());
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

Qt::ItemFlags TeacherTableModel::flags(const QModelIndex & index) const{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

bool TeacherTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    // NOTE: [row, row+count-1], not [row, row+count)
    beginInsertRows(parent, row, row + count - 1);
    for(int i = row; i < row + count; ++i){
        this->list_.insert(i, Teacher());
    }
    endInsertRows();
    return true;
}

// remove from high index to low index
bool TeacherTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    // NOTE: [row, row+count-1], not [row, row+count)
    beginRemoveRows(parent, row, row + count -1);
    for(int i = row + count - 1; i >= row; i--){
        this->list_.removeAt(i);
    }
    endRemoveRows();
    return true;
}

QVariant TeacherTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(orientation == Qt::Vertical){
        return section;
    } else {
        return CONST::getHeadingString(this->indexMap_.at(section));
    }
}

void TeacherTableModel::setDataList(QList<Teacher> list){
    beginResetModel();
    this->list_ = list;
    endResetModel();
}

QList<Teacher> TeacherTableModel::getDataList(){
    return this->list_;
}

QVector<CONST::HDG> TeacherTableModel::getHeaderIndexs() const{
    return this->indexMap_;
}

void TeacherTableModel::sort(int column, Qt::SortOrder order){
    // std::function is about two times slower than lambda expression
//    std::function<bool(Teacher, Teacher)> func;
    beginResetModel();

    switch(this->indexMap_[column]){
    case CONST::HDG::ID:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getId() < b.getId(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getId() >= b.getId(); });
        }
        break;
    case CONST::HDG::NAME:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [this](Teacher a, Teacher b) -> bool {
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
            qStableSort(this->list_.begin(), this->list_.end(), [this](Teacher a, Teacher b) -> bool {
#if QT_VERSION >= 0x050200
                return this->zh_CN_collate_.compare(a.getName(), b.getName()) < 0;
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
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getSex() < b.getSex(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getSex() >= b.getSex(); });
        }
        break;
    case CONST::HDG::BIRTHDAY:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getBirthday() < b.getBirthday(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getBirthday() >= b.getBirthday(); });
        }
        break;
    case CONST::HDG::IDNUMBER:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getIdNumber() < b.getIdNumber(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getIdNumber() >= b.getIdNumber(); });
        }
        break;
    case CONST::HDG::POSITION:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getPosition() < b.getPosition(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getPosition() >= b.getPosition(); });
        }
        break;
    case CONST::HDG::DEPT:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getDepartment() < b.getDepartment(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](Teacher a, Teacher b) -> bool {return a.getDepartment() >= b.getDepartment(); });
        }
        break;
    default:
        break;
    }
    endResetModel();
}

void TeacherTableModel::filter(){
    if(! this->filterEnabled_)
        return;
    this->isFiltering_ = true;

    QList<Teacher> newList;
    switch (this->filterColumn_) {
    case CONST::HDG::ID: {
        if (this->useRegexp_) {
            for (Teacher &teacher : this->listOrig_){
                if(teacher.getId().contains(this->filterRegexp_)){
                    newList.append(teacher);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Teacher &teacher : this->listOrig_){
                if(teacher.getId().contains(this->filterString_, sensitivity)) {
                    newList.append(teacher);
                }
            }
        }
    }
        break;
    case CONST::HDG::NAME: {
        if (this->useRegexp_) {
            for (Teacher &teacher : this->listOrig_){
                if(teacher.getName().contains(this->filterRegexp_)){
                    newList.append(teacher);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Teacher &teacher : this->listOrig_){
                if(teacher.getName().contains(this->filterString_, sensitivity)) {
                    newList.append(teacher);
                }
            }
        }
    }
        break;
    case CONST::HDG::SEX: {
        for (Teacher &teacher : this->listOrig_){
            if(teacher.getSex() == this->filterSex_){
                newList.append(teacher);
            }
        }
    }
        break;
    case CONST::HDG::BIRTHDAY: {
        for (Teacher &teacher : this->listOrig_){
            if(this->minDate_ <= teacher.getBirthday() && teacher.getBirthday() <= this->maxDate_){
                newList.append(teacher);
            }
        }
    }
        break;
    case CONST::HDG::IDNUMBER: {
        if (this->useRegexp_) {
            for (Teacher &teacher : this->listOrig_){
                if(teacher.getIdNumber().contains(this->filterRegexp_)){
                    newList.append(teacher);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Teacher &teacher : this->listOrig_){
                if(teacher.getIdNumber().contains(this->filterString_, sensitivity)) {
                    newList.append(teacher);
                }
            }
        }
    }
        break;
    case CONST::HDG::POSITION: {
        if (this->useRegexp_) {
            for (Teacher &teacher : this->listOrig_){
                if(teacher.getPosition().contains(this->filterRegexp_)){
                    newList.append(teacher);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Teacher &teacher : this->listOrig_){
                if(teacher.getPosition().contains(this->filterString_, sensitivity)) {
                    newList.append(teacher);
                }
            }
        }
    }
        break;
    case CONST::HDG::DEPT: {
        if (this->useRegexp_) {
            for (Teacher &teacher : this->listOrig_){
                if(teacher.getDepartment().contains(this->filterRegexp_)){
                    newList.append(teacher);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(Teacher &teacher : this->listOrig_){
                if(teacher.getDepartment().contains(this->filterString_, sensitivity)) {
                    newList.append(teacher);
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

void TeacherTableModel::setRegexp(QString pattern){
    this->filterRegexp_.setPattern(pattern);
    if(this->caseSensitive_){
        this->filterRegexp_.setPatternOptions(QRegularExpression::DontCaptureOption);
    } else {
        this->filterRegexp_.setPatternOptions(QRegularExpression::DontCaptureOption|QRegularExpression::CaseInsensitiveOption);
    }
}
