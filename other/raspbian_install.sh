#!/bin/bash
set -Eeuxo pipefail

#------------------------------------------------------------
# Copyright (c) 2017 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'COPYING', which is part of this source code package.
#------------------------------------------------------------

echo "Prepare system for 'FotoBox'..."
echo "estimated running time: 15-30 minutes"
echo ""
read -r -p "Are you sure? [y/N] " response
response=${response,,}    # tolower
if [[ "$response" =~ ^(yes|y)$ ]]; then
  echo "install WiringPi, Qt and XScreenSaver"
  sudo apt-get update -qq
  sudo apt-get install -qq \
    wiringpi     \
    qt5-default  \
    xscreensaver

  xhost +localhost

  echo "install gphoto2"
  sudo apt-get purge gphoto2 libgphoto2-6
  curl --silent https://raw.githubusercontent.com/gonzalo/gphoto2-updater/master/gphoto2-updater.sh
  chmod +x gphoto2-updater.sh
  sudo ./gphoto2-updater.sh --stable
fi
