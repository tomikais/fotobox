#------------------------------------------------------------
#
# Project created by QtCreator 2016-10-13T20:58:51
#
# Copyright (c) 2017 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.
#------------------------------------------------------------

QT              += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET          = fotobox
TEMPLATE        = app


SOURCES         += main.cpp \
                fotobox.cpp \
                settings.cpp \
                buzzer.cpp

HEADERS         += \
                fotobox.h \
                settings.h \
                buzzer.h

FORMS           = mainwindow.ui

OTHER_FILES     += \
                README.md \
                LICENSE


#Use C++ 11
QMAKE_CXXFLAGS  += -std=c++11

#Speed-Up compiling time with ccache (apt-get install ccache)
QMAKE_CXX       = ccache g++

#wiringPi LIB
LIBS            += -lwiringPi -lwiringPiDev
