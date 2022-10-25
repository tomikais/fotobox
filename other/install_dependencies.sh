#!/bin/bash
set -Eeuo pipefail

#------------------------------------------------------------
# Copyright (c) 2019 Thomas Kais
#
# This file is subject to the terms and conditions defined in
# file 'COPYING', which is part of this source code package.
#------------------------------------------------------------



if [ "$(whoami)" != "root" ]; then
    echo "Sorry, this script must be executed with 'sudo' or as root user"
    exit 1
fi



echo
echo "-------------------------------------------"
echo "Prepare your operating system for 'FotoBox'"
echo "estimated running time: 5-10 minutes       "
echo "-------------------------------------------"
echo

read -r -p "Start with the installation? [y/N] " response
response=${response,,}    # tolower
if [[ "$response" =~ ^(no|n)$ ]]; then
    exit 0;
fi



echo
echo "----------------------"
echo "Updating dependencies"
echo "----------------------"
echo

apt-get update --quiet
apt-get upgrade --quiet --yes
apt-get dist-upgrade --quiet --yes



echo
echo "-----------------------"
echo "Installing dependencies"
echo "-----------------------"
echo

apt-get install --quiet --yes \
            gphoto2 \
            libgphoto2-6 \
            pigpio \
            qtbase5-dev



echo
echo "--------------------"
echo "Enable pigpio deamon"
echo "--------------------"
echo

systemctl enable pigpiod
systemctl start pigpiod



echo
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "! INSTALLATION SUCCESSFUL !"
echo "!      enjoy FotoBox      !"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo
