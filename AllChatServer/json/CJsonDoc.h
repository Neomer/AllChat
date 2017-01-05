#ifndef CJSONDOC_H
#define CJSONDOC_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "defines.h"
#include "CJsonElement.h"

class CJson;

class CJsonDoc : public QObject
{
	Q_OBJECT

	friend class CJson;

public:
	CJsonDoc(QObject *parent = 0);
	CJsonDoc(const CJsonDoc &other);
	
	const CJsonElement * root() { return __root; }

	void appendElement(CJsonElement *element);

	QList<CJsonElement> findElementsByKey(QString key, bool recursively = true);
	CJsonElement findFirstElementByKey(QString key, bool recursively = true);

	bool isEmpty() { return root() == 0; }

	CJsonElement operator [] (QString key) { return findFirstElementByKey(key); }

private:
	QList<CJsonElement> __elements;
	CJsonElement * __root;

};

Q_DECLARE_METATYPE(CJsonDoc)

#endif // CJSONDOC_H
