#include "teacher.h"

Teacher::Teacher(const QString name /*= QString()*/,
        const QString id /*= QString()*/,
        const QString idNum /*= QString("000000000000000000")*/,
        const Sex sex /*= QString()*/,
        const QDate birthDay /*= QDate(1880,1,1)*/,
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

QList<Teacher> Teacher::readFromFile(QFile &file, QString &error)
{
    QList<Teacher> list;

    QDataStream in(&file);
    u_int32_t magicNumber;
    in >> magicNumber;
    if(magicNumber != CONST::MAGIC_NUMBER){
        error = tr("Wrong file format, not a School Personnel Management data file");
        return list;
    }

    u_int32_t filetype;
    in >> filetype;
    if(filetype != CONST::FILE_TYPE_TEACHER){
        error = tr("Wrong file type, not a Teacher data file (extension: %1)").arg(CONST::FILE_EXTENSION_TEACHER);
        return list;
    }

    u_int32_t version;
    in >> version;
    if(version != CONST::VERSION_1_20131109){
        error = tr("unknow Teacher data file version: %1").arg(version);
        return list;
    }
    in.setVersion(QDataStream::Qt_4_2);

    quint64 size;
    in >> size;
    list.reserve(size);

    Teacher stu;
    for(quint64 i=0; i < size; ++i){
        in >> stu;
        list.append(stu);
    }
    return list;
}

bool Teacher::writeToFile(QFile &file, const QList<Teacher> list)
{
    QDataStream out(&file);

    out << CONST::MAGIC_NUMBER << CONST::FILE_TYPE_TEACHER << CONST::VERSION_1_20131109;

    out.setVersion(QDataStream::Qt_4_2);
    out << static_cast<quint64>(list.size());

    for(Teacher item : list){
        out << item;
    }
    return true;
}
