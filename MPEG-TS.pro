#-------------------------------------------------
#
# Project created by QtCreator 2013-03-27T00:36:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPEG-TS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    parser.cpp

HEADERS  += mainwindow.h \
    parser.h \
    types.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    OC3.demo.ts