#ifndef TESTSUBCLASS
#define TESTSUBCLASS

#include "testint.h"

class TestSubClass : public TestInt
{
    Q_OBJECT
    Q_PROPERTY(int idSubClass READ idSubClass WRITE setIdSubClass NOTIFY idSubClassChanged)

    int m_idSubClass;

public:
    Q_INVOKABLE TestSubClass() :
        TestInt()
    {
    }

    int idSubClass() const
    {
        return m_idSubClass;
    }
public slots:
    void setIdSubClass(int idSubClass)
    {
        if (m_idSubClass == idSubClass)
            return;

        m_idSubClass = idSubClass;
        emit idSubClassChanged(idSubClass);
    }
signals:
    void idSubClassChanged(int idSubClass);
};

#endif // TESTSUBCLASS

