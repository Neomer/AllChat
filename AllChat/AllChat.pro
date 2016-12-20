#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T12:13:55
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AllChat
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    LoginForm.cpp \
    CDatabase.cpp \
    CDatabaseResult.cpp \
    CErrorProcessor.cpp \
    ../AllChatServer/CChatProtocol.cpp \
    CSocket.cpp \
    WidgetCreateRoom.cpp \
    WidgetRoom.cpp \
    WidgetFindRoom.cpp

HEADERS  += MainWindow.h \
    LoginForm.h \
    CDatabase.h \
    CDatabaseResult.h \
    CErrorProcessor.h \
    defines.h \
    globalVars.h \
    ../AllChatServer/CChatProtocol.h \
    ../AllChatServer/CChatProtocolStructures.h \
    CSocket.h \
    WidgetCreateRoom.h \
    WidgetRoom.h \
    WidgetFindRoom.h

FORMS    += MainWindow.ui \
    LoginForm.ui \
    WidgetCreateRoom.ui \
    WidgetRoom.ui \
    WidgetFindRoom.ui

CONFIG(unix): {
    LIBS += -L/usr/lib -lpq
} else {
    LIBS += $$PWD/pgre/libpq.lib
}

RESOURCES += \
    Resources.qrc
