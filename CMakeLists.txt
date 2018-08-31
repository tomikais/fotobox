# http://doc.qt.io/qt-5/cmake-manual.html
cmake_minimum_required(VERSION 3.9.0)

project(FotoBox)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
if(Qt5Core_FOUND)
    if(Qt5Core_VERSION VERSION_LESS 5.7.0)
        message(FATAL_ERROR "Your Qt 5 version is outdated (required Qt >=5.7)")
    endif()
endif(Qt5Core_FOUND)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Running moc/uic/rcc automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/forms)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTORCC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/resources)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    src/main.cpp
    src/fotobox.cpp
    src/buzzer.cpp
    src/camera.cpp
    src/preferences.cpp
    src/preferenceprovider.cpp
)
set(FORMS
    forms/fotobox.ui
    forms/preferences.ui
)
set(RESOURCES
    resources/qresource.qrc
)


add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCES}
    ${FORMS}
    ${RESOURCES}
)

target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Widgets)

if(LINUX)
    # Speed-Up compiling time with ccache (apt-get install ccache)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    endif()

    # Raspberry Pi wiringPi framework
    find_library(wiringPi_LIB wiringPi)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${wiringPi_LIB})
endif(LINUX)
