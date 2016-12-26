#include "CJsonDoc.h"

CJsonDoc::CJsonDoc(QObject *parent) : QObject(parent)
{
	
}

CJsonDoc::CJsonDoc(const CJsonDoc &other)
{
	
}

void CJsonDoc::appendElement(CJsonElement *element)
{
	__elements.append(element);
}

