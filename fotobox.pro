#-------------------------------------------------
#
# Project created by QtCreator 2016-10-13T20:58:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fotobox
TEMPLATE = app


SOURCES += main.cpp \
    fotobox.cpp

HEADERS  += \
    fotobox.h

FORMS    += mainwindow.ui

#Use C++ 11
QMAKE_CXXFLAGS += -std=c++11


#wiringPi LIB
INCLUDEPATH   += /usr/local/include
LIBS          += -lwiringPi
