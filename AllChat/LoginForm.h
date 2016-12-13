#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>

#include "CDatabase.h"
#include "../AllChatServer/CChatProtocol.h"
#include "CSocket.h"
#include "CErrorProcessor.h"

namespace Ui {
	class LoginForm;
}

class LoginForm : public QDialog
{
	Q_OBJECT

public:
	explicit LoginForm(CChatProtocol * processor, QWidget *parent = 0);
	~LoginForm();

signals:
	void userInfoReceived(SChatProtoUserInfo info);

private slots:
	void authResult(quint32 id, SChatProtoUserInfo pkg);
	void sendLogin();

private:
	Ui::LoginForm *ui;
	CChatProtocol * __protocol;
};

#endif // LOGINFORM_H
