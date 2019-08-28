#!/bin/bash
set -Eeuxo pipefail

#------------------------------------------------------------
# Copyright (c) 2019 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'COPYING', which is part of this source code package.
#------------------------------------------------------------


if [ "$(whoami)" != "root" ]; then
    echo "Sorry, this script must be executed with 'sudo' or as root"
    exit 1
fi


echo "Prepare your operating system for 'FotoBox'"
echo "estimated running time: 15-30 minutes"
echo ""
read -r -p "Start with the installation? [y/N] " response
response=${response,,}    # tolower
if [[ "$response" =~ ^(no|n)$ ]]; then
    exit 0;
fi


echo
echo "-----------------------"
echo "Installing dependencies"
echo "-----------------------"
echo
apt-get update --quiet
apt-get purge --yes \
            gphoto2 \
            libgphoto2-6
apt-get install --yes \
            wget \
            build-essential \
            git \
            qt5-default

echo
echo "-----------------------------"
echo "Installing libgphoto2/gphoto2"
echo "-----------------------------"
echo
wget --quiet https://raw.githubusercontent.com/gonzalo/gphoto2-updater/master/gphoto2-updater.sh
chmod +x gphoto2-updater.sh
./gphoto2-updater.sh --stable
rm --force gphoto2-updater.sh


echo
echo "---------------------------"
echo "! INSTALLATION SUCCESSFUL !"
echo "!      enjoy FotoBox      !"
echo "---------------------------"
echo
