#ifndef SMARTENTITY_H
#define SMARTENTITY_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>
#include <QString>
#include <QMap>
#include <QDebug>

//SmartEntity class;
//Please inherit this class to produce your smart classes;
class SmartEntity : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Damian Cholewa")
    Q_CLASSINFO("Email", "damian.cholewa.choli@gmail.com")

public:
    Q_INVOKABLE SmartEntity(QObject *parent=nullptr);
    //Returns class representation as QJsonObject;
    QJsonObject toJsonObject() const;

public slots:
    //Fills all members of class by values from QJsonObject;
    void fillObject(const QJsonObject &object);
};

//////////////////////////////////////////////////////////////////////////////////////
//Type to handle function pointer for QList<T> creators;
typedef QVariant (*listCreator)(const QString &className, const QJsonArray &array);
//Type to handle function pointer for QJsonArray<T> creators;
typedef QJsonArray (*jsonArrayCreator)(const QVariant &variant);

//Returns type between brackets;
//For example QList<int> -> 'int', QList< QList<int> > -> 'QList<int>';
//Returns given argument if brackets are not found in string;
static inline QString getTypeName(const char *listType) {
    QString listTypeStr(listType);
    QRegExp regExp("\\<(\\S+)\\>$");
    if (regExp.indexIn(listTypeStr) != -1) {
        return regExp.cap(1);
    } else {
        return listTypeStr;
    }
}

//Creates funcion with given name of class;
//Class must be registered in QMetaTypes;
//Class must have Q_INVOKABLE constructor;
//Class must be based on SmartEntity;
static inline SmartEntity* createSmartClass(const QByteArray &className) {
    int id = QMetaType::type(className);
    if (!QMetaType::isRegistered(id)) {
        return nullptr;
    }
    const QMetaObject *metaObj = QMetaType::metaObjectForType(id);
    return qobject_cast<SmartEntity*>(metaObj->newInstance());
}

//QList creator;
//Returns QList of type T filled by values from array;
//Please note QList<T> is converted to QVariant;
template<typename T>
QVariant createList(const QString &classType, const QJsonArray &array) {
    Q_UNUSED(classType)
    QList<T> list;
    foreach (const QJsonValue &value, array) {
        list.append(qvariant_cast<T>(value));
    }
    return QVariant::fromValue(list);
}

//QList creator;
//Returns QList of type T, T is class.
//List is filled by values from array;
//Please note QList<T> is converted to QVariant;
template<typename T>
QVariant createObjectList(const QString &classType, const QJsonArray &array) {
    QList<T> list;
    foreach (const QJsonValue &value, array) {
        T obj = qobject_cast<T>(createSmartClass(classType.toUtf8()));
        if (obj == nullptr) {
            return QVariant::fromValue(list);
        }
        obj->fillObject(value.toObject());
        list.append(obj);
    }
    return QVariant::fromValue(list);
}

//Creates QJsonVariant from QList<T> (QVariant);
//In this case T is 'primitive' type like: bool, double, QString;
template<typename T>
QJsonArray createJsonArray(const QVariant &variant) {
    return QJsonArray::fromVariantList(variant.value<QVariantList>());
}

//Creates QJsonVariant from QList<T> (QVariant);
//In this case T is class based on SmartEntity;
template<typename T>
QJsonArray createObjectJsonArray(const QVariant &variant) {
    const QList<T> list = qvariant_cast<QList<T> >(variant);
    QJsonArray array;
    foreach (const T entity, list) {
        if (entity == nullptr) {
            continue;
        }
        QJsonObject jsonObj = entity->toJsonObject();
        array.append(jsonObj);
    }
    return array;
}

//Helper class to handle all creators/fillers;
//SmartGlobalMap is singleton;
class SmartGlobalMap
{
public:
    //Creates instance of SmartGlobalMap if not exists;
    //Also registers all standard types handled by QJson;
    static SmartGlobalMap *instance() {
        if (!m_instance) {
            m_instance = new SmartGlobalMap();
            //Handle all bool types;
            m_instance->insertListCreator<bool>("bool");
            m_instance->insertJsonArrayCreator<bool>("bool");
            //Handle all int types;
            m_instance->insertListCreator<int>("int");
            m_instance->insertJsonArrayCreator<int>("int");
            //Handle all double types;
            m_instance->insertListCreator<double>("double");
            m_instance->insertJsonArrayCreator<double>("double");
            //Handle all QString types;
            m_instance->insertListCreator<QString>("QString");
            m_instance->insertListCreator<QString>("QStringList");
            m_instance->insertJsonArrayCreator<QString>("QString");
            m_instance->insertJsonArrayCreator<QString>("QStringList");
        }
        return m_instance;
    }

    //Returns QList<T> creator;
    //For example if type is equal to 'int'
    //then returned creator will handle QList<int>;
    listCreator returnListCreator(const QString &type) {
        if (!m_listCreatorMap.contains(type))
            qCritical() << "Warning! Unsupported type: " << type;
        return m_listCreatorMap.value(type, nullptr);
    }

    //Returns QJsonArray creator;
    //For example if type is eual to 'int'
    //then returned creator will handle conversion QList<int> to QJsonArray;
    jsonArrayCreator returnJsonArrayCreator(const QString &type) {
        if (!m_jsonArrayCreatorMap.contains(type))
            qCritical() << "Warning! Unsupported type: " << type;
        return m_jsonArrayCreatorMap.value(type, nullptr);
    }

    //Inserts new listCreator with given type;
    template<typename T>
    void insertListCreator(const QString &type) {
        m_listCreatorMap.insert(type, createList<T>);
    }

    //Inserts new listCreator with given type;
    //In this case listCreator will handle object types;
    template<typename T>
    void insertObjectListCreator(const QString &type) {
        m_listCreatorMap.insert(type, createObjectList<T>);
    }

    //Inserts new arrayCreator with given type;
    template<typename T>
    void insertJsonArrayCreator(const QString &type) {
        m_jsonArrayCreatorMap.insert(type, createJsonArray<T>);
    }

    //Inserts new arrayCreator with given type;
    //In this case arrayCreator will handle object types;
    template<typename T>
    void insertObjectJsonArrayCreator(const QString &type) {
        m_jsonArrayCreatorMap.insert(type, createObjectJsonArray<T>);
    }

public:
    SmartGlobalMap(){}
    static SmartGlobalMap *m_instance;
    QMap<QString, listCreator> m_listCreatorMap;
    QMap<QString, jsonArrayCreator> m_jsonArrayCreatorMap;
};

//Registers new class types based on SmartEntity;
#define registerSmartClass(t) { \
    qRegisterMetaType<t>(#t); \
    SmartGlobalMap::instance()->insertObjectListCreator<t>(#t); \
    SmartGlobalMap::instance()->insertObjectJsonArrayCreator<t>(#t); \
}

#endif // SMARTENTITY_H
