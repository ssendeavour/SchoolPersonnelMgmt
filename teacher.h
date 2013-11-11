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
            const QString dept_ = QString(),
            const QString position_ = QString());
    virtual ~Teacher();

    friend QDataStream &operator <<(QDataStream &out, const Teacher &teacher);
    friend QDataStream &operator >>(QDataStream &in, Teacher &teacher);

    virtual QString toString() const override;

 protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;
public:
    QString getDepartment() const;
    bool setDepartment(QString dept_);

    QString getPosition() const;
    bool setPosition(QString position_);

protected:
    QString dept_;
    QString position_;
};


inline QString Teacher::getDepartment() const{
    return this->dept_;
}

inline bool Teacher::setDepartment(QString dept){
    if(dept.length() > TEACHER_DEPT_LEN){
        this->errorString_ = tr("department name %1 too long, current length: %2, max length: %3")
                .arg(dept).arg(dept.length()).arg(TEACHER_DEPT_LEN);
        return false;
    }
    this->dept_ = dept;
    return true;
}

inline QString Teacher::getPosition() const{
    return this->position_;
}

inline bool Teacher::setPosition(QString position){
    if(position.length() > TEACHER_POSITION_LEN){
        this->errorString_ = tr("position name %1 too long, current length %2, max length: %3")
                .arg(position).arg(position.length()).arg(TEACHER_POSITION_LEN);
        return false;
    }
    this->position_ = position;
    return true;
}
