/* main.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "preferences.h"
#include "fotobox.h"

#include <QApplication>
#include <QTranslator>


/*!
 * \brief main
 * \param argc argument count
 * \param argv argument vector
 * \return int EXIT CODE
 */
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setOrganizationName("Thomas Kais");
  app.setApplicationName("FotoBox");
  app.setApplicationVersion("1.2.0");

  QTranslator translator;
  if (QLocale::system().language() == QLocale::German) {
      //German
      if (translator.load(":/translations/german")) {
          app.installTranslator(&translator);
        }
      else {
          //English as Default
          if (translator.load(":/translations/english")) {
              app.installTranslator(&translator);
            }
        }
    }

  //Show preferences (modal window)
  Preferences::getInstance().exec();
  if(Preferences::getInstance().result() == QDialog::Rejected) {
      //Quit Application
      return EXIT_FAILURE;
    }

  //Start FotoBox in fullscreen mode
  FotoBox fotobox;

  //gphoto2 installed on the operating system?
#if defined __APPLE__ || defined __linux__
  if (!fotobox.checkGPhoto2()) {
      //gphoto not found -> exit
      return EXIT_FAILURE;
    }
#endif

  fotobox.showFullScreen();

  //Start the Qt EventLoop
  return app.exec();
}

