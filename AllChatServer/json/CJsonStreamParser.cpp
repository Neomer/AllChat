#include "CJsonStreamParser.h"

CJsonStreamParser::CJsonStreamParser(QIODevice *device, QObject *parent) : QObject(parent)
{
	connect(device, SIGNAL(readyRead()),
			this, SLOT(readData()));
}

void CJsonStreamParser::readData()
{

}

