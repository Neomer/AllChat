#include "CErrorProcessor.h"
#include <QDateTime>

CErrorProcessor* CErrorProcessor::__instance = 0;

CErrorProcessor::CErrorProcessor(QObject *parent) :
	QObject(parent)
{

}

void CErrorProcessor::setLogDetails(quint8 level)
{
	this->__details = level;
}

void CErrorProcessor::log(QString title, QString file, int line, QString funcInfo, QString msg)
{
	qDebug(QString("[%5] %6\n\tFUNC: %1\n\tFILE: %3 line %2\n\tMSG: %4").arg(
					funcInfo,
					QString::number(line),
					file,
					msg,
					QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"),
					title).toLatin1().constData());
}

void CErrorProcessor::debug(QString file, int line, QString funcInfo, QString msg)
{
	if (this->__details < 3)
		return;

	log("DEBUG", file, line, funcInfo, msg);
}

void CErrorProcessor::warning(QString file, int line, QString funcInfo, QString msg)
{
	if (this->__details < 2)
		return;

	log("WARNING!", file, line, funcInfo, msg);
}

void CErrorProcessor::critical(QString file, int line, QString funcInfo, QString msg)
{
	log("CRITICAL ERROR!", file, line, funcInfo, msg);
	abort();
}
