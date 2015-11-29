#ifndef TESTSTRING
#define TESTSTRING

#include "smartentity.h"

class TestString : public SmartEntity
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QList<QString> nameList READ nameList WRITE setNameList NOTIFY nameListChanged)
    Q_PROPERTY(QStringList names READ names WRITE setNames NOTIFY namesChanged)

private:
    QString m_name;
    QList<QString> m_nameList;
    QStringList m_names;

public:
    Q_INVOKABLE TestString() :
        SmartEntity(nullptr)
    {
    }
    QString name() const
    {
        return m_name;
    }
    QList<QString> nameList() const
    {
        return m_nameList;
    }

    QStringList names() const
    {
        return m_names;
    }

public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(name);
    }
    void setNameList(QList<QString> nameList)
    {
        if (m_nameList == nameList)
            return;

        m_nameList = nameList;
        emit nameListChanged(nameList);
    }

    void setNames(QStringList names)
    {
        if (m_names == names)
            return;

        m_names = names;
        emit namesChanged(names);
    }

signals:
    void nameChanged(QString name);
    void nameListChanged(QList<QString> nameList);
    void namesChanged(QStringList names);
};
#endif // TESTSTRING

