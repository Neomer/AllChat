#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "CDatabase.h"
#include "WidgetRoom.h"
#include "../AllChatServer/CChatProtocol.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(CChatProtocol * processor, QWidget *parent = 0);
	~MainWindow();

public slots:
	void saveUserInfo(SChatProtoUserInfo info);

private slots:
	void enterRoom(quint32 id, SChatProtoRoomInfo pkg);

private:
	Ui::MainWindow *ui;
	CDatabase * __db;
	CChatProtocol * __processor;
	QMap<int, WidgetRoom *> __rooms;
	SChatProtoUserInfo __uinfo;
};

#endif // MAINWINDOW_H
