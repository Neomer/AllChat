#include "CJsonElement.h"

CJsonElement::CJsonElement(QString key,
						   QVariant value,
						   JsonElementType type,
						   QObject *parent) :
	QObject(parent)
{

}

const CJsonElement &CJsonElement::operator [](QString key)
{

}
