# http://doc.qt.io/qt-5/cmake-manual.html
cmake_minimum_required(VERSION 3.9.0)

project(FotoBox)

# MacOS Qt5 Homebrew fix
if(APPLE AND EXISTS /usr/local/opt/qt5)
    # Homebrew installs Qt5 (up to at least 5.9.1) in /usr/local/qt5, ensure
    # it can be found by CMake since it is not in the default /usr/local prefix.
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()

# By default, QT contains core and gui
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
if(Qt5Core_FOUND)
    if(Qt5Core_VERSION VERSION_LESS 5.3.0)
        message(WARNING "Your Qt 5 version '${Qt5Core_VERSION}' is might not be compatible anymore.")
    endif()
endif()

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
add_definitions(-DQT_DEPRECATED_WARNINGS)

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)    # disables all the APIs deprecated before Qt 6.0.0

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Running moc/uic/rcc automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/forms)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(SRC
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/fotobox.cpp
    ${PROJECT_SOURCE_DIR}/src/buzzer.cpp
    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/preferences.cpp
    ${PROJECT_SOURCE_DIR}/src/preferenceprovider.cpp
    )
set(FORMS
    ${PROJECT_SOURCE_DIR}/forms/fotobox.ui
    ${PROJECT_SOURCE_DIR}/forms/preferences.ui
    )
set(TS_FILES
    ${PROJECT_SOURCE_DIR}/i18n/translation_de.ts
    ${PROJECT_SOURCE_DIR}/i18n/translation_en.ts
    )
set(RESOURCES
    ${CMAKE_BINARY_DIR}/qresource.qrc
    )
set(OTHER
    ${PROJECT_SOURCE_DIR}/resources/Info.plist
    ${PROJECT_SOURCE_DIR}/.gitignore
    ${PROJECT_SOURCE_DIR}/.gitmodules
    ${PROJECT_SOURCE_DIR}/.travis.yml
    ${PROJECT_SOURCE_DIR}/DockerfileJessie
    ${PROJECT_SOURCE_DIR}/DockerfileStretch
    ${PROJECT_SOURCE_DIR}/Doxyfile
    ${PROJECT_SOURCE_DIR}/fotobox.pro
    ${PROJECT_SOURCE_DIR}/LICENSE
    ${PROJECT_SOURCE_DIR}/README.md
    )

# Translation: build qm files and copy qrc file from 'resources' folder to build directory
qt5_add_translation(QM_FILES ${TS_FILES})
configure_file(resources/qresource.qrc ${CMAKE_BINARY_DIR} COPYONLY)

add_executable(${CMAKE_PROJECT_NAME}
    ${SRC}
    ${FORMS}
    ${RESOURCES}
    ${QM_FILES}
    ${OTHER}
    )

# Libraries
target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Widgets)
if(UNIX AND NOT APPLE)
    # Speed-Up compiling time with ccache (apt-get install ccache)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
    endif()

    # Raspberry Pi wiringPi framework
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
    find_package(WiringPi)
    if(WiringPi_FOUND)
        target_include_directories(${WiringPi_INCLUDE_DIR})
        target_link_libraries(${CMAKE_PROJECT_NAME} optimized ${WiringPi_LIBRARY_REL})
        target_link_libraries(${CMAKE_PROJECT_NAME} debug     ${WiringPi_LIBRARY_DEB})
    endif()
endif()

# Disable the Dark Mode in the app if macOS SDK is Mojave (10.14) and Qt version is less than 5.12
if(UNIX AND APPLE)
  # macOS SDK version
  execute_process(COMMAND xcrun --sdk macosx --show-sdk-version OUTPUT_VARIABLE MAC_SDK_INFO)
  if(MAC_SDK_INFO STREQUAL "")
    message(FATAL_ERROR "Could not resolve SDK" ${MAC_SDK_INFO})
  endif()
  if(MAC_SDK_INFO VERSION_GREATER 10.13)
    if(Qt5Core_VERSION VERSION_LESS 5.12.0)
      # https://developer.apple.com/documentation/appkit/nsappearancecustomization/choosing_a_specific_appearance_for_your_app
      # use Info.plist with NSRequiresAquaSystemAppearance=true to disable Dark Mode
      set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/resources/Info.plist)
    endif()
  endif()
endif()
