#-------------------------------------------------
#
# Project created by QtCreator 2016-10-13T20:58:51
#
#-------------------------------------------------

QT              += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET          = fotobox
TEMPLATE        = app


SOURCES         += main.cpp \
                fotobox.cpp \
    settings.cpp

HEADERS         += \
                fotobox.h \
    settings.h

FORMS           = mainwindow.ui

#Use C++ 11
QMAKE_CXXFLAGS  += -std=c++11

#Speed-Up compiling time with ccache (apt-get install ccache)
QMAKE_CXX       = ccache g++

#wiringPi LIB
INCLUDEPATH     += /usr/local/include
LIBS            += -lwiringPi -lwiringPiDev

OTHER_FILES += \
    README.md
