#ifndef WIDGETFINDROOM_H
#define WIDGETFINDROOM_H


#include <QWidget>

#include "../AllChatServer/CChatProtocol.h"

namespace Ui {
class WidgetFindRoom;
}

class WidgetFindRoom : public QWidget
{
	Q_OBJECT

public:
	explicit WidgetFindRoom(CChatProtocol * processor, QWidget *parent = 0);
	~WidgetFindRoom();

private slots:
	void findResult(quint32 id, SChatProtoRoomInfo msg);
	void findClicked();

private:
	Ui::WidgetFindRoom *ui;
	CChatProtocol * __processor;
	int __row;
};

#endif // WIDGETFINDROOM_H
