#ifndef TESTOBJECT
#define TESTOBJECT

#include "smartentity.h"

//Test objects;
#include "testint.h"
#include "teststring.h"

class TestObject : public SmartEntity
{
    Q_OBJECT
    Q_PROPERTY(TestObject* object READ object WRITE setObject NOTIFY objectChanged)
    Q_PROPERTY(TestInt* intObj READ intObj WRITE setIntObj NOTIFY intObjChanged)
    Q_PROPERTY(TestString* strObj READ strObj WRITE setStrObj NOTIFY strObjChanged)
    Q_PROPERTY(QList<TestObject*> objects READ objects WRITE setObjects NOTIFY objectsChanged)
    Q_PROPERTY(QList<TestInt*> intObjs READ intObjs WRITE setIntObjs NOTIFY intObjsChanged)
    Q_PROPERTY(QList<TestString*> strObjs READ strObjs WRITE setStrObjs NOTIFY strObjsChanged)

private:
    TestObject* m_object;
    TestInt* m_intObj;
    TestString* m_strObj;
    QList<TestObject*> m_objects;
    QList<TestInt*> m_intObjs;
    QList<TestString*> m_strObjs;

public:
    Q_INVOKABLE TestObject() :
        SmartEntity(nullptr),
        m_object(nullptr),
        m_intObj(nullptr),
        m_strObj(nullptr)
    {
    }
    TestObject* object() const
    {
        return m_object;
    }
    TestInt* intObj() const
    {
        return m_intObj;
    }

    TestString* strObj() const
    {
        return m_strObj;
    }

    QList<TestObject*> objects() const
    {
        return m_objects;
    }

    QList<TestInt*> intObjs() const
    {
        return m_intObjs;
    }

    QList<TestString*> strObjs() const
    {
        return m_strObjs;
    }

public slots:
    void setObject(TestObject* object)
    {
        if (m_object == object)
            return;

        m_object = object;
        emit objectChanged(object);
    }
    void setIntObj(TestInt* intObj)
    {
        if (m_intObj == intObj)
            return;

        m_intObj = intObj;
        emit intObjChanged(intObj);
    }

    void setStrObj(TestString* strObj)
    {
        if (m_strObj == strObj)
            return;

        m_strObj = strObj;
        emit strObjChanged(strObj);
    }

    void setObjects(QList<TestObject*> objects)
    {
        if (m_objects == objects)
            return;

        m_objects = objects;
        emit objectsChanged(objects);
    }

    void setIntObjs(QList<TestInt*> intObjs)
    {
        if (m_intObjs == intObjs)
            return;

        m_intObjs = intObjs;
        emit intObjsChanged(intObjs);
    }

    void setStrObjs(QList<TestString*> strObjs)
    {
        if (m_strObjs == strObjs)
            return;

        m_strObjs = strObjs;
        emit strObjsChanged(strObjs);
    }

signals:
    void objectChanged(TestObject* object);
    void intObjChanged(TestInt* intObj);
    void strObjChanged(TestString* strObj);
    void objectsChanged(QList<TestObject*> objects);
    void intObjsChanged(QList<TestInt*> intObjs);
    void strObjsChanged(QList<TestString*> strObjs);
};

#endif // TESTOBJECT

