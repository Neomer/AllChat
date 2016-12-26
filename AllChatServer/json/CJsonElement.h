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
	
	explicit CJsonElement(QString key, QVariant value, JsonElementType type, QObject *parent = 0);

	bool isValue() { return !isBranch(); }
	bool isBranch();

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
};

#endif // CJSONBELEMENT_H
