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
	
	CJsonElement first() { return __elements.at(0); }

	void appendElement(CJsonElement *element);

	QList<CJsonElement> findElementsByKey(QString key, bool recursively = true);
	CJsonElement findFirstElementByKey(QString key, bool recursively = true);

	bool isEmpty() { return __elements.isEmpty(); }

	CJsonElement operator [] (QString key) { return findFirstElementByKey(key); }

signals:

public slots:

private:
	QList<CJsonElement> __elements;

};

Q_DECLARE_METATYPE(CJsonDoc)

#endif // CJSONDOC_H
