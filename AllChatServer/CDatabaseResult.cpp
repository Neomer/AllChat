#include "CDatabaseResult.h"

CDatabaseResult::CDatabaseResult(PGresult * result, QObject *parent) :
	QObject(parent)
{
	__result = result;
	__carrier = 0;
}

QVariant CDatabaseResult::value(QString name)
{
	return value(columnByName(name));
}

QVariant CDatabaseResult::value(int index)
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return QVariant();
	}
	if (!isBinary(index))
		return QVariant::fromValue(QString(PQgetvalue(__result, __carrier, index)));
	else
		return QVariant::fromValue(QByteArray(PQgetvalue(__result, __carrier, index), columnSize(index)));
}

int CDatabaseResult::rowCount()
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return -1;
	}
	return PQntuples(__result);
}

int CDatabaseResult::columnCount()
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return -1;
	}
	return PQnfields(__result);
}

int CDatabaseResult::columnByName(QString name)
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return -1;
	}
	return PQfnumber(__result, name.toLatin1().constData());
}

QString CDatabaseResult::columnByIndex(int index)
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return QString();
	}
	return PQfname(__result, index);
}

void CDatabaseResult::close()
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return;
	}
	PQclear(__result);
}

bool CDatabaseResult::isEoF()
{
	return (__carrier >= rowCount());
}

bool CDatabaseResult::isValid()
{
	if (!__result)
		return false;

	return ((PQresultStatus(__result) != PGRES_BAD_RESPONSE) &&
			(PQresultStatus(__result) != PGRES_NONFATAL_ERROR) &&
				(PQresultStatus(__result) != PGRES_FATAL_ERROR));
}

bool CDatabaseResult::isEmpty()
{
	return rowCount() == 0;
}

bool CDatabaseResult::isNull(int index)
{
	return PQgetisnull(__result, __carrier, index);
}

bool CDatabaseResult::isNull(QString name)
{
	return isNull(columnByName(name));
}

bool CDatabaseResult::next()
{
	__carrier++;
	if (__carrier == rowCount())
		return false;
	return true;
}

bool CDatabaseResult::previous()
{
	if (__carrier == 0)
		return false;
	__carrier--;
	return true;
}

bool CDatabaseResult::first()
{
	if (rowCount() == 0)
		return false;
	__carrier = 0;
}

bool CDatabaseResult::last()
{
	if (rowCount() == 0)
		return false;
	__carrier = rowCount() - 1;
}

Oid CDatabaseResult::getID()
{
	return PQoidValue(__result);
}

bool CDatabaseResult::isBinary(QString name)
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return false;
	}
	return PQfformat(__result, columnByName(name.toLatin1().constData()));
}

bool CDatabaseResult::isBinary(int index)
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return false;
	}
	return PQfformat(__result, index);
}

int CDatabaseResult::columnSize(QString name)
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return -1;
	}
	return PQfsize(__result, columnByName(name.toLatin1().constData()));
}

int CDatabaseResult::columnSize(int index)
{
	if (!isValid())
	{
		mDebug(tr("Empty result set!"));
		return -1;
	}
	return PQfsize(__result, index);
}

int CDatabaseResult::cellSize(QString name)
{
	return cellSize(columnByName(name));
}

int CDatabaseResult::cellSize(int index)
{
	return PQgetlength(__result, __carrier, index);
}
