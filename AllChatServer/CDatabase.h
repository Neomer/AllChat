#ifndef CDATABASE_H
#define CDATABASE_H

#include <QObject>

#include "includes/pgre/libpq-fe.h"
#include "CDatabaseResult.h"
#include "CErrorProcessor.h"

class CDatabase : public QObject
{
	Q_OBJECT
public:
	explicit CDatabase(QObject *parent = 0);

	void setHost(QString host);
	void setPort(quint16 port);
	void setUser(QString user);
	void setPassword(QString password);

	bool open(QString name);
	void close();

	QString lastError();

	CDatabaseResult *execute(QString sql);

signals:

public slots:

private:
	PGconn * __connection;
	QString __host, __port, __user, __password, __dbname;
};

#endif // CDATABASE_H
