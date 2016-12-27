#include "CJsonElement.h"

CJsonElement::CJsonElement(QString key,
						   QVariant value,
						   JsonElementType type,
						   QObject *parent) :
	QObject(parent),
	__key(key),
	__value(value),
	__type(type),
	__valid(true)
{
}

CJsonElement::CJsonElement(QObject *parent)
{
	__valid = false;
}

CJsonElement::CJsonElement(const CJsonElement &other)
{
	__key = other.__key;
	__type = other.__type;
	__value = other.__value;
	__valid = other.__valid;
}

const CJsonElement &CJsonElement::operator [](QString key)
{

}
