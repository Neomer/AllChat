#ifndef CCHATPROTOCOL_H
#define CCHATPROTOCOL_H

#include <QObject>
#include <QByteArray>
#include <QVariant>
#include <QMap>

#include <QTcpSocket>
#include "CErrorProcessor.h"
#include "CChatProtocolStructures.h"


class CChatProtocol : public QObject
{
	Q_OBJECT

public:
	explicit CChatProtocol(QObject *parent = 0);

	QByteArray createPackage(void * data, int length, SChatProtoHeaderType type, int id = -1);
	QByteArray createPackage(QByteArray data, SChatProtoHeaderType type, int id = -1);

	QByteArray createPackageRoomInfo(SChatProtoRoomInfo data);
	QByteArray createPackageUserInfo(SChatProtoUserInfo data);

	void setSocket(QTcpSocket * socket);
	void send(QByteArray data);
	quint32 send(void * data, int length, SChatProtoHeaderType type, int id = -1);
	quint32 send(QByteArray data, SChatProtoHeaderType type, int id = -1);

	bool checkID(QString name, quint32 id);
	bool saveID(QString name, quint32 id);

public slots:
	void appendData(QByteArray data);

signals:
	void receivePackageAuth(quint32 id, SChatProtoAuth package);
	void receivePackageRoomCreate(quint32 id, SChatProtoRoomCreate package);
	void receivePackageRoomIn(quint32 id, SChatProtoRoomIn package);
	void receivePackageRoomOut(quint32 id, SChatProtoRoomOut package);
	void receivePackageRoomInfo(quint32 id, SChatProtoRoomInfo package);
	void receivePackageMessage(quint32 id, SChatProtoMessage package);
	void receivePackageRoomFind(quint32 id, SChatProtoRoomFind package);
	void receivePackageUserInfo(quint32 id, SChatProtoUserInfo package);

private:
	quint32 getID();
	quint16 getControl(const char * data, int length);

	quint32 parse();
	bool checkControl();

	bool parseAuth(quint32 id);
	bool parseRoomCreate(quint32 id);
	bool parseRoomInfo(quint32 id);
	bool parseRoomIn(quint32 id);
	bool parseRoomOut(quint32 id);
	bool parseMessage(quint32 id);
	bool parseUserInfo(quint32 id);
	bool parseRoomFind(quint32 id);


	QByteArray __buffer;
	quint32 __id;
	QTcpSocket * __socket;

	QMap<QString, quint32> __ids;
};

#endif // CCHATPROTOCOL_H
