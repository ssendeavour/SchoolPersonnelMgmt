#include "postgraduate.h"

Postgraduate::Postgraduate(const QString classNo /*= QString()*/,
                           const QString name /*= QString()*/,
                           const QString id /*= QString()*/,
                           const QString idNum /*= QString()*/,
                           const QString sex /*= QString()*/,
                           const QDate birthDay /*= QDate(0,0,0)*/,
                           const QString major /*= QString()*/,
                           const QString tutorId /*= QString()*/) :
    Student(classNo, name, id, idNum, sex, birthDay), tutorId(tutorId)
{
    this->major = major;
}

Postgraduate::~Postgraduate()
{
}

QDataStream &operator <<(QDataStream &out, const Postgraduate &pg)
{
    return pg.writeBinary(out);
}

QDataStream &operator >> (QDataStream &in, Postgraduate &pg)
{
    return pg.readBinary(in);
}

QDataStream &Postgraduate::writeBinary(QDataStream &out) const{
    return Student::writeBinary(out) << this->major << this->tutorId;
}

QDataStream &Postgraduate::readBinary(QDataStream &in){
    return Student::readBinary(in) >> this->major >> this->tutorId;
}

QString Postgraduate::toString()
{
    return Student::toString() + ", Major: " + this->major + "\nTutorId: " + this->tutorId;
}
