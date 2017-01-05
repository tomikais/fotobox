# Fotobox / Photobooth for Raspberry Pi's
## Requirement
* Raspberry Pi with latest Raspbian
* Qt v4.8.6
* gphoto2 library to have access to camera

# Development
## Project
###  GitLab
`git clone git@gitlab.com:tomikais/fotobox.git`

### DS414 (Synology NAS)
`git clone ssh://"thomas kais"@192.168.0.4/volume1/git/fotobox`

## Qt
You need Qt v4.8.6 on your RasPi. Follow these steps: [Apt-get Qt4 on the Raspberry Pi](https://wiki.qt.io/Apt-get_Qt4_on_the_Raspberry_Pi)

## gphoto2
gphoto2: access to photo camera
https://github.com/gonzalo/gphoto2-updater
CLI Arguments
http://www.gphoto.org/doc/manual/ref-gphoto2-cli.html

## wiringPi
 GPIO access library
http://wiringpi.com/


## Application dependency
Navigate to application and run in terminal
`ldd ./fotobox`

Thomas Kais
