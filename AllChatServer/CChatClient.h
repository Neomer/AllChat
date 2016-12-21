#ifndef CCHATCLIENT_H
#define CCHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QSqlDatabase>

#include "CErrorProcessor.h"
#include "CChatUser.h"
#include "CDatabase.h"
#include "CChatProtocol.h"

class CChatRoom;
class CChatServer;

class CChatClient : public QObject
{
	Q_OBJECT

	friend class CChatServer;

	public:
		CChatClient(QTcpSocket * socket, CDatabase * db);
		void sendMessage(SChatProtoMessage message, quint32 id = -1);
		void sendRoomInfo(SChatProtoRoomInfo message, quint32 id = -1);

		quint32 userId();
		QString nickname();

	private slots:
		void connectionClosed();
		void dataReceive();
		void packageAuth(quint32 id, SChatProtoAuth package);
		void packageCreateRoom(quint32 id, SChatProtoRoomCreate package);
		void packageRoomIn(quint32 id, SChatProtoRoomIn package);
		void packageRoomFind(quint32 id, SChatProtoRoomFind package);
		void packageMessage(quint32 id, SChatProtoMessage package);

	signals:
		void closingConnection(QTcpSocket * socket);

		void userSendMessage(CChatClient * client, SChatProtoMessage message, quint32 id);
		void userJoinRoom(CChatClient * client, SChatProtoRoomIn message, quint32 id);
		void userCreateRoom(CChatClient * client, SChatProtoRoomCreate message, quint32 id);
		void userFindRoom(CChatClient * client, SChatProtoRoomFind message, quint32 id);

	private:
		QList<CChatRoom *> __rooms;
		QTcpSocket * __socket;
		CChatUser * __profile;
		CDatabase * __db;
		CChatProtocol * __processor;
		SChatProtoUserInfo __ui;
};

#endif // CCHATCLIENT_H
