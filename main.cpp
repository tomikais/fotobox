#include "fotobox.h"

#include <QApplication>

// *********************************************
// # 1
// git clone ssh://"thomas kais"@192.168.0.4/volume1/git/fotobox

// # 2
// Qt 4.8.6
// https://wiki.qt.io/Apt-get_Qt4_on_the_Raspberry_Pi

// # 3
// gphoto2: access to photo camera
// https://github.com/gonzalo/gphoto2-updater
// CLI Arguments
// http://www.gphoto.org/doc/manual/ref-gphoto2-cli.html

// # 4
// wiringPi: GPIO access library
// http://wiringpi.com/
// *********************************************


int main(int argc, char *argv[])
{
    // qApp
    QApplication app(argc, argv);
    app.setOrganizationName("Thomas Kais");
    app.setApplicationName("Fotobox");


    FotoBox fotobox;
    //Fullscreen
    fotobox.setWindowState(Qt::WindowFullScreen);
    fotobox.show();

    //gphoto2 intitalisierung Probleme?
    if(!fotobox.status)
    {
        exit(EXIT_FAILURE);
    }

    //Start EventLoop
    return app.exec();
}
