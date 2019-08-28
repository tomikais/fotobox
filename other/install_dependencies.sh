#!/bin/bash
set -Eeuo pipefail

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



echo
echo "-------------------------------------------"
echo "Prepare your operating system for 'FotoBox'"
echo "estimated running time: 15-30 minutes"
echo "-------------------------------------------"
echo

read -r -p "Start with the installation? [y/N] " response
response=${response,,}    # tolower
if [[ "$response" =~ ^(no|n)$ ]]; then
    exit 0;
fi



echo
echo "-------------------------"
echo "Creating temporary folder"
echo "-------------------------"
echo

# the temp directory
TMP_DIR="$(mktemp --directory)"
# check if tmp dir was created
if [[ ! "$TMP_DIR" || ! -d "$TMP_DIR" ]]; then
    echo "Could not create temporary directory"
    exit 1
fi
# deletes the temp directory
function cleanup {
    rm --force --recursive "$TMP_DIR"
}
# register the cleanup function to be called on the EXIT signal
trap cleanup EXIT
pushd "$TMP_DIR"



echo
echo "-----------------------"
echo "Installing dependencies"
echo "-----------------------"
echo

apt-get update --quiet
apt-get purge --quiet --yes \
            gphoto2 \
            libgphoto2-6
apt-get install --quiet --yes \
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
popd



echo
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "! INSTALLATION SUCCESSFUL !"
echo "!      enjoy FotoBox      !"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo
