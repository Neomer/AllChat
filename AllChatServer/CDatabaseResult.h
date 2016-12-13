#ifndef CDATABASERESULT_H
#define CDATABASERESULT_H

#include <QObject>
#include <QVariant>
#include <QByteArray>

#include "CErrorProcessor.h"
#include "includes/pgre/libpq-fe.h"

class CDatabaseResult : public QObject
{
	Q_OBJECT
public:
	explicit CDatabaseResult(PGresult * result, QObject *parent = 0);

	QVariant value(QString name);
	QVariant value(int index);

	int rowCount();
	int columnCount();

	int columnByName(QString name);
	QString columnByIndex(int index);

	bool isBinary(QString name);
	bool isBinary(int index);

	// Размер колонки в базе данных
	int columnSize(QString name);
	int columnSize(int index);

	// Размер данных
	int cellSize(QString name);
	int cellSize(int index);

	void close();

	bool isEoF();
	bool isValid();
	bool isEmpty();
	bool isNull(int index);
	bool isNull(QString name);

	bool next();
	bool previous();
	bool first();
	bool last();

	Oid getID();

signals:

public slots:

private:
	PGresult * __result;
	int __carrier;

};


#endif // CDATABASERESULT_H
