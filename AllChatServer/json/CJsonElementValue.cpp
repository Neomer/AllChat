#include "CJsonElementValue.h"

CJsonElementValue::CJsonElementValue(QObject *parent) : QObject(parent)
{

}

CJsonElementValue::CJsonElementValue(QVariant value, CJsonElementValue::JsonValueType type, QObject *parent) :
	QObject(parent),
	__value(value),
	__type(type)
{

}

CJsonElementValue::CJsonElementValue(QString value, CJsonElementValue::JsonValueType type, QObject *parent) :
	QObject(parent),
	__type(type)
{
	Q_UNUSED(value);
}

CJsonElementValue::CJsonElementValue(const CJsonElementValue &other)
{
	__type = other.__type;
	__value = other.__value;
}

void CJsonElementValue::operator =(const CJsonElementValue &other)
{
	__type = other.__type;
	__value = other.__value;
}
