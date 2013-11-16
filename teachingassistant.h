#pragma once

#include <QObject>
#include "teacher.h"
#include "postgraduate.h"

class TeachingAssistant : public Teacher, public Postgraduate
{
public:
    TeachingAssistant(const QString classNo = QString(),
                      const QString name_ = QString(),
                      const QString id_ = QString(),
                      const QString idNum_ = QString("000000000000000000"),
                      const Sex sex_ = Sex::Unspecified,
                      const QDate birthDay_ = QDate(1880,1,1),
                      const QString major = QString(),
                      const QString tutorId_ = QString(),
                      const QString dept_ = QString(),
                      const QString position_ = QString());

    virtual ~TeachingAssistant();

    friend QDataStream &operator <<(QDataStream &out, const TeachingAssistant &ta);
    friend QDataStream &operator >>(QDataStream &in, TeachingAssistant &ta);

    virtual QString toString() const override;

 protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;
};

