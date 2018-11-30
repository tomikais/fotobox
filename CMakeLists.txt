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
find_package(Qt5 COMPONENTS Widgets REQUIRED)
if(Qt5Core_FOUND)
    if(Qt5Core_VERSION VERSION_LESS 5.5.0)
        message(WARNING "Your Qt 5 version is might not be compatible anymore.")
    endif()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Running moc/uic/rcc automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/forms)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTORCC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/resources)

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
set(RESOURCES
    ${PROJECT_SOURCE_DIR}/resources/qresource.qrc
    ${PROJECT_SOURCE_DIR}/resources/translation_de.ts
    ${PROJECT_SOURCE_DIR}/resources/translation_en.ts
    )
add_executable(${CMAKE_PROJECT_NAME}
    ${SRC}
    ${FORMS}
    ${RESOURCES}
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
