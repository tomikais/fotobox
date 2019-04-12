#------------------------------------------------------------
# Copyright (c) 2019 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'COPYING', which is part of this source code package.
#------------------------------------------------------------

# Try to find the wiringPi library.
# This will define:
#
# WiringPi_FOUND       - wiringPi library is available
# WiringPi_INCLUDE_DIR - Where the wiringPi.h header file is
# WiringPi_LIBRARY_REL - The release libraries to link in.
# WiringPi_LIBRARY_DEB - The debug libraries to link in.

find_library(WiringPi_LIBRARY_REL NAMES wiringPi PATHS
    /usr
    /usr/local
    /opt
    )

find_library(WiringPi_LIBRARY_DEB NAMES wiringPiDev PATHS
    /usr
    /usr/local
    /opt
    )

find_path(WiringPi_INCLUDE_DIR wiringPi.h PATHS
    /usr
    /usr/local
    /opt
    PATH_SUFFIXES
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WiringPi DEFAULT_MSG
    WiringPi_LIBRARY_REL
    WiringPi_LIBRARY_DEB
    WiringPi_INCLUDE_DIR
    )

mark_as_advanced(
    WiringPi_LIBRARY_REL
    WiringPi_LIBRARY_DEB
    WiringPi_INCLUDE_DIR
    )
