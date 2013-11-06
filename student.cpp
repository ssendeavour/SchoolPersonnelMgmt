#include "student.h"

Student::Student(const QString classNo /*= QString()*/,
                 const QString name /*= QString()*/,
                 const QString id /*= QString()*/,
                 const QString idNum /*= QString()*/,
                 const QString sex /*= QString()*/,
                 const QDate birthDay /*= QDate(0,0,0)*/):
    Person(name, id, idNum, sex, birthDay), classNo(classNo)
{
}

Student::~Student()
{
}

QDataStream &operator <<(QDataStream &out, const Student &student)
{
    return student.writeBinary(out);
}

QDataStream &operator >> (QDataStream &in, Student &student)
{
    return student.readBinary(in);
}

QDataStream &Student::writeBinary(QDataStream &out) const{
    return Person::writeBinary(out) << this->classNo;
}

QDataStream &Student::readBinary(QDataStream &in){
    return Person::readBinary(in) >> this->classNo;
}

QString Student::toString()
{
    return Person::toString() + ", ClassNo: " + classNo;
}
