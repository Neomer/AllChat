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
	ui->txtMessage->setEnabled(false);

	SChatProtoMessage msg;

	msg.room_id = __info.id;
	msg.writer_id = __uinfo.id;
	strcpy(msg.writer_name, __uinfo.username);
	msg.messageLength = ui->txtMessage->text().length();

	QByteArray tmp((const char *)&msg, sizeof(SChatProtoMessage));
	if (msg.messageLength > 0)
	{
		tmp.append(ui->txtMessage->text().toLatin1().constData(), ui->txtMessage->text().length());
	}

	__processor->send(tmp, PHT_Message);

	ui->txtMessage->setText(QString());
	ui->txtMessage->setEnabled(true);
}

void WidgetRoom::messageReceived(quint32 id, SChatProtoMessage msg)
{
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
