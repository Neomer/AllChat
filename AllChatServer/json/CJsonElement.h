#ifndef CJSONBELEMENT_H
#define CJSONBELEMENT_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QVector>
#include "defines.h"
#include "CJsonElementValue.h"

class CJsonDoc;

class CJsonElement : public QObject
{
	Q_OBJECT

	friend class CJsonDoc;
	friend class CJson;

public:
	
	CJsonElement(QString key, CJsonElementValue value, QObject *parent = 0);
	CJsonElement(QObject *parent = 0);
	CJsonElement(const CJsonElement &other);

	void operator = (const CJsonElement &other);


	bool isValid()  { return ((!key().isEmpty())||(value().type() == CJsonElementValue::JsonTypeRoot)) && (__valid); }
	bool isNull()   { return ((key().isEmpty())&&(value().type() != CJsonElementValue::JsonTypeRoot)) || (!__valid); }
	bool isLast()   { return __next == 0; }

	QString key() { return __key; }
	CJsonElementValue value() { return __value; }

	const CJsonElement * next() { return __next; }

signals:

public slots:

private:
	QString __key;
	CJsonElementValue __value;
	bool __valid;
	CJsonElement * __next;
};


Q_DECLARE_METATYPE(CJsonElement)

#endif // CJSONBELEMENT_H
