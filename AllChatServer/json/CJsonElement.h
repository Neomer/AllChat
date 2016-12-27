#ifndef CJSONBELEMENT_H
#define CJSONBELEMENT_H

#include <QObject>
#include <QVariant>
#include <QMap>

class CJsonElement : public QObject
{
	Q_OBJECT
public:
	enum JsonElementType
	{
		JsonTypeString,
		JsonTypeArray,
		JsonTypeNumber,
		JsonTypeObject
	};
	
	CJsonElement(QString key, QVariant value, JsonElementType type, QObject *parent = 0);
	CJsonElement(QObject *parent = 0);
	CJsonElement(const CJsonElement &other);

	bool isText()   { return type() == JsonTypeString; }
	bool isArray()  { return type() == JsonTypeArray;  }
	bool isNumber() { return type() == JsonTypeNumber; }
	bool isObject() { return type() == JsonTypeObject; }

	bool isValid()  { return (!key().isEmpty()) && (__valid); }

	QString key() { return __key; }
	QVariant value() { return __value; }
	JsonElementType type() { return __type; }

	const CJsonElement &operator [](QString key);

signals:

public slots:

private:
	QString __key;
	QVariant __value;
	JsonElementType __type;
	bool __valid;
};

#endif // CJSONBELEMENT_H
