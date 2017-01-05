#include "CJsonElement.h"

CJsonElement::CJsonElement(QString key,
						   CJsonElementValue value,
						   QObject *parent) :
	QObject(parent),
	__key(key),
	__valid(true),
	__next(0),
	__value(value)
{
}

CJsonElement::CJsonElement(QObject *parent) :
	QObject(parent)
{
	__valid = false;
}

CJsonElement::CJsonElement(const CJsonElement &other)
{
	__key = other.__key;
	__value = other.__value;
	__valid = other.__valid;
}

void CJsonElement::operator =(const CJsonElement &other)
{
	__key = other.__key;
	__value = other.__value;
	__valid = other.__valid;
}
