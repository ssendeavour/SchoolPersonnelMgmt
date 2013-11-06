#pragma once

#include <QObject>
#include "teacher.h"
#include "postgraduate.h"

class TeachingAssistant : public Teacher, public Postgraduate
{
    Q_OBJECT
public:
    TeachingAssistant(const QString classNo = QString(),
                      const QString name = QString(),
                      const QString id = QString(),
                      const QString idNum = QString(),
                      const QString sex = QString(),
                      const QDate birthDay = QDate(0,0,0),
                      const QString major = QString(),
                      const QString tutorId = QString(),
                      const QString dept = QString(),
                      const QString position = QString());

    virtual ~TeachingAssistant();

    friend QDataStream &operator <<(QDataStream &out, const TeachingAssistant &ta);
    friend QDataStream &operator >>(QDataStream &in, TeachingAssistant &ta);

    virtual QString toString() override;

 protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;
};

