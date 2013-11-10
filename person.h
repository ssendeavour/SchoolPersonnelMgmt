#pragma once
#include <QDate>
#include <QString>
#include <QDataStream>
#include <QList>
#include <QDebug>
#include <QCoreApplication>
#include <cstdint>

#include "const.h"

class Person
{
    Q_DECLARE_TR_FUNCTIONS(Person)

public:
    static const int PERSON_NAME_LEN = 10;
    static const int PERSON_ID_LEN = 6;
    static const int PERSON_ID_NUM_LEN = 18;
    static const int PERSON_SEX_LEN = 2;

    enum class Sex : u_int8_t {
        Male,
        Female,
        Unspecified,
        Other
    };

    Person(const QString name_ = QString(),
           const QString id_ = QString(),
           const QString idNum_ = QString(),
           const Sex sex_ = Sex::Unspecified,
           const QDate birthDay_ = QDate(0,0,0));

    virtual ~Person();

     friend QDataStream &operator <<(QDataStream &out, const Person &person);
     friend QDataStream &operator >>(QDataStream &in, Person &person);

public:

    static QString getSexString(Person::Sex sex);
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
    QString errorString_ = "";
    QString name_ = "";
    QString id_ = "";
    QString idNum_ = "";
    Sex sex_ = Sex::Unspecified;
    QDate birthDay_ = QDate();
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
     if(idNum.length() > PERSON_ID_NUM_LEN){
         this->errorString_ = tr("ID number \"%1\" too long, length: %2, max length: %3")
                 .arg(idNum).arg(idNum.length()).arg(PERSON_ID_NUM_LEN);
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
    if(!birthday.isValid()){
        this->errorString_ = tr("date is not valid");
        return false;
    }
    this->birthDay_ = birthday;
    return true;
 }
