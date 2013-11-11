#include "teachingassistant.h"

TeachingAssistant::TeachingAssistant(
        const QString classNo /*= QString()*/,
        const QString name /*= QString()*/,
        const QString id /*= QString()*/,
        const QString idNum /*= QString()*/,
        const Sex sex /*= QString()*/,
        const QDate birthDay /*= QDate(0,0,0)*/,
        const QString major /*= QString()*/,
        const QString tutorId /*= QString()*/,
        const QString dept /*= QString()*/,
        const QString position /*= QString()*/
        ) : Person(name, id, idNum, sex, birthDay),
    Teacher(name, id, idNum, sex, birthDay, dept, position),
    Postgraduate(classNo, name, id, idNum, sex, birthDay, major, tutorId)
{
}

TeachingAssistant::~TeachingAssistant()
{
}

QDataStream &operator <<(QDataStream &out, const TeachingAssistant &ta){
    return ta.writeBinary(out);
}

QDataStream &operator >>(QDataStream &in, TeachingAssistant &ta){
    return ta.readBinary(in);
}

QString TeachingAssistant::toString() const{
   return tr("%1, Department: %2, Position: %3").arg(Postgraduate::toString()).arg(dept_).arg(position_);
}

QDataStream &TeachingAssistant::writeBinary(QDataStream &out) const{
    return Postgraduate::writeBinary(out) << this->dept_ << this->position_;
}

QDataStream &TeachingAssistant::readBinary(QDataStream &in){
    return Postgraduate::readBinary(in) >> this->dept_ >> this->position_;
}

