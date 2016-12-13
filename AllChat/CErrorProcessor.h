#ifndef CERRORPROCESSOR_H
#define CERRORPROCESSOR_H

#include <QObject>
#include <QFile>
#include <QMessageBox>

#define mDebug(msg) CErrorProcessor::instance(this)->debug(__FILE__, __LINE__, Q_FUNC_INFO, msg);
#define mWarning(msg) CErrorProcessor::instance()->warning(__FILE__, __LINE__, Q_FUNC_INFO, msg);
#define mCritical(msg) CErrorProcessor::instance()->critical(__FILE__, __LINE__, Q_FUNC_INFO, msg);

class CErrorProcessor : public QObject
{
	Q_OBJECT

public:


	// Создаем Singletone
	static CErrorProcessor * instance(QObject * parent = 0)
	{
		if (!__instance)
			__instance = new CErrorProcessor(parent);

		return __instance;
	}

	void setLogDetails(quint8 level);

	void log(QString title, QString file, int line, QString funcInfo, QString msg);
	void debug(QString file, int line, QString funcInfo, QString msg);
	void warning(QString file, int line, QString funcInfo, QString msg);
	void critical(QString file, int line, QString funcInfo, QString msg);

private:
	CErrorProcessor(QObject * parent = 0);
	static CErrorProcessor * __instance;

	Q_DISABLE_COPY(CErrorProcessor)

	QFile __file;
	quint8 __details;

};

#endif // CERRORPROCESSOR_H
