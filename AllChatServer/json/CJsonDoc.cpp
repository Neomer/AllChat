#include "CJsonDoc.h"

CJsonDoc::CJsonDoc(QObject *parent) :
	QObject(parent),
	__root(0)
{
	
}

CJsonDoc::CJsonDoc(const CJsonDoc &other)
{
	__elements = other.__elements;
	__root = other.__root;
}

void CJsonDoc::appendElement(CJsonElement *element)
{
	__elements.append(*element);
}

QList<CJsonElement> CJsonDoc::findElementsByKey(QString key, bool recursively)
{
}

CJsonElement CJsonDoc::findFirstElementByKey(QString key, bool recursively)
{
}

