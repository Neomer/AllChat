#include "CRoomCollection.h"

CRoomCollection::CRoomCollection(CDatabase *db, QObject *parent) : QObject(parent)
{
	__db = db;
}

void CRoomCollection::registerRoom(CChatRoom *room)
{
	if (room)
		__rooms.append(room);
}

CChatRoom *CRoomCollection::registerRoom(quint32 id)
{
	CDatabaseResult * res = __db->execute(QString("select * from room where id=%1;").arg(
											  QString::number(id)));

	if (!res->isValid())
	{
		mDebug("Database error!");
		return 0;
	}

	if (res->isEmpty())
	{
		mDebug(tr("Room ID: %1 mot found!").arg(
				   QString::number(id)));
		return 0;
	}

	SChatProtoRoomInfo ri;

	ri.id = id;
	strcpy(ri.name, res->value("name").toString().toLatin1().constData());
	ri.filterLength = 0;
	ri.userCount = 0;

	CChatRoom * ret = new CChatRoom(ri, this);
	__rooms.append(ret);

	return ret;
}

CChatRoom *CRoomCollection::roomById(quint32 id)
{
	foreach (CChatRoom * room, __rooms)
	{
		if (room->roomId() == id)
		{
			return room;
		}
	}

	return 0;
}

QList<CChatRoom *> CRoomCollection::find(SChatProtoRoomFind params)
{	
	QList<CChatRoom *> ret;
	QString ids;

	mDebug(tr("Try to find rooms by given name %1").arg(
			   params.name));
//	foreach (CChatRoom * room, __rooms)
//	{
//		if (room->name() == params.name)
//		{
//			ret.append(room);
//			ids.append(QString("%1,").arg(QString::number(room->roomId())));
//		}
//	}

//	mDebug(tr("Found %1 room(s) by name in memory. Try to find in the base").arg(
//			   QString::number(ret.count())));



	CDatabaseResult * r = __db->execute(QString("select * from room where name like '%1';").arg(
											params.name,
											ids));

	if (r->isValid())
	{
		while (!r->isEoF())
		{
			SChatProtoRoomInfo ri;

			ri.id = r->value("id").toString().toInt();
			strcpy(ri.name , r->value("name").toString().toLatin1().constData());
			ri.userCount = 0;
			ri.filterLength = r->value("filter").toString().length();
			if (ri.filterLength > 0)
			{
				ri.filter = new char[r->value("filter").toString().length() + 1];
				ri.filter[r->value("filter").toString().length()]= 0;
				strcpy(ri.filter , r->value("filter").toString().toLatin1().constData());
			}

			CChatRoom * room = new CChatRoom(ri, this);
			ret.append(room);

			r->next();
		}
	}
	else
	{
		mWarning(tr("Database error!\n%1").arg(
					 __db->lastError()));
	}

	return ret;
}
