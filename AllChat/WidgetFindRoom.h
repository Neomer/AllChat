#ifndef WIDGETFINDROOM_H
#define WIDGETFINDROOM_H


#include <QWidget>

#include "../AllChatServer/CChatProtocol.h"

#define CELL_ROLE	Qt::UserRole + 1

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
	void roomSelected(int row, int column);

private:
	Ui::WidgetFindRoom *ui;
	CChatProtocol * __processor;
	int __row;
};

#endif // WIDGETFINDROOM_H
