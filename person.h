#pragma once
#include <QObject>
#include <QDate>
#include <QString>
#include <QDataStream>

class Person : public QObject
{
    Q_OBJECT
public:
    static const int PERSON_NAME_LEN = 10;
    static const int PERSON_ID_LEN = 6;
    static const int PERSON_ID_NUM_LEN = 18;
    static const int PERSON_SEX_LEN = 2;

    Person(const QString name = QString(),
           const QString id = QString(),
           const QString idNum = QString(),
           const QString sex = QString(),
           const QDate birthDay = QDate(0,0,0));

    virtual QString toString();

    virtual ~Person();

     friend QDataStream &operator <<(QDataStream &out, const Person &person);
     friend QDataStream &operator >>(QDataStream &in, Person &person);

 protected:
     virtual QDataStream &writeBinary(QDataStream &out) const;
     virtual QDataStream &readBinary(QDataStream &in);

signals:

public slots:

    QString getName() const;
    bool setName(QString name);

    QString getId() const;
    bool setId(QString id);

    QString getIdNumber() const;
    bool setIdNumber(QString idNum);

    QString getSex() const;
    bool setSex(QString sex);

    QDate getBirthday() const;
    bool setBirthday(QDate birthday);

protected:
    QString name;
    QString id;
    QString idNum;
    QString sex;
    QDate birthDay;
};


inline QString Person::getName() const{
     return this->name;
 }

inline bool Person::setName(QString name){
     if(name.length() > PERSON_NAME_LEN){
         return false;
     }
     this->name = name;
     return true;
 }

inline QString Person::getId() const{
     return this->id;
 }

inline bool Person::setId(QString id){
     if(id.length() > PERSON_ID_LEN){
         return false;
     }
     this->id = id;
     return true;
 }

inline QString Person::getIdNumber() const{
     return this->idNum;
 }

inline bool Person::setIdNumber(QString idNum){
     if(idNum.length() > PERSON_ID_NUM_LEN){
         return false;
     }
     this->idNum = idNum;
     return true;
 }

inline QString Person::getSex() const{
     return this->sex;
 }

inline bool Person::setSex(QString sex){
     if(sex.length() > PERSON_SEX_LEN){
         return false;
     }
     this->sex = sex;
     return true;
 }

inline QDate Person::getBirthday() const{
     return this->birthDay;
 }

inline bool Person::setBirthday(QDate birthday){
     this->birthDay = birthday;
     return true;
 }
