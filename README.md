# Photobooth for Raspberry Pi's
Tutorial to built and run your own "photo booth" on a [Raspberry Pi](https://www.raspberrypi.org):
* [Raspberry Pi model >2](https://www.raspberrypi.org/products/) with latest [Raspbian Stretch](https://www.raspberrypi.org/downloads/noobs/) installed. Update to latest version, run this commands in [Terminal](https://www.raspberrypi.org/documentation/usage/terminal/): `sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade`
* Download and install the [Qt](https://www.qt.io) libraries with [Terminal](https://www.raspberrypi.org/documentation/usage/terminal/) command: `sudo apt-get install qt5-default`
* Run [gPhoto2 and libgphoto2 compiler and installer script](http://github.com/gonzalo/gphoto2-updater) to get latest [gPhoto2](http://gphoto.org) or [activate Raspberry Pi Camera](https://www.raspberrypi.org/documentation/usage/camera/) if you want to use the official [Camera Module V2](https://www.raspberrypi.org/products/camera-module-v2/).
* Download latest "fotobox" application [here](https://gitlab.com/tomikais/fotobox/tags). Start the application and configure it accordingly. 
* Prepare for a continuous run
 * Disable the screen saver: `sudo apt-get install xscreensaver`, run `xhost +localhost` from a local terminal session (not SSH) and reboot the system. After that you can launch the *Screensaver* application and select *disable screen saver* from the drop down.
 * Autostart: open file with `sudo nano ~/.config/lxsession/LXDE-pi/autostart` add this line `@sudo /home/pi/Downloads/fotobox` at the end of the file.


---


# Development environment
## update Raspbian and firmware
Raspbian: `sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade`<br>
firmware: `sudo rpi-update`

## Qt
`sudo apt-get install qt5-default qttools5-dev-tools qtdeclarative5-dev qtcreator qt5-doc ccache`<br>
set GCC path in Qt Creator:
* gcc C: `/usr/bin/gcc`
* gcc C++: `/usr/bin/g++`

## gphoto2 and libgphoto2
[gPhoto2 and libgphoto2 compiler and installer script](http://github.com/gonzalo/gphoto2-updater)

## wiringPi (preinstalled >Raspbian Stretch)
[Download and Install](http://wiringpi.com/download-and-install/)

## source code
Prepare git client: `sudo apt-get install gitk git-gui git-doc`<br>
GitLab source code: `git clone git@gitlab.com:tomikais/fotobox.git` or `https://gitlab.com/tomikais/fotobox.git`


## other
### SSH connect
`ssh pi@raspberrypi.local`
### enable OpenGL support
`sudo raspi-config` and select 7 *Advance Options* and A6 *GL Drivers*<br>
You can now choose between:
* **GL (Full KMS) Desktop Drivers**
* GL (Fake KMS) Desktop Driver
* Legacy - Non-GL Driver

### application dependency
Navigate to application directory and run in terminal: `ldd ./fotobox`
