#include "person.h"
#include <QStringBuilder>

Person::Person(const QString name /*= QString()*/,
               const QString id /*= QString()*/,
               const QString idNum/* = QString()*/,
               const QString sex /*= QString()*/,
               const QDate birthDay /*= QDate(0,0,0)*/):
    name(name), id(id), idNum(idNum), sex(sex), birthDay(birthDay)
{
}

Person::~Person()
{
}

 QDataStream &Person::writeBinary(QDataStream &out) const {
     out << this->name << this->id << this->idNum << this->sex << this->birthDay;
     return out;
 }

 QDataStream &Person::readBinary(QDataStream &in) {
     in >> this->name >> this->id >> this->idNum >> this->sex >> this->birthDay;
     return in;
 }

 QDataStream &operator <<(QDataStream &out, const Person &person){
     return person.writeBinary(out);
 }

 QDataStream &operator >>(QDataStream &in, Person &person){
     return person.readBinary(in);
 }

 QString Person::toString()
 {
     return "Name: " % name % ", Id: " % id % ", IdNum: " % idNum
             % ", Sex: " % sex % ", Birthday: " % birthDay.toString("yyyy-MM-dd");
 }


