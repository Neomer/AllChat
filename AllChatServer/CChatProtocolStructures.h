#ifndef CCHATPROTOCOLSTRUCTURES_H
#define CCHATPROTOCOLSTRUCTURES_H

#include <QString>

#include "defines.h"

enum SChatProtoHeaderType
{
	PHT_Auth,
	PHT_Message,
	PHT_RoomCreate,
	PHT_RoomFilters,
	PHT_RoomFind,
	PHT_RoomIcon,
	PHT_RoomIn,
	PHT_RoomInfo,
	PHT_RoomOut,
	PHT_UserInfo,
};

struct SChatProtoHeader
{
	SChatProtoHeaderType type;
	quint32 id;
	quint16 control;
	quint16 length;
};

struct SChatProtoAuth
{
	char username[LENGTH_LOGIN];
	char password[LENGTH_PASSWORD];
};

struct SChatProtoRoomFind
{
	quint16 nameLength;
	quint16 filterLength;
	char * name;
	char * filter;
};

struct SChatRoomIcon
{
};

struct SChatProtoRoomInfo
{
	quint32 id;
	quint16 userCount;
	quint16 filterLength;
	char name[LENGTH_ROOM_NAME];
	char * filter;
};

struct SChatProtoRoomCreate
{
	quint32 id;
	quint32 filterLength;
	quint32 iconLength;
	char name[LENGTH_ROOM_NAME];
	char * filter;
	char * icon_data;
};

struct SChatProtoRoomIn
{
	quint32 id;
};

struct SChatProtoRoomOut
{
	quint32 id;
};

struct SChatProtoMessage
{
	quint32 room_id;
	char writer_name[LENGTH_NICKNAME];
	quint32 writer_id;
	quint16 messageLength;
	char * message;
};

struct SChatProtoUserInfo
{
	quint32 id;
	char username[LENGTH_NICKNAME];
};

#endif // CCHATPROTOCOLSTRUCTURES_H
