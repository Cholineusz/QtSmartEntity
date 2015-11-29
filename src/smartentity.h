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
    QJsonObject toJsonObject() const;

public slots:
    void fillObject(const QJsonObject &object);
};

//////////////////////////////////////////////////////////////////////////////////////

typedef QVariant (*listCreator)(const QString &className, const QJsonArray &array);
typedef QJsonArray (*jsonArrayCreator)(const QVariant &variant);

static inline QString getTypeName(const char *listType) {
    QString listTypeStr(listType);
    QRegExp regExp("\\<(\\S+)\\>$");
    if (regExp.indexIn(listTypeStr) != -1) {
        return regExp.cap(1);
    } else {
        return listTypeStr;
    }
}

static inline SmartEntity* createSmartClass(const QByteArray &className) {
    int id = QMetaType::type(className);
    if (!QMetaType::isRegistered(id)) {
        return nullptr;
    }
    const QMetaObject *metaObj = QMetaType::metaObjectForType(id);
    return qobject_cast<SmartEntity*>(metaObj->newInstance());
}

template<typename T>
QVariant createList(const QString &classType, const QJsonArray &array) {
    Q_UNUSED(classType)
    QList<T> list;
    foreach (const QJsonValue &value, array) {
        list.append(qvariant_cast<T>(value));
    }
    return QVariant::fromValue(list);
}

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

template<typename T>
QJsonArray createJsonArray(const QVariant &variant) {
    return QJsonArray::fromVariantList(variant.value<QVariantList>());
}

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

class SmartGlobalMap
{
public:
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

    listCreator returnListCreator(const QString &type) {
        if (!m_listCreatorMap.contains(type))
            qCritical() << "Warning! Unsupported type: " << type;
        return m_listCreatorMap.value(type, nullptr);
    }

    jsonArrayCreator returnJsonArrayCreator(const QString &type) {
        if (!m_jsonArrayCreatorMap.contains(type))
            qCritical() << "Warning! Unsupported type: " << type;
        return m_jsonArrayCreatorMap.value(type, nullptr);
    }

    template<typename T>
    void insertListCreator(const QString &type) {
        m_listCreatorMap.insert(type, createList<T>);
    }

    template<typename T>
    void insertObjectListCreator(const QString &type) {
        m_listCreatorMap.insert(type, createObjectList<T>);
    }

    template<typename T>
    void insertJsonArrayCreator(const QString &type) {
        m_jsonArrayCreatorMap.insert(type, createJsonArray<T>);
    }

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

#define registerSmartClass(t) { \
    qRegisterMetaType<t>(#t); \
    SmartGlobalMap::instance()->insertObjectListCreator<t>(#t); \
    SmartGlobalMap::instance()->insertObjectJsonArrayCreator<t>(#t); \
}

#endif // SMARTENTITY_H
