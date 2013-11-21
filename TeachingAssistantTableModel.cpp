#include "TeachingAssistantTableModel.h"

#include <QSize>

TeachingAssistantTableModel::TeachingAssistantTableModel(QVector<CONST::HDG> indexMap,  QObject *parent /*= 0*/) :
    QAbstractTableModel(parent), indexMap_(indexMap) {
}

TeachingAssistantTableModel::~TeachingAssistantTableModel(){

}

int TeachingAssistantTableModel::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->list_.size();
}

int TeachingAssistantTableModel::columnCount(const QModelIndex & parent /*= QModelIndex()*/) const{
    return this->indexMap_.size();
}

QVariant TeachingAssistantTableModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const{
    //* NOTE: if used across days, Today string will be inaccurate.
    static QString birthTooltip = "Valid date range: 1880-01-01 to Today(" + QDate::currentDate().toString("yyyy-MM-dd") + ")";
    if(!index.isValid()){
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole){
        return static_cast<int>(Qt::AlignLeft|Qt::AlignVCenter);
    } else if ((role == Qt::DisplayRole|| role == Qt::EditRole) && !this->list_.isEmpty()){
        const TeachingAssistant &t = list_.at(index.row());
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
        case CONST::HDG::POSITION:
            return t.getPosition();
        case CONST::HDG::DEPT:
            return t.getDepartment();
        case CONST::HDG::TUTORID:
            return t.getTutorId();
        case CONST::HDG::MAJOR:
            return t.getMajor();
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

bool TeachingAssistantTableModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/){
    if(index.isValid() && role == Qt::EditRole){
        bool result = false;
        TeachingAssistant &t = this->list_[index.row()];
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
        case CONST::HDG::IDNUMBER:
            result = t.setIdNumber(value.toString());
            break;
        case CONST::HDG::POSITION:
            result = t.setPosition(value.toString());
        case CONST::HDG::DEPT:
            result = t.setDepartment(value.toString());
        case CONST::HDG::TUTORID:
            result = t.setTutorId(value.toString());
        case CONST::HDG::MAJOR:
            result = t.setMajor(value.toString());
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

Qt::ItemFlags TeachingAssistantTableModel::flags(const QModelIndex & index) const{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

bool TeachingAssistantTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    // NOTE: [row, row+count-1], not [row, row+count)
    beginInsertRows(parent, row, row + count - 1);
    for(int i = row; i < row + count; ++i){
        this->list_.insert(i, TeachingAssistant());
    }
    endInsertRows();
    return true;
}

// remove from high index to low index
bool TeachingAssistantTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    // NOTE: [row, row+count-1], not [row, row+count)
    beginRemoveRows(parent, row, row + count -1);
    for(int i = row + count - 1; i >= row; i--){
        this->list_.removeAt(i);
    }
    endRemoveRows();
    return true;
}

QVariant TeachingAssistantTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(orientation == Qt::Vertical){
        return section;
    } else {
        return CONST::getHeadingString(this->indexMap_.at(section));
    }
}

void TeachingAssistantTableModel::setDataList(QList<TeachingAssistant> list){
    beginResetModel();
    this->list_ = list;
    endResetModel();
}

QList<TeachingAssistant> TeachingAssistantTableModel::getDataList(){
    return this->list_;
}

QVector<CONST::HDG> TeachingAssistantTableModel::getHeaderIndexs() const{
    return this->indexMap_;
}

void TeachingAssistantTableModel::sort(int column, Qt::SortOrder order){
    // std::function is about two times slower than lambda expression
//    std::function<bool(TeachingAssistant, TeachingAssistant)> func;
    beginResetModel();

    switch(this->indexMap_[column]){
    case CONST::HDG::ID:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getId() < b.getId(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getId() >= b.getId(); });
        }
        break;
    case CONST::HDG::NAME:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [this](TeachingAssistant a, TeachingAssistant b) -> bool {
                std::string sa = a.getName().toStdString();
                std::string sb = b.getName().toStdString();
                const char *pa = sa.data();
                const char *pb = sb.data();
                return this->zh_CN_collate_.compare(pa, pa + sa.size(), pb, pb + sb.size()) < 0;
            });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [this](TeachingAssistant a, TeachingAssistant b) -> bool {
                std::string sa = a.getName().toStdString();
                std::string sb = b.getName().toStdString();
                const char *pa = sa.data();
                const char *pb = sb.data();
                return this->zh_CN_collate_.compare(pa, pa + sa.size(), pb, pb + sb.size()) >= 0;
            });
        }
        break;
    case CONST::HDG::SEX:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getSex() < b.getSex(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getSex() >= b.getSex(); });
        }
        break;
    case CONST::HDG::BIRTHDAY:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getBirthday() < b.getBirthday(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getBirthday() >= b.getBirthday(); });
        }
        break;
    case CONST::HDG::CLASSNO:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getClassNo() < b.getClassNo(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getClassNo() >= b.getClassNo(); });
        }
        break;
    case CONST::HDG::IDNUMBER:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getIdNumber() < b.getIdNumber(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getIdNumber() >= b.getIdNumber(); });
        }
        break;
    case CONST::HDG::POSITION:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getPosition() < b.getPosition(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getPosition() >= b.getPosition(); });
        }
        break;
    case CONST::HDG::DEPT:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getDepartment() < b.getDepartment(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getDepartment() >= b.getDepartment(); });
        }
        break;
    case CONST::HDG::MAJOR:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getMajor() < b.getMajor(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getMajor() >= b.getMajor(); });
        }
        break;
    case CONST::HDG::TUTORID:
        if(Qt::AscendingOrder == order){
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getTutorId() < b.getTutorId(); });
        } else {
            qStableSort(this->list_.begin(), this->list_.end(), [](TeachingAssistant a, TeachingAssistant b) -> bool {return a.getTutorId() >= b.getTutorId(); });
        }
        break;
    default:
        break;
    }
    endResetModel();
}

void TeachingAssistantTableModel::filter(){
    if(! this->filterEnabled_)
        return;
    this->isFiltering_ = true;

    QList<TeachingAssistant> newList;
    switch (this->filterColumn_) {
    case CONST::HDG::ID: {
        if (this->useRegexp_) {
            for (TeachingAssistant &ta : this->listOrig_){
                if(ta.getId().contains(this->filterRegexp_)){
                    newList.append(ta);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(TeachingAssistant &ta : this->listOrig_){
                if(ta.getId().contains(this->filterString_, sensitivity)) {
                    newList.append(ta);
                }
            }
        }
    }
        break;
    case CONST::HDG::NAME: {
        if (this->useRegexp_) {
            for (TeachingAssistant &ta : this->listOrig_){
                if(ta.getName().contains(this->filterRegexp_)){
                    newList.append(ta);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(TeachingAssistant &ta : this->listOrig_){
                if(ta.getName().contains(this->filterString_, sensitivity)) {
                    newList.append(ta);
                }
            }
        }
    }
        break;
    case CONST::HDG::SEX: {
        for (TeachingAssistant &ta : this->listOrig_){
            if(ta.getSex() == this->filterSex_){
                newList.append(ta);
            }
        }
    }
        break;
    case CONST::HDG::BIRTHDAY: {
        for (TeachingAssistant &ta : this->listOrig_){
            if(this->minDate_ <= ta.getBirthday() && ta.getBirthday() <= this->maxDate_){
                newList.append(ta);
            }
        }
    }
        break;
    case CONST::HDG::IDNUMBER: {
        if (this->useRegexp_) {
            for (TeachingAssistant &ta : this->listOrig_){
                if(ta.getIdNumber().contains(this->filterRegexp_)){
                    newList.append(ta);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(TeachingAssistant &ta : this->listOrig_){
                if(ta.getIdNumber().contains(this->filterString_, sensitivity)) {
                    newList.append(ta);
                }
            }
        }
    }
        break;
    case CONST::HDG::POSITION: {
        if (this->useRegexp_) {
            for (TeachingAssistant &ta : this->listOrig_){
                if(ta.getPosition().contains(this->filterRegexp_)){
                    newList.append(ta);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(TeachingAssistant &ta : this->listOrig_){
                if(ta.getPosition().contains(this->filterString_, sensitivity)) {
                    newList.append(ta);
                }
            }
        }
    }
        break;
    case CONST::HDG::CLASSNO: {
        if (this->useRegexp_) {
            for (TeachingAssistant &ta : this->listOrig_){
                if(ta.getClassNo().contains(this->filterRegexp_)){
                    newList.append(ta);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(TeachingAssistant &ta : this->listOrig_){
                if(ta.getClassNo().contains(this->filterString_, sensitivity)) {
                    newList.append(ta);
                }
            }
        }
    }
        break;
    case CONST::HDG::DEPT: {
        if (this->useRegexp_) {
            for (TeachingAssistant &ta : this->listOrig_){
                if(ta.getDepartment().contains(this->filterRegexp_)){
                    newList.append(ta);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(TeachingAssistant &ta : this->listOrig_){
                if(ta.getDepartment().contains(this->filterString_, sensitivity)) {
                    newList.append(ta);
                }
            }
        }
    }
    case CONST::HDG::MAJOR: {
        if (this->useRegexp_) {
            for (TeachingAssistant &ta : this->listOrig_){
                if(ta.getMajor().contains(this->filterRegexp_)){
                    newList.append(ta);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(TeachingAssistant &ta : this->listOrig_){
                if(ta.getMajor().contains(this->filterString_, sensitivity)) {
                    newList.append(ta);
                }
            }
        }
    }
        break;
    case CONST::HDG::TUTORID: {
        if (this->useRegexp_) {
            for (TeachingAssistant &ta : this->listOrig_){
                if(ta.getTutorId().contains(this->filterRegexp_)){
                    newList.append(ta);
                }
            }
        } else {
            Qt::CaseSensitivity sensitivity = this->caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
            for(TeachingAssistant &ta : this->listOrig_){
                if(ta.getTutorId().contains(this->filterString_, sensitivity)) {
                    newList.append(ta);
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

void TeachingAssistantTableModel::setRegexp(QString pattern){
    this->filterRegexp_.setPattern(pattern);
    if(this->caseSensitive_){
        this->filterRegexp_.setPatternOptions(QRegularExpression::DontCaptureOption);
    } else {
        this->filterRegexp_.setPatternOptions(QRegularExpression::DontCaptureOption|QRegularExpression::CaseInsensitiveOption);
    }
}
