#pragma once

/* QT Q_OBJECTS doesn't support templated classes, so convert back to TeacherTabloeModel */

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include <QStringList>
#include <QAtomicInt>
#include <QRegularExpression>

#if QT_VERSION >= 0x050200
#include <QCollator>
#else
#include <locale>
#endif

#include "teacher.h"

class locale;

class TeacherTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TeacherTableModel(QVector<CONST::HDG> indexMap, QObject *parent = 0);
    ~TeacherTableModel();

    // SortProxyModel using this model as source model should override these same functions
    // must override
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // optional override
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order);

    void setDataList(QList<Teacher> list);
    QList<Teacher> getDataList();
    QVector<CONST::HDG> getHeaderIndexs() const;

signals:

public slots:
    void setEnableFilter(bool enable);
    void setFilterColumn(const CONST::HDG column);
    void setFilterString(const QString &newString);
    void setFilterSex(const Person::Sex sex);
    void setFilterCaseSentivity(bool sensitive);
    void setFilterUseRegexp(bool yes);
    void setFilterMinDate(const QDate &min);
    void setFilterMaxDate(const QDate &max);

protected:

// sort Chinese by Pinyin
// Linux g++ locale 名称: "zh_CN.utf"
// VC2010 locale 名称：	"Chinese"或者"Chinese_china"
//#ifdef _MSC_VER
#if QT_VERSION >= 0x050200
    const QCollator zh_CN_locale_ = QCollator(QLocale(QLocale::Chinese, QLocale::China));
#else
#   ifdef _WIN32
    const std::locale zh_CN_locale_ = std::locale("Chinese_china");
#   else
    const std::locale zh_CN_locale_ = std::locale("zh_CN.utf8");
#   endif
    const std::collate<char>& zh_CN_collate_ = std::use_facet<std::collate<char> >(zh_CN_locale_);
#endif

    void filter();

    void setRegexp(const QString pattern);

private:
    QList<Teacher> list_ = QList<Teacher>();
    QList<Teacher> listOrig_ = list_;
    bool isFiltering_ = false;
    bool filterEnabled_ = false;
    bool useRegexp_ = false;
    bool caseSensitive_ = false;
    CONST::HDG filterColumn_ = CONST::HDG::ID;
    QRegularExpression filterRegexp_ = QRegularExpression(".*", QRegularExpression::DontCaptureOption);
    Person::Sex filterSex_ = Person::Sex::Male;
    QString filterString_ = "";
    QDate minDate_ = QDate(1880, 1, 1);
    QDate maxDate_ = QDate::currentDate();

    // column index map to CONST::HDG enum types
    QVector<CONST::HDG> indexMap_ = QVector<CONST::HDG>(static_cast<int>(CONST::HDG::COUNT));
};

inline void TeacherTableModel::setEnableFilter(bool enable){
    this->filterEnabled_ = enable;
    if(enable){
        this->listOrig_ = this->list_;
    } else {
        beginResetModel();
        this->list_ = this->listOrig_;
        endResetModel();
    }
    qDebug() << "filter enabled: " << enable;
}

inline void TeacherTableModel::setFilterColumn(const CONST::HDG column){
    if(this->isFiltering_ == false && column != this->filterColumn_){
        this->filterColumn_ = column;
        qDebug() << "filter column: " << CONST::getHeadingString(column);
        filter();
    }
}

inline void TeacherTableModel::setFilterString(const QString &newString){
    if(this->isFiltering_ == false && newString != this->filterString_){
        this->filterString_ = newString;
        if(this->useRegexp_) {
            setRegexp(newString);
        }
        qDebug() << "Filter string: " << newString << ", use regexp: " << this->useRegexp_;
        filter();
    }
}

inline void TeacherTableModel::setFilterSex(const Person::Sex sex){
    if(this->isFiltering_ == 0 && sex != this->filterSex_){
        this->filterSex_ =sex;
        qDebug() << "filter Sex: " << Person::getSexString(sex);
        filter();
    }
}

inline void TeacherTableModel::setFilterCaseSentivity(bool sensitive){
    if(this->isFiltering_ == 0 && sensitive != this->caseSensitive_){
        this->caseSensitive_ = sensitive;
        qDebug() << "filter Case: " << sensitive;
        if(this->useRegexp_){
            setRegexp(this->filterString_);
        }
        filter();
    }
}

inline void TeacherTableModel::setFilterUseRegexp(bool yes){
    if(this->isFiltering_ == 0 && yes != this->useRegexp_){
        this->useRegexp_ = yes;
        qDebug() << "filter use Regexp: " << yes;
        if(yes){
            setRegexp(this->filterString_);
        }
        filter();
    }
}

inline void TeacherTableModel::setFilterMinDate(const QDate &min){
    if(this->isFiltering_ == 0 && min != this->minDate_){
        this->minDate_ = min;
        qDebug() << "filter Min date: " << min.toString("yyyy-MM-dd");
        filter();
    }
}

inline void TeacherTableModel::setFilterMaxDate(const QDate &max){
    if(this->isFiltering_ == 0 && max != this->maxDate_){
        this->maxDate_ = max;
        qDebug() << "filter Max date: " << max.toString("yyyy-MM-dd");
        filter();
    }
}
