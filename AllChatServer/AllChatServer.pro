QT += core network sql
QT -= gui

CONFIG += c++11

TARGET = AllChatServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    CChatClient.cpp \
    CChatServer.cpp \
    CErrorProcessor.cpp \
    CChatUser.cpp \
    CChatProtocol.cpp \
    CChatRoom.cpp \
    CDatabase.cpp \
    CDatabaseResult.cpp \
    CRoomCollection.cpp \
    json/CJson.cpp \
    json/CJsonDoc.cpp \
    json/CJsonElement.cpp \
    json/CJsonStreamParser.cpp

HEADERS += \
    CChatClient.h \
    CChatServer.h \
    CErrorProcessor.h \
    CChatUser.h \
    CChatProtocol.h \
    CChatProtocolStructures.h \
    CChatRoom.h \
    CDatabase.h \
    CDatabaseResult.h \
    globalVars.h \
    defines.h \
    CRoomCollection.h \
    json/CJson.h \
    json/CJsonDoc.h \
    json/CJsonElement.h \
    json/CJsonStreamParser.h

CONFIG(unix): {
    LIBS += -L/usr/lib -lpq
} else {
    LIBS += $$PWD/pgre/libpq.lib
}
