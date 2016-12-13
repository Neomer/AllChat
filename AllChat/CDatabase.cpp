#include "CDatabase.h"

CDatabase::CDatabase(QObject *parent) : QObject(parent)
{
	__connection = 0;
	__port = "5432";
}

void CDatabase::setHost(QString host)
{
	__host = host;
}

void CDatabase::setPort(quint16 port)
{
	__port = QString::number(port);
}

void CDatabase::setUser(QString user)
{
	__user = user;
}

void CDatabase::setPassword(QString password)
{
	__password = password;
}

bool CDatabase::open(QString name)
{
	__connection = PQsetdbLogin(__host.toLatin1().constData(),
									 __port.toLatin1().constData(),
									 "",
									 "",
									 name.toLatin1().constData(),
									 __user.toLatin1().constData(),
									 __password.toLatin1().constData());

	if (PQstatus(__connection) != CONNECTION_OK)
	{
		mDebug(tr(PQerrorMessage(__connection)));
	}

	return  PQstatus(__connection) == CONNECTION_OK;
}

void CDatabase::close()
{
	if (__connection)
		PQflush(__connection);
}

QString CDatabase::lastError()
{
	return PQerrorMessage(__connection);
}

CDatabaseResult * CDatabase::execute(QString sql)
{
	if (!__connection)
	{
		mDebug(tr("Connection is not open!"));
		return new CDatabaseResult((PGresult *)0, this);
	}

	if (PQstatus(__connection) != CONNECTION_OK)
	{
		mDebug(tr("Connection lost!"));
		return new CDatabaseResult((PGresult *)0, this);
	}

	PGresult * res = PQexec(__connection, sql.toLatin1().constData());

	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		mDebug(tr("Command execution failed: ").arg(PQerrorMessage(__connection)));
		return new CDatabaseResult((PGresult *)0, this);
	}

	return new CDatabaseResult(res, this);
}
