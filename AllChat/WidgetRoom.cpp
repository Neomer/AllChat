#include "WidgetRoom.h"
#include "ui_WidgetRoom.h"
#include <QDateTime>

WidgetRoom::WidgetRoom(SChatProtoUserInfo user, SChatProtoRoomInfo info, CChatProtocol *processor, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetRoom)
{
	ui->setupUi(this);

	__processor = processor;

	__info = info;
	__uinfo = user;

	connect(ui->cmdSend, SIGNAL(clicked(bool)),
			this, SLOT(sendMessageClicked()));

	connect(processor, SIGNAL(receivePackageMessage(quint32,SChatProtoMessage)),
			this, SLOT(messageReceived(quint32,SChatProtoMessage)));

}

WidgetRoom::~WidgetRoom()
{
	delete ui;
}

void WidgetRoom::printMessage(QString writer, QString text, WidgetRoom::MessageType type)
{
	ui->txtChat->setPlainText(ui->txtChat->toPlainText().append(
								  QString("[%1] %2: %3\n").arg(
									  QDateTime::currentDateTime().toString(sChatDateFormat),
									  writer,
									  text)));
}

void WidgetRoom::sendMessageClicked()
{
	ui->txtMessage->setText(QString());
}

void WidgetRoom::messageReceived(quint32 id, SChatProtoMessage msg)
{
	if (!__processor->checkID("WidgetRoom", id))
	{
		mDebug(tr("Skip..."));
		return;
	}

	if (msg.room_id != __info.id)
	{
		mDebug(tr("Skip... (%1)").arg(
				   QString(__info.name)));
		return;
	}

	WidgetRoom::MessageType mtype = MessageTypeGeneral;
	if (msg.writer_id == __uinfo.id)
	{
		mtype = MessageTypeEcho;
	}
	printMessage(msg.writer_name, msg.message, mtype);
}
