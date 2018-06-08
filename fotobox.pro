#------------------------------------------------------------
# Project created by QtCreator 2016-10-13T20:58:51
#
# Copyright (c) 2018 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.
#------------------------------------------------------------

contains(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 7)
{
  error("Your Qt version is too old (required >=5.7.0)")
}

QT            += core gui widgets

CONFIG        += c++11

TARGET         = FotoBox
target.path    = /home/pi
INSTALLS      += target

TEMPLATE       = app

SOURCES       += src/main.cpp \
                 src/fotobox.cpp \
                 src/buzzer.cpp \
                 src/camera.cpp \
                 src/preferences.cpp \
                 src/preferenceprovider.cpp

INCLUDEPATH   += src
HEADERS       += src/fotobox.h \
                 src/buzzer.h \
                 src/camera.h \
                 src/preferences.h \
                 src/preferenceprovider.h

FORMS          = forms/fotobox.ui \
                 forms/preferences.ui

RESOURCES     += resources/qresource.qrc
TRANSLATIONS  += resources/translation_en.ts \
                 resources/translation_de.ts

OTHER_FILES   += README.md \
                 LICENSE

# Speed-Up compiling time with ccache (apt-get install ccache)
linux
{
  QMAKE_CXX = ccache g++
}

# wiringPi framework
linux
{
  # DEVICE: Raspberry Pi (wiringPi available)
  contains(QMAKE_HOST.arch, arm.*):
  {
    debug
    {
      LIBS += -lwiringPiDev
    }
    release
    {
      LIBS += -lwiringPi
    }
  }
}
