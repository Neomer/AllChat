#ifndef CJSONELEMENTVALUE_H
#define CJSONELEMENTVALUE_H

#include <QObject>
#include <QVariant>

class CJsonElementValue : public QObject
{
	Q_OBJECT
public:
	enum JsonValueType
	{
		JsonTypeRoot,
		JsonTypeString,
		JsonTypeArray,
		JsonTypeNumber,
		JsonTypeBool,
		JsonTypeObject
	};

	explicit CJsonElementValue(QObject *parent = 0);
	explicit CJsonElementValue(QVariant value, JsonValueType type, QObject *parent = 0);
	explicit CJsonElementValue(QString value, JsonValueType type, QObject *parent = 0);
	CJsonElementValue(const CJsonElementValue &other);
	void operator = (const CJsonElementValue &other);

	bool isText()   { return type() == JsonTypeString; }
	bool isArray()  { return type() == JsonTypeArray;  }
	bool isNumber() { return type() == JsonTypeNumber; }
	bool isObject() { return type() == JsonTypeObject; }
	bool isBool()   { return type() == JsonTypeBool;   }

	JsonValueType type() { return __type; }

	QString toString()	{ return __value.toString(); }
	int toInt()			{ return __value.toInt(); }
	bool toBool()		{ return __value.toBool(); }
	const char * toConstChar() { return __value.toString().toLatin1().constData(); }

signals:

public slots:

private:
	CJsonElementValue::JsonValueType __type;
	QVariant __value;
};

Q_DECLARE_METATYPE(CJsonElementValue)

#endif // CJSONELEMENTVALUE_H
