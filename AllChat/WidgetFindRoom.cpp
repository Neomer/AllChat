#include "WidgetFindRoom.h"
#include "ui_WidgetFindRoom.h"

WidgetFindRoom::WidgetFindRoom(CChatProtocol *processor, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetFindRoom)
{
	ui->setupUi(this);

	__processor = processor;

	connect(processor, SIGNAL(receivePackageRoomInfo(quint32,SChatProtoRoomInfo)),
			this, SLOT(findResult(quint32,SChatProtoRoomInfo)));

	connect(ui->cmdFind, SIGNAL(clicked(bool)),
			this, SLOT(findClicked()));

	__row = 0;

}

WidgetFindRoom::~WidgetFindRoom()
{
	delete ui;
}

void WidgetFindRoom::findResult(quint32 id, SChatProtoRoomInfo msg)
{
	if (!__processor->checkID("WidgetFindRoom", id))
	{
		mDebug(tr("Skip"));
		return;
	}

	mDebug(tr("Received room info:\n\tID: %1\n\tName: %2").arg(
			   QString::number(msg.id),
			   QString(msg.name)));

	ui->twResult->setRowCount(__row + 1);
	ui->twResult->setItem(__row, 0, new QTableWidgetItem(QString::number(msg.id)));
	ui->twResult->setItem(__row, 1, new QTableWidgetItem(QString(msg.name)));
	__row++;
}

void WidgetFindRoom::findClicked()
{
	ui->twResult->clearContents();
	ui->twResult->setRowCount(0);

	SChatProtoRoomFind rf;
	rf.filterLength = 0;
	rf.nameLength = ui->txtName->text().length();

	QByteArray tmp((const char *)&rf, sizeof(rf));
	tmp.append(ui->txtName->text().toLatin1());

	__processor->saveID("WidgetFindRoom", __processor->send(tmp, PHT_RoomFind));
}
