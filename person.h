#pragma once

#include <QDate>
#include <QString>
#include <QDataStream>
#include <QList>
#include <QDebug>
#include <QCoreApplication>
#include <cstdint>
#include <QRegExp>
#include <QRegExpValidator>

#include "const.h"

class Person
{
    Q_DECLARE_TR_FUNCTIONS(Person)

public:
    static const int PERSON_NAME_LEN = 10;
    static const int PERSON_ID_LEN = 6;
    static const int PERSON_ID_NUM_LEN = 18;
    static const int PERSON_SEX_LEN = 2;
    static const QRegExpValidator *idNumberValidator;

    enum class Sex : quint8 {
        Male = 0,
        Female,
        Other,
        Unspecified
    };

    Person(const QString name_ = QString(),
           const QString id_ = QString(),
           const QString idNum_ = QString("000000000000000000"),
           const Sex sex_ = Sex::Unspecified,
           const QDate birthDay_ = QDate(1880,1,1));

    virtual ~Person();

     friend QDataStream &operator <<(QDataStream &out, const Person &person);
     friend QDataStream &operator >>(QDataStream &in, Person &person);

public:

    static QString getSexString(Person::Sex sex);
    static const QRegExpValidator* obtainIDNumberValidator();

    virtual QString toString() const;

    QString errorString() const;

    QString getName() const;
    bool setName(QString name_);

    QString getId() const;
    bool setId(QString id_);

    QString getIdNumber() const;
    bool setIdNumber(QString idNum_);

    Sex getSex() const;
    void setSex(Sex sex_);

    QDate getBirthday() const;
    bool setBirthday(QDate birthday);

protected:
    virtual QDataStream &writeBinary(QDataStream &out) const;
    virtual QDataStream &readBinary(QDataStream &in);

protected:
    QString errorString_;
    QString name_;
    QString id_;
    QString idNum_;
    Sex sex_;
    /* birtday valid Range: QDate(1880, 1, 1) <= birthDay_ <= QDate::currentDay(); */
    QDate birthDay_;
};


inline QString Person::errorString() const{
    return this->errorString_;
}
inline QString Person::getName() const{
     return this->name_;
 }

inline bool Person::setName(QString name){
     if(name.length() > PERSON_NAME_LEN){
         this->errorString_ = tr("name \"%1\" too long, length: %2, max length: %3").arg(name).arg(name.length()).arg(PERSON_NAME_LEN);
         return false;
     }
     this->name_ = name;
     return true;
 }

inline QString Person::getId() const{
     return this->id_;
 }

inline bool Person::setId(QString id){
     if(id.length() > PERSON_ID_LEN){
         this->errorString_ = tr("id \"%1\" too long, length: %2, max length: %3")
                 .arg(id).arg(id.length()).arg(PERSON_ID_LEN);
         return false;
     }
     this->id_ = id;
     return true;
 }

inline QString Person::getIdNumber() const{
     return this->idNum_;
 }

inline bool Person::setIdNumber(QString idNum){
    int pos = 0;
    if(Person::idNumberValidator->validate(idNum, pos) != QValidator::Acceptable){
         this->errorString_ = tr("ID number format error, should be 18 digitis or "
                                 "17 digits and letter x/X, id number: %1") .arg(idNum);
         return false;
     }
     this->idNum_ = idNum;
     return true;
 }

inline Person::Sex Person::getSex() const{
     return this->sex_;
 }

inline void Person::setSex(Sex sex){
     this->sex_ = sex;
 }

inline QDate Person::getBirthday() const{
     return this->birthDay_;
 }

inline bool Person::setBirthday(QDate birthday){
    if(birthday.isValid() && birthday <= QDate::currentDate() && birthday >= QDate(1880,1,1)){
        this->birthDay_ = birthday;
        return true;
    } else {
        this->errorString_ = tr("date is not valid or out of range, Date is: %1").arg(birthday.toString("yyyy-MM-dd"));
        return false;
    }
 }
