#ifndef TESTINT
#define TESTINT

#include "smartentity.h"

class TestInt : public SmartEntity
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QList<int> ids READ ids WRITE setIds NOTIFY idsChanged)

private:
    int m_id;
    QList<int> m_ids;

public:
    Q_INVOKABLE TestInt() :
        SmartEntity(nullptr)
    {
    }
    int id() const
    {
        return m_id;
    }
    QList<int> ids() const
    {
        return m_ids;
    }

public slots:
    void setId(int id)
    {
        if (m_id == id)
            return;

        m_id = id;
        emit idChanged(id);
    }
    void setIds(QList<int> ids)
    {
        if (m_ids == ids)
            return;

        m_ids = ids;
        emit idsChanged(ids);
    }

signals:
    void idChanged(int id);
    void idsChanged(QList<int> ids);
};


#endif // TESTINT

