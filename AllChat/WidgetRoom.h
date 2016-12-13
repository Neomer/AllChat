#ifndef WIDGETROOM_H
#define WIDGETROOM_H

#include <QWidget>

#include "../AllChatServer/CChatProtocol.h"
#include "globalVars.h"

namespace Ui {
class WidgetRoom;
}

class WidgetRoom : public QWidget
{
	Q_OBJECT

public:
	enum MessageType
	{
		// Обычное сообщение
		MessageTypeGeneral = 0,
		// Собственное сообщение
		MessageTypeEcho = 1,
		// Системное сообщение
		MessageTypeSystem = 255,
	};

	explicit WidgetRoom(SChatProtoUserInfo user, SChatProtoRoomInfo info, CChatProtocol * processor, QWidget *parent = 0);
	~WidgetRoom();

public slots:
	void printMessage(QString writer, QString text, MessageType type = MessageTypeGeneral);

private slots:
	void sendMessageClicked();
	void messageReceived(quint32 id, SChatProtoMessage msg);

private:
	Ui::WidgetRoom *ui;
	CChatProtocol * __processor;
	SChatProtoRoomInfo __info;
	SChatProtoUserInfo __uinfo;
};

#endif // WIDGETROOM_H
