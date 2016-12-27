#include "CJsonDoc.h"

CJsonDoc::CJsonDoc(QObject *parent) : QObject(parent)
{
	
}

CJsonDoc::CJsonDoc(const CJsonDoc &other)
{
	__elements = other.__elements;
}

void CJsonDoc::appendElement(CJsonElement *element)
{

	__elements.append(*element);
}

QList<CJsonElement> CJsonDoc::findElementsByKey(QString key, bool recursively)
{
	QList<CJsonElement> ret;

	foreach (CJsonElement element, this->__elements)
	{
		if (element.key() == key)
		{
			ret.append(element);
		}
		if ((element.isObject()) && (recursively))
		{
			ret.append(element.value().value<CJsonDoc>().findElementsByKey(key, recursively));
		}
	}

	return ret;
}

CJsonElement CJsonDoc::findFirstElementByKey(QString key, bool recursively)
{
	foreach (CJsonElement element, this->__elements)
	{
		if (element.key() == key)
		{
			return element;
		}
		if ((element.isObject()) && (recursively))
		{
			CJsonElement ret = element.value().value<CJsonDoc>().findFirstElementByKey(key, recursively);

			if (ret.isValid()) return ret;
		}
	}

	return CJsonElement();
}

