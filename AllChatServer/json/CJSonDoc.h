#ifndef CJSONDOC_H
#define CJSONDOC_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "CJSonElement.h"

class CJson;

class CJSonDoc : public QObject
{
	Q_OBJECT

	friend class CJson;

public:
	CJSonDoc(QObject *parent = 0);
	CJSonDoc(const CJSonDoc &other);
	
	const CJSonElement& operator [] (QString key);
	const CJSonElement& first() { return __first; }

signals:

public slots:

private:
	CJSonElement __first;

};

#endif // CJSONDOC_H
