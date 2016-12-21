#ifndef CCHATSERVER_H
#define CCHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QSqlDatabase>

#include "CErrorProcessor.h"
#include "CChatClient.h"
#include "CChatRoom.h"
#include "CDatabase.h"
#include "CRoomCollection.h"

class CChatServer : public QTcpServer
{
	Q_OBJECT

	public:
		CChatServer(CDatabase * db);

		void start(quint16 port);
		void stop();

	private slots:
		void connectionEstablished();
		void connectionRefused(QAbstractSocket::SocketError error);
		void connectionClosed(QTcpSocket* socket);

		void echoClientMessage(CChatClient * client, SChatProtoMessage message, quint32 id);
		void createRoom(CChatClient * client, SChatProtoRoomCreate message, quint32 id);
		void clientJoinRoom(CChatClient * client, SChatProtoRoomIn message, quint32 id);
		void clientFindRoom(CChatClient * client, SChatProtoRoomFind message, quint32 id);

	private:
		QTcpServer * __socket;
		QList<CChatClient *> __clients;
		CRoomCollection * __rooms;
		CDatabase * __db;
};

#endif // CCHATSERVER_H
