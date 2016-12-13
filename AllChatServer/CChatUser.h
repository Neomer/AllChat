#ifndef CCHATUSER_H
#define CCHATUSER_H

#include <QObject>
#include <QSqlQuery>

#include "CErrorProcessor.h"

class CChatUser : public QObject
{
	Q_OBJECT
public:
	explicit CChatUser(QString username, QString password, QSqlDatabase db, QObject *parent = 0);

	bool isValid();

signals:

public slots:

private:
	QSqlQuery __rec;
	QSqlDatabase __db;
};

#endif // CCHATUSER_H
