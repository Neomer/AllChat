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
	__elements.append(element);
}

CJsonElement *CJsonDoc::findElementByKey(QString key, bool recursively)
{
	foreach (CJsonElement * element, this->__elements)
	{
		if (element->key() == key)
		{
			return element;
		}
		if ((element->isObject()) && (recursively))
		{
			CJsonElement * ret = element->value().value<CJsonDoc>().findElementByKey(key, recursively);

			if (ret) return ret;
		}
	}

	return 0;
}

