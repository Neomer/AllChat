#ifndef CJSONSTREAMPARSER_H
#define CJSONSTREAMPARSER_H

#include <QObject>
#include <QIODevice>

class CJsonStreamParser : public QObject
{
	Q_OBJECT

public:
	explicit CJsonStreamParser(QIODevice * device, QObject *parent = 0);

signals:

private slots:
	void readData();
};

#endif // CJSONSTREAMPARSER_H
