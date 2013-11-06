#pragma once
#include "person.h"

class Student : virtual public Person
{
    Q_OBJECT
public:
    static const int STUDENT_CLASSNO_LEN = 6;

    Student(const QString classNo = QString(),
            const QString name = QString(),
            const QString id = QString(),
            const QString idNum = QString(),
            const QString sex = QString(),
            const QDate birthDay = QDate(0,0,0));

    virtual ~Student();
    friend QDataStream &operator <<(QDataStream &out, const Student &student);
    friend QDataStream &operator >>(QDataStream &in, Student &student);

    virtual QString toString() override;

 protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;

signals:

public slots:
   QString getClassNo() const;
   bool setClassNo(QString classNo);

protected:
    QString classNo;
};

inline QString Student::getClassNo() const{
    return this->classNo;
}

inline bool Student::setClassNo(QString classNo){
    if(classNo.length() > STUDENT_CLASSNO_LEN){
        return false;
    }
    this->classNo = classNo;
    return true;
}
