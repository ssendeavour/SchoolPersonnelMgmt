#include "teacher.h"

Teacher::Teacher(const QString name /*= QString()*/,
        const QString id /*= QString()*/,
        const QString idNum /*= QString()*/,
        const Sex sex /*= QString()*/,
        const QDate birthDay /*= QDate(0,0,0)*/,
        const QString dept /*= QString()*/,
        const QString position /*= QString()*/):
    Person(name, id, idNum, sex, birthDay)
{
    if(!setDepartment(dept)){
        qDebug() << this->errorString_;
        setDepartment(tr("none"));
    }

    if(!setPosition(position)){
        qDebug() << this->errorString_;
        setPosition(tr("none"));
    }
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
    return Person::writeBinary(out) << this->dept_ << this->position_;
}

QDataStream &Teacher::readBinary(QDataStream &in){
    return Person::readBinary(in) >> this->dept_ >> this->position_;
}

QString Teacher::toString() const
{
    return tr("%1, Department: %2, Position: %3").arg(Person::toString()).arg(dept_).arg(position_);
}
