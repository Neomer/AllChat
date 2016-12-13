#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QRegExp>
#include <QPen>
#include <QBrush>

#define RE_TEST_FAILED		-1

static QRegExp reAuth("^[a-zA-Z_]{5,100}$");

static QString sChatDateFormat("hh:mm");
static QPen sChatWriterStyle(QBrush(QColor(188, 0, 0)), 1);
static QPen sChatMyStyle(QBrush(QColor(0, 188, 0)), 1);


#endif // GLOBALVARS_H
