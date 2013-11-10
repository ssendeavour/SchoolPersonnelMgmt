#include "teacher.h"

Teacher::Teacher(const QString name /*= QString()*/,
        const QString id /*= QString()*/,
        const QString idNum /*= QString()*/,
        const Sex sex /*= QString()*/,
        const QDate birthDay /*= QDate(0,0,0)*/,
        const QString dept /*= QString()*/,
        const QString position /*= QString()*/):
    Person(name, id, idNum, sex, birthDay), dept(dept), position(position)
{
}

Teacher::~Teacher()
{
}

QDataStream &operator <<(QDataStream &out, const Teacher &teacher)
{
    return teacher.writeBinary(out);
}

QDataStream &operator >>(QDataStream &in, Teacher &teacher)
{
    return teacher.readBinary(in);
}

QDataStream &Teacher::writeBinary(QDataStream &out) const{
    return Person::writeBinary(out) << this->dept << this->position;
}

QDataStream &Teacher::readBinary(QDataStream &in){
    return Person::readBinary(in) >> this->dept >> this->position;
}

QString Teacher::toString() const
{
    return tr("%1, Department: %2, Position: %3").arg(Person::toString()).arg(dept).arg(position);
}
