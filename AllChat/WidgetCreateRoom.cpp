#include "WidgetCreateRoom.h"
#include "ui_WidgetCreateRoom.h"

WidgetCreateRoom::WidgetCreateRoom(CChatProtocol *processor, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetCreateRoom)
{
	ui->setupUi(this);

	__processor = processor;

	connect(__processor, SIGNAL(receivePackageRoomInfo(quint32,SChatProtoRoomInfo)),
			this, SLOT(roomCreationResult(quint32,SChatProtoRoomInfo)));

	connect(ui->cmdCreateRoom, SIGNAL(clicked(bool)),
			this, SLOT(createRoomClicked()));

}

WidgetCreateRoom::~WidgetCreateRoom()
{
	delete ui;
}

void WidgetCreateRoom::createRoomClicked()
{
	SChatProtoRoomCreate rc;
	rc.filterLength = ui->txtFilters->toPlainText().length();
	strcpy(rc.name, ui->txtName->text().toLatin1().constData());

	mDebug(tr("Creating room. Name: %1").arg(
			   rc.name));

	QByteArray tmp((const char *)&rc, sizeof(rc));
	tmp.append(ui->txtFilters->toPlainText().toLatin1());

	__processor->saveID("WidgetCreateRoom", __processor->send(tmp, PHT_RoomCreate));
}

void WidgetCreateRoom::roomCreationResult(quint32 id, SChatProtoRoomInfo package)
{
	if (!__processor->checkID("WidgetCreateRoom", id))
	{
		mDebug(tr("Skip..."));
		return;
	}

	if (package.id != PROTOCOL_AUTHORIZATION_FAILED)
	{
		mDebug(tr("Room creation SUCCESS!"));

		SChatProtoRoomIn ri;
		ri.id = package.id;

		__processor->saveID("MainWindow", __processor->send(&ri, sizeof(SChatProtoRoomIn), PHT_RoomIn));
	}
	else
	{
		mWarning(tr("Room creation FAILED!"));
		return;
	}
}
