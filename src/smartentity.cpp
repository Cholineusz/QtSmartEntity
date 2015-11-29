#include "smartentity.h"

//Qt-Meta:
#include <QMetaObject>
#include <QMetaProperty>
//Qt-Types:
#include <QVariant>
//Qt-Json:
#include <QJsonObject>
#include <QJsonArray>
//Qt-Debug:
#include <QDebug>

SmartGlobalMap *SmartGlobalMap::m_instance = nullptr;

SmartEntity::SmartEntity(QObject *parent) :
    QObject(parent)
{
}

QJsonObject SmartEntity::toJsonObject() const
{
    QJsonObject object;
    for (int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount() ; ++i) {

        QMetaProperty property = metaObject()->property(i);
        QString propertyName(property.name());

        QVariant var = property.read(this);

        QJsonValue val;
        if (var.canConvert<QVariantList>()) {
            QString listItemType = getTypeName(property.typeName());
            jsonArrayCreator filler = SmartGlobalMap::instance()->returnJsonArrayCreator(listItemType);
            if (filler != nullptr)
                val = filler(var);
            else
                continue;
        } else {
            SmartEntity *entity = nullptr;
            entity = qvariant_cast<SmartEntity*>(var);
            if (entity != nullptr) {
                val = QJsonValue(entity->toJsonObject());
            } else {
                val = QJsonValue::fromVariant(var);
            }
        }

        object.insert(propertyName, val);
    }
    return object;
}

void SmartEntity::fillObject(const QJsonObject &object)
{
    for (int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount() ; ++i) {
        QMetaProperty property = metaObject()->property(i);
        QString propertyName(property.name());

        if ( !property.isWritable() )
            return;

        QJsonValue val = object.value(propertyName);

        if (val.isUndefined()) {
            continue;
        }

        QVariant var;
        if (property.type() == QVariant::StringList) {
            var = val.toArray().toVariantList();
        } else if (val.type() == QJsonValue::Array) {
            QString listItemType = getTypeName(property.typeName());

            listCreator filler = SmartGlobalMap::instance()->returnListCreator(listItemType);
            if (filler != nullptr) {
                var = filler(listItemType, val.toArray());
            } else
                continue;
        } else if (val.type() == QJsonValue::Object){
            QByteArray type(property.typeName());
            SmartEntity *entity = createSmartClass(type);
            if (entity != nullptr) {
                entity->fillObject(val.toObject());
                var = QVariant::fromValue(entity);
            } else {
                var = QVariant();
            }

        } else {
            var = QVariant::fromValue(val);
        }

        property.write(this, var);
    }
}
