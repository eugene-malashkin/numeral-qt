#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T09:30:31
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += test.cpp \
    numeral.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    numeral.h
