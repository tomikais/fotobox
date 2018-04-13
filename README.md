# Photobooth for Raspberry Pi's
Requirement to start application:
* Raspberry Pi model >2 with latest Raspbian Stretch installed
* Qt 5.7.1 libraries. Terminal: `sudo apt-get install qt5-default`
* [gPhoto2 and libgphoto2 compiler and installer script](http://github.com/gonzalo/gphoto2-updater)
* Disable the screen saver: `sudo apt-get install xscreensaver`, run `xhost +localhost` from a local terminal session (not ssh) and reboot the system. After that you can launch the *Screensaver* application and select *disable screen saver* from the drop down.


---


# Development environment
## update Raspbian and firmware
`sudo apt-get update && sudo apt-get dist-upgrade`

`sudo rpi-update`

## Qt
`sudo apt-get install qt5-default qttools5-dev-tools qtdeclarative5-dev qtcreator qt5-doc`

`sudo apt-get install ccache`

set GCC path in Qt Creator
gcc C: `/usr/bin/gcc`

gcc C++: `/usr/bin/g++`

## gphoto2 and libgphoto2
[gPhoto2 and libgphoto2 compiler and installer script](http://github.com/gonzalo/gphoto2-updater)

## wiringPi (preinstalled >Raspbian Stretch)
[Download and Install](http://wiringpi.com/download-and-install/)

## source code
Prepare git client: `sudo apt-get install gitk git-gui git-doc`

GitLab source code: `git clone git@gitlab.com:tomikais/fotobox.git`


## other stuff
### SSH
`ssh pi@raspberrypi.local`
### enable OpenGL support
`sudo raspi-config` and select 7 *Advance Options* and A6 *GL Drivers*

You can now choose between:
* **GL (Full KMS) Desktop Drivers**
* GL (Fake KMS) Desktop Driver
* Legacy - Non-GL Driver

### application dependency
Navigate to application directory and run in terminal: `ldd ./fotobox`
