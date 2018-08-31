# http://doc.qt.io/qt-5/cmake-manual.html
cmake_minimum_required(VERSION 3.9.0)

project(FotoBox)

# By default, QT contains core and gui
find_package(Qt5 COMPONENTS Widgets REQUIRED)
if(Qt5Core_FOUND)
    if(Qt5Core_VERSION VERSION_LESS 5.7.0)
        message(FATAL_ERROR "Your Qt 5 version is outdated (required Qt >=5.7)")
    endif()
endif()

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
file(GLOB SOURCES src/*.cpp)
file(GLOB FORMS forms/*.ui)
file(GLOB RESOURCES
    resources/*.qrc
    resources/*.ts
    )

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCES}
    ${FORMS}
    ${RESOURCES}
    )

target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Widgets)

if(UNIX AND NOT APPLE)
    # Speed-Up compiling time with ccache (apt-get install ccache)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
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
