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

QList<Student> *Student::readFromFile(QString fileName)
{
    QList<Student> *list = new QList<Student>;
    if(!list){
        qDebug() << "memroty allocation fail";
        return nullptr;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << file.errorString();
        delete list;
        return nullptr;
    }

    QDataStream in(&file);
    u_int32_t magicNumber;
    in >> magicNumber;
    if(magicNumber != CONST::MAGIC_NUMBER){
        qDebug() << "unknown file format";
        delete list;
        return nullptr;
    }

    u_int32_t version;
    in >> version;
    if(version != CONST::VERSION_1_20131109){
        qDebug() << "unknow students data file version: " << version;
        delete list;
        return nullptr;
    }

    in.setVersion(QDataStream::Qt_4_2);

    quint64 size;
    in >> size;
    list->reserve(size);

    Student *stu = nullptr;
    for(quint64 i=0; i < size; ++i){
        stu = new Student;
        if(!stu){
            qDebug() << "memory allocation fail";
            list->clear();
            return list;
        }
        in >> *stu;
        list->append(*stu);
    }
    return list;
}

bool Student::writeToFile(QString fileName, const QList<Student> *list)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << file.errorString();
        return false;
    }

    QDataStream out(&file);

    out << CONST::MAGIC_NUMBER << CONST::VERSION_1_20131109;

    out.setVersion(QDataStream::Qt_4_2);
    out << static_cast<quint64>(list->size());

    for(Student item : *list){
        out << item;
        qDebug() << item.toString();
    }
    file.close();
    return true;
}
