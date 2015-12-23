#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T14:16:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-mapper-testdevs
TEMPLATE = app

CONFIG += c++11 no_keywords

LIBS += -L/usr/local/lib/ -llo
LIBS += -L/usr/local/lib/ -lmapper-0

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/mapper-0

QMAKE_MAC_SDK = macosx10.11

SOURCES += main.cpp\
        mainwindow.cpp \
    testmapperdevice.cpp \
    mapperdbthread.cpp

HEADERS  += mainwindow.h \
    testmapperdevice.h \
    mapperdbthread.h

FORMS    += mainwindow.ui
