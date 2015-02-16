#-------------------------------------------------
# Test project for numeral-qt library
#-------------------------------------------------

# Include the library
include(numeral-qt.pri)

QT       += testlib
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += test.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
