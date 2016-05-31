#-------------------------------------------------
#
# Project created by QtCreator 2016-04-30T21:20:17
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clogindlg.cpp \
    cgbdatamanager.cpp \
    clogin.cpp

HEADERS  += mainwindow.h \
    clogindlg.h \
    cgbdatamanager.h \
    clogin.h \
    define.h




FORMS    += mainwindow.ui \
    logindlg.ui
