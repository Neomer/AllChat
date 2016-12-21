#ifndef CCHATROOM_H
#define CCHATROOM_H

#include <QObject>
#include <QList>

#include "CErrorProcessor.h"
#include "../AllChatServer/CChatProtocolStructures.h"
#include "CChatClient.h"

class CChatRoom : public QObject
{
	Q_OBJECT

public:
	explicit CChatRoom(SChatProtoRoomInfo ri, QObject *parent = 0);

	QStringList filters() { return QString(__info.filter).split(',', QString::SkipEmptyParts);	}
	QString name()	{ return __info.name;	}
	quint32 roomId()	{ return __info.id; }
	SChatProtoRoomInfo info()	{ return __info; }

	void addUser(CChatClient * user);
	void removeUser(CChatClient * user);
	void sendMessage(SChatProtoMessage message, quint32 id = -1);
	bool containsUser(quint32 clientID);

signals:

public slots:

private:
	CChatRoom * __parent;
	SChatProtoRoomInfo __info;
	QList<CChatClient *> __users;
};

#endif // CCHATROOM_H
