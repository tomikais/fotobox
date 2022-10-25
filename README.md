# FotoBox for Raspberry Pi, Linux and macOS [![Travis CI:build status](https://travis-ci.org/tomikais/fotobox.svg?branch=develop)](https://travis-ci.org/tomikais/fotobox) [![GitLab pipeline status](https://gitlab.com/tomikais/fotobox/badges/develop/pipeline.svg)](https://gitlab.com/tomikais/fotobox/-/commits/develop) [![Slack:FotoBox](https://img.shields.io/badge/slack-FotoBox-blue.svg?logo=slack)](https://foto-box.slack.com)

FotoBox is a free open source multi platform application, that offers you the possibility to operate a photo booth (photobooth).

## features

* support a variety of different [DSLR camera models](http://www.gphoto.org/proj/libgphoto2/support.php)
* trigger photos directly or start a countdown by touching, clicking the screen, using soft-/hardware buttons, pressing keyboard shortcuts or using a presenter
* lightning fast and low-memory Linux / macOS customizable application

## Raspberry Pi OS Bullseye installation

Download latest [FotoBox version](https://github.com/tomikais/fotobox/releases) according to your operating system. Extract all files and execute `sudo ./install_dependencies.sh` in a terminal to install all needed dependencies automatically __or__ follow these manual steps:

1. update your operating system: `sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade`
2. install [gPhoto2](http://gphoto.org) when you are using a DSLR camera
    * __beginners:__ use latest available gphoto2 provided from operating system: `sudo apt-get install gphoto2`
    * __advanced (latest version)__: use [gPhoto2 and libgphoto2 compiler and installer script](http://github.com/gonzalo/gphoto2-updater) to get latest version and make sure the default installed has been removed: `sudo apt-get purge gphoto2 libgphoto2-6`

### optional steps

* _Raspberry Pi_: [Setting up](https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up) your [Raspberry Pi Model >=2](https://www.raspberrypi.org/products/) with latest Raspberry Pi OS version (Bullseye). If you are using the official [Camera Module V2](https://www.raspberrypi.org/products/camera-module-v2/) follow the official [activate Raspberry Pi Camera](https://www.raspberrypi.org/documentation/usage/camera/) tutorial.
* _Disable the screen saver_: `sudo apt-get install xscreensaver`, run `xhost +localhost` from a local terminal session (not SSH) and reboot the system. After reboot you can launch the _'Screensaver'_ application and select _'disable screen saver'_ from the drop down.
* _Autostart_: open autostart file with `sudo nano /etc/xdg/lxsession/LXDE-pi/autostart` add this line `@/home/pi/Downloads/FotoBox` (__adjust path if necessary__) at the end of the file.
* _Using a buzzer_: It's possible to connect a hardware buzzer to the Raspberry Pi General Purpose Input Outputs (GPIO) pins to trigger the FotoBox. Please install [pigpio](http://abyz.me.uk/rpi/pigpio/index.html) library: `sudo apt-get install pigpio`  
FotoBox needs to connect to the pigpio deamon. To enable deamon on boot (autostart): `sudo systemctl enable pigpiod && sudo systemctl start pigpiod`  
You can set your pin in the FotoBox application. Please note the [GPIO pin mapping](http://abyz.me.uk/rpi/pigpio/index.html#GPIO).

## macOS installation

1. follow the short instruction to install [Homebrew - The missing package manager for macOS](https://brew.sh/)
2. use Homebrew to install gphoto2. Paste that in a macOS Terminal prompt: `brew install gphoto2`
3. download latest [FotoBox version](https://github.com/tomikais/fotobox/releases)

## keyboard shortcuts

| key                                                              | action            |
|------------------------------------------------------------------|-------------------|
| `N`, `Enter`, `Page Up/Down`, `Arrow Keys`, `Space`, `Backspace` | start FotoBox     |
| `P`, `S`, `E`                                                    | preference dialog |
| `Shift` + `Escape`, `Q`                                          | quit application  |

## Frequently Asked Questions

**Q:** I have misconfigured FotoBox and now it isn't working properly anymore?  
**A:** Start FotoBox application and press "Restore Defaults" button to load the default settings.

**Q:** Is my DSLR camera supported by FotoBox?  
**A:** Visit website [libgphoto2 supported cameras](http://www.gphoto.org/proj/libgphoto2/support.php) to check if your camera model is listed and supports _Image Capture_. Use [gPhoto2 and libgphoto2 compiler and installer script](http://github.com/gonzalo/gphoto2-updater) to get latest version and make sure the OS default one has been removed: `sudo apt-get purge gphoto2 libgphoto2-6`

**Q:** Can I use the FotoBox on Linux without X Window System (e.g. using Linux framebuffer on Raspberry Pi OS Lite)?  
**A:** Yes, that is possible because of [Qt for Embedded Linux](https://doc.qt.io/qt-5/embedded-linux.html). For Example to use Linux framebuffer execute `./FotoBox -platform linuxfb:fb=/dev/fb0` or set environment variable `QT_QPA_PLATFORM=linuxfb:fb=/dev/fb0`

**Q:** It shows me the following error message `qt.qpa.plugin: Could not load the Qt platform plugin "xcb" in "" even though it was found.` (Qt 6 issue)  
**A:** Please install the following packages by executing `sudo apt-get install libxcb-xinerama0 libgl1-mesa-dev libvulkan-dev libxcb-xinput-dev libxcb-xinerama0-dev`

**Q:** My DSLR camera model is supported by libgphoto2 but don't work with FotoBox. How can I fix it?  
**A:** Test if gphoto2 has access to your camera. Execute this command `gphoto2 --capture-image-and-download` in terminal to test it. If the error message '_gphoto2 could not claim the usb device_' appears, try this fix:

> 1. get the C code [here](http://marc.info/?l=linux-usb&m=121459435621262&q=p3)
> 2. save it to a file named `usbreset.c`
> 3. execute `cc usbreset.c -o usbreset` to compile it
> 4. execute `lsusb` to get the Bus/Device ID of your camera, i.e. 'Bus __001__ Device __008__'
> 5. execute `sudo ./usbreset /dev/bus/usb/001/008` each time before running FotoBox

**Q:** Where can I report FotoBox software bugs or suggest new features?  
**A:** [GitLab issue tracker](https://gitlab.com/tomikais/fotobox/issues)

**Q:** Where can I get FotoBox support?  
**A:** [German Raspberry Pi Forum](https://forum-raspberrypi.de/forum/thread/39672-fotobox-projekt-fuer-den-raspberry-pi-c-qt-wiringpi-gphoto2/) or official [Raspberry Pi Forum](https://www.raspberrypi.org/forums/viewtopic.php?t=218279) (english)

---

## development [![Doxygen:documentation](https://img.shields.io/badge/Doxygen-documentation-lightgrey.svg?logo=doxygen)](https://tomikais.github.io/fotobox)

Follow the normal installation instructions and additionally install the development tools according to your operating system. Paste the commands in a terminal prompt.

### Linux (Debian, Ubuntu)

* install Linux development tools: `sudo apt-get install build-essential ccache pigpio`
* install Qt development tools: `sudo apt-get install qtbase5-dev qtcreator`
* install git with tools: `sudo apt-get install git git-doc git-gui gitk`
* _optional tools:_ `sudo apt-get install cmake doxygen doxygen-doc doxygen-gui graphviz`

### macOS

* install macOS development tools: `xcode-select --install`
* install Qt development tools: `brew install qt && brew link --force qt && brew cask install qt-creator`
* _optional tools:_ `brew install cmake && brew install doxygen`

### get source code

GitLab source code: `git clone git@gitlab.com:tomikais/fotobox.git` or `https://gitlab.com/tomikais/fotobox.git`
