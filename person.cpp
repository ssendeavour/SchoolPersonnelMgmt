#include "person.h"
#include <QStringBuilder>
#include <type_traits>

Person::Person(const QString name /*= QString()*/,
               const QString id /*= QString()*/,
               const QString idNum/* = QString()*/,
               const Sex sex /*= Sex::Unspecified*/,
               const QDate birthDay /*= QDate(0,0,0)*/) : sex_(sex)
{
    if(!setName(name)){
        qDebug() << this->errorString_;
        setName("");
    }
    if(!setId(id)){
        qDebug() << this->errorString_;
        id_ = tr("");
    }
    if(!setIdNumber(idNum)){
        qDebug() << this->errorString_;
        setIdNumber(QString("000000000000000000"));
    }
    if(!setBirthday(birthDay)){
        qDebug() << this->errorString_;
        birthDay_ = QDate(1880, 1, 1);
    }
}

Person::~Person()
{
}

const QRegExpValidator *Person::idNumberValidator =  new QRegExpValidator(QRegExp("^\\d{17}[0-9xX]$") , nullptr);

 QDataStream &Person::writeBinary(QDataStream &out) const {
     out << this->name_ << this->id_ << this->idNum_
         << static_cast<std::underlying_type<Person::Sex>::type>(this->sex_) << this->birthDay_;
     return out;
 }

 QDataStream &Person::readBinary(QDataStream &in) {
     std::underlying_type<Person::Sex>::type sex;
     in >> this->name_ >> this->id_ >> this->idNum_
        >> sex >> this->birthDay_;
     sex_ = static_cast<Person::Sex>(sex);
     return in;
 }

 QDataStream &operator <<(QDataStream &out, const Person &person){
     return person.writeBinary(out);
 }

 QDataStream &operator >>(QDataStream &in, Person &person){
     return person.readBinary(in);
 }

 QString Person::toString() const
 {
     return tr("Name: %1, Id: %2, Id number: %3, Sex: %4, Birthday: %5")
             .arg(name_).arg(id_).arg(idNum_).arg(getSexString(sex_)).arg(birthDay_.toString("yyyy-MM-dd"));
 }

QString Person::getSexString(Sex sex){
    switch(sex){
    case Person::Sex::Male:
        return tr("Male");
    case Person::Sex::Female:
        return tr("Female");
    case Person::Sex::Unspecified:
        return tr("Unspecified");
    default:
        return tr("Other");
    }
}

const QRegExpValidator* Person::obtainIDNumberValidator(){
    return idNumberValidator;
}
