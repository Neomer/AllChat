#ifndef CJSONBELEMENT_H
#define CJSONBELEMENT_H

#include <QObject>
#include <QMap>

class CJSonElement : public QObject
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
	
	explicit CJSonElement(QObject *parent = 0);

	bool isValue() { return !isBranch(); }
	bool isBranch();

	const CJSonElement &operator [](QString key);

signals:

public slots:

};

#endif // CJSONBELEMENT_H
