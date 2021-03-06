#pragma once
#include "student.h"
#include "teacher.h"

class Postgraduate : public Student
{
//    Q_OBJECT
public:
    static const int POSTGRADUATE_MAJOR_LEN = 20;

    Postgraduate(const QString classNo = QString(),
                 const QString name_ = QString(),
                 const QString id_ = QString(),
                 const QString idNum_ = QString("000000000000000000"),
                 const Sex sex_ = Sex::Unspecified,
                 const QDate birthDay_ = QDate(1880,1,1),
                 const QString major = QString(),
                 const QString tutorId_ = QString());

    virtual ~Postgraduate();

    friend QDataStream &operator <<(QDataStream &out, const Postgraduate &pg);
    friend QDataStream &operator >>(QDataStream &in, Postgraduate &pg);

    /* Parameter: file is an opend file, don't close it here.
     * currently, always return true */
   static bool writeToFile(QFile &file, const QList<Postgraduate> list);

    /* Parameter: file is an opend file, don't close it here */
   static QList<Postgraduate> readFromFile(QFile &file, QString &error);
    virtual QString toString() const override;

public:
    QString getMajor() const;
    bool setMajor(QString major);

    QString getTutorId() const;
    bool setTutorId(QString id_);

 protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;

protected:
    QString major_;
    QString tutorId_;
};


inline QString Postgraduate::getMajor() const{
    return this->major_;
}

inline bool Postgraduate::setMajor(QString major){
    if(major.length() > POSTGRADUATE_MAJOR_LEN){
        this->errorString_ = tr("major name %1 too long, current length: %2, max length: %3")
                .arg(major).arg(major.length()).arg(POSTGRADUATE_MAJOR_LEN);
        return false;
    }
    this->major_ = major;
    return true;
}

inline QString Postgraduate::getTutorId() const{
    return this->tutorId_;
}

inline bool Postgraduate::setTutorId(QString id){
    if(id.length() > PERSON_ID_LEN){
        this->errorString_ = tr("tutor's id %1 too long, current length %2, max length %3")
                .arg(tutorId_).arg(tutorId_.length()).arg(PERSON_ID_LEN);
        return false;
    }
    this->tutorId_ = id;
    return true;
}
