#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T09:30:31
#
#-------------------------------------------------

include(numeral-qt.pri)

QT       += testlib

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
