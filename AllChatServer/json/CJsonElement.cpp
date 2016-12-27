#include "CJsonElement.h"

CJsonElement::CJsonElement(QString key,
						   QVariant value,
						   JsonElementType type,
						   QObject *parent) :
	QObject(parent),
	__key(key),
	__type(type),
	__valid(true)
{
	QStringList sl;

	switch (type())
	{
		case JsonTypeArray:
			sl = value.toString().split(',', QString::SkipEmptyParts);

			QVector<QString> tmp;

			foreach (QString pp, sl)
			{
				pp.trimmed();
				tmp.append(pp);
			}

			break;

		case JsonTypeString:
			break;

		case JsonTypeBool:
			break;

		case JsonTypeObject:
			__value = value;
			break;

		case JsonTypeNumber:
			break;
	}
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

void CJsonElement::operator =(const CJsonElement &other)
{
	__key = other.__key;
	__type = other.__type;
	__value = other.__value;
	__valid = other.__valid;
}
