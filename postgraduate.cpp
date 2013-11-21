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

QList<Postgraduate> Postgraduate::readFromFile(QFile &file, QString &error)
{
    QList<Postgraduate> list;

    QDataStream in(&file);
    u_int32_t magicNumber;
    in >> magicNumber;
    if(magicNumber != CONST::MAGIC_NUMBER){
        error = tr("Wrong file format, not a School Personnel Management data file");
        return list;
    }

    u_int32_t filetype;
    in >> filetype;
    if(filetype != CONST::FILE_TYPE_POSTGRADUATE){
        error = tr("Wrong file type, not a Postgraduate data file (extension: %1)").arg(CONST::FILE_EXTENSION_POSTGRADUATE);
        return list;
    }

    u_int32_t version;
    in >> version;
    if(version != CONST::VERSION_1_20131109){
        error = tr("unknow Postgraduate data file version: %1").arg(version);
        return list;
    }
    in.setVersion(QDataStream::Qt_4_2);

    quint64 size;
    in >> size;
    list.reserve(size);

    Postgraduate stu;
    for(quint64 i=0; i < size; ++i){
        in >> stu;
        list.append(stu);
    }
    return list;
}

bool Postgraduate::writeToFile(QFile &file, const QList<Postgraduate> list)
{
    QDataStream out(&file);

    out << CONST::MAGIC_NUMBER << CONST::FILE_TYPE_POSTGRADUATE << CONST::VERSION_1_20131109;

    out.setVersion(QDataStream::Qt_4_2);
    out << static_cast<quint64>(list.size());

    for(Postgraduate item : list){
        out << item;
    }
    return true;
}
