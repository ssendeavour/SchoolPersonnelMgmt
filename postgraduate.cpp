#include "postgraduate.h"

Postgraduate::Postgraduate(const QString classNo /*= QString()*/,
                           const QString name /*= QString()*/,
                           const QString id /*= QString()*/,
                           const QString idNum /*= QString("000000000000000000")*/,
                           const Sex sex /*= QString()*/,
                           const QDate birthDay /*= QDate(1880,1,1)*/,
                           const QString major /*= QString()*/,
                           const QString tutorId /*= QString()*/) :
    Person(name, id, idNum, sex, birthDay),
    Student(classNo, name, id, idNum, sex, birthDay)
{
    if(!setMajor(major)){
        qDebug() << this->errorString_;
        setMajor(tr("none"));
    }

    if(!setTutorId(tutorId)){
        qDebug() << this->errorString_;
        setTutorId(tr("none"));
    }
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
    return Student::writeBinary(out) << this->major_ << this->tutorId_;
}

QDataStream &Postgraduate::readBinary(QDataStream &in){
    return Student::readBinary(in) >> this->major_ >> this->tutorId_;
}

QString Postgraduate::toString() const
{
    return tr("%1, Major: %2, Tutor's id: %3").arg(Student::toString()).arg(major_).arg(tutorId_);
}
