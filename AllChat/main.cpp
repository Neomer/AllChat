#include "MainWindow.h"
#include <QApplication>

#include "CSocket.h"
#include "LoginForm.h"
#include "CDatabase.h"
#include "../AllChatServer/CChatProtocol.h"
#include "CErrorProcessor.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	CChatProtocol proc(&a);

	CSocket socket(&proc, &a);
	socket.connectToHost("127.0.0.1", 4547);
	if (!socket.waitForConnected(5000))
	{
		QMessageBox::critical(0, QObject::tr("Critical error!"), QObject::tr("Server is unavailable!"));
		return 0;
	}

	proc.setSocket(&socket);

	MainWindow w(&proc);
	w.show();

	LoginForm lf(&proc, &w);

	QObject::connect(&lf, SIGNAL(userInfoReceived(SChatProtoUserInfo)),
					 &w, SLOT(saveUserInfo(SChatProtoUserInfo)));
	if (lf.exec() == QDialog::Rejected)
	{
		a.quit();
		return 0;
	}

	return a.exec();
}
