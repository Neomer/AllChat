#include <QCoreApplication>
#include <QSqlDatabase>
#include <QObject>
#include <QTextCodec>
#include <QStringList>

#include "CChatServer.h"
#include "CDatabase.h"
#include "CErrorProcessor.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "json/CJson.h"

int main(int argc, char *argv[])
{

	QCoreApplication::addLibraryPath(".");
	QCoreApplication::addLibraryPath("sqldrivers");
	QCoreApplication::addLibraryPath("G:/Projects/build-AllChatServer-Desktop_Qt_5_7_0_MinGW_32bit-Release/release/sqldrivers");

	QCoreApplication a(argc, argv);

	CJsonDoc doc = CJson::instance(&a).fromFile("json_test.json");
	CJsonElement el = doc.findFirstElementByKey("key1", false);

	if (el.isValid())
		qDebug("Found element value: %s",
			   el.toString().toStdString().c_str());

	qDebug("Found element value: %s",
		   doc["key 2"].toString().toStdString().c_str());


//	QFile f("json_test.json");
//	if (!f.open(QFile::ReadOnly))
//	{
//		return 0;
//	}
//	QJsonParseError jerr;
//	QJsonDocument jdoc = QJsonDocument::fromJson(f.readAll(), &jerr);
//	f.close();
//	if (!jdoc.isNull())
//	{
//		if (jdoc.isArray())
//		{

//		}
//		else
//		{
//			qDebug(jdoc.object()["key1"].toString().toLatin1().constData());
//			f.setFileName(("json_binary.json"));
//			f.open(QFile::WriteOnly);
//			f.write(jdoc.toBinaryData());
//			f.close();
//		}
//	}
//	else
//	{

//		qDebug("[%d] %s",
//			   jerr.offset,
//			   jerr.errorString().toLatin1().constData());
//	}

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
