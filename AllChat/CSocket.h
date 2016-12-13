#ifndef CSOCKET_H
#define CSOCKET_H

#include <QObject>
#include <QTcpSocket>

#include "../AllChatServer/CChatProtocol.h"

class CSocket : public QTcpSocket
{
	Q_OBJECT

public:
	CSocket(CChatProtocol * processor, QObject * parent = 0);

private slots:
	void readData();

private:
	CChatProtocol * __processor;
};

#endif // CSOCKET_H
