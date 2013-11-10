#pragma once
#include "person.h"

class Student : virtual public Person
{
//    Q_OBJECT
public:
    static const int STUDENT_CLASSNO_LEN = 6;

    Student(const QString classNo = QString(),
            const QString name_ = QString(),
            const QString id_ = QString(),
            const QString idNum_ = QString(),
            const Sex sex_ = Sex::Unspecified,
            const QDate birthDay_ = QDate(0,0,0));

    virtual ~Student();
    friend QDataStream &operator <<(QDataStream &out, const Student &student);
    friend QDataStream &operator >>(QDataStream &in, Student &student);

public:
   static bool writeToFile(QString fileName, const QList<Student> *list);
   static QList<Student> *readFromFile(QString fileName);

public:
   virtual QString toString() const override;
   QString getClassNo() const;
   bool setClassNo(QString classNo);

protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;

protected:
    QString classNo;
};

inline QString Student::getClassNo() const{
    return this->classNo;
}

inline bool Student::setClassNo(QString classNo){
    if(classNo.length() > STUDENT_CLASSNO_LEN){
         this->errorString_ = tr("Class Number \"%1\" too long, length: %2, max length: %3")
                .arg(classNo).arg(classNo.length()).arg(STUDENT_CLASSNO_LEN);
        return false;
    }
    this->classNo = classNo;
    return true;
}
