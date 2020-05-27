#------------------------------------------------------------
# Project created by QtCreator 2016-10-13T20:58:51
#
# Copyright (c) 2016 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'COPYING', which is part of this source code package.
#------------------------------------------------------------

if (lessThan(QT_MAJOR_VERSION, 5)) {
  error("Qt 4 isn't supported (required Qt 5)")
} else {
  # Ubuntu Xenial Qt v5.5.1
  if (equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 5)) {
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

# By default, Qt contains core and gui
QT            += widgets printsupport

CONFIG        += c++11

INCLUDEPATH   += include
HEADERS        = \
                 include/buzzer.h \
                 include/camera.h \
                 include/countdown.h \
                 include/fotobox.h \
                 include/preferenceprovider.h \
                 include/preferences.h

SOURCES        = source/main.cpp \
                 source/buzzer.cpp \
                 source/camera.cpp \
                 source/countdown.cpp \
                 source/fotobox.cpp \
                 source/preferenceprovider.cpp \
                 source/preferences.cpp

FORMS          = forms/fotobox.ui \
                 forms/preferences.ui \
                 forms/commandlineoptions.ui

RESOURCES     += resources/qresource.qrc
TRANSLATIONS  += resources/translation_de.ts \
                 resources/translation_en.ts
# run lrelease to generate the QM files and to embed them in the application resources
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
  qmfile  = $$tsfile
  qmfile ~= s,.ts$,.qm,
  command = $$LRELEASE $$tsfile -qm $$qmfile
  system($$command)|error("Failed to run: $$command")
}

OTHER_FILES    = .gitignore \
                 .travis.yml \
                 CMakeLists.txt \
                 COPYING \
                 README.md \
                 cmake/FindWiringPi.cmake \
                 other/DockerfileStretch \
                 other/DockerfileBuster \
                 other/DockerfileBullseye \
                 other/Doxyfile \
                 other/Info.plist \
                 other/install_dependencies.sh \
                 other/RaspPi_2B_default_GPIO.jpg

linux {
  # Speed-Up compiling time with ccache (apt-get install ccache)
  QMAKE_CXX = ccache $$QMAKE_CXX

  # WORKAROUND: double click on application wasn't working, so disable "Position-independent code" to fix it
  # remove workaround if OS can deal with PIE (not detecting execbutable as shared object "file FotoBox")
  QMAKE_LFLAGS += -no-pie

  # Raspberry Pi wiringPi framework
  contains(QMAKE_HOST.arch, arm.*) {
      LIBS += -lwiringPi
  }
}

# WORKAROUND QTBUG-36714: closing a full screen QMainWindow leaves the screen black on macOS if there are multiple instances of QMainWindow created
mac {
  OBJECTIVE_SOURCES += source/fotoboxmac.mm
  LIBS              += -framework Foundation \
                       -framework AppKit
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
      QMAKE_INFO_PLIST = other/Info.plist
    }
  }
}
