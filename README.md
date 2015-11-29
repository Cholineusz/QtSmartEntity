# QtSmartEntity
## About:
SmartEntity will help you with handling JSON in your projects.
You can speed up your work by simply creation of class based on SmartEntity instead of manually writing a lot of setters and getters which manipulates JSON and your objects.
Invocation of methods like "toJson Object" and "fill Object" will do job for you!

## How to add:
1. Open and run project to check test results.
2. If test results are OK then copy folder 'SmartEntity' which includes smartentity.pri to your project directory.
3. Add magical line: 'include(\<path_to_smartentity.pri\>)' in your pro file.
4. Done!

## Example:
* Class creation:
```
class Doge : public SmartEntity
{
    Q_PROPERTY(QStringList muchStrings READ muchStrings WRITE setMuchStrings NOTIFY muchStringsChanged)
    Q_PROPERTY(QList<Doge*> manyDoggies READ manyDoggies WRITE setManyDoggies NOTIFY manyDoggiesChanged)

    Q_OBJECT
public:
    Q_INVOKABLE Doge(QObject *parent=nullptr);
    
    .. Generated Q_PROPERTY members ..;
};
```
* Register smart class:
```
registerSmartClass(Doge*);
```

* Smart class in action:
```
registerSmartClass(Doge*);

Doge *doge = new Doge;
doge->setMuchStrings(QStringList() << "WoW!" << "Nice!" << "Veri smart!");
doge->setManyDoggies(QList<Doge*>{new Doge(doge), new Doge(doge)} );
qDebug() << "Doge: " << doge->toJsonObject();

Doge *sadEmptyDoge = new Doge;
qDebug() << "[Before] Sad empty doge :( -> " << sadEmptyDoge->toJsonObject();

sadEmptyDoge->fillObject(doge->toJsonObject());
qDebug() << "[After] Sad empty doge :) -> " << sadEmptyDoge->toJsonObject();
```

* Output:
```
Doge:
{
	"manyDoggies":[{"manyDoggies":[],"muchStrings":[]},
				  {"manyDoggies":[],"muchStrings":[]}],
	"muchStrings":["WoW!","Nice!","Veri smart!"]
}

[Before] Sad empty doge :( -> 
{
	"manyDoggies":[],
	"muchStrings":[]
}

[After] Sad empty doge :) -> 
{
	"manyDoggies":[{"manyDoggies":[],"muchStrings":[]},
				   {"manyDoggies":[],"muchStrings":[]}],
	"muchStrings":["WoW!","Nice!","Veri smart!"]
}
```
