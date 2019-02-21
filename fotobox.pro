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
  if (equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 3)) {
    warning("Your Qt 5 version '"$$QT_MAJOR_VERSION"."$$QT_MINOR_VERSION"' is might not be compatible anymore.")
  }
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET         = FotoBox
TEMPLATE       = app

# By default, QT contains core and gui
QT            += widgets

CONFIG        += c++11

INCLUDEPATH   += src
HEADERS        = \
                 src/buzzer.h \
                 src/camera.h \
                 src/countdown.h \
                 src/fotobox.h \
                 src/preferenceprovider.h \
                 src/preferences.h

SOURCES        = src/main.cpp \
                 src/buzzer.cpp \
                 src/camera.cpp \
                 src/countdown.cpp \
                 src/fotobox.cpp \
                 src/preferenceprovider.cpp \
                 src/preferences.cpp

FORMS          = forms/fotobox.ui \
                 forms/preferences.ui

RESOURCES     += i18n/qresource.qrc
TRANSLATIONS  += i18n/translation_de.ts \
                 i18n/translation_en.ts
# run lrelease to generate the QM files and to embed them in the application resources
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
  qmfile  = $$tsfile
  qmfile ~= s,.ts$,.qm,
  command = $$LRELEASE $$tsfile -qm $$qmfile
  system($$command)|error("Failed to run: $$command")
}

OTHER_FILES    = resources/Info.plist \
                 .gitignore \
                 .gitmodules \
                 .travis.yml \
                 CMakeLists.txt \
                 DockerfileJessie \
                 DockerfileStretch \
                 Doxyfile \
                 LICENSE \
                 README.md

linux {
  # Speed-Up compiling time with ccache (apt-get install ccache)
  QMAKE_CXX = ccache $$QMAKE_CXX

  # Raspberry Pi wiringPi framework
  contains(QMAKE_HOST.arch, arm.*) {
      LIBS += -lwiringPi
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
