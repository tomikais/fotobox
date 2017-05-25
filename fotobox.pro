#------------------------------------------------------------
# Project created by QtCreator 2016-10-13T20:58:51
#
# Copyright (c) 2017 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.
#------------------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 5)
QT              += core gui widgets

TARGET          = fotobox
TEMPLATE        = app

SOURCES         += main.cpp \
                fotobox.cpp \
                buzzer.cpp \
                camera.cpp

HEADERS         += \
                fotobox.h \
                buzzer.h \
                camera.h

FORMS           = mainwindow.ui

OTHER_FILES     += \
                README.md \
                LICENSE

#Use C++ 11
QMAKE_CXXFLAGS  += -std=c++11

#Speed-Up compiling time with ccache (apt-get install ccache)
QMAKE_CXX       = ccache g++

#wiringPi LIB for debug/release
debug {
LIBS            += -lwiringPiDev
}
release {
LIBS            += -lwiringPi
}
