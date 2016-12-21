#include "CChatServer.h"

CChatServer::CChatServer(CDatabase *db) :
	__db(db)
{
	this->__socket = new QTcpServer(this);
	__rooms = new CRoomCollection(__db, this);

	connect(this->__socket, SIGNAL(newConnection()),
			this, SLOT(connectionEstablished()));

	connect(this->__socket, SIGNAL(acceptError(QAbstractSocket::SocketError)),
			this, SLOT(connectionRefused(QAbstractSocket::SocketError)));

}

void CChatServer::start(quint16 port)
{
	if (!this->__socket->listen(QHostAddress::Any, port))
	{
        mCritical(tr("Port is busy! %1").arg(
                      this->__socket->errorString()));
	}

	mDebug(tr("Waiting for connection to port %1...").arg(QString::number(port)));
}

void CChatServer::stop()
{
	this->__socket->close();
	mDebug(tr("Port released"));

	__db->close();
}

void CChatServer::connectionEstablished()
{
	QTcpSocket * socket = this->__socket->nextPendingConnection();
	mDebug(tr("New connection. Remote host: %1").arg(socket->peerAddress().toString()));

	CChatClient * client = new CChatClient(socket, __db);
	this->__clients.append(client);

	connect(client, SIGNAL(userSendMessage(CChatClient*,SChatProtoMessage, quint32)),
			this, SLOT(echoClientMessage(CChatClient*,SChatProtoMessage, quint32)));

	connect(client, SIGNAL(userCreateRoom(CChatClient*,SChatProtoRoomCreate, quint32)),
			this, SLOT(createRoom(CChatClient*,SChatProtoRoomCreate, quint32)));

	connect(client, SIGNAL(userFindRoom(CChatClient*,SChatProtoRoomFind,quint32)),
			this, SLOT(clientFindRoom(CChatClient*,SChatProtoRoomFind,quint32)));

	connect(client, SIGNAL(userJoinRoom(CChatClient*,SChatProtoRoomIn,quint32)),
			this, SLOT(clientJoinRoom(CChatClient*,SChatProtoRoomIn,quint32)));

	connect(client, SIGNAL(closingConnection(QTcpSocket*)),
			this, SLOT(connectionClosed(QTcpSocket*)));
}

void CChatServer::connectionRefused(QAbstractSocket::SocketError error)
{
	mDebug(tr("Connection refused: %1").arg(QString::number(error)));
}

void CChatServer::connectionClosed(QTcpSocket *socket)
{
	CChatClient * client = qobject_cast<CChatClient *>(QObject::sender());

	if (!socket)
	{
		mWarning(tr("Signal sender null"));
		return;
	}

	foreach (CChatRoom * r, client->__rooms)
	{
		r->removeUser(client);
	}

	mDebug(tr("Connection closed. Client ID [%1]").arg(
			   QString::number(client->userId())));
}

void CChatServer::echoClientMessage(CChatClient *client, SChatProtoMessage message, quint32 id)
{
	mDebug(tr("Receive message\n\tRoom: %1\n\tWriter: [%2] %3\n\tMessage: %4").arg(
			   QString::number(message.room_id),
			   QString::number(message.writer_id),
			   message.writer_name,
			   message.message));

	CChatRoom * room = __rooms->roomById(message.room_id);

	if (!room)
	{
		mWarning(tr("User tries to send message in null room!"));
		return;
	}

	if (room->containsUser(client->userId()))
	{
		room->sendMessage(message, id);
	}
	else
	{
		mWarning(tr("User tries to send message in wrong room!"));
	}
}

void CChatServer::createRoom(CChatClient *client, SChatProtoRoomCreate message, quint32 id)
{
	CDatabaseResult * res = __db->execute(QString("insert into room (name, iconname, filter, owner) values ('%1', null, '%3', %2) returning id;").arg(
											  message.name,
											  QString::number(client->userId()),
											  message.filter));

	if (!res->isValid())
	{
		mDebug(tr("Result set isn't valid! %1").arg(
				   __db->lastError()));

		return;
	}


	mDebug(tr("User [%1] sucessfully create a room. ID: %2\nName: %3").arg(
			   QString::number(client->userId()),
			   res->value(0).toString(),
			   message.name));

	SChatProtoRoomInfo ri;

	ri.id = res->value(0).toString().toUInt();
	strcpy(ri.name, message.name);
	ri.userCount = 0;
	ri.filterLength = 0;
	ri.filter = new char[1];
	ri.filter = 0;


	__rooms->registerRoom(new CChatRoom(ri, this));

	client->sendRoomInfo(ri, id);
}

void CChatServer::clientJoinRoom(CChatClient *client, SChatProtoRoomIn message, quint32 id)
{
	CDatabaseResult * res = __db->execute(QString("select name, filter from room where id=%1;").arg(
											  QString::number(message.id)));

	SChatProtoRoomInfo ri;

	ri.id = message.id;
	strcpy(ri.name, res->value(0).toString().toLatin1().constData());

	CChatRoom * room = __rooms->roomById(message.id);
	if (!room)
	{
		mDebug(tr("Room not found in memory. Creating new room..."));
		room = __rooms->registerRoom(id);

		if (!room)
		{
			mDebug("Room creation failed!");
			return;
		}
	}
	room->addUser(client);
	client->__rooms.append(room);

	client->sendRoomInfo(room->info(), id);
}

void CChatServer::clientFindRoom(CChatClient *client, SChatProtoRoomFind message, quint32 id)
{
	QList<CChatRoom *> result = __rooms->find(message);

	mDebug(tr("Found %1 room(s)").arg(
			   QString::number(result.count())));

	foreach (CChatRoom * room, result)
	{
		client->sendRoomInfo(room->info(), id);
	}
}
