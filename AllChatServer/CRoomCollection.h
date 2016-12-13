#ifndef CROOMCOLLECTION_H
#define CROOMCOLLECTION_H

#include <QObject>
#include <QList>

#include "CChatRoom.h"
#include "../AllChatServer/CChatProtocolStructures.h"
#include "CDatabase.h"

class CRoomCollection : public QObject
{
	Q_OBJECT

public:
	explicit CRoomCollection(CDatabase * db, QObject *parent = 0);

	void registerRoom(CChatRoom * room);
	CChatRoom * roomById(quint32 id);

	QList<CChatRoom *> find(SChatProtoRoomFind params);

signals:

public slots:

private:
	QList<CChatRoom *> __rooms;
	CDatabase * __db;
};

#endif // CROOMCOLLECTION_H
