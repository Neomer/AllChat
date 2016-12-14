#include "CChatRoom.h"

CChatRoom::CChatRoom(SChatProtoRoomInfo ri, QObject *parent) : QObject(parent)
{
	__info = ri;
}

void CChatRoom::addUser(CChatClient *user)
{
	// TODO: проверка пользователя по фильтрам

	__users.append(user);

	mDebug(tr("User [%1] %2 added to room [%3] %4").arg(
			   QString::number(user->userId()),
			   user->nickname(),
			   QString::number(this->roomId()),
			   this->name()));
}

void CChatRoom::sendMessage(SChatProtoMessage message, quint32 id)
{
	mDebug(tr("Send message"));

	foreach (CChatClient * client, __users)
	{
		mDebug(tr("Sending message to [%1] %2").arg(
				   QString::number(client->userId()),
				   client->nickname()));

		client->sendMessage(message, id);
	}
}

bool CChatRoom::containsUser(quint32 clientID)
{
	foreach (CChatClient * client, __users)
	{
		if (client->userId() == clientID)
		{
			return true;
		}
	}

	return false;
}
