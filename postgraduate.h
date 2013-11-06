#pragma once
#include "student.h"
#include "teacher.h"

class Postgraduate : public Student
{
    Q_OBJECT
public:
    static const int POSTGRADUATE_MAJOR_LEN = 20;

    Postgraduate(const QString classNo = QString(),
                 const QString name = QString(),
                 const QString id = QString(),
                 const QString idNum = QString(),
                 const QString sex = QString(),
                 const QDate birthDay = QDate(0,0,0),
                 const QString major = QString(),
                 const QString tutorId = QString());

    virtual ~Postgraduate();

    friend QDataStream &operator <<(QDataStream &out, const Postgraduate &pg);
    friend QDataStream &operator >>(QDataStream &in, Postgraduate &pg);

    virtual QString toString() override;

 protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;

signals:

public slots:
    QString getMajor() const;
    bool setMajor(QString major);

    QString getTutorId() const;
    bool setTutorId(QString id);

protected:
    QString major;
    QString tutorId;
};


inline QString Postgraduate::getMajor() const{
    return this->major;
}

inline bool Postgraduate::setMajor(QString major){
    if(major.length() > POSTGRADUATE_MAJOR_LEN){
        return false;
    }
    this->major = major;
    return true;
}

inline QString Postgraduate::getTutorId() const{
    return this->tutorId;
}

inline bool Postgraduate::setTutorId(QString id){
    if(id.length() > PERSON_ID_LEN){
        return false;
    }
    this->tutorId = id;
    return true;
}
