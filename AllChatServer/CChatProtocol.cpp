#include "CChatProtocol.h"
#include "globalVars.h"

CChatProtocol::CChatProtocol(QObject *parent) : QObject(parent)
{
	__id = 0;
	__socket = 0;
}

QByteArray CChatProtocol::createPackageRoomInfo(SChatProtoRoomInfo data)
{
	data;
	return QByteArray();
}

QByteArray CChatProtocol::createPackageUserInfo(SChatProtoUserInfo data)
{
	char ret[sizeof(data) + sizeof(SChatProtoHeader)];

	SChatProtoHeader hdr;

	hdr.id = getID();
	hdr.length = sizeof(data) + sizeof(SChatProtoHeader);
	hdr.control = getControl((const char *) &data, sizeof(data));
	hdr.type = PHT_UserInfo;

	memcpy(ret, &hdr, sizeof(SChatProtoHeader));
	memcpy(ret + sizeof(SChatProtoHeader), &data, sizeof(data));

	return QByteArray(ret, sizeof(data) + sizeof(SChatProtoHeader));
}

void CChatProtocol::setSocket(QTcpSocket *socket)
{
	__socket = socket;
}

void CChatProtocol::send(QByteArray data)
{
	if (!__socket)
	{
		mWarning(tr("Socket not set!"));
		return;
	}

	__socket->write(data);
}

quint32 CChatProtocol::send(void *data, int length, SChatProtoHeaderType type, int id)
{
	if (!__socket)
	{
		mWarning(tr("Socket not set!"));
		return -1;
	}

	char ret[length + sizeof(SChatProtoHeader)];

	SChatProtoHeader hdr;

	if (id == -1)
	{
		hdr.id = getID();
	}
	else
	{
		hdr.id = id;
	}
	hdr.length = length + sizeof(SChatProtoHeader);
	hdr.control = getControl((const char *)data, length);
	hdr.type = type;

	memcpy(ret, &hdr, sizeof(SChatProtoHeader));
	memcpy(ret + sizeof(SChatProtoHeader), data, length);

	__socket->write(QByteArray(ret, length + sizeof(SChatProtoHeader)));

	return hdr.id;

}

quint32 CChatProtocol::send(QByteArray data, SChatProtoHeaderType type, int id)
{
	if (!__socket)
	{
		mWarning(tr("Socket not set!"));
		return -1;
	}

	SChatProtoHeader hdr;

	if (id == -1)
	{
		hdr.id = getID();
	}
	else
	{
		hdr.id = id;
	}
	hdr.length = data.length() + sizeof(SChatProtoHeader);
	hdr.control = getControl((const char *)data, data.length());
	hdr.type = type;

	QByteArray ret((const char *) &hdr, sizeof(SChatProtoHeader));
	ret = ret.append(data);

	mDebug(tr("Send data\n\tID: %1\n\tTYPE: %2\n\tLength: %3").arg(
			   QString::number(hdr.id),
			   QString::number(hdr.type),
			   QString::number(hdr.length)));

	__socket->write(ret);

	return hdr.id;

}

bool CChatProtocol::checkID(QString name, quint32 id)
{
	if (!__ids.keys().contains(name))
	{
		mDebug(tr("Key %1 not found").arg(
				   name));
		return false;
	}

	return __ids[name] == id;
}

bool CChatProtocol::saveID(QString name, quint32 id)
{
	mDebug(tr("ID saved '%1' = %2").arg(
			   name,
			   QString::number(id)));

	__ids[name] = id;
}

QByteArray CChatProtocol::createPackage(void *data, int length, SChatProtoHeaderType type, int id)
{
	char ret[length + sizeof(SChatProtoHeader)];

	SChatProtoHeader hdr;

	if (id == -1)
	{
		hdr.id = getID();
	}
	else
	{
		hdr.id = id;
	}
	hdr.length = length + sizeof(SChatProtoHeader);
	hdr.control = getControl((const char *)data, length);
	hdr.type = type;

	memcpy(ret, &hdr, sizeof(SChatProtoHeader));
	memcpy(ret + sizeof(SChatProtoHeader), data, length);

	return QByteArray(ret,length + sizeof(SChatProtoHeader));
}

QByteArray CChatProtocol::createPackage(QByteArray data, SChatProtoHeaderType type, int id)
{
	SChatProtoHeader hdr;

	if (id == -1)
	{
		hdr.id = getID();
	}
	else
	{
		hdr.id = id;
	}
	hdr.length = data.length() + sizeof(SChatProtoHeader);
	hdr.control = getControl((const char *)data, data.length());
	hdr.type = type;

	QByteArray ret((const char *) &hdr, sizeof(SChatProtoHeader));
	ret = ret.append(data);

	return ret;
}

void CChatProtocol::appendData(QByteArray data)
{
	__buffer.append(data);
	quint32 p = parse();
	__buffer = __buffer.right(__buffer.length() - p);
	mDebug(tr("Buffer size: %2 byte(s)\n\tProcessed: %1 byte(s)").arg(
			   QString::number(p),
			   QString::number(__buffer.length())));
}

quint32 CChatProtocol::getID()
{
	return __id++;
}

quint16 CChatProtocol::getControl(const char *data, int length)
{
	data; length;
	return 0;
}

quint32 CChatProtocol::parse()
{
	quint32 ret = 0;
	while (__buffer.length() > sizeof(SChatProtoHeader))
	{
		SChatProtoHeader hdr;

		memcpy(&hdr, __buffer.constData(), sizeof(hdr));
		mDebug(tr("Received package: %1 length: %2 type:%3").arg(
				   QString::number(hdr.id),
				   QString::number(hdr.length),
				   QString::number(hdr.type)));

		switch (hdr.type)
		{
			case PHT_Auth:
				parseAuth(hdr.id);
				break;

			case PHT_RoomCreate:
				parseRoomCreate(hdr.id);
				break;

			case PHT_RoomInfo:
				parseRoomInfo(hdr.id);
				break;

			case PHT_RoomIn:
				parseRoomIn(hdr.id);
				break;

			case PHT_RoomOut:
				parseRoomOut(hdr.id);
				break;

			case PHT_UserInfo:
				parseUserInfo(hdr.id);
				break;

			case PHT_RoomFind:
				parseRoomFind(hdr.id);
				break;

			case PHT_Message:
				parseMessage(hdr.id);
				break;

		}
		ret += hdr.length;
		__buffer = __buffer.right(__buffer.length() - hdr.length);
	}
	return ret;
}

bool CChatProtocol::parseAuth(quint32 id)
{
	if ((quint32)__buffer.length() < (sizeof(SChatProtoHeader) + sizeof(SChatProtoAuth)))
		return false;

	SChatProtoAuth msg;
	memcpy(&msg, __buffer.constData() + sizeof(SChatProtoHeader), sizeof(SChatProtoAuth));

	emit receivePackageAuth(id, msg);

	return true;
}

bool CChatProtocol::parseRoomCreate(quint32 id)
{
	if ((quint32)__buffer.length() < (sizeof(SChatProtoHeader) + sizeof(SChatProtoRoomCreate)))
		return false;

	SChatProtoRoomCreate msg;
	memcpy(&msg, __buffer.constData() + sizeof(SChatProtoHeader), sizeof(SChatProtoRoomCreate));

	quint32 idx = sizeof(SChatProtoHeader) + sizeof(SChatProtoRoomCreate);

	msg.filter = new char[msg.filterLength + 1];
	msg.filter[msg.filterLength] = 0;
	memcpy(msg.filter, __buffer.constData() + idx, msg.filterLength);

	mDebug(tr("Filters Length: %1 Text: %2").arg(
			   QString::number(msg.filterLength),
			   QString::fromLatin1(msg.filter, msg.filterLength)));


	emit receivePackageRoomCreate(id, msg);

	return true;
}

bool CChatProtocol::parseRoomInfo(quint32 id)
{
	if ((quint32)__buffer.length() < (sizeof(SChatProtoHeader) + sizeof(SChatProtoRoomInfo)))
		return false;

	SChatProtoRoomInfo msg;
	memcpy(&msg, __buffer.constData() + sizeof(SChatProtoHeader), sizeof(SChatProtoRoomInfo));

	emit receivePackageRoomInfo(id, msg);

	return true;
}

bool CChatProtocol::parseRoomIn(quint32 id)
{
	if ((quint32)__buffer.length() < (sizeof(SChatProtoHeader) + sizeof(SChatProtoRoomIn)))
		return false;

	SChatProtoRoomIn msg;
	memcpy(&msg, __buffer.constData() + sizeof(SChatProtoHeader), sizeof(SChatProtoRoomIn));

	emit receivePackageRoomIn(id, msg);

	return true;

}

bool CChatProtocol::parseRoomOut(quint32 id)
{
	if ((quint32)__buffer.length() < (sizeof(SChatProtoHeader) + sizeof(SChatProtoRoomOut)))
		return false;

	SChatProtoRoomOut msg;
	memcpy(&msg, __buffer.constData() + sizeof(SChatProtoHeader), sizeof(SChatProtoRoomOut));

	emit receivePackageRoomOut(id, msg);

	return true;

}

bool CChatProtocol::parseMessage(quint32 id)
{
	if ((quint32)__buffer.length() < (sizeof(SChatProtoHeader) + sizeof(SChatProtoMessage)))
		return false;

	SChatProtoMessage msg;
	memcpy(&msg, __buffer.constData() + sizeof(SChatProtoHeader), sizeof(SChatProtoMessage));

	emit receivePackageMessage(id, msg);

	return true;
}

bool CChatProtocol::parseUserInfo(quint32 id)
{
	if ((quint32)__buffer.length() < (sizeof(SChatProtoHeader) + sizeof(SChatProtoUserInfo)))
		return false;

	SChatProtoUserInfo msg;
	memcpy(&msg, __buffer.constData() + sizeof(SChatProtoHeader), sizeof(SChatProtoUserInfo));

	emit receivePackageUserInfo(id, msg);

	return true;
}

bool CChatProtocol::parseRoomFind(quint32 id)
{
	if ((quint32)__buffer.length() < (sizeof(SChatProtoHeader) + sizeof(SChatProtoRoomFind)))
		return false;

	SChatProtoRoomFind msg;
	memcpy(&msg, __buffer.constData() + sizeof(SChatProtoHeader), sizeof(SChatProtoRoomFind));

	quint32 idx = sizeof(SChatProtoHeader) + sizeof(SChatProtoRoomFind);

	msg.name = new char[msg.nameLength + 1];
	msg.name[msg.nameLength] = 0;
	memcpy(msg.name, __buffer.constData() + idx, msg.nameLength);

	msg.filter = new char[msg.filterLength + 1];
	msg.filter[msg.filterLength] = 0;
	memcpy(msg.filter, __buffer.constData() + idx + msg.nameLength, msg.filterLength);


	emit receivePackageRoomFind(id, msg);

	return true;
}
