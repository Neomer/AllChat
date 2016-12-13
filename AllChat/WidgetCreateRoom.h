#ifndef WIDGETCREATEROOM_H
#define WIDGETCREATEROOM_H

#include <QWidget>
#include "../AllChatServer/CChatProtocol.h"

namespace Ui {
class WidgetCreateRoom;
}

class WidgetCreateRoom : public QWidget
{
	Q_OBJECT

public:
	explicit WidgetCreateRoom(CChatProtocol * processor, QWidget *parent = 0);
	~WidgetCreateRoom();

private slots:
	void createRoomClicked();
	void roomCreationResult(quint32 id, SChatProtoRoomInfo package);

private:
	Ui::WidgetCreateRoom *ui;
	CChatProtocol * __processor;
};

#endif // WIDGETCREATEROOM_H
