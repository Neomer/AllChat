#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QRegExp>
#include <QString>

#include "defines.h"

#define RE_TEST_FAILED		-1

static QRegExp reAuth("^[a-zA-Z_]{4,100}$");
static QRegExp reRoomName("^[\\w\\d .!{}\\[\\]():]{3,100}$");
static QRegExp reRoomFilters("^[\\w\\d, ]{3,100}$");

#endif // GLOBALVARS_H
