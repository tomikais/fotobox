#------------------------------------------------------------
# Project created by QtCreator 2016-10-13T20:58:51
#
# Copyright (c) 2018 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.
#------------------------------------------------------------

if (lessThan(QT_MAJOR_VERSION, 5)) {
  error("Qt 4 isn't supported (required Qt 5)")
} else {
  if (equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 5)) {
    warning("Your Qt 5 version: '"$$QT_MAJOR_VERSION"."$$QT_MINOR_VERSION"' is might not be compatible anymore.")
  }
}

TARGET         = FotoBox
TEMPLATE       = app

# By default, QT contains core and gui
QT            += widgets

CONFIG        += c++11
QMAKE_CXXFLAGS+= -std=c++11

INCLUDEPATH   += src
HEADERS        = src/fotobox.h \
                 src/buzzer.h \
                 src/camera.h \
                 src/preferences.h \
                 src/preferenceprovider.h

SOURCES        = src/main.cpp \
                 src/fotobox.cpp \
                 src/buzzer.cpp \
                 src/camera.cpp \
                 src/preferences.cpp \
                 src/preferenceprovider.cpp

FORMS          = forms/fotobox.ui \
                 forms/preferences.ui

RESOURCES      = resources/qresource.qrc
TRANSLATIONS   = resources/translation_en.ts \
                 resources/translation_de.ts

OTHER_FILES    = .gitignore \
                 .gitmodules \
                 .travis.yml \
                 Dockerfile \
                 Doxyfile \
                 LICENSE \
                 README.md

linux {
  # Speed-Up compiling time with ccache (apt-get install ccache)
  QMAKE_CXX = ccache $$QMAKE_CXX

  # Raspberry Pi wiringPi framework
  contains(QMAKE_HOST.arch, arm.*) {
    CONFIG(release, debug|release) {
      LIBS += -lwiringPi
    }
    CONFIG(debug, debug|release) {
      LIBS += -lwiringPiDev
    }
  }
}

# Disable the Dark Mode in the app if macOS SDK is Mojave (10.14) and Qt version is less than 5.12
mac {
  # macOS SDK version
  QMAKE_MAC_SDK_INFO = $$system("xcrun --sdk macosx --show-sdk-version")
  isEmpty(QMAKE_MAC_SDK_INFO): error("Could not resolve SDK \'$$QMAKE_MAC_SDK_INFO\'")
  if (greaterThan(QMAKE_MAC_SDK_INFO, 10.13)) {
    if (equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 12)) {
      # https://developer.apple.com/documentation/appkit/nsappearancecustomization/choosing_a_specific_appearance_for_your_app
      # use Info.plist with NSRequiresAquaSystemAppearance=true to disable Dark Mode
      QMAKE_INFO_PLIST = resources/Info.plist
    }
  }
}
