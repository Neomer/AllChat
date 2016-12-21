#include "CChatClient.h"
#include "globalVars.h"
#include "defines.h"

CChatClient::CChatClient(QTcpSocket *socket, CDatabase *db) :
	__socket(socket)
{
	__db = db;
	__profile = 0;

	connect(socket, SIGNAL(aboutToClose()),
			this, SLOT(connectionClosed()));

	connect(socket, SIGNAL(disconnected()),
			this, SLOT(connectionClosed()));

	connect(socket, SIGNAL(readyRead()),
			SLOT(dataReceive()));

	__processor = new CChatProtocol(this);
	__processor->setSocket(socket);

	connect(__processor, SIGNAL(receivePackageAuth(quint32, SChatProtoAuth)),
			this, SLOT(packageAuth(quint32, SChatProtoAuth)));

	connect(__processor, SIGNAL(receivePackageRoomCreate(quint32,SChatProtoRoomCreate)),
			this, SLOT(packageCreateRoom(quint32,SChatProtoRoomCreate)));

	connect(__processor, SIGNAL(receivePackageRoomIn(quint32,SChatProtoRoomIn)),
			this, SLOT(packageRoomIn(quint32,SChatProtoRoomIn)));

	connect(__processor, SIGNAL(receivePackageRoomFind(quint32,SChatProtoRoomFind)),
			this, SLOT(packageRoomFind(quint32,SChatProtoRoomFind)));

	connect(__processor, SIGNAL(receivePackageMessage(quint32,SChatProtoMessage)),
			this, SLOT(packageMessage(quint32,SChatProtoMessage)));

	__ui.id = PROTOCOL_AUTHORIZATION_FAILED;

}

void CChatClient::sendMessage(SChatProtoMessage message, quint32 id)
{
	QByteArray tmp((const char *)&message, sizeof(SChatProtoMessage));
	if (message.messageLength > 0)
	{
		tmp.append(message.message, message.messageLength);
	}

	__processor->send(tmp, PHT_Message, id);
}

void CChatClient::sendRoomInfo(SChatProtoRoomInfo message, quint32 id)
{
	mDebug(tr("Send room info:\n\tID: %1\n\tName: %2").arg(
			   QString::number(message.id),
			   message.name));

	QByteArray tmp((const char *)&message, sizeof(SChatProtoRoomInfo));

	mDebug(tr("Buffer %1 Filter: %2").arg(
			   QString::number(tmp.length()),
			   QString::number(message.filterLength)));

	if (message.filterLength > 0)
	{
		tmp.append(message.filter, message.filterLength);
	}

	mDebug(tr("Buffer %1").arg(
			   QString::number(tmp.length())));

	__processor->send(tmp, PHT_RoomInfo, id);
}

quint32 CChatClient::userId()
{
	return __ui.id;
}

QString CChatClient::nickname()
{
	return __ui.username;
}

void CChatClient::connectionClosed()
{
	mDebug(tr("Connection closed!"));

	emit(closingConnection(__socket));
}

void CChatClient::dataReceive()
{
	__processor->appendData(__socket->readAll());
}

void CChatClient::packageAuth(quint32 id, SChatProtoAuth package)
{
	if  (reAuth.indexIn(package.username) == RE_TEST_FAILED)
	{
		mDebug(tr("Wrong username format: [%1]").arg(package.username));
		return;
	}

	if  (reAuth.indexIn(package.password) == RE_TEST_FAILED)
	{
		mDebug(tr("Wrong password format: [%1]").arg(package.password));
		return;
	}

	CDatabaseResult * result =  __db->execute(QString("select id, username from users where username='%1' and password='%2';").arg(
												  QString(package.username),
												  QString(package.password)));

	SChatProtoUserInfo ui;

	if (result->isValid())
	{
		if (result->isEmpty())
		{
			mDebug(tr("User not found"));
			ui.id = PROTOCOL_AUTHORIZATION_FAILED;
		}
		else
		{
			mDebug(tr("User %1[%2] is authorized successfully!").arg(
					   result->value(1).toString(),
					   result->value(0).toString()));


			ui.id = result->value(0).toUInt();
			strcpy(ui.username, result->value(1).toString().toLatin1().constData());
		}
	}
	else
	{
		mDebug(tr("Result set isn't valid! %1").arg(
				   __db->lastError()));

		ui.id = PROTOCOL_AUTHORIZATION_FAILED;
	}

	memcpy(&__ui, &ui, sizeof(SChatProtoUserInfo));

	__socket->write(__processor->createPackage(&ui, sizeof(SChatProtoUserInfo), PHT_UserInfo, id));
}

void CChatClient::packageCreateRoom(quint32 id, SChatProtoRoomCreate package)
{
	if (reRoomName.indexIn(package.name) == RE_TEST_FAILED)
	{
		mDebug(tr("Wrong name format: [%1]").arg(package.name));
		return;
	}

	if (__ui.id == PROTOCOL_AUTHORIZATION_FAILED)
	{
		mDebug(tr("User not logged in!"));
		return;
	}

	emit userCreateRoom(this, package, id);
}

void CChatClient::packageRoomIn(quint32 id, SChatProtoRoomIn package)
{
	emit userJoinRoom(this, package, id);
}

void CChatClient::packageRoomFind(quint32 id, SChatProtoRoomFind package)
{
	mDebug(tr("Emitting userFindRoom(this, package, id)"));
	emit userFindRoom(this, package, id);
}

/**
 * @brief CChatClient::packageMessage Вызывается, когда процессор пакетов принимает пакет с сообщением
 * @param id ID пакета
 * @param package Структура, описывающая присланное сообщение
 */
void CChatClient::packageMessage(quint32 id, SChatProtoMessage package)
{
	if (package.writer_id != userId())
	{
		mWarning(tr("Writer ID do not equals User ID!"));
		return;
	}

	emit userSendMessage(this, package, id);
}
