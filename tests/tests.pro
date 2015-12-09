#-------------------------------------------------
#
# Project created by QtCreator 2015-10-25T22:17:23
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = Test_SmartEntity
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include($$PWD/../SmartEntity/smartentity.pri)

SOURCES += \
    SmartEntityTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    testint.h \
    teststring.h \
    testobject.h \
    testsubclass.h
