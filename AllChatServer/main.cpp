#include <QCoreApplication>
#include <QSqlDatabase>
#include <QObject>
#include <QTextCodec>
#include <QStringList>

#include "CChatServer.h"
#include "CDatabase.h"
#include "CErrorProcessor.h"

int main(int argc, char *argv[])
{

	QCoreApplication::addLibraryPath(".");
	QCoreApplication::addLibraryPath("sqldrivers");
	QCoreApplication::addLibraryPath("G:/Projects/build-AllChatServer-Desktop_Qt_5_7_0_MinGW_32bit-Release/release/sqldrivers");

	QCoreApplication a(argc, argv);
	CErrorProcessor::instance(&a)->setLogDetails(4);

	CDatabase db(&a);
	db.setHost("127.0.0.1");
	db.setUser("allchat");
	db.setPassword("123456");

	if (!db.open("AllChat"))
	{
		mWarning(QObject::tr("Database is unavailable!"));
		a.exec();
	}


	CChatServer * srv = new CChatServer(&db);
	srv->start(4547);

	return a.exec();
}
