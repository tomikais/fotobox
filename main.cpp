#include "fotobox.h"

#include <QApplication>


// *********************************************

// Qt 4.8.6

// gphoto2: access to photo camera
// https://github.com/gonzalo/gphoto2-updater
// CLI Arguments
// http://www.gphoto.org/doc/manual/ref-gphoto2-cli.html

// wiringPi: GPIO access library
// http://wiringpi.com/
// *********************************************


int main(int argc, char *argv[])
{
  // qApp
  QApplication app(argc, argv);
  FotoBox fotobox;

  //Fullscreen: Qt::WindowMaximized
  fotobox.setWindowState(Qt::WindowFullScreen);
  fotobox.show();

  //Intitalisierung Probleme?
  if(!fotobox.status)
  {
    exit(EXIT_FAILURE);
  }

  //Start EventLoop
  return app.exec();
}
