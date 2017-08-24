# C++ Photobooth for Raspberry Pi's
Requirement to run:
* Raspberry Pi model >2 with Raspbian Stretch installed
* wiringPi library (preinstalled with Raspberry Stretch)
* Qt 5.7.1 library
* gphoto2 and libgphoto2 library

# Development environment
## Raspbian
### Update Raspbian
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade

### Update Firmware
sudo rpi-update

### Development tools for Raspbian
sudo apt-get install build-essential

## Qt
Qt 5 is needed on your Raspberry Pi. Follow these steps to get everything you need: [ubuntuusers](https://wiki.ubuntuusers.de/Qt/)
sudo apt-get install qt5-default qttools5-dev-tools qtcreator qt5-doc

## gphoto2 and libgphoto2
[gPhoto2](http://www.gphoto.org/proj/gphoto2/) is a command line client to for the libgphoto2. It allows to use gPhoto software from a terminal or from a script shell to perform any camera operation that can be done. This is the main user interface.
[libgphoto2](http://www.gphoto.org/proj/libgphoto2/) is the core library designed to allow access to digital camera by external programs.

[gPhoto2 and libgphoto2 compiler and installer script.](http://github.com/gonzalo/gphoto2-updater)

## wiringPi
[WiringPi](http://wiringpi.com/) is a GPIO access library written in C for the BCM2835 used in the Raspberry Pi. It’s released under the [GNU LGPLv3](http://www.gnu.org/copyleft/lesser.html) license and is usable from C and C++ and many other languages with suitable wrappers.
Download and install: http://wiringpi.com/download-and-install/

## Project files
### GitLab
`git clone git@gitlab.com:tomikais/fotobox.git`

### DS414 (Synology NAS)
`git clone ssh://"thomas kais"@192.168.0.4/volume1/git/fotobox`

# Other
## Application dependency
Navigate to application directory and run in terminal
`ldd ./fotobox`
