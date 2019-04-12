# Photobooth (FotoBox) for Raspberry Pi, Linux and macOS [![Build Status](https://travis-ci.org/tomikais/fotobox.svg?branch=develop)](https://travis-ci.org/tomikais/fotobox)

## installation and operating instructions

Tutorial to run your own "photo booth" on a [Raspberry Pi](https://www.raspberrypi.org):

* [Raspberry Pi model >=2](https://www.raspberrypi.org/products/) with [Raspbian](https://www.raspberrypi.org/downloads/noobs/) installed. To update to latest [Raspbian version](https://en.wikipedia.org/wiki/Raspbian), run this commands in [Terminal](https://www.raspberrypi.org/documentation/usage/terminal/): `sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade`
* Download and install the [Qt](https://www.qt.io) and [WiringPi](http://wiringpi.com) library with [Terminal](https://www.raspberrypi.org/documentation/usage/terminal/) command: `sudo apt-get install qt5-default wiringpi`
* Set up DSLR and/or official [Camera Module V2](https://www.raspberrypi.org/products/camera-module-v2/)
  * [gPhoto2 and libgphoto2 compiler and installer script](http://github.com/gonzalo/gphoto2-updater) to get latest [gPhoto2](http://gphoto.org) (__recommended__) or use latest available gphoto2 version provided from Raspbian: `sudo apt-get install gphoto2`
  * [activate Raspberry Pi Camera](https://www.raspberrypi.org/documentation/usage/camera/)
* Download latest "__FotoBox__" application [here](https://gitlab.com/tomikais/fotobox/tags) according to your [Raspbian version](https://en.wikipedia.org/wiki/Raspbian). Start the application and configure it accordingly.
* Prepare for a continuous solution
  * _Disable the screen saver_: `sudo apt-get install xscreensaver`, run `xhost +localhost` from a local terminal session (not SSH) and reboot the system. After that you can launch the *Screensaver* application and select *disable screen saver* from the drop down.
  * _Autostart_: open file with `sudo nano /etc/xdg/lxsession/LXDE-pi/autostart` add this line `@/home/pi/Downloads/FotoBox` (__if necessary adjust path__) at the end of the file.

## keyboard shortcuts

| key                     | action                               |
| ------------------------|--------------------------------------|
| `Enter`, `PgUp`, `PgDn` | start FotoBox (presenter compatible) |
| `P`, `S`, `E`           | preference dialog                    |
| `Shift` + `Escape`      | quit application                     |

---

## development

[latest Doxygen documentation](https://tomikais.github.io/fotobox/)

### update Raspbian and firmware

Raspbian: `sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade`  
firmware: `sudo rpi-update`

### Qt

`sudo apt-get install qt5-default qttools5-dev-tools qtdeclarative5-dev qtcreator qt5-doc ccache`  
set GCC compiler path in Qt Creator:

* gcc C: `/usr/bin/gcc`
* gcc C++: `/usr/bin/g++`

### wiringPi

`sudo apt-get install wiringpi` (pre-installed Raspbian >=Stretch)

### source code

Prepare git client: `sudo apt-get install gitk git-gui git-doc`  
GitLab source code: `git clone --recursive git@gitlab.com:tomikais/fotobox.git` or `https://gitlab.com/tomikais/fotobox.git`

### gphoto2 and libgphoto2

`chmod +x ./gphoto2-updater/gphoto2-updater.sh && sudo ./gphoto2-updater/gphoto2-updater.sh`
