#include "fotobox.h"

#include <QApplication>


// *********************************************
// gphoto2: access to photo camera
// https://github.com/gonzalo/gphoto2-updater

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
