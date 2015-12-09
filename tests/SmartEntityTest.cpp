#include <QString>
#include <QtTest>
#include "smartentity.h"

//TestObjects
#include "testobject.h"

//TestSubClass
#include "testsubclass.h"

class SmartEntityTest : public QObject
{
    Q_OBJECT

public:
    SmartEntityTest();

private Q_SLOTS:
    void testInt();
    void testString();
    void testObject();
    void testSubClass();
};

SmartEntityTest::SmartEntityTest()
{
}

void SmartEntityTest::testInt()
{
    TestInt *intObj = new TestInt;
    intObj->setId(10);
    intObj->setIds(QList<int>{1,2,3,4,5});

    TestInt *intFillObj = new TestInt;
    intFillObj->fillObject(intObj->toJsonObject());

    QCOMPARE(intFillObj->id(), intObj->id());
    QCOMPARE(intFillObj->ids(), intObj->ids());

    intObj->deleteLater();
    intFillObj->deleteLater();
}

void SmartEntityTest::testString()
{
    TestString *strObj = new TestString;
    strObj->setName("Test");
    strObj->setNameList(QList<QString>{"test1","test2","test3"});
    strObj->setNames(QStringList() << "test1" << "test2" << "test3");

    TestString *strFillObj = new TestString;
    strFillObj->fillObject(strObj->toJsonObject());

    QCOMPARE(strObj->name(), strFillObj->name());
    QCOMPARE(strObj->nameList(), strFillObj->nameList());
    QCOMPARE(strObj->names(), strFillObj->names());

    strObj->deleteLater();
    strFillObj->deleteLater();
}

void SmartEntityTest::testObject()
{
    //Handle all TestInt* types;
    registerSmartClass(TestInt*);
    //Handle all TestString* types;
    registerSmartClass(TestString*);
    //Handle all TestObject* types;
    registerSmartClass(TestObject*);


    TestInt *intObj = new TestInt;
    intObj->setId(10);
    intObj->setIds(QList<int>{1,2,3,4,5});

    TestString *strObj = new TestString;
    strObj->setName("Test");
    strObj->setNameList(QList<QString>{"test1","test2","test3"});
    strObj->setNames(QStringList() << "test1" << "test2" << "test3");

    TestObject *simpleObj = new TestObject;
    simpleObj->setIntObj(intObj);
    simpleObj->setStrObj(strObj);

    TestObject *obj = new TestObject;
    obj->setObject(simpleObj);
    obj->setObjects(QList<TestObject*>{new TestObject, new TestObject, simpleObj});
    obj->setIntObj(intObj);
    obj->setIntObjs(QList<TestInt*>{new TestInt, new TestInt, intObj});
    obj->setStrObj(strObj);
    obj->setStrObjs(QList<TestString*>{new TestString, new TestString, strObj});

    TestObject *fillObj = new TestObject;

    QBENCHMARK {
        fillObj->fillObject(obj->toJsonObject());
    }

    QJsonObject jsonObj = obj->toJsonObject();
    QJsonObject jsonFillObj = fillObj->toJsonObject();

    QCOMPARE(jsonObj, jsonFillObj);

    qDeleteAll(obj->objects());
    qDeleteAll(obj->intObjs());
    qDeleteAll(obj->strObjs());

    qDeleteAll(fillObj->objects());
    qDeleteAll(fillObj->intObjs());
    qDeleteAll(fillObj->strObjs());

    obj->deleteLater();
    fillObj->deleteLater();
}

void SmartEntityTest::testSubClass()
{
    TestSubClass *obj = new TestSubClass;
    obj->setId(2);
    obj->setIdSubClass(-2);

    TestSubClass *emptyObj = new TestSubClass;
    emptyObj->fillObject(obj->toJsonObject());

    QCOMPARE(emptyObj->toJsonObject(), obj->toJsonObject());

    obj->deleteLater();
    emptyObj->deleteLater();
}


QTEST_APPLESS_MAIN(SmartEntityTest)

#include "SmartEntityTest.moc"
