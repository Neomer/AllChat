#include "CChatUser.h"

CChatUser::CChatUser(QString username, QString password, QSqlDatabase db, QObject *parent) :
	QObject(parent),
	__db(db)
{
	__rec = db.exec(QString("select * from users where username='%1' and password='%2' limit 0, 1;").arg(
						username,
						password));

	if (!__rec.isValid())
	{
		mDebug(tr("User [U: %1 P: %2] not found!").arg(
				   username,
				   password));
	}
}

bool CChatUser::isValid()
{
	return __rec.isValid();
}
