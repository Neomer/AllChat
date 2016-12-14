#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LoginForm.h"

#include "WidgetCreateRoom.h"
#include "WidgetFindRoom.h"
#include "WidgetRoom.h"

#include "defines.h"

MainWindow::MainWindow(CChatProtocol *processor, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	__processor = processor;
	__uinfo.id = PROTOCOL_AUTHORIZATION_FAILED;

	connect(processor, SIGNAL(receivePackageRoomInfo(quint32,SChatProtoRoomInfo)),
			this, SLOT(enterRoom(quint32,SChatProtoRoomInfo)));

	ui->twChats->addTab(new WidgetCreateRoom(processor, this), QIcon(":/images/chpositive.png"), tr("Create Room"));
	ui->twChats->addTab(new WidgetFindRoom(processor, this), QIcon(":/images/chpositive.png"), tr("Find Room"));


}

MainWindow::~MainWindow()
{
	delete ui;
}

MainWindow::saveUserInfo(SChatProtoUserInfo info)
{
	__uinfo = info;
}

void MainWindow::enterRoom(quint32 id, SChatProtoRoomInfo pkg)
{
	if (!__processor->checkID("MainWindow", id))
	{
		mDebug(tr("Skip..."));
		return;
	}

	if (__rooms.keys().contains(pkg.id))
	{
		mDebug(tr("Already created room"));
		return;
	}

	mDebug(tr("Coming room..."));

	WidgetRoom * room = new WidgetRoom(__uinfo, pkg, __processor, this);

	__rooms[pkg.id] = room;
	ui->twChats->addTab(room, QIcon(":/images/team.png"), tr("[%1] %2").arg(
							QString(pkg.userCount),
							QString(pkg.name)));
}
