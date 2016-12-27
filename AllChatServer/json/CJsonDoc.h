#ifndef CJSONDOC_H
#define CJSONDOC_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "CJsonElement.h"

class CJson;

class CJsonDoc : public QObject
{
	Q_OBJECT

	friend class CJson;

public:
	CJsonDoc(QObject *parent = 0);
	CJsonDoc(const CJsonDoc &other);
	
	const CJsonElement& operator [] (QString key);
	const CJsonElement& first() { return *(__elements.at(0)); }

	void appendElement(CJsonElement *element);

	CJsonElement * findElementByKey(QString key, bool recursively = true);

	bool isEmpty() { return __elements.isEmpty(); }

signals:

public slots:

private:
	QList<CJsonElement *> __elements;

};

Q_DECLARE_METATYPE(CJsonDoc)

#endif // CJSONDOC_H
