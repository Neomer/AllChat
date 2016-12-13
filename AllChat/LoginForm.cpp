#include "LoginForm.h"
#include "ui_LoginForm.h"

LoginForm::LoginForm(CChatProtocol * processor, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoginForm)
{
	ui->setupUi(this);

	__protocol = processor;

	setWindowTitle(tr("Sign In"));

	connect(ui->cmdAccept, SIGNAL(clicked(bool)),
			this, SLOT(sendLogin()));

	connect(ui->cmdExit, SIGNAL(clicked(bool)),
			this, SLOT(reject()));

	connect(processor, SIGNAL(receivePackageUserInfo(quint32, SChatProtoUserInfo)),
			this, SLOT(authResult(quint32, SChatProtoUserInfo)));
}

LoginForm::~LoginForm()
{
	delete ui;
}

void LoginForm::authResult(quint32 id, SChatProtoUserInfo pkg)
{
	qDebug("%d", pkg.id);

	if (pkg.id == 0)
	{
		QMessageBox::critical(this, tr("Authorization error"), tr("Wrong login or password!"));
		return;
	}

	emit userInfoReceived(pkg);

	disconnect(__protocol, SIGNAL(receivePackageUserInfo(quint32, SChatProtoUserInfo)),
			   this, SLOT(authResult(quint32, SChatProtoUserInfo)));

	this->accept();
}

void LoginForm::sendLogin()
{
	SChatProtoAuth pa;

	strcpy(pa.username, ui->txtLogin->text().toLatin1().constData());
	strcpy(pa.password, ui->txtPassword->text().toLatin1().constData());

	__protocol->send(__protocol->createPackage(&pa, sizeof(SChatProtoAuth), PHT_Auth));

}
