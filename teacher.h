#pragma once
#include "person.h"

class Teacher : virtual public Person
{
public:
    static const int TEACHER_DEPT_LEN = 20;
    static const int TEACHER_POSITION_LEN = 10;

    Teacher(const QString name_ = QString(),
            const QString id_ = QString(),
            const QString idNum_ = QString(),
            const Sex sex_ = Sex::Unspecified,
            const QDate birthDay_ = QDate(0,0,0),
            const QString dept = QString(),
            const QString position = QString());
    virtual ~Teacher();

    friend QDataStream &operator <<(QDataStream &out, const Teacher &teacher);
    friend QDataStream &operator >>(QDataStream &in, Teacher &teacher);

    virtual QString toString() const override;

 protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;
public:
    QString getDepartment() const;
    bool setDepartment(QString dept);

    QString getPosition() const;
    bool setPosition(QString position);

protected:
    QString dept;
    QString position;
};


inline QString Teacher::getDepartment() const{
    return this->dept;
}

inline bool Teacher::setDepartment(QString dept){
    if(dept.length() > TEACHER_DEPT_LEN){
        return false;
    }
    this->dept = dept;
    return true;
}

inline QString Teacher::getPosition() const{
    return this->position;
}

inline bool Teacher::setPosition(QString position){
    if(position.length() > TEACHER_POSITION_LEN){
        return false;
    }
    this->position = position;
    return true;
}
