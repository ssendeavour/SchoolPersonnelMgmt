#include "student.h"

#include <QFile>

Student::Student(const QString classNo /*= QString()*/,
                 const QString name /*= QString()*/,
                 const QString id /*= QString()*/,
                 const QString idNum /*= QString()*/,
                 const Sex sex /*= QString()*/,
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

QString Student::toString() const
{
    return tr("%1, Class No.: %2").arg(Person::toString()).arg(classNo);
}

QList<Student> Student::readFromFile(QFile &file, bool &succeed)
{
    QList<Student> list;
    succeed = false;

    QDataStream in(&file);
    u_int32_t magicNumber;
    in >> magicNumber;
    if(magicNumber != CONST::MAGIC_NUMBER){
        qDebug() << "unknown file format";
        return list;
    }

    u_int32_t version;
    in >> version;
    if(version != CONST::VERSION_1_20131109){
        qDebug() << "unknow students data file version: " << version;
        return list;
    }

    in.setVersion(QDataStream::Qt_4_2);

    quint64 size;
    in >> size;
    list.reserve(size);

    Student stu;
    for(quint64 i=0; i < size; ++i){
        in >> stu;
        list.append(stu);
    }
    succeed = true;
    return list;
}

bool Student::writeToFile(QFile &file, const QList<Student> list)
{
    QDataStream out(&file);

    out << CONST::MAGIC_NUMBER << CONST::VERSION_1_20131109;

    out.setVersion(QDataStream::Qt_4_2);
    out << static_cast<quint64>(list.size());

    for(Student item : list){
        out << item;
        qDebug() << item.toString();
    }
    return true;
}
