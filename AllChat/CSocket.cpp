#include "CSocket.h"

CSocket::CSocket(CChatProtocol *processor, QObject *parent) :
	QTcpSocket(parent)
{
	connect(this, SIGNAL(readyRead()),
			this, SLOT(readData()));

	__processor = processor;
}

void CSocket::readData()
{
	__processor->appendData(this->readAll());
}
